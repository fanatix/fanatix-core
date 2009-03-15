/* Script Data Start
SDName: Boss anomalus
SDAuthor: LordVanMartin,TonyMontana5000
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = '' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Spells
#define SPELL_SPARK_1              47751
#define SPELL_SPARK_2              57062 //Heroic? - Deals more about 2000 more dmg than SPELL_SPARK_1
#define SPELL_RIFT_SHIELD          47748
#define SPELL_CHARGE_RIFT          47747
#define SPELL_ARCANE_ATTRACTION    57063
#define SPELL_CREATE_RIFT          47743 //Dummy ---> summons (npc 26918) "Chaotic Rift", which spawns "Crazed Mana Wraith"s  (npc 26746)

#define MOB_CRAZED_MANA_WRAITH     26746
#define NPC_CHAOTIC_RIFT           26918

//Yell
#define SAY_AGGRO               -1999790
#define SAY_KILL                -1999789
#define SAY_DEATH               -1999788
#define SAY_RIFT                -1999787
#define SAY_SHIELD              -1999786

struct MANGOS_DLL_DECL boss_anomalusAI : public ScriptedAI
{
    boss_anomalusAI(Creature *c) : ScriptedAI(c) { Reset(); }

	uint8 Phase;
	// 0 = Not started
	// 1 = reached 75% HP
	// 2 = reached 50% HP
	// 3 = reached 25% HP

	bool isImmune;

    void Reset() 
	{
		Phase = 0;

	}
    void Aggro(Unit* who) 
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }
    void AttackStart(Unit* who) {}
    void MoveInLineOfSight(Unit* who) {}
    void UpdateAI(const uint32 diff) 
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
        
		//Create Rift at 75% HP
		if (Phase != 1 && m_creature->GetMaxHealth() * 0.75 > m_creature->GetHealth())
		{
			Phase = 1;
			//Make Anomalus immune
			DoScriptText(SAY_SHIELD , m_creature);
			DoCast(m_creature,SPELL_RIFT_SHIELD);

			DoCast(m_creature,47743); //Only Dummy
			//TODO: Fix Position
			Creature* Rift = m_creature->SummonCreature(NPC_CHAOTIC_RIFT, m_creature->GetPositionX()+1, m_creature->GetPositionY()+1, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000);

			//TODO: Implement SPELL_CHARGE_RIFT
		}

		//Create Rift at 50% HP
		if (Phase == 1 && m_creature->GetMaxHealth() * 0.50 > m_creature->GetHealth())
		{
			Phase = 2;
			//Make Anomalus immune
			DoScriptText(SAY_SHIELD , m_creature);
			DoCast(m_creature,SPELL_RIFT_SHIELD);

			DoCast(m_creature,47743); //Only Dummy
			//TODO: Fix Position
			Creature* Rift = m_creature->SummonCreature(NPC_CHAOTIC_RIFT, m_creature->GetPositionX()+1, m_creature->GetPositionY()+1, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000);

			//TODO: Implement SPELL_CHARGE_RIFT
		}

		//Create Rift at 25% HP
		if (Phase == 2 && m_creature->GetMaxHealth() * 0.25 > m_creature->GetHealth())
		{
			Phase = 3;
			//Make Anomalus immune
			DoScriptText(SAY_SHIELD , m_creature);
			DoCast(m_creature,SPELL_RIFT_SHIELD);

			DoCast(m_creature,47743); //Only Dummy
			//TODO: Fix Position
			Creature* Rift = m_creature->SummonCreature(NPC_CHAOTIC_RIFT, m_creature->GetPositionX()+1, m_creature->GetPositionY()+1, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000);

			//TODO: Implement SPELL_CHARGE_RIFT
		}

        DoMeleeAttackIfReady();    
    }
    void JustDied(Unit* killer)  
    {
        DoScriptText(SAY_DEATH, m_creature);
    }
    void KilledUnit(Unit *victim)
    {
        if(victim == m_creature)
            return;
        DoScriptText(SAY_KILL, m_creature);
    }
};

/*######
## Crazed Mana Wraiths
######*/
#define SPELL_ARCANE_MISSILES								33833

//TODO: fix Timers, they are imaginary
#define ARCANE_MISSILES_TIMER								10000

struct MANGOS_DLL_DECL mob_crazed_mana_wraithAI : public ScriptedAI
{
    mob_crazed_mana_wraithAI(Creature *c) : ScriptedAI(c) { Reset(); }

    void Reset() {}
    void Aggro(Unit* who) {}
    void UpdateAI(const uint32 diff) 
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
               
		if (ARCANE_MISSILES_TIMER < diff)
		{
			//if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
				//DoCast(target, SPELL_ARCANE_MISSILES); //Disable till timers are fixed
		}

        DoMeleeAttackIfReady();    
    }
    void JustDied(Unit* killer)  {}
};

/*######
## Chaotic Rift
######*/
#define SPELL_CHAOTIC_ENERGY_BURST                      47688
#define SPELL_CHARGED_CHAOTIC_ENERGY_BURST              47737

//TODO: fix Timers, they are imaginary
#define CHAOTIC_ENERGY_BURST_TIMER						10000
#define CHARGED_CHAOTIC_ENERGY_BURST_TIMER				10000

struct MANGOS_DLL_DECL npc_chaotic_riftAI : public ScriptedAI
{
    npc_chaotic_riftAI(Creature *c) : ScriptedAI(c) { Reset(); }

    void Reset() {}
    void Aggro(Unit* who) {}
    void UpdateAI(const uint32 diff) 
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

		if (CHAOTIC_ENERGY_BURST_TIMER < diff)
		{
			//if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
				//DoCast(target, SPELL_CHAOTIC_ENERGY_BURST); //Disable till timers are fixed
		}        

		if (CHARGED_CHAOTIC_ENERGY_BURST_TIMER < diff)
		{
			//if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
				//DoCast(target, SPELL_CHARGED_CHAOTIC_ENERGY_BURST); //Disable till timers are fixed
		}
        DoMeleeAttackIfReady();    
    }
    void JustDied(Unit* killer)  
	{

	}
};

CreatureAI* GetAI_npc_chaotic_rift(Creature *_Creature)
{
    return new npc_chaotic_riftAI (_Creature);
}

CreatureAI* GetAI_boss_anomalus(Creature *_Creature)
{
    return new boss_anomalusAI (_Creature);
}

CreatureAI* GetAI_mob_crazed_mana_wraith(Creature *_Creature)
{
    return new boss_anomalusAI (_Creature);
}

void AddSC_boss_anomalus()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_anomalus";
    newscript->GetAI = &GetAI_boss_anomalus;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name="mob_crazed_mana_wraith";
    newscript->GetAI = &GetAI_mob_crazed_mana_wraith;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name="npc_chaotic_rift";
    newscript->GetAI = &GetAI_npc_chaotic_rift;
    newscript->RegisterSelf();
}
