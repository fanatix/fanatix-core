/* Script Data Start
SDName: Boss Commander Stoutbeard
SDAuthor: LordVanMartin
SD%Complete: 
SDComment:  Only Horde Heroic
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = 'boss_commander_stoutbeard' where entry = '';
*** SQL END ***/
#include "precompiled.h"

#define SPELL_BATTLE_SHOUT                                       31403
#define SPELL_CHARGE                                             60067
#define SPELL_FRIGHTENING_SHOUT                                  19134
#define SPELL_WHIRLWIND_1                                        38619
#define SPELL_WHIRLWIND_2                                        38618

//Yell
#define SAY_AGGRO                                              1576021
#define SAY_KILL                                               1576022
#define SAY_DEATH                                              1576023

struct MANGOS_DLL_DECL boss_commander_stoutbeardAI : public ScriptedAI
{
    boss_commander_stoutbeardAI(Creature *c) : ScriptedAI(c)
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
          //  pInstance->SetData(data_keristrasza, NOT_STARTED);
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

CreatureAI* GetAI_boss_commander_stoutbeard(Creature *_Creature)
{
    return new boss_commander_stoutbeardAI (_Creature);
}

void AddSC_boss_commander_stoutbeard()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="commander_stoutbeard";
    newscript->GetAI = &GetAI_boss_commander_stoutbeard;
    newscript->RegisterSelf();
}
