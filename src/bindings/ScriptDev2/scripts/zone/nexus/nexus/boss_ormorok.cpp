/* Script Data Start
SDName: Boss ormorok
SDAuthor: FBPBM
SD%Complete: 80%
SDComment: Ready for Testing , Instance not Scripted Yet
SDCategory: Nexus , Script by  http://www.wowparadox.de/
Script Data End */

#include "precompiled.h"

//Spells
#define SPELL_CRYSTAL_SPIKES_N                     47958
#define SPELL_CRYSTAL_SPIKES_H                     57082
#define SPELL_SPELL_REFLECTION                     47981
#define SPELL_TRAMPLE_N                            48016
#define SPELL_TRAMPLE_H                            57066
#define SPELL_FRENZY_N                             57086
#define SPELL_FRENZY_H                             48017
#define SPELL_SUMMON_CRYSTALLINE_TANGLER           61564 //summons npc 32665

//Yell
#define SAY_AGGRO                               -1999774
#define SAY_KILL                                -1999773
#define SAY_DEATH                               -1999772
#define SAY_REFLECT                             -1999771
#define SAY_ICE_SPIKES                          -1999770

struct MANGOS_DLL_DECL boss_ormorokAI : public ScriptedAI
{
    boss_ormorokAI(Creature *c) : ScriptedAI(c) 

		{
        //pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
		HeroicMode = m_creature->GetMap()->IsHeroic();

    }

	//ScriptedInstance* pInstance;

	bool HeroicMode;

    uint32 SPELL_CRYSTAL_SPIKES_Timer;
    uint32 SPELL_TRAMPLE_Timer;
	uint32 SPELL_FRENZY_Timer;
    uint32 SPELL_SPELL_REFLECTION_Timer;
    uint32 SPELL_SUMMON_CRYSTALLINE_TANGLER_Timer;



    void Reset() 
	{
	//These times are probably wrong
	   SPELL_CRYSTAL_SPIKES_Timer = 15000;                         
       SPELL_TRAMPLE_Timer = 10000;
	   SPELL_FRENZY_Timer = 65000;
       SPELL_SPELL_REFLECTION_Timer = 25000;
       SPELL_SUMMON_CRYSTALLINE_TANGLER_Timer = 20000;
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

		if (SPELL_FRENZY_Timer < diff)
        {
			m_creature->CastSpell(m_creature, SPELL_FRENZY_N, true);
			if (HeroicMode)
			m_creature->CastSpell(m_creature, SPELL_FRENZY_H, true);
		
            SPELL_FRENZY_Timer = 5000;
}else SPELL_FRENZY_Timer -=diff;

		if (SPELL_SUMMON_CRYSTALLINE_TANGLER_Timer < diff)
        {
			m_creature->CastSpell(m_creature, SPELL_SUMMON_CRYSTALLINE_TANGLER, true);
			m_creature->CastSpell(m_creature, SPELL_SUMMON_CRYSTALLINE_TANGLER, true);
			m_creature->CastSpell(m_creature, SPELL_SUMMON_CRYSTALLINE_TANGLER, true);

            SPELL_SUMMON_CRYSTALLINE_TANGLER_Timer = 20000;
}else SPELL_SUMMON_CRYSTALLINE_TANGLER_Timer -=diff;

if (SPELL_CRYSTAL_SPIKES_Timer < diff)
            {
				DoScriptText(SAY_ICE_SPIKES, m_creature);
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target,SPELL_CRYSTAL_SPIKES_N);
				if (HeroicMode)
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target,SPELL_CRYSTAL_SPIKES_H);
                SPELL_CRYSTAL_SPIKES_Timer = 15000;
            }else SPELL_CRYSTAL_SPIKES_Timer -=diff;

 if (SPELL_TRAMPLE_Timer < diff)
            {
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_TRAMPLE_N);
				if (HeroicMode)
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target,SPELL_TRAMPLE_H);

                SPELL_TRAMPLE_Timer = 10000;
            }else SPELL_TRAMPLE_Timer -= diff;

 if (SPELL_SPELL_REFLECTION_Timer < diff)
            {
				DoScriptText(SAY_REFLECT, m_creature);
                m_creature->CastSpell(m_creature,SPELL_SPELL_REFLECTION, true);

                 SPELL_SPELL_REFLECTION_Timer = 25000;
            }else SPELL_SPELL_REFLECTION_Timer -= diff;

                
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

CreatureAI* GetAI_boss_ormorok(Creature *_Creature)
{
    return new boss_ormorokAI (_Creature);
}

void AddSC_boss_ormorok()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_ormorok";
    newscript->GetAI = GetAI_boss_ormorok;
    newscript->RegisterSelf();
}
