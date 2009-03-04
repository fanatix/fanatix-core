/* Copyright  2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
 
/* ScriptData
SDName: boss_muru_entropius
SD%Complete: 60
SDComment: Needs Test XD
EndScriptData */
 
#include "precompiled.h"
#include "def_sunwell_plateau.h"
 
// Boss spells
#define ENRAGE 26662
#define DARKNESS 45996
#define SHADOW_PORTAL 40280
#define ENTROPIUS_EFFECT 46223
#define SUMMON_BERSERKER 46037
#define SUMMON_ENTROPIUS 46217
#define SUMMON_FURY_MAGE 46038
#define SUMMON_VOID_SENTINEL 45988
#define NEGATIVE_ENERGY_TRIGGER 46009
 
#define ENTROPIUS_DARKNESS 46269 //Need's core support
#define ENTROPIUS_VOID_ZONE 46263
#define ENTROPIUS_NEGATIVE_ENERGY_TRIGGER 46284 //Triggered spell need's core support
 
//Spells casting time (in msecs)
#define ENRAGE_TIMER 360000
#define DARKNESS_TIMER 45000
#define SUMMON_HUMANOIDS_TIMER 60000
#define SUMMON_VOID_SENTINEL_TIMER 30000
 
#define SINGULARITY_TIMER 60000
 
//Boss Summons
#define ID_SWB 25799
#define ID_SWM 25798
#define ID_DARK_FIEND 25744
#define ID_VOID_SENTINEL 25772
 
#define ID_SINGULARITY 25855
 
#define DARK_FIEND_AURA 45934
#define DARK_FIEND_DEBUFF 45944
 
//Distance of Mob Spawn
#define MOBMIN 5
#define MOBMAX 10
 
//Boss sounds
#define SOUND_CHANGE_PHASE 12560
#define SAY_ENTROPIUS_SUMMON	-1950000
 
//Shadow Portal Spawns
#define SP_SPAWN_Z 78
#define ID_WORLD_TRIGGER 12999
 
float ShadowPortalSpawn[5][3] =
{
        {1797.644,613.744,0.56290},
        {1822.956,646.437,4.40262},
        {1794.275,626.539,6.23522},
        {1809.726,646.494,5.01348},     
        {1798.854,638.952,5.62435}
};
 
uint32 DarkFiendsSpell[8] =
{
    46000,46001,46002,46003,
    46004,46005,46006,46007
};
 
Creature* WayPoint[5] = {NULL, NULL, NULL, NULL, NULL};
 
 
/* --- Global Var/Functions --- */
 
uint32 EnrageTimer;
uint32 DarknessTimer;
bool MuruDead = false;
 
void ResetEntro(ScriptedInstance* instance, Creature* Caller);
void ResetMuru(ScriptedInstance* instance, Creature* Caller);

float GetRandom()
    {
        float r = MOBMIN + rand()%(MOBMAX-MOBMIN+1);
            
        switch(rand()%2)
        {
                case 0:
                        r = -r;
                        break;
                default:
                        break;
        }
        
        return r;
    }
 
