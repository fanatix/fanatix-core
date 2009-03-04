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
SDName: Boss_Wage_Winterchill
SD%Complete: 85
SDComment:
SDCategory: Caverns of Time, Mount Hyjal
EndScriptData */

#include "precompiled.h"
#include "sc_creature.h"
#include "def_hyjal.h"

//Texte
#define SAY_AGGRO               "Ihr wagt es der dunklen Legion entgegen zu treten!" 
#define SAY_SLAY1               "Spürt meine Macht!"
#define SAY_SLAY2               "Ihr werdet mich nie vernichten!"
#define SAY_SLAY3               "HAHAHAHA Seid ihr schwach!"
#define SAY_DEATH               "Ihr habt es echt geschafft mich zu töten, verdammt!" 
#define YELL_BERSERK            -1580029
//Spells
#define SPELL_BERSERK			26662
#define SPELL_DEATH_DECAY		31258
#define SPELL_FROST_ARMOR		31256
#define SPELL_FROST_NOVA		31250
#define SPELL_ICEBOLT			31249 

struct MANGOS_DLL_DECL boss_rage_winterchillAI : public ScriptedAI
{
    boss_rage_winterchillAI(Creature *c) : ScriptedAI(c)
{
 Reset();
    }

	uint32 BerserkTimer;
    uint32 DeathDecayTimer;
    uint32 FrostArmorTimer;
    uint32 FrostNovaTimer;
	uint32 Ice_BoltTimer;
	
    void Reset()
    {
		BerserkTimer = 600000; // 10 min 
        DeathDecayTimer = 60000;  // 2,5 min    
        FrostArmorTimer =  8000; // 8 sec 
		FrostNovaTimer =  90000;  // 1,5 min 
		Ice_BoltTimer = 30000; // 30 sec
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

	 if (BerserkTimer < diff)
        {
            DoScriptText(YELL_BERSERK, m_creature); 
            DoCast(m_creature,SPELL_BERSERK);
            BerserkTimer = 600000;
        }
        else BerserkTimer -= diff;

		if(DeathDecayTimer < diff)
            {
                 DoCast(m_creature,SPELL_DEATH_DECAY);
				 DeathDecayTimer = 60000;
            }else DeathDecayTimer -= diff;

            if(FrostNovaTimer < diff)
            {
               DoCast(m_creature,SPELL_FROST_NOVA);
				 FrostNovaTimer = 90000;

            }else FrostNovaTimer -= diff;  

			if (FrostArmorTimer < diff )
                {
                  DoCast(m_creature,SPELL_FROST_ARMOR);
                    FrostArmorTimer = 8000;

                }else FrostArmorTimer -= diff;

			if (Ice_BoltTimer < diff) 
{ 
if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
DoCast(target,SPELL_ICEBOLT); 
 
Ice_BoltTimer = 30000; 
}else Ice_BoltTimer -= diff; 
			

        DoMeleeAttackIfReady();
}
    
};

CreatureAI* GetAI_boss_rage_winterchill(Creature *_Creature)
{
    return new boss_rage_winterchillAI (_Creature);
}

void AddSC_boss_rage_winterchill()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_rage_winterchill";
    newscript->GetAI = &GetAI_boss_rage_winterchill;
    newscript->RegisterSelf();
}
