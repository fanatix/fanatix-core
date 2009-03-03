/* Script Data Start
SDName: Boss sartharion
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = '' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Sartharion Spells
#define SPELL_BERSERK                                 61632 // Increases the caster's attack speed by 150% and all damage it deals by 500% for 5 min.
#define SPELL_CLEAVE                                  56909 // Inflicts 35% weapon damage to an enemy and its nearest allies, affecting up to 10 targets.
#define SPELL_FLAME_BREATH_N                          56908 // Inflicts 8750 to 11250 Fire damage to enemies in a cone in front of the caster.
#define SPELL_FLAME_BREATH_H                          58956 // Inflicts 10938 to 14062 Fire damage to enemies in a cone in front of the caster.
#define SPELL_TAIL_LASH_N                             56910 // A sweeping tail strike hits all enemies behind the caster, inflicting 3063 to 3937 damage and stunning them for 2 sec.
#define SPELL_TAIL_LASH_H                             58957 // A sweeping tail strike hits all enemies behind the caster, inflicting 4375 to 5625 damage and stunning them for 2 sec.
#define SPELL_WILL_OF_SARTHARION                      61254 // Sartharion's presence bolsters the resolve of the Twilight Drakes, increasing their total health by 25%. This effect also increases Sartharion's health by 25%.
#define SPELL_POWER_OF_SARTHARION                     61254 //Shadron's presence increases Fire damage taken by all enemies by 100%.

//Sartharion Yell
#define SAY_SARTHARION_AGGRO                         -1999885
#define SAY_SARTHARION_BERSERK                       -1999884
#define SAY_SARTHARION_BREATH                        -1999883
#define SAY_SARTHARION_CALL_SHADRON                  -1999882                     
#define SAY_SARTHARION_CALL_TENEBRON                 -1999881                    
#define SAY_SARTHARION_CALL_VESPERON                 -1999880                    
#define SAY_SARTHARION_DEATH                         -1999879    
#define SAY_SARTHARION_SPECIAL_1                     -1999878
#define SAY_SARTHARION_SPECIAL_2                     -1999877
#define SAY_SARTHARION_SPECIAL_3                     -1999876        
#define SAY_SARTHARION_SPECIAL_4                     -1999875     
#define SAY_SARTHARION_SLAY_1                        -1999874  
#define SAY_SARTHARION_SLAY_2                        -1999873 
#define SAY_SARTHARION_SLAY_3                        -1999872 

//Miniboses (Vesperon, Shadron, Tenebron)
#define SPELL_SHADOW_BREATH_H                         59126 //Inflicts 8788 to 10212 Fire damage to enemies in a cone in front of the caster.
#define SPELL_SHADOW_BREATH_N                         57570 //Inflicts 6938 to 8062 Fire damage to enemies in a cone in front of the caster.
//TAIL LASH the same as Sartharion's
#define SPELL_SHADOW_FISSURE_H                        59127 //Deals 9488 to 13512 Shadow damage to any enemy within the Shadow fissure after 5 sec.
#define SPELL_SHADOW_FISSURE_N                        57579 //Deals 6188 to 8812 Shadow damage to any enemy within the Shadow fissure after 5 sec.

//Vesperon
#define MINIBOSS_VESPERON                             30449 //npc 30449  //In portal is a disciple, when disciple killed remove Power_of_vesperon, portal spawns multiple times
#define BUFF_POWER_OF_VESPERON                        61251 //Vesperon's presence decreases the maximum health of all enemies by 25%.

//Shadron
#define MINIBOSS_SHADRON                              30451 //npc 30451  //In portal is a disciple, when disciple killed remove Power_of_vesperon, portal spawns multiple times
#define BUFF_POWER_OF_SHADRON                         58105 //Shadron's presence increases Fire damage taken by all enemies by 100%.                     

//Tenebron
#define MINIBOSS_TENEBRON                             30452 //npc 30452 //in the portal spawns 6 eggs, if not killed in time (approx. 20s)  they will hatch,  whelps can cast 60708
#define BUFF_POWER_OF_TENEBRON                        61248 //Tenebron's presence increases Shadow damage taken by all enemies by 100%.

//Minibosses common spells
#define SPELL_SHADOW_BREATH_N                         57570
#define SPELL_SHADOW_BREATH_H                         59126
#define SPELL_SHADOW_FISSURE_N                        57579
#define SPELL_SHADOW_FISSURE_H                        59127
#define SPELL_TWILIGHT_REVENGE                        60639

//Tenebron, dummy spell
#define SPELL_HATCH_EGGS                              58793

//Whelps
#define TWILIGHT_WHELP                                30890 //npc 30890
#define SHARTHARION_TWILIGHT_WHELP                    31214 //npc 31214
#define SPELL_FADE_ARMOR                              60708 //Reduces the armor of an enemy by 1500 for 15s

/*Summons a portal, which all DPS and one healer should go through and proceed to kill all eggs. After about 20 seconds these eggs will hatch into whelplings with 60k health. Whelplings that hatched should be picked up by a tank and killed.
Hatch Eggs Tenebron summons a portal in which eggs are spawned. After about 20s the eggs hatches into Whelps (156,396 hp - amount of whelps determined by Normal or Heroic mode) that do Fade Armor. */

