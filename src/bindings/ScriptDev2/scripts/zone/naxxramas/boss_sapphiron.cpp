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
SDName: Boss_Sapphiron
SD%Complete: 0
SDComment: Place Holder
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "def_naxxramas.h"

//Spells
#define SPELL_ICEBOLT				28522
#define SPELL_FROST_BREATH			29318
#define SPELL_FROST_AURA			28531
#define SPELL_LIFE_DRAIN			28542
#define SPELL_CHILL					28547
#define SPELL_BESERK                26662
#define IMMUNE_SCHOOL_FROST         38697       


//  TO DO:
// 	- Movement of the Chill Spell
//	- Bomb Save Place
//  - Timer vor Chill
//  - Sapph Mvement in the center before Flying

struct MANGOS_DLL_DECL boss_sapphironAI : public ScriptedAI
{
	boss_sapphironAI(Creature* c) : ScriptedAI(c)
	{
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
		Reset();	
	}
    ScriptedInstance *pInstance;
	
	uint32 Icebolt_Count;
	uint32 Icebolt_Timer;
	uint32 FrostBreath_Timer;
	uint32 FrostAura_Timer;
	uint32 LifeDrain_Timer;
	uint32 Chill_Timer;
	uint32 Fly_Timer;
	uint32 Fly2_Timer;
	uint32 phase;
	bool InCombat;
	bool landoff;
	uint32 land_Timer;
	
