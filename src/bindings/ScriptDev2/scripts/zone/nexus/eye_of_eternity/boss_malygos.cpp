/* Script Data Start
SDName: Boss malygos
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
#define SPELL_ARCANE_BREATH_N                        56272
#define SPELL_ARCANE_BREATH_H                        60072
#define SPELL_ARCANE_PULSE                           57432
#define SPELL_ARCANE_STORM_1                         57459
#define SPELL_ARCANE_STORM_2                         61693
#define SPELL_ARCANE_STORM_3                         61694
#define SPELL_STATIC_FIELD                           57430
#define SPELL_SURGE_OF_POWER_1                       56505 
#define SPELL_SURGE_OF_POWER_2                       57407
#define SPELL_SURGE_OF_POWER_3                       60936
#define SPELL_VORTEX                                 56105

//Dragon "mounts" spells in Phase3
//they use Rugelike energy
#define SPELL_DMOUNT_FLAME_SPIKE                     56091 //maybe not accurate
#define SPELL_DMOUNT_ENGULF_IN_FLAMES                61621
#define SPELL_DMOUNT_REVIVIFY                        57090
#define SPELL_DMOUNT_LIFE_BURST                      57143
#define SPELL_DMOUNT_FLAME_SHIELD                    57108
//#define SPELL_DMOUNT_UNKNOWN                      XYZ //Increases your drake's flight speed by 500%. 


//Yell
//-->Other
#define SAY_ANTI_MAGIC_SHELL                       -1999819
#define SAY_BREATH_ATTACK                          -1999818 
#define SAY_HIGH_DAMAGE_MODE                       -1999817    
#define SAY_MAGIC_BLAST                            -1999816
//--> Generic Spells
#define SAY_GENERIC_SPELL_1                        -1999815   
#define SAY_GENERIC_SPELL_2                        -1999814   
#define SAY_GENERIC_SPELL_3                        -1999813   
#define SAY_DEATH                                  -1999812
//--> Prefight
#define SAY_PREFIGHT_1                             -1999811   
#define SAY_PREFIGHT_2                             -1999810  
#define SAY_PREFIGHT_3                             -1999809  
#define SAY_PREFIGHT_4                             -1999808 
#define SAY_PREFIGHT_5                             -1999807 
//--> Phase1
#define SAY_PHASE1_AGGRO                           -1999806 
#define SAY_PHASE1_END                             -1999805
#define SAY_PHASE1_SLAY_1                          -1999804
#define SAY_PHASE1_SLAY_2                          -1999803
#define SAY_PHASE1_SLAY_3                          -1999802

//--> Phase2 at 50% HP,

/*Malygos himself is not targetable during this phase, it will end when the adds he spawns are all killed. However, he does continue to play a part in the encounter.
During this phase he drops anti-magic zones onto the ground the raid MUST stand inside of, it reduces magical damage taken by 50%. They shrink over time, so it's important that your raid moves to each new one he drops.
Throughout the phase, he will deep breath doing ~4k damage per second, unless you are standing inside of the anti-magic zone.
The way the fight works during this phase is there are NPCs riding around on disks in the room. There are two types of mobs, Lords and Scions.
The Lords will move down onto the group, and need to be tanked (They will one-shot a non-tank). After they die, they drop a disk that a raid member can mount onto, which allows them to fly, to attack the Scions that do not come down to the ground. 
It is recommended to let melee take the first disks, then ranged. As those mobs die, they also drop disks, which allows the rest of your dps to get onto them. 
The Scions will continually cast Arcane Blast on random targets on the floor, which is mitigated by the anti-magic zones. While mounted on a disk, you will not take damage.
After all of the NPCs riding on the disks die, the players on the disks need to dismount as Phase 3 is about to begin.*/
#define SAY_PHASE2_AGGRO                           -1999801
#define SAY_PHASE2_END                             -1999800
#define SAY_PHASE2_SLAY_1                          -1999799
#define SAY_PHASE2_SLAY_2                          -1999798
#define SAY_PHASE2_SLAY_3                          -1999797
//--> Phase3 Malygos destroys the floor, encounter continues on dragon "mounts"
#define SAY_PHASE3_INTRO                           -1999796
#define SAY_PHASE3_AGGRO                           -1999795
#define SAY_PHASE3_SLAY_1                          -1999794
#define SAY_PHASE3_SLAY_2                          -1999793
#define SAY_PHASE3_SLAY_3                          -1999792
#define SAY_PHASE3_BIG_ATTACK                      -1999791

bool MalygosPhase1 = true, //Phases not yet implemented
     MalygosPhase2 = false,
     MalygosPhase3 = false;

struct MANGOS_DLL_DECL boss_malygosAI : public ScriptedAI
{
    boss_malygosAI(Creature *c) : ScriptedAI(c) { Reset(); }

    void Reset() {}
    void Aggro(Unit* who) 
    {
        if(MalygosPhase1)
            DoScriptText(SAY_PHASE1_AGGRO, m_creature);
        if(MalygosPhase2)
            DoScriptText(SAY_PHASE1_AGGRO, m_creature);
        if(MalygosPhase3)
            DoScriptText(SAY_PHASE1_AGGRO, m_creature);
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

        if(MalygosPhase1)
            switch(rand()%3)
            {
                case 0: DoScriptText(SAY_PHASE1_SLAY_1, m_creature); break;
                case 1: DoScriptText(SAY_PHASE1_SLAY_2, m_creature);break;
                case 2: DoScriptText(SAY_PHASE1_SLAY_3, m_creature);break;
            }
        if(MalygosPhase2)
            switch(rand()%3)
            {
                case 0: DoScriptText(SAY_PHASE2_SLAY_1, m_creature);break;
                case 1: DoScriptText(SAY_PHASE2_SLAY_2, m_creature);break;
                case 2: DoScriptText(SAY_PHASE2_SLAY_3, m_creature);break;
            }
        if(MalygosPhase3)
            switch(rand()%3)
            {
                case 0: DoScriptText(SAY_PHASE3_SLAY_1, m_creature);break;
                case 1: DoScriptText(SAY_PHASE3_SLAY_2, m_creature);break;
                case 2: DoScriptText(SAY_PHASE3_SLAY_3, m_creature);break;
            }
    }
};

CreatureAI* GetAI_boss_malygos(Creature *_Creature)
{
    return new boss_malygosAI (_Creature);
}

void AddSC_boss_malygos()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_malygos";
    newscript->GetAI = &GetAI_boss_malygos;
    newscript->RegisterSelf();
}
