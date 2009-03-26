/* Script Data Start
SDName: Boss keristrasza
SDAuthor: FBPBM
SD%Complete: 80%
SDComment: Ready for Testing , Instance not Scripted Yet
SDCategory: Nexus , Script by  http://www.wowparadox.de/
Script Data End */

#include "precompiled.h"
#include "def_nexus.h"

//Spells
#define SPELL_TAIL_SWEEP                                50155
#define SPELL_CRYSTAL_CHAINS                            50997
#define SPELL_ENRAGE                                    8599
#define SPELL_CRYSTALFIRE_BREATH_1                      48096
#define SPELL_CRYSTALFIRE_BREATH_2                      57091
#define SPELL_CRYSTALIZE                                48179
#define SPELL_INTENSE_COLD                              48095 //stackable debuff

//Yell
#define SAY_AGGRO                                    -1999785
#define SAY_SLAY                                     -1999784
#define SAY_ENRAGE                                   -1999783
#define SAY_DEATH                                    -1999782
#define SAY_CRYSTAL_NOVA                             -1999781

struct MANGOS_DLL_DECL boss_keristraszaAI : public ScriptedAI
{
    boss_keristraszaAI(Creature *c) : ScriptedAI(c)
    {
        //pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
		HeroicMode = m_creature->GetMap()->IsHeroic();

    }

//ScriptedInstance* pInstance;

 bool HeroicMode;

    uint32 CRYSTALFIRE_BREATH_Timer;
    uint32 CRYSTAL_CHAINS_Timer;
    uint32 INTENSE_COLD_Timer;
    uint32 TAIL_SWEEP_Timer;
    uint32 CRYSTALIZE_Timer;
	uint32 Enrage_Timer;

    void Reset() 
	{
		//These times are probably wrong
	   CRYSTALFIRE_BREATH_Timer = 8000;                         
       CRYSTAL_CHAINS_Timer = 12000;
       INTENSE_COLD_Timer = 7000;
       TAIL_SWEEP_Timer = 10000;
       CRYSTALIZE_Timer = 3000;          
       Enrage_Timer = 600000;

	   m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, true);
		//if(pInstance)
        //    pInstance->SetData(DATA_KERISTRASZA, NOT_STARTED);
	}
    void Aggro(Unit* who) 
    {
		//if(pInstance)
        //pInstance->SetData(DATA_KERISTRASZA, IN_PROGRESS);
        DoScriptText(SAY_AGGRO, m_creature);
	//	DoZoneInCombat();
    }

    void UpdateAI(const uint32 diff) 
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
          
if (Enrage_Timer < diff)
        {
			DoScriptText(SAY_ENRAGE , m_creature);
            m_creature->CastSpell(m_creature, SPELL_ENRAGE, true);
		
            Enrage_Timer = 600000;
}else Enrage_Timer -=diff;

if (CRYSTALIZE_Timer < diff)
        {
			m_creature->CastSpell(m_creature, SPELL_CRYSTALIZE, true);
		
            CRYSTALIZE_Timer = 3000;
}else CRYSTALIZE_Timer -=diff;

if (CRYSTALFIRE_BREATH_Timer < diff)
            {
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target,SPELL_CRYSTALFIRE_BREATH_1);
				if (HeroicMode)
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target,SPELL_CRYSTALFIRE_BREATH_2);
                CRYSTALFIRE_BREATH_Timer = 8000;
            }else CRYSTALFIRE_BREATH_Timer -=diff;

 if (CRYSTAL_CHAINS_Timer < diff)
            {
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_CRYSTAL_CHAINS);

                CRYSTAL_CHAINS_Timer = 10000;
            }else CRYSTAL_CHAINS_Timer -= diff;

 if (INTENSE_COLD_Timer < diff)
            {
				DoScriptText(SAY_CRYSTAL_NOVA , m_creature);
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_INTENSE_COLD);

                INTENSE_COLD_Timer = 7000;
            }else INTENSE_COLD_Timer -= diff;



        DoMeleeAttackIfReady();    
    }
    void JustDied(Unit* killer)  
    {
		//if(pInstance)
        //   pInstance->SetData(DATA_KERISTRASZA, DONE);
        DoScriptText(SAY_DEATH, m_creature);
    }
    void KilledUnit(Unit *victim)
    {
        if(victim == m_creature)
            return;
        DoScriptText(SAY_SLAY, m_creature);
    }
};

CreatureAI* GetAI_boss_keristrasza(Creature *_Creature)
{
    return new boss_keristraszaAI (_Creature);
}

void AddSC_boss_keristrasza()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_keristrasza";
    newscript->GetAI = &GetAI_boss_keristrasza;
    newscript->RegisterSelf();
}
