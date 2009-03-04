/* Copyright © 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

/* ScriptData
SDName: boss_kiljaeden
SD%Complete: 0
SDComment: Development
SDCategory: Sunwell_Plateau
EndScriptData */
#include "precompiled.h"
#include "sc_creature.h"
#include "sc_instance.h"
#include "def_sunwell_plateau.h"
#include "GameObject.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"

/*** Spells used during the encounter ***/
/* Hand of the Deceiver's spells and cosmetics */
#define SPELL_SHADOW_BOLT_VOLLEY 45575 // ~30 yard range Shadow Bolt Volley for ~2k(?) damage
#define SPELL_SHADOW_INFUSION 45772 // They gain this at 20% - Immunity to Stun/Silence and makes them look angry!
#define SPELL_FELFIRE_PORTAL 46875 // Creates a portal that spawns Felfire Fiends (LIVE FOR THE SWARM!1 FOR THE OVERMIND!)
#define SPELL_SHADOW_CHANNELING 46757 // Channeling animation out of combat

/* Volatile Felfire Fiend's spells */
#define SPELL_FELFIRE_FISSION 45779 // Felfire Fiends explode when they die or get close to target.

/* Kil'Jaeden's spells and cosmetics */
#define SPELL_TRANS 23188 // Surprisingly, this seems to be the right spell.. (Where is it used?)
#define SPELL_REBIRTH 44200 // Emerge from the Sunwell
#define SPELL_SOUL_FLAY_DAMAGE 45442 // 9k Shadow damage over 3 seconds. Spammed throughout all the fight.
#define SPELL_SOUL_FLAY_MOVEMENT 47106 // -50% Movement component of the above. Players cast it on selves unless this spell is fixed.
#define SPELL_LEGION_LIGHTNING 45664 // Chain Lightning, 4 targets, ~3k Shadow damage, 1.5k mana burn
#define SPELL_FIRE_BLOOM 45641 // Places a debuff on 5 raid members, which causes them to deal 2k Fire damage to nearby allies and selves. MIGHT NOT WORK
#define SPELL_SINISTER_REFLECTION 45785 // Summon shadow copies of 5 raid members that fight against KJ's enemies
#define SPELL_COPY_WEAPON 41055 // }
#define SPELL_COPY_WEAPON2 41054 // }
#define SPELL_COPY_OFFHAND 45206 // }- Spells used in Sinister Reflection creation
#define SPELL_COPY_OFFHAND_WEAPON 45205 // }
#define SPELL_SHADOW_SPIKE 46680 // Bombard random raid members with Shadow Spikes (Very similar to Void Reaver orbs)
#define SPELL_FLAME_DART 45740 // Bombards the raid with flames every 3(?) seconds
#define SPELL_DARKNESS_OF_A_THOUSAND_SOULS 46605 // Begins a 8-second channeling, after which he will deal 50'000 damage to the raid
#define SPELL_ARMAGEDDON 45909 // Meteor spell
#define SPELL_ARMAGEDDON_VISUAL 45911 // Does the hellfire visual to indicate where the meteor missle lands

/* Anveena's spells and cosmetics (Or, generally, everything that has "Anveena" in name) */
#define SPELL_ANVEENA_PRISON 46367 // She hovers locked within a bubble
#define SPELL_ANVEENA_ENERGY_DRAIN 46410 // Sunwell energy glow animation (Control mob uses this)
#define SPELL_SACRIFICE_OF_ANVEENA 46474 // This is cast on Kil'Jaeden when Anveena sacrifices herself into the Sunwell

/*** Other Spells (used by players, etc) ***/
#define SPELL_VENGEANCE_OF_THE_BLUE_FLIGHT 45839 // Possess the blue dragon from the orb to help the raid.
#define SPELL_ENTROPIUS_BODY 46819 // Visual for Entropius at the Epilogue

