/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_Halazzi
SD%Complete: 80
SDComment: 
SDCategory: Zul��Aman
EndScriptData */

#include "precompiled.h"
#include "def_zulaman.h"
//#include "spell.h"

#define SAY_AGGRO                       -1568042
#define SAY_SPLIT                       -1568043
#define SAY_MERGE                       -1568044
#define SAY_SABERLASH1                  -1568045
#define SAY_SABERLASH2                  -1568046
#define SAY_BERSERK                     -1568047
#define SAY_KILL1                       -1568048
#define SAY_KILL2                       -1568049
#define SAY_DEATH                       -1568050

#define SPELL_DUAL_WIELD                29651
#define SPELL_SABER_LASH                43267
#define SPELL_FRENZY                    43139
#define SPELL_FLAMESHOCK                43303
#define SPELL_EARTHSHOCK                43305
#define SPELL_TRANSFORM_SPLIT           43142
#define SPELL_TRANSFORM_SPLIT2          43573
#define SPELL_TRANSFORM_MERGE           43271
#define SPELL_SUMMON_LYNX               43143
#define SPELL_SUMMON_TOTEM              43302
#define SPELL_BERSERK                   45078

#define MOB_SPIRIT_LYNX                 24143
#define SPELL_LYNX_FRENZY               43290
#define SPELL_SHRED_ARMOR               43243

#define MOB_TOTEM                       24224
#define SPELL_LIGHTNING                 43301

enum PhaseHalazzi
{
    PHASE_NONE = 0,
    PHASE_LYNX = 1,
    PHASE_SPLIT = 2,
    PHASE_HUMAN = 3,
    PHASE_MERGE = 4,
    PHASE_ENRAGE = 5
};

