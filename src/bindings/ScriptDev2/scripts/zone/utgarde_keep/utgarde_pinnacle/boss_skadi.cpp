/* Script Data Start
SDName: Boss skadi
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = 'boss_skadi' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Phase 0 "gauntlet even" Skadi on a flying mount, waves of adds charging to the group periodicaly carrying harpoons
//Phase 1 Kill the Skadi drake mount with harppons launcher
//Phase 2 Kill the Skadi

//Skadi Spells
#define SPELL_CRUSH                              50234
#define SPELL_POISONED_SPEAR                     50225
#define SPELL_WHIRLWIND                          50228 //random target,  but not the tank approx. every 20s

//Yell
#define SAY_AGGRO                             -1999656
#define SAY_KILL_1                            -1999655
#define SAY_KILL_2                            -1999654
#define SAY_KILL_3                            -1999653
#define SAY_DEATH                             -1999652
#define SAY_DRAKE_DEATH                       -1999651
#define SAY_DRAKE_HIT_1                       -1999650
#define SAY_DRAKE_HIT_2                       -1999649
#define SAY_DRAKE_BREATH_1                    -1999648
#define SAY_DRAKE_BREATH_2                    -1999647
#define SAY_DRAKE_BREATH_3                    -1999646

struct MANGOS_DLL_DECL boss_skadiAI : public ScriptedAI
{
    boss_skadiAI(Creature *c) : ScriptedAI(c) { Reset(); } 
    
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

CreatureAI* GetAI_boss_skadi(Creature *_Creature)
{
    return new boss_skadiAI (_Creature);
}

void AddSC_boss_skadi()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_skadi";
    newscript->GetAI = GetAI_boss_skadi;
    newscript->RegisterSelf();
}
