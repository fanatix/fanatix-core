/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
SDName: Boss_Nightbane
SD%Complete: 0
SDComment: Place holder
SDCategory: Karazhan
EndScriptData */

#include "precompiled.h"
#include "sc_creature.h"
#include "WorldPacket.h"
#include "def_karazhan.h"

// Ground Abilities
#define SPELL_BELLOWING_ROAR        36922 // Typical dragon fear with a 2.5-second cast time, short duration, and 30-second cooldown. He casts this once every 45 seconds to 1 minute.
#define SPELL_CLEAVE                30131 // Typical dragon cleave, frontal arc. Hits for approximately 6,000 on plate, 11,000 on cloth.
#define SPELL_CHARRED_EARTH         30129 // Chars the ground, setting it flame, 2188 to 2812 Fire damage every 3 sec
#define SPELL_DISTRACTING_ASH       30130 // Reduces the chance to hit with spells and abilities by 30%.
#define SPELL_SMOLDERING_BREATH     30210 // Inflicts 3700 to 4300 Fire damage to enemies in a cone in front of the caster and ~1700 tvery 3 sec (5 ticks)
#define SPELL_TAIL_SWEEP            25653 // Inflicts 450 damage on enemies in a cone behind the caster, knocking them back.

// Fly Abilities
#define SPELL_RAIN_OF_BONES         37098 // Bombards the target and nearby allies with bone fragments, summoning 5 skeletons in the process.
#define SPELL_SMOKING_BLAST         37057 // Deals 1850 to 2150 physical damage to the target and additional fire damage over time.
#define SPELL_FIREBALL_BARRAGE      30282 // Throws fireballs at any enemies farther than 40 yards away.

#define SOUND_DEATH                 9454
#define SOUND_PRE_AGGRO             9455
#define SOUND_AGGRO                 9456
#define SOUND_ATTACK                9451
#define SOUND_WOUND                 9452
#define SOUND_WOUND_CRIT            9453
#define SOUND_LOOP                  9457

// summon (Fly Phase)
#define SUMMON                      17261

// Move boss - new
struct Locations
{
    float x, y, z;
};

static Locations Flight[]=
{
	{-11181, -1893, 104},
    {-11163, -1903, 92}
};

struct MANGOS_DLL_DECL boss_nightbaneAI : public ScriptedAI
{
    boss_nightbaneAI(Creature *c) : ScriptedAI(c) {pInstance = ((ScriptedInstance*)c->GetInstanceData());Reset();}

    ScriptedInstance* pInstance;

    // Ground State
    uint32 Bellowing_Timer;
    uint32 Charred_Timer;
    uint32 Distracting_Timer;
    uint32 Smoldering_Timer;
    uint32 Cleave_Timer;
    uint32 Tail_Timer;

    // Air State
    uint32 Fly_Timer;
    uint32 Rain_Timer;
    uint32 Summon_Timer;
    uint32 Smoking_Timer;
    uint32 Fireball_Timer;

    uint32 Phase;
    uint32 Wait_Timer;
    uint32 Summons;
    Locations Summon_Loc;
    Creature* Summoned;
    bool Flying;
    bool Wait;
    bool InCombat;

