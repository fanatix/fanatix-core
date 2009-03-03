/* Script Data Start
SDName: Boss cyanigosa
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
#define SPELL_ARCANE_VACUM                             58694
#define SPELL_BLIZZARD                                 58693
#define SPELL_MANA_DESTRUCTION                         59374
#define SPELL_TAIL_SWEEP                               58690
#define SPELL_UNCONTROLLABLE_ENERGY                    58688

//Yells
#define SAY_AGGRO                                   -1999622
#define SAY_SLAY_1                                  -1999621
#define SAY_SLAY_2                                  -1999620
#define SAY_SLAY_3                                  -1999619
#define SAY_DEATH                                   -1999618
#define SAY_SPAWN                                   -1999617
#define SAY_DISRUPTION                              -1999616
#define SAY_BREATH_ATTACK                           -1999615
#define SAY_SPECIAL_ATTACK_1                        -1999614
#define SAY_SPECIAL_ATTACK_2                        -1999613

struct MANGOS_DLL_DECL boss_cyanigosaAI : public ScriptedAI
{
    boss_cyanigosaAI(Creature *c) : ScriptedAI(c) { Reset(); }

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

CreatureAI* GetAI_boss_cyanigosa(Creature *_Creature)
{
    return new boss_cyanigosaAI (_Creature);
}

void AddSC_boss_cyanigosa()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_cyanigosa";
    newscript->GetAI = GetAI_boss_cyanigosa;
    newscript->RegisterSelf();
}