/*** Creatures used in the encounter ***/
#define CREATURE_ANVEENA 26046 // Embodiment of the Sunwell
#define CREATURE_KALECGOS 25319 // Helps the raid throughout the fight
#define CREATURE_KILJAEDEN 25315 // Give it to 'em KJ!
#define CREATURE_HAND_OF_THE_DECEIVER 25588 // Adds found before KJ emerges
#define CREATURE_FELFIRE_PORTAL 25603 // Portal spawned be Hand of the Deceivers
#define CREATURE_VOLATILE_FELFIRE_FIEND 25598 // Fiends spawned by the above portal
#define CREATURE_ARMAGEDDON_TARGET 25735 // This mob casts meteor on itself.. I think
#define CREATURE_SHIELD_ORB 25502 // Shield orbs circle the room raining shadow bolts on raid
#define CREATURE_THE_CORE_OF_ENTROPIUS 26262 // Used in the ending cinematic?
#define CREATURE_POWER_OF_THE_BLUE_DRAGONFLIGHT 25653 // NPC that players possess when using the Orb of the Blue Dragonflight

/*** GameObjects ***/
#define GAMEOBJECT_ORB_OF_THE_BLUE_DRAGONFLIGHT 188415

/*** Speech and sounds***/
// These are used throughout Sunwell and Magisters(?). Players can hear this while running through the instances.
#define SAY_KJ_OFFCOMBAT1 "All my plans have led to this!"
#define SOUND_KJ_OFFCOMBAT1 12495
#define SAY_KJ_OFFCOMBAT2 "Stay on task! Do not waste time!"
#define SOUND_KJ_OFFCOMBAT2 12496
#define SAY_KJ_OFFCOMBAT3 "I have waited long enough!"
#define SOUND_KJ_OFFCOMBAT3 12497
#define SAY_KJ_OFFCOMBAT4 "Fail me and suffer for eternity!"
#define SOUND_KJ_OFFCOMBAT4 12498
#define SAY_KJ_OFFCOMBAT5 "Drain the girl! Drain her power until there is nothing but a vacant shell!"
#define SOUND_KJ_OFFCOMBAT5 12499

// Encounter speech and sounds
#define SAY_KJ_EMERGE "The expendible have perished... So be it! Now I shall succeed where Sargeras could not! I will bleed this wretched world and secure my place as the true master of the Burning Legion. The end has come! Let the unraveling of this world commence!"
#define SOUND_KJ_EMERGE 12500
#define SAY_KJ_SLAY1 "Another step towards destruction!"
#define SOUND_KJ_SLAY1 12501
#define SAY_KJ_SLAY2 "Anak-ky'ri!" // TODO: WTB Eredun dictionary
#define SOUND_KJ_SLAY2 12502
#define SAY_KJ_REFLECTION1 "Who can you trust?"
#define SOUND_KJ_REFLECTION1 12503
#define SAY_KJ_REFLECTION2 "The enemy is upon you!"
#define SOUND_KJ_REFLECTION2 12504
#define SAY_KJ_DARKNESS1 "Chaos!"
#define SOUND_KJ_DARKNESS1 12505
#define SAY_KJ_DARKNESS2 "Destruction!"
#define SOUND_KJ_DARKNESS2 12506
#define SAY_KJ_DARKNESS3 "Oblivion!"
#define SOUND_KJ_DARKNESS3 12507
#define SAY_KJ_PHASE3 "I will not be denied! This world shall fall!"
#define SOUND_KJ_PHASE3 12508
#define SAY_KJ_PHASE4 "Do not harbor false hope. You cannot win!"
#define SOUND_KJ_PHASE4 12509
#define SAY_KJ_PHASE5 "Aggghh! The powers of the Sunwell... turned... against me! What have you done? WHAT HAVE YOU DONE?"
#define SOUND_KJ_PHASE5 12510

