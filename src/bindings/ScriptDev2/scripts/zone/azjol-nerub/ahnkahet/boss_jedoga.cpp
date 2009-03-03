/* Script Data Start
SDName: Boss jedoga
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = 'boss_jedoga' where entry = '';
*** SQL END ***/
#include "precompiled.h"

#define SPELL_CYCLONE_STRIKE                       56855
#define SPELL_LIGHTNING_BOLT                       56891
#define SPELL_THUNDERSHOCK                         56926

#define SAY_AGGRO                                 -1999989
#define SAY_C_SACRIFICE_1                         -1999988
#define SAY_C_SACRIFICE_2                         -1999987
#define SAY_SACRIFICE_1                           -1999986
#define SAY_SACRIFICE_2                           -1999985
#define SAY_SLAY_1                                -1999984
#define SAY_SLAY_2                                -1999983
#define SAY_SLAY_3                                -1999982
#define SAY_DEATH                                 -1999981
#define SAY_PREACHING_1                           -1999980 
#define SAY_PREACHING_2                           -1999979
#define SAY_PREACHING_3                           -1999978
#define SAY_PREACHING_4                           -1999977
#define SAY_PREACHING_5                           -1999976

struct MANGOS_DLL_DECL boss_jedogaAI : public ScriptedAI
{
    boss_jedogaAI(Creature *c) : ScriptedAI(c) { Reset(); }

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

CreatureAI* GetAI_boss_jedoga(Creature *_Creature)
{
    return new boss_jedogaAI (_Creature);
}

void AddSC_boss_jedoga()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_jedoga";
    newscript->GetAI = GetAI_boss_jedoga;
    newscript->RegisterSelf();
}
