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
SDName: Boss_Majordomo_Executus
SD%Complete: 90
SDComment: Teleport not implemented
SDCategory: Molten Core
EndScriptData */

#include "precompiled.h"
#include "def_molten_core.h"

#define SAY_AGGRO           -1409003
#define SAY_SPAWN           -1409004
#define SAY_SLAY            -1409005
#define SAY_SPECIAL         -1409006
#define SAY_DEFEAT          -1409007

#define SOUND_AGGRO     8035
#define SAY_SUMMON_MAJ      -1409008
#define SAY_ARRIVAL2_MAJ    -1409010
#define SOUND_SPAWN     8039

#define SPAWN_RAG_X         829.07147
#define SPAWN_RAG_Y         -849.46392
#define SPAWN_RAG_Z         -´229.5953
#define SPAWN_RAG_O         1.157160

#define SOUND_DEFEAT    8038
#define SPELL_MAGIC_REFLECTION      20619
#define SPELL_DAMAGE_REFLECTION     21075
#define SPELL_BLASTWAVE             20229
#define SPELL_AEGIS                 20620                   //This is self casted whenever we are below 50%
#define SPELL_TELEPORT              20618
#define SPELL_SUMMON_RAGNAROS       19774

#define SOUND_KILL      8037
#define ENTRY_FLAMEWALKER_HEALER    11663
#define ENTRY_FLAMEWALKER_ELITE     11664

#define SOUND_SPECIAL   8036
#define SOUND_SUMMON    8040
#define SOUND_ARRIVAL1  8041

#define GOSSIP_ITEM_DOMO   "Let me speak to your Master, Servant"

//ADDS right site
#define ADD_PRIEST_R1_X 756.115
#define ADD_PRIEST_R1_Y -1222.127
#define ADD_PRIEST_R1_Z -119.700
#define ADD_PRIEST_R1_O 1.839

#define ADD_PRIEST_R2_X 756.338
#define ADD_PRIEST_R2_Y -1212.633
#define ADD_PRIEST_R2_Z -119.650
#define ADD_PRIEST_R2_O 1.796

#define ADD_ELITE_R1_X 770.432
#define ADD_ELITE_R1_Y -1204.971
#define ADD_ELITE_R1_Z -119.606
#define ADD_ELITE_R1_O 1.867

#define ADD_ELITE_R2_X 761.817
#define ADD_ELITE_R2_Y -1215.765
#define ADD_ELITE_R2_Z -119.636
#define ADD_ELITE_R2_O 1.792

//ADDS left site
#define ADD_PRIEST_L1_X 744.201
#define ADD_PRIEST_L1_Y -1227.621
#define ADD_PRIEST_L1_Z -119.633
#define ADD_PRIEST_L1_O 1.792

#define ADD_PRIEST_L2_X 731.172
#define ADD_PRIEST_L2_Y -1224.489
#define ADD_PRIEST_L2_Z -119.948
#define ADD_PRIEST_L2_O 1.792

#define ADD_ELITE_L1_X 736.566
#define ADD_ELITE_L1_Y -1222.606
#define ADD_ELITE_L1_Z -119.633
#define ADD_ELITE_L1_O 1.751

#define ADD_ELITE_L2_X 721.837
#define ADD_ELITE_L2_Y -1216.640
#define ADD_ELITE_L2_Z -119.979
#define ADD_ELITE_L2_O 1.751

struct MANGOS_DLL_DECL boss_majordomoAI : public ScriptedAI
{
    boss_majordomoAI(Creature *c) : ScriptedAI(c)
	{
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
		Reset();
    }
    ScriptedInstance *pInstance;

    uint32 MagicReflection_Timer;
    uint32 DamageReflection_Timer;
    uint32 Blastwave_Timer;

	bool Speech, Death, Summon, Teleport, SaySpawn;
	uint32 Speech_Timer;
	uint32 Death_Timer;
	uint32 Summon_Timer;
	uint32 Teleport_Timer;
	uint32 CheckFlamewaker_Timer;

	uint32 Reset_Count;

	Creature *EliteR1, *EliteR2, *EliteL1, *EliteL2, *PriestR1, *PriestR2, *PriestL1, *PriestL2;

