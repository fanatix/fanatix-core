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

/* Copyright (C) 2009
Paradox Costom Raidboss
Coded FBPBM
*/

/* ScriptData
SDName: boss_Terrordar_der_Peiniger
SD%Complete: Stats Von Maggi benutzen !!! 
SDComment:  Raidboss / Loot : 2 Abzeichen der gerechtigkeit , 3 x Random epic rezept drop
SDCategory: Schergrat /Resparwn : alle 6 Stunden 
EndScriptData */

/* ContentData
boss_Terrordar_der_Peiniger
EndContentData */

#include "precompiled.h"
#include "sc_creature.h"


#define SAY_AGGRO                    "Ihr Sterblichen wagt es mich zu stören ???"
#define SAY_SLAY1                    "Der erste beißt ins Grass"
#define SAY_SLAY2                    "Der Zweite gesellt sich zu den Toten"
#define SAY_SLAY3                    "Ihr alle werdet Sterben !"
#define SAY_DEATH                    "Wie ?! ich bin doch ein Gott ..."


#define SPELL_CHAIN_LIGHTNING       44318
#define SPELL_RAINOFFIRE            28794
#define SPELL_ENVELOPING_WINDS      31718
#define SPELL_ARCANE_EXPLOSION      38197
#define H_SPELL_ARCANE_EXPLOSION    40425
#define SPELL_FIREBALL_BARRAGE      30282
#define SPELL_DISTRACTING_ASH       30130

struct MANGOS_DLL_DECL boss_Terrordar_der_PeinigerAI : public ScriptedAI
{
    boss_Terrordar_der_PeinigerAI(Creature *c) : ScriptedAI(c) 
{
 Reset();
    }


    uint32 RainOfFire_Timer;
    uint32 ChainLightningTimer;
    uint32 EnvelopingWinds_Timer;
    uint32 Enrage_Timer;
	uint32 Fireball_Timer;
	uint32 timer_da;


    void Reset()
    {
        RainOfFire_Timer = 28000;
        ChainLightningTimer = 12000;
        EnvelopingWinds_Timer = 9000;
		timer_da = 2000 + (rand() % 2000);
		Fireball_Timer = 3000;
        Enrage_Timer = 900000;
}

    void Aggro(Unit *who)
    {
        DoYell(SAY_AGGRO,LANG_UNIVERSAL, NULL);
    }

    void KilledUnit()
    {
        switch(rand()%3)
        {
            case 0: DoYell(SAY_SLAY1,LANG_UNIVERSAL, NULL); break;
            case 1: DoYell(SAY_SLAY2,LANG_UNIVERSAL, NULL); break;
            case 2: DoYell(SAY_SLAY3,LANG_UNIVERSAL, NULL); break;
        }
    }

    void JustDied(Unit* Killer)
    {
        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
    }

    void UpdateAI(const uint32 diff)
    {
        
if (Enrage_Timer < diff)
        {
            DoCast(m_creature, H_SPELL_ARCANE_EXPLOSION ,SPELL_ARCANE_EXPLOSION);
            DoCast(m_creature, H_SPELL_ARCANE_EXPLOSION ,SPELL_ARCANE_EXPLOSION);
            DoCast(m_creature, H_SPELL_ARCANE_EXPLOSION ,SPELL_ARCANE_EXPLOSION);
            Enrage_Timer = 900000+rand()%5000;
}else Enrage_Timer -=diff;

        if (RainOfFire_Timer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target) DoCast(target,SPELL_RAINOFFIRE);
            RainOfFire_Timer = 28000;
        } else RainOfFire_Timer -= diff;

        if (ChainLightningTimer < diff)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(target, SPELL_CHAIN_LIGHTNING);
            ChainLightningTimer = 12000;
        }else ChainLightningTimer -= diff;
      
 if(timer_da < diff) 
        { 
            DoCast(m_creature->getVictim(), SPELL_DISTRACTING_ASH);
            timer_da = 20000 + (rand() % 20000);
        } else timer_da -= diff;

       if (Fireball_Timer < diff) 
{ 
if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
DoCast(target,SPELL_FIREBALL_BARRAGE); 
 
Fireball_Timer = 3000; 
}else Fireball_Timer -= diff; 

 if (EnvelopingWinds_Timer < diff)
        {
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target, SPELL_ENVELOPING_WINDS);
            EnvelopingWinds_Timer = 10000+rand()%5000;
        }else EnvelopingWinds_Timer -=diff;

        DoMeleeAttackIfReady();
}
    
};

CreatureAI* GetAI_boss_Terrordar_der_Peiniger(Creature *_Creature)
{
    return new boss_Terrordar_der_PeinigerAI (_Creature);
}

void AddSC_boss_Terrordar_der_Peiniger()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_ambassador_hellmaw";
    newscript->GetAI = &GetAI_boss_Terrordar_der_Peiniger;
    newscript->RegisterSelf();
}