//Kills target
void KillMob(Creature* Mob)
{
        if(Mob && Mob->isAlive())
                Mob->DealDamage(Mob, Mob->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
}
 
 
/* --- M'uru AI --- */
    
struct MANGOS_DLL_DECL boss_muruAI : public Scripted_NoMovementAI
{
    boss_muruAI(Creature *c) : Scripted_NoMovementAI(c) { 
	pInstance = ((ScriptedInstance*)c->GetInstanceData());
	Reset(); 
   }
 
    bool AuraApplied;
    uint8 ShadowPortalN;
    uint32 SummonHumanoidsTimer;
    uint32 SummonVoidSentinelTimer;

    ScriptedInstance* pInstance; 
    
    void Reset()
    {
	 ShadowPortalN = 0;
        EnrageTimer = 1800000;
        DarknessTimer = DARKNESS_TIMER;
        m_creature->SetVisibility(VISIBILITY_ON);
        SummonHumanoidsTimer = SUMMON_HUMANOIDS_TIMER;
        SummonVoidSentinelTimer = SUMMON_VOID_SENTINEL_TIMER;
        m_creature->RemoveAurasDueToSpell(NEGATIVE_ENERGY_TRIGGER);
        
        if(m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        
	 MuruDead = false;
        /*for (uint8 i = 0; i < 5; i++)
        {
                KillMob(WayPoint[i]);
                WayPoint[i] = NULL;
        }*/
	
	//ResetEntro(pInstance, m_creature);

    }
    
    void Aggro(Unit *who)
    {
        if(!m_creature->HasAura(NEGATIVE_ENERGY_TRIGGER,0))
            m_creature->CastSpell(m_creature, NEGATIVE_ENERGY_TRIGGER, true);
    }
 
    void DamageTaken(Unit* done_by, uint32 &damage)
    {
        if (damage > m_creature->GetHealth() && done_by != m_creature)
        {
              damage = 0;
        	                
        	DoCast(m_creature, ENTROPIUS_EFFECT, true);
		//DoCast(m_creature, SUMMON_ENTROPIUS, true);


        	if (Creature* Entro = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_ENTROPIUS))))
        	{
               	Entro->SetVisibility(VISIBILITY_ON);
               	Entro->setFaction(14);

		  	DoScriptText(SAY_ENTROPIUS_SUMMON, m_creature);

		  	if(Entro) Entro->AI()->AttackStart(m_creature->getVictim());
		}

		DoPlaySoundToSet(m_creature, SOUND_CHANGE_PHASE);

        	m_creature->SetVisibility(VISIBILITY_OFF);
        	m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        	/*for (uint8 i = 0; i < 5; i++)
        	{
        	        KillMob(WayPoint[i]);
        	        WayPoint[i] = NULL;
        	}
        	ShadowPortalN = 0;*/
	 	MuruDead = true;
      }
    }
    
    void KilledUnit(Unit *Victim) {}
    
    void UpdateAI(const uint32 diff)
    {       
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
                return;
        
        //Goes in enrage
        if(EnrageTimer < diff)
        {
                m_creature->CastSpell(m_creature, ENRAGE, true);
                EnrageTimer = 20000;
        }
        else EnrageTimer -= diff;
        
        //Cast Darkness
        if(DarknessTimer < diff)
        {
                m_creature->CastSpell(m_creature, DARKNESS, false);
                
                for(uint8 i = 0; i < 8; i++) 
                        m_creature->CastSpell(m_creature, DarkFiendsSpell[i], true);
                
                DarknessTimer = DARKNESS_TIMER;
        }
        else DarknessTimer -= diff;
        
        //Spawns Shadow portals and then one Void Sentinel
        if(SummonVoidSentinelTimer < diff)
        {
                float SP_SPAWN_X, SP_SPAWN_Y, SP_SPAWN_O = 0;
                
                if(ShadowPortalN < 5) //Not every portal is spawned
                {
                        SP_SPAWN_X = ShadowPortalSpawn[ShadowPortalN][0];
                        SP_SPAWN_Y = ShadowPortalSpawn[ShadowPortalN][1];
                        SP_SPAWN_O = ShadowPortalSpawn[ShadowPortalN][2];
                                
                        WayPoint[ShadowPortalN] = m_creature->SummonCreature(ID_WORLD_TRIGGER, SP_SPAWN_X, SP_SPAWN_Y, SP_SPAWN_Z, SP_SPAWN_O, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 10000);
                        if(WayPoint[ShadowPortalN])
                        {
                                WayPoint[ShadowPortalN]->StopMoving();
                                WayPoint[ShadowPortalN]->GetMotionMaster()->Clear(false); // No moving!
                                WayPoint[ShadowPortalN]->CastSpell(WayPoint[ShadowPortalN], SHADOW_PORTAL, true);
                        }
                        ShadowPortalN++;
                }
                else // Choose 1 random portal in wich spawn Void Sentinel
                {
                        uint8 RandSpwn = rand()%5;
                        
                        SP_SPAWN_X = ShadowPortalSpawn[RandSpwn][0];
                        SP_SPAWN_Y = ShadowPortalSpawn[RandSpwn][1];
                        SP_SPAWN_O = ShadowPortalSpawn[RandSpwn][2];
                }
                
                Creature* VoidSentinel = m_creature->SummonCreature(ID_VOID_SENTINEL, SP_SPAWN_X, SP_SPAWN_Y, m_creature->GetPositionZ(), SP_SPAWN_O, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 10000);
                if(VoidSentinel) VoidSentinel->AI()->AttackStart(m_creature->getVictim());
                
                SummonVoidSentinelTimer = SUMMON_VOID_SENTINEL_TIMER + rand()%15000;
        }
        else SummonVoidSentinelTimer -= diff;
        
        //Summon 6 humanoids every min (1mage & 2berserkers)
        /*if(SummonHumanoidsTimer < diff)
        {
                for(uint8 i = 0; i < 6; i++)
                {
                        if((i == 2) | (i == 5))
                                m_creature->CastSpell(m_creature, SUMMON_FURY_MAGE, true);
                        else
                                m_creature->CastSpell(m_creature, SUMMON_BERSERKER, true);
                }
                SummonHumanoidsTimer = SUMMON_HUMANOIDS_TIMER;
        }      
        else SummonHumanoidsTimer -= diff;*/
    }
};
 
 
/* --- Entropius AI --- */
 
