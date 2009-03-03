/* Script Data Start
SDName: Boss drakos
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = '' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Spells
#define SPELL_MAGIC_PULL                           51336
#define SPELL_THUNDERING_STOMP                     50774
#define SPELL_THUNDERING_STOMP_2                   59370

//Yell
#define SAY_AGGRO                               -1999769
#define SAY_KILL_1                              -1999768
#define SAY_KILL_2                              -1999767
#define SAY_KILL_3                              -1999766
#define SAY_DEATH                               -1999765
#define SAY_PULL_1                              -1999764
#define SAY_PULL_2                              -1999763
#define SAY_PULL_3                              -1999762
#define SAY_PULL_4                              -1999761
#define SAY_STOMP_1                             -1999760
#define SAY_STOMP_2                             -1999759
#define SAY_STOMP_3                             -1999758

struct MANGOS_DLL_DECL boss_drakosAI : public ScriptedAI
{
    boss_drakosAI(Creature *c) : ScriptedAI(c) { Reset(); }

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
        if(victim == m_creature)
            return;
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_KILL_1, m_creature);break;
            case 1: DoScriptText(SAY_KILL_2, m_creature);break;
            case 2: DoScriptText(SAY_KILL_3, m_creature);break;
        }
    }
};

CreatureAI* GetAI_boss_drakos(Creature *_Creature)
{
    return new boss_drakosAI (_Creature);
}

void AddSC_boss_drakos()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_drakos";
    newscript->GetAI = GetAI_boss_drakos;
    newscript->RegisterSelf();
}