struct MANGOS_DLL_DECL boss_sartharionAI : public ScriptedAI
{
    boss_sartharionAI(Creature *c) : ScriptedAI(c) { Reset(); }

    void Reset() {}
    void Aggro(Unit* who) {}
    void AttackStart(Unit* who) {}
    void MoveInLineOfSight(Unit* who) {}
    void UpdateAI(const uint32 diff) 
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
                
        DoMeleeAttackIfReady();    
    }
    void JustDied(Unit* killer)  {}
};

/*######
## Mob Vesperon
######*/

//Vesperon Yell
#define SAY_VESPERON_AGGRO                       -1999871
#define SAY_VESPERON_SLAY_1                      -1999870
#define SAY_VESPERON_SLAY_2                      -1999869
#define SAY_VESPERON_DEATH                       -1999868
#define SAY_VESPERON_BREATH                      -1999867
#define SAY_VESPERON_RESPOND                     -1999866
#define SAY_VESPERON_SPECIAL_1                   -1999865
#define SAY_VESPERON_SPECIAL_2                   -1999864

struct MANGOS_DLL_DECL mob_vesperonAI : public ScriptedAI
{
    mob_vesperonAI(Creature *c) : ScriptedAI(c) { Reset(); }
    
    void Reset() {}
    void Aggro(Unit* who) {}
    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
                
        DoMeleeAttackIfReady();    
    }
    void JustDied(Unit* killer)  {}
};

/*######
## Mob Shadron
######*/

//Shadron Yell
#define SAY_SHADRON_AGGRO                       -1999863
#define SAY_SHADRON_SLAY_1                      -1999862
#define SAY_SHADRON_SLAY_2                      -1999861
#define SAY_SHADRON_DEATH                       -1999860
#define SAY_SHADRON_BREATH                      -1999859
#define SAY_SHADRON_RESPOND                     -1999858
#define SAY_SHADRON_SPECIAL_1                   -1999857
#define SAY_SHADRON_SPECIAL_2                   -1999856

struct MANGOS_DLL_DECL mob_shadronAI : public ScriptedAI
{
    mob_shadronAI(Creature *c) : ScriptedAI(c) { Reset(); }

    void Reset() {}
    void Aggro(Unit* who) {}
    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
            
        DoMeleeAttackIfReady();    
    }
    void JustDied(Unit* killer)  {}
};
/*######
## Mob Tenebron
######*/

//Yell
#define SAY_TENEBRON_AGGRO                       -1999855
#define SAY_TENEBRON_SLAY_1                      -1999854
#define SAY_TENEBRON_SLAY_2                      -1999853
#define SAY_TENEBRON_DEATH                       -1999852
#define SAY_TENEBRON_BREATH                      -1999851
#define SAY_TENEBRON_RESPOND                     -1999850
#define SAY_TENEBRON_SPECIAL_1                   -1999849
#define SAY_TENEBRON_SPECIAL_2                   -1999848

struct MANGOS_DLL_DECL mob_tenebronAI : public ScriptedAI
{
    mob_tenebronAI(Creature *c) : ScriptedAI(c) { Reset(); }

    void Reset() {}
    void Aggro(Unit* who) {}
    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
            
        DoMeleeAttackIfReady();    
    }
    void JustDied(Unit* killer)  {}
};

/*######
## Mob Whelps
######*/
struct MANGOS_DLL_DECL mob_whelpAI : public ScriptedAI
{
    mob_whelpAI(Creature *c) : ScriptedAI(c) { Reset(); }
    
    uint32 Fade_armor_Timer;
    
    void Reset() {}
    void Aggro(Unit* who) {}
    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
            
        DoMeleeAttackIfReady();    
    }
    void JustDied(Unit* killer)  {}
};

CreatureAI* GetAI_mob_whelp(Creature *_Creature)
{
    return new mob_whelpAI (_Creature);
}

CreatureAI* GetAI_mob_tenebron(Creature *_Creature)
{
    return new mob_tenebronAI (_Creature);
}

CreatureAI* GetAI_mob_shadron(Creature *_Creature)
{
    return new mob_shadronAI (_Creature);
}

CreatureAI* GetAI_mob_vesperon(Creature *_Creature)
{
    return new mob_vesperonAI (_Creature);
}

CreatureAI* GetAI_boss_sartharion(Creature *_Creature)
{
    return new boss_sartharionAI (_Creature);
}

void AddSC_boss_sartharion()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_sartharion";
    newscript->GetAI = GetAI_boss_sartharion;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name="mob_vesperon";
    newscript->GetAI = GetAI_mob_vesperon;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name="mob_shadron";
    newscript->GetAI = GetAI_mob_shadron;
    newscript->RegisterSelf();    

    newscript = new Script;
    newscript->Name="mob_tenebron";
    newscript->GetAI = GetAI_mob_tenebron;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name="mob_whelp";
    newscript->GetAI = GetAI_mob_whelp;
    newscript->RegisterSelf();
}