struct MANGOS_DLL_DECL boss_entropiusAI : public ScriptedAI
{
    boss_entropiusAI(Creature *c) : ScriptedAI(c) 
    { 
       pInstance = ((ScriptedInstance*)c->GetInstanceData());
	Reset(); 
     }

    ScriptedInstance* pInstance;
    uint32 SingularityTimer;

    
    void Reset()
    {
        EnrageTimer = 1800000;
        DarknessTimer = DARKNESS_TIMER;
        SingularityTimer = SINGULARITY_TIMER;

	 m_creature->SetVisibility(VISIBILITY_OFF);
	 m_creature->setFaction(35);

	 ResetMuru(pInstance, m_creature);
    }
    
    void Aggro(Unit *who)
    {
        if(!m_creature->HasAura(ENTROPIUS_NEGATIVE_ENERGY_TRIGGER,0))
            m_creature->CastSpell(m_creature, ENTROPIUS_NEGATIVE_ENERGY_TRIGGER, true);
    }
    
    void JustDied(Unit* Killer) 
    {
	 if(MuruDead) {
		GameObject* Gate = GameObject::GetGameObject(*m_creature, pInstance->GetData64(DATA_GATE_4));
        	Gate->SetGoState(0);
	
	 if(pInstance)
        	pInstance->SetData(DATA_MURU_EVENT, DONE);
	}
    }
    void KilledUnit(Unit *Victim) {}
            
    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
                return;
        
        //Goes in enrage
        if(EnrageTimer < diff)
        {
                m_creature->CastSpell(m_creature, ENRAGE, true);
                EnrageTimer = 20000;
        }
        else EnrageTimer -= diff;
        
        //Cast Darkness
        if(DarknessTimer < diff)
        {
                m_creature->CastSpell(SelectUnit(SELECT_TARGET_RANDOM, 0), ENTROPIUS_VOID_ZONE, false);
                DarknessTimer = DARKNESS_TIMER;
        }
        else DarknessTimer -= diff;
        
