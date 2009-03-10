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
SDName: Boss_Brutallus
SD%Complete: 80
SDComment: Find a way to start the intro
EndScriptData */

#include "precompiled.h"
#include "def_sunwell_plateau.h"

//Yells & Quotes
#define YELL_INTRO                      -1580017
#define YELL_INTRO_BREAK_ICE            -1580018
#define YELL_INTRO_CHARGE               -1580019
#define YELL_INTRO_KILL_MADRIGOSA       -1580020
#define YELL_INTRO_TAUNT                -1580021
 
#define YELL_MADR_ICE_BARRIER           -1580031
#define YELL_MADR_INTRO                 -1580032
#define YELL_MADR_ICE_BLOCK             -1580033
#define YELL_MADR_TRAP                  -1580034
#define YELL_MADR_DEATH                 -1580035
 
#define YELL_AGGRO                      -1580022
#define YELL_KILL1                      -1580023
#define YELL_KILL2                      -1580024
#define YELL_KILL3                      -1580025
#define YELL_LOVE1                      -1580026
#define YELL_LOVE2                      -1580027
#define YELL_LOVE3                      -1580028
#define YELL_BERSERK                    -1580029
#define YELL_DEATH                      -1580030

//Spells for Brutallus & Madrigosa
#define SPELL_METEOR_SLASH              45150
#define SPELL_BURN                      45141
#define SPELL_STOMP                     45185
#define SPELL_BERSERK                   26662
#define SPELL_DUAL_WIELD                42459

#define FREEZING                        45203
#define FROST_BOLT                      44843
#define ENCAPSULATE                     45665
#define ENCAPSULATE_CHANELLING          45661

struct MANGOS_DLL_DECL boss_brutallusAI : public ScriptedAI
{
    boss_brutallusAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Intro = true;
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 SlashTimer;
    uint32 BurnTimer;
    uint32 StompTimer;
    uint32 BerserkTimer;
    uint32 ConversationTimer;
    uint32 FrostBoltTimer;
    uint32 Step;

    uint64 MadrigosaGUID;

    bool Intro;
    bool IsEvent;
    bool Enraged;

    void Reset()
    {
        SlashTimer = 11000;
        StompTimer = 30000;
        BurnTimer = 60000;
        BerserkTimer = 360000;
        ConversationTimer = 0;
        FrostBoltTimer = 0;
        Step = 0;

        MadrigosaGUID = 0;

        IsEvent = false;
        Enraged = false;

        m_creature->CastSpell(m_creature, SPELL_DUAL_WIELD, true);

        if(pInstance)
            pInstance->SetData(DATA_BRUTALLUS_EVENT, NOT_STARTED);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(YELL_AGGRO, m_creature);

        if(pInstance)
            pInstance->SetData(DATA_BRUTALLUS_EVENT, IN_PROGRESS);
    }

    void StartEvent()
    {
        MadrigosaGUID = pInstance->GetData64(DATA_MADRIGOSA);
        Unit* Madrigosa = Unit::GetUnit(*m_creature, MadrigosaGUID);
        if(Madrigosa)
        {
            DoScriptText(YELL_MADR_ICE_BARRIER, Madrigosa);
            //Madrigosa->activate(true);
            IsEvent = true;
            Step = 0;
            ConversationTimer = 5000;
        }
    }

    void KilledUnit(Unit* victim)
    {
        switch(rand()%3)
        {
            case 0: DoScriptText(YELL_KILL1, m_creature); break;
            case 1: DoScriptText(YELL_KILL2, m_creature); break;
            case 2: DoScriptText(YELL_KILL3, m_creature); break;
        }
    }
 
    void JustDied(Unit* Killer)
    {
        DoScriptText(YELL_DEATH, m_creature);

        if(pInstance)
            pInstance->SetData(DATA_BRUTALLUS_EVENT, DONE);
    }

