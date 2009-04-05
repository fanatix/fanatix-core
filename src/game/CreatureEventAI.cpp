/*
 * Copyright (C) 2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "Common.h"
#include "CreatureEventAI.h"
#include "CreatureEventAIMgr.h"
#include "ObjectMgr.h"
#include "Spell.h"
#include "World.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GameEventMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "WorldPacket.h"
#include "InstanceData.h"

int CreatureEventAI::Permissible(const Creature *creature)
{
    if( creature->GetCreatureInfo()->AIName == "EventAI" )
        return PERMIT_BASE_SPECIAL;
    return PERMIT_BASE_NO;
}

CreatureEventAI::CreatureEventAI(Creature &c ) : m_creature(c), InCombat(false)
{
    CreatureEventAI_Event_Map::iterator CreatureEvents = CreatureEAI_Mgr.GetCreatureEventAIMap().find(m_creature.GetEntry());
    if (CreatureEvents != CreatureEAI_Mgr.GetCreatureEventAIMap().end())
    {
        std::vector<CreatureEventAI_Event>::iterator i;
        for (i = (*CreatureEvents).second.begin(); i != (*CreatureEvents).second.end(); ++i)
        {

            //Debug check
            #ifndef _DEBUG
            if ((*i).event_flags & EFLAG_DEBUG_ONLY)
                continue;
            #endif
            if( m_creature.GetMap()->IsDungeon() )
            {
                if( (m_creature.GetMap()->IsHeroic() && (*i).event_flags & EFLAG_HEROIC) ||
                    (!m_creature.GetMap()->IsHeroic() && (*i).event_flags & EFLAG_NORMAL))
                {
                    //event flagged for instance mode
                    CreatureEventAIList.push_back(CreatureEventAIHolder(*i));
                }
                continue;
            }
            CreatureEventAIList.push_back(CreatureEventAIHolder(*i));
        }
        //EventMap had events but they were not added because they must be for instance
        if (CreatureEventAIList.empty())
            sLog.outError("CreatureEventAI: CreatureId has events but no events added to list because of instance flags.", m_creature.GetEntry());
    }
    else
        sLog.outError("CreatureEventAI: EventMap for Creature %u is empty but creature is using CreatureEventAI.", m_creature.GetEntry());

    bEmptyList = CreatureEventAIList.empty();
    Phase = 0;
    CombatMovementEnabled = true;
    MeleeEnabled = true;
    AttackDistance = 0;
    AttackAngle = 0.0f;

    //Handle Spawned Events
    if (!bEmptyList)
    {
        for (std::list<CreatureEventAIHolder>::iterator i = CreatureEventAIList.begin(); i != CreatureEventAIList.end(); ++i)
        {
            if ((*i).Event.event_type == EVENT_T_SPAWNED)
                ProcessEvent(*i);
        }
    }
    Reset();
}

bool CreatureEventAI::ProcessEvent(CreatureEventAIHolder& pHolder, Unit* pActionInvoker)
{
    if (!pHolder.Enabled || pHolder.Time)
        return false;

    //Check the inverse phase mask (event doesn't trigger if current phase bit is set in mask)
    if (pHolder.Event.event_inverse_phase_mask & (1 << Phase))
        return false;

    //Store random here so that all random actions match up
    uint32 rnd = rand();

    //Return if chance for event is not met
    if (pHolder.Event.event_chance <= rnd % 100)
        return false;

    union
    {
        uint32 param1;
        int32 param1_s;
    };

    union
    {
        uint32 param2;
        int32 param2_s;
    };

    union
    {
        uint32 param3;
        int32 param3_s;
    };

    union
    {
        uint32 param4;
        int32 param4_s;
    };

    param1 = pHolder.Event.event_param1;
    param2 = pHolder.Event.event_param2;
    param3 = pHolder.Event.event_param3;
    param4 = pHolder.Event.event_param4;

    //Check event conditions based on the event type, also reset events
    switch (pHolder.Event.event_type)
    {
        case EVENT_T_TIMER:
        {
            if (!InCombat)
                return false;

            //Repeat Timers
            if (param3 == param4)
            {
                pHolder.Time = param3;

            }else if (param4 > param3)
            pHolder.Time = urand(param3, param4);
            else
            {
                sLog.outErrorDb("CreatureEventAI: Creature %u using Event %u (Type = %u) has RandomMax < RandomMin. Event repeating disabled.", m_creature.GetEntry(), pHolder.Event.event_id, pHolder.Event.event_type);
                pHolder.Enabled = false;
            }
        }
        break;
        case EVENT_T_TIMER_OOC:
        {
            if (InCombat)
                return false;

            //Repeat Timers
            if (param3 == param4)
            {
                pHolder.Time = param3;

            }else if (param4 > param3)
            pHolder.Time = urand(param3, param4);
            else
            {

                sLog.outErrorDb("CreatureEventAI: Creature %u using Event %u (Type = %u) has RandomMax < RandomMin. Event repeating disabled.", m_creature.GetEntry(), pHolder.Event.event_id, pHolder.Event.event_type);
                pHolder.Enabled = false;
            }
        }
        break;
        case EVENT_T_HP:
        {
            if (!InCombat || !m_creature.GetMaxHealth())
                return false;

            uint32 perc = (m_creature.GetHealth()*100) / m_creature.GetMaxHealth();

            if (perc > param1 || perc < param2)
                return false;

            //Repeat Timers
            if (param3 == param4)
            {
                pHolder.Time = param3;

            }else if (param4 > param3)
            pHolder.Time = urand(param3, param4);
            else
            {

                sLog.outErrorDb("CreatureEventAI: Creature %u using Event %u (Type = %u) has RandomMax < RandomMin. Event repeating disabled.", m_creature.GetEntry(), pHolder.Event.event_id, pHolder.Event.event_type);
                pHolder.Enabled = false;
            }
        }
        break;
        case EVENT_T_MANA:
        {
            if (!InCombat || !m_creature.GetMaxPower(POWER_MANA))
                return false;

            uint32 perc = (m_creature.GetPower(POWER_MANA)*100) / m_creature.GetMaxPower(POWER_MANA);

            if (perc > param1 || perc < param2)
                return false;

            //Repeat Timers
            if (param3 == param4)
            {
                pHolder.Time = param3;

            }else if (param4 > param3)
            pHolder.Time = urand(param3, param4);
            else
            {

                sLog.outErrorDb("CreatureEventAI: Creature %u using Event %u (Type = %u) has RandomMax < RandomMin. Event repeating disabled.", m_creature.GetEntry(), pHolder.Event.event_id, pHolder.Event.event_type);
                pHolder.Enabled = false;
            }
        }
        break;
        case EVENT_T_AGGRO:
        {
        }
        break;
        case EVENT_T_KILL:
        {
            //Repeat Timers
            if (param1 == param2)
            {
                pHolder.Time = param1;

            }else if (param2 > param1)
            pHolder.Time = urand(param1, param2);
            else
            {

                sLog.outErrorDb("CreatureEventAI: Creature %u using Event %u (Type = %u) has RandomMax < RandomMin. Event repeating disabled.", m_creature.GetEntry(), pHolder.Event.event_id, pHolder.Event.event_type);
                pHolder.Enabled = false;
            }
        }
        case EVENT_T_DEATH:
        {
        }
        break;
        case EVENT_T_EVADE:
        {
        }
        break;
        case EVENT_T_SPELLHIT:
        {
            //Spell hit is special case, param1 and param2 handled within CreatureEventAI::SpellHit

            //Repeat Timers
            if (param3 == param4)
            {
                pHolder.Time = param3;

            }else if (param4 > param3)
            pHolder.Time = urand(param3, param4);
            else
            {

                sLog.outErrorDb("CreatureEventAI: Creature %u using Event %u (Type = %u) has RandomMax < RandomMin. Event repeating disabled.", m_creature.GetEntry(), pHolder.Event.event_id, pHolder.Event.event_type);
                pHolder.Enabled = false;
            }
        }
        break;
        case EVENT_T_RANGE:
        {
            //Repeat Timers
            if (param3 == param4)
            {
                pHolder.Time = param3;

            }else if (param4 > param3)
            pHolder.Time = urand(param3, param4);
            else
            {

                sLog.outErrorDb("CreatureEventAI: Creature %u using Event %u (Type = %u) has RandomMax < RandomMin. Event repeating disabled.", m_creature.GetEntry(), pHolder.Event.event_id, pHolder.Event.event_type);
                pHolder.Enabled = false;
            }
        }
        break;
        case EVENT_T_OOC_LOS:
        {
            //Repeat Timers
            if (param3 == param4)
            {
                pHolder.Time = param3;

            }else if (param4 > param3)
            pHolder.Time = urand(param3, param4);
            else
            {

                sLog.outErrorDb("CreatureEventAI: Creature %u using Event %u (Type = %u) has RandomMax < RandomMin. Event repeating disabled.", m_creature.GetEntry(), pHolder.Event.event_id, pHolder.Event.event_type);
                pHolder.Enabled = false;
            }
        }
        break;
        case EVENT_T_SPAWNED:
        {
        }
        break;
        case EVENT_T_TARGET_HP:
        {
            if (!InCombat || !m_creature.getVictim() || !m_creature.getVictim()->GetMaxHealth())
                return false;

            uint32 perc = (m_creature.getVictim()->GetHealth()*100) / m_creature.getVictim()->GetMaxHealth();

            if (perc > param1 || perc < param2)
                return false;

            //Repeat Timers
            if (param3 == param4)
            {
                pHolder.Time = param3;

            }else if (param4 > param3)
            pHolder.Time = urand(param3, param4);
            else
            {

                sLog.outErrorDb("CreatureEventAI: Creature %u using Event %u (Type = %u) has RandomMax < RandomMin. Event repeating disabled.", m_creature.GetEntry(), pHolder.Event.event_id, pHolder.Event.event_type);
                pHolder.Enabled = false;
            }
        }
        break;
        case EVENT_T_TARGET_CASTING:
        {
            if (!InCombat || !m_creature.getVictim() || !m_creature.getVictim()->IsNonMeleeSpellCasted(false, false, true))
                return false;

            //Repeat Timers
            if (param1 == param2)
            {
                pHolder.Time = param1;

            }else if (param2 > param1)
            pHolder.Time = urand(param1, param2);
            else
            {

                sLog.outErrorDb("CreatureEventAI: Creature %u using Event %u (Type = %u) has RandomMax < RandomMin. Event repeating disabled.", m_creature.GetEntry(), pHolder.Event.event_id, pHolder.Event.event_type);
                pHolder.Enabled = false;
            }
        }
        break;
        case EVENT_T_FRIENDLY_HP:
        {
            if (!InCombat)
                return false;

            Unit* pUnit = DoSelectLowestHpFriendly(param2, param1);

            if (!pUnit)
                return false;

            pActionInvoker = pUnit;

            //Repeat Timers
            if (param3 == param4)
            {
                pHolder.Time = param3;

            }else if (param4 > param3)
            pHolder.Time = urand(param3, param4);
            else
            {

                sLog.outErrorDb("CreatureEventAI: Creature %u using Event %u (Type = %u) has RandomMax < RandomMin. Event repeating disabled.", m_creature.GetEntry(), pHolder.Event.event_id, pHolder.Event.event_type);
                pHolder.Enabled = false;
            }
        }
        break;
        case EVENT_T_FRIENDLY_IS_CC:
        {
            if (!InCombat)
                return false;

            std::list<Creature*> pList;
            DoFindFriendlyCC(pList, param2);

            //List is empty
            if (pList.empty())
                return false;

            //We don't really care about the whole list, just return first available
            pActionInvoker = *(pList.begin());

            //Repeat Timers
            if (param3 == param4)
            {
                pHolder.Time = param3;

            }else if (param4 > param3)
            pHolder.Time = urand(param3, param4);
            else
            {
                sLog.outErrorDb("CreatureEventAI: Creature %u using Event %u (Type = %u) has RandomMax < RandomMin. Event repeating disabled.", m_creature.GetEntry(), pHolder.Event.event_id, pHolder.Event.event_type);
                pHolder.Enabled = false;
            }
        }
        break;
        case EVENT_T_FRIENDLY_MISSING_BUFF:
        {
            std::list<Creature*> pList;
            DoFindFriendlyMissingBuff(pList, param2, param1);

            //List is empty
            if (pList.empty())
                return false;

            //We don't really care about the whole list, just return first available
            pActionInvoker = *(pList.begin());

            //Repeat Timers
            if (param3 == param4)
            {
                pHolder.Time = param3;

            }else if (param4 > param3)
            pHolder.Time = urand(param3, param4);
            else
            {

                sLog.outErrorDb("CreatureEventAI: Creature %u using Event %u (Type = %u) has RandomMax < RandomMin. Event repeating disabled.", m_creature.GetEntry(), pHolder.Event.event_id, pHolder.Event.event_type);
                pHolder.Enabled = false;
            }
        }
        break;
        case EVENT_T_SUMMONED_UNIT:
        {
            //Prevent event from occuring on no unit or non creatures
            if (!pActionInvoker || pActionInvoker->GetTypeId()!=TYPEID_UNIT)
                return false;

            //Creature id doesn't match up
            if (param1 && ((Creature*)pActionInvoker)->GetEntry() != param1)
                return false;

            //Repeat Timers
            if (param2 == param3)
            {
                pHolder.Time = param2;

            }else if (param3 > param2)
            pHolder.Time = urand(param2, param3);
            else
            {

                sLog.outErrorDb("CreatureEventAI: Creature %u using Event %u (Type = %u) has RandomMax < RandomMin. Event repeating disabled.", m_creature.GetEntry(), pHolder.Event.event_id, pHolder.Event.event_type);
                pHolder.Enabled = false;
            }
        }
        break;
        case EVENT_T_REACHED_HOME:
        {
        }
        break;
        case EVENT_T_RECEIVE_EMOTE:
        {
        }
        break;
        default:

            sLog.outErrorDb("CreatureEventAI: Creature %u using Event %u has invalid Event Type(%u), missing from ProcessEvent() Switch.", m_creature.GetEntry(), pHolder.Event.event_id, pHolder.Event.event_type);
            break;
    }

    //Disable non-repeatable events
    if (!(pHolder.Event.event_flags & EFLAG_REPEATABLE))
        pHolder.Enabled = false;

    //Process actions
    for (uint32 j = 0; j < MAX_ACTIONS; j++)
        ProcessAction(pHolder.Event.action[j].type, pHolder.Event.action[j].param1, pHolder.Event.action[j].param2, pHolder.Event.action[j].param3, rnd, pHolder.Event.event_id, pActionInvoker);

    return true;
}

void CreatureEventAI::ProcessAction(uint16 type, uint32 param1, uint32 param2, uint32 param3, uint32 rnd, uint32 EventId, Unit* pActionInvoker)
{
    switch (type)
    {
        case ACTION_T_TEXT:
        {
            if (!param1)
                return;

            uint32 temp = 0;

            if (param2 && param3)
            {
                switch( rand()%3 )
                {
                    case 0: temp = param1; break;
                    case 2: temp = param2; break;
                    case 3: temp = param3; break;
                }
            }else if ( param2 && urand(0,1) )
            {
                temp = param2;
            }else
            {
                temp = param1;
            }

            if (temp)
            {
                Unit* target = NULL;
                Unit* owner = NULL;

                if (pActionInvoker)
                {
                    if (pActionInvoker->GetTypeId() == TYPEID_PLAYER)
                        target = pActionInvoker;
                    else if (owner = pActionInvoker->GetOwner())
                    {
                        if (owner->GetTypeId() == TYPEID_PLAYER)
                            target = owner;
                    }
                }
                else if (target = m_creature.getVictim())
                {
                    if (target->GetTypeId() != TYPEID_PLAYER)
                    {
                        if (owner = target->GetOwner())
                        {
                            if (owner->GetTypeId() == TYPEID_PLAYER)
                                target = owner;
                        }
                    }
                }

                DoScriptText(temp, &m_creature, target);
            }
        }
        break;
        case ACTION_T_SET_FACTION:
        {
            if (param1)
                m_creature.setFaction(param1);
            else
            {
                if (CreatureInfo const* ci = GetCreatureTemplateStore(m_creature.GetEntry()))
                {
                    //if no id provided, assume reset and then use default
                    if (m_creature.getFaction() != ci->faction_A)
                        m_creature.setFaction(ci->faction_A);
                }
            }
        }
        break;
        case ACTION_T_MORPH_TO_ENTRY_OR_MODEL:
        {
            if (param1 || param2)
            {
                //set model based on entry from creature_template
                if (param1)
                {
                    if (CreatureInfo const* ci = GetCreatureTemplateStore(param1))
                    {
                        //use default display
                        if (ci->DisplayID_A)
                            m_creature.SetDisplayId(ci->DisplayID_A);
                    }
                }
                //if no param1, then use value from param2 (modelId)
                else
                    m_creature.SetDisplayId(param2);
            }
            else
                m_creature.DeMorph();
        }
        break;
        case ACTION_T_SOUND:
            m_creature.PlayDirectSound(param1);
            break;
        case ACTION_T_EMOTE:
            m_creature.HandleEmoteCommand(param1);
            break;
        case ACTION_T_RANDOM_SOUND:
        {
            uint32 temp = GetRandActionParam(rnd, param1, param2, param3);

            if (temp != uint32(0xffffffff))
                m_creature.PlayDirectSound( temp );
        }
        break;
        case ACTION_T_RANDOM_EMOTE:
        {
            uint32 temp = GetRandActionParam(rnd, param1, param2, param3);

            if (temp != uint32(0xffffffff))
                m_creature.HandleEmoteCommand(temp);
        }
        break;
        case ACTION_T_CAST:
        {
            Unit* target = GetTargetByType(param2, pActionInvoker);
            Unit* caster = &m_creature;

            if (!target)
                return;

            //Cast is always triggered if target is forced to cast on self
            if (param3 & CAST_FORCE_TARGET_SELF)
            {
                param3 |= CAST_TRIGGERED;
                caster = target;
            }

            //Allowed to cast only if not casting (unless we interrupt ourself) or if spell is triggered
            bool canCast = !(caster->IsNonMeleeSpellCasted(false) && (param3 & CAST_TRIGGERED | CAST_INTURRUPT_PREVIOUS));

            // If cast flag CAST_AURA_NOT_PRESENT is active, check if target already has aura on them
            if(param3 & CAST_AURA_NOT_PRESENT)
            {
                for(uint8 i = 0; i < 3; ++i)
                    if(target->HasAura(param1, i))
                        return;
            }

            if (canCast)
            {
                const SpellEntry* tSpell = GetSpellStore()->LookupEntry(param1);

                //Verify that spell exists
                if (tSpell)
                {
                    //Check if cannot cast spell
                    if (!(param3 & (CAST_FORCE_TARGET_SELF | CAST_FORCE_CAST)) &&
                        !CanCast(target, tSpell, (param3 & CAST_TRIGGERED)))
                    {
                        //Melee current victim if flag not set
                        if (!(param3 & CAST_NO_MELEE_IF_OOM))
                        {
                            if (m_creature.GetMotionMaster()->GetCurrentMovementGeneratorType() == TARGETED_MOTION_TYPE)
                            {
                                AttackDistance = 0;
                                AttackAngle = 0;

                                m_creature.GetMotionMaster()->Clear(false);
                                m_creature.GetMotionMaster()->MoveChase(m_creature.getVictim(), AttackDistance, AttackAngle);
                            }
                        }

                    }
                    else
                    {
                        //Interrupt any previous spell
                        if (caster->IsNonMeleeSpellCasted(false) && param3 & CAST_INTURRUPT_PREVIOUS)
                            caster->InterruptNonMeleeSpells(false);

                        caster->CastSpell(target, param1, (param3 & CAST_TRIGGERED));
                    }

                }else
                sLog.outErrorDb("CreatureEventAI: event %d creature %d attempt to cast spell that doesn't exist %d", EventId, m_creature.GetEntry(), param1);
            }
        }
        break;
        case ACTION_T_SUMMON:
        {
            Unit* target = GetTargetByType(param2, pActionInvoker);

            Creature* pCreature = NULL;

            if (param3)
                pCreature = m_creature.SummonCreature(param1, 0, 0, 0, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, param3);
            else
                pCreature = m_creature.SummonCreature(param1, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 0);

            if (!pCreature)
            {

                sLog.outErrorDb( "CreatureEventAI: failed to spawn creature %u. Spawn event %d is on creature %d", param1, EventId, m_creature.GetEntry());
            }
            else if (param2 != TARGET_T_SELF && target)
                pCreature->AI()->AttackStart(target);
        }
        break;
        case ACTION_T_THREAT_SINGLE_PCT:
        {
            Unit* target = GetTargetByType(param2, pActionInvoker);

            if (target)
                m_creature.getThreatManager().modifyThreatPercent(target, param1);
        }
        break;
        case ACTION_T_THREAT_ALL_PCT:
        {
            Unit* Temp = NULL;

            std::list<HostilReference*>::iterator i = m_creature.getThreatManager().getThreatList().begin();
            for (; i != m_creature.getThreatManager().getThreatList().end(); ++i)
            {
                Temp = Unit::GetUnit(m_creature,(*i)->getUnitGuid());
                if (Temp)
                    m_creature.getThreatManager().modifyThreatPercent(Temp, param1);
            }
        }
        break;
        case ACTION_T_QUEST_EVENT:
        {
            Unit* target = GetTargetByType(param2, pActionInvoker);

            if (target && target->GetTypeId() == TYPEID_PLAYER)
                ((Player*)target)->AreaExploredOrEventHappens(param1);
        }
        break;
        case ACTION_T_CASTCREATUREGO:
        {
            Unit* target = GetTargetByType(param3, pActionInvoker);

            if (target && target->GetTypeId() == TYPEID_PLAYER)
                ((Player*)target)->CastedCreatureOrGO(param1, m_creature.GetGUID(), param2);
        }
        break;
        case ACTION_T_SET_UNIT_FIELD:
        {
            Unit* target = GetTargetByType(param3, pActionInvoker);

            if (param1 < OBJECT_END || param1 >= UNIT_END)
                return;

            if (target)
                target->SetUInt32Value(param1, param2);
        }
        break;
        case ACTION_T_SET_UNIT_FLAG:
        {
            Unit* target = GetTargetByType(param2, pActionInvoker);

            if (target)
                target->SetFlag(UNIT_FIELD_FLAGS, param1);
        }
        break;
        case ACTION_T_REMOVE_UNIT_FLAG:
        {
            Unit* target = GetTargetByType(param2, pActionInvoker);

            if (target)
                target->RemoveFlag(UNIT_FIELD_FLAGS, param1);
        }
        break;
        case ACTION_T_AUTO_ATTACK:
        {
            if (param1)
                MeleeEnabled = true;
            else MeleeEnabled = false;
        }
        break;
        case ACTION_T_COMBAT_MOVEMENT:
        {
            CombatMovementEnabled = param1;

            //Allow movement (create new targeted movement gen only if idle)
            if (CombatMovementEnabled)
            {
                if (m_creature.GetMotionMaster()->GetCurrentMovementGeneratorType() == IDLE_MOTION_TYPE)
                {
                    m_creature.GetMotionMaster()->Clear(false);
                    m_creature.GetMotionMaster()->MoveChase(m_creature.getVictim(), AttackDistance, AttackAngle);
                }
            }
            else
            if (m_creature.GetMotionMaster()->GetCurrentMovementGeneratorType() == TARGETED_MOTION_TYPE)
            {
                m_creature.GetMotionMaster()->Clear(false);
                m_creature.GetMotionMaster()->MoveIdle();
                m_creature.StopMoving();
            }
        }
        break;
        case ACTION_T_SET_PHASE:
        {
            Phase = param1;
        }
        break;
        case ACTION_T_INC_PHASE:
        {
            Phase += param1;

            if (Phase > 31)

                sLog.outErrorDb( "CreatureEventAI: Event %d incremented Phase above 31. Phase mask cannot be used with phases past 31. CreatureEntry = %d", EventId, m_creature.GetEntry());
        }
        break;
        case ACTION_T_EVADE:
        {
            EnterEvadeMode();
        }
        break;
        case ACTION_T_FLEE:
        {
            //TODO: Replace with Flee movement generator
            m_creature.CastSpell(&m_creature, SPELL_RUN_AWAY, true);
        }
        break;
        case ACTION_T_QUEST_EVENT_ALL:
        {
            Unit* Temp = NULL;
            if( pActionInvoker && pActionInvoker->GetTypeId() == TYPEID_PLAYER )
            {
                Temp = Unit::GetUnit(m_creature,pActionInvoker->GetGUID());
                if( Temp )
                    ((Player*)Temp)->GroupEventHappens(param1,&m_creature);
            }
        }
        break;
        case ACTION_T_CASTCREATUREGO_ALL:
        {
            Unit* Temp = NULL;

            std::list<HostilReference*>::iterator i = m_creature.getThreatManager().getThreatList().begin();
            for (; i != m_creature.getThreatManager().getThreatList().end(); ++i)
            {
                Temp = Unit::GetUnit(m_creature,(*i)->getUnitGuid());
                if (Temp && Temp->GetTypeId() == TYPEID_PLAYER)
                    ((Player*)Temp)->CastedCreatureOrGO(param1, m_creature.GetGUID(), param2);
            }
        }
        break;
        case ACTION_T_REMOVEAURASFROMSPELL:
        {
            Unit* target = GetTargetByType(param1, pActionInvoker);

            if (target)
                target->RemoveAurasDueToSpell(param2);
        }
        break;
        case ACTION_T_RANGED_MOVEMENT:
        {
            AttackDistance = param1;
            AttackAngle = ((float)param2/180)*M_PI;

            if (CombatMovementEnabled)
            {
                if (m_creature.GetMotionMaster()->GetCurrentMovementGeneratorType() == TARGETED_MOTION_TYPE)
                {
                    //Drop current movement gen
                    m_creature.GetMotionMaster()->Clear(false);
                    m_creature.GetMotionMaster()->MoveChase(m_creature.getVictim(), AttackDistance, AttackAngle);
                }
            }
        }
        break;
        case ACTION_T_RANDOM_PHASE:
        {
            uint32 temp = GetRandActionParam(rnd, param1, param2, param3);

            Phase = temp;
        }
        break;
        case ACTION_T_RANDOM_PHASE_RANGE:
        {
            if (param2 > param1)
            {
                Phase = param1 + (rnd % (param2 - param1));
            }
            else
                sLog.outErrorDb( "CreatureEventAI: ACTION_T_RANDOM_PHASE_RANGE cannot have Param2 <= Param1. Divide by Zero. Event = %d. CreatureEntry = %d", EventId, m_creature.GetEntry());
        }
        break;
        case ACTION_T_SUMMON_ID:
        {
            Unit* target = GetTargetByType(param2, pActionInvoker);

            //Duration
            Creature* pCreature = NULL;

            CreatureEventAI_Summon_Map::const_iterator i = CreatureEAI_Mgr.GetCreatureEventAISummonMap().find(param3);
            if (i == CreatureEAI_Mgr.GetCreatureEventAISummonMap().end())
            {

                sLog.outErrorDb( "CreatureEventAI: failed to spawn creature %u. Summon map index %u does not exist. EventID %d. CreatureID %d", param1, param3, EventId, m_creature.GetEntry());
                return;
            }

            if ((*i).second.SpawnTimeSecs)
                pCreature = m_creature.SummonCreature(param1, (*i).second.position_x, (*i).second.position_y, (*i).second.position_z, (*i).second.orientation, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, (*i).second.SpawnTimeSecs);
            else pCreature = m_creature.SummonCreature(param1, (*i).second.position_x, (*i).second.position_y, (*i).second.position_z, (*i).second.orientation, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 0);

            if (!pCreature)
            {

                sLog.outErrorDb( "CreatureEventAI: failed to spawn creature %u. EventId %d.Creature %d", param1, EventId, m_creature.GetEntry());
            }
            else if (param2 != TARGET_T_SELF && target)
                pCreature->AI()->AttackStart(target);
        }
        break;
        case ACTION_T_KILLED_MONSTER:
        {
            //first attempt player who tapped creature
            if (Player* pPlayer = m_creature.GetLootRecipient())
                pPlayer->RewardPlayerAndGroupAtEvent(param1, &m_creature);
            else
            {
                //if not available, use pActionInvoker
                Unit* pTarget = GetTargetByType(param2, pActionInvoker);

                if (Player* pPlayer = pTarget->GetCharmerOrOwnerPlayerOrPlayerItself())
                    pPlayer->RewardPlayerAndGroupAtEvent(param1, &m_creature);
            }
        }
        break;
        case ACTION_T_SET_INST_DATA:
        {
            InstanceData* pInst = (InstanceData*)m_creature.GetInstanceData();
            if (!pInst)
            {
                sLog.outErrorDb("CreatureEventAI: Event %d attempt to set instance data without instance script. Creature %d", EventId, m_creature.GetEntry());
                return;
            }

            pInst->SetData(param1, param2);
        }
        break;
        case ACTION_T_SET_INST_DATA64:
        {
            Unit* target = GetTargetByType(param2, pActionInvoker);
            if (!target)
            {
                sLog.outErrorDb("CreatureEventAI: Event %d attempt to set instance data64 but Target == NULL. Creature %d", EventId, m_creature.GetEntry());
                return;
            }

            InstanceData* pInst = (InstanceData*)m_creature.GetInstanceData();
            if (!pInst)
            {
                sLog.outErrorDb("CreatureEventAI: Event %d attempt to set instance data64 without instance script. Creature %d", EventId, m_creature.GetEntry());
                return;
            }

            pInst->SetData64(param1, target->GetGUID());
        }
        break;
        case ACTION_T_UPDATE_TEMPLATE:
        {
            if (m_creature.GetEntry() == param1)
            {

                sLog.outErrorDb("CreatureEventAI: Event %d ACTION_T_UPDATE_TEMPLATE call with param1 == current entry. Creature %d", EventId, m_creature.GetEntry());
                return;
            }

            m_creature.UpdateEntry(param1, param2 ? HORDE : ALLIANCE);
        }
        break;
        case ACTION_T_DIE:
        {
            if (m_creature.isDead())
            {

                sLog.outErrorDb("CreatureEventAI: Event %d ACTION_T_DIE on dead creature. Creature %d", EventId, m_creature.GetEntry());
                return;
            }
            m_creature.DealDamage(&m_creature, m_creature.GetMaxHealth(),NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
        break;
        case ACTION_T_ZONE_COMBAT_PULSE:
        {
            if (!m_creature.isInCombat() || !m_creature.GetMap()->IsDungeon())
            {

                sLog.outErrorDb("CreatureEventAI: Event %d ACTION_T_ZONE_COMBAT_PULSE on creature out of combat or in non-dungeon map. Creature %d", EventId, m_creature.GetEntry());
                return;
            }

            DoZoneInCombat(&m_creature);
        }
        break;
    }
}

void CreatureEventAI::JustRespawned()
{
    InCombat = false;
    Reset();

    if (bEmptyList)
        return;

    //Handle Spawned Events
    for (std::list<CreatureEventAIHolder>::iterator i = CreatureEventAIList.begin(); i != CreatureEventAIList.end(); ++i)
    {
        if ((*i).Event.event_type == EVENT_T_SPAWNED)
            ProcessEvent(*i);
    }
}

void CreatureEventAI::Reset()
{
    EventUpdateTime = EVENT_UPDATE_TIME;
    EventDiff = 0;

    if (bEmptyList)
        return;

    //Reset all events to enabled
    for (std::list<CreatureEventAIHolder>::iterator i = CreatureEventAIList.begin(); i != CreatureEventAIList.end(); ++i)
    {
        switch ((*i).Event.event_type)
        {
            //Reset all out of combat timers
            case EVENT_T_TIMER_OOC:
            {
                if ((*i).Event.event_param2 == (*i).Event.event_param1)
                {
                    (*i).Time = (*i).Event.event_param1;
                    (*i).Enabled = true;
                }
                else if ((*i).Event.event_param2 > (*i).Event.event_param1)
                {
                    (*i).Time = urand((*i).Event.event_param1, (*i).Event.event_param2);
                    (*i).Enabled = true;
                }
                else
                    sLog.outErrorDb("CreatureEventAI: Creature %u using Event %u (Type = %u) has InitialMax < InitialMin. Event disabled.", m_creature.GetEntry(), (*i).Event.event_id, (*i).Event.event_type);
            }
            break;
            //default:
            //TODO: enable below code line / verify this is correct to enable events previously disabled (ex. aggro yell), instead of enable this in void Aggro()
            //(*i).Enabled = true;
            //(*i).Time = 0;
            //break;
        }
    }
}

void CreatureEventAI::JustReachedHome()
{
    m_creature.LoadCreaturesAddon();

    if (!bEmptyList)
    {
        for (std::list<CreatureEventAIHolder>::iterator i = CreatureEventAIList.begin(); i != CreatureEventAIList.end(); ++i)
        {
            if ((*i).Event.event_type == EVENT_T_REACHED_HOME)
                ProcessEvent(*i);
        }
    }

    Reset();
}

void CreatureEventAI::EnterEvadeMode()
{
    m_creature.InterruptNonMeleeSpells(true);
    m_creature.RemoveAllAuras();
    m_creature.DeleteThreatList();
    m_creature.CombatStop();

    if (m_creature.isAlive())
        m_creature.GetMotionMaster()->MoveTargetedHome();

    m_creature.SetLootRecipient(NULL);

    InCombat = false;

    if (bEmptyList)
        return;

    //Handle Evade events
    for (std::list<CreatureEventAIHolder>::iterator i = CreatureEventAIList.begin(); i != CreatureEventAIList.end(); ++i)
    {
        if ((*i).Event.event_type == EVENT_T_EVADE)
            ProcessEvent(*i);
    }
}

void CreatureEventAI::JustDied(Unit* killer)
{
    InCombat = false;
    Reset();

    if (bEmptyList)
        return;

    //Handle Evade events
    for (std::list<CreatureEventAIHolder>::iterator i = CreatureEventAIList.begin(); i != CreatureEventAIList.end(); ++i)
    {
        if ((*i).Event.event_type == EVENT_T_DEATH)
            ProcessEvent(*i, killer);
    }
}

void CreatureEventAI::KilledUnit(Unit* victim)
{
    if (bEmptyList || victim->GetTypeId() != TYPEID_PLAYER)
        return;

    for (std::list<CreatureEventAIHolder>::iterator i = CreatureEventAIList.begin(); i != CreatureEventAIList.end(); ++i)
    {
        if ((*i).Event.event_type == EVENT_T_KILL)
            ProcessEvent(*i, victim);
    }
}

void CreatureEventAI::JustSummoned(Creature* pUnit)
{
    if (bEmptyList || !pUnit)
        return;

    for (std::list<CreatureEventAIHolder>::iterator i = CreatureEventAIList.begin(); i != CreatureEventAIList.end(); ++i)
    {
        if ((*i).Event.event_type == EVENT_T_SUMMONED_UNIT)
            ProcessEvent(*i, pUnit);
    }
}

void CreatureEventAI::Aggro(Unit *who)
{
    //Check for on combat start events
    if (!bEmptyList)
    {
        for (std::list<CreatureEventAIHolder>::iterator i = CreatureEventAIList.begin(); i != CreatureEventAIList.end(); ++i)
        {
            switch ((*i).Event.event_type)
            {
                case EVENT_T_AGGRO:
                    (*i).Enabled = true;
                    ProcessEvent(*i, who);
                    break;
                    //Reset all in combat timers
                case EVENT_T_TIMER:
                    if ((*i).Event.event_param2 == (*i).Event.event_param1)
                    {
                        (*i).Time = (*i).Event.event_param1;
                        (*i).Enabled = true;
                    }
                    else if ((*i).Event.event_param2 > (*i).Event.event_param1)
                    {
                        (*i).Time = urand((*i).Event.event_param1, (*i).Event.event_param2);
                        (*i).Enabled = true;
                    }
                    else
                        sLog.outErrorDb("CreatureEventAI: Creature %u using Event %u (Type = %u) has InitialMax < InitialMin. Event disabled.", m_creature.GetEntry(), (*i).Event.event_id, (*i).Event.event_type);
                    break;
                    //All normal events need to be re-enabled and their time set to 0
                default:
                    (*i).Enabled = true;
                    (*i).Time = 0;
                    break;
            }
        }
    }

    EventUpdateTime = EVENT_UPDATE_TIME;
    EventDiff = 0;
}

void CreatureEventAI::AttackStart(Unit *who)
{
    if (!who)
        return;

    if (m_creature.Attack(who, MeleeEnabled))
    {
        m_creature.AddThreat(who, 0.0f);
        m_creature.SetInCombatWith(who);
        who->SetInCombatWith(&m_creature);

        if (!InCombat)
        {
            InCombat = true;
            Aggro(who);
        }

        if (CombatMovementEnabled)
        {
            m_creature.GetMotionMaster()->MoveChase(who, AttackDistance, AttackAngle);
        }
        else
        {
            m_creature.GetMotionMaster()->MoveIdle();
            m_creature.StopMoving();
        }
    }
}

void CreatureEventAI::MoveInLineOfSight(Unit *who)
{
    if (!who)
        return;

    //Check for OOC LOS Event
    if (!bEmptyList && !m_creature.getVictim())
    {
        for (std::list<CreatureEventAIHolder>::iterator itr = CreatureEventAIList.begin(); itr != CreatureEventAIList.end(); ++itr)
        {
            if ((*itr).Event.event_type == EVENT_T_OOC_LOS)
            {
                //can trigger if closer than fMaxAllowedRange
                float fMaxAllowedRange = (*itr).Event.event_param2;

                //if range is ok and we are actually in LOS
                if (m_creature.IsWithinDistInMap(who, fMaxAllowedRange) && m_creature.IsWithinLOSInMap(who))
                {
                    //if friendly event&&who is not hostile OR hostile event&&who is hostile
                    if (((*itr).Event.event_param1 && !m_creature.IsHostileTo(who)) ||
                        ((!(*itr).Event.event_param1) && m_creature.IsHostileTo(who)))
                        ProcessEvent(*itr, who);
                }
            }
        }
    }

    if (m_creature.isCivilian() && m_creature.IsNeutralToAll())
        return;

    if (!m_creature.hasUnitState(UNIT_STAT_STUNNED) && who->isTargetableForAttack() &&
        m_creature.IsHostileTo(who) && who->isInAccessablePlaceFor(&m_creature))
    {
        if (!m_creature.canFly() && m_creature.GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
            return;

        float attackRadius = m_creature.GetAttackDistance(who);
        if (m_creature.IsWithinDistInMap(who, attackRadius) && m_creature.IsWithinLOSInMap(who))
        {
            if (!m_creature.getVictim())
            {
                AttackStart(who);
                who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
            }
            else if (m_creature.GetMap()->IsDungeon())
            {
                who->SetInCombatWith(&m_creature);
                m_creature.AddThreat(who, 0.0f);
            }
        }
    }
}

void CreatureEventAI::SpellHit(Unit* pUnit, const SpellEntry* pSpell)
{

    if (bEmptyList)
        return;

    for (std::list<CreatureEventAIHolder>::iterator i = CreatureEventAIList.begin(); i != CreatureEventAIList.end(); ++i)
    {
        if ((*i).Event.event_type == EVENT_T_SPELLHIT)
        {
            //If spell id matches (or no spell id) & if spell school matches (or no spell school)
            if (!(*i).Event.event_param1 || pSpell->Id == (*i).Event.event_param1)
            {
                if ((*i).Event.event_param2_s == -1 || pSpell->SchoolMask == (*i).Event.event_param2)
                    ProcessEvent(*i, pUnit);
            }
        }
    }
}

void CreatureEventAI::UpdateAI(const uint32 diff)
{
    //Check if we are in combat (also updates calls threat update code)
    bool Combat = InCombat ? (m_creature.SelectHostilTarget() && m_creature.getVictim()) : false;

    //Must return if creature isn't alive. Normally select hostil target and get victim prevent this
    if (!m_creature.isAlive())
        return;

    if (!bEmptyList)
    {
        //Events are only updated once every EVENT_UPDATE_TIME ms to prevent lag with large amount of events
        if (EventUpdateTime < diff)
        {
            EventDiff += diff;

            //Check for time based events
            for (std::list<CreatureEventAIHolder>::iterator i = CreatureEventAIList.begin(); i != CreatureEventAIList.end(); ++i)
            {
                //Decrement Timers
                if ((*i).Time)
                {
                    if ((*i).Time > EventDiff)
                    {
                        //Do not decrement timers if event cannot trigger in this phase
                        if (!((*i).Event.event_inverse_phase_mask & (1 << Phase)))
                            (*i).Time -= EventDiff;

                        //Skip processing of events that have time remaining
                        continue;
                    }
                    else (*i).Time = 0;
                }

                //Events that are updated every EVENT_UPDATE_TIME
                switch ((*i).Event.event_type)
                {
                    case EVENT_T_TIMER_OOC:
                        ProcessEvent(*i);
                        break;
                    case EVENT_T_TIMER:
                    case EVENT_T_MANA:
                    case EVENT_T_HP:
                    case EVENT_T_TARGET_HP:
                    case EVENT_T_TARGET_CASTING:
                    case EVENT_T_FRIENDLY_HP:
                        if (Combat)
                            ProcessEvent(*i);
                        break;
                    case EVENT_T_RANGE:
                        if (Combat)
                        {
                            if (m_creature.IsWithinDistInMap(m_creature.getVictim(),(float)(*i).Event.event_param2))
                            {
                                if (m_creature.GetDistance(m_creature.getVictim()) >= (float)(*i).Event.event_param1)
                                    ProcessEvent(*i);
                            }
                        }
                        break;
                }
            }

            EventDiff = 0;
            EventUpdateTime = EVENT_UPDATE_TIME;
        }
        else
        {
            EventDiff += diff;
            EventUpdateTime -= diff;
        }
    }

    //Melee Auto-Attack
    if (Combat && MeleeEnabled)
        DoMeleeAttackIfReady();
}

bool CreatureEventAI::IsVisible(Unit *pl) const
{
    return m_creature.GetDistance(pl) < sWorld.getConfig(CONFIG_SIGHT_MONSTER)
        && pl->isVisibleForOrDetect(&m_creature,true);
}

inline Unit* CreatureEventAI::SelectUnit(AttackingTarget target, uint32 position)
{
    //ThreatList m_threatlist;
    std::list<HostilReference*>& m_threatlist = m_creature.getThreatManager().getThreatList();
    std::list<HostilReference*>::iterator i = m_threatlist.begin();
    std::list<HostilReference*>::reverse_iterator r = m_threatlist.rbegin();

    if (position >= m_threatlist.size() || !m_threatlist.size())
        return NULL;

    switch (target)
    {
        case ATTACKING_TARGET_RANDOM:
        {
            advance ( i , position +  (rand() % (m_threatlist.size() - position ) ));
            return Unit::GetUnit(m_creature,(*i)->getUnitGuid());
        }
        case ATTACKING_TARGET_TOPAGGRO:
        {
            advance ( i , position);
            return Unit::GetUnit(m_creature,(*i)->getUnitGuid());
        }
        case ATTACKING_TARGET_BOTTOMAGGRO:
        {
            advance ( r , position);
            return Unit::GetUnit(m_creature,(*r)->getUnitGuid());
        }
    }
    return NULL;
}

inline uint32 CreatureEventAI::GetRandActionParam(uint32 rnd, uint32 param1, uint32 param2, uint32 param3)
{
    switch (rnd % 3)
    {
        case 0:
            return param1;
            break;
        case 1:
            return param2;
            break;
        case 2:
            return param3;
            break;
    }
    return 0;
}

inline Unit* CreatureEventAI::GetTargetByType(uint32 Target, Unit* pActionInvoker)
{
    switch (Target)
    {
        case TARGET_T_SELF:
            return &m_creature;
            break;
        case TARGET_T_HOSTILE:
            return m_creature.getVictim();
            break;
        case TARGET_T_HOSTILE_SECOND_AGGRO:
            return SelectUnit(ATTACKING_TARGET_TOPAGGRO,1);
            break;
        case TARGET_T_HOSTILE_LAST_AGGRO:
            return SelectUnit(ATTACKING_TARGET_BOTTOMAGGRO,0);
            break;
        case TARGET_T_HOSTILE_RANDOM:
            return SelectUnit(ATTACKING_TARGET_RANDOM,0);
            break;
        case TARGET_T_HOSTILE_RANDOM_NOT_TOP:
            return SelectUnit(ATTACKING_TARGET_RANDOM,1);
            break;
        case TARGET_T_ACTION_INVOKER:
            return pActionInvoker;
            break;
        default:
            return NULL;
            break;
    };
}

Unit* CreatureEventAI::DoSelectLowestHpFriendly(float range, uint32 MinHPDiff)
{
    CellPair p(MaNGOS::ComputeCellPair(m_creature.GetPositionX(), m_creature.GetPositionY()));
    Cell cell(p);
    cell.data.Part.reserved = ALL_DISTRICT;
    cell.SetNoCreate();

    Unit* pUnit = NULL;

    MaNGOS::MostHPMissingInRange u_check(&m_creature, range, MinHPDiff);
    MaNGOS::UnitLastSearcher<MaNGOS::MostHPMissingInRange> searcher(&m_creature, pUnit, u_check);

    /*
    typedef TYPELIST_4(GameObject, Creature*except pets*, DynamicObject, Corpse*Bones*) AllGridObjectTypes;
    This means that if we only search grid then we cannot possibly return pets or players so this is safe
    */
    TypeContainerVisitor<MaNGOS::UnitLastSearcher<MaNGOS::MostHPMissingInRange>, GridTypeMapContainer >  grid_unit_searcher(searcher);

    CellLock<GridReadGuard> cell_lock(cell, p);
    cell_lock->Visit(cell_lock, grid_unit_searcher, *m_creature.GetMap());
    return pUnit;
}