	void Reset()
	{
		FrostAura_Timer = 2000;
		LifeDrain_Timer = 24000;
		Chill_Timer = 20000;
		Fly_Timer = 45000; // Sapphi heb beim ersten mal schon nach 45 Sekunden ab
		Icebolt_Timer = 4000;
		InCombat = false;
		phase = 1;
		Icebolt_Count = 0;
		landoff = false;
		
        if(m_creature->GetUnitMovementFlags() == (MOVEMENTFLAG_ONTRANSPORT + MOVEMENTFLAG_LEVITATING))
        {
            m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT + MOVEMENTFLAG_LEVITATING);
		    m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LAND);	
        }
		m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_FROST, true);
        
        if(pInstance)
        {
            pInstance->SetData(ENCOUNT_SAPPHIRON, 0);
            HandleDoors(GO_FROSTWYRM_WATERFALL, 1);
        }

		//m_creature->ApplySpellMod(SPELL_FROST_AURA, SPELLMOD_DURATION, -1);
	}

    void JustDied(Unit* Killer)
    {
        if(pInstance)
        {
            pInstance->SetData(ENCOUNT_SAPPHIRON, 2);
            HandleDoors(GO_FROSTWYRM_WATERFALL, 0);
        }
    }
	
	void Aggro(Unit *who)
	{
        if (!InCombat)
        {
            DoZoneInCombat();
            InCombat = true;
            if(pInstance)
            {
                pInstance->SetData(ENCOUNT_SAPPHIRON, 1);
            }
        }	
	}

    //Open/Closes Doors 
    void HandleDoors(uint32 identifier, uint32 doorstate)
    {
        if(pInstance)
        {
            GameObject *door;
            door = GameObject::GetGameObject((*m_creature),pInstance->GetData64(identifier));
            if (door)
            {
                switch (doorstate)
                {
                case 0: //open
                    door->SetUInt32Value(GAMEOBJECT_FLAGS, 33);
                    door->SetUInt32Value(GAMEOBJECT_BYTES_1, 0);
                    break;
                case 1: //close
                    door->SetUInt32Value(GAMEOBJECT_FLAGS, 0);
                    door->SetUInt32Value(GAMEOBJECT_BYTES_1, 1);
                    break;
                }
            }
            door = NULL;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget())
            return;
		
		if(m_creature->getVictim() && m_creature->isAlive())
        {
			if(phase == 1)//Phase 1 Kampf am Boden
			{
				if(FrostAura_Timer < diff)  // Frostaura eigendlich auf alle aber hier nur auf einen
				{
					DoCast(m_creature,SPELL_FROST_AURA);
					FrostAura_Timer = 5000; // Frostaura erneuerung aller 5 Sekunden
				}
				else FrostAura_Timer -= diff;
			
				if(LifeDrain_Timer < diff) // Lifedrain
				{
					Unit* target = NULL;
					target = SelectUnit(SELECT_TARGET_RANDOM,0);

					DoCast(target,SPELL_LIFE_DRAIN);
					LifeDrain_Timer = 24000;
				}
				else LifeDrain_Timer -= diff;
			
				if(Chill_Timer < diff) // "ab" Chillen aller 20 Sekunden
				{
					Unit* target = NULL;
					target = SelectUnit(SELECT_TARGET_RANDOM,0);

					DoCast(target,SPELL_CHILL);
					Chill_Timer = 20000;
				}
				else Chill_Timer -= diff;

				if(Fly_Timer < diff && !((m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 10))
				{
					// Sapphie hebt aller 67 Sekunden ab und geht somit in die Flugphase (phase 2)
					phase = 2;
					m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
                    m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
				    m_creature->AddUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT + MOVEMENTFLAG_LEVITATING);
                    (*m_creature).GetMotionMaster()->Clear(false);
                    (*m_creature).GetMotionMaster()->MoveIdle();
                    DoCast(m_creature,11010);
                    m_creature->SetHover(true);
                    DoCast(m_creature,18430);
                    Icebolt_Timer = 4000;
                    Icebolt_Count = 0;
                    landoff = false;
                }else Fly_Timer -= diff;    

                DoMeleeAttackIfReady();
			}	

			if (phase == 2) // Phase 2 Flugphase Spells usw
			{
				//if(Icebolt_Timer < diff && Icebolt_Count < 5) // Icebolt mit counter
				//{
				//	Unit* target = NULL;

				//	target = SelectUnit(SELECT_TARGET_RANDOM,0);

				//	DoCast(target,SPELL_ICEBOLT);
				//	Icebolt_Count ++; 
				//	Icebolt_Timer = 4000;
				//	if(Icebolt_Count == 5) // wenn alle 5 gespellt wurden begint der countdown für die Bombe :D
				//	{
				//		DoTextEmote("takes a deep breath...",NULL);
				//		FrostBreath_Timer = 6000;	
				//	}
				//}			
				//else Icebolt_Timer -= diff;

				//if(Icebolt_Count == 5 && !landoff) // Bombe wird gezündet
				//{
				//	if(FrostBreath_Timer < diff )
				//	{
				//		DoCast(m_creature->getVictim(),SPELL_FROST_BREATH);
				//		land_Timer = 1000;
				//		landoff = true; // landemanöver einleiten
				//	}
				//	else FrostBreath_Timer -= diff;
				//}

                if(Icebolt_Timer < diff && Icebolt_Count < 10) // Icebolt mit counter
				{
					Unit* target = NULL;
					target = SelectUnit(SELECT_TARGET_RANDOM,0);
					DoCast(target,SPELL_ICEBOLT);
					Icebolt_Count ++; 
					Icebolt_Timer = 2000;
                    if(Icebolt_Count >= 9)
                    {
                        landoff = true;
                        land_Timer = 1000;
                    }
				}			
				else Icebolt_Timer -= diff;

				if(landoff)
				{
					if(land_Timer < diff) // landen und wieder in die Bodenkampfphase wechseln wenn Bombe gezündet
					{
					phase = 1;
					m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT + MOVEMENTFLAG_LEVITATING);
				    m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LAND);          
                    (*m_creature).GetMotionMaster()->MoveChase(m_creature->getVictim());
                    Fly_Timer = 67000;
                    }else land_Timer -= diff;
                }
			}
		}
	}
};

CreatureAI* GetAI_boss_sapphiron(Creature *_Creature)
{
    return new boss_sapphironAI (_Creature);
}

void AddSC_boss_sapphiron()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_sapphiron";
    newscript->GetAI = GetAI_boss_sapphiron;
    newscript->RegisterSelf();
}