/*** Kalecgos - Anveena speech at the beginning of Phase 5; Anveena's sacrifice ***/
#define SAY_KALECGOS_AWAKEN "Anveena, you must awaken, this world needs you!"
#define SOUND_KALECGOS_AWAKEN 12445
#define SAY_ANVEENA_IMPRISONED "I serve only the Master now."
#define SOUND_ANVEENA_IMPRISONED 12511
#define SAY_KALECGOS_LETGO "You must let go! You must become what you were always meant to be! The time is now, Anveena!"
#define SOUND_KALECGOS_LETGO 12446
#define SAY_ANVEENA_LOST "But I'm... lost... I cannot find my way back!"
#define SOUND_ANVEENA_LOST 12512
#define SAY_KALECGOS_FOCUS "Anveena, I love you! Focus on my voice, come back for me now! Only you can cleanse the Sunwell!"
#define SOUND_KALECGOS_FOCUS 12447
#define SAY_ANVEENA_KALEC "Kalec... Kalec?"
#define SOUND_ANVEENA_KALEC 12513
#define SAY_KALECGOS_FATE "Yes, Anveena! Let fate embrace you now!"
#define SOUND_KALECGOS_FATE 12448
#define SAY_ANVEENA_GOODBYE "The nightmare is over, the spell is broken! Goodbye, Kalec, my love!"
#define SOUND_ANVEENA_GOODBYE 12514
#define SAY_KALECGOS_GOODBYE "Goodbye, Anveena, my love. Few will remember your name, yet this day you change the course of destiny. What was once corrupt is now pure. Heroes, do not let her sacrifice be in vain."
#define SOUND_KALECGOS_GOODBYE 12450
#define SAY_KALECGOS_ENCOURAGE "Strike now, heroes, while he is weakened! Vanquish the Deceiver!"
#define SOUND_KALECGOS_ENCOURAGE 12449
// Charming

/*** Error messages ***/
#define ERROR_KJ_NOT_SUMMONED "SD2 ERROR: Unable to summon Kil'Jaeden for some reason"

/*** Others ***/
#define FLOOR_Z 28.050388
#define SHIELD_ORB_Z 45.000000 // TODO: Find correct height.

enum Phase
{
PHASE_DECEIVERS = 1, // Fight 3 adds
PHASE_NORMAL = 2, // Kil'Jaeden emerges from the sunwell
PHASE_DARKNESS = 3, // At 85%, he gains few abilities; Kalecgos joins the fight
PHASE_ARMAGEDDON = 4, // At 55%, he gains even more abilities
PHASE_SACRIFICE = 5, // At 25%, Anveena sacrifices herself into the Sunwell; at this point he becomes enraged and has *significally* shorter cooldowns.
};

// Locations of the Hand of Deceiver adds
float DeceiverLocations[3][3]=
{
// X Y O
{1682.045, 631.299, 5.936},
{1684.099, 618.848, 0.589},
{1694.170, 612.272, 1.416},
};

/* TODO:
Middle of the Sunwell: {1698.897217, 628.214600}
Northeast: {1713.255249, 619.132324}
2D distance ~17 */

// Locations, where Shield Orbs will spawn, TODO: Use Sunwell middle point, radius and sin/cos instead of this.
float ShieldOrbLocations[3][2]=
{ // TODO: Find correct XY's
{1683.000, 625.000}, // First one spawns northeast of KJ
{1683.000, 625.000}, // Second one spawns southeast
{1683.000, 625.000}, // Third one spawns (?)
};


struct Speech
{
char* text;
uint32 creature, timer;
};

// TODO: Timers
static Speech Sacrifice[]=
{
{"Anveena, you must awaken, this world needs you!", CREATURE_KALECGOS, 5000},
{"I serve only the Master now.", CREATURE_ANVEENA, 5000},
{"You must let go! You must become what you were always meant to be! The time is now, Anveena!", CREATURE_KALECGOS, 8000},
{"But I'm... lost... I cannot find my way back!", CREATURE_ANVEENA, 5000},
{"Anveena, I love you! Focus on my voice, come back for me now! Only you can cleanse the Sunwell!", CREATURE_KALECGOS, 7000},
{"Kalec... Kalec?", CREATURE_ANVEENA, 2000},
{"Yes, Anveena! Let fate embrace you now!", CREATURE_KALECGOS, 3000},
{"The nightmare is over, the spell is broken! Goodbye, Kalec, my love!", CREATURE_ANVEENA, 6000},
{"Goodbye, Anveena, my love. Few will remember your name, yet this day you change the course of destiny. What was once corrupt is now pure. Heroes, do not let her sacrifice be in vain.", CREATURE_KALECGOS, 12000},
{"Aggghh! The powers of the Sunwell... turned... against me! What have you done? WHAT HAVE YOU DONE?", CREATURE_KILJAEDEN, 8000},
{"Strike now, heroes, while he is weakened! Vanquish the Deceiver!", CREATURE_KALECGOS, 5000},

};

