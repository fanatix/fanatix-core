/* Script Data Start
SDName: Boss Commander Kolurg
SDAuthor: Thyros
SD%Complete: 80%
SDComment:  Ready to Test
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = 'boss_commander_kolurg' where entry = '';
*** SQL END ***/
#include "precompiled.h"
#include "def_nexus.h"

//Spell
#define SPELL_FRIGHTENING_SHOUT                               19134
#define SPELL_BATTLE_SHOUT                                    31403
#define SPELL_CHARGE                                          60067
#define SPELL_WHIRLWIND_1                                     38619
#define SPELL_WHIRLWIND_2                                     38618

//Yell
#define SAY_AGGRO                                          1576024
#define SAY_KILL                                           1576025
#define SAY_DEATH                                          1576026

struct MANGOS_DLL_DECL boss_commander_kolurgAI : public ScriptedAI
{
    boss_commander_kolurgAI(Creature *c) : ScriptedAI(c)
    {
        //pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
		HeroicMode = m_creature->GetMap()->IsHeroic();
    }
    
    bool HeroicMode;

    uint32 FRIGHTENING_Timer;
	uint32 BATTLE_SHOUT_Timer; 
    uint32 CHARGE_Timer;
    uint32 WHIRLWIND_Timer; 

    void Reset()
	{
		//These times are probably wrong
       FRIGHTENING_Timer = 6000;
       BATTLE_SHOUT_Timer = 60000;
	   CHARGE_Timer = 2000;
       WHIRLWIND_Timer = 1000;

		//if(pInstance)
            //pInstance->SetData(DATA_COMMANDER_KOLURG, NOT_STARTED);
	}

    void Aggro(Unit* who)
	{
				//if(pInstance)
        //pInstance->SetData(data_keristrasza, IN_PROGRESS);
        DoScriptText(SAY_AGGRO, m_creature);
	//	DoZoneInCombat();
	}

    void AttackStart(Unit* who) {}
    void MoveInLineOfSight(Unit* who) {}
    void UpdateAI(const uint32 diff) 
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
if (FRIGHTENING_Timer < diff)
        {
            m_creature->CastSpell(m_creature, SPELL_FRIGHTENING_SHOUT, true);
		
            FRIGHTENING_Timer = 6000;
}else FRIGHTENING_Timer -=diff;

if (BATTLE_SHOUT_Timer < diff)
        {
            m_creature->CastSpell(m_creature, SPELL_CHARGE, true);
		
            BATTLE_SHOUT_Timer = 60000;
}else BATTLE_SHOUT_Timer -=diff;

if (CHARGE_Timer < diff)
        {
            m_creature->CastSpell(m_creature, SPELL_CHARGE, true);
		
            CHARGE_Timer = 2000;
}else CHARGE_Timer -=diff;

if (WHIRLWIND_Timer < diff)
            {
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target,SPELL_WHIRLWIND_1);
				if (HeroicMode)
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target,SPELL_WHIRLWIND_2);

                WHIRLWIND_Timer = 1000;
            }else WHIRLWIND_Timer -=diff;

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
        DoScriptText(SAY_KILL, m_creature);
    }
};

CreatureAI* GetAI_boss_commander_kolurg(Creature *_Creature)
{
    return new boss_commander_kolurgAI (_Creature);
}

void AddSC_boss_commander_kolurg()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="commander_kolurg";
    newscript->GetAI = &GetAI_boss_commander_kolurg;
    newscript->RegisterSelf();
}
