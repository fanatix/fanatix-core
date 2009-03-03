/* Script Data Start
SDName: Boss dalronn
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = 'boss_dalronn' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Spells
#define SPELL_DEBILITATE                            43650
#define SPELL_SHADOWBOLT                            43649
#define SPELL_SHADOWBOLT_2                          59575
#define SPELL_SUMMON_SKELETONS                      52611  //Trigger Spell: 52612, summons NPC (28878)

//Yell
#define SAY_AGGRO                                -1999685
#define SAY_KILL                                 -1999684
#define SAY_DEATH_1                              -1999683
#define SAY_DEATH_2                              -1999682
#define SAY_RAISES                               -1999681

struct MANGOS_DLL_DECL boss_dalronnAI : public ScriptedAI
{
    boss_dalronnAI(Creature *c) : ScriptedAI(c) { Reset(); }

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
    void JustDied(Unit* killer) {}
    void KilledUnit(Unit *victim)
    {
        if(victim == m_creature)
            return;
        DoScriptText(SAY_KILL, m_creature);
    }
};

CreatureAI* GetAI_boss_dalronn(Creature *_Creature)
{
    return new boss_dalronnAI (_Creature);
}

void AddSC_boss_dalronn()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_dalronn";
    newscript->GetAI = GetAI_boss_dalronn;
    newscript->RegisterSelf();
}