    void Reset()
    {
        // Ground State
        Bellowing_Timer = 60000;
        Charred_Timer = (20+rand()%20)*1000;
        Distracting_Timer = (30+rand()%60)*1000;;
        Smoldering_Timer = 35000;
        Cleave_Timer = 5000;
        Tail_Timer = 25000;
        Summon_Loc.x = Flight[1].x;
        Summon_Loc.y = Flight[1].y;
        Summon_Loc.z = Flight[1].z;
        Phase = 1;

        Flying = false;
        Wait = false;
        InCombat = false;

        if(pInstance)
            pInstance->SetData(DATA_NIGHTBANE_EVENT, 0);

        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, SPELL_AURA_MOD_TAUNT, true);
    }

    void JustDied(Unit* Killer)
    {
        DoPlaySoundToSet(m_creature, SOUND_DEATH);
        if(pInstance)
            pInstance->SetData(DATA_NIGHTBANE_EVENT, 3); // Completed
    }

    void Aggro(Unit *who)
    {
        DoPlaySoundToSet(m_creature, SOUND_AGGRO);
        if(pInstance)
            pInstance->SetData(DATA_NIGHTBANE_EVENT, 1);
        InCombat = true;
    }

    void ResetThreat()
    {
        std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();
        if(m_threatlist.empty()) return;
        std::list<HostilReference*>::iterator i = m_threatlist.begin();
        for(i = m_threatlist.begin(); i != m_threatlist.end(); ++i)
        {
            Unit* pUnit = NULL;
            pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
            if(pUnit)
                m_creature->getThreatManager().modifyThreatPercent(pUnit, -100);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim()) 
            return;

        if((m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 75) && Phase == 1)
        {
            m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
            m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
            (*m_creature).GetMotionMaster()->Clear(false);
            (*m_creature).GetMotionMaster()->MoveIdle();
            m_creature->SetHover(true);
            Wait = true;
            Wait_Timer = CreatureMove(Flight[0].x, Flight[0].y, Flight[0].z) + 2;
            Phase++;
        }
        if((m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 50) && Phase == 2)
        {
            m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
            m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
            (*m_creature).GetMotionMaster()->Clear(false);
            (*m_creature).GetMotionMaster()->MoveIdle();
            m_creature->SetHover(true);
            Wait = true;
            Wait_Timer = CreatureMove(Flight[0].x, Flight[0].y, Flight[0].z) + 2;
            Phase++;
        }

        if((m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 25) && Phase == 3)
        {
            m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
            m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
            (*m_creature).GetMotionMaster()->Clear(false);
            (*m_creature).GetMotionMaster()->MoveIdle();
            m_creature->SetHover(true);
            Wait = true;
            Wait_Timer = CreatureMove(Flight[0].x, Flight[0].y, Flight[0].z) + 2;
            Phase++;
        }

        if(Wait)
        {
            if (Wait_Timer < diff)
            {
                if (Flying)
                {
                    Bellowing_Timer = 60000;
                    Distracting_Timer = (30+rand()%60)*1000;;
                    Charred_Timer = (20+rand()%20)*1000;
                    Cleave_Timer = 5000;
                    Smoldering_Timer = 35000;
                    Tail_Timer = 25000;
                    Flying = false;
                    m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LAND);
                    m_creature->SetHover(false);
                    (*m_creature).GetMotionMaster()->Clear(false);
                    (*m_creature).GetMotionMaster()->MoveChase(m_creature->getVictim());
                    ResetThreat();
                }
                else
                {
                    Rain_Timer = 2000;
                    Fireball_Timer = 5000;
                    Smoking_Timer = 999999;
                    Summon_Timer = 999999;
                    Fly_Timer = 999999;
                    Summons = 1;
                    Flying = true;
                }
                Wait = false;
            }
            else Wait_Timer -= diff;
        }
        else
        {
            if (!Flying)
            {
                if (Bellowing_Timer < diff)
                {
                    DoCast(m_creature,SPELL_BELLOWING_ROAR);
                    Bellowing_Timer = (45+rand()%15)*1000;
                }else Bellowing_Timer -= diff;

                if (Distracting_Timer < diff)
                {
                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    if (target)
                        DoCast(target,SPELL_DISTRACTING_ASH);
                    else
                        DoCast(m_creature->getVictim(),SPELL_DISTRACTING_ASH);
                    Distracting_Timer = (30+rand()%60)*1000;
                }else Distracting_Timer -= diff;

                if (Charred_Timer < diff)
                {
                    std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();
                    std::list<Unit*> targets;
                    std::list<HostilReference*>::iterator i = m_threatlist.begin();
                    for(i = m_threatlist.begin(); i != m_threatlist.end(); ++i)
                    {
                        Unit* pUnit = NULL;
                        pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
                        if (pUnit && pUnit->isAlive() && !m_creature->IsWithinDistInMap(pUnit, 10) && (pUnit->GetTypeId() == TYPEID_PLAYER))
                            targets.push_back(pUnit);
                    }
                    if(!targets.empty())
                    {
                        std::list<Unit*>::iterator itr = targets.begin();
                        advance(itr,(rand()%(targets.size())));
                        Creature* Target_Helper;
                        Unit* target = *itr;
                        Target_Helper = m_creature->SummonCreature(17260,target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,30000);
                        Target_Helper->SetUInt32Value(UNIT_NPC_FLAGS,0);
                        Target_Helper->setFaction(45);
                        Target_Helper->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        DoCast(Target_Helper,SPELL_CHARRED_EARTH);
                        Charred_Timer = (20+rand()%20)*1000;
                    }
                }else Charred_Timer -= diff;

                if (Cleave_Timer < diff)
                {
                    DoCast(m_creature->getVictim(),SPELL_CLEAVE);
                    Cleave_Timer = 10000;
                }else Cleave_Timer -= diff;

                if(Smoldering_Timer <diff)
                {
                    DoCast(m_creature->getVictim(),SPELL_SMOLDERING_BREATH);
                    Smoldering_Timer = 30000;
                }else Smoldering_Timer -= diff;

                if(Tail_Timer <diff)
                {
                    DoCast(m_creature,SPELL_TAIL_SWEEP);
                    Tail_Timer = 30000;
                }else Tail_Timer -= diff;

                DoMeleeAttackIfReady();
            }
            else
            {
                if (Rain_Timer < diff)
                {
                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    if (!target) 
                        target = m_creature->getVictim();
                    DoCast(target,SPELL_RAIN_OF_BONES);
                    Summon_Loc.x = target->GetPositionX();
                    Summon_Loc.y = target->GetPositionY();
                    Summon_Loc.z = target->GetPositionZ();
                    Summon_Timer = 3000;
                    Smoking_Timer = 12000;
                    Rain_Timer = 999999;
                }else Rain_Timer -= diff;

                if ((Summon_Timer < diff) && (Summons <= 5))
                {
                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    Summoned = m_creature->SummonCreature(SUMMON,Summon_Loc.x - (rand()%10) + 5, Summon_Loc.y - (rand()%10) + 5, Summon_Loc.z,0,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,900000);
                    if (target)
                        ((CreatureAI*)Summoned->AI())->AttackStart(target);
                    else
                        ((CreatureAI*)Summoned->AI())->AttackStart(m_creature->getVictim());
                    Summon_Timer = 5000;
                    Summons++;
                }else Summon_Timer -= diff;

                if (Smoking_Timer < diff)
                {
                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    if (target)
                        DoCast(target,SPELL_SMOKING_BLAST);
                    else
                        DoCast(m_creature->getVictim(),SPELL_SMOKING_BLAST);
                    Fly_Timer = 16000;
                    Smoking_Timer = 999999;
                }else Smoking_Timer -= diff;

                if (Fireball_Timer < diff)
                {
                    std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();
                    std::list<HostilReference*>::iterator i = m_threatlist.begin();
                    for(i = m_threatlist.begin(); i != m_threatlist.end(); ++i)
                    {
                        Unit* pUnit = NULL;
                        pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
                        if(pUnit && pUnit->isAlive() && !m_creature->IsWithinDistInMap(pUnit, 40))
                            DoCast(pUnit,SPELL_FIREBALL_BARRAGE);
                    }
                    Fireball_Timer = 1000;
                }else Fireball_Timer -= diff;

                if (Fly_Timer < diff)
                {
                    m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
                    Wait = true;
                    Wait_Timer = CreatureMove(Flight[1].x, Flight[1].y, Flight[1].z) + 2;
                }else Fly_Timer -= diff;
            }
        }
    }

    uint32 CreatureMove(float DestX, float DestY, float DestZ)
    {
        float FromX = m_creature->GetPositionX();
        float FromY = m_creature->GetPositionY();
        float FromZ = m_creature->GetPositionZ();

        float dx = DestX - FromX;
        float dy = DestY - FromY;
        float dz = DestZ - FromZ;
        double dist = ::sqrt((dx*dx) + (dy*dy) + (dz*dz));
        double speed = m_creature->GetSpeed(MOVE_RUN);

        if(speed<=0)
            speed = 2.5f;
        speed *= 0.001f;

        uint32 TotalTime = static_cast<uint32>( dist/speed + 0.5 );
        m_creature->SendMonsterMove(DestX,DestY,DestZ,0,true,TotalTime);
        m_creature->Relocate(DestX,DestY,DestZ);
        return TotalTime;
    }
};

CreatureAI* GetAI_boss_nightbane(Creature *_Creature)
{
    return new boss_nightbaneAI (_Creature);
}

void AddSC_boss_nightbane()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_nightbane";
    newscript->GetAI = &GetAI_boss_nightbane;
    newscript->RegisterSelf();
}
