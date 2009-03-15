/* Script Data Start
SDName: Boss keristrasza
SDAuthor: FBPBM & TonyMontana5000
SD%Complete: 80%
SDComment: Ready for Testing , Instance not Scripted Yet
SDCategory: Nexus
Script Data End */

#include "precompiled.h"
//#include "def_nexus_h"

//Spells
#define SPELL_TAIL_SWEEP 50155
#define SPELL_CRYSTAL_CHAINS 50997
#define SPELL_ENRAGE 8599
#define SPELL_CRYSTALFIRE_BREATH_1 48096
#define SPELL_CRYSTALFIRE_BREATH_2 57091
#define SPELL_CRYSTALIZE 48179
#define SPELL_INTENSE_COLD 48095 //stackable debuff

//Yell
#define SAY_AGGRO -1999785
#define SAY_SLAY -1999784
#define SAY_ENRAGE -1999783
#define SAY_DEATH -1999782
#define SAY_CRYSTAL_NOVA -1999781

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
	CRYSTAL_CHAINS_Timer = 12000; //Heroic timer ticks faster
	INTENSE_COLD_Timer = 7000; //Should be ~1500, it ticks much faster then every 7sec. on live
	TAIL_SWEEP_Timer = 10000;
	CRYSTALIZE_Timer = 3000;
	Enrage_Timer = 600000;

    //if(pInstance)
          // pInstance->SetData(data_keristrasza, NOT_STARTED);
	}

    void Aggro(Unit* who)
    {
    //if(pInstance)
        //pInstance->SetData(data_keristrasza, IN_PROGRESS);
        DoScriptText(SAY_AGGRO, m_creature);
  //  DoZoneInCombat();
    }
    //void AttackStart(Unit* who) {}
    //void MoveInLineOfSight(Unit* who) {}

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
			//TODO: Only targets that are range of 50yard should get hit by this
			//Cast it on the entire group
			std::list<HostilReference*>::iterator i = m_creature->getThreatManager().getThreatList().begin();
			for (i = m_creature->getThreatManager().getThreatList().begin(); i!= m_creature->getThreatManager().getThreatList().end();++i)
			{
				Unit* pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
				if (pUnit && (pUnit->GetTypeId() == TYPEID_PLAYER))
				{
					DoCast(pUnit, SPELL_CRYSTALIZE);
				}
			}

            CRYSTALIZE_Timer = 3000;
		}else CRYSTALIZE_Timer -=diff;

		if (CRYSTALFIRE_BREATH_Timer < diff)
        {
			//TODO: Only targets that in cone in front of the caster should be hit
			if (HeroicMode)
			{
				if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target,SPELL_CRYSTALFIRE_BREATH_2);
			}
			else
			{
				if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
					DoCast(target,SPELL_CRYSTALFIRE_BREATH_1);
			}

            CRYSTALFIRE_BREATH_Timer = 8000;
		}else CRYSTALFIRE_BREATH_Timer -=diff;

		if (CRYSTAL_CHAINS_Timer < diff)
		{
			//Cast it on the whole group on heroic mode
			if(HeroicMode)
			{
				std::list<HostilReference*>::iterator i = m_creature->getThreatManager().getThreatList().begin();
				for (i = m_creature->getThreatManager().getThreatList().begin(); i!= m_creature->getThreatManager().getThreatList().end();++i)
				{
					Unit* pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
					if (pUnit && (pUnit->GetTypeId() == TYPEID_PLAYER))
					{
						DoCast(pUnit, SPELL_INTENSE_COLD);
					}
				}
			}
			else
			{
				if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
					DoCast(target, SPELL_CRYSTAL_CHAINS);
			}

                CRYSTAL_CHAINS_Timer = 10000;
            }else CRYSTAL_CHAINS_Timer -= diff;

		if (INTENSE_COLD_Timer < diff)
        {
			//Cast it on the whole group
			std::list<HostilReference*>::iterator i = m_creature->getThreatManager().getThreatList().begin();
			for (i = m_creature->getThreatManager().getThreatList().begin(); i!= m_creature->getThreatManager().getThreatList().end();++i)
			{
				Unit* pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
				if (pUnit && (pUnit->GetTypeId() == TYPEID_PLAYER)) //TODO: Just non jumping players should get hit by the spell, should also be casted on player's pet
				{
					DoCast(pUnit, SPELL_INTENSE_COLD);
				}
			}

			INTENSE_COLD_Timer = 7000;
		}else INTENSE_COLD_Timer -= diff;

		//Disable TAIL_SWEEP while not fixed all problems
		/*if (TAIL_SWEEP_Timer < diff)
        {
			//Cast it on the whole group
			std::list<HostilReference*>::iterator i = m_creature->getThreatManager().getThreatList().begin();
			for (i = m_creature->getThreatManager().getThreatList().begin(); i!= m_creature->getThreatManager().getThreatList().end();++i)
			{
				Unit* pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
				if (pUnit && (pUnit->GetTypeId() == TYPEID_PLAYER)) //TODO: Only players that are in a cone behind the boss should get hit
				{
					DoCast(pUnit, SPELL_INTENSE_COLD);
				}
			}

			TAIL_SWEEP_Timer = 10000;
		}else TAIL_SWEEP_Timer -= diff;*/

        DoMeleeAttackIfReady();
    }

    void JustDied(Unit* killer)
    {
    //if(pInstance)
          // pInstance->SetData(data_keristrasza, DONE);
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