/* Script Data Start
SDName: Boss xevozz
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
#define SPELL_ARCANE_BARRAGE_VOLLEY                    54202
#define SPELL_ARCANE_BUFFET                            54226
#define SPELL_SUMMON_ETHEREAL_SPHERE_1                 54102
#define SPELL_SUMMON_ETHEREAL_SPHERE_2                 54137
#define SPELL_SUMMON_ETHEREAL_SPHERE_3                 54138

//Yells
#define SAY_AGGRO                                   -1999595
#define SAY_SLAY_1                                  -1999594
#define SAY_SLAY_2                                  -1999593
#define SAY_SLAY_3                                  -1999592
#define SAY_DEATH                                   -1999591
#define SAY_SPAWN                                   -1999590
#define SAY_CHARGED                                 -1999589
#define SAY_REPEAT_SUMMON_1                         -1999588
#define SAY_REPEAT_SUMMON_2                         -1999587
#define SAY_SUMMON_ENERGY                           -1999586

struct MANGOS_DLL_DECL boss_xevozzAI : public ScriptedAI
{
    boss_xevozzAI(Creature *c) : ScriptedAI(c) { Reset(); }

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
            case 0: DoScriptText(SAY_SLAY_1, m_creature);break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature);break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature);break;
        }
    }
};

CreatureAI* GetAI_boss_xevozz(Creature *_Creature)
{
    return new boss_xevozzAI (_Creature);
}

void AddSC_boss_xevozz()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_xevozz";
    newscript->GetAI = GetAI_boss_xevozz;
    newscript->RegisterSelf();
}