void CreatureEventAI::DoFindFriendlyCC(std::list<Creature*>& _list, float range)
{
    CellPair p(MaNGOS::ComputeCellPair(m_creature.GetPositionX(), m_creature.GetPositionY()));
    Cell cell(p);
    cell.data.Part.reserved = ALL_DISTRICT;
    cell.SetNoCreate();

    MaNGOS::FriendlyCCedInRange u_check(&m_creature, range);
    MaNGOS::CreatureListSearcher<MaNGOS::FriendlyCCedInRange> searcher(&m_creature, _list, u_check);

    TypeContainerVisitor<MaNGOS::CreatureListSearcher<MaNGOS::FriendlyCCedInRange>, GridTypeMapContainer >  grid_creature_searcher(searcher);

    CellLock<GridReadGuard> cell_lock(cell, p);
    cell_lock->Visit(cell_lock, grid_creature_searcher, *m_creature.GetMap());
}

void CreatureEventAI::DoFindFriendlyMissingBuff(std::list<Creature*>& _list, float range, uint32 spellid)
{
    CellPair p(MaNGOS::ComputeCellPair(m_creature.GetPositionX(), m_creature.GetPositionY()));
    Cell cell(p);
    cell.data.Part.reserved = ALL_DISTRICT;
    cell.SetNoCreate();

    MaNGOS::FriendlyMissingBuffInRange u_check(&m_creature, range, spellid);
    MaNGOS::CreatureListSearcher<MaNGOS::FriendlyMissingBuffInRange> searcher(&m_creature, _list, u_check);

    TypeContainerVisitor<MaNGOS::CreatureListSearcher<MaNGOS::FriendlyMissingBuffInRange>, GridTypeMapContainer >  grid_creature_searcher(searcher);

    CellLock<GridReadGuard> cell_lock(cell, p);
    cell_lock->Visit(cell_lock, grid_creature_searcher, *m_creature.GetMap());
}

