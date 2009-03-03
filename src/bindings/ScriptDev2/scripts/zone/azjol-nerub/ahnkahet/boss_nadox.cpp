/* Script Data Start
SDName: Boss nadox
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = 'boss_nadox' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Spells
#define SPELL_BROOD_PLAGUE_N                    56130 //Diseases the enemy, causing 12750 Nature damage over 30 sec.
#define SPELL_BROOD_PLAGUE_H                    59467 //Diseases the enemy, causing 25500 Nature damage over 30 sec.
#define BUFF_BROOD_RAGE_H                       59465 //Enrages a Swarmer, increasing size and damage done.

//Yell
#define SAY_AGGRO                              -1999975
#define SAY_SLAY_1                             -1999974
#define SAY_SLAY_2                             -1999973
#define SAY_SLAY_3                             -1999972
#define SAY_DEATH                              -1999971
#define SAY_EGG_SAC_1                          -1999970
#define SAY_EGG_SAC_2                          -1999969

struct MANGOS_DLL_DECL boss_nadoxAI : public ScriptedAI
{
    boss_nadoxAI(Creature *c) : ScriptedAI(c) { Reset(); }

    void Reset() {}
    
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
            
        DoMeleeAttackIfReady();
    }
    
    void JustDied(Unit* killer)  
    {
        DoScriptText(SAY_DEATH, m_creature);
    }

    void KilledUnit(Unit *victim)
    {
        if (victim == m_creature)
            return;

        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature);break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature);break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature);break;
        }
    }
};

CreatureAI* GetAI_boss_nadox(Creature *_Creature)
{
    return new boss_nadoxAI (_Creature);
}

void AddSC_boss_nadox()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_nadox";
    newscript->GetAI = GetAI_boss_nadox;
    newscript->RegisterSelf();
}
