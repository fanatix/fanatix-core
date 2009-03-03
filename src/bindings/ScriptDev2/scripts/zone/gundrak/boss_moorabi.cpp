/* Script Data Start
SDName: Boss moorabi
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = 'boss_moorabi' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Spells
#define SPELL_DETERMINED_STAB_1                     55102
#define SPELL_DETERMINED_STAB_2                     59444
#define SPELL_DETERMINED_STAB_3                     55104

//#define SPELL_DETERMINED_GORE                       55102
#define SPELL_GROUND_TREMOR                         55142
#define SPELL_MOJO_FRENZY                           55163
#define SPELL_NUMBING_ROAR_1                        55100
#define SPELL_NUMBING_ROAR_2                        55106
#define SPELL_TRANSFORMATION                        55098
#define SPELL_QUAKE                                 55101

//Yell
#define SAY_AGGRO                                 -1999832
#define SAY_SLAY_1                                -1999831
#define SAY_SLAY_2                                -1999830
#define SAY_SLAY_3                                -1999829
#define SAY_DEATH                                 -1999828
#define SAY_TRANSFORM                             -1999827

struct MANGOS_DLL_DECL boss_moorabiAI : public ScriptedAI
{
    boss_moorabiAI(Creature *c) : ScriptedAI(c) { Reset(); }

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

CreatureAI* GetAI_boss_moorabi(Creature *_Creature)
{
    return new boss_moorabiAI (_Creature);
}

void AddSC_boss_moorabi()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_moorabi";
    newscript->GetAI = GetAI_boss_moorabi;
    newscript->RegisterSelf();
}