        //Summons Singularity
        if(SingularityTimer < diff)
        {
                Unit* sTarget = SelectUnit(SELECT_TARGET_RANDOM, 0);
                
                Creature* Singularity = m_creature->SummonCreature(ID_SINGULARITY, sTarget->GetPositionX(), sTarget->GetPositionY(), sTarget->GetPositionZ(), m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, SINGULARITY_TIMER-5000);
                if(Singularity)
                {
                        Singularity->CastSpell(Singularity, 46228, true);  //Sigularity aura
                        Singularity->AI()->AttackStart(sTarget);
                }              
                SingularityTimer = SINGULARITY_TIMER;
        }
        else SingularityTimer -= diff;
        
        DoMeleeAttackIfReady();
    }
};
 
 
/* --- Dark Fiend AI --- */
 
struct MANGOS_DLL_DECL dark_fiendAI : public ScriptedAI
{
    dark_fiendAI(Creature *c) : ScriptedAI(c) { Reset(); }
 
    uint32 SingularityTimer;
    
    void Reset() {}
    
    void Aggro(Unit *who)
    {
        if(!m_creature->HasAura(DARK_FIEND_AURA,0))
            m_creature->CastSpell(m_creature, DARK_FIEND_AURA, true);
    }
    
    void JustDied(Unit* Killer) {}
    void KilledUnit(Unit *Victim) {}
            
    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
                return;
        
        if( m_creature->isAttackReady() && !m_creature->IsNonMeleeSpellCasted(false))
        {
                //If we are within range melee the target
                if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
                {
                        m_creature->CastSpell(m_creature->getVictim(), DARK_FIEND_DEBUFF, true);
                        m_creature->AttackerStateUpdate(m_creature->getVictim());
                        //m_creature->resetAttackTimer();
                }
        }
    }
};
 

void ResetEntro(ScriptedInstance* instance, Creature* Caller)
{
	 ScriptedInstance* pInstance;
	 pInstance = instance;
	 Creature* Entro = ((Creature*)Unit::GetUnit(*Caller, pInstance->GetData64(DATA_ENTROPIUS)));
        if (Entro)
        {
             if (!Entro->isAlive())
                 Entro->Respawn();

                ((boss_entropiusAI*)Entro->AI())->Reset();
                ((boss_entropiusAI*)Entro->AI())->EnterEvadeMode();
                Entro->GetMotionMaster()->MoveTargetedHome();
	 }
}

void ResetMuru(ScriptedInstance* instance, Creature* Caller)
{
	 ScriptedInstance* pInstance;
	 pInstance = instance;
	 Creature* Muru = ((Creature*)Unit::GetUnit(*Caller, pInstance->GetData64(DATA_MURU)));
        if (Muru)
        {
             if (!Muru->isAlive())
             {    Muru->Respawn();

                ((boss_muruAI*)Muru->AI())->Reset();
                ((boss_muruAI*)Muru->AI())->EnterEvadeMode();
                Muru->GetMotionMaster()->MoveTargetedHome(); 
		}
	 }
}

/* Adding Scripts */
 
CreatureAI* GetAI_boss_muru(Creature *_Creature)
{
    return new boss_muruAI(_Creature);
}
 
CreatureAI* GetAI_boss_entropius(Creature *_Creature)
{
    return new boss_entropiusAI(_Creature);
}
 
CreatureAI* GetAI_dark_fiend(Creature *_Creature)
{
    return new dark_fiendAI(_Creature);
}
 
void AddSC_boss_muru_entropius()
{
    Script *newscript;
        
    newscript = new Script;
    newscript->Name="boss_muru";
    newscript->GetAI = &GetAI_boss_muru;
    newscript->RegisterSelf();
 
    newscript = new Script;
    newscript->Name="boss_entropius";
    newscript->GetAI = &GetAI_boss_entropius;
    newscript->RegisterSelf();
 
    newscript = new Script;
    newscript->Name="dark_fiend";
    newscript->GetAI = &GetAI_dark_fiend;
    newscript->RegisterSelf();
}