class AllOrbsInGrid
{
public:
AllOrbsInGrid() {}
bool operator() (GameObject* go)
{
if(go->GetEntry() == GAMEOBJECT_ORB_OF_THE_BLUE_DRAGONFLIGHT)
return true;

return false;
}
};

struct MANGOS_DLL_DECL boss_kalecgosAI : public ScriptedAI
{
boss_kalecgosAI(Creature* c) : ScriptedAI(c)
{
for(uint8 i = 0; i < 4; ++i)
Orb[i] = 0;

FindOrbs();

Reset();
}

uint64 Orb[4];

uint8 OrbsEmpowered;

uint32 EmpowerTimer;

void Reset()
{
EmpowerTimer = 0;
}

void Aggro(Unit* who) {}

void FindOrbs()
{
CellPair pair(MaNGOS::ComputeCellPair(m_creature->GetPositionX(), m_creature->GetPositionY()));
Cell cell(pair);
cell.data.Part.reserved = ALL_DISTRICT;
cell.SetNoCreate();

std::list<GameObject*> orbList;
AllOrbsInGrid check;
MaNGOS::GameObjectListSearcher<AllOrbsInGrid> searcher(m_creature, orbList, check);
TypeContainerVisitor<MaNGOS::GameObjectListSearcher<AllOrbsInGrid>, GridTypeMapContainer> visitor(searcher);

CellLock<GridReadGuard> cell_lock(cell, pair);
cell_lock->Visit(cell_lock, visitor, *(m_creature->GetMap()));

if(orbList.empty())
return;

uint8 i = 0;
for(std::list<GameObject*>::iterator itr = orbList.begin(); itr != orbList.end(); ++itr, ++i)
Orb[i] = (*itr)->GetGUID();
}

void EmpowerOrb()
{
GameObject* orb = GameObject::GetGameObject(*m_creature, Orb[OrbsEmpowered]);
if(!orb)
return;

orb->SetUInt32Value(GAMEOBJECT_FACTION, 35);
orb->Refresh();

++OrbsEmpowered;
}

void UpdateAI(const uint32 diff)
{
}
};

struct MANGOS_DLL_DECL boss_KJAI : public Scripted_NoMovementAI
{
boss_KJAI(Creature* c) : Scripted_NoMovementAI(c)
{
pInstance = ((ScriptedInstance*)c->GetInstanceData());
Reset();
}

/* GUIDs */
uint64 KalecGUID;

/* General */
ScriptedInstance* pInstance;
uint32 Phase;

/* Phase 2+ spells */
uint32 SoulFlayTimer;
uint32 LegionLightningTimer;
uint32 FireBloomTimer;
uint32 SummonShieldOrbTimer;

/* Phase 3+ spells */
uint32 ShadowSpikeTimer;
uint32 FlameDartTimer;
uint32 DarknessTimer;

/* Phase 4+ spells */
uint32 ArmageddonTimer;

void Reset()
{
KalecGUID = 0;

// TODO: Fix timers
SoulFlayTimer = 15000;
LegionLightningTimer = 30000;
FireBloomTimer = 30000;
SummonShieldOrbTimer = 45000;

ShadowSpikeTimer = 60000;
FlameDartTimer = 3000;
DarknessTimer = 60000;

ArmageddonTimer = 10000;

Phase = PHASE_DECEIVERS;
}

void EnterEvadeMode()
{
Scripted_NoMovementAI::EnterEvadeMode();

// Reset the controller
if(pInstance)
{
Creature* Control = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_KILJAEDEN_CONTROLLER)));
if(Control)
((Scripted_NoMovementAI*)Control->AI())->Reset();
}
}

