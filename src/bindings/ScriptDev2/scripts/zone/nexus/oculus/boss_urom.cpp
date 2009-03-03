/* Script Data Start
SDName: Boss urom
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
#define SPELL_ARCANE_SHIELD                      53813 //Dummy --> Channeled, shields the caster from damage.
#define SPELL_EMPOWERED_ARCANE_EXPLOSION         51110
#define SPELL_EMPOWERED_ARCANE_EXPLOSION_2       59377
#define SPELL_FROSTBOMB                          51103
#define SPELL_SUMMON_MENAGERIE                   50476 //Summons an assortment of creatures and teleports the caster to safety.
#define SPELL_SUMMON_MENAGERIE_2                 50495
#define SPELL_SUMMON_MENAGERIE_3                 50496
#define SPELL_TELEPORT                           51112 //Teleports to the center of Oculus
#define SPELL_TIME_BOMB                          51121
#define SPELL_TIME_BOMB_2                        59376

//Yell
#define SAY_AGGRO                             -1999757
#define SAY_KILL_1                            -1999756 
#define SAY_KILL_2                            -1999755 
#define SAY_KILL_3                            -1999754 
#define SAY_DEATH                             -1999753
#define SAY_EXPLOSION_1                       -1999752      
#define SAY_EXPLOSION_2                       -1999751      
#define SAY_SUMMON_1                          -1999750   
#define SAY_SUMMON_2                          -1999739   
#define SAY_SUMMON_3                          -1999748   

struct MANGOS_DLL_DECL boss_uromAI : public ScriptedAI
{
    boss_uromAI(Creature *c) : ScriptedAI(c) { Reset(); }

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

CreatureAI* GetAI_boss_urom(Creature *_Creature)
{
    return new boss_uromAI (_Creature);
}

void AddSC_boss_urom()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_urom";
    newscript->GetAI = GetAI_boss_urom;
    newscript->RegisterSelf();
}
