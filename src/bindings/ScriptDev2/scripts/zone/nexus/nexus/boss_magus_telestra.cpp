/* Script Data Start
SDName: Boss magus_telestra
SDAuthor: FBPBM
SD%Complete:  80%
SDComment:  Testbereit ^^
SDCategory: Script by  http://www.wowparadox.de/
Script Data End */

#include "precompiled.h"


//Spells

//phase 1
#define SPELL_ICE_NOVA_N          47772
#define SPELL_ICE_NOVA_H          56935
#define SPELL_FIREBOMB_N          47773
#define SPELL_FIREBOMB_H          56934
#define SPELL_GAVITY_WELL         47756

//Phase 2      -->50% HP (3 clones, Frost, Fire, Arcane)
#define Mob_Frost_Magus         26930
#define SPELL_BLIZZARD_N        47727
#define SPELL_ICE_BARB_N        47729
//heroic
#define SPELL_BLIZZARD_H        56936
#define SPELL_ICE_BARB_H        56937

#define Mob_Fire_Magus          26928
#define SPELL_FIRE_BLAST_N      47721
#define SPELL_SCORCH_N          47723
//heroic
#define SPELL_FIRE_BLAST_H      56939
#define SPELL_SCORCH_H          56938

#define Mob_Arcane_Magus        26929
#define SPELL_CRITTER           47731
#define SPELL_TIME_STOP         47736

//Yell
#define SAY_AGGRO             -1999780   
#define SAY_KILL              -1999779      
#define SAY_DEATH             -1999778
#define SAY_MERGE             -1999777
#define SAY_SPLIT_1           -1999776
#define SAY_SPLIT_2           -1999775


struct MANGOS_DLL_DECL boss_magus_telestraAI : public ScriptedAI
{
    boss_magus_telestraAI(Creature *c) : ScriptedAI(c) 
	{
		Reset();
		//pInstance = ((ScriptedInstance*)c->GetInstanceData());
	      HeroicMode = m_creature->GetMap()->IsHeroic();
	}

	//ScriptedInstance* pInstance;
bool HeroicMode;

 uint8 Phase; 
 // 0 = Not started  
 // 1 = reached 50% HP  
 // 2 = reached 10% HP  Heroic Mode

uint32 SPELL_ICE_NOVA_Timer;                    
uint32 SPELL_FIREBOMB_Timer;                    
uint32 SPELL_GAVITY_WELL_Timer;         
    void Reset()
{
Phase = 0;
//These times are probably wrong
SPELL_ICE_NOVA_Timer =  8000;                  
SPELL_FIREBOMB_Timer =  10000;                
SPELL_GAVITY_WELL_Timer = 7500;

//if(pInstance)
//  pInstance->SetData(data_keristrasza, NOT_STARTED);
}
    void Aggro(Unit* who) 
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }
    //void AttackStart(Unit* who) {}
    //void MoveInLineOfSight(Unit* who) {}
    void UpdateAI(const uint32 diff) 
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
            
        if (Phase != 0 && m_creature->GetMaxHealth() * 0.50 > m_creature->GetHealth()) 
        {     
	     Phase = 1;
        
			 DoCast(m_creature,26928); 
			 DoCast(m_creature,26929); 
			 DoCast(m_creature,26930); 
	    }
		if (Phase == 1 && m_creature->GetMaxHealth() * 0.10 > m_creature->GetHealth())  
		{      Phase = 2;
        if (HeroicMode)
			DoCast(m_creature,26928); 
			DoCast(m_creature,26929); 
			DoCast(m_creature,26930); 
        }else Phase = 0;

    if (SPELL_ICE_NOVA_Timer < diff)
            {
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target,SPELL_ICE_NOVA_N);
				if (HeroicMode)
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target,SPELL_ICE_NOVA_H);
                SPELL_ICE_NOVA_Timer = 8000;
            }else SPELL_ICE_NOVA_Timer -=diff;

    if (SPELL_FIREBOMB_Timer < diff)
            {
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target,SPELL_FIREBOMB_N);
				if (HeroicMode)
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target,SPELL_FIREBOMB_H);
                SPELL_FIREBOMB_Timer = 10000;
            }else SPELL_FIREBOMB_Timer -=diff;

    if (SPELL_GAVITY_WELL_Timer < diff)
            {
            m_creature->CastSpell(m_creature, SPELL_GAVITY_WELL, true);
				    SPELL_GAVITY_WELL_Timer = 7500;
            }else SPELL_GAVITY_WELL_Timer -=diff;

        DoMeleeAttackIfReady();    
    }
    void JustDied(Unit* killer)  {}
    void KilledUnit(Unit *victim)
    {
        if(victim == m_creature)
            return;
        DoScriptText(SAY_KILL, m_creature);
    }
};

struct MANGOS_DLL_DECL Mob_Frost_MagusAI : public ScriptedAI
{
    Mob_Frost_MagusAI(Creature *c) : ScriptedAI(c) { Reset(); 
	 HeroicMode = m_creature->GetMap()->IsHeroic();
	}
    bool HeroicMode;