void Aggro(Unit* who) {}

void CastSinisterReflection() {} // TODO: Create copies of 5 random raid members.

void UpdateAI(const uint32 diff)
{
if(!m_creature->SelectHostilTarget() || !m_creature->getVictim() || Phase < PHASE_NORMAL)
return;

if(SoulFlayTimer < diff) // 9k damage over 3 seconds on highest aggro
{
DoCast(m_creature->getVictim(), SPELL_SOUL_FLAY_DAMAGE);
m_creature->getVictim()->CastSpell(m_creature->getVictim(), SPELL_SOUL_FLAY_MOVEMENT, true);
SoulFlayTimer = 3500;
}else SoulFlayTimer -= diff;

if(LegionLightningTimer < diff) // Lightning on random target, jumps to 3 additional players, burns 1500 mana and deals 3k damage
{
m_creature->InterruptNonMeleeSpells(true);
DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), SPELL_LEGION_LIGHTNING);
LegionLightningTimer = (Phase == PHASE_SACRIFICE) ? 18000 : 30000; // 18 seconds in PHASE_SACRIFICE
SoulFlayTimer = 2500;
}else LegionLightningTimer -= diff;

if(FireBloomTimer < diff) // Places a debuff on 5 random targets; targets and all nearby allies suffer ~1.7k fire damage every 2 seconds
{
// FIXME: Should be limited to 5 targets; it isn't at the very moment.
m_creature->InterruptNonMeleeSpells(true);
DoCast(m_creature, SPELL_FIRE_BLOOM);
FireBloomTimer = (Phase == PHASE_SACRIFICE) ? 25000 : 40000; // 25 seconds in PHASE_SACRIFICE
SoulFlayTimer = 1000;
}else FireBloomTimer -= diff;

if(SummonShieldOrbTimer < diff)
{
for(uint8 i = 1; i < Phase; ++i)
m_creature->SummonCreature(CREATURE_SHIELD_ORB, ShieldOrbLocations[i-1][0], ShieldOrbLocations[i-1][1], SHIELD_ORB_Z, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 45000);
SummonShieldOrbTimer = (Phase == PHASE_SACRIFICE) ? 30000 : 60000; // 30 seconds in PHASE_SACRIFICE
SoulFlayTimer = 2000;
}else SummonShieldOrbTimer -= diff;

if(Phase <= PHASE_NORMAL)
{
if(Phase == PHASE_NORMAL && ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 85))
{
CastSinisterReflection();
DoYell(SAY_KJ_PHASE3, LANG_UNIVERSAL, 0);
DoPlaySoundToSet(m_creature, SOUND_KJ_PHASE3);
Phase = PHASE_DARKNESS;
}
else return;
}

if(DarknessTimer < diff) // Begins to channel for 8 seconds, then deals 50'000 damage to all raid members.
{
m_creature->InterruptNonMeleeSpells(true);
DoCast(m_creature, SPELL_DARKNESS_OF_A_THOUSAND_SOULS);
DarknessTimer = (Phase == PHASE_SACRIFICE) ? 20000 + rand()%15000 : 40000 + rand()%30000;
SummonShieldOrbTimer += 8000;
FireBloomTimer += 8000; // Don't let other spells
LegionLightningTimer += 8000; // interrupt this cast!
FlameDartTimer += 8000;
ShadowSpikeTimer += 8000;
if(Phase == PHASE_SACRIFICE)
ArmageddonTimer += 8000; // Armageddon on the other hand, can be casted now, if Anveena has already sacrificed
SoulFlayTimer = 9000;
}else DarknessTimer -= diff;