//*********************************
//*** Functions used globally ***

void CreatureEventAI::DoScriptText(int32 textEntry, WorldObject* pSource, Unit* target)
{
    if (!pSource)
    {
        sLog.outErrorDb("CreatureEventAI: DoScriptText entry %i, invalid Source pointer.",textEntry);
        return;
    }

    if (textEntry >= 0)
    {
        sLog.outErrorDb("CreatureEventAI: DoScriptText with source entry %u (TypeId=%u, guid=%u) attempts to process text entry %i, but text entry must be negative.",pSource->GetEntry(),pSource->GetTypeId(),pSource->GetGUIDLow(),textEntry);
        return;
    }

    CreatureEventAI_TextMap::const_iterator i = CreatureEAI_Mgr.GetCreatureEventAITextMap().find(textEntry);

    if (i == CreatureEAI_Mgr.GetCreatureEventAITextMap().end())
    {
        sLog.outErrorDb("CreatureEventAI: DoScriptText with source entry %u (TypeId=%u, guid=%u) could not find text entry %i.",pSource->GetEntry(),pSource->GetTypeId(),pSource->GetGUIDLow(),textEntry);
        return;
    }

    sLog.outDebug("CreatureEventAI: DoScriptText: text entry=%i, Sound=%u, Type=%u, Language=%u, Emote=%u",textEntry,(*i).second.SoundId,(*i).second.Type,(*i).second.Language,(*i).second.Emote);

    if((*i).second.SoundId)
    {
        if (GetSoundEntriesStore()->LookupEntry((*i).second.SoundId))
            pSource->PlayDirectSound((*i).second.SoundId);
        else
            sLog.outErrorDb("CreatureEventAI: DoScriptText entry %i tried to process invalid sound id %u.",textEntry,(*i).second.SoundId);
    }

    if((*i).second.Emote)
    {
        if (pSource->GetTypeId() == TYPEID_UNIT || pSource->GetTypeId() == TYPEID_PLAYER)
        {
            ((Unit*)pSource)->HandleEmoteCommand((*i).second.Emote);
        }
        else
            sLog.outErrorDb("CreatureEventAI: DoScriptText entry %i tried to process emote for invalid TypeId (%u).",textEntry,pSource->GetTypeId());
    }

    switch((*i).second.Type)
    {
        case CHAT_TYPE_SAY:
            pSource->MonsterSay(textEntry, (*i).second.Language, target ? target->GetGUID() : 0);
            break;
        case CHAT_TYPE_YELL:
            pSource->MonsterYell(textEntry, (*i).second.Language, target ? target->GetGUID() : 0);
            break;
        case CHAT_TYPE_TEXT_EMOTE:
            pSource->MonsterTextEmote(textEntry, target ? target->GetGUID() : 0);
            break;
        case CHAT_TYPE_BOSS_EMOTE:
            pSource->MonsterTextEmote(textEntry, target ? target->GetGUID() : 0, true);
            break;
        case CHAT_TYPE_WHISPER:
        {
            if (target && target->GetTypeId() == TYPEID_PLAYER)
                pSource->MonsterWhisper(textEntry, target->GetGUID());
            else sLog.outErrorDb("CreatureEventAI: DoScriptText entry %i cannot whisper without target unit (TYPEID_PLAYER).", textEntry);
        }break;
        case CHAT_TYPE_BOSS_WHISPER:
        {
            if (target && target->GetTypeId() == TYPEID_PLAYER)
                pSource->MonsterWhisper(textEntry, target->GetGUID(), true);
            else sLog.outErrorDb("CreatureEventAI: DoScriptText entry %i cannot whisper without target unit (TYPEID_PLAYER).", textEntry);
        }break;
        case CHAT_TYPE_ZONE_YELL:
            pSource->MonsterYellToZone(textEntry, (*i).second.Language, target ? target->GetGUID() : 0);
            break;
    }
}

