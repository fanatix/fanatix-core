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
SD%Complete: 100%
SDComment: Review for bugs, and other thingies
SDCategory: Karazhan
EndScriptData */

#include "precompiled.h"
#include "sc_creature.h"
#include "TargetedMovementGenerator.h"
#include "PointMovementGenerator.h"

#define SPELL_BELLOWING_ROAR        39427
#define SPELL_CHARRED_EARTH         30129 //Also 30209 (Target Charred Earth) triggers this
#define SPELL_DISTRACTING_ASH       30130
#define SPELL_SMOLDERING_BREATH     30210
#define SPELL_TAIL_SWEEP            25653
#define SPELL_RAIN_OF_BONES         37098
#define SPELL_SMOKING_BLAST         37057
#define SPELL_FIREBALL_BARRAGE      30282
#define SPELL_SHADOWCLEAVE            29832

#define SPELL_SUMMON_SKELL1            30170
#define SPELL_SUMMON_SKELL2            30181

float FlightMove[2][3] =
{
{-11182.244, -1876.486, 110.665}, //flight
{-11139.851, -1891.353, 91.473}, // ladning
};

struct MANGOS_DLL_DECL boss_nightbaneAI : public ScriptedAI
{
    boss_nightbaneAI(Creature* c) : ScriptedAI(c) {Reset();}

    uint32 timer_br;
    uint32 timer_ce;
    uint32 timer_c;
    uint32 timer_da;
    uint32 timer_sb;
    uint32 timer_ts;
    uint32 timer_fly_ss1;
    uint32 timer_fly_sb;
    uint32 timer_fly_ss2;
    uint32 timer_fly;
    uint32 procent;
    uint8 phase_switch;
    bool fly;
    bool phase;

    void Reset()
    {
        phase = 0;
        timer_br = 45000 + (rand() % 15000); //between 45 and 60 seconds
        timer_ce = 30000 + (rand() % 30000);  //between 30 and 60 seconds
        timer_c = 10000 + (rand() % 10000);  //between 10 and 20 seconds
        timer_da = 20000 + (rand() % 20000);  //between 20 and 40 seconds
        timer_sb = 30000 + (rand() % 10000);  //between 30 and 40 seconds
        timer_ts = 20000 + (rand() % 20000);  //between 20 and 40 seconds
        
        timer_fly = 50000;
        timer_fly_ss1 = 5000;
        timer_fly_sb = 6000;
        timer_fly_ss2 = 8000;

        phase_switch = 75;
        fly = false;
    };

    void Aggro(Unit* who)
    {
    }
        
        //Called the instant we kill another unit
    void KilledUnit();
        
    //Called the instant this unit dies
    void JustDied(Unit* Killer)
        {
    }
        
    //Called when we are attacked or when start attacking someone else
    void AttackStart(Unit* who)
    {
    }
   

    //Called every Map Update
    void UpdateAI(const uint32 diff)
    {
        if(fly == true)
        {
            if(timer_fly_ss1 < diff)
            { //Start summon of 1 skellet
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                if (!target) return;
                DoCast(target, SPELL_SUMMON_SKELL1);
            }else timer_fly_ss1 -= diff;

            if(timer_fly_ss2 < diff)
            { //Start summon of 4 skelletons
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                if (!target) return;
                DoCast(target, SPELL_RAIN_OF_BONES);
                for(int i=0; i<2; i++)
                {  //we need to summon twice 2 skelletons on 2 random players
                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                    if (!target) return;
                    DoCast(target, SPELL_SUMMON_SKELL2);
                }
            }else timer_fly_ss2 -= diff;

            if(timer_fly_sb < diff)
            { //Counting down sb, then do it every 1 sec
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_TOPAGGRO,1); //Dont know if this work, or if this still keeps targeting main tank, maybe reset threa after flight?
                DoCast(target, SPELL_SMOKING_BLAST);
                timer_fly_sb = 1000;
            }else timer_fly_sb -= diff;

            if(timer_fly < diff)
            { //start landing
                m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
                m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT + MOVEMENTFLAG_LEVITATING);
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LAND);
                fly = false;
            }else timer_fly -= diff;
            return;
        }

        procent = (m_creature->GetHealth()*100)/m_creature->GetMaxHealth();
        //if(procent =< phase_switch && procent !< 1);
                if(procent <= phase_switch && procent >= 1)
        { //FLy switch
            m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
            m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
            m_creature->AddUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT + MOVEMENTFLAG_LEVITATING);

            timer_fly_ss1 = 5000;
            timer_fly_sb = 6000;
            timer_fly_ss2 = 8000;
            timer_fly = 50000;
            phase_switch -= 25;
            fly = true;
        }

        if(timer_br < diff) 
        { //BELOWING ROAR
            DoCast(m_creature, SPELL_BELLOWING_ROAR);
            timer_br = 45000 + (rand() % 15000);
        } else timer_br -= diff;

        if(timer_ce < diff) 
        { //CHEARED EARTH
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if (!target) return;

            DoCast(target, SPELL_CHARRED_EARTH);
            timer_ce = 30000 + (rand() % 30000);
        } else timer_ce -= diff;

        if(timer_c < diff) 
        { //CLEAVE
            DoCast(m_creature->getVictim(), SPELL_SHADOWCLEAVE);
            timer_c = 10000 + (rand() % 10000);
        } else timer_c -= diff;

        if(timer_da < diff) 
        { //DISCTRACTING ASH
            DoCast(m_creature->getVictim(), SPELL_DISTRACTING_ASH);
            timer_da = 20000 + (rand() % 20000);
        } else timer_da -= diff;

        if(timer_sb < diff) 
        { //SMOLDERING BREATH
            DoCast(m_creature->getVictim(), SPELL_SMOLDERING_BREATH);
            timer_sb = 30000 + (rand() % 10000);
        } else timer_sb -= diff;

        if(timer_ts < diff) 
        { //TAIL_SWEEP

            Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 1);
            if(target && !m_creature->HasInArc(M_PI, target))
            {
                DoCast(target, SPELL_TAIL_SWEEP);
                timer_ts = 20000 + (rand() % 20000);
            }
        } else timer_ts -= diff;
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
    newscript->Name="boss_nightbane";
    newscript->GetAI = &GetAI_boss_nightbane;
    newscript->RegisterSelf();
}