/* Script Data Start
SDName: Boss keleseth
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = 'boss_keleseth' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Spells
#define SPELL_SHADOWBOLT                            43667
#define SPELL_SHADOWBOLT_H                          59389
#define SPELL_FROST_TOMB                            48400
//After 20s Summons 5 Vykrul Skeletons (npc 23970) which can cast Decrepify (spell 42702)

//Yells
#define SAY_AGGRO                                -1999675
#define SAY_KILL                                 -1999674
#define SAY_DEATH                                -1999673
#define SAY_FROST_TOMB                           -1999672
#define SAY_SKELETONS                            -1999671

struct MANGOS_DLL_DECL boss_kelesethAI : public ScriptedAI
{
    boss_kelesethAI(Creature *c) : ScriptedAI(c) { Reset(); }

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
        DoScriptText(SAY_KILL, m_creature);
    }
};

CreatureAI* GetAI_boss_keleseth(Creature *_Creature)
{
    return new boss_kelesethAI (_Creature);
}

void AddSC_boss_keleseth()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_keleseth";
    newscript->GetAI = GetAI_boss_keleseth;
    newscript->RegisterSelf();
}