void CreatureEventAI::DoZoneInCombat(Unit* pUnit)
{
    if (!pUnit)
        pUnit = &m_creature;

    Map *map = pUnit->GetMap();

    if (!map->IsDungeon())                                  //use IsDungeon instead of Instanceable, in case battlegrounds will be instantiated
    {
        sLog.outErrorDb("CreatureEventAI: DoZoneInCombat call for map that isn't an instance (pUnit entry = %d)", pUnit->GetTypeId() == TYPEID_UNIT ? ((Creature*)pUnit)->GetEntry() : 0);
        return;
    }

    if (!pUnit->CanHaveThreatList() || pUnit->getThreatManager().isThreatListEmpty())
    {
        sLog.outErrorDb("CreatureEventAI: DoZoneInCombat called for creature that either cannot have threat list or has empty threat list (pUnit entry = %d)", pUnit->GetTypeId() == TYPEID_UNIT ? ((Creature*)pUnit)->GetEntry() : 0);

        return;
    }

    Map::PlayerList const &PlayerList = map->GetPlayers();
    for(Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
        if (Player* i_pl = i->getSource())
            if (!i_pl->isGameMaster())
                pUnit->AddThreat(i_pl, 0.0f);
}

void CreatureEventAI::DoMeleeAttackIfReady()
{
    //Make sure our attack is ready before checking distance
    if (m_creature.isAttackReady())
    {
        //If we are within range melee the target
        if (m_creature.IsWithinDistInMap(m_creature.getVictim(), ATTACK_DISTANCE))
        {
            m_creature.AttackerStateUpdate(m_creature.getVictim());
            m_creature.resetAttackTimer();
        }
    }
}

bool CreatureEventAI::CanCast(Unit* Target, SpellEntry const *Spell, bool Triggered)
{
    //No target so we can't cast
    if (!Target || !Spell)
        return false;

    //Silenced so we can't cast
    if (!Triggered && m_creature.HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED))
        return false;

    //Check for power
    if (!Triggered && m_creature.GetPower((Powers)Spell->powerType) < Spell->manaCost)
        return false;

    SpellRangeEntry const *TempRange = NULL;

    TempRange = GetSpellRangeStore()->LookupEntry(Spell->rangeIndex);

    //Spell has invalid range store so we can't use it
    if (!TempRange)
        return false;

    //Unit is out of range of this spell
    if (m_creature.GetDistance(Target) > TempRange->maxRange || m_creature.GetDistance(Target) < TempRange->minRange)
        return false;

    return true;
}

