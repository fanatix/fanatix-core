/* Script Data Start
SDName: Boss anub_arak
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = 'boss_anub_arak' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Spells
#define SPELL_CARRION_BEETLES                         53520
#define SPELL_LOCUST_SWARM                            53467
#define SPELL_IMPALE                                  53454
#define SPELL_POUND                                   53472

//Yell
#define SAY_INTRO                                  -1999999
#define SAY_AGGRO                                  -1999968
#define SAY_SLAY_1                                 -1999968
#define SAY_SLAY_2                                 -1999968
#define SAY_SLAY_3                                 -1999968
#define SAY_LOCUST_1                               -1999968  
#define SAY_LOCUST_2                               -1999968  
#define SAY_LOCUST_3                               -1999968  
#define SAY_SUBMERGE_1                             -1999968    
#define SAY_SUBMERGE_2                             -1999968    
#define SAY_DEATH                                  -1999968

struct MANGOS_DLL_DECL boss_anub_arakAI : public ScriptedAI
{
    boss_anub_arakAI(Creature *c) : ScriptedAI(c) { Reset(); }

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

CreatureAI* GetAI_boss_anub_arak(Creature *_Creature)
{
    return new boss_anub_arakAI (_Creature);
}

void AddSC_boss_anub_arak()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_anub_arak";
    newscript->GetAI = GetAI_boss_anub_arak;
    newscript->RegisterSelf();
}
