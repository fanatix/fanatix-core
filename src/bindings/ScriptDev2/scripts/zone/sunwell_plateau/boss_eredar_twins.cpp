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
SDName: Boss_Alythess
SD%Complete: 0
SDComment: I bet they will never even see this one ... :-(
EndScriptData */

#include "precompiled.h"
#include "def_sunwell_plateau.h"

#define SPELL_TWINS_ENRAGE			 27680

#define YELL_TWINS_AGGRO			-1940001
#define YELL_TWINS_BOTH_DEATH		        -1940002
#define YELL_TWINS_SINGLE_DEATH		        -1940003
#define YELL_TWINS_ENRAGE			-1940004

#define YELL_TWINS_SPELL_STURMKNOCKBACK         -1940010 
#define YELL_TWINS_SPELL_STURMKNOCKBACK0	-1940005 
#define YELL_TWINS_SPELL_STURMKNOCKBACK1	-1940006
#define YELL_TWINS_SPELL_STURMKNOCKBACK2	-1940007
#define YELL_TWINS_SPELL_STURMKNOCKBACK3	-1940008
#define YELL_TWINS_SPELL_STURMKNOCKBACK4	-1940009 

#define YELL_ALY_SLAY			        -1941000
#define YELL_TWINS_SPELL_LOHE		        -1941001
#define YELL_TWINS_SPELL_SCHATTENKLINGEN        -1941002

#define YELL_SACRO_SLAY			        -1942000
#define YELL_TWINS_SPELL_SCHATTENNOVA	        -1942001
#define YELL_TWINS_SPELL_VERWIRRENDERS	        -1942002

void ResetAly(ScriptedInstance* instance, Creature* Caller);
void ResetSacro(ScriptedInstance* instance, Creature* Caller);

//############################################
//################## ALYTHESS ################
//############################################
struct MANGOS_DLL_DECL boss_AlythessAI : public ScriptedAI
{
    boss_AlythessAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;  

    uint32 SchattenklingenTimer;
    uint32 LoheTimer;
    uint32 SturmknockbackTimer;
    uint32 SpawnAddTimer;

    bool isEnraged;

    void Reset()
    {	
	SchattenklingenTimer = 23450;
	LoheTimer = 40000;
	SturmknockbackTimer = 50000;
	SpawnAddTimer = 20000;

	isEnraged = false;

	ResetSacro(pInstance, m_creature);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(YELL_TWINS_AGGRO, m_creature);
	 DoPlaySoundToSet(m_creature, 12484);
    }

    void KilledUnit(Unit* victim)
    {
           DoScriptText(YELL_ALY_SLAY, m_creature);
           switch(rand()%2)
           {
            	case 0: DoPlaySoundToSet(m_creature, 12490); break;
            	case 1: DoPlaySoundToSet(m_creature, 12491); break;
            }
    }

    void JustDied(Unit* Killer)
    {
        if (Creature* Sacro = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_SACROLASH))))
		if(Sacro->GetHealth() == 0)
		{	DoScriptText(YELL_TWINS_BOTH_DEATH, m_creature);
			Sacro->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
			if(pInstance)
                     	pInstance->SetData(DATA_EREDAR_TWINS_EVENT, DONE);
                	if(GameObject* Gate = GameObject::GetGameObject(*m_creature, pInstance->GetData64(DATA_GATE_3)))
	                	Gate->SetGoState(0);
		}
		else
		{	DoScriptText(YELL_TWINS_SINGLE_DEATH, m_creature);
			m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
			DoPlaySoundToSet(m_creature, 12488);
		}
    }

    void UpdateAI(const uint32 diff)
    {

	 if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

	 if (SchattenklingenTimer < diff)
        {
	     DoScriptText(YELL_TWINS_SPELL_SCHATTENKLINGEN, m_creature);
            DoCast(m_creature->getVictim(), 45248);
            SchattenklingenTimer = 23450;
        }else SchattenklingenTimer -= diff;

	 if (LoheTimer < diff)
        {
	     DoScriptText(YELL_TWINS_SPELL_LOHE, m_creature);
            DoCast(SelectUnit(SELECT_TARGET_BOTTOMAGGRO, 1), 45235);
            LoheTimer= 40000;
        }else LoheTimer-= diff;

        if (Creature* Sacro = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_SACROLASH))))
		if(Sacro->GetHealth() == 0 && isEnraged == false)
		{
		    DoScriptText(YELL_TWINS_ENRAGE, m_creature);
		    DoCast(m_creature, 45366, true);
		    DoCast(m_creature, SPELL_TWINS_ENRAGE, true);
		    isEnraged = true;
		}

	if (SturmknockbackTimer < diff)
	{
         if(isEnraged == true)
         {
		Unit* target = m_creature->getVictim();

		DoScriptText(YELL_TWINS_SPELL_STURMKNOCKBACK, m_creature);

		for(int i = 0; i < 5; i++)
		{
			switch(i)
			{	case 0: DoScriptText(YELL_TWINS_SPELL_STURMKNOCKBACK0, m_creature); break;
				case 1: DoScriptText(YELL_TWINS_SPELL_STURMKNOCKBACK1, m_creature); break;
				case 2: DoScriptText(YELL_TWINS_SPELL_STURMKNOCKBACK2, m_creature); break;
				case 3: DoScriptText(YELL_TWINS_SPELL_STURMKNOCKBACK3, m_creature); break;
				case 4: DoScriptText(YELL_TWINS_SPELL_STURMKNOCKBACK4, m_creature); break;
			}
			DoCast(SelectUnit(SELECT_TARGET_RANDOM, 1), 25787, true);			
			m_creature->DealDamage(m_creature->getVictim(), 5000, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
			DoCast(SelectUnit(SELECT_TARGET_RANDOM, 1), 10689, true);
		}

		DoCast(target, 25787, true);			
		DoCast(target, 45256, true);

	   }
	   SturmknockbackTimer = 50000;
	}else SturmknockbackTimer -= diff;


	if (SpawnAddTimer < diff)
	{
		if(isEnraged == true)
	 	{
			Creature* Helper = m_creature->SummonCreature(8000000, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 20000);
			if(Helper) 
			{
				Unit* sTarget = SelectUnit(SELECT_TARGET_RANDOM, 1);
				DoCast(Helper,45263);
				Helper->AI()->AttackStart(sTarget);
			}
		}
		SpawnAddTimer = 20000;
	}else SpawnAddTimer -= diff;


        DoMeleeAttackIfReady();
    }
};