bool CreatureEventAI::ReceiveEmote(Player* pPlayer, Creature* pCreature, uint32 uiEmote)
{
    CreatureEventAI* pTmpCreature = (CreatureEventAI*)(pCreature->AI());

    if (pTmpCreature->bEmptyList)
        return true;

    for (std::list<CreatureEventAIHolder>::iterator itr = pTmpCreature->CreatureEventAIList.begin(); itr != pTmpCreature->CreatureEventAIList.end(); ++itr)
    {
        if ((*itr).Event.event_type == EVENT_T_RECEIVE_EMOTE)
        {
            if ((*itr).Event.event_param1 != uiEmote)
                return true;

            bool bProcess = false;

            switch((*itr).Event.event_param2)
            {
                //enum ConditionType
                case CONDITION_NONE:                        // 0 0
                    bProcess = true;
                    break;
                case CONDITION_AURA:                        // spell_id     effindex
                    if (pPlayer->HasAura((*itr).Event.event_param3,(*itr).Event.event_param4))
                        bProcess = true;
                    break;
                case CONDITION_ITEM:                        // item_id      count
                    if (pPlayer->HasItemCount((*itr).Event.event_param3,(*itr).Event.event_param4))
                        bProcess = true;
                    break;
                case CONDITION_ITEM_EQUIPPED:               // item_id      count
                    if (pPlayer->HasItemOrGemWithIdEquipped((*itr).Event.event_param3,(*itr).Event.event_param4))
                        bProcess = true;
                    break;
                case CONDITION_ZONEID:                      // zone_id      0
                    if (pPlayer->GetZoneId() == (*itr).Event.event_param3)
                        bProcess = true;
                    break;
                case CONDITION_REPUTATION_RANK:             // faction_id   min_rank
                    if (pPlayer->GetReputationRank((*itr).Event.event_param3) >= (*itr).Event.event_param4)
                        bProcess = true;
                    break;
                case CONDITION_TEAM:                        // player_team  0, (469 - Alliance 67 - Horde)
                    if (pPlayer->GetTeam() == (*itr).Event.event_param3)
                        bProcess = true;
                    break;
                case CONDITION_SKILL:                       // skill_id     min skill_value
                    if (pPlayer->HasSkill((*itr).Event.event_param3) && pPlayer->GetSkillValue((*itr).Event.event_param3) >= (*itr).Event.event_param4)
                        bProcess = true;
                    break;
                case CONDITION_QUESTREWARDED:               // quest_id     0
                    if (pPlayer->GetQuestRewardStatus((*itr).Event.event_param3))
                        bProcess = true;
                    break;
                case CONDITION_QUESTTAKEN:                  // quest_id     0, for condition true while quest active.
                    if (pPlayer->GetQuestStatus((*itr).Event.event_param3) == QUEST_STATUS_INCOMPLETE)
                        bProcess = true;
                    break;
                case CONDITION_ACTIVE_EVENT:                // event_id     0
                    if (IsHolidayActive(HolidayIds((*itr).Event.event_param3)))
                        bProcess = true;
                    break;
            }

            if (bProcess)
            {
                sLog.outDebug("CreatureEventAI: ReceiveEmote CreatureEventAI: Condition ok, processing");
                pTmpCreature->ProcessEvent(*itr, pPlayer);
            }
        }
    }

    return true;
}