    uint32 SPELL_BLIZZARD_TIMER;
	uint32 SPELL_ICE_BARB_TIMER;

    void Reset()
	{
	SPELL_BLIZZARD_TIMER = 10000;
	SPELL_ICE_BARB_TIMER = 8000;
	}
    void Aggro(Unit* who) {}
    void UpdateAI(const uint32 diff) 
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

		if (SPELL_BLIZZARD_TIMER < diff)
		{
			if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
				DoCast(target, SPELL_BLIZZARD_N); 
				if (HeroicMode)
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target,SPELL_BLIZZARD_H);
                SPELL_BLIZZARD_TIMER = 10000;
            }else SPELL_BLIZZARD_TIMER -=diff;

		if (SPELL_ICE_BARB_TIMER < diff)
		{
			if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
				DoCast(target, SPELL_ICE_BARB_N); 
			if (HeroicMode)
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target,SPELL_ICE_BARB_H);
                SPELL_ICE_BARB_TIMER = 8000;
            }else SPELL_ICE_BARB_TIMER -=diff;
			
        DoMeleeAttackIfReady();    
    }
    void JustDied(Unit* killer)  
	{

	}
};

struct MANGOS_DLL_DECL Mob_Fire_MagusAI : public ScriptedAI
{
    Mob_Fire_MagusAI(Creature *c) : ScriptedAI(c) { Reset(); 
	 HeroicMode = m_creature->GetMap()->IsHeroic();
	}
    bool HeroicMode;

    uint32 SPELL_FIRE_BLAST_TIMER;
    uint32 SPELL_SCORCH_TIMER;

    void Reset() 
	{
	SPELL_FIRE_BLAST_TIMER = 10000;
	SPELL_SCORCH_TIMER = 8000;
	}
    void Aggro(Unit* who) {}
    void UpdateAI(const uint32 diff) 
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

		if (SPELL_FIRE_BLAST_TIMER < diff)
		{
			if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
				DoCast(target, SPELL_FIRE_BLAST_N);
			if (HeroicMode)
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target,SPELL_FIRE_BLAST_H);
                SPELL_FIRE_BLAST_TIMER = 10000;
            }else SPELL_FIRE_BLAST_TIMER -=diff;        

		if (SPELL_SCORCH_TIMER < diff)
		{
			if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
				DoCast(target, SPELL_SCORCH_N);
			if (HeroicMode)
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target,SPELL_SCORCH_H);
                SPELL_SCORCH_TIMER = 8000;
            }else SPELL_SCORCH_TIMER -=diff;

        DoMeleeAttackIfReady();    
    }
    void JustDied(Unit* killer)  
	{

	}
};

struct MANGOS_DLL_DECL Mob_Arcane_MagusAI : public ScriptedAI
{
    Mob_Arcane_MagusAI(Creature *c) : ScriptedAI(c) { Reset();
	 HeroicMode = m_creature->GetMap()->IsHeroic();
	}
    bool HeroicMode;

    uint32 SPELL_CRITTER_TIMER;
    uint32 SPELL_TIME_STOP_TIMER;

    void Reset()
	{
	SPELL_CRITTER_TIMER = 10000;
	SPELL_TIME_STOP_TIMER = 15000;
	}
    void Aggro(Unit* who) {}
    void UpdateAI(const uint32 diff) 
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

		if (SPELL_CRITTER_TIMER < diff)
		{
			if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
				DoCast(target, SPELL_CRITTER); 
			
                SPELL_CRITTER_TIMER = 10000;
            }else SPELL_CRITTER_TIMER -=diff;
		        

		if (SPELL_TIME_STOP_TIMER < diff)
		{
			if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
				DoCast(target, SPELL_TIME_STOP);
			
                SPELL_TIME_STOP_TIMER = 15000;
            }else SPELL_TIME_STOP_TIMER -=diff;
		
        DoMeleeAttackIfReady();    
    }
    void JustDied(Unit* killer)  
	{

	}
};
CreatureAI* GetAI_boss_magus_telestra(Creature *_Creature)
{
    return new boss_magus_telestraAI (_Creature);
}

CreatureAI* GetAI_Mob_Frost_Magus(Creature *_Creature)
{
    return new Mob_Frost_MagusAI (_Creature);
}

CreatureAI* GetAI_Mob_Fire_Magus(Creature *_Creature)
{
    return new Mob_Fire_MagusAI (_Creature);
}

CreatureAI* GetAI_Mob_Arcane_Magus(Creature *_Creature)
{
    return new Mob_Arcane_MagusAI (_Creature);
}

void AddSC_boss_magus_telestra()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_magus_telestra";
    newscript->GetAI = &GetAI_boss_magus_telestra;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "Mob_Frost_Magus";
    newscript->GetAI = &GetAI_Mob_Frost_Magus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "Mob_Fire_Magus";
    newscript->GetAI = &GetAI_Mob_Fire_Magus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "Mob_Arcane_Magus";
    newscript->GetAI = &GetAI_Mob_Arcane_Magus;
    newscript->RegisterSelf();
}