if(ArmageddonTimer < diff)
{
Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
if(target)
{
float x, y, z;
target->GetPosition(x, y, z);
Creature* Armageddon = m_creature->SummonCreature(CREATURE_ARMAGEDDON_TARGET, x, y, z, 0, TEMPSUMMON_TIMED_DESPAWN, 5000);
if(Armageddon)
{
Armageddon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
DoCast(Armageddon, SPELL_ARMAGEDDON, true);
Armageddon->CastSpell(Armageddon, SPELL_ARMAGEDDON_VISUAL, true);
}
}
ArmageddonTimer = 2000; // No, I'm not kidding
}else ArmageddonTimer -= diff;

if(ShadowSpikeTimer < diff)
{
ShadowSpikeTimer = 90000 + rand()%30000; // Not too often, amirite? It's 28 second casting timer...
}else ShadowSpikeTimer -= diff;

if(Phase <= PHASE_DARKNESS)
{
if(Phase == PHASE_DARKNESS && ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 55))
{
DoYell(SAY_KJ_PHASE4, LANG_UNIVERSAL, 0);
DoPlaySoundToSet(m_creature, SOUND_KJ_PHASE4);
Phase = PHASE_ARMAGEDDON;
}
else return;
}
// TODO: Phase 4 specific spells

/* if(Phase <= PHASE_ARMAGEDDON)
{
if(Phase == PHASE_ARMAGEDDON && ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 25))
{
Phase = PHASE_SACRIFICE;
// He shouldn't cast spells for ~10 seconds after Anveena's sacrifice. This will be done within Anveena's script
}
else return;
} */
}
};

struct MANGOS_DLL_DECL mob_kiljaeden_controllerAI : public Scripted_NoMovementAI
{
mob_kiljaeden_controllerAI(Creature* c) : Scripted_NoMovementAI(c)
{
pInstance = ((ScriptedInstance*)c->GetInstanceData());
Reset();
}

ScriptedInstance* pInstance;

uint64 DeceiverGUID[3];
uint64 AnveenaGUID;

uint16 DeceiverDeathCount;
uint32 Phase;

bool SummonedDeceivers;

void Reset()
{
for(uint8 i = 0; i < 3; ++i)
{
if(DeceiverGUID[i])
{
Creature* Deceiver = ((Creature*)Unit::GetUnit(*m_creature, DeceiverGUID[i]));
if(Deceiver)
{
Deceiver->SetVisibility(VISIBILITY_OFF);
Deceiver->setDeathState(JUST_DIED);
}
DeceiverGUID[i] = 0;
}
}

if(AnveenaGUID)
{
Creature* Anveena = ((Creature*)Unit::GetUnit(*m_creature, AnveenaGUID));
if(Anveena)
{
Anveena->SetVisibility(VISIBILITY_OFF);
Anveena->setDeathState(JUST_DIED);
}
}

Phase = PHASE_DECEIVERS;
DeceiverDeathCount = 0;

SummonedDeceivers = false;

m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
}

void MoveInLineOfSight(Unit* who)
{
if(Phase == PHASE_DECEIVERS)
return;

Scripted_NoMovementAI::MoveInLineOfSight(who);
}

void AttackStart(Unit* who)
{
if(Phase == PHASE_DECEIVERS)
return;

Scripted_NoMovementAI::AttackStart(who);
}

void Aggro(Unit* who) {}

void UpdateAI(const uint32 diff)
{
if(!SummonedDeceivers)
{
for(uint8 i = 0; i < 3; ++i)
{
Creature* Deceiver = m_creature->SummonCreature(CREATURE_HAND_OF_THE_DECEIVER, DeceiverLocations[i][0], DeceiverLocations[i][1], FLOOR_Z, DeceiverLocations[i][2], TEMPSUMMON_DEAD_DESPAWN, 0);
if(Deceiver)
{
Deceiver->CastSpell(Deceiver, SPELL_SHADOW_CHANNELING, false);
DeceiverGUID[i] = Deceiver->GetGUID();
}
}

Creature* Anveena = DoSpawnCreature(CREATURE_ANVEENA, 0, 0, 20, 0, TEMPSUMMON_DEAD_DESPAWN, 0);
if(Anveena)
{
Anveena->AddUnitMovementFlag(MOVEMENTFLAG_LEVITATING);
Anveena->CastSpell(Anveena, SPELL_ANVEENA_PRISON, false);
DoCast(m_creature, SPELL_ANVEENA_ENERGY_DRAIN);
AnveenaGUID = Anveena->GetGUID();
}

SummonedDeceivers = true;
}

if((DeceiverDeathCount > 2) && Phase == PHASE_DECEIVERS)
{
m_creature->RemoveAurasDueToSpell(SPELL_ANVEENA_ENERGY_DRAIN);
Phase = PHASE_NORMAL;
Creature* KJ = DoSpawnCreature(CREATURE_KILJAEDEN, 0, 0,0, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 45000);
if(KJ)
{
KJ->CastSpell(KJ, SPELL_REBIRTH, false);
((boss_KJAI*)KJ->AI())->Phase = PHASE_NORMAL;
KJ->AddThreat(m_creature->getVictim(), 1.0f);
KJ->MonsterYell(SAY_KJ_EMERGE, LANG_UNIVERSAL, 0);
DoPlaySoundToSet(KJ, SOUND_KJ_EMERGE);
}
else error_log(ERROR_KJ_NOT_SUMMONED);
}
}
};