struct MANGOS_DLL_DECL boss_halazziAI : public ScriptedAI
{
    boss_halazziAI(Creature *c) : ScriptedAI(c)
    { 
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
        // wait for core patch be accepted
        SpellEntry *TempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_SUMMON_TOTEM);
        if(TempSpell && TempSpell->EffectImplicitTargetA[0] != 1)
            TempSpell->EffectImplicitTargetA[0] = 1;
        // need to find out what controls totem's spell cooldown
        TempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_LIGHTNING);
        if(TempSpell && TempSpell->CastingTimeIndex != 5)
            TempSpell->CastingTimeIndex = 5; // 2000 ms casting time
    }

    ScriptedInstance *pInstance;

    uint32 FrenzyTimer;
    uint32 SaberlashTimer;
    uint32 ShockTimer;
    uint32 TotemTimer;
    uint32 CheckTimer;
    uint32 BerserkTimer;

    uint32 TransformCount;

    PhaseHalazzi Phase;

    uint64 LynxGUID;

    void Reset()
    {
		if(pInstance)
            pInstance->SetData(DATA_HALAZZIEVENT, NOT_STARTED);

        TransformCount = 0;
        BerserkTimer = 600000;
        CheckTimer = 1000;

        DoCast(m_creature, SPELL_DUAL_WIELD, true);

        Phase = PHASE_NONE;
        EnterPhase(PHASE_LYNX);
    }

    void Aggro(Unit *who)
    {
		if(pInstance)
            pInstance->SetData(DATA_HALAZZIEVENT, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
        EnterPhase(PHASE_LYNX);
    }

    void JustSummoned(Creature* summon)
    {
        summon->AI()->AttackStart(m_creature->getVictim());
        if(summon->GetEntry() == MOB_SPIRIT_LYNX)
            LynxGUID = summon->GetGUID();
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if(damage >= m_creature->GetHealth() && Phase != PHASE_ENRAGE)
            damage = 0;
    }

    void SpellHit(Unit*, const SpellEntry *spell)
    {
        if(spell->Id == SPELL_TRANSFORM_SPLIT2)
            EnterPhase(PHASE_HUMAN);
    }

    void AttackStart(Unit *who)
    {
        if(Phase != PHASE_MERGE)
            ScriptedAI::AttackStart(who);
    }

    void EnterPhase(PhaseHalazzi NextPhase)
    {
        switch(NextPhase)
        {
        case PHASE_LYNX:
        case PHASE_ENRAGE:
            if(Phase == PHASE_MERGE)
            {
                m_creature->CastSpell(m_creature, SPELL_TRANSFORM_MERGE, true);
                m_creature->Attack(m_creature->getVictim(), true);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            }
            if(Unit *Lynx = Unit::GetUnit(*m_creature, LynxGUID))
            {
                Lynx->SetVisibility(VISIBILITY_OFF);
                Lynx->setDeathState(JUST_DIED);
            }
            m_creature->SetMaxHealth(600000);
            m_creature->SetHealth(600000 - 150000 * TransformCount);
            FrenzyTimer = 16000;
            SaberlashTimer = 20000;
            ShockTimer = 10000;
            TotemTimer = 12000;
            break;
        case PHASE_SPLIT:
            DoScriptText(SAY_SPLIT, m_creature);
            m_creature->CastSpell(m_creature, SPELL_TRANSFORM_SPLIT, true);
            break;
        case PHASE_HUMAN:
            //DoCast(m_creature, SPELL_SUMMON_LYNX, true);
            DoSpawnCreature(MOB_SPIRIT_LYNX, 0,0,0,0, TEMPSUMMON_CORPSE_DESPAWN, 0);
            m_creature->SetMaxHealth(400000);
            m_creature->SetHealth(400000);
            ShockTimer = 10000;
            TotemTimer = 12000;
            break;
        case PHASE_MERGE:
            if(Unit *Lynx = Unit::GetUnit(*m_creature, LynxGUID))
            {
                DoScriptText(SAY_MERGE, m_creature);
                Lynx->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                Lynx->GetMotionMaster()->Clear();
                Lynx->GetMotionMaster()->MoveFollow(m_creature, 0, 0);
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveFollow(Lynx, 0, 0);
                TransformCount++;
            }break;
        default:
            break;
        }
        Phase = NextPhase;
    }

     void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() && !m_creature->getVictim())
            return;

        if(BerserkTimer < diff)
        {
            DoScriptText(SAY_BERSERK, m_creature);
            DoCast(m_creature, SPELL_BERSERK, true);
            BerserkTimer = 60000;
        }else BerserkTimer -= diff;

        if(Phase == PHASE_LYNX || Phase == PHASE_ENRAGE)
        {
            if(SaberlashTimer < diff)
            {
                // A tank with more than 490 defense skills should receive no critical hit
                //m_creature->CastSpell(m_creature, 41296, true);
                m_creature->CastSpell(m_creature->getVictim(), SPELL_SABER_LASH, true);
                //m_creature->RemoveAurasDueToSpell(41296);
                
                switch (rand()%2)
                {
                    case 0: DoScriptText(SAY_SABERLASH1, m_creature); break;
                    case 1: DoScriptText(SAY_SABERLASH2, m_creature); break;
                }
                SaberlashTimer = 30000;
            }else SaberlashTimer -= diff;

            if(FrenzyTimer < diff)
            {
                DoCast(m_creature, SPELL_FRENZY);
                FrenzyTimer = (10+rand()%5)*1000;
            }else FrenzyTimer -= diff;

            if(Phase == PHASE_LYNX)
            {
                if(CheckTimer < diff)
                {
                    if(m_creature->GetHealth() * 4 < m_creature->GetMaxHealth() * (3 - TransformCount))
                        EnterPhase(PHASE_SPLIT);
                    CheckTimer = 1000;
                }else CheckTimer -= diff;
            }
        }

        if(Phase == PHASE_HUMAN || Phase == PHASE_ENRAGE)
        {
            if(TotemTimer < diff)
            {
                DoCast(m_creature, SPELL_SUMMON_TOTEM);
                TotemTimer = 20000;
            }else TotemTimer -= diff;

            if(ShockTimer < diff)
            {
                if(Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                {
                    if(target->IsNonMeleeSpellCasted(false))
                        DoCast(target,SPELL_EARTHSHOCK);
                    else
                        DoCast(target,SPELL_FLAMESHOCK);
                    ShockTimer = 10000 + rand()%5000;
                }
            }else ShockTimer -= diff;

            if(Phase == PHASE_HUMAN)
            {
                if(CheckTimer < diff)
                {
                    if(m_creature->GetHealth() * 10 < m_creature->GetMaxHealth())
                        EnterPhase(PHASE_MERGE);
                    else
                    {
                        Unit *Lynx = Unit::GetUnit(*m_creature, LynxGUID);
                        if(Lynx && Lynx->GetHealth() * 10 < Lynx->GetMaxHealth())
                            EnterPhase(PHASE_MERGE);
                    }
                    CheckTimer = 1000;
                }else CheckTimer -= diff;
            }
        }

        if(Phase == PHASE_MERGE)
        {
            if(CheckTimer < diff)
            {
                Unit *Lynx = Unit::GetUnit(*m_creature, LynxGUID);
                if(Lynx && m_creature->IsWithinDistInMap(Lynx, 6.0f))
                {
                    if(TransformCount < 3)
                        EnterPhase(PHASE_LYNX);
                    else
                        EnterPhase(PHASE_ENRAGE);
                }
                CheckTimer = 1000;
            }else CheckTimer -= diff;
        }

        DoMeleeAttackIfReady();
    }

    void KilledUnit(Unit* victim)
    {
        switch(rand()%2)
        {
        case 0: DoScriptText(SAY_KILL1, m_creature); break;
        case 1: DoScriptText(SAY_KILL2, m_creature); break;
        }
    }

    void JustDied(Unit* Killer)
    {
		if(pInstance)
            pInstance->SetData(DATA_HALAZZIEVENT, DONE);

        DoScriptText(SAY_DEATH, m_creature);
    }
};

// Spirits Lynx AI

struct MANGOS_DLL_DECL boss_spiritlynxAI : public ScriptedAI
{
    boss_spiritlynxAI(Creature *c) : ScriptedAI(c) { Reset(); }

    uint32 FrenzyTimer;
    uint32 shredder_timer;

    void Reset()
    {
        FrenzyTimer = (30+rand()%20)*1000;  //frenzy every 30-50 seconds
        shredder_timer = 4000;
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if(damage >= m_creature->GetHealth())
            damage = 0;
    }

    void AttackStart(Unit *who)
    {
        if(!m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            ScriptedAI::AttackStart(who);
    }

    void Aggro(Unit *who) {DoZoneInCombat();}

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(FrenzyTimer < diff)
        {
            DoCast(m_creature, SPELL_LYNX_FRENZY);
            FrenzyTimer = (30+rand()%20)*1000;
        }else FrenzyTimer -= diff;

        if(shredder_timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_SHRED_ARMOR);
            shredder_timer = 4000;
        }else shredder_timer -= diff;

        DoMeleeAttackIfReady();
    }

};

CreatureAI* GetAI_boss_halazziAI(Creature *_Creature)
{
    return new boss_halazziAI (_Creature);
}

CreatureAI* GetAI_boss_spiritlynxAI(Creature *_Creature)
{
    return new boss_spiritlynxAI (_Creature);
}

void AddSC_boss_halazzi()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_halazzi";
    newscript->GetAI = &GetAI_boss_halazziAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_halazzi_lynx";
    newscript->GetAI = &GetAI_boss_spiritlynxAI;
    newscript->RegisterSelf();
}
