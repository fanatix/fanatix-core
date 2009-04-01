/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef SC_CREATURE_H
#define SC_CREATURE_H

#include "CreatureAI.h"
#include "Creature.h"

//Spell targets used by SelectSpell
enum SelectTarget
{
    SELECT_TARGET_DONTCARE = 0,                             //All target types allowed

    SELECT_TARGET_SELF,                                     //Only Self casting

    SELECT_TARGET_SINGLE_ENEMY,                             //Only Single Enemy
    SELECT_TARGET_AOE_ENEMY,                                //Only AoE Enemy
    SELECT_TARGET_ANY_ENEMY,                                //AoE or Single Enemy

    SELECT_TARGET_SINGLE_FRIEND,                            //Only Single Friend
    SELECT_TARGET_AOE_FRIEND,                               //Only AoE Friend
    SELECT_TARGET_ANY_FRIEND,                               //AoE or Single Friend
};

//Spell Effects used by SelectSpell
enum SelectEffect
{
    SELECT_EFFECT_DONTCARE = 0,                             //All spell effects allowed
    SELECT_EFFECT_DAMAGE,                                   //Spell does damage
    SELECT_EFFECT_HEALING,                                  //Spell does healing
    SELECT_EFFECT_AURA,                                     //Spell applies an aura
};

//Selection method used by SelectTarget
enum SelectAggroTarget
{
    SELECT_TARGET_RANDOM = 0,                               //Just selects a random target
    SELECT_TARGET_TOPAGGRO,                                 //Selects targes from top aggro to bottom
    SELECT_TARGET_BOTTOMAGGRO,                              //Selects targets from bottom aggro to top
};

enum SCEquip
{
    EQUIP_NO_CHANGE = -1,
    EQUIP_UNEQUIP   = 0
};

struct PointMovement
{
    uint32 m_uiCreatureEntry;
    uint32 m_uiPointId;
    float m_fX;
    float m_fY;
    float m_fZ;
    uint32 m_uiWaitTime;
};

struct MANGOS_DLL_DECL ScriptedAI : public CreatureAI
{
    ScriptedAI(Creature* creature) : m_creature(creature), InCombat(false), bCombatMovement(true) {}
    ~ScriptedAI() {}

    //*************
    //CreatureAI Functions
    //*************

    //Called if IsVisible(Unit *who) is true at each *who move
    void MoveInLineOfSight(Unit *);

    //Called at each attack of m_creature by any victim
    void AttackStart(Unit *);

    //Called at stoping attack by any attacker
    void EnterEvadeMode();

    //Called at any heal cast/item used (call non implemented in mangos)
    void HealBy(Unit *healer, uint32 amount_healed) {}

    // Called at any Damage to any victim (before damage apply)
    void DamageDeal(Unit *done_to, uint32 &damage) {}

    // Called at any Damage from any attacker (before damage apply)
    void DamageTaken(Unit *done_by, uint32 &damage) {}

    //Is unit visible for MoveInLineOfSight
    bool IsVisible(Unit *who) const;

    //Called at World update tick
    void UpdateAI(const uint32);

    //Called at creature death
    void JustDied(Unit*){}

    //Called at creature killing another unit
    void KilledUnit(Unit*){}

    // Called when the creature summon successfully other creature
    void JustSummoned(Creature* ) {}

    // Called when a summoned creature is despawned
    void SummonedCreatureDespawn(Creature* /*unit*/) {}

    // Called when hit by a spell
    void SpellHit(Unit*, const SpellEntry*) {}

    // Called when creature is spawned or respawned (for reseting variables)
    void JustRespawned();

    //Called at waypoint reached or PointMovement end
    void MovementInform(uint32, uint32){}

    //*************
    // Variables
    //*************

    //Pointer to creature we are manipulating
    Creature* m_creature;

    //Bool for if we are in combat or not
    bool InCombat;

    //*************
    //Pure virtual functions
    //*************

    //Called at creature reset either by death or evade
    virtual void Reset() = 0;

    //Called at creature aggro either by MoveInLOS or Attack Start
    virtual void Aggro(Unit*) = 0;

    //*************
    //AI Helper Functions
    //*************

    //Start movement toward victim
    void DoStartMovement(Unit* victim, float distance = 0, float angle = 0);

    //Start no movement on victim
    void DoStartNoMovement(Unit* victim);

    //Do melee swing of current victim if in rnage and ready and not casting
    void DoMeleeAttackIfReady();

    //Stop attack of current victim
    void DoStopAttack();

    //Cast spell by Id
    void DoCast(Unit* victim, uint32 spellId, bool triggered = false);

    //Cast spell by spell info
    void DoCastSpell(Unit* who,SpellEntry const *spellInfo, bool triggered = false);

    //Creature say
    void DoSay(const char* text, uint32 language, Unit* target);

    //Creature Yell
    void DoYell(const char* text, uint32 language, Unit* target);

    //Creature Text emote, optional bool for boss emote text
    void DoTextEmote(const char* text, Unit* target, bool IsBossEmote = false);

    //Creature whisper, optional bool for boss whisper
    void DoWhisper(const char* text, Unit* reciever, bool IsBossWhisper = false);

    //Plays a sound to all nearby players
    void DoPlaySoundToSet(Unit* unit, uint32 sound);

    //Places the entire map into combat with creature
    void DoZoneInCombat(Unit* pUnit = 0);

    //Drops all threat to 0%. Does not remove players from the threat list
    void DoResetThreat();

    //Teleports a player without dropping threat (only teleports to same map)
    void DoTeleportPlayer(Unit* pUnit, float x, float y, float z, float o);

    //Returns friendly unit with the most amount of hp missing from max hp
    Unit* DoSelectLowestHpFriendly(float range, uint32 MinHPDiff = 1);

    //Returns a list of friendly CC'd units within range
    std::list<Creature*> DoFindFriendlyCC(float range);

    //Returns a list of all friendly units missing a specific buff within range
    std::list<Creature*> DoFindFriendlyMissingBuff(float range, uint32 spellid);

    //Spawns a creature relative to m_creature
    Creature* DoSpawnCreature(uint32 id, float x, float y, float z, float angle, uint32 type, uint32 despawntime);

    //Selects a unit from the creature's current aggro list
    Unit* SelectUnit(SelectAggroTarget target, uint32 position);

    //Returns spells that meet the specified criteria from the creatures spell list
    SpellEntry const* SelectSpell(Unit* Target, int32 School, int32 Mechanic, SelectTarget Targets,  uint32 PowerCostMin, uint32 PowerCostMax, float RangeMin, float RangeMax, SelectEffect Effect);

    //Checks if you can cast the specified spell
    bool CanCast(Unit* Target, SpellEntry const *Spell, bool Triggered = false);

    void SetEquipmentSlots(bool bLoadDefault, int32 uiMainHand = EQUIP_NO_CHANGE, int32 uiOffHand = EQUIP_NO_CHANGE, int32 uiRanged = EQUIP_NO_CHANGE);

    void SetSheathState(SheathState newState);

    void SetCombatMovement(bool bCombatMove);

    protected:
        bool bCombatMovement;
};

struct MANGOS_DLL_DECL Scripted_NoMovementAI : public ScriptedAI
{
    Scripted_NoMovementAI(Creature* creature) : ScriptedAI(creature) {}

    //Called if IsVisible(Unit *who) is true at each *who move
    void MoveInLineOfSight(Unit *);

    //Called at each attack of m_creature by any victim
    void AttackStart(Unit *);
};
#endif