    void Reset()
    {
		Reset_Count++;
        MagicReflection_Timer =  30000;      //Damage reflection first so we alternate
        DamageReflection_Timer = 15000;
        Blastwave_Timer = 10000;
		
		Speech = Death = Summon = Teleport = SaySpawn = false;
		if(Reset_Count != 1)
		{
			if(EliteR1->isDead())
				EliteR1 = m_creature->SummonCreature(11664,ADD_ELITE_R1_X,ADD_ELITE_R1_Y,ADD_ELITE_R1_Z,ADD_ELITE_R1_O,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,1200000);
			if(EliteR2->isDead())
				EliteR2 = m_creature->SummonCreature(11664,ADD_ELITE_R2_X,ADD_ELITE_R2_Y,ADD_ELITE_R2_Z,ADD_ELITE_R2_O,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,1200000);
			if(EliteL1->isDead())
				EliteL1 = m_creature->SummonCreature(11664,ADD_ELITE_L1_X,ADD_ELITE_L1_Y,ADD_ELITE_L1_Z,ADD_ELITE_L1_O,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,1200000);
			if(EliteL2->isDead())
				EliteL2 = m_creature->SummonCreature(11664,ADD_ELITE_L2_X,ADD_ELITE_L2_Y,ADD_ELITE_L2_Z,ADD_ELITE_L2_O,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,1200000);
			if(PriestR1->isDead())
				PriestR1 = m_creature->SummonCreature(11662,ADD_PRIEST_R1_X,ADD_PRIEST_R1_Y,ADD_PRIEST_R1_Z,ADD_PRIEST_R1_O,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,1200000);
			if(PriestR2->isDead())
				PriestR2 = m_creature->SummonCreature(11662,ADD_PRIEST_R2_X,ADD_PRIEST_R2_Y,ADD_PRIEST_R2_Z,ADD_PRIEST_R2_O,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,1200000);
			if(PriestL1->isDead())
				PriestL1 = m_creature->SummonCreature(11662,ADD_PRIEST_L1_X,ADD_PRIEST_L1_Y,ADD_PRIEST_L1_Z,ADD_PRIEST_L1_O,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,1200000);
			if(PriestL2->isDead())
				PriestL2 = m_creature->SummonCreature(11662,ADD_PRIEST_L2_X,ADD_PRIEST_L2_Y,ADD_PRIEST_L2_Z,ADD_PRIEST_L2_O,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,1200000);
		}

		Speech_Timer=23000;
		Death_Timer=30000;
		Summon_Timer=11000;
		Teleport_Timer=40000;
		CheckFlamewaker_Timer = 2000;
    }

	void BeginEvent(Player* target)
    {
        DoScriptText(SAY_SUMMON_MAJ, m_creature);
        DoPlaySoundToSet(m_creature,SOUND_SUMMON);

		Summon=true;
    }

	void MoveInLineOfSight(Unit *who)
    {
		if(m_creature->getFaction() == 35)
			return;

        if( !m_creature->getVictim() && who->isTargetableForAttack() && ( m_creature->IsHostileTo( who )) && who->isInAccessablePlaceFor(m_creature) && !SaySpawn)
        {
			DoScriptText(SAY_SPAWN, m_creature);
			DoPlaySoundToSet(m_creature,SOUND_SPAWN);
			SaySpawn = true;
        }

		float attackRadius = m_creature->GetAttackDistance(who);
		if( m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->IsWithinLOSInMap(who))
        {
			if(who->getFaction()== 54 || who->getFaction() == 14)
				return;
			who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
            AttackStart(who);
        }
    }


    void KilledUnit(Unit* victim)
    {
        if (rand()%5)
            return;

        DoScriptText(SAY_SLAY, m_creature);
    }

    void Aggro(Unit *who)
    {
		DoScriptText(SAY_AGGRO, m_creature);
        DoPlaySoundToSet(m_creature,SOUND_AGGRO);
    }