    uint32 NextStep(uint32 Step)
    {              
        Unit* Madrigosa = Unit::GetUnit(*m_creature, MadrigosaGUID);

        switch(Step)
        {
        case 0: return 0;
        case 1:
            m_creature->SetInFront(Madrigosa);
            Madrigosa->SetInFront(m_creature);
            DoScriptText(YELL_MADR_INTRO, Madrigosa, m_creature);  
            return 7000;
        case 2: DoScriptText(YELL_INTRO, m_creature, Madrigosa); return 18000;
        case 3:
            DoCast(m_creature, FREEZING);
            Madrigosa->AddUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT + MOVEMENTFLAG_LEVITATING);
            FrostBoltTimer = 3000;
            return 28000;                                                
        case 4: DoScriptText(YELL_INTRO_BREAK_ICE, m_creature); return 6000;
        case 5:
             Madrigosa->CastSpell(m_creature, ENCAPSULATE_CHANELLING, false);
             DoScriptText(YELL_MADR_TRAP, Madrigosa);
             DoCast(m_creature,ENCAPSULATE);
             return 11000;
        case 6:
            m_creature->SetSpeed(MOVE_RUN, 4.0f, true);                        
            DoScriptText(YELL_INTRO_CHARGE, m_creature);
            return 3000;
        case 7:
            m_creature->DealDamage(Madrigosa, Madrigosa->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, true);
            DoScriptText(YELL_MADR_DEATH, Madrigosa);
            m_creature->SetHealth(m_creature->GetMaxHealth());            
            m_creature->AttackStop();
            m_creature->SetSpeed(MOVE_RUN, 1.0f, true);
            return 3000;
        case 8:
            DoScriptText(YELL_INTRO_KILL_MADRIGOSA, m_creature);
            m_creature->SetOrientation(0.14);
            Madrigosa->setDeathState(CORPSE);
            return 5000;
        case 9:
            DoScriptText(YELL_INTRO_TAUNT, m_creature);
            return 5000;
        case 10:
            //opendoor
            return 5000;
        default : return 0;
        }
    }
 
    void UpdateAI(const uint32 diff)
    {
        if((pInstance->GetData(DATA_BRUTALLUS_EVENT) == SPECIAL) && !IsEvent)
            StartEvent();

        if(IsEvent)
        {
            if(ConversationTimer < diff && Intro)
            {
                ConversationTimer = NextStep(++Step);            
            }else ConversationTimer -= diff;

            if(Step == 3)
            {
                if(FrostBoltTimer < diff)
                {
                    Unit* Madrigosa = Unit::GetUnit(*m_creature, MadrigosaGUID);
                    Madrigosa->CastSpell(m_creature, FROST_BOLT, false);
                    FrostBoltTimer = 2000;
                }else FrostBoltTimer -= diff;
            }
        }

        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;             
 
        if(SlashTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_METEOR_SLASH);
            SlashTimer = 11000;
        }else SlashTimer -= diff;

        if(StompTimer < diff)
        {
            switch(rand()%3)
            {
                case 0: DoScriptText(YELL_LOVE1, m_creature); break;
                case 1: DoScriptText(YELL_LOVE2, m_creature); break;
                case 2: DoScriptText(YELL_LOVE3, m_creature); break;
            }

            Unit *Target = m_creature->getVictim();
            DoCast(Target, SPELL_STOMP);

            if(Target->HasAura(45151, 0))
                Target->RemoveAura(45151, 0);
            StompTimer = 30000;
        }else StompTimer -= diff;

        if(BurnTimer < diff)
        {
            if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                target->CastSpell(target,SPELL_BURN,true);
            BurnTimer = 60000;
        }else BurnTimer -= diff;

        if(BerserkTimer < diff && !Enraged)
        {
            DoScriptText(YELL_BERSERK, m_creature);
            DoCast(m_creature, SPELL_BERSERK);
            Enraged = true;
        }else BerserkTimer -= diff;
 
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_brutallus(Creature *_Creature)
{
    return new boss_brutallusAI (_Creature);
}
 
void AddSC_boss_brutallus()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_brutallus";
    newscript->GetAI = &GetAI_boss_brutallus;
    newscript->RegisterSelf();
}