//############################################
//################# SACROLASH ################
//############################################
struct MANGOS_DLL_DECL boss_SacrolashAI : public ScriptedAI
{
    boss_SacrolashAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 VerwirrenderSchlagTimer;
    uint32 SchattennovaTimer;
    uint32 SturmknockbackTimer;
    uint32 SpawnAddTimer;

    bool isEnraged;

    void Reset()
    {

	VerwirrenderSchlagTimer = 35000;
	SchattennovaTimer = 45000;
	SturmknockbackTimer = 50000;
	SpawnAddTimer = 20000;
	
	isEnraged = false;
	
	ResetAly(pInstance, m_creature);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(YELL_TWINS_AGGRO, m_creature);
    }

    void KilledUnit(Unit* victim)
    {
           DoScriptText(YELL_SACRO_SLAY, m_creature);
           switch(rand()%2)
           {
            	case 0: DoPlaySoundToSet(m_creature, 12486); break;
            	case 1: DoPlaySoundToSet(m_creature, 12487); break;
            }
    }

    void JustDied(Unit* Killer)
    {
        if (Creature* Aly = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_ALYTHESS))))
		if(Aly->GetHealth() == 0)
		{	DoScriptText(YELL_TWINS_BOTH_DEATH, m_creature);
			Aly->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
			if(pInstance)
                     	pInstance->SetData(DATA_EREDAR_TWINS_EVENT, DONE);
                	if(GameObject* Gate = GameObject::GetGameObject(*m_creature, pInstance->GetData64(DATA_GATE_3)))
	                	Gate->SetGoState(0);
		}
		else
		{	DoScriptText(YELL_TWINS_SINGLE_DEATH, m_creature);
			m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
			DoPlaySoundToSet(m_creature, 12492);
		}	
    }

    void UpdateAI(const uint32 diff)
    {

	 if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

	 if (VerwirrenderSchlagTimer < diff)
        {
	     DoScriptText(YELL_TWINS_SPELL_VERWIRRENDERS, m_creature);
            DoCast(m_creature->getVictim(), 45256);
            VerwirrenderSchlagTimer = 35000;
        }else VerwirrenderSchlagTimer -= diff;

	 if (SchattennovaTimer < diff)
        {
	     DoScriptText(YELL_TWINS_SPELL_SCHATTENNOVA, m_creature);
            DoCast(SelectUnit(SELECT_TARGET_RANDOM, 1), 45329);
            SchattennovaTimer = 45000;
        }else SchattennovaTimer -= diff;

        if (Creature* Aly = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_ALYTHESS))))
		if(Aly->GetHealth() == 0 && isEnraged == false)
		{
		    DoScriptText(YELL_TWINS_ENRAGE, m_creature);
		    DoCast(m_creature, 45366, true);
		    DoCast(m_creature, SPELL_TWINS_ENRAGE, true);
		    isEnraged = true;
		}

	if (SturmknockbackTimer < diff)
	{
         if(isEnraged == true)
         {
		Unit* target = m_creature->getVictim();

		DoScriptText(YELL_TWINS_SPELL_STURMKNOCKBACK, m_creature);

		for(int i = 0; i < 5; i++)
		{
			switch(i)
			{	case 0: DoScriptText(YELL_TWINS_SPELL_STURMKNOCKBACK0, m_creature); break;
				case 1: DoScriptText(YELL_TWINS_SPELL_STURMKNOCKBACK1, m_creature); break;
				case 2: DoScriptText(YELL_TWINS_SPELL_STURMKNOCKBACK2, m_creature); break;
				case 3: DoScriptText(YELL_TWINS_SPELL_STURMKNOCKBACK3, m_creature); break;
				case 4: DoScriptText(YELL_TWINS_SPELL_STURMKNOCKBACK4, m_creature); break;
			}
			DoCast(SelectUnit(SELECT_TARGET_RANDOM, 1), 25787, true);			
			m_creature->DealDamage(m_creature->getVictim(), 5000 , NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
			DoCast(SelectUnit(SELECT_TARGET_RANDOM, 1), 10689, true);
		}

		DoCast(target, 25787, true);			
		DoCast(target, 45256, true);

	   }
	   SturmknockbackTimer = 50000;

	}else SturmknockbackTimer -= diff;

	if (SpawnAddTimer < diff)
	{
		if(isEnraged == true)
	 	{
			Creature* Helper = m_creature->SummonCreature(8000000, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 20000);
			if(Helper) 
			{
				Unit* sTarget = SelectUnit(SELECT_TARGET_RANDOM, 1);
				DoCast(Helper,45263);
				Helper->AI()->AttackStart(sTarget);
			}
		}
		SpawnAddTimer = 20000;
	}else SpawnAddTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

void ResetAly(ScriptedInstance* instance, Creature* Caller)
{
	 ScriptedInstance* pInstance;
	 pInstance = instance;

	 Creature* Aly = ((Creature*)Unit::GetUnit(*Caller, pInstance->GetData64(DATA_ALYTHESS)));
        if (Aly)
        {
             if (!Aly->isAlive())
             {    Aly->Respawn();
                ((boss_AlythessAI*)Aly->AI())->Reset();
                ((boss_AlythessAI*)Aly->AI())->EnterEvadeMode();
                Aly->GetMotionMaster()->MoveTargetedHome();
	     }
	 }
}

void ResetSacro(ScriptedInstance* instance, Creature* Caller)
{
	 ScriptedInstance* pInstance;
	 pInstance = instance;

	 Creature* Sacro = ((Creature*)Unit::GetUnit(*Caller, pInstance->GetData64(DATA_SACROLASH)));
        if (Sacro)
        {
             if (!Sacro->isAlive())
             {    Sacro->Respawn();

                ((boss_SacrolashAI*)Sacro->AI())->Reset();
                ((boss_SacrolashAI*)Sacro->AI())->EnterEvadeMode();
                Sacro->GetMotionMaster()->MoveTargetedHome();
	      }
	 }
}
//############### DEKLARATIONs ####################
CreatureAI* GetAI_boss_Alythess(Creature *_Creature)
{
    return new boss_AlythessAI (_Creature);
}

CreatureAI* GetAI_boss_Sacrolash(Creature *_Creature)
{
    return new boss_SacrolashAI (_Creature);
}

void AddSC_boss_eredar_twins()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_Alythess";
    newscript->GetAI = &GetAI_boss_Alythess;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_Sacrolash";
    newscript->GetAI = &GetAI_boss_Sacrolash;
    newscript->RegisterSelf();
}