    void UpdateAI(const uint32 diff)
    {
		if(pInstance->GetData(DATA_ALL_BOSSES_DEAD) == 1 && Reset_Count == 1)
		{
			m_creature->SetVisibility(VISIBILITY_ON);
			m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			m_creature->setFaction(54);

			EliteR1 = m_creature->SummonCreature(11664,ADD_ELITE_R1_X,ADD_ELITE_R1_Y,ADD_ELITE_R1_Z,ADD_ELITE_R1_O,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,1200000);
			EliteR2 = m_creature->SummonCreature(11664,ADD_ELITE_R2_X,ADD_ELITE_R2_Y,ADD_ELITE_R2_Z,ADD_ELITE_R2_O,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,1200000);
			EliteL1 = m_creature->SummonCreature(11664,ADD_ELITE_L1_X,ADD_ELITE_L1_Y,ADD_ELITE_L1_Z,ADD_ELITE_L1_O,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,1200000);
			EliteL2 = m_creature->SummonCreature(11664,ADD_ELITE_L2_X,ADD_ELITE_L2_Y,ADD_ELITE_L2_Z,ADD_ELITE_L2_O,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,1200000);
			PriestR1 = m_creature->SummonCreature(11662,ADD_PRIEST_R1_X,ADD_PRIEST_R1_Y,ADD_PRIEST_R1_Z,ADD_PRIEST_R1_O,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,1200000);
			PriestR2 = m_creature->SummonCreature(11662,ADD_PRIEST_R2_X,ADD_PRIEST_R2_Y,ADD_PRIEST_R2_Z,ADD_PRIEST_R2_O,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,1200000);
			PriestL1 = m_creature->SummonCreature(11662,ADD_PRIEST_L1_X,ADD_PRIEST_L1_Y,ADD_PRIEST_L1_Z,ADD_PRIEST_L1_O,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,1200000);
			PriestL2 = m_creature->SummonCreature(11662,ADD_PRIEST_L2_X,ADD_PRIEST_L2_Y,ADD_PRIEST_L2_Z,ADD_PRIEST_L2_O,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,1200000);
			Reset_Count++;
		}

		if(Teleport){
			if(Teleport_Timer < diff){
				
				m_creature->SetUInt32Value(UNIT_NPC_FLAGS,1);

				m_creature->GetMotionMaster()->Clear();
				//m_creature->GetMotionMaster()->MovePoint(0, SPAWN_RAG_X, SPAWN_RAG_Y, SPAWN_RAG_Z);
				
				Teleport=false;
			}else Teleport_Timer -= diff;
		}

		if(Summon){
			if(Summon_Timer == 10000)
				DoCast(m_creature,SPELL_SUMMON_RAGNAROS);
			if(Summon_Timer < diff){
				
				m_creature->SummonCreature(11502,837.919,-833.090,-231.896,3.874637,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,12000000);

				Speech=true;
				Summon=false;
			}else Summon_Timer -= diff;
		}


		if(Speech){
			if (Speech_Timer < diff){
				DoScriptText(SAY_ARRIVAL2_MAJ, m_creature);
				DoPlaySoundToSet(m_creature,SOUND_ARRIVAL1);
				Death=true;
				Speech=false;
			}else Speech_Timer -= diff;
		}

		if(Death){
			if (Death_Timer <diff){
				m_creature->setDeathState(JUST_DIED);
				m_creature->RemoveCorpse();
				Death=false;
			}else Death_Timer -= diff;
		}

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() || (m_creature->getFaction() == 35))
            return;

		
        //Cast Ageis if less than 50% hp
        if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 50)
        {
            DoCast(m_creature,SPELL_AEGIS);
        }

        //MagicReflection_Timer
        if (MagicReflection_Timer < diff)
        {
            DoCast(m_creature, SPELL_MAGIC_REFLECTION);

            //60 seconds until we should cast this agian
            MagicReflection_Timer = 30000;
        }else MagicReflection_Timer -= diff;

        //DamageReflection_Timer
        if (DamageReflection_Timer < diff)
        {
            DoCast(m_creature, SPELL_DAMAGE_REFLECTION);

            //60 seconds until we should cast this agian
            DamageReflection_Timer = 30000;
        }else DamageReflection_Timer -= diff;

        //Blastwave_Timer
        if (Blastwave_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_BLASTWAVE);

            //10 seconds until we should cast this agian
            Blastwave_Timer = 10000;
        }else Blastwave_Timer -= diff;		

		if (CheckFlamewaker_Timer <diff)
		{
			if(EliteR1->isDead() && EliteR2->isDead() && EliteL1->isDead() && EliteL2->isDead() && PriestR1->isDead() && PriestR2->isDead() && PriestL1->isDead() && PriestL2->isDead())
			{
				m_creature->InterruptNonMeleeSpells(false);
				m_creature->DeleteThreatList();
				m_creature->ClearInCombat();
				m_creature->AttackStop();
				m_creature->setFaction(35);

				if(pInstance)
					pInstance->SetData(DATA_MAJORDOMO_SURRENDER,0);

				CheckFlamewaker_Timer=600000;
				Teleport=true;

				//m_creature->AddGameObject(GameObject::GetGameObject(*m_creature,pInstance->GetData64(DATA_CACHEOFTHEFIRELORD_GUID)));

				DoScriptText(SAY_DEFEAT, m_creature);
				DoPlaySoundToSet(m_creature,SOUND_DEFEAT);
			}
		}else CheckFlamewaker_Timer -= diff;

		DoMeleeAttackIfReady();
    }		
};

CreatureAI* GetAI_boss_majordomo(Creature *_Creature)
{
    return new boss_majordomoAI (_Creature);
}

bool GossipHello_boss_majordomo(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_DOMO , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    player->SEND_GOSSIP_MENU(68,_Creature->GetGUID());
    return true;
}

bool GossipSelect_boss_majordomo(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF+1:

            player->CLOSE_GOSSIP_MENU();
			
			((boss_majordomoAI*)_Creature->AI())->BeginEvent(player);
            break;
    }
    return true;
}

void AddSC_boss_majordomo()
{
    Script *newscript;
    newscript = new Script;

    newscript->Name = "boss_majordomo";
    newscript->GetAI = &GetAI_boss_majordomo;
    newscript->pGossipHello = &GossipHello_boss_majordomo;
    newscript->pGossipSelect = &GossipSelect_boss_majordomo;
    newscript->RegisterSelf();
}