struct MANGOS_DLL_DECL mob_hand_of_the_deceiverAI : public ScriptedAI
{
mob_hand_of_the_deceiverAI(Creature* c) : ScriptedAI(c)
{
pInstance = ((ScriptedInstance*)c->GetInstanceData());
Reset();
}

ScriptedInstance* pInstance;

uint32 ShadowBoltVolleyTimer;
uint32 FelfirePortalTimer;

void Reset()
{
// TODO: Timers!
ShadowBoltVolleyTimer = 8000 + rand()%6000; // So they don't all cast it in the same moment.
FelfirePortalTimer = 20000;
}

void Aggro(Unit* who)
{
if(pInstance)
{
Creature* Control = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_KILJAEDEN_CONTROLLER)));
if(Control)
Control->AddThreat(who, 1.0f);
}
m_creature->InterruptNonMeleeSpells(true);
}

void JustDied(Unit* killer)
{
if(!pInstance)
return;

Creature* Control = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_KILJAEDEN_CONTROLLER)));
if(Control)
((mob_kiljaeden_controllerAI*)Control->AI())->DeceiverDeathCount++;
}

void UpdateAI(const uint32 diff)
{
if(!m_creature->getVictim() || !m_creature->SelectHostilTarget())
return;

// Gain Shadow Infusion at 20% health
if(((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 20) && !m_creature->HasAura(SPELL_SHADOW_INFUSION, 0))
DoCast(m_creature, SPELL_SHADOW_INFUSION, true);

// Shadow Bolt Volley - Shoots Shadow Bolts at all enemies within 30 yards, for ~2k Shadow damage.
if(ShadowBoltVolleyTimer < diff)
{
DoCast(m_creature->getVictim(), SPELL_SHADOW_BOLT_VOLLEY);
ShadowBoltVolleyTimer = 12000;
}else ShadowBoltVolleyTimer -= diff;

// Felfire Portal - Creatres a portal, that spawns Volatile Felfire Fiends, which do suicide bombing.
if(FelfirePortalTimer < diff)
{
Creature* Portal = DoSpawnCreature(CREATURE_FELFIRE_PORTAL, 0, 0,0, 0, TEMPSUMMON_TIMED_DESPAWN, 20000);
if(Portal)
{
std::list<HostilReference*>::iterator itr;
for(itr = m_creature->getThreatManager().getThreatList().begin(); itr != m_creature->getThreatManager().getThreatList().end(); ++itr)
{
Unit* pUnit = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());
if(pUnit)
Portal->AddThreat(pUnit, 1.0f);
}
}
FelfirePortalTimer = 20000;
}else FelfirePortalTimer -= diff;

DoMeleeAttackIfReady();
}
};

struct MANGOS_DLL_DECL mob_felfire_portalAI : public Scripted_NoMovementAI
{
mob_felfire_portalAI(Creature* c) : Scripted_NoMovementAI(c)
{
Reset();
}

uint32 SpawnFiendTimer;

// TODO: Timers
void Reset() { SpawnFiendTimer = 5000; }

void Aggro(Unit* who) {}

void UpdateAI(const uint32 diff)
{
if(!m_creature->SelectHostilTarget() || !m_creature->getVictim()) return;

if(SpawnFiendTimer < diff)
{
Creature* Fiend = DoSpawnCreature(CREATURE_VOLATILE_FELFIRE_FIEND, 0, 0, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 20000);
if(Fiend)
Fiend->AddThreat(SelectUnit(SELECT_TARGET_RANDOM,0), 100000.0f);
SpawnFiendTimer = 4000 + rand()%4000;
}else SpawnFiendTimer -= diff;
}
};

struct MANGOS_DLL_DECL mob_volatile_felfire_fiendAI : public ScriptedAI
{
mob_volatile_felfire_fiendAI(Creature* c) : ScriptedAI(c)
{
Reset();
}

uint32 ExplodeTimer;
bool LockedTarget;

void Reset()
{
ExplodeTimer = 2000;
LockedTarget = false;
}

void Aggro(Unit* who) {}

void DamageTaken(Unit *done_by, uint32 &damage)
{
if(damage > m_creature->GetHealth())
DoCast(m_creature, SPELL_FELFIRE_FISSION, true);
}

void UpdateAI(const uint32 diff)
{
if(!m_creature->SelectHostilTarget() || !m_creature->getVictim()) return;

if(!LockedTarget)
{
m_creature->AddThreat(m_creature->getVictim(), 10000000.0f);
LockedTarget = true;
}

if(ExplodeTimer) // Just so it doesn't explode as soon as it spawn
{
if(ExplodeTimer < diff) ExplodeTimer = 0;
else ExplodeTimer -= diff;
}
else if(m_creature->IsWithinDistInMap(m_creature->getVictim(), 3)) // Explode if it's close enough to it's target
{
DoCast(m_creature->getVictim(), SPELL_FELFIRE_FISSION);
m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
}
}
};

CreatureAI* GetAI_boss_KJ(Creature *_Creature)
{
return new boss_KJAI (_Creature);
}

CreatureAI* GetAI_mob_kiljaeden_controller(Creature *_Creature)
{
return new mob_kiljaeden_controllerAI (_Creature);
}

CreatureAI* GetAI_mob_hand_of_the_deceiver(Creature *_Creature)
{
return new mob_hand_of_the_deceiverAI (_Creature);
}

CreatureAI* GetAI_mob_felfire_portal(Creature *_Creature)
{
return new mob_felfire_portalAI (_Creature);
}

CreatureAI* GetAI_mob_volatile_felfire_fiend(Creature *_Creature)
{
return new mob_volatile_felfire_fiendAI (_Creature);
}

void AddSC_boss_kiljaeden()
{
Script* newscript;

newscript = new Script;
newscript->GetAI = &GetAI_boss_KJ;
newscript->Name = "boss_KJ";
newscript->RegisterSelf();

newscript = new Script;
newscript->GetAI = &GetAI_mob_kiljaeden_controller;
newscript->Name = "mob_kiljaeden_controller";
newscript->RegisterSelf();

newscript = new Script;
newscript->GetAI = &GetAI_mob_hand_of_the_deceiver;
newscript->Name = "mob_hand_of_the_deceiver";
newscript->RegisterSelf();

newscript = new Script;
newscript->GetAI = &GetAI_mob_felfire_portal;
newscript->Name = "mob_felfire_portal";
newscript->RegisterSelf();

newscript = new Script;
newscript->GetAI = &GetAI_mob_volatile_felfire_fiend;
newscript->Name = "mob_volatile_felfire_fiend";
newscript->RegisterSelf();
};
