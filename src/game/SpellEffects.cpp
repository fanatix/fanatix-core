/*
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "WorldPacket.h"
#include "Opcodes.h"
#include "Log.h"
#include "UpdateMask.h"
#include "World.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"
#include "Player.h"
#include "SkillExtraItems.h"
#include "Unit.h"
#include "Spell.h"
#include "DynamicObject.h"
#include "SpellAuras.h"
#include "Group.h"
#include "UpdateData.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "SharedDefines.h"
#include "Pet.h"
#include "GameObject.h"
#include "GossipDef.h"
#include "Creature.h"
#include "Totem.h"
#include "CreatureAI.h"
#include "BattleGroundMgr.h"
#include "BattleGround.h"
#include "BattleGroundEY.h"
#include "BattleGroundWS.h"
#include "OutdoorPvPMgr.h"
#include "VMapFactory.h"
#include "Language.h"
#include "SocialMgr.h"
#include "Util.h"
#include "TemporarySummon.h"
#include "ScriptCalls.h"
#include "SkillDiscovery.h"
#include "Formulas.h"

pEffect SpellEffects[TOTAL_SPELL_EFFECTS]=
{
    &Spell::EffectNULL,                                     //  0
    &Spell::EffectInstaKill,                                //  1 SPELL_EFFECT_INSTAKILL
    &Spell::EffectSchoolDMG,                                //  2 SPELL_EFFECT_SCHOOL_DAMAGE
    &Spell::EffectDummy,                                    //  3 SPELL_EFFECT_DUMMY
    &Spell::EffectUnused,                                   //  4 SPELL_EFFECT_PORTAL_TELEPORT          unused
    &Spell::EffectTeleportUnits,                            //  5 SPELL_EFFECT_TELEPORT_UNITS
    &Spell::EffectApplyAura,                                //  6 SPELL_EFFECT_APPLY_AURA
    &Spell::EffectEnvirinmentalDMG,                         //  7 SPELL_EFFECT_ENVIRONMENTAL_DAMAGE
    &Spell::EffectPowerDrain,                               //  8 SPELL_EFFECT_POWER_DRAIN
    &Spell::EffectHealthLeech,                              //  9 SPELL_EFFECT_HEALTH_LEECH
    &Spell::EffectHeal,                                     // 10 SPELL_EFFECT_HEAL
    &Spell::EffectUnused,                                   // 11 SPELL_EFFECT_BIND
    &Spell::EffectNULL,                                     // 12 SPELL_EFFECT_PORTAL
    &Spell::EffectUnused,                                   // 13 SPELL_EFFECT_RITUAL_BASE              unused
    &Spell::EffectUnused,                                   // 14 SPELL_EFFECT_RITUAL_SPECIALIZE        unused
    &Spell::EffectUnused,                                   // 15 SPELL_EFFECT_RITUAL_ACTIVATE_PORTAL   unused
    &Spell::EffectQuestComplete,                            // 16 SPELL_EFFECT_QUEST_COMPLETE
    &Spell::EffectWeaponDmg,                                // 17 SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL
    &Spell::EffectResurrect,                                // 18 SPELL_EFFECT_RESURRECT
    &Spell::EffectAddExtraAttacks,                          // 19 SPELL_EFFECT_ADD_EXTRA_ATTACKS
    &Spell::EffectUnused,                                   // 20 SPELL_EFFECT_DODGE                    one spell: Dodge
    &Spell::EffectUnused,                                   // 21 SPELL_EFFECT_EVADE                    one spell: Evade (DND)
    &Spell::EffectParry,                                    // 22 SPELL_EFFECT_PARRY
    &Spell::EffectBlock,                                    // 23 SPELL_EFFECT_BLOCK                    one spell: Block
    &Spell::EffectCreateItem,                               // 24 SPELL_EFFECT_CREATE_ITEM
    &Spell::EffectUnused,                                   // 25 SPELL_EFFECT_WEAPON
    &Spell::EffectUnused,                                   // 26 SPELL_EFFECT_DEFENSE                  one spell: Defense
    &Spell::EffectPersistentAA,                             // 27 SPELL_EFFECT_PERSISTENT_AREA_AURA
    &Spell::EffectSummonType,                               // 28 SPELL_EFFECT_SUMMON
    &Spell::EffectMomentMove,                               // 29 SPELL_EFFECT_LEAP
    &Spell::EffectEnergize,                                 // 30 SPELL_EFFECT_ENERGIZE
    &Spell::EffectWeaponDmg,                                // 31 SPELL_EFFECT_WEAPON_PERCENT_DAMAGE
    &Spell::EffectTriggerMissileSpell,                      // 32 SPELL_EFFECT_TRIGGER_MISSILE
    &Spell::EffectOpenLock,                                 // 33 SPELL_EFFECT_OPEN_LOCK
    &Spell::EffectSummonChangeItem,                         // 34 SPELL_EFFECT_SUMMON_CHANGE_ITEM
    &Spell::EffectApplyAreaAura,                            // 35 SPELL_EFFECT_APPLY_AREA_AURA_PARTY
    &Spell::EffectLearnSpell,                               // 36 SPELL_EFFECT_LEARN_SPELL
    &Spell::EffectUnused,                                   // 37 SPELL_EFFECT_SPELL_DEFENSE            one spell: SPELLDEFENSE (DND)
    &Spell::EffectDispel,                                   // 38 SPELL_EFFECT_DISPEL
    &Spell::EffectUnused,                                   // 39 SPELL_EFFECT_LANGUAGE
    &Spell::EffectDualWield,                                // 40 SPELL_EFFECT_DUAL_WIELD
    &Spell::EffectUnused,                                   // 41 SPELL_EFFECT_JUMP
    &Spell::EffectJump,                                     // 42 SPELL_EFFECT_JUMP2
    &Spell::EffectTeleUnitsFaceCaster,                      // 43 SPELL_EFFECT_TELEPORT_UNITS_FACE_CASTER
    &Spell::EffectLearnSkill,                               // 44 SPELL_EFFECT_SKILL_STEP
    &Spell::EffectAddHonor,                                 // 45 SPELL_EFFECT_ADD_HONOR                honor/pvp related
    &Spell::EffectNULL,                                     // 46 SPELL_EFFECT_SPAWN                    we must spawn pet there
    &Spell::EffectTradeSkill,                               // 47 SPELL_EFFECT_TRADE_SKILL
    &Spell::EffectUnused,                                   // 48 SPELL_EFFECT_STEALTH                  one spell: Base Stealth
    &Spell::EffectUnused,                                   // 49 SPELL_EFFECT_DETECT                   one spell: Detect
    &Spell::EffectTransmitted,                              // 50 SPELL_EFFECT_TRANS_DOOR
    &Spell::EffectUnused,                                   // 51 SPELL_EFFECT_FORCE_CRITICAL_HIT       unused
    &Spell::EffectUnused,                                   // 52 SPELL_EFFECT_GUARANTEE_HIT            one spell: zzOLDCritical Shot
    &Spell::EffectEnchantItemPerm,                          // 53 SPELL_EFFECT_ENCHANT_ITEM
    &Spell::EffectEnchantItemTmp,                           // 54 SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY
    &Spell::EffectTameCreature,                             // 55 SPELL_EFFECT_TAMECREATURE
    &Spell::EffectSummonPet,                                // 56 SPELL_EFFECT_SUMMON_PET
    &Spell::EffectLearnPetSpell,                            // 57 SPELL_EFFECT_LEARN_PET_SPELL
    &Spell::EffectWeaponDmg,                                // 58 SPELL_EFFECT_WEAPON_DAMAGE
    &Spell::EffectOpenSecretSafe,                           // 59 SPELL_EFFECT_OPEN_LOCK_ITEM
    &Spell::EffectProficiency,                              // 60 SPELL_EFFECT_PROFICIENCY
    &Spell::EffectSendEvent,                                // 61 SPELL_EFFECT_SEND_EVENT
    &Spell::EffectPowerBurn,                                // 62 SPELL_EFFECT_POWER_BURN
    &Spell::EffectThreat,                                   // 63 SPELL_EFFECT_THREAT
    &Spell::EffectTriggerSpell,                             // 64 SPELL_EFFECT_TRIGGER_SPELL
    &Spell::EffectApplyAreaAura,                            // 65 SPELL_EFFECT_APPLY_AREA_AURA_RAID
    &Spell::EffectUnused,                                   // 66 SPELL_EFFECT_CREATE_MANA_GEM          (possibly recharge it, misc - is item ID)
    &Spell::EffectHealMaxHealth,                            // 67 SPELL_EFFECT_HEAL_MAX_HEALTH
    &Spell::EffectInterruptCast,                            // 68 SPELL_EFFECT_INTERRUPT_CAST
    &Spell::EffectDistract,                                 // 69 SPELL_EFFECT_DISTRACT
    &Spell::EffectPull,                                     // 70 SPELL_EFFECT_PULL                     one spell: Distract Move
    &Spell::EffectPickPocket,                               // 71 SPELL_EFFECT_PICKPOCKET
    &Spell::EffectAddFarsight,                              // 72 SPELL_EFFECT_ADD_FARSIGHT
    &Spell::EffectUnused,                                   // 73 SPELL_EFFECT_UNTRAIN_TALENTS
    &Spell::EffectApplyGlyph,                               // 74 SPELL_EFFECT_APPLY_GLYPH
    &Spell::EffectHealMechanical,                           // 75 SPELL_EFFECT_HEAL_MECHANICAL          one spell: Mechanical Patch Kit
    &Spell::EffectSummonObjectWild,                         // 76 SPELL_EFFECT_SUMMON_OBJECT_WILD
    &Spell::EffectScriptEffect,                             // 77 SPELL_EFFECT_SCRIPT_EFFECT
    &Spell::EffectUnused,                                   // 78 SPELL_EFFECT_ATTACK
    &Spell::EffectSanctuary,                                // 79 SPELL_EFFECT_SANCTUARY
    &Spell::EffectAddComboPoints,                           // 80 SPELL_EFFECT_ADD_COMBO_POINTS
    &Spell::EffectUnused,                                   // 81 SPELL_EFFECT_CREATE_HOUSE             one spell: Create House (TEST)
    &Spell::EffectNULL,                                     // 82 SPELL_EFFECT_BIND_SIGHT
    &Spell::EffectDuel,                                     // 83 SPELL_EFFECT_DUEL
    &Spell::EffectStuck,                                    // 84 SPELL_EFFECT_STUCK
    &Spell::EffectSummonPlayer,                             // 85 SPELL_EFFECT_SUMMON_PLAYER
    &Spell::EffectActivateObject,                           // 86 SPELL_EFFECT_ACTIVATE_OBJECT
    &Spell::EffectUnused,                                   // 87 SPELL_EFFECT_WMO_DAMAGE
    &Spell::EffectUnused,                                   // 88 SPELL_EFFECT_WMO_REPAIR
    &Spell::EffectUnused,                                   // 89 SPELL_EFFECT_WMO_CHANGE
    &Spell::EffectUnused,                                   // 90 SPELL_EFFECT_KILL_CREDIT
    &Spell::EffectUnused,                                   // 91 SPELL_EFFECT_THREAT_ALL               one spell: zzOLDBrainwash
    &Spell::EffectEnchantHeldItem,                          // 92 SPELL_EFFECT_ENCHANT_HELD_ITEM
    &Spell::EffectUnused,                                   // 93 SPELL_EFFECT_SUMMON_PHANTASM
    &Spell::EffectSelfResurrect,                            // 94 SPELL_EFFECT_SELF_RESURRECT
    &Spell::EffectSkinning,                                 // 95 SPELL_EFFECT_SKINNING
    &Spell::EffectCharge,                                   // 96 SPELL_EFFECT_CHARGE
    &Spell::EffectUnused,                                   // 97 SPELL_EFFECT_97
    &Spell::EffectKnockBack,                                // 98 SPELL_EFFECT_KNOCK_BACK
    &Spell::EffectDisEnchant,                               // 99 SPELL_EFFECT_DISENCHANT
    &Spell::EffectInebriate,                                //100 SPELL_EFFECT_INEBRIATE
    &Spell::EffectFeedPet,                                  //101 SPELL_EFFECT_FEED_PET
    &Spell::EffectDismissPet,                               //102 SPELL_EFFECT_DISMISS_PET
    &Spell::EffectReputation,                               //103 SPELL_EFFECT_REPUTATION
    &Spell::EffectSummonObject,                             //104 SPELL_EFFECT_SUMMON_OBJECT_SLOT1
    &Spell::EffectSummonObject,                             //105 SPELL_EFFECT_SUMMON_OBJECT_SLOT2
    &Spell::EffectSummonObject,                             //106 SPELL_EFFECT_SUMMON_OBJECT_SLOT3
    &Spell::EffectSummonObject,                             //107 SPELL_EFFECT_SUMMON_OBJECT_SLOT4
    &Spell::EffectDispelMechanic,                           //108 SPELL_EFFECT_DISPEL_MECHANIC
    &Spell::EffectSummonDeadPet,                            //109 SPELL_EFFECT_SUMMON_DEAD_PET
    &Spell::EffectDestroyAllTotems,                         //110 SPELL_EFFECT_DESTROY_ALL_TOTEMS
    &Spell::EffectDurabilityDamage,                         //111 SPELL_EFFECT_DURABILITY_DAMAGE
    &Spell::EffectUnused,                                   //112 SPELL_EFFECT_112
    &Spell::EffectResurrectNew,                             //113 SPELL_EFFECT_RESURRECT_NEW
    &Spell::EffectTaunt,                                    //114 SPELL_EFFECT_ATTACK_ME
    &Spell::EffectDurabilityDamagePCT,                      //115 SPELL_EFFECT_DURABILITY_DAMAGE_PCT
    &Spell::EffectSkinPlayerCorpse,                         //116 SPELL_EFFECT_SKIN_PLAYER_CORPSE       one spell: Remove Insignia, bg usage, required special corpse flags...
    &Spell::EffectSpiritHeal,                               //117 SPELL_EFFECT_SPIRIT_HEAL              one spell: Spirit Heal
    &Spell::EffectSkill,                                    //118 SPELL_EFFECT_SKILL                    professions and more
    &Spell::EffectApplyAreaAura,                            //119 SPELL_EFFECT_APPLY_AREA_AURA_PET
    &Spell::EffectUnused,                                   //120 SPELL_EFFECT_TELEPORT_GRAVEYARD       one spell: Graveyard Teleport Test
    &Spell::EffectWeaponDmg,                                //121 SPELL_EFFECT_NORMALIZED_WEAPON_DMG
    &Spell::EffectUnused,                                   //122 SPELL_EFFECT_122                      unused
    &Spell::EffectSendTaxi,                                 //123 SPELL_EFFECT_SEND_TAXI                taxi/flight related (misc value is taxi path id)
    &Spell::EffectPlayerPull,                               //124 SPELL_EFFECT_PLAYER_PULL              opposite of knockback effect (pulls player twoard caster)
    &Spell::EffectModifyThreatPercent,                      //125 SPELL_EFFECT_MODIFY_THREAT_PERCENT
    &Spell::EffectStealBeneficialBuff,                      //126 SPELL_EFFECT_STEAL_BENEFICIAL_BUFF    spell steal effect?
    &Spell::EffectProspecting,                              //127 SPELL_EFFECT_PROSPECTING              Prospecting spell
    &Spell::EffectApplyAreaAura,                            //128 SPELL_EFFECT_APPLY_AREA_AURA_FRIEND
    &Spell::EffectApplyAreaAura,                            //129 SPELL_EFFECT_APPLY_AREA_AURA_ENEMY
    &Spell::EffectNULL,                                     //130 SPELL_EFFECT_REDIRECT_THREAT
    &Spell::EffectUnused,                                   //131 SPELL_EFFECT_131                      used in some test spells
    &Spell::EffectNULL,                                     //132 SPELL_EFFECT_PLAY_MUSIC               sound id in misc value (SoundEntries.dbc)
    &Spell::EffectUnlearnSpecialization,                    //133 SPELL_EFFECT_UNLEARN_SPECIALIZATION   unlearn profession specialization
    &Spell::EffectKillCredit,                               //134 SPELL_EFFECT_KILL_CREDIT              misc value is creature entry
    &Spell::EffectNULL,                                     //135 SPELL_EFFECT_CALL_PET
    &Spell::EffectHealPct,                                  //136 SPELL_EFFECT_HEAL_PCT
    &Spell::EffectEnergisePct,                              //137 SPELL_EFFECT_ENERGIZE_PCT
    &Spell::EffectNULL,                                     //138 SPELL_EFFECT_138                      Leap
    &Spell::EffectUnused,                                   //139 SPELL_EFFECT_CLEAR_QUEST              (misc - is quest ID)
    &Spell::EffectForceCast,                                //140 SPELL_EFFECT_FORCE_CAST
    &Spell::EffectNULL,                                     //141 SPELL_EFFECT_141                      damage and reduce speed?
    &Spell::EffectTriggerSpellWithValue,                    //142 SPELL_EFFECT_TRIGGER_SPELL_WITH_VALUE
    &Spell::EffectApplyAreaAura,                            //143 SPELL_EFFECT_APPLY_AREA_AURA_OWNER
    &Spell::EffectNULL,                                     //144 SPELL_EFFECT_144                      Spectral Blast
    &Spell::EffectNULL,                                     //145 SPELL_EFFECT_145                      Black Hole Effect
    &Spell::EffectActivateRune,                             //146 SPELL_EFFECT_ACTIVATE_RUNE
    &Spell::EffectQuestFail,                                //147 SPELL_EFFECT_QUEST_FAIL               quest fail
    &Spell::EffectUnused,                                   //148 SPELL_EFFECT_148                      unused
    &Spell::EffectNULL,                                     //149 SPELL_EFFECT_149                      swoop
    &Spell::EffectUnused,                                   //150 SPELL_EFFECT_150                      unused
    &Spell::EffectTriggerRitualOfSummoning,                 //151 SPELL_EFFECT_TRIGGER_SPELL_2
    &Spell::EffectNULL,                                     //152 SPELL_EFFECT_152                      summon Refer-a-Friend
    &Spell::EffectNULL,                                     //153 SPELL_EFFECT_CREATE_PET               misc value is creature entry
    &Spell::EffectNULL,                                     //154 unused
    &Spell::EffectTitanGrip,                                //155 SPELL_EFFECT_TITAN_GRIP Allows you to equip two-handed axes, maces and swords in one hand, but you attack $49152s1% slower than normal.
    &Spell::EffectEnchantItemPrismatic,                     //156 SPELL_EFFECT_ENCHANT_ITEM_PRISMATIC
    &Spell::EffectCreateItem2,                              //157 SPELL_EFFECT_CREATE_ITEM_2            create/learn item/spell for profession
    &Spell::EffectMilling,                                  //158 SPELL_EFFECT_MILLING                  milling
    &Spell::EffectRenamePet                                 //159 SPELL_EFFECT_ALLOW_RENAME_PET         allow rename pet once again
};

void Spell::EffectNULL(uint32 /*i*/)
{
    sLog.outDebug("WORLD: Spell Effect DUMMY");
}

void Spell::EffectUnused(uint32 /*i*/)
{
    // NOT USED BY ANY SPELL OR USELESS OR IMPLEMENTED IN DIFFERENT WAY IN MANGOS
}

void Spell::EffectResurrectNew(uint32 i)
{
    if(!unitTarget || unitTarget->isAlive())
        return;

    if(unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    if(!unitTarget->IsInWorld())
        return;

    Player* pTarget = ((Player*)unitTarget);

    if(pTarget->isRessurectRequested())       // already have one active request
        return;

    uint32 health = damage;
    uint32 mana = m_spellInfo->EffectMiscValue[i];
    pTarget->setResurrectRequestData(m_caster->GetGUID(), m_caster->GetMapId(), m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), health, mana);
    SendResurrectRequest(pTarget);
}

void Spell::EffectInstaKill(uint32 /*i*/)
{
    if( !unitTarget || !unitTarget->isAlive() )
        return;

    // Demonic Sacrifice
    if(m_spellInfo->Id==18788 && unitTarget->GetTypeId()==TYPEID_UNIT)
    {
        uint32 entry = unitTarget->GetEntry();
        uint32 spellID;
        switch(entry)
        {
            case   416: spellID=18789; break;               //imp
            case   417: spellID=18792; break;               //fellhunter
            case  1860: spellID=18790; break;               //void
            case  1863: spellID=18791; break;               //succubus
            case 17252: spellID=35701; break;               //fellguard
            default:
                sLog.outError("EffectInstaKill: Unhandled creature entry (%u) case.",entry);
                return;
        }

        m_caster->CastSpell(m_caster,spellID,true);
    }

	// Death Pact
	if(m_spellInfo->Id==48743)
	{	Unit *target = m_targets.getUnitTarget();
		if(target && !target->isDead())
		{
			int32 heal =  m_caster->GetMaxHealth()  * 0.4;
			m_caster->ModifyHealth(heal);
			m_caster->SendHealSpellLog(m_caster,48743,heal,false);
			target->SetHealth(0);
			target->setDeathState(JUST_DIED);
			m_targets.setUnitTarget(NULL);
		}
		return;
	}

    if(m_caster==unitTarget)                                // prevent interrupt message
        finish();

    uint32 health = unitTarget->GetHealth();
    m_caster->DealDamage(unitTarget, health, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
}

void Spell::EffectEnvirinmentalDMG(uint32 i)
{
    uint32 absorb = 0;
    uint32 resist = 0;

    // Note: this hack with damage replace required until GO casting not implemented
    // environment damage spells already have around enemies targeting but this not help in case not existed GO casting support
    // currently each enemy selected explicitly and self cast damage, we prevent apply self casted spell bonuses/etc
    damage = m_spellInfo->EffectBasePoints[i]+m_spellInfo->EffectBaseDice[i];

    m_caster->CalcAbsorbResist(m_caster,GetSpellSchoolMask(m_spellInfo), SPELL_DIRECT_DAMAGE, damage, &absorb, &resist);

    m_caster->SendSpellNonMeleeDamageLog(m_caster, m_spellInfo->Id, damage, GetSpellSchoolMask(m_spellInfo), absorb, resist, false, 0, false);
    if(m_caster->GetTypeId() == TYPEID_PLAYER)
        ((Player*)m_caster)->EnvironmentalDamage(DAMAGE_FIRE,damage);
}

void Spell::EffectSchoolDMG(uint32 effect_idx)
{
    if( unitTarget && unitTarget->isAlive())
    {
        switch(m_spellInfo->SpellFamilyName)
        {
            case SPELLFAMILY_GENERIC:
            {
                //Gore
                if(m_spellInfo->SpellIconID == 2269 )
                {
                    damage+= rand()%2 ? damage : 0;
                }

                switch(m_spellInfo->Id)                     // better way to check unknown
                {
                    // Meteor like spells (divided damage to targets)
                    case 24340: case 26558: case 28884:     // Meteor
                    case 36837: case 38903: case 41276:     // Meteor
                    case 26789:                             // Shard of the Fallen Star
                    case 31436:                             // Malevolent Cleave
                    case 35181:                             // Dive Bomb
                    case 40810: case 43267: case 43268:     // Saber Lash
                    case 42384:                             // Brutal Swipe
                    case 45150:                             // Meteor Slash
                    {
                        uint32 count = 0;
                        for(std::list<TargetInfo>::iterator ihit= m_UniqueTargetInfo.begin();ihit != m_UniqueTargetInfo.end();++ihit)
                            if(ihit->effectMask & (1<<effect_idx))
                                ++count;

                        damage /= count;                    // divide to all targets
                        break;
                    }
                    // percent from health with min
                    case 25599:                             // Thundercrash
                    {
                        damage = unitTarget->GetHealth() / 2;
                        if(damage < 200)
                            damage = 200;
                        break;
                    }
                    // Intercept (warrior spell trigger)
                    case 20253:
                    case 61491:
                    {
                        damage+= uint32(m_caster->GetTotalAttackPowerValue(BASE_ATTACK) * 0.12f);
                        break;
                    }
                }
                break;
            }

            case SPELLFAMILY_MAGE:
            {
                // Arcane Blast
                if(m_spellInfo->SpellFamilyFlags & 0x20000000LL)
                {
                    m_caster->CastSpell(m_caster,36032,true);
                }
                break;
            }
            case SPELLFAMILY_WARRIOR:
            {
                // Bloodthirst
                if(m_spellInfo->SpellFamilyFlags & 0x40000000000LL)
                {
                    damage = uint32(damage * (m_caster->GetTotalAttackPowerValue(BASE_ATTACK)) / 100);
                }
                // Shield Slam
                else if(m_spellInfo->SpellFamilyFlags & 0x0000020000000000LL)
                    damage += int32(m_caster->GetShieldBlockValue());
                // Victory Rush
                else if(m_spellInfo->SpellFamilyFlags & 0x10000000000LL)
                {
                    damage = uint32(damage * m_caster->GetTotalAttackPowerValue(BASE_ATTACK) / 100);
                    m_caster->ModifyAuraState(AURA_STATE_WARRIOR_VICTORY_RUSH, false);
                }
                // Revenge ${$m1+$AP*0.207} to ${$M1+$AP*0.207}
                else if(m_spellInfo->SpellFamilyFlags & 0x0000000000000400LL)
                    damage+= uint32(m_caster->GetTotalAttackPowerValue(BASE_ATTACK) * 0.207f);
                // Heroic Throw ${$m1+$AP*.50}
                else if(m_spellInfo->SpellFamilyFlags & 0x0000000100000000LL)
                    damage+= uint32(m_caster->GetTotalAttackPowerValue(BASE_ATTACK) * 0.5f);
                // Shockwave ${$m3/100*$AP}
                else if(m_spellInfo->SpellFamilyFlags & 0x0000800000000000LL)
                {
                    int32 pct = m_caster->CalculateSpellDamage(m_spellInfo, 2, m_spellInfo->EffectBasePoints[2], unitTarget);
                    if (pct > 0)
                        damage+= int32(m_caster->GetTotalAttackPowerValue(BASE_ATTACK) * pct / 100);
                    break;
                }
                break;
            }
            case SPELLFAMILY_WARLOCK:
            {
                // Incinerate Rank 1 & 2
                if((m_spellInfo->SpellFamilyFlags & 0x00004000000000LL) && m_spellInfo->SpellIconID==2128)
                {
                    // Incinerate does more dmg (dmg*0.25) if the target is Immolated.
                    if(unitTarget->HasAuraState(AURA_STATE_IMMOLATE))
                        damage += int32(damage*0.25f);
                }
                break;
            }
            case SPELLFAMILY_PRIEST:
            {
                // Shadow Word: Death - deals damage equal to damage done to caster
                if (m_spellInfo->SpellFamilyFlags & 0x0000000200000000LL)
                    m_caster->CastCustomSpell(m_caster, 32409, &damage, 0, 0, true);
                break;
            }
            case SPELLFAMILY_DRUID:
            {
                // Ferocious Bite
                if(m_caster->GetTypeId()==TYPEID_PLAYER && (m_spellInfo->SpellFamilyFlags & 0x000800000) && m_spellInfo->SpellVisual[0]==6587)
                {
                    // converts each extra point of energy into ($f1+$AP/410) additional damage
                    float ap = m_caster->GetTotalAttackPowerValue(BASE_ATTACK);
                    float multiple = ap / 410 + m_spellInfo->DmgMultiplier[effect_idx];
                    damage += int32(m_caster->GetPower(POWER_ENERGY) * multiple);
                    damage += int32(((Player*)m_caster)->GetComboPoints() * ap * 7 / 100);
                    m_caster->SetPower(POWER_ENERGY,0);
                }
                // Rake
                else if(m_spellInfo->SpellFamilyFlags & 0x0000000000001000LL)
                {
                    damage += int32(m_caster->GetTotalAttackPowerValue(BASE_ATTACK) / 100);
                }
                // Swipe
                else if(m_spellInfo->SpellFamilyFlags & 0x0010000000000000LL)
                {
                    damage += int32(m_caster->GetTotalAttackPowerValue(BASE_ATTACK)*0.08f);
                }
                //Mangle Bonus for the initial damage of Lacerate and Rake
                if((m_spellInfo->SpellFamilyFlags==0x0000000000001000LL && m_spellInfo->SpellIconID==494) ||
                    (m_spellInfo->SpellFamilyFlags==0x0000010000000000LL && m_spellInfo->SpellIconID==2246))
                {
                    Unit::AuraList const& mDummyAuras = unitTarget->GetAurasByType(SPELL_AURA_DUMMY);
                    for(Unit::AuraList::const_iterator i = mDummyAuras.begin(); i != mDummyAuras.end(); ++i)
                        if((*i)->GetSpellProto()->SpellFamilyFlags & 0x0000044000000000LL && (*i)->GetSpellProto()->SpellFamilyName==SPELLFAMILY_DRUID)
                        {
                            damage = int32(damage*(100.0f+(*i)->GetModifier()->m_amount)/100.0f);
                            break;
                        }
                }
                break;
            }
            case SPELLFAMILY_ROGUE:
            {
                // Envenom
                if(m_caster->GetTypeId()==TYPEID_PLAYER && (m_spellInfo->SpellFamilyFlags & 0x800000000LL))
                {
                    // consume from stack dozes not more that have combo-points
                    if(uint32 combo = ((Player*)m_caster)->GetComboPoints())
                    {
                        Aura *poison = 0;
                        // Lookup for Deadly poison (only attacker applied)
                        Unit::AuraList const& auras = unitTarget->GetAurasByType(SPELL_AURA_PERIODIC_DAMAGE);
                        for(Unit::AuraList::const_iterator itr = auras.begin(); itr!=auras.end(); ++itr)
                            if( (*itr)->GetSpellProto()->SpellFamilyName==SPELLFAMILY_ROGUE &&
                                (*itr)->GetSpellProto()->SpellFamilyFlags & 0x10000 &&
                                (*itr)->GetCasterGUID()==m_caster->GetGUID() )
                            {
                                poison = *itr;
                                break;
                            }
                        // count consumed deadly poison doses at target
                        if (poison)
                        {
                            uint32 spellId = poison->GetId();
                            uint32 doses = poison->GetStackAmount();
                            if (doses > combo)
                                doses = combo;
                            for (int i=0; i< doses; i++)
                                unitTarget->RemoveSingleSpellAurasFromStack(spellId);
                            damage *= doses;
                            damage += int32(((Player*)m_caster)->GetTotalAttackPowerValue(BASE_ATTACK) * 0.03f * doses);
                        }
                        // Eviscerate and Envenom Bonus Damage (item set effect)
                        if(m_caster->GetDummyAura(37169))
                            damage += ((Player*)m_caster)->GetComboPoints()*40;
                    }
                }
                // Eviscerate
                else if((m_spellInfo->SpellFamilyFlags & 0x00020000LL) && m_caster->GetTypeId()==TYPEID_PLAYER)
                {
                    if(uint32 combo = ((Player*)m_caster)->GetComboPoints())
                    {
                        float ap = m_caster->GetTotalAttackPowerValue(BASE_ATTACK);
                        damage += irand(int32(ap * combo * 0.03f), int32(ap * combo * 0.07f));

                        // Eviscerate and Envenom Bonus Damage (item set effect)
                        if(m_caster->GetDummyAura(37169))
                            damage += combo*40;
                    }
                }
                // Gouge
                else if(m_spellInfo->SpellFamilyFlags & 0x0000000000000008LL)
                {
                    damage += int32(m_caster->GetTotalAttackPowerValue(BASE_ATTACK)*0.02f);
                }
                // Instant Poison
                else if(m_spellInfo->SpellFamilyFlags & 0x0000000000002000LL)
                {
                    damage += int32(m_caster->GetTotalAttackPowerValue(BASE_ATTACK)*0.10f);
                }
                // Wound Poison
                else if(m_spellInfo->SpellFamilyFlags & 0x0000000010000000LL)
                {
                    damage += int32(m_caster->GetTotalAttackPowerValue(BASE_ATTACK)*0.04f);
                }
                break;
            }
            case SPELLFAMILY_HUNTER:
            {
                // Mongoose Bite
                if((m_spellInfo->SpellFamilyFlags & 0x000000002) && m_spellInfo->SpellVisual[0]==342)
                {
                    damage += int32(m_caster->GetTotalAttackPowerValue(BASE_ATTACK)*0.2f);
                }
                // Counterattack
                else if(m_spellInfo->SpellFamilyFlags & 0x0008000000000000LL)
                {
                    damage += int32(m_caster->GetTotalAttackPowerValue(BASE_ATTACK)*0.2f);
                }
                // Arcane Shot
                else if((m_spellInfo->SpellFamilyFlags & 0x00000800) && m_spellInfo->maxLevel > 0)
                {
                    damage += int32(m_caster->GetTotalAttackPowerValue(RANGED_ATTACK)*0.15f);
                }
                // Steady Shot
                else if(m_spellInfo->SpellFamilyFlags & 0x100000000LL)
                {
                    int32 base = irand((int32)m_caster->GetWeaponDamageRange(RANGED_ATTACK, MINDAMAGE),(int32)m_caster->GetWeaponDamageRange(RANGED_ATTACK, MAXDAMAGE));
                    damage += int32(float(base)/m_caster->GetAttackTime(RANGED_ATTACK)*2800 + m_caster->GetTotalAttackPowerValue(RANGED_ATTACK)*0.2f);
                }
                // Explosive Trap Effect
                else if(m_spellInfo->SpellFamilyFlags & 0x00000004)
                {
                    damage += int32(m_caster->GetTotalAttackPowerValue(RANGED_ATTACK)*0.1f);
                }
                break;
            }
            case SPELLFAMILY_PALADIN:
            {
                // Judgement of Vengeance ${1+0.22*$SPH+0.14*$AP} + 10% for each application of Holy Vengeance on the target
                if((m_spellInfo->SpellFamilyFlags & 0x800000000LL) && m_spellInfo->SpellIconID==2292)
                {
                    float ap = m_caster->GetTotalAttackPowerValue(BASE_ATTACK);
                    int32 holy = m_caster->SpellBaseDamageBonus(GetSpellSchoolMask(m_spellInfo)) +
                                 m_caster->SpellBaseDamageBonusForVictim(GetSpellSchoolMask(m_spellInfo), unitTarget);
                    damage+=int32(ap * 0.14f) + int32(holy * 22 / 100);
                    // Get stack of Holy Vengeance on the target added by caster
                    uint32 stacks = 0;
                    Unit::AuraList const& auras = unitTarget->GetAurasByType(SPELL_AURA_PERIODIC_DAMAGE);
                    for(Unit::AuraList::const_iterator itr = auras.begin(); itr!=auras.end(); ++itr)
                        if((*itr)->GetId() == 31803 && (*itr)->GetCasterGUID()==m_caster->GetGUID())
                        {
                            stacks = (*itr)->GetStackAmount();
                            break;
                        }
                    // + 10% for each application of Holy Vengeance on the target
                    if(stacks)
                        damage += damage * stacks * 10 /100;
                }
                // Avenger's Shield ($m1+0.07*$SPH+0.07*$AP) - ranged sdb for future
                else if(m_spellInfo->SpellFamilyFlags & 0x0000000000004000LL)
                {
                    float ap = m_caster->GetTotalAttackPowerValue(BASE_ATTACK);
                    int32 holy = m_caster->SpellBaseDamageBonus(GetSpellSchoolMask(m_spellInfo)) +
                                 m_caster->SpellBaseDamageBonusForVictim(GetSpellSchoolMask(m_spellInfo), unitTarget);
                    damage += int32(ap * 0.07f) + int32(holy * 7 / 100);
                }
                // Hammer of Wrath ($m1+0.15*$SPH+0.15*$AP) - ranged type sdb future fix
                else if(m_spellInfo->SpellFamilyFlags & 0x0000008000000000LL)
                {
                    float ap = m_caster->GetTotalAttackPowerValue(BASE_ATTACK);
                    int32 holy = m_caster->SpellBaseDamageBonus(GetSpellSchoolMask(m_spellInfo)) +
                                 m_caster->SpellBaseDamageBonusForVictim(GetSpellSchoolMask(m_spellInfo), unitTarget);
                    damage += int32(ap * 0.15f) + int32(holy * 15 / 100);
                }
                // Hammer of the Righteous
                else if(m_spellInfo->SpellFamilyFlags&0x0004000000000000LL)
                {
                    // Add main hand dps * effect[2] amount
                    float averange = (m_caster->GetFloatValue(UNIT_FIELD_MINDAMAGE) + m_caster->GetFloatValue(UNIT_FIELD_MAXDAMAGE)) / 2;
                    int32 count = m_caster->CalculateSpellDamage(m_spellInfo, 2, m_spellInfo->EffectBasePoints[2], unitTarget);
                    damage += count * int32(averange * IN_MILISECONDS) / m_caster->GetAttackTime(BASE_ATTACK);
                }
                // Shield of Righteousness
                else if(m_spellInfo->SpellFamilyFlags&0x0010000000000000LL)
                {
                    damage+=int32(m_caster->GetShieldBlockValue());
                }
                break;
            }
        }

        if(damage >= 0)
            m_damage+= damage;
    }
}

void Spell::EffectDummy(uint32 i)
{
    if(!unitTarget && !gameObjTarget && !itemTarget)
        return;

    //raise dead
    if( m_spellInfo->Id == 46584 && i == 2)
	EffectSpecialSummon( 52150, 0, m_caster );

    // Death Grip
      //todo: insert it to right spell family
    if(m_spellInfo->Id == 49560 || m_spellInfo->Id == 49576)//wtf 2 IDs
    {
         // Init dest coordinates
	 if(m_caster && unitTarget)
	 {	      
	    uint32 mapid = m_caster->GetMapId();
	    float x = m_caster->GetPositionX();
	    float y = m_caster->GetPositionY();
	    float z = m_caster->GetPositionZ()+1;
	    float orientation = unitTarget->GetOrientation();
				
	     unitTarget->SendMonsterMove(x, y, z, orientation, MOVEMENTFLAG_JUMPING, 1);
	     if(unitTarget->GetTypeId() != TYPEID_PLAYER)
	     unitTarget->GetMap()->CreatureRelocation((Creature*)unitTarget,x,y,z,unitTarget->GetOrientation());
	 }
     }
    // selection by spell family
    switch(m_spellInfo->SpellFamilyName)
    {
        case SPELLFAMILY_GENERIC:
        {
            switch(m_spellInfo->Id )
            {
                case 8063:                                  // Deviate Fish
                {
                    if(m_caster->GetTypeId() != TYPEID_PLAYER)
                        return;

                    uint32 spell_id = 0;
                    switch(urand(1,5))
                    {
                        case 1: spell_id = 8064; break;     // Sleepy
                        case 2: spell_id = 8065; break;     // Invigorate
                        case 3: spell_id = 8066; break;     // Shrink
                        case 4: spell_id = 8067; break;     // Party Time!
                        case 5: spell_id = 8068; break;     // Healthy Spirit
                    }
                    m_caster->CastSpell(m_caster,spell_id,true,NULL);
                    return;
                }
                case 8213:                                  // Savory Deviate Delight
                {
                    if(m_caster->GetTypeId() != TYPEID_PLAYER)
                        return;

                    uint32 spell_id = 0;
                    switch(urand(1,2))
                    {
                        // Flip Out - ninja
                        case 1: spell_id = (m_caster->getGender() == GENDER_MALE ? 8219 : 8220); break;
                        // Yaaarrrr - pirate
                        case 2: spell_id = (m_caster->getGender() == GENDER_MALE ? 8221 : 8222); break;
                    }
                    m_caster->CastSpell(m_caster,spell_id,true,NULL);
                    return;
                }
                case 8593:                                  // Symbol of life (restore creature to life)
                case 31225:                                 // Shimmering Vessel (restore creature to life)
                {
                    if(!unitTarget || unitTarget->GetTypeId()!=TYPEID_UNIT)
                        return;
                    ((Creature*)unitTarget)->setDeathState(JUST_ALIVED);
                    return;
                }
                case 12162:                                 // Deep wounds
                case 12850:                                 // (now good common check for this spells)
                case 12868:
                {
                    if(!unitTarget)
                        return;

                    float damage;
                    // DW should benefit of attack power, damage percent mods etc.
                    // TODO: check if using offhand damage is correct and if it should be divided by 2
                    if (m_caster->haveOffhandWeapon() && m_caster->getAttackTimer(BASE_ATTACK) > m_caster->getAttackTimer(OFF_ATTACK))
                        damage = (m_caster->GetFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE) + m_caster->GetFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE))/2;
                    else
                        damage = (m_caster->GetFloatValue(UNIT_FIELD_MINDAMAGE) + m_caster->GetFloatValue(UNIT_FIELD_MAXDAMAGE))/2;

                    switch (m_spellInfo->Id)
                    {
                        case 12850: damage *= 0.2f; break;
                        case 12162: damage *= 0.4f; break;
                        case 12868: damage *= 0.6f; break;
                        default:
                            sLog.outError("Spell::EffectDummy: Spell %u not handled in DW",m_spellInfo->Id);
                            return;
                    };

                    int32 deepWoundsDotBasePoints0 = int32(damage / 4);
                    m_caster->CastCustomSpell(unitTarget, 12721, &deepWoundsDotBasePoints0, NULL, NULL, true, NULL);
                    return;
                }
                case 13120:                                 // net-o-matic
                {
                    if(!unitTarget)
                        return;

                    uint32 spell_id = 0;

                    uint32 roll = urand(0, 99);

                    if(roll < 2)                            // 2% for 30 sec self root (off-like chance unknown)
                        spell_id = 16566;
                    else if(roll < 4)                       // 2% for 20 sec root, charge to target (off-like chance unknown)
                        spell_id = 13119;
                    else                                    // normal root
                        spell_id = 13099;

                    m_caster->CastSpell(unitTarget,spell_id,true,NULL);
                    return;
                }
                case 13567:                                 // Dummy Trigger
                {
                    // can be used for different aura triggering, so select by aura
                    if(!m_triggeredByAuraSpell || !unitTarget)
                        return;

                    switch(m_triggeredByAuraSpell->Id)
                    {
                        case 26467:                         // Persistent Shield
                            m_caster->CastCustomSpell(unitTarget, 26470, &damage, NULL, NULL, true);
                            break;
                        default:
                            sLog.outError("EffectDummy: Non-handled case for spell 13567 for triggered aura %u",m_triggeredByAuraSpell->Id);
                            break;
                    }
                    return;
                }
                case 15998:                                 // Capture Worg Pup
                case 29435:                                 // Capture Female Kaliri Hatchling
                {
                    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_UNIT)
                        return;

                    Creature* creatureTarget = (Creature*)unitTarget;
                    creatureTarget->setDeathState(JUST_DIED);
                    creatureTarget->RemoveCorpse();
                    creatureTarget->SetHealth(0);           // just for nice GM-mode view
                    return;
                }
                case 16589:                                 // Noggenfogger Elixir
                {
                    if(m_caster->GetTypeId()!=TYPEID_PLAYER)
                        return;

                    uint32 spell_id = 0;
                    switch(urand(1,3))
                    {
                        case 1: spell_id = 16595; break;
                        case 2: spell_id = 16593; break;
                        default:spell_id = 16591; break;
                    }

                    m_caster->CastSpell(m_caster,spell_id,true,NULL);
                    return;
                }
                case 17251:                                 // Spirit Healer Res
                {
                    if(!unitTarget || !m_originalCaster)
                        return;

                    if(m_originalCaster->GetTypeId() == TYPEID_PLAYER)
                    {
                        WorldPacket data(SMSG_SPIRIT_HEALER_CONFIRM, 8);
                        data << unitTarget->GetGUID();
                        ((Player*)m_originalCaster)->GetSession()->SendPacket( &data );
                    }
                    return;
                }
                case 17271:                                 // Test Fetid Skull
                {
                    if(!itemTarget && m_caster->GetTypeId()!=TYPEID_PLAYER)
                        return;

                    uint32 spell_id = roll_chance_i(50) ? 17269 : 17270;

                    m_caster->CastSpell(m_caster,spell_id,true,NULL);
                    return;
                }
                case 20577:                                 // Cannibalize
                    if (unitTarget)
                        m_caster->CastSpell(m_caster,20578,false,NULL);
                    return;
                case 23019:                                 // Crystal Prison Dummy DND
                {
                    if(!unitTarget || !unitTarget->isAlive() || unitTarget->GetTypeId() != TYPEID_UNIT || ((Creature*)unitTarget)->isPet())
                        return;

                    Creature* creatureTarget = (Creature*)unitTarget;
                    if(creatureTarget->isPet())
                        return;

                    GameObject* pGameObj = new GameObject;

                    Map *map = creatureTarget->GetMap();

                    // create before death for get proper coordinates
                    if(!pGameObj->Create(objmgr.GenerateLowGuid(HIGHGUID_GAMEOBJECT), 179644, map, m_caster->GetPhaseMask(),
                        creatureTarget->GetPositionX(), creatureTarget->GetPositionY(), creatureTarget->GetPositionZ(),
                        creatureTarget->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, 100, 1) )
                    {
                        delete pGameObj;
                        return;
                    }

                    pGameObj->SetRespawnTime(creatureTarget->GetRespawnTime()-time(NULL));
                    pGameObj->SetOwnerGUID(m_caster->GetGUID() );
                    pGameObj->SetUInt32Value(GAMEOBJECT_LEVEL, m_caster->getLevel() );
                    pGameObj->SetSpellId(m_spellInfo->Id);

                    creatureTarget->setDeathState(JUST_DIED);
                    creatureTarget->RemoveCorpse();
                    creatureTarget->SetHealth(0);                   // just for nice GM-mode view

                    DEBUG_LOG("AddObject at SpellEfects.cpp EffectDummy");
                    map->Add(pGameObj);

                    WorldPacket data(SMSG_GAMEOBJECT_SPAWN_ANIM_OBSOLETE, 8);
                    data << uint64(pGameObj->GetGUID());
                    m_caster->SendMessageToSet(&data,true);

                    return;
                }
                case 23074:                                 // Arcanite Dragonling
                    if (!m_CastItem) return;
                    m_caster->CastSpell(m_caster,19804,true,m_CastItem);
                    return;
                case 23075:                                 // Mithril Mechanical Dragonling
                    if (!m_CastItem) return;
                    m_caster->CastSpell(m_caster,12749,true,m_CastItem);
                    return;
                case 23076:                                 // Mechanical Dragonling
                    if (!m_CastItem) return;
                    m_caster->CastSpell(m_caster,4073,true,m_CastItem);
                    return;
                case 23133:                                 // Gnomish Battle Chicken
                    if (!m_CastItem) return;
                    m_caster->CastSpell(m_caster,13166,true,m_CastItem);
                    return;
                case 23448:                                 // Ultrasafe Transporter: Gadgetzan - backfires
                {
                    int32 r = irand(0, 119);
                    if ( r < 20 )                           // 1/6 polymorph
                        m_caster->CastSpell(m_caster,23444,true);
                    else if ( r < 100 )                     // 4/6 evil twin
                        m_caster->CastSpell(m_caster,23445,true);
                    else                                    // 1/6 miss the target
                        m_caster->CastSpell(m_caster,36902,true);
                    return;
                }
                case 23453:                                 // Ultrasafe Transporter: Gadgetzan
                    if ( roll_chance_i(50) )                // success
                        m_caster->CastSpell(m_caster,23441,true);
                    else                                    // failure
                        m_caster->CastSpell(m_caster,23446,true);
                    return;
                case 23645:                                 // Hourglass Sand
                    m_caster->RemoveAurasDueToSpell(23170);
                    return;
                case 23725:                                 // Gift of Life (warrior bwl trinket)
                    m_caster->CastSpell(m_caster,23782,true);
                    m_caster->CastSpell(m_caster,23783,true);
                    return;
                case 25860:                                 // Reindeer Transformation
                {
                    if (!m_caster->HasAuraType(SPELL_AURA_MOUNTED))
                        return;

                    float flyspeed = m_caster->GetSpeedRate(MOVE_FLIGHT);
                    float speed = m_caster->GetSpeedRate(MOVE_RUN);

                    m_caster->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);

                    //5 different spells used depending on mounted speed and if mount can fly or not
                    if (flyspeed >= 4.1f)
                        m_caster->CastSpell(m_caster, 44827, true); //310% flying Reindeer
                    else if (flyspeed >= 3.8f)
                        m_caster->CastSpell(m_caster, 44825, true); //280% flying Reindeer
                    else if (flyspeed >= 1.6f)
                        m_caster->CastSpell(m_caster, 44824, true); //60% flying Reindeer
                    else if (speed >= 2.0f)
                        m_caster->CastSpell(m_caster, 25859, true); //100% ground Reindeer
                    else
                        m_caster->CastSpell(m_caster, 25858, true); //60% ground Reindeer

                    return;
                }
                //case 26074:                               // Holiday Cheer
                //    return; -- implemented at client side
                case 28006:                                 // Arcane Cloaking
                {
                    if( unitTarget->GetTypeId() == TYPEID_PLAYER )
                        m_caster->CastSpell(unitTarget,29294,true);
                    return;
                }
                case 28730:                                 // Arcane Torrent (Mana)
                {
                    Aura * dummy = m_caster->GetDummyAura(28734);
                    if (dummy)
                    {
                        int32 bp = damage * dummy->GetStackAmount();
                        m_caster->CastCustomSpell(m_caster, 28733, &bp, NULL, NULL, true);
                        m_caster->RemoveAurasDueToSpell(28734);
                    }
                    return;
                }
                case 29200:                                 // Purify Helboar Meat
                {
                    if( m_caster->GetTypeId() != TYPEID_PLAYER )
                        return;

                    uint32 spell_id = roll_chance_i(50) ? 29277 : 29278;

                    m_caster->CastSpell(m_caster,spell_id,true,NULL);
                    return;
                }
                case 29858:                                 // Soulshatter
                    if (unitTarget && unitTarget->GetTypeId() == TYPEID_UNIT && unitTarget->IsHostileTo(m_caster))
                        m_caster->CastSpell(unitTarget,32835,true);
                    return;
                case 30458:                                 // Nigh Invulnerability
                    if (!m_CastItem) return;
                    if(roll_chance_i(86))                   // success
                        m_caster->CastSpell(m_caster, 30456, true, m_CastItem);
                    else                                    // backfire in 14% casts
                        m_caster->CastSpell(m_caster, 30457, true, m_CastItem);
                    return;
                case 30507:                                 // Poultryizer
                    if (!m_CastItem) return;
                    if(roll_chance_i(80))                   // success
                        m_caster->CastSpell(unitTarget, 30501, true, m_CastItem);
                    else                                    // backfire 20%
                        m_caster->CastSpell(unitTarget, 30504, true, m_CastItem);
                    return;
                case 33060:                                         // Make a Wish
                {
                    if(m_caster->GetTypeId()!=TYPEID_PLAYER)
                        return;

                    uint32 spell_id = 0;

                    switch(urand(1,5))
                    {
                        case 1: spell_id = 33053; break;
                        case 2: spell_id = 33057; break;
                        case 3: spell_id = 33059; break;
                        case 4: spell_id = 33062; break;
                        case 5: spell_id = 33064; break;
                    }

                    m_caster->CastSpell(m_caster,spell_id,true,NULL);
                    return;
                }
                case 35745:
                {
                    uint32 spell_id;
                    switch(m_caster->GetAreaId())
                    {
                        case 3900: spell_id = 35743; break;
                        case 3742: spell_id = 35744; break;
                        default: return;
                    }

                    m_caster->CastSpell(m_caster,spell_id,true);
                    return;
                }
                case 37674:                                 // Chaos Blast
                {
                    if(!unitTarget)
                        return;

                    int32 basepoints0 = 100;
                    m_caster->CastCustomSpell(unitTarget,37675,&basepoints0,NULL,NULL,true);
                    return;
                }
                case 40802:                                 // Mingo's Fortune Generator (Mingo's Fortune Giblets)
                {
                    // selecting one from Bloodstained Fortune item
                    uint32 newitemid;
                    switch(urand(1,20))
                    {
                        case 1:  newitemid = 32688; break;
                        case 2:  newitemid = 32689; break;
                        case 3:  newitemid = 32690; break;
                        case 4:  newitemid = 32691; break;
                        case 5:  newitemid = 32692; break;
                        case 6:  newitemid = 32693; break;
                        case 7:  newitemid = 32700; break;
                        case 8:  newitemid = 32701; break;
                        case 9:  newitemid = 32702; break;
                        case 10: newitemid = 32703; break;
                        case 11: newitemid = 32704; break;
                        case 12: newitemid = 32705; break;
                        case 13: newitemid = 32706; break;
                        case 14: newitemid = 32707; break;
                        case 15: newitemid = 32708; break;
                        case 16: newitemid = 32709; break;
                        case 17: newitemid = 32710; break;
                        case 18: newitemid = 32711; break;
                        case 19: newitemid = 32712; break;
                        case 20: newitemid = 32713; break;
                        default:
                            return;
                    }

                    DoCreateItem(i,newitemid);
                    return;
                }
                // Demon Broiled Surprise
                /* FIX ME: Required for correct work implementing implicit target 7 (in pair (22,7))
                case 43723:
                {
                    if (m_caster->GetTypeId() != TYPEID_PLAYER)
                        return;

                    ((Player*)m_caster)->CastSpell(unitTarget, 43753, true);
                    return;
                }
                */
                case 44875:                                 // Complete Raptor Capture
                {
                    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_UNIT)
                        return;

                    Creature* creatureTarget = (Creature*)unitTarget;

                    creatureTarget->setDeathState(JUST_DIED);
                    creatureTarget->RemoveCorpse();
                    creatureTarget->SetHealth(0);           // just for nice GM-mode view

                    //cast spell Raptor Capture Credit
                    m_caster->CastSpell(m_caster,42337,true,NULL);
                    return;
                }
                case 34665:                                 //Administer Antidote
                {
                    if(!unitTarget || m_caster->GetTypeId() != TYPEID_PLAYER )
                        return;

                    if(!unitTarget)
                        return;

                    TemporarySummon* tempSummon = dynamic_cast<TemporarySummon*>(unitTarget);
                    if(!tempSummon)
                        return;

                    uint32 health = tempSummon->GetHealth();

                    float x = tempSummon->GetPositionX();
                    float y = tempSummon->GetPositionY();
                    float z = tempSummon->GetPositionZ();
                    float o = tempSummon->GetOrientation();
                    tempSummon->UnSummon();

                    Creature* pCreature = m_caster->SummonCreature(16992, x, y, z, o,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,180000);
                    if (!pCreature)
                        return;

                    pCreature->SetHealth(health);
                    ((Player*)m_caster)->KilledMonster(16992,pCreature->GetGUID());

                    if (pCreature->AI())
                        pCreature->AI()->AttackStart(m_caster);

                    return;
                }
                case 44997:                                 // Converting Sentry
                {
                    //Converted Sentry Credit
                    m_caster->CastSpell(m_caster, 45009, true);
                    return;
                }
                case 45030:                                 // Impale Emissary
                {
                    // Emissary of Hate Credit
                    m_caster->CastSpell(m_caster, 45088, true);
                    return;
                }
                case 50243:                                 // Teach Language
                {
                    if(m_caster->GetTypeId() != TYPEID_PLAYER)
                        return;

                    // spell has a 1/3 chance to trigger one of the below
                    if(roll_chance_i(66))
                        return;
                    if(((Player*)m_caster)->GetTeam() == ALLIANCE)
                    {
                        // 1000001 - gnomish binary
                        m_caster->CastSpell(m_caster, 50242, true);
                    }
                    else
                    {
                        // 01001000 - goblin binary
                        m_caster->CastSpell(m_caster, 50246, true);
                    }

                    return;
                }
                case 51582:                                 //Rocket Boots Engaged (Rocket Boots Xtreme and Rocket Boots Xtreme Lite)
                {
                    if(m_caster->GetTypeId() != TYPEID_PLAYER)
                        return;

                    if(BattleGround* bg = ((Player*)m_caster)->GetBattleGround())
                        bg->EventPlayerDroppedFlag((Player*)m_caster);

                    m_caster->CastSpell(m_caster, 30452, true, NULL);
                    return;
                }
                case 53341:
                case 53343:
                {
                    m_caster->CastSpell(m_caster,54586,true);
                    return;
                }
                case 58418:                                 // Portal to Orgrimmar
                {
                    if(!unitTarget)
                        return;

                    unitTarget->CastSpell(unitTarget, 58419, true);
                    return;
                }
                case 58420:                                 // Portal to Stormwind
                {
                    if(!unitTarget)
                        return;

                    unitTarget->CastSpell(unitTarget, 58421, true);
                    return;
                }
            }

            //All IconID Check in there
            switch(m_spellInfo->SpellIconID)
            {
                // Berserking (troll racial traits)
                case 1661:
                {
                    uint32 healthPerc = uint32((float(m_caster->GetHealth())/m_caster->GetMaxHealth())*100);
                    int32 melee_mod = 10;
                    if (healthPerc <= 40)
                        melee_mod = 30;
                    if (healthPerc < 100 && healthPerc > 40)
                        melee_mod = 10+(100-healthPerc)/3;

                    int32 hasteModBasePoints0 = melee_mod;          // (EffectBasePoints[0]+1)-1+(5-melee_mod) = (melee_mod-1+1)-1+5-melee_mod = 5-1
                    int32 hasteModBasePoints1 = (5-melee_mod);
                    int32 hasteModBasePoints2 = 5;

                    // FIXME: custom spell required this aura state by some unknown reason, we not need remove it anyway
                    m_caster->ModifyAuraState(AURA_STATE_BERSERKING,true);
                    m_caster->CastCustomSpell(m_caster,26635,&hasteModBasePoints0,&hasteModBasePoints1,&hasteModBasePoints2,true,NULL);
                    return;
                }
            }
            break;
        }
        case SPELLFAMILY_MAGE:
            switch(m_spellInfo->Id )
            {
                case 11958:                                 // Cold Snap
                {
                    if(m_caster->GetTypeId()!=TYPEID_PLAYER)
                        return;

                    // immediately finishes the cooldown on Frost spells
                    const PlayerSpellMap& sp_list = ((Player *)m_caster)->GetSpellMap();
                    for (PlayerSpellMap::const_iterator itr = sp_list.begin(); itr != sp_list.end(); ++itr)
                    {
                        if (itr->second->state == PLAYERSPELL_REMOVED)
                            continue;

                        uint32 classspell = itr->first;
                        SpellEntry const *spellInfo = sSpellStore.LookupEntry(classspell);

                        if( spellInfo->SpellFamilyName == SPELLFAMILY_MAGE &&
                            (GetSpellSchoolMask(spellInfo) & SPELL_SCHOOL_MASK_FROST) &&
                            spellInfo->Id != 11958 && GetSpellRecoveryTime(spellInfo) > 0 )
                        {
                            ((Player*)m_caster)->RemoveSpellCooldown(classspell);

                            WorldPacket data(SMSG_CLEAR_COOLDOWN, (4+8));
                            data << uint32(classspell);
                            data << uint64(m_caster->GetGUID());
                            ((Player*)m_caster)->GetSession()->SendPacket(&data);
                        }
                    }
                    return;
                }
                case 32826:
                {
                    if ( unitTarget && unitTarget->GetTypeId() == TYPEID_UNIT )
                    {
                        //Polymorph Cast Visual Rank 1
                        const uint32 spell_list[6] = {32813, 32816, 32817, 32818, 32819, 32820};
                        unitTarget->CastSpell( unitTarget, spell_list[urand(0, 5)], true);
                    }
                    return;
                }
            }
            break;
        case SPELLFAMILY_WARRIOR:
            // Charge
            if(m_spellInfo->SpellFamilyFlags & 0x1 && m_spellInfo->SpellVisual[0] == 867)
            {
                int32 chargeBasePoints0 = damage;
                m_caster->CastCustomSpell(m_caster,34846,&chargeBasePoints0,NULL,NULL,true);
                return;
            }
            // Execute
            if(m_spellInfo->SpellFamilyFlags & 0x20000000)
            {
                 if(!unitTarget)
                     return;
        int lastrage=0;
        uint32 rage2 = 30;
        uint32 rage = m_caster->GetPower(POWER_RAGE);

        if(m_targets.getUnitTarget()->GetHealth() < m_targets.getUnitTarget()->GetMaxHealth()*0.2) rage2=rage; //Clean execute, ignore Sudden death
        else if(m_caster->HasAura(29723)) lastrage=3;
        else if (m_caster->HasAura(29725)) lastrage=7;
        else if (m_caster->HasAura(29724)) lastrage=10;
		
                 // Glyph of Execution bonus
                 if (Aura *aura = m_caster->GetDummyAura(58367))
                 rage2+=aura->GetModifier()->m_amount;
 
                int32 basePoints0 = damage+int32(rage2 * m_spellInfo->DmgMultiplier[i] +
                                                  m_caster->GetTotalAttackPowerValue(BASE_ATTACK)*0.2f);
                 m_caster->CastCustomSpell(unitTarget, 20647, &basePoints0, NULL, NULL, true, 0);

        if(m_caster->HasAura(52437)) m_caster->RemoveSingleSpellAurasFromStack(52437);
        if(lastrage !=0  ) 
        {
        if(rage-30 < lastrage)
            m_caster->SetPower(POWER_RAGE,lastrage);
        else
            m_caster->SetPower(POWER_RAGE,rage-30);

        }
        else
        {
               		m_caster->SetPower(POWER_RAGE,0);
        }
		
                 return;
             }
            // Slam
            if(m_spellInfo->SpellFamilyFlags & 0x0000000000200000LL)
            {
                if(!unitTarget)
                    return;
                m_damage+=m_caster->CalculateDamage(m_attackType, false);
                m_damage+=damage;
                return;
            }
            switch(m_spellInfo->Id)
            {
                // Warrior's Wrath
                case 21977:
                {
                    if(!unitTarget)
                        return;
                    m_caster->CastSpell(unitTarget,21887,true); // spell mod
                    return;
                }
                // Last Stand
                case 12975:
                {
                    int32 healthModSpellBasePoints0 = int32(m_caster->GetMaxHealth()*0.3);
                    m_caster->CastCustomSpell(m_caster, 12976, &healthModSpellBasePoints0, NULL, NULL, true, NULL);
                    return;
                }
                // Bloodthirst
                case 23881:
                {
                    m_caster->CastCustomSpell(unitTarget, 23885, &damage, NULL, NULL, true, NULL);
                    return;
                }
            }
            break;
        case SPELLFAMILY_WARLOCK:
            // Life Tap
            if (m_spellInfo->SpellFamilyFlags & 0x0000000000040000LL)
            {
                // In 303 exist spirit depend
                uint32 spirit = uint32(m_caster->GetStat(STAT_SPIRIT));
                switch (m_spellInfo->Id)
                {
                    case  1454: damage+=spirit; break;
                    case  1455: damage+=spirit*15/10; break;
                    case  1456: damage+=spirit*2; break;
                    case 11687: damage+=spirit*25/10; break;
                    case 11688:
                    case 11689:
                    case 27222:
                    case 57946: damage+=spirit*3; break;
                    default:
                        sLog.outError("Spell::EffectDummy: %u Life Tap need set spirit multipler", m_spellInfo->Id);
                        return;
                }
//              Think its not need (also need remove Life Tap from SpellDamageBonus or add new value)
//              damage = m_caster->SpellDamageBonus(m_caster, m_spellInfo,uint32(damage > 0 ? damage : 0), SPELL_DIRECT_DAMAGE);
                if(int32(unitTarget->GetHealth()) > damage)
                {
                    // Shouldn't Appear in Combat Log
                    unitTarget->ModifyHealth(-damage);

                    int32 mana = damage;
                    // Improved Life Tap mod
                    Unit::AuraList const& auraDummy = m_caster->GetAurasByType(SPELL_AURA_DUMMY);
                    for(Unit::AuraList::const_iterator itr = auraDummy.begin(); itr != auraDummy.end(); ++itr)
                    {
                        if((*itr)->GetSpellProto()->SpellFamilyName==SPELLFAMILY_WARLOCK && (*itr)->GetSpellProto()->SpellIconID == 208)
                            mana = ((*itr)->GetModifier()->m_amount + 100)* mana / 100;
                    }
                    m_caster->CastCustomSpell(unitTarget, 31818, &mana, NULL, NULL, true);

                    // Mana Feed
                    int32 manaFeedVal = 0;
                    Unit::AuraList const& mod = m_caster->GetAurasByType(SPELL_AURA_ADD_FLAT_MODIFIER);
                    for(Unit::AuraList::const_iterator itr = mod.begin(); itr != mod.end(); ++itr)
                    {
                        if((*itr)->GetSpellProto()->SpellFamilyName==SPELLFAMILY_WARLOCK && (*itr)->GetSpellProto()->SpellIconID == 1982)
                            manaFeedVal+= (*itr)->GetModifier()->m_amount;
                    }
                    if(manaFeedVal > 0)
                    {
                        manaFeedVal = manaFeedVal * mana / 100;
                        m_caster->CastCustomSpell(m_caster, 32553, &manaFeedVal, NULL, NULL, true, NULL);
                    }
                }
                else
                    SendCastResult(SPELL_FAILED_FIZZLE);
                return;
            }
            break;
        case SPELLFAMILY_PRIEST:
            // Penance
            if (m_spellInfo->SpellFamilyFlags & 0x0080000000000000LL)
            {
                if (!unitTarget)
                    return;

                int hurt = 0;
                int heal = 0;
                switch(m_spellInfo->Id)
                {
                    case 47540: hurt = 47758; heal = 47757; break;
                    case 53005: hurt = 53001; heal = 52986; break;
                    case 53006: hurt = 53002; heal = 52987; break;
                    case 53007: hurt = 53003; heal = 52988; break;
                    default:
                        sLog.outError("Spell::EffectDummy: Spell %u Penance need set correct heal/damage spell", m_spellInfo->Id);
                        return;
                }
                if (m_caster->IsFriendlyTo(unitTarget))
                    m_caster->CastSpell(unitTarget, heal, true, 0);
                else
                    m_caster->CastSpell(unitTarget, hurt, true, 0);
                return;
            }
            switch(m_spellInfo->Id )
            {
                case 28598:                                 // Touch of Weakness triggered spell
                {
                    if(!unitTarget || !m_triggeredByAuraSpell)
                        return;

                    uint32 spellid = 0;
                    switch(m_triggeredByAuraSpell->Id)
                    {
                        case 2652:  spellid =  2943; break; // Rank 1
                        case 19261: spellid = 19249; break; // Rank 2
                        case 19262: spellid = 19251; break; // Rank 3
                        case 19264: spellid = 19252; break; // Rank 4
                        case 19265: spellid = 19253; break; // Rank 5
                        case 19266: spellid = 19254; break; // Rank 6
                        case 25461: spellid = 25460; break; // Rank 7
                        default:
                            sLog.outError("Spell::EffectDummy: Spell 28598 triggered by unhandeled spell %u",m_triggeredByAuraSpell->Id);
                            return;
                    }
                    m_caster->CastSpell(unitTarget, spellid, true, NULL);
                    return;
                }
            }
            break;
        case SPELLFAMILY_DRUID:
            // Starfall
            if (m_spellInfo->SpellFamilyFlags2 & 0x00000100LL)
            {
                //Shapeshifting into an animal form or mounting cancels the effect.
                if(m_caster->GetCreatureType() == CREATURE_TYPE_BEAST || m_caster->IsMounted())
                {
                    if(m_triggeredByAuraSpell)
                        m_caster->RemoveAurasDueToSpell(m_triggeredByAuraSpell->Id);
                    return;
                }

                //Any effect which causes you to lose control of your character will supress the starfall effect.
                if(m_caster->hasUnitState(UNIT_STAT_STUNNED | UNIT_STAT_FLEEING | UNIT_STAT_ROOT | UNIT_STAT_CONFUSED))
                    return;

                switch(m_spellInfo->Id)
                {
                    case 50286: m_caster->CastSpell(unitTarget, 50288, true); return;
                    case 53196: m_caster->CastSpell(unitTarget, 53191, true); return;
                    case 53197: m_caster->CastSpell(unitTarget, 53194, true); return;
                    case 53198: m_caster->CastSpell(unitTarget, 53195, true); return;
                    default:
                        sLog.outError("Spell::EffectDummy: Unhandeled Starfall spell rank %u",m_spellInfo->Id);
                        return;
                }
            }
            break;
        case SPELLFAMILY_ROGUE:
            switch(m_spellInfo->Id )
            {
                case 5938:                                  // Shiv
                {
                    if(m_caster->GetTypeId() != TYPEID_PLAYER)
                        return;

                    Player *pCaster = ((Player*)m_caster);

                    Item *item = pCaster->GetWeaponForAttack(OFF_ATTACK);
                    if(!item)
                        return;

                    // all poison enchantments is temporary
                    uint32 enchant_id = item->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT);
                    if(!enchant_id)
                        return;

                    SpellItemEnchantmentEntry const *pEnchant = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
                    if(!pEnchant)
                        return;

                    for (int s=0;s<3;s++)
                    {
                        if(pEnchant->type[s]!=ITEM_ENCHANTMENT_TYPE_COMBAT_SPELL)
                            continue;

                        SpellEntry const* combatEntry = sSpellStore.LookupEntry(pEnchant->spellid[s]);
                        if(!combatEntry || combatEntry->Dispel != DISPEL_POISON)
                            continue;

                        m_caster->CastSpell(unitTarget, combatEntry, true, item);
                    }

                    m_caster->CastSpell(unitTarget, 5940, true);
                    return;
                }
                case 14185:                                 // Preparation Rogue
                {
                    if(m_caster->GetTypeId()!=TYPEID_PLAYER)
                        return;

                    //immediately finishes the cooldown on certain Rogue abilities
                    const PlayerSpellMap& sp_list = ((Player *)m_caster)->GetSpellMap();
                    for (PlayerSpellMap::const_iterator itr = sp_list.begin(); itr != sp_list.end(); ++itr)
                    {
                        uint32 classspell = itr->first;
                        SpellEntry const *spellInfo = sSpellStore.LookupEntry(classspell);

                        if (spellInfo->SpellFamilyName == SPELLFAMILY_ROGUE && (spellInfo->SpellFamilyFlags & 0x0000024000000860LL))
                        {
                            ((Player*)m_caster)->RemoveSpellCooldown(classspell);

                            WorldPacket data(SMSG_CLEAR_COOLDOWN, (4+8));
                            data << uint32(classspell);
                            data << uint64(m_caster->GetGUID());
                            ((Player*)m_caster)->GetSession()->SendPacket(&data);
                        }
                    }
                    return;
                }
                case 31231:                                 // Cheat Death
                {
                    m_caster->CastSpell(m_caster,45182,true);
                    return;
                }
            }
            break;
        case SPELLFAMILY_HUNTER:
            // Steady Shot
            if(m_spellInfo->SpellFamilyFlags & 0x100000000LL)
            {
                if( !unitTarget || !unitTarget->isAlive())
                    return;

                bool found = false;

                // check dazed affect
                Unit::AuraList const& decSpeedList = unitTarget->GetAurasByType(SPELL_AURA_MOD_DECREASE_SPEED);
                for(Unit::AuraList::const_iterator iter = decSpeedList.begin(); iter != decSpeedList.end(); ++iter)
                {
                    if((*iter)->GetSpellProto()->SpellIconID==15 && (*iter)->GetSpellProto()->Dispel==0)
                    {
                        found = true;
                        break;
                    }
                }

                if(found)
                    m_damage+= damage;
                return;
            }

            switch(m_spellInfo->Id)
            {
                case 781:                                 // Disengage
                {
                    if (m_caster->GetTypeId()!=TYPEID_PLAYER)
                        return;

                    WorldPacket data(SMSG_MOVE_KNOCK_BACK, 50);
                    data.append(m_caster->GetPackGUID());
                    data << getMSTime();
                    data << float(cosf(m_caster->GetOrientation()+M_PI));
                    data << float(sinf(m_caster->GetOrientation()+M_PI));
                    data << float(15);
                    data << float(-7.0f);
                    ((Player*)m_caster)->GetSession()->SendPacket(&data);
                    return;
                }
                case 23989:                                 //Readiness talent
                {
                    if(m_caster->GetTypeId()!=TYPEID_PLAYER)
                        return;

                    //immediately finishes the cooldown for hunter abilities
                    const PlayerSpellMap& sp_list = ((Player *)m_caster)->GetSpellMap();
                    for (PlayerSpellMap::const_iterator itr = sp_list.begin(); itr != sp_list.end(); ++itr)
                    {
                        uint32 classspell = itr->first;
                        SpellEntry const *spellInfo = sSpellStore.LookupEntry(classspell);

                        if (spellInfo->SpellFamilyName == SPELLFAMILY_HUNTER && spellInfo->Id != 23989 && GetSpellRecoveryTime(spellInfo) > 0 )
                        {
                            ((Player*)m_caster)->RemoveSpellCooldown(classspell);

                            WorldPacket data(SMSG_CLEAR_COOLDOWN, (4+8));
                            data << uint32(classspell);
                            data << uint64(m_caster->GetGUID());
                            ((Player*)m_caster)->GetSession()->SendPacket(&data);
                        }
                    }
                    return;
                }
                case 37506:                                 // Scatter Shot
                {
                    if (m_caster->GetTypeId()!=TYPEID_PLAYER)
                        return;

                    // break Auto Shot and autohit
                    m_caster->InterruptSpell(CURRENT_AUTOREPEAT_SPELL);
                    m_caster->AttackStop();
                    ((Player*)m_caster)->SendAttackSwingCancelAttack();
                    return;
                }
            }
            break;
        case SPELLFAMILY_PALADIN:
            switch(m_spellInfo->SpellIconID)
            {
                case  156:                                  // Holy Shock
                {
                    if(!unitTarget)
                        return;

                    int hurt = 0;
                    int heal = 0;

                    switch(m_spellInfo->Id)
                    {
                        case 20473: hurt = 25912; heal = 25914; break;
                        case 20929: hurt = 25911; heal = 25913; break;
                        case 20930: hurt = 25902; heal = 25903; break;
                        case 27174: hurt = 27176; heal = 27175; break;
                        case 33072: hurt = 33073; heal = 33074; break;
                        case 48824: hurt = 48822; heal = 48820; break;
                        case 48825: hurt = 48823; heal = 48821; break;
                        default:
                            sLog.outError("Spell::EffectDummy: Spell %u not handled in HS",m_spellInfo->Id);
                            return;
                    }

                    if(m_caster->IsFriendlyTo(unitTarget))
                        m_caster->CastSpell(unitTarget, heal, true, 0);
                    else
                        m_caster->CastSpell(unitTarget, hurt, true, 0);

                    return;
                }
                case 561:                                   // Judgement of command
                {
                    if(!unitTarget)
                        return;

                    uint32 spell_id = m_currentBasePoints[i]+1;
                    SpellEntry const* spell_proto = sSpellStore.LookupEntry(spell_id);
                    if(!spell_proto)
                        return;

                    if( !unitTarget->hasUnitState(UNIT_STAT_STUNNED) && m_caster->GetTypeId()==TYPEID_PLAYER)
                    {
                        // decreased damage (/2) for non-stunned target.
                        SpellModifier *mod = new SpellModifier;
                        mod->op = SPELLMOD_DAMAGE;
                        mod->value = -50;
                        mod->type = SPELLMOD_PCT;
                        mod->spellId = m_spellInfo->Id;
                        mod->mask = 0x0000020000000000LL;
                        mod->mask2= 0LL;

                        ((Player*)m_caster)->AddSpellMod(mod, true);
                        m_caster->CastSpell(unitTarget,spell_proto,true,NULL);
                                                            // mod deleted
                        ((Player*)m_caster)->AddSpellMod(mod, false);
                    }
                    else
                        m_caster->CastSpell(unitTarget,spell_proto,true,NULL);

                    return;
                }
            }

            switch(m_spellInfo->Id)
            {
                // Judgement of Righteousness (0.2*$AP+0.32*$SPH) holy added in spellDamagBonus
                case 20187:
                {
                    if (!unitTarget)
                        return;
                    m_damage+=int32(0.2f*m_caster->GetTotalAttackPowerValue(BASE_ATTACK));
                    return;
                }
                case 31789:                                 // Righteous Defense (step 1)
                {
                    // 31989 -> dummy effect (step 1) + dummy effect (step 2) -> 31709 (taunt like spell for each target)

                    // non-standard cast requirement check
                    if (!unitTarget || unitTarget->getAttackers().empty())
                    {
                        // clear cooldown at fail
                        if(m_caster->GetTypeId()==TYPEID_PLAYER)
                        {
                            ((Player*)m_caster)->RemoveSpellCooldown(m_spellInfo->Id);

                            WorldPacket data(SMSG_CLEAR_COOLDOWN, (4+8));
                            data << uint32(m_spellInfo->Id);
                            data << uint64(m_caster->GetGUID());
                            ((Player*)m_caster)->GetSession()->SendPacket(&data);
                        }

                        SendCastResult(SPELL_FAILED_TARGET_AFFECTING_COMBAT);
                        return;
                    }

                    // Righteous Defense (step 2) (in old version 31980 dummy effect)
                    // Clear targets for eff 1
                    for(std::list<TargetInfo>::iterator ihit= m_UniqueTargetInfo.begin();ihit != m_UniqueTargetInfo.end();++ihit)
                        ihit->effectMask &= ~(1<<1);

                    // not empty (checked)
                    Unit::AttackerSet const& attackers = unitTarget->getAttackers();

                    // chance to be selected from list
                    float chance = 100.0f/attackers.size();
                    uint32 count=0;
                    for(Unit::AttackerSet::const_iterator aItr = attackers.begin(); aItr != attackers.end() && count < 3; ++aItr)
                    {
                        if(!roll_chance_f(chance))
                            continue;
                        ++count;
                        AddUnitTarget((*aItr), 1);
                    }

                    // now let next effect cast spell at each target.
                    return;
                }
                case 37877:                                 // Blessing of Faith
                {
                    if(!unitTarget)
                        return;

                    uint32 spell_id = 0;
                    switch(unitTarget->getClass())
                    {
                        case CLASS_DRUID:   spell_id = 37878; break;
                        case CLASS_PALADIN: spell_id = 37879; break;
                        case CLASS_PRIEST:  spell_id = 37880; break;
                        case CLASS_SHAMAN:  spell_id = 37881; break;
                        default: return;                    // ignore for not healing classes
                    }

                    m_caster->CastSpell(m_caster,spell_id,true);
                    return;
                }
            }
            break;
        case SPELLFAMILY_SHAMAN:
            //Shaman Rockbiter Weapon
            if (m_spellInfo->SpellFamilyFlags == 0x400000)
            {
                // TODO: use expect spell for enchant (if exist talent)
                // In 3.0.3 no mods present for rockbiter
                uint32 spell_id = 0;
                switch(m_spellInfo->Id)
                {
                    case  8017: spell_id = 36494; break;    // Rank 1
                    case  8018: spell_id = 36750; break;    // Rank 2
                    case  8019: spell_id = 36755; break;    // Rank 3
                    case 10399: spell_id = 36759; break;    // Rank 4
                    default:
                        sLog.outError("Spell::EffectDummy: Spell %u not handled in RW",m_spellInfo->Id);
                        return;
                }

                SpellEntry const *spellInfo = sSpellStore.LookupEntry( spell_id );

                if(!spellInfo)
                {
                    sLog.outError("WORLD: unknown spell id %i", spell_id);
                    return;
                }

                if(m_caster->GetTypeId() != TYPEID_PLAYER)
                    return;

                for(int i = BASE_ATTACK; i <= OFF_ATTACK; ++i)
                {
                    if(Item* item = ((Player*)m_caster)->GetWeaponForAttack(WeaponAttackType(i)))
                    {
                        if(item->IsFitToSpellRequirements(m_spellInfo))
                        {
                            Spell *spell = new Spell(m_caster, spellInfo, true);

                            // enchanting spell selected by calculated damage-per-sec in enchanting effect
                            // at calculation applied affect from Elemental Weapons talent
                            // real enchantment damage-1
                            spell->m_currentBasePoints[1] = damage-1;

                            SpellCastTargets targets;
                            targets.setItemTarget( item );
                            spell->prepare(&targets);
                        }
                    }
                }
                return;
            }
            // Healing Stream Totem
            if(m_spellInfo->SpellFamilyFlags & 0x0000000000002000LL)
            {
                m_caster->CastCustomSpell(unitTarget, 52042, &damage, 0, 0, true, 0, 0, m_originalCasterGUID);
                return;
            }
            // Mana Spring Totem
            if(m_spellInfo->SpellFamilyFlags & 0x0000000000004000LL)
            {
                if(unitTarget->getPowerType()!=POWER_MANA)
                    return;
                m_caster->CastCustomSpell(unitTarget, 52032, &damage, 0, 0, true, 0, 0, m_originalCasterGUID);
                return;
            }
            if(m_spellInfo->Id == 39610)                    // Mana Tide Totem effect
            {
                if(!unitTarget || unitTarget->getPowerType() != POWER_MANA)
                    return;
                // Glyph of Mana Tide
                Unit *owner = m_caster->GetOwner();
                if (owner)
                    if (Aura *dummy = owner->GetDummyAura(55441))
                        damage+=dummy->GetModifier()->m_amount;
                // Regenerate 6% of Total Mana Every 3 secs
                int32 EffectBasePoints0 = unitTarget->GetMaxPower(POWER_MANA)  * damage / 100;
                m_caster->CastCustomSpell(unitTarget,39609,&EffectBasePoints0,NULL,NULL,true,NULL,NULL,m_originalCasterGUID);
                return;
            }
            // Lava Lash
            if (m_spellInfo->SpellFamilyFlags2 & 0x00000004)
            {
                if (m_caster->GetTypeId()!=TYPEID_PLAYER)
                    return;
                Item *item = ((Player*)m_caster)->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (item)
                {
                    // Damage is increased if your off-hand weapon is enchanted with Flametongue.
                    Unit::AuraList const& auraDummy = m_caster->GetAurasByType(SPELL_AURA_DUMMY);
                    for(Unit::AuraList::const_iterator itr = auraDummy.begin(); itr != auraDummy.end(); ++itr)
                    {
                        if( (*itr)->GetSpellProto()->SpellFamilyName==SPELLFAMILY_SHAMAN &&
                            (*itr)->GetSpellProto()->SpellFamilyFlags & 0x0000000000200000LL &&
                            (*itr)->GetCastItemGUID() == item->GetGUID())
                        {
                           m_damage += m_damage * damage / 100;
                           return;
                        }
                    }
                }
                return;
            }
            break;
        case SPELLFAMILY_DEATHKNIGHT:
            // Death Coil
            if(m_spellInfo->SpellFamilyFlags & 0x002000LL)
            {
                uint32 spell_id = NULL;
				int32 bp = 0;
                damage += m_caster->GetTotalAttackPowerValue(BASE_ATTACK) * 0.15f;
                if(m_caster->IsFriendlyTo(unitTarget))
                {
                    if(unitTarget->GetCreatureType() != CREATURE_TYPE_UNDEAD)
                        return;

                    spell_id = 47633;
                    damage *= 1.5f;
                }
                else
                    spell_id = 47632;

                bp = int32(damage);
                m_caster->CastCustomSpell(unitTarget,spell_id,&bp,NULL,NULL,true);
                return;
            }
            break;
    }

    // pet auras
    if(PetAura const* petSpell = spellmgr.GetPetAura(m_spellInfo->Id))
    {
        m_caster->AddPetAura(petSpell);
        return;
    }

    // Script based implementation. Must be used only for not good for implementation in core spell effects
    // So called only for not proccessed cases
    if(gameObjTarget)
        Script->EffectDummyGameObj(m_caster, m_spellInfo->Id, i, gameObjTarget);
    else if(unitTarget && unitTarget->GetTypeId()==TYPEID_UNIT)
        Script->EffectDummyCreature(m_caster, m_spellInfo->Id, i, (Creature*)unitTarget);
    else if(itemTarget)
        Script->EffectDummyItem(m_caster, m_spellInfo->Id, i, itemTarget);
}

void Spell::EffectTriggerSpellWithValue(uint32 i)
{
    uint32 triggered_spell_id = m_spellInfo->EffectTriggerSpell[i];

    // normal case
    SpellEntry const *spellInfo = sSpellStore.LookupEntry( triggered_spell_id );

    if(!spellInfo)
    {
        sLog.outError("EffectTriggerSpellWithValue of spell %u: triggering unknown spell id %i", m_spellInfo->Id,triggered_spell_id);
        return;
    }

    int32 bp = damage;
    m_caster->CastCustomSpell(unitTarget,triggered_spell_id,&bp,&bp,&bp,true,NULL,NULL,m_originalCasterGUID);
}

void Spell::EffectTriggerRitualOfSummoning(uint32 i)
{
    uint32 triggered_spell_id = m_spellInfo->EffectTriggerSpell[i];
    SpellEntry const *spellInfo = sSpellStore.LookupEntry( triggered_spell_id );

    if(!spellInfo)
    {
        sLog.outError("EffectTriggerRitualOfSummoning of spell %u: triggering unknown spell id %i", m_spellInfo->Id,triggered_spell_id);
        return;
    }

    finish();
    Spell *spell = new Spell(m_caster, spellInfo, true);

    SpellCastTargets targets;
    targets.setUnitTarget( unitTarget);
    spell->prepare(&targets);

    m_caster->SetCurrentCastedSpell(spell);
    spell->m_selfContainer = &(m_caster->m_currentSpells[spell->GetCurrentContainer()]);

}

void Spell::EffectForceCast(uint32 i)
{
    if( !unitTarget )
        return;

    uint32 triggered_spell_id = m_spellInfo->EffectTriggerSpell[i];

    // normal case
    SpellEntry const *spellInfo = sSpellStore.LookupEntry( triggered_spell_id );

    if(!spellInfo)
    {
        sLog.outError("EffectForceCast of spell %u: triggering unknown spell id %i", m_spellInfo->Id,triggered_spell_id);
        return;
    }

    unitTarget->CastSpell(unitTarget,spellInfo,true,NULL,NULL,m_originalCasterGUID);
}

void Spell::EffectTriggerSpell(uint32 i)
{
    uint32 triggered_spell_id = m_spellInfo->EffectTriggerSpell[i];

    // special cases
    switch(triggered_spell_id)
    {
        // Vanish
        case 18461:
        {
            m_caster->RemoveSpellsCausingAura(SPELL_AURA_MOD_ROOT);
            m_caster->RemoveSpellsCausingAura(SPELL_AURA_MOD_DECREASE_SPEED);
            m_caster->RemoveSpellsCausingAura(SPELL_AURA_MOD_STALKED);

            // if this spell is given to NPC it must handle rest by it's own AI
            if ( m_caster->GetTypeId() != TYPEID_PLAYER )
                return;

            // get highest rank of the Stealth spell
            uint32 spellId = 0;
            const PlayerSpellMap& sp_list = ((Player*)m_caster)->GetSpellMap();
            for (PlayerSpellMap::const_iterator itr = sp_list.begin(); itr != sp_list.end(); ++itr)
            {
                // only highest rank is shown in spell book, so simply check if shown in spell book
                if(!itr->second->active || itr->second->disabled || itr->second->state == PLAYERSPELL_REMOVED)
                    continue;

                SpellEntry const *spellInfo = sSpellStore.LookupEntry(itr->first);
                if (!spellInfo)
                    continue;

                if (spellInfo->SpellFamilyName == SPELLFAMILY_ROGUE && spellInfo->SpellFamilyFlags & SPELLFAMILYFLAG_ROGUE_STEALTH)
                {
                    spellId = spellInfo->Id;
                    break;
                }
            }

            // no Stealth spell found
            if (!spellId)
                return;

            // reset cooldown on it if needed
            if(((Player*)m_caster)->HasSpellCooldown(spellId))
                ((Player*)m_caster)->RemoveSpellCooldown(spellId);

            m_caster->CastSpell(m_caster, spellId, true);
            return;
        }
        // just skip
        case 23770:                                         // Sayge's Dark Fortune of *
            // not exist, common cooldown can be implemented in scripts if need.
            return;
        // Brittle Armor - (need add max stack of 24575 Brittle Armor)
        case 29284:
        {
            const SpellEntry *spell = sSpellStore.LookupEntry(24575);
            if (!spell)
                return;

            for (int i=0; i < spell->StackAmount; ++i)
                m_caster->CastSpell(unitTarget,spell->Id, true, m_CastItem, NULL, m_originalCasterGUID);
            return;
        }
        // Mercurial Shield - (need add max stack of 26464 Mercurial Shield)
        case 29286:
        {
            const SpellEntry *spell = sSpellStore.LookupEntry(26464);
            if (!spell)
                return;

            for (int i=0; i < spell->StackAmount; ++i)
                m_caster->CastSpell(unitTarget,spell->Id, true, m_CastItem, NULL, m_originalCasterGUID);
            return;
        }
        // Righteous Defense
        case 31980:
        {
            m_caster->CastSpell(unitTarget, 31790, true,m_CastItem,NULL,m_originalCasterGUID);
            return;
        }
        // Cloak of Shadows
        case 35729 :
        {
            Unit::AuraMap& Auras = m_caster->GetAuras();
            for(Unit::AuraMap::iterator iter = Auras.begin(); iter != Auras.end(); ++iter)
            {
                // remove all harmful spells on you...
                if( // ignore positive and passive auras
                    !iter->second->IsPositive() && !iter->second->IsPassive() &&
                    // ignore physical auras
                    (GetSpellSchoolMask(iter->second->GetSpellProto()) & SPELL_SCHOOL_MASK_NORMAL)==0 )
                {
                    m_caster->RemoveAurasDueToSpell(iter->second->GetSpellProto()->Id);
                    iter = Auras.begin();
                }
            }
            return;
        }
        // Priest Shadowfiend (34433) need apply mana gain trigger aura on pet
        case 41967:
        {
            if (Unit *pet = m_caster->GetPet())
                pet->CastSpell(pet, 28305, true);
            return;
        }
    }

    // normal case
    SpellEntry const *spellInfo = sSpellStore.LookupEntry( triggered_spell_id );

    if(!spellInfo)
    {
        sLog.outError("EffectTriggerSpell of spell %u: triggering unknown spell id %i", m_spellInfo->Id,triggered_spell_id);
        return;
    }

    // some triggered spells require specific equipment
    if(spellInfo->EquippedItemClass >=0 && m_caster->GetTypeId()==TYPEID_PLAYER)
    {
        // main hand weapon required
        if(spellInfo->AttributesEx3 & SPELL_ATTR_EX3_MAIN_HAND)
        {
            Item* item = ((Player*)m_caster)->GetWeaponForAttack(BASE_ATTACK);

            // skip spell if no weapon in slot or broken
            if(!item || item->IsBroken() )
                return;

            // skip spell if weapon not fit to triggered spell
            if(!item->IsFitToSpellRequirements(spellInfo))
                return;
        }

        // offhand hand weapon required
        if(spellInfo->AttributesEx3 & SPELL_ATTR_EX3_REQ_OFFHAND)
        {
            Item* item = ((Player*)m_caster)->GetWeaponForAttack(OFF_ATTACK);

            // skip spell if no weapon in slot or broken
            if(!item || item->IsBroken() )
                return;

            // skip spell if weapon not fit to triggered spell
            if(!item->IsFitToSpellRequirements(spellInfo))
                return;
        }
    }

    // some triggered spells must be casted instantly (for example, if next effect case instant kill caster)
    bool instant = false;
    for(uint32 j = i+1; j < 3; ++j)
    {
        if(m_spellInfo->Effect[j]==SPELL_EFFECT_INSTAKILL && m_spellInfo->EffectImplicitTargetA[j]==TARGET_SELF)
        {
            instant = true;
            break;
        }
    }

    if(instant)
    {
        if (unitTarget)
            m_caster->CastSpell(unitTarget,spellInfo,true,m_CastItem,NULL,m_originalCasterGUID);
    }
    else
        m_TriggerSpells.push_back(spellInfo);
}

void Spell::EffectTriggerMissileSpell(uint32 effect_idx)
{
    uint32 triggered_spell_id = m_spellInfo->EffectTriggerSpell[effect_idx];

    // normal case
    SpellEntry const *spellInfo = sSpellStore.LookupEntry( triggered_spell_id );

    if(!spellInfo)
    {
        sLog.outError("EffectTriggerMissileSpell of spell %u (eff: %u): triggering unknown spell id %u",
            m_spellInfo->Id,effect_idx,triggered_spell_id);
        return;
    }

    if (m_CastItem)
        DEBUG_LOG("WORLD: cast Item spellId - %i", spellInfo->Id);

    m_caster->CastSpell(m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, spellInfo, true, m_CastItem, 0, m_originalCasterGUID);
}

void Spell::EffectJump(uint32 i)
{
    if(m_caster->isInFlight())
        return;

    // Init dest coordinates
    float x,y,z,o;
    if(m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION)
    {
        x = m_targets.m_destX;
        y = m_targets.m_destY;
        z = m_targets.m_destZ;

        if(m_spellInfo->EffectImplicitTargetA[i] == TARGET_BEHIND_VICTIM)
        {
            // explicit cast data from client or server-side cast
            // some spell at client send caster
            Unit* pTarget = NULL;
            if(m_targets.getUnitTarget() && m_targets.getUnitTarget()!=m_caster)
                pTarget = m_targets.getUnitTarget();
            else if(unitTarget->getVictim())
                pTarget = m_caster->getVictim();
            else if(m_caster->GetTypeId() == TYPEID_PLAYER)
                pTarget = ObjectAccessor::GetUnit(*m_caster, ((Player*)m_caster)->GetSelection());

            o = pTarget ? pTarget->GetOrientation() : m_caster->GetOrientation();
        }
        else
            o = m_caster->GetOrientation();
    }
    else if(unitTarget)
    {
        unitTarget->GetContactPoint(m_caster,x,y,z,CONTACT_DISTANCE);
        o = m_caster->GetOrientation();
    }
    else if(gameObjTarget)
    {
        gameObjTarget->GetContactPoint(m_caster,x,y,z,CONTACT_DISTANCE);
        o = m_caster->GetOrientation();
    }
    else
    {
        sLog.outError( "Spell::EffectJump - unsupported target mode for spell ID %u", m_spellInfo->Id );
        return;
    }

    m_caster->NearTeleportTo(x,y,z,o,true);
}

void Spell::EffectTeleportUnits(uint32 i)
{
    if(!unitTarget || unitTarget->isInFlight())
        return;

    switch (m_spellInfo->EffectImplicitTargetB[i])
    {
        case TARGET_INNKEEPER_COORDINATES:
        {
            // Only players can teleport to innkeeper
            if (unitTarget->GetTypeId() != TYPEID_PLAYER)
                return;

            ((Player*)unitTarget)->TeleportTo(((Player*)unitTarget)->m_homebindMapId,((Player*)unitTarget)->m_homebindX,((Player*)unitTarget)->m_homebindY,((Player*)unitTarget)->m_homebindZ,unitTarget->GetOrientation(),unitTarget==m_caster ? TELE_TO_SPELL : 0);
            return;
        }
        case TARGET_TABLE_X_Y_Z_COORDINATES:
        {
            SpellTargetPosition const* st = spellmgr.GetSpellTargetPosition(m_spellInfo->Id);
            if(!st)
            {
                sLog.outError( "Spell::EffectTeleportUnits - unknown Teleport coordinates for spell ID %u", m_spellInfo->Id );
                return;
            }

            if(st->target_mapId==unitTarget->GetMapId())
                unitTarget->NearTeleportTo(st->target_X,st->target_Y,st->target_Z,st->target_Orientation,unitTarget==m_caster);
            else if(unitTarget->GetTypeId()==TYPEID_PLAYER)
                ((Player*)unitTarget)->TeleportTo(st->target_mapId,st->target_X,st->target_Y,st->target_Z,st->target_Orientation,unitTarget==m_caster ? TELE_TO_SPELL : 0);
            break;
        }
        case TARGET_BEHIND_VICTIM:
        {
            Unit *pTarget = NULL;

            // explicit cast data from client or server-side cast
            // some spell at client send caster
            if(m_targets.getUnitTarget() && m_targets.getUnitTarget()!=unitTarget)
                pTarget = m_targets.getUnitTarget();
            else if(unitTarget->getVictim())
                pTarget = unitTarget->getVictim();
            else if(unitTarget->GetTypeId() == TYPEID_PLAYER)
                pTarget = ObjectAccessor::GetUnit(*unitTarget, ((Player*)unitTarget)->GetSelection());

            // Init dest coordinates
            float x = m_targets.m_destX;
            float y = m_targets.m_destY;
            float z = m_targets.m_destZ;
            float orientation = pTarget ? pTarget->GetOrientation() : unitTarget->GetOrientation();
            unitTarget->NearTeleportTo(x,y,z,orientation,unitTarget==m_caster);
            return;
        }
        default:
        {
            // If not exist data for dest location - return
            if(!(m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION))
            {
                sLog.outError( "Spell::EffectTeleportUnits - unknown EffectImplicitTargetB[%u] = %u for spell ID %u", i, m_spellInfo->EffectImplicitTargetB[i], m_spellInfo->Id );
                return;
            }
            // Init dest coordinates
            uint32 mapid = m_caster->GetMapId();
            float x = m_targets.m_destX;
            float y = m_targets.m_destY;
            float z = m_targets.m_destZ;
            float orientation = unitTarget->GetOrientation();
            // Teleport
            unitTarget->NearTeleportTo(x,y,z,orientation,unitTarget==m_caster);
            return;
        }
    }

    // post effects for TARGET_TABLE_X_Y_Z_COORDINATES
    switch ( m_spellInfo->Id )
    {
        // Dimensional Ripper - Everlook
        case 23442:
        {
            int32 r = irand(0, 119);
            if ( r >= 70 )                                  // 7/12 success
            {
                if ( r < 100 )                              // 4/12 evil twin
                    m_caster->CastSpell(m_caster,23445,true);
                else                                        // 1/12 fire
                    m_caster->CastSpell(m_caster,23449,true);
            }
            return;
        }
        // Ultrasafe Transporter: Toshley's Station
        case 36941:
        {
            if ( roll_chance_i(50) )                        // 50% success
            {
                int32 rand_eff = urand(1,7);
                switch ( rand_eff )
                {
                    case 1:
                        // soul split - evil
                        m_caster->CastSpell(m_caster,36900,true);
                        break;
                    case 2:
                        // soul split - good
                        m_caster->CastSpell(m_caster,36901,true);
                        break;
                    case 3:
                        // Increase the size
                        m_caster->CastSpell(m_caster,36895,true);
                        break;
                    case 4:
                        // Decrease the size
                        m_caster->CastSpell(m_caster,36893,true);
                        break;
                    case 5:
                    // Transform
                    {
                        if (((Player*)m_caster)->GetTeam() == ALLIANCE )
                            m_caster->CastSpell(m_caster,36897,true);
                        else
                            m_caster->CastSpell(m_caster,36899,true);
                        break;
                    }
                    case 6:
                        // chicken
                        m_caster->CastSpell(m_caster,36940,true);
                        break;
                    case 7:
                        // evil twin
                        m_caster->CastSpell(m_caster,23445,true);
                        break;
                }
            }
            return;
        }
        // Dimensional Ripper - Area 52
        case 36890:
        {
            if ( roll_chance_i(50) )                        // 50% success
            {
                int32 rand_eff = urand(1,4);
                switch ( rand_eff )
                {
                    case 1:
                        // soul split - evil
                        m_caster->CastSpell(m_caster,36900,true);
                        break;
                    case 2:
                        // soul split - good
                        m_caster->CastSpell(m_caster,36901,true);
                        break;
                    case 3:
                        // Increase the size
                        m_caster->CastSpell(m_caster,36895,true);
                        break;
                    case 4:
                    // Transform
                    {
                        if (((Player*)m_caster)->GetTeam() == ALLIANCE )
                            m_caster->CastSpell(m_caster,36897,true);
                        else
                            m_caster->CastSpell(m_caster,36899,true);
                        break;
                    }
                }
            }
            return;
        }
    }
}

void Spell::EffectApplyAura(uint32 i)
{
    if( m_spellInfo->Id == 49206)//summon gargoyle
        EffectSpecialSummon( 49206, 0, m_caster );

    if(!unitTarget)
        return;

    // ghost spell check, allow apply any auras at player loading in ghost mode (will be cleanup after load)
    if( !unitTarget->isAlive() && m_spellInfo->Id != 20584 && m_spellInfo->Id != 8326 &&
        (unitTarget->GetTypeId()!=TYPEID_PLAYER || !((Player*)unitTarget)->GetSession()->PlayerLoading()) )
        return;

    Unit* caster = m_originalCaster ? m_originalCaster : m_caster;
    if(!caster)
        return;

    sLog.outDebug("Spell: Aura is: %u", m_spellInfo->EffectApplyAuraName[i]);

    Aura* Aur = CreateAura(m_spellInfo, i, &m_currentBasePoints[i], unitTarget, caster, m_CastItem);

    // Now Reduce spell duration using data received at spell hit
    int32 duration = Aur->GetAuraMaxDuration();
    unitTarget->ApplyDiminishingToDuration(m_diminishGroup,duration,m_caster,m_diminishLevel);
    Aur->setDiminishGroup(m_diminishGroup);

    // if Aura removed and deleted, do not continue.
    if(duration== 0 && !(Aur->IsPermanent()))
    {
        delete Aur;
        return;
    }

    if(duration != Aur->GetAuraMaxDuration())
    {
        Aur->SetAuraMaxDuration(duration);
        Aur->SetAuraDuration(duration);
    }

    bool added = unitTarget->AddAura(Aur);

    // Aura not added and deleted in AddAura call;
    if (!added)
        return;

    // found crash at character loading, broken pointer to Aur...
    // Aur was deleted in AddAura()...
    if(!Aur)
        return;

    // Prayer of Mending (jump animation), we need formal caster instead original for correct animation
    if( m_spellInfo->SpellFamilyName == SPELLFAMILY_PRIEST && (m_spellInfo->SpellFamilyFlags & 0x00002000000000LL))
        m_caster->CastSpell(unitTarget, 41637, true, NULL, Aur, m_originalCasterGUID);
}

void Spell::EffectUnlearnSpecialization( uint32 i )
{
    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player *_player = (Player*)unitTarget;
    uint32 spellToUnlearn = m_spellInfo->EffectTriggerSpell[i];

    _player->removeSpell(spellToUnlearn);

    sLog.outDebug( "Spell: Player %u have unlearned spell %u from NpcGUID: %u", _player->GetGUIDLow(), spellToUnlearn, m_caster->GetGUIDLow() );
}

void Spell::EffectPowerDrain(uint32 i)
{
    if(m_spellInfo->EffectMiscValue[i] < 0 || m_spellInfo->EffectMiscValue[i] >= MAX_POWERS)
        return;

    Powers drain_power = Powers(m_spellInfo->EffectMiscValue[i]);
	Powers powertype = Powers(m_spellInfo->EffectMiscValue[i]);

    if(!unitTarget)
        return;
    if(!unitTarget->isAlive())
        return;
    if(unitTarget->getPowerType() != drain_power)
        return;
    if(damage < 0)
        return;

    uint32 curPower = unitTarget->GetPower(drain_power);

    //add spell damage bonus
    damage=m_caster->SpellDamageBonus(unitTarget,m_spellInfo,uint32(damage),SPELL_DIRECT_DAMAGE);

    uint32 power = damage;

    SkillLineAbilityMap::const_iterator const skillLine = spellmgr.GetBeginSkillLineAbilityMap(m_spellInfo->Id);
    if(skillLine->second->skillId == SKILL_DISCIPLINE)
    {
       power = unitTarget->GetMaxPower(powertype) * damage /100;

       if(power > GetCaster()->GetMaxPower(powertype) * damage / 50)
          power = GetCaster()->GetMaxPower(powertype) * damage / 50;
    }

    // resilience reduce mana draining effect at spell crit damage reduction (added in 2.4)
    if ( drain_power == POWER_MANA && unitTarget->GetTypeId() == TYPEID_PLAYER )
        power -= ((Player*)unitTarget)->GetSpellCritDamageReduction(power);

    int32 new_damage;
    if(curPower < power)
        new_damage = curPower;
    else
        new_damage = power;

    unitTarget->ModifyPower(drain_power,-new_damage);

    // Don`t restore from self drain
    if(drain_power == POWER_MANA && m_caster != unitTarget)
    {
        float manaMultiplier = m_spellInfo->EffectMultipleValue[i];
        if(manaMultiplier==0)
            manaMultiplier = 1;

        if(Player *modOwner = m_caster->GetSpellModOwner())
            modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_MULTIPLE_VALUE, manaMultiplier);

        int32 gain = int32(new_damage*manaMultiplier);

        m_caster->ModifyPower(POWER_MANA,gain);
        //send log
        m_caster->SendEnergizeSpellLog(m_caster, m_spellInfo->Id,gain,POWER_MANA);
    }
}

void Spell::EffectSendEvent(uint32 EffectIndex)
{
    if (m_caster->GetTypeId() == TYPEID_PLAYER && ((Player*)m_caster)->InBattleGround())
    {
        BattleGround* bg = ((Player *)m_caster)->GetBattleGround();
        if(bg && bg->GetStatus() == STATUS_IN_PROGRESS)
        {
            switch(m_spellInfo->Id)
            {
                case 23333:                                 // Pickup Horde Flag
                    /*do not uncomment .
                    if(bg->GetTypeID()==BATTLEGROUND_WS)
                        bg->EventPlayerClickedOnFlag((Player*)m_caster, gameObjTarget);
                    sLog.outDebug("Send Event Horde Flag Picked Up");
                    break;
                    /* not used :
                    case 23334:                                 // Drop Horde Flag
                        if(bg->GetTypeID()==BATTLEGROUND_WS)
                            bg->EventPlayerDroppedFlag((Player*)m_caster);
                        sLog.outDebug("Drop Horde Flag");
                        break;
                    */
                case 23335:                                 // Pickup Alliance Flag
                    /*do not uncomment ... (it will cause crash, because of null targetobject!) anyway this is a bad way to call that event, because it would cause recursion
                    if(bg->GetTypeID()==BATTLEGROUND_WS)
                        bg->EventPlayerClickedOnFlag((Player*)m_caster, gameObjTarget);
                    sLog.outDebug("Send Event Alliance Flag Picked Up");
                    break;
                    /* not used :
                    case 23336:                                 // Drop Alliance Flag
                        if(bg->GetTypeID()==BATTLEGROUND_WS)
                            bg->EventPlayerDroppedFlag((Player*)m_caster);
                        sLog.outDebug("Drop Alliance Flag");
                        break;
                    case 23385:                                 // Alliance Flag Returns
                        if(bg->GetTypeID()==BATTLEGROUND_WS)
                            bg->EventPlayerClickedOnFlag((Player*)m_caster, gameObjTarget);
                        sLog.outDebug("Alliance Flag Returned");
                        break;
                    case 23386:                                   // Horde Flag Returns
                        if(bg->GetTypeID()==BATTLEGROUND_WS)
                            bg->EventPlayerClickedOnFlag((Player*)m_caster, gameObjTarget);
                        sLog.outDebug("Horde Flag Returned");
                        break;*/
                case 34976:
                    /*
                    if(bg->GetTypeID()==BATTLEGROUND_EY)
                        bg->EventPlayerClickedOnFlag((Player*)m_caster, gameObjTarget);
                    */
                    break;
                default:
                    sLog.outDebug("Unknown spellid %u in BG event", m_spellInfo->Id);
                    break;
            }
        }
    }
    sLog.outDebug("Spell ScriptStart %u for spellid %u in EffectSendEvent ", m_spellInfo->EffectMiscValue[EffectIndex], m_spellInfo->Id);
    sWorld.ScriptsStart(sEventScripts, m_spellInfo->EffectMiscValue[EffectIndex], m_caster, focusObject);
}

void Spell::EffectPowerBurn(uint32 i)
{
    if(m_spellInfo->EffectMiscValue[i] < 0 || m_spellInfo->EffectMiscValue[i] >= MAX_POWERS)
        return;

    Powers powertype = Powers(m_spellInfo->EffectMiscValue[i]);

    if(!unitTarget)
        return;
    if(!unitTarget->isAlive())
        return;
    if(unitTarget->getPowerType()!=powertype)
        return;
    if(damage < 0)
        return;

    int32 curPower = int32(unitTarget->GetPower(powertype));

    // resilience reduce mana draining effect at spell crit damage reduction (added in 2.4)
    uint32 power = damage;
    if ( powertype == POWER_MANA && unitTarget->GetTypeId() == TYPEID_PLAYER )
        power -= ((Player*)unitTarget)->GetSpellCritDamageReduction(power);

    int32 new_damage = (curPower < power) ? curPower : power;

    unitTarget->ModifyPower(powertype,-new_damage);
    float multiplier = m_spellInfo->EffectMultipleValue[i];

    if(Player *modOwner = m_caster->GetSpellModOwner())
        modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_MULTIPLE_VALUE, multiplier);

    new_damage = int32(new_damage*multiplier);
    m_damage+=new_damage;
}

void Spell::EffectHeal( uint32 /*i*/ )
{
    if( unitTarget && unitTarget->isAlive() && damage >= 0)
    {
        // Try to get original caster
        Unit *caster = m_originalCasterGUID ? m_originalCaster : m_caster;

        // Skip if m_originalCaster not available
        if (!caster)
            return;

        int32 addhealth = damage;

        // Vessel of the Naaru (Vial of the Sunwell trinket)
        if (m_spellInfo->Id == 45064)
        {
            // Amount of heal - depends from stacked Holy Energy
            int damageAmount = 0;
            Unit::AuraList const& mDummyAuras = m_caster->GetAurasByType(SPELL_AURA_DUMMY);
            for(Unit::AuraList::const_iterator i = mDummyAuras.begin();i != mDummyAuras.end(); ++i)
                if((*i)->GetId() == 45062)
                    damageAmount+=(*i)->GetModifier()->m_amount;
            if (damageAmount)
                m_caster->RemoveAurasDueToSpell(45062);

            addhealth += damageAmount;
        }
        // Swiftmend - consumes Regrowth or Rejuvenation
        else if (m_spellInfo->TargetAuraState == AURA_STATE_SWIFTMEND && unitTarget->HasAuraState(AURA_STATE_SWIFTMEND))
        {
            Unit::AuraList const& RejorRegr = unitTarget->GetAurasByType(SPELL_AURA_PERIODIC_HEAL);
            // find most short by duration
            Aura *targetAura = NULL;
            for(Unit::AuraList::const_iterator i = RejorRegr.begin(); i != RejorRegr.end(); ++i)
            {
                if((*i)->GetSpellProto()->SpellFamilyName == SPELLFAMILY_DRUID
                    && ((*i)->GetSpellProto()->SpellFamilyFlags == 0x40 || (*i)->GetSpellProto()->SpellFamilyFlags == 0x10) )
                {
                    if(!targetAura || (*i)->GetAuraDuration() < targetAura->GetAuraDuration())
                        targetAura = *i;
                }
            }

            if(!targetAura)
            {
                sLog.outError("Target(GUID:" I64FMTD ") has aurastate AURA_STATE_SWIFTMEND but no matching aura.", unitTarget->GetGUID());
                return;
            }
            int idx = 0;
            while(idx < 3)
            {
                if(targetAura->GetSpellProto()->EffectApplyAuraName[idx] == SPELL_AURA_PERIODIC_HEAL)
                    break;
                idx++;
            }

            int32 tickheal = caster->SpellHealingBonus(unitTarget, targetAura->GetSpellProto(), targetAura->GetModifier()->m_amount, DOT);
            int32 tickcount = GetSpellDuration(targetAura->GetSpellProto()) / targetAura->GetSpellProto()->EffectAmplitude[idx];
            
            //Glyph of Swiftmend-Dummy
            if(!caster->HasAura(54824))
                unitTarget->RemoveAurasDueToSpell(targetAura->GetId());

            addhealth += tickheal * tickcount;
        }
        else
            addhealth = caster->SpellHealingBonus(unitTarget, m_spellInfo, addhealth, HEAL);

        m_healing+=addhealth;
    }
}

void Spell::EffectHealPct( uint32 /*i*/ )
{
    if( unitTarget && unitTarget->isAlive() && damage >= 0)
    {
        // Try to get original caster
        Unit *caster = m_originalCasterGUID ? m_originalCaster : m_caster;

        // Skip if m_originalCaster not available
        if (!caster)
            return;

        uint32 addhealth = unitTarget->GetMaxHealth() * damage / 100;
        caster->SendHealSpellLog(unitTarget, m_spellInfo->Id, addhealth, false);

        int32 gain = unitTarget->ModifyHealth( int32(addhealth) );
        unitTarget->getHostilRefManager().threatAssist(m_caster, float(gain) * 0.5f, m_spellInfo);

        if(caster->GetTypeId()==TYPEID_PLAYER)
            if(BattleGround *bg = ((Player*)caster)->GetBattleGround())
                bg->UpdatePlayerScore(((Player*)caster), SCORE_HEALING_DONE, gain);
    }
}

void Spell::EffectHealMechanical( uint32 /*i*/ )
{
    // Mechanic creature type should be correctly checked by targetCreatureType field
    if( unitTarget && unitTarget->isAlive() && damage >= 0)
    {
        // Try to get original caster
        Unit *caster = m_originalCasterGUID ? m_originalCaster : m_caster;

        // Skip if m_originalCaster not available
        if (!caster)
            return;

        uint32 addhealth = caster->SpellHealingBonus(unitTarget, m_spellInfo, uint32(damage), HEAL);
        caster->SendHealSpellLog(unitTarget, m_spellInfo->Id, addhealth, false);
        unitTarget->ModifyHealth( int32(damage) );
    }
}

void Spell::EffectHealthLeech(uint32 i)
{
    if(!unitTarget)
        return;
    if(!unitTarget->isAlive())
        return;

    if(damage < 0)
        return;

    sLog.outDebug("HealthLeech :%i", damage);

    float multiplier = m_spellInfo->EffectMultipleValue[i];

    if(Player *modOwner = m_caster->GetSpellModOwner())
        modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_MULTIPLE_VALUE, multiplier);

    int32 new_damage = int32(damage*multiplier);
    uint32 curHealth = unitTarget->GetHealth();
    new_damage = m_caster->SpellNonMeleeDamageLog(unitTarget, m_spellInfo->Id, new_damage, m_IsTriggeredSpell, true);
    if(curHealth < new_damage)
        new_damage = curHealth;

    if(m_caster->isAlive())
    {
        new_damage = m_caster->SpellHealingBonus(m_caster, m_spellInfo, new_damage, HEAL);

        m_caster->ModifyHealth(new_damage);

        if(m_caster->GetTypeId() == TYPEID_PLAYER)
            m_caster->SendHealSpellLog(m_caster, m_spellInfo->Id, uint32(new_damage));
    }
//    m_healthLeech+=tmpvalue;
//    m_damage+=new_damage;
}

void Spell::DoCreateItem(uint32 i, uint32 itemtype)
{
    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* player = (Player*)unitTarget;

    uint32 newitemid = itemtype;
    ItemPrototype const *pProto = objmgr.GetItemPrototype( newitemid );
    if(!pProto)
    {
        player->SendEquipError( EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL );
        return;
    }

    uint32 num_to_add;

    // TODO: maybe all this can be replaced by using correct calculated `damage` value
    if(pProto->Class != ITEM_CLASS_CONSUMABLE || m_spellInfo->SpellFamilyName != SPELLFAMILY_MAGE)
    {
        int32 basePoints = m_currentBasePoints[i];
        int32 randomPoints = m_spellInfo->EffectDieSides[i];
        if (randomPoints)
            num_to_add = basePoints + irand(1, randomPoints);
        else
            num_to_add = basePoints + 1;
    }
    else if (pProto->MaxCount == 1)
        num_to_add = 1;
    else if(player->getLevel() >= m_spellInfo->spellLevel)
    {
        int32 basePoints = m_currentBasePoints[i];
        float pointPerLevel = m_spellInfo->EffectRealPointsPerLevel[i];
        num_to_add = basePoints + 1 + uint32((player->getLevel() - m_spellInfo->spellLevel)*pointPerLevel);
    }
    else
        num_to_add = 2;

    if (num_to_add < 1)
        num_to_add = 1;
    if (num_to_add > pProto->GetMaxStackSize())
        num_to_add = pProto->GetMaxStackSize();

    // init items_count to 1, since 1 item will be created regardless of specialization
    int items_count=1;
    // the chance to create additional items
    float additionalCreateChance=0.0f;
    // the maximum number of created additional items
    uint8 additionalMaxNum=0;
    // get the chance and maximum number for creating extra items
    if ( canCreateExtraItems(player, m_spellInfo->Id, additionalCreateChance, additionalMaxNum) )
    {
        // roll with this chance till we roll not to create or we create the max num
        while ( roll_chance_f(additionalCreateChance) && items_count<=additionalMaxNum )
            ++items_count;
    }

    // really will be created more items
    num_to_add *= items_count;

    // can the player store the new item?
    ItemPosCountVec dest;
    uint32 no_space = 0;
    uint8 msg = player->CanStoreNewItem( NULL_BAG, NULL_SLOT, dest, newitemid, num_to_add, &no_space );
    if( msg != EQUIP_ERR_OK )
    {
        // convert to possible store amount
        if( msg == EQUIP_ERR_INVENTORY_FULL || msg == EQUIP_ERR_CANT_CARRY_MORE_OF_THIS )
            num_to_add -= no_space;
        else
        {
            // if not created by another reason from full inventory or unique items amount limitation
            player->SendEquipError( msg, NULL, NULL );
            return;
        }
    }

    if(num_to_add)
    {
        // create the new item and store it
        Item* pItem = player->StoreNewItem( dest, newitemid, true, Item::GenerateItemRandomPropertyId(newitemid));

        // was it successful? return error if not
        if(!pItem)
        {
            player->SendEquipError( EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL );
            return;
        }

        // set the "Crafted by ..." property of the item
        if( pItem->GetProto()->Class != ITEM_CLASS_CONSUMABLE && pItem->GetProto()->Class != ITEM_CLASS_QUEST)
            pItem->SetUInt32Value(ITEM_FIELD_CREATOR,player->GetGUIDLow());

        // send info to the client
        if(pItem)
            player->SendNewItem(pItem, num_to_add, true, true);

        // we succeeded in creating at least one item, so a levelup is possible
        player->UpdateCraftSkill(m_spellInfo->Id);
    }

    // for battleground marks send by mail if not add all expected
    if(no_space > 0 )
    {
        BattleGroundTypeId bgType;
        switch(m_spellInfo->Id)
        {
            case SPELL_AV_MARK_WINNER:
            case SPELL_AV_MARK_LOSER:
                bgType = BATTLEGROUND_AV;
                break;
            case SPELL_WS_MARK_WINNER:
            case SPELL_WS_MARK_LOSER:
                bgType = BATTLEGROUND_WS;
                break;
            case SPELL_AB_MARK_WINNER:
            case SPELL_AB_MARK_LOSER:
                bgType = BATTLEGROUND_AB;
                break;
            default:
                return;
        }

        if(BattleGround* bg = sBattleGroundMgr.GetBattleGroundTemplate(bgType))
            bg->SendRewardMarkByMail(player,newitemid,no_space);
    }
}

void Spell::EffectCreateItem(uint32 i)
{
    DoCreateItem(i,m_spellInfo->EffectItemType[i]);
}

void Spell::EffectCreateItem2(uint32 i)
{
    // special case: generate using spell_loot_template
    if(!m_spellInfo->EffectItemType[i])
    {
        if(m_caster->GetTypeId()!=TYPEID_PLAYER)
            return;

        // create some random items
        ((Player*)m_caster)->AutoStoreLoot(m_spellInfo->Id,LootTemplates_Spell);
        return;
    }
    DoCreateItem(i,m_spellInfo->EffectItemType[i]);
}

void Spell::EffectPersistentAA(uint32 i)
{
    float radius = GetSpellRadius(sSpellRadiusStore.LookupEntry(m_spellInfo->EffectRadiusIndex[i]));

    if(Player* modOwner = m_caster->GetSpellModOwner())
        modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_RADIUS, radius);

    int32 duration = GetSpellDuration(m_spellInfo);
    DynamicObject* dynObj = new DynamicObject;
    if(!dynObj->Create(objmgr.GenerateLowGuid(HIGHGUID_DYNAMICOBJECT), m_caster, m_spellInfo->Id, i, m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, duration, radius))
    {
        delete dynObj;
        return;
    }
    dynObj->SetUInt32Value(OBJECT_FIELD_TYPE, 65);
    dynObj->SetUInt32Value(GAMEOBJECT_DISPLAYID, 368003);
    dynObj->SetUInt32Value(DYNAMICOBJECT_BYTES, 0x01eeeeee);
    m_caster->AddDynObject(dynObj);
    dynObj->GetMap()->Add(dynObj);
}

void Spell::EffectEnergize(uint32 i)
{
    if(!unitTarget)
        return;
    if(!unitTarget->isAlive())
        return;

    if(m_spellInfo->EffectMiscValue[i] < 0 || m_spellInfo->EffectMiscValue[i] >= MAX_POWERS)
        return;

    Powers power = Powers(m_spellInfo->EffectMiscValue[i]);

    // Some level depends spells
    int multiplier = 0;
    int level_diff = 0;
    switch (m_spellInfo->Id)
    {
        // Restore Energy
        case 9512:
            level_diff = m_caster->getLevel() - 40;
            multiplier = 2;
            break;
        // Blood Fury
        case 24571:
            level_diff = m_caster->getLevel() - 60;
            multiplier = 10;
            break;
        // Burst of Energy
        case 24532:
            level_diff = m_caster->getLevel() - 60;
            multiplier = 4;
            break;
        default:
            break;
    }

    if (level_diff > 0)
        damage -= multiplier * level_diff;

    if(damage < 0)
        return;

    if(unitTarget->GetMaxPower(power) == 0)
        return;

    unitTarget->ModifyPower(power,damage);
    m_caster->SendEnergizeSpellLog(unitTarget, m_spellInfo->Id, damage, power);

    // Mad Alchemist's Potion
    if (m_spellInfo->Id == 45051)
    {
        // find elixirs on target
        uint32 elixir_mask = 0;
        Unit::AuraMap& Auras = unitTarget->GetAuras();
        for(Unit::AuraMap::iterator itr = Auras.begin(); itr != Auras.end(); ++itr)
        {
            uint32 spell_id = itr->second->GetId();
            if(uint32 mask = spellmgr.GetSpellElixirMask(spell_id))
                elixir_mask |= mask;
        }

        // get available elixir mask any not active type from battle/guardian (and flask if no any)
        elixir_mask = (elixir_mask & ELIXIR_FLASK_MASK) ^ ELIXIR_FLASK_MASK;

        // get all available elixirs by mask and spell level
        std::vector<uint32> elixirs;
        SpellElixirMap const& m_spellElixirs = spellmgr.GetSpellElixirMap();
        for(SpellElixirMap::const_iterator itr = m_spellElixirs.begin(); itr != m_spellElixirs.end(); ++itr)
        {
            if (itr->second & elixir_mask)
            {
                if (itr->second & (ELIXIR_UNSTABLE_MASK | ELIXIR_SHATTRATH_MASK))
                    continue;

                SpellEntry const *spellInfo = sSpellStore.LookupEntry(itr->first);
                if (spellInfo && (spellInfo->spellLevel < m_spellInfo->spellLevel || spellInfo->spellLevel > unitTarget->getLevel()))
                    continue;

                elixirs.push_back(itr->first);
            }
        }

        if (!elixirs.empty())
        {
            // cast random elixir on target
            uint32 rand_spell = urand(0,elixirs.size()-1);
            m_caster->CastSpell(unitTarget,elixirs[rand_spell],true,m_CastItem);
        }
    }
}

void Spell::EffectEnergisePct(uint32 i)
{
    if(!unitTarget)
        return;
    if(!unitTarget->isAlive())
        return;

    if(m_spellInfo->EffectMiscValue[i] < 0 || m_spellInfo->EffectMiscValue[i] >= MAX_POWERS)
        return;

    Powers power = Powers(m_spellInfo->EffectMiscValue[i]);

    uint32 maxPower = unitTarget->GetMaxPower(power);
    if(maxPower == 0)
        return;

    uint32 gain = damage * maxPower / 100;
    unitTarget->ModifyPower(power, gain);
    m_caster->SendEnergizeSpellLog(unitTarget, m_spellInfo->Id, gain, power);
}

void Spell::SendLoot(uint64 guid, LootType loottype)
{
    Player* player = (Player*)m_caster;
    if (!player)
        return;

    if (gameObjTarget)
    {
        if (Script->GOHello(player, gameObjTarget))
            return;

        switch (gameObjTarget->GetGoType())
        {
            case GAMEOBJECT_TYPE_DOOR:
            case GAMEOBJECT_TYPE_BUTTON:
                gameObjTarget->UseDoorOrButton();
                sWorld.ScriptsStart(sGameObjectScripts, gameObjTarget->GetDBTableGUIDLow(), player, gameObjTarget);
                return;

            case GAMEOBJECT_TYPE_QUESTGIVER:
                // start or end quest
                player->PrepareQuestMenu(guid);
                player->SendPreparedQuest(guid);
                return;

            case GAMEOBJECT_TYPE_SPELL_FOCUS:
                // triggering linked GO
                if(uint32 trapEntry = gameObjTarget->GetGOInfo()->spellFocus.linkedTrapId)
                    gameObjTarget->TriggeringLinkedGameObject(trapEntry,m_caster);
                return;

            case GAMEOBJECT_TYPE_GOOBER:
                // goober_scripts can be triggered if the player don't have the quest
                if (gameObjTarget->GetGOInfo()->goober.eventId)
                {
                    sLog.outDebug("Goober ScriptStart id %u for GO %u", gameObjTarget->GetGOInfo()->goober.eventId,gameObjTarget->GetDBTableGUIDLow());
                    sWorld.ScriptsStart(sEventScripts, gameObjTarget->GetGOInfo()->goober.eventId, player, gameObjTarget);
                }

                // cast goober spell
                if (gameObjTarget->GetGOInfo()->goober.questId)
                    ///Quest require to be active for GO using
                    if(player->GetQuestStatus(gameObjTarget->GetGOInfo()->goober.questId) != QUEST_STATUS_INCOMPLETE)
                        return;

                gameObjTarget->AddUniqueUse(player);
                gameObjTarget->SetLootState(GO_JUST_DEACTIVATED);

                //TODO? Objective counting called without spell check but with quest objective check
                // if send spell id then this line will duplicate to spell casting call (double counting)
                // So we or have this line and not required in quest_template have reqSpellIdN
                // or must remove this line and required in DB have data in quest_template have reqSpellIdN for all quest using cases.
                player->CastedCreatureOrGO(gameObjTarget->GetEntry(), gameObjTarget->GetGUID(), 0);

                // triggering linked GO
                if(uint32 trapEntry = gameObjTarget->GetGOInfo()->goober.linkedTrapId)
                    gameObjTarget->TriggeringLinkedGameObject(trapEntry,m_caster);

                return;

            case GAMEOBJECT_TYPE_CHEST:
                // TODO: possible must be moved to loot release (in different from linked triggering)
                if (gameObjTarget->GetGOInfo()->chest.eventId)
                {
                    sLog.outDebug("Chest ScriptStart id %u for GO %u", gameObjTarget->GetGOInfo()->chest.eventId,gameObjTarget->GetDBTableGUIDLow());
                    sWorld.ScriptsStart(sEventScripts, gameObjTarget->GetGOInfo()->chest.eventId, player, gameObjTarget);
                }

                // triggering linked GO
                if(uint32 trapEntry = gameObjTarget->GetGOInfo()->chest.linkedTrapId)
                    gameObjTarget->TriggeringLinkedGameObject(trapEntry,m_caster);

                // Don't return, let loots been taken
        }
    }

    // Send loot
    player->SendLoot(guid, loottype);
}

void Spell::EffectOpenLock(uint32 effIndex)
{
    if(!m_caster || m_caster->GetTypeId() != TYPEID_PLAYER)
    {
        sLog.outDebug( "WORLD: Open Lock - No Player Caster!");
        return;
    }

    Player* player = (Player*)m_caster;

    uint32 lockId = 0;
    uint64 guid = 0;

    // Get lockId
    if(gameObjTarget)
    {
        GameObjectInfo const* goInfo = gameObjTarget->GetGOInfo();
        // Arathi Basin banner opening !
        if( goInfo->type == GAMEOBJECT_TYPE_BUTTON && goInfo->button.noDamageImmune ||
            goInfo->type == GAMEOBJECT_TYPE_GOOBER && goInfo->goober.losOK )
        {
            //CanUseBattleGroundObject() already called in CheckCast()
            // in battleground check
            if(BattleGround *bg = player->GetBattleGround())
            {
                // check if it's correct bg
                if(bg && bg->GetTypeID() == BATTLEGROUND_AB)
                    bg->EventPlayerClickedOnFlag(player, gameObjTarget);
                return;
            }
        }
        else if (goInfo->type == GAMEOBJECT_TYPE_FLAGSTAND)
        {
            //CanUseBattleGroundObject() already called in CheckCast()
            // in battleground check
            if(BattleGround *bg = player->GetBattleGround())
            {
                if(bg->GetTypeID() == BATTLEGROUND_EY)
                    bg->EventPlayerClickedOnFlag(player, gameObjTarget);
                return;
            }
        }
        // handle outdoor pvp object opening, return true if go was registered for handling
        // these objects must have been spawned by outdoorpvp!
        else if(gameObjTarget->GetGOInfo()->type == GAMEOBJECT_TYPE_GOOBER && sOutdoorPvPMgr.HandleOpenGo(player, gameObjTarget->GetGUID()))
            return;
        lockId = gameObjTarget->GetLockId();
        guid = gameObjTarget->GetGUID();
    }
    else if(itemTarget)
    {
        lockId = itemTarget->GetProto()->LockID;
        guid = itemTarget->GetGUID();
    }
    else
    {
        sLog.outDebug( "WORLD: Open Lock - No GameObject/Item Target!");
        return;
    }

    SkillType skillId = SKILL_NONE;
    int32 reqSkillValue = 0;
    int32 skillValue;

    SpellCastResult res = CanOpenLock(effIndex,lockId,skillId,reqSkillValue,skillValue);
    if(res != SPELL_CAST_OK)
    {
        SendCastResult(res);
        return;
    }

    SendLoot(guid, LOOT_SKINNING);

    // not allow use skill grow at item base open
    if(!m_CastItem && skillId != SKILL_NONE)
    {
        // update skill if really known
        if(uint32 pureSkillValue = player->GetPureSkillValue(skillId))
        {
            if(gameObjTarget)
            {
                // Allow one skill-up until respawned
                if ( !gameObjTarget->IsInSkillupList( player->GetGUIDLow() ) &&
                    player->UpdateGatherSkill(skillId, pureSkillValue, reqSkillValue) )
                    gameObjTarget->AddToSkillupList( player->GetGUIDLow() );
            }
            else if(itemTarget)
            {
                // Do one skill-up
                player->UpdateGatherSkill(skillId, pureSkillValue, reqSkillValue);
            }
        }
    }
}

void Spell::EffectSummonChangeItem(uint32 i)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player *player = (Player*)m_caster;

    // applied only to using item
    if(!m_CastItem)
        return;

    // ... only to item in own inventory/bank/equip_slot
    if(m_CastItem->GetOwnerGUID()!=player->GetGUID())
        return;

    uint32 newitemid = m_spellInfo->EffectItemType[i];
    if(!newitemid)
        return;

    uint16 pos = m_CastItem->GetPos();

    Item *pNewItem = Item::CreateItem( newitemid, 1, player);
    if( !pNewItem )
        return;

    for(uint8 i= PERM_ENCHANTMENT_SLOT; i<=TEMP_ENCHANTMENT_SLOT; ++i)
    {
        if(m_CastItem->GetEnchantmentId(EnchantmentSlot(i)))
            pNewItem->SetEnchantment(EnchantmentSlot(i), m_CastItem->GetEnchantmentId(EnchantmentSlot(i)), m_CastItem->GetEnchantmentDuration(EnchantmentSlot(i)), m_CastItem->GetEnchantmentCharges(EnchantmentSlot(i)));
    }

    if(m_CastItem->GetUInt32Value(ITEM_FIELD_DURABILITY) < m_CastItem->GetUInt32Value(ITEM_FIELD_MAXDURABILITY))
    {
        double loosePercent = 1 - m_CastItem->GetUInt32Value(ITEM_FIELD_DURABILITY) / double(m_CastItem->GetUInt32Value(ITEM_FIELD_MAXDURABILITY));
        player->DurabilityLoss(pNewItem, loosePercent);
    }

    if( player->IsInventoryPos( pos ) )
    {
        ItemPosCountVec dest;
        uint8 msg = player->CanStoreItem( m_CastItem->GetBagSlot(), m_CastItem->GetSlot(), dest, pNewItem, true );
        if( msg == EQUIP_ERR_OK )
        {
            player->DestroyItem(m_CastItem->GetBagSlot(), m_CastItem->GetSlot(),true);

            // prevent crash at access and unexpected charges counting with item update queue corrupt
            if(m_CastItem==m_targets.getItemTarget())
                m_targets.setItemTarget(NULL);

            m_CastItem = NULL;

            player->StoreItem( dest, pNewItem, true);
            return;
        }
    }
    else if( player->IsBankPos ( pos ) )
    {
        ItemPosCountVec dest;
        uint8 msg = player->CanBankItem( m_CastItem->GetBagSlot(), m_CastItem->GetSlot(), dest, pNewItem, true );
        if( msg == EQUIP_ERR_OK )
        {
            player->DestroyItem(m_CastItem->GetBagSlot(), m_CastItem->GetSlot(),true);

            // prevent crash at access and unexpected charges counting with item update queue corrupt
            if(m_CastItem==m_targets.getItemTarget())
                m_targets.setItemTarget(NULL);

            m_CastItem = NULL;

            player->BankItem( dest, pNewItem, true);
            return;
        }
    }
    else if( player->IsEquipmentPos ( pos ) )
    {
        uint16 dest;
        uint8 msg = player->CanEquipItem( m_CastItem->GetSlot(), dest, pNewItem, true );
        if( msg == EQUIP_ERR_OK )
        {
            player->DestroyItem(m_CastItem->GetBagSlot(), m_CastItem->GetSlot(),true);

            // prevent crash at access and unexpected charges counting with item update queue corrupt
            if(m_CastItem==m_targets.getItemTarget())
                m_targets.setItemTarget(NULL);

            m_CastItem = NULL;

            player->EquipItem( dest, pNewItem, true);
            player->AutoUnequipOffhandIfNeed();
            return;
        }
    }

    // fail
    delete pNewItem;
}

void Spell::EffectOpenSecretSafe(uint32 i)
{
    EffectOpenLock(i);                                      //no difference for now
}

void Spell::EffectProficiency(uint32 /*i*/)
{
    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;
    Player *p_target = (Player*)unitTarget;

    uint32 subClassMask = m_spellInfo->EquippedItemSubClassMask;
    if(m_spellInfo->EquippedItemClass == 2 && !(p_target->GetWeaponProficiency() & subClassMask))
    {
        p_target->AddWeaponProficiency(subClassMask);
        p_target->SendProficiency(uint8(0x02),p_target->GetWeaponProficiency());
    }
    if(m_spellInfo->EquippedItemClass == 4 && !(p_target->GetArmorProficiency() & subClassMask))
    {
        p_target->AddArmorProficiency(subClassMask);
        p_target->SendProficiency(uint8(0x04),p_target->GetArmorProficiency());
    }
}

void Spell::EffectApplyAreaAura(uint32 i)
{
    if(!unitTarget)
        return;
    if(!unitTarget->isAlive())
        return;

    AreaAura* Aur = new AreaAura(m_spellInfo, i, &m_currentBasePoints[i], unitTarget, m_caster, m_CastItem);
    unitTarget->AddAura(Aur);
}

void Spell::EffectSummonType(uint32 i)
{
    switch(m_spellInfo->EffectMiscValueB[i])
    {
        case SUMMON_TYPE_GUARDIAN:
        case SUMMON_TYPE_POSESSED:
        case SUMMON_TYPE_POSESSED2:
        case SUMMON_TYPE_FORCE_OF_NATURE:
        case SUMMON_TYPE_GUARDIAN2:
        case SUMMON_TYPE_UNKNOWN13:
            EffectSummonGuardian(i);
            break;
        case SUMMON_TYPE_WILD:
        case SUMMON_TYPE_UNKNOWN6:
        case SUMMON_TYPE_UNKNOWN7:
        case SUMMON_TYPE_UNKNOWN8:
        case SUMMON_TYPE_UNKNOWN9:
        case SUMMON_TYPE_UNKNOWN10:
        case SUMMON_TYPE_UNKNOWN11:
        case SUMMON_TYPE_UNKNOWN12:
        case SUMMON_TYPE_WILD2:
            EffectSummonWild(i);
            break;
        case SUMMON_TYPE_DEMON:
            EffectSummonDemon(i);
            break;
        case SUMMON_TYPE_SUMMON:
            EffectSummon(i);
            break;
        case SUMMON_TYPE_CRITTER:
        case SUMMON_TYPE_CRITTER2:
        case SUMMON_TYPE_CRITTER3:
            EffectSummonCritter(i);
            break;
        case SUMMON_TYPE_TOTEM_SLOT1:
        case SUMMON_TYPE_TOTEM_SLOT2:
        case SUMMON_TYPE_TOTEM_SLOT3:
        case SUMMON_TYPE_TOTEM_SLOT4:
        case SUMMON_TYPE_TOTEM:
            EffectSummonTotem(i);
            break;
        case SUMMON_TYPE_UNKNOWN1:
        case SUMMON_TYPE_UNKNOWN2:
        case SUMMON_TYPE_UNKNOWN3:
        case SUMMON_TYPE_UNKNOWN4:
        case SUMMON_TYPE_UNKNOWN5:
            break;
        default:
            sLog.outError("EffectSummonType: Unhandled summon type %u", m_spellInfo->EffectMiscValueB[i]);
            break;
    }
}

void Spell::EffectSummon(uint32 i)
{
    if(m_caster->GetPetGUID())
        return;

    if(!unitTarget)
        return;
    uint32 pet_entry = m_spellInfo->EffectMiscValue[i];
    if(!pet_entry)
        return;
    uint32 level = m_caster->getLevel();
    Pet* spawnCreature = new Pet(SUMMON_PET);

    if(m_caster->GetTypeId()==TYPEID_PLAYER && spawnCreature->LoadPetFromDB((Player*)m_caster,pet_entry))
    {
        // set timer for unsummon
        int32 duration = GetSpellDuration(m_spellInfo);
        if(duration > 0)
            spawnCreature->SetDuration(duration);

        return;
    }

    Map *map = m_caster->GetMap();
    uint32 pet_number = objmgr.GeneratePetNumber();
    if(!spawnCreature->Create(objmgr.GenerateLowGuid(HIGHGUID_PET),map,m_caster->GetPhaseMask(),
        m_spellInfo->EffectMiscValue[i], pet_number))
    {
        sLog.outErrorDb("Spell::EffectSummon: no such creature entry %u",m_spellInfo->EffectMiscValue[i]);
        delete spawnCreature;
        return;
    }

    // Summon in dest location
    float x,y,z;
    if(m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION)
    {
        x = m_targets.m_destX;
        y = m_targets.m_destY;
        z = m_targets.m_destZ;
    }
    else
        m_caster->GetClosePoint(x,y,z,spawnCreature->GetObjectSize());

    spawnCreature->Relocate(x,y,z,-m_caster->GetOrientation());

    if(!spawnCreature->IsPositionValid())
    {
        sLog.outError("Pet (guidlow %d, entry %d) not summoned. Suggested coordinates isn't valid (X: %f Y: %f)",
            spawnCreature->GetGUIDLow(), spawnCreature->GetEntry(), spawnCreature->GetPositionX(), spawnCreature->GetPositionY());
        delete spawnCreature;
        return;
    }

    // set timer for unsummon
    int32 duration = GetSpellDuration(m_spellInfo);
    if(duration > 0)
        spawnCreature->SetDuration(duration);

    spawnCreature->SetOwnerGUID(m_caster->GetGUID());
    spawnCreature->SetUInt32Value(UNIT_NPC_FLAGS, 0);
    spawnCreature->setPowerType(POWER_MANA);
    spawnCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, m_caster->getFaction());
    spawnCreature->SetUInt32Value(UNIT_FIELD_FLAGS, 0);
    spawnCreature->SetUInt32Value(UNIT_FIELD_BYTES_0, 2048);
    spawnCreature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
    spawnCreature->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, 0);
    spawnCreature->SetUInt32Value(UNIT_FIELD_PETEXPERIENCE, 0);
    spawnCreature->SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, 1000);
    spawnCreature->SetCreatorGUID(m_caster->GetGUID());
    spawnCreature->SetUInt32Value(UNIT_CREATED_BY_SPELL, m_spellInfo->Id);

    spawnCreature->InitStatsForLevel(level);

    spawnCreature->GetCharmInfo()->SetPetNumber(pet_number, false);

    spawnCreature->AIM_Initialize();
    spawnCreature->InitPetCreateSpells();
    spawnCreature->SetHealth(spawnCreature->GetMaxHealth());
    spawnCreature->SetPower(POWER_MANA, spawnCreature->GetMaxPower(POWER_MANA));

    std::string name = m_caster->GetName();
    name.append(petTypeSuffix[spawnCreature->getPetType()]);
    spawnCreature->SetName( name );

    map->Add((Creature*)spawnCreature);

    if(m_caster->GetTypeId() == TYPEID_PLAYER)
    {
        m_caster->SetPet(spawnCreature);
        spawnCreature->GetCharmInfo()->SetReactState( REACT_DEFENSIVE );
        spawnCreature->SavePetToDB(PET_SAVE_AS_CURRENT);
        ((Player*)m_caster)->PetSpellInitialize();
    }
}

void Spell::EffectLearnSpell(uint32 i)
{
    if(!unitTarget)
        return;

    if(unitTarget->GetTypeId() != TYPEID_PLAYER)
    {
        if(m_caster->GetTypeId() == TYPEID_PLAYER)
            EffectLearnPetSpell(i);

        return;
    }

    Player *player = (Player*)unitTarget;

    uint32 spellToLearn = ((m_spellInfo->Id==SPELL_ID_GENERIC_LEARN) || (m_spellInfo->Id==SPELL_ID_GENERIC_LEARN_PET)) ? damage : m_spellInfo->EffectTriggerSpell[i];
    player->learnSpell(spellToLearn,false);

    sLog.outDebug( "Spell: Player %u have learned spell %u from NpcGUID=%u", player->GetGUIDLow(), spellToLearn, m_caster->GetGUIDLow() );
}

void Spell::EffectDispel(uint32 i)
{
    if(!unitTarget)
        return;

    // Fill possible dispell list
    std::vector <Aura *> dispel_list;

    // Create dispel mask by dispel type
    uint32 dispel_type = m_spellInfo->EffectMiscValue[i];
    uint32 dispelMask  = GetDispellMask( DispelType(dispel_type) );
    Unit::AuraMap const& auras = unitTarget->GetAuras();
    for(Unit::AuraMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
    {
        Aura *aur = (*itr).second;
        if (aur && (1<<aur->GetSpellProto()->Dispel) & dispelMask)
        {
            if(aur->GetSpellProto()->Dispel == DISPEL_MAGIC)
            {
                bool positive = true;
                if (!aur->IsPositive())
                    positive = false;
                else
                    positive = (aur->GetSpellProto()->AttributesEx & SPELL_ATTR_EX_NEGATIVE)==0;

                // do not remove positive auras if friendly target
                //               negative auras if non-friendly target
                if(positive == unitTarget->IsFriendlyTo(m_caster))
                    continue;
            }
            // Add aura to dispel list
            dispel_list.push_back(aur);
        }
    }
    // Ok if exist some buffs for dispel try dispel it
    if (!dispel_list.empty())
    {
        std::list < std::pair<uint32,uint64> > success_list;// (spell_id,casterGuid)
        std::list < uint32 > fail_list;                     // spell_id
        int32 list_size = dispel_list.size();
        // Dispell N = damage buffs (or while exist buffs for dispel)
        for (int32 count=0; count < damage && list_size > 0; ++count)
        {
            // Random select buff for dispel
            Aura *aur = dispel_list[urand(0, list_size-1)];

            SpellEntry const* spellInfo = aur->GetSpellProto();
            // Base dispel chance
            // TODO: possible chance depend from spell level??
            int32 miss_chance = 0;
            // Apply dispel mod from aura caster
            if (Unit *caster = aur->GetCaster())
            {
                if ( Player* modOwner = caster->GetSpellModOwner() )
                    modOwner->ApplySpellMod(spellInfo->Id, SPELLMOD_RESIST_DISPEL_CHANCE, miss_chance, this);
            }
            // Try dispel
            if (roll_chance_i(miss_chance))
                fail_list.push_back(aur->GetId());
            else
                success_list.push_back(std::pair<uint32,uint64>(aur->GetId(),aur->GetCasterGUID()));
            // Remove buff from list for prevent doubles
            for (std::vector<Aura *>::iterator j = dispel_list.begin(); j != dispel_list.end(); )
            {
                Aura *dispeled = *j;
                if (dispeled->GetId() == aur->GetId() && dispeled->GetCasterGUID() == aur->GetCasterGUID())
                {
                    j = dispel_list.erase(j);
                    --list_size;
                }
                else
                    ++j;
            }
        }
        // Send success log and really remove auras
        if (!success_list.empty())
        {
            int32 count = success_list.size();
            WorldPacket data(SMSG_SPELLDISPELLOG, 8+8+4+1+4+count*5);
            data.append(unitTarget->GetPackGUID());         // Victim GUID
            data.append(m_caster->GetPackGUID());           // Caster GUID
            data << uint32(m_spellInfo->Id);                // Dispell spell id
            data << uint8(0);                               // not used
            data << uint32(count);                          // count
            for (std::list<std::pair<uint32,uint64> >::iterator j = success_list.begin(); j != success_list.end(); ++j)
            {
                SpellEntry const* spellInfo = sSpellStore.LookupEntry(j->first);
                data << uint32(spellInfo->Id);              // Spell Id
                data << uint8(0);                           // 0 - dispeled !=0 cleansed
                unitTarget->RemoveAurasDueToSpellByDispel(spellInfo->Id, j->second, m_caster);
            }
            m_caster->SendMessageToSet(&data, true);

            // On succes dispel
            // Devour Magic
            if (m_spellInfo->SpellFamilyName == SPELLFAMILY_WARLOCK && m_spellInfo->Category == SPELLCATEGORY_DEVOUR_MAGIC)
            {
                uint32 heal_spell = 0;
                switch (m_spellInfo->Id)
                {
                    case 19505: heal_spell = 19658; break;
                    case 19731: heal_spell = 19732; break;
                    case 19734: heal_spell = 19733; break;
                    case 19736: heal_spell = 19735; break;
                    case 27276: heal_spell = 27278; break;
                    case 27277: heal_spell = 27279; break;
                    default:
                        sLog.outDebug("Spell for Devour Magic %d not handled in Spell::EffectDispel", m_spellInfo->Id);
                        break;
                }
                if (heal_spell)
                    m_caster->CastSpell(m_caster, heal_spell, true);
            }
        }
        // Send fail log to client
        if (!fail_list.empty())
        {
            // Failed to dispell
            WorldPacket data(SMSG_DISPEL_FAILED, 8+8+4+4*fail_list.size());
            data << uint64(m_caster->GetGUID());            // Caster GUID
            data << uint64(unitTarget->GetGUID());          // Victim GUID
            data << uint32(m_spellInfo->Id);                // Dispell spell id
            for (std::list< uint32 >::iterator j = fail_list.begin(); j != fail_list.end(); ++j)
                data << uint32(*j);                         // Spell Id
            m_caster->SendMessageToSet(&data, true);
        }
    }
}

void Spell::EffectDualWield(uint32 /*i*/)
{
    if (unitTarget && unitTarget->GetTypeId() == TYPEID_PLAYER)
        ((Player*)unitTarget)->SetCanDualWield(true);
}

void Spell::EffectPull(uint32 /*i*/)
{
    // TODO: create a proper pull towards distract spell center for distract
    sLog.outDebug("WORLD: Spell Effect DUMMY");
}

void Spell::EffectDistract(uint32 /*i*/)
{
    // Check for possible target
    if (!unitTarget || unitTarget->isInCombat())
        return;

    // target must be OK to do this
    if( unitTarget->hasUnitState(UNIT_STAT_CONFUSED | UNIT_STAT_STUNNED | UNIT_STAT_FLEEING ) )
        return;

    float angle = unitTarget->GetAngle(m_targets.m_destX, m_targets.m_destY);

    if ( unitTarget->GetTypeId() == TYPEID_PLAYER )
    {
        // For players just turn them
        WorldPacket data;
        ((Player*)unitTarget)->BuildTeleportAckMsg(&data, unitTarget->GetPositionX(), unitTarget->GetPositionY(), unitTarget->GetPositionZ(), angle);
        ((Player*)unitTarget)->GetSession()->SendPacket( &data );
        ((Player*)unitTarget)->SetPosition(unitTarget->GetPositionX(), unitTarget->GetPositionY(), unitTarget->GetPositionZ(), angle, false);
    }
    else
    {
        // Set creature Distracted, Stop it, And turn it
        unitTarget->SetOrientation(angle);
        unitTarget->StopMoving();
        unitTarget->GetMotionMaster()->MoveDistract(damage*IN_MILISECONDS);
    }
}

void Spell::EffectPickPocket(uint32 /*i*/)
{
    if( m_caster->GetTypeId() != TYPEID_PLAYER )
        return;

    // victim must be creature and attackable
    if( !unitTarget || unitTarget->GetTypeId() != TYPEID_UNIT || m_caster->IsFriendlyTo(unitTarget) )
        return;

    // victim have to be alive and humanoid or undead
    if( unitTarget->isAlive() && (unitTarget->GetCreatureTypeMask() &CREATURE_TYPEMASK_HUMANOID_OR_UNDEAD) != 0)
    {
        int32 chance = 10 + int32(m_caster->getLevel()) - int32(unitTarget->getLevel());

        if (chance > irand(0, 19))
        {
            // Stealing successful
            //sLog.outDebug("Sending loot from pickpocket");
            ((Player*)m_caster)->SendLoot(unitTarget->GetGUID(),LOOT_PICKPOCKETING);
        }
        else
        {
            // Reveal action + get attack
            m_caster->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
            if (((Creature*)unitTarget)->AI())
                ((Creature*)unitTarget)->AI()->AttackStart(m_caster);
        }
    }
}

void Spell::EffectAddFarsight(uint32 i)
{
    float radius = GetSpellRadius(sSpellRadiusStore.LookupEntry(m_spellInfo->EffectRadiusIndex[i]));
    int32 duration = GetSpellDuration(m_spellInfo);
    DynamicObject* dynObj = new DynamicObject;
    if(!dynObj->Create(objmgr.GenerateLowGuid(HIGHGUID_DYNAMICOBJECT), m_caster, m_spellInfo->Id, i, m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, duration, radius))
    {
        delete dynObj;
        return;
    }
    dynObj->SetUInt32Value(OBJECT_FIELD_TYPE, 65);
    dynObj->SetUInt32Value(DYNAMICOBJECT_BYTES, 0x80000002);
    m_caster->AddDynObject(dynObj);
    dynObj->GetMap()->Add(dynObj);
    if(m_caster->GetTypeId() == TYPEID_PLAYER)
        ((Player*)m_caster)->SetFarSightGUID(dynObj->GetGUID());
}

void Spell::EffectSummonWild(uint32 i)
{
    uint32 creature_entry = m_spellInfo->EffectMiscValue[i];
    if(!creature_entry)
        return;

    uint32 level = m_caster->getLevel();

    // level of creature summoned using engineering item based at engineering skill level
    if(m_caster->GetTypeId()==TYPEID_PLAYER && m_CastItem)
    {
        ItemPrototype const *proto = m_CastItem->GetProto();
        if(proto && proto->RequiredSkill == SKILL_ENGINERING)
        {
            uint16 skill202 = ((Player*)m_caster)->GetSkillValue(SKILL_ENGINERING);
            if(skill202)
            {
                level = skill202/5;
            }
        }
    }

    // select center of summon position
    float center_x = m_targets.m_destX;
    float center_y = m_targets.m_destY;
    float center_z = m_targets.m_destZ;

    float radius = GetSpellRadius(sSpellRadiusStore.LookupEntry(m_spellInfo->EffectRadiusIndex[i]));

    int32 amount = damage > 0 ? damage : 1;

    for(int32 count = 0; count < amount; ++count)
    {
        float px, py, pz;
        // If dest location if present
        if (m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION)
        {
            // Summon 1 unit in dest location
            if (count == 0)
            {
                px = m_targets.m_destX;
                py = m_targets.m_destY;
                pz = m_targets.m_destZ;
            }
            // Summon in random point all other units if location present
            else
                m_caster->GetRandomPoint(center_x,center_y,center_z,radius,px,py,pz);
        }
        // Summon if dest location not present near caster
        else
            m_caster->GetClosePoint(px,py,pz,3.0f);

        int32 duration = GetSpellDuration(m_spellInfo);

        TempSummonType summonType = (duration == 0) ? TEMPSUMMON_DEAD_DESPAWN : TEMPSUMMON_TIMED_OR_DEAD_DESPAWN;

        m_caster->SummonCreature(creature_entry,px,py,pz,m_caster->GetOrientation(),summonType,duration);
    }
}

void Spell::EffectSummonGuardian(uint32 i)
{
    uint32 pet_entry = m_spellInfo->EffectMiscValue[i];
    if(!pet_entry)
        return;

    // Jewelery statue case (totem like)
    if(m_spellInfo->SpellIconID==2056)
    {
        EffectSummonTotem(i);
        return;
    }

    // set timer for unsummon
    int32 duration = GetSpellDuration(m_spellInfo);

    // Search old Guardian only for players (if casted spell not have duration or cooldown)
    // FIXME: some guardians have control spell applied and controlled by player and anyway player can't summon in this time
    //        so this code hack in fact
    if( m_caster->GetTypeId() == TYPEID_PLAYER && (duration <= 0 || GetSpellRecoveryTime(m_spellInfo)==0) )
        if(((Player*)m_caster)->HasGuardianWithEntry(pet_entry))
            return;                                         // find old guardian, ignore summon

    // in another case summon new
    uint32 level = m_caster->getLevel();

    // level of pet summoned using engineering item based at engineering skill level
    if(m_caster->GetTypeId()==TYPEID_PLAYER && m_CastItem)
    {
        ItemPrototype const *proto = m_CastItem->GetProto();
        if(proto && proto->RequiredSkill == SKILL_ENGINERING)
        {
            uint16 skill202 = ((Player*)m_caster)->GetSkillValue(SKILL_ENGINERING);
            if(skill202)
            {
                level = skill202/5;
            }
        }
    }

    // select center of summon position
    float center_x = m_targets.m_destX;
    float center_y = m_targets.m_destY;
    float center_z = m_targets.m_destZ;

    float radius = GetSpellRadius(sSpellRadiusStore.LookupEntry(m_spellInfo->EffectRadiusIndex[i]));

    int32 amount = damage > 0 ? damage : 1;

    for(int32 count = 0; count < amount; ++count)
    {
        Pet* spawnCreature = new Pet(GUARDIAN_PET);

        Map *map = m_caster->GetMap();
        uint32 pet_number = objmgr.GeneratePetNumber();
        if(!spawnCreature->Create(objmgr.GenerateLowGuid(HIGHGUID_PET), map,m_caster->GetPhaseMask(),
            m_spellInfo->EffectMiscValue[i], pet_number))
        {
            sLog.outError("no such creature entry %u",m_spellInfo->EffectMiscValue[i]);
            delete spawnCreature;
            return;
        }

        float px, py, pz;
        // If dest location if present
        if (m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION)
        {
            // Summon 1 unit in dest location
            if (count == 0)
            {
                px = m_targets.m_destX;
                py = m_targets.m_destY;
                pz = m_targets.m_destZ;
            }
            // Summon in random point all other units if location present
            else
                m_caster->GetRandomPoint(center_x,center_y,center_z,radius,px,py,pz);
        }
        // Summon if dest location not present near caster
        else
            m_caster->GetClosePoint(px,py,pz,spawnCreature->GetObjectSize());

        spawnCreature->Relocate(px,py,pz,m_caster->GetOrientation());

        if(!spawnCreature->IsPositionValid())
        {
            sLog.outError("Pet (guidlow %d, entry %d) not created base at creature. Suggested coordinates isn't valid (X: %f Y: %f)",
                spawnCreature->GetGUIDLow(), spawnCreature->GetEntry(), spawnCreature->GetPositionX(), spawnCreature->GetPositionY());
            delete spawnCreature;
            return;
        }

        if(duration > 0)
            spawnCreature->SetDuration(duration);

        spawnCreature->SetOwnerGUID(m_caster->GetGUID());
        spawnCreature->setPowerType(POWER_MANA);
        spawnCreature->SetUInt32Value(UNIT_NPC_FLAGS , 0);
        spawnCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE,m_caster->getFaction());
        spawnCreature->SetUInt32Value(UNIT_FIELD_FLAGS,0);
        spawnCreature->SetUInt32Value(UNIT_FIELD_BYTES_1,0);
        spawnCreature->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP,0);
        spawnCreature->SetCreatorGUID(m_caster->GetGUID());
        spawnCreature->SetUInt32Value(UNIT_CREATED_BY_SPELL, m_spellInfo->Id);

        spawnCreature->InitStatsForLevel(level);
        spawnCreature->GetCharmInfo()->SetPetNumber(pet_number, false);

        spawnCreature->AIM_Initialize();

        if(m_caster->GetTypeId()==TYPEID_PLAYER)
            ((Player*)m_caster)->AddGuardian(spawnCreature);

        map->Add((Creature*)spawnCreature);
    }
}

void Spell::EffectTeleUnitsFaceCaster(uint32 i)
{
    if(!unitTarget)
        return;

    if(unitTarget->isInFlight())
        return;

    float dis = GetSpellRadius(sSpellRadiusStore.LookupEntry(m_spellInfo->EffectRadiusIndex[i]));

    float fx,fy,fz;
    m_caster->GetClosePoint(fx,fy,fz,unitTarget->GetObjectSize(),dis);

    unitTarget->NearTeleportTo(fx,fy,fz,-m_caster->GetOrientation(),unitTarget==m_caster);
}

void Spell::EffectLearnSkill(uint32 i)
{
    if(unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    if(damage < 0)
        return;

    uint32 skillid =  m_spellInfo->EffectMiscValue[i];
    uint16 skillval = ((Player*)unitTarget)->GetPureSkillValue(skillid);
    ((Player*)unitTarget)->SetSkill(skillid, skillval?skillval:1, damage*75);
}

void Spell::EffectAddHonor(uint32 /*i*/)
{
    if(unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    // not scale value for item based reward (/10 value expected)
    if(m_CastItem)
    {
        ((Player*)unitTarget)->RewardHonor(NULL, 1, damage/10);
        sLog.outError("SpellEffect::AddHonor (spell_id %u) rewards %d honor points (item %u) for player: %u", m_spellInfo->Id, damage/10, m_CastItem->GetEntry(),((Player*)unitTarget)->GetGUIDLow());
        return;
    }

    // do not allow to add too many honor for player (50 * 21) = 1040 at level 70, or (50 * 31) = 1550 at level 80
    if( damage <= 50)
    {
        uint32 honor_reward = MaNGOS::Honor::hk_honor_at_level(unitTarget->getLevel(), damage);
        ((Player*)unitTarget)->RewardHonor(NULL, 1, honor_reward);
        sLog.outDebug("SpellEffect::AddHonor (spell_id %u) rewards %u honor points (scale) to player: %u", m_spellInfo->Id, honor_reward, ((Player*)unitTarget)->GetGUIDLow());
    }
    else
    {
        //maybe we have correct honor_gain in damage already
        ((Player*)unitTarget)->RewardHonor(NULL, 1, damage);
        sLog.outError("SpellEffect::AddHonor (spell_id %u) rewards %u honor points (non scale) for player: %u", m_spellInfo->Id, damage, ((Player*)unitTarget)->GetGUIDLow());
    }
}

void Spell::EffectTradeSkill(uint32 /*i*/)
{
    if(unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;
    // uint32 skillid =  m_spellInfo->EffectMiscValue[i];
    // uint16 skillmax = ((Player*)unitTarget)->(skillid);
    // ((Player*)unitTarget)->SetSkill(skillid,skillval?skillval:1,skillmax+75);
}

void Spell::EffectEnchantItemPerm(uint32 effect_idx)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;
    if (!itemTarget)
        return;

    Player* p_caster = (Player*)m_caster;

    // not grow at item use at item case
    p_caster->UpdateCraftSkill(m_spellInfo->Id);

    uint32 enchant_id = m_spellInfo->EffectMiscValue[effect_idx];
    if (!enchant_id)
        return;

    SpellItemEnchantmentEntry const *pEnchant = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
    if(!pEnchant)
        return;

    // item can be in trade slot and have owner diff. from caster
    Player* item_owner = itemTarget->GetOwner();
    if(!item_owner)
        return;

    if(item_owner!=p_caster && p_caster->GetSession()->GetSecurity() > SEC_PLAYER && sWorld.getConfig(CONFIG_GM_LOG_TRADE) )
    {
        sLog.outCommand(p_caster->GetSession()->GetAccountId(),"GM %s (Account: %u) enchanting(perm): %s (Entry: %d) for player: %s (Account: %u)",
            p_caster->GetName(),p_caster->GetSession()->GetAccountId(),
            itemTarget->GetProto()->Name1,itemTarget->GetEntry(),
            item_owner->GetName(),item_owner->GetSession()->GetAccountId());
    }

    // remove old enchanting before applying new if equipped
    item_owner->ApplyEnchantment(itemTarget,PERM_ENCHANTMENT_SLOT,false);

    itemTarget->SetEnchantment(PERM_ENCHANTMENT_SLOT, enchant_id, 0, 0);

    // add new enchanting if equipped
    item_owner->ApplyEnchantment(itemTarget,PERM_ENCHANTMENT_SLOT,true);
}

void Spell::EffectEnchantItemPrismatic(uint32 effect_idx)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;
    if (!itemTarget)
        return;

    Player* p_caster = (Player*)m_caster;

    uint32 enchant_id = m_spellInfo->EffectMiscValue[effect_idx];
    if (!enchant_id)
        return;

    SpellItemEnchantmentEntry const *pEnchant = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
    if(!pEnchant)
        return;

    // support only enchantings with add socket in this slot
    {
        bool add_socket = false;
        for(int i = 0; i < 3; ++i)
        {
            if(pEnchant->type[i]==ITEM_ENCHANTMENT_TYPE_PRISMATIC_SOCKET)
            {
                add_socket = true;
                break;
            }
        }
        if(!add_socket)
        {
            sLog.outError("Spell::EffectEnchantItemPrismatic: attempt apply enchant spell %u with SPELL_EFFECT_ENCHANT_ITEM_PRISMATIC (%u) but without ITEM_ENCHANTMENT_TYPE_PRISMATIC_SOCKET (u), not suppoted yet.",
                m_spellInfo->Id,SPELL_EFFECT_ENCHANT_ITEM_PRISMATIC,ITEM_ENCHANTMENT_TYPE_PRISMATIC_SOCKET);
            return;
        }
    }

    // item can be in trade slot and have owner diff. from caster
    Player* item_owner = itemTarget->GetOwner();
    if(!item_owner)
        return;

    if(item_owner!=p_caster && p_caster->GetSession()->GetSecurity() > SEC_PLAYER && sWorld.getConfig(CONFIG_GM_LOG_TRADE) )
    {
        sLog.outCommand(p_caster->GetSession()->GetAccountId(),"GM %s (Account: %u) enchanting(perm): %s (Entry: %d) for player: %s (Account: %u)",
            p_caster->GetName(),p_caster->GetSession()->GetAccountId(),
            itemTarget->GetProto()->Name1,itemTarget->GetEntry(),
            item_owner->GetName(),item_owner->GetSession()->GetAccountId());
    }

    // remove old enchanting before applying new if equipped
    item_owner->ApplyEnchantment(itemTarget,PRISMATIC_ENCHANTMENT_SLOT,false);

    itemTarget->SetEnchantment(PRISMATIC_ENCHANTMENT_SLOT, enchant_id, 0, 0);

    // add new enchanting if equipped
    item_owner->ApplyEnchantment(itemTarget,PRISMATIC_ENCHANTMENT_SLOT,true);
}

void Spell::EffectEnchantItemTmp(uint32 i)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* p_caster = (Player*)m_caster;

    if(!itemTarget)
        return;

    uint32 enchant_id = m_spellInfo->EffectMiscValue[i];

    // Shaman Rockbiter Weapon
    if(i==0 && m_spellInfo->Effect[1]==SPELL_EFFECT_DUMMY)
    {
        int32 enchnting_damage = m_currentBasePoints[1]+1;

        // enchanting id selected by calculated damage-per-sec stored in Effect[1] base value
        // with already applied percent bonus from Elemental Weapons talent
        // Note: damage calculated (correctly) with rounding int32(float(v)) but
        // RW enchantments applied damage int32(float(v)+0.5), this create  0..1 difference sometime
        switch(enchnting_damage)
        {
            // Rank 1
            case  2: enchant_id =   29; break;              //  0% [ 7% ==  2, 14% == 2, 20% == 2]
            // Rank 2
            case  4: enchant_id =    6; break;              //  0% [ 7% ==  4, 14% == 4]
            case  5: enchant_id = 3025; break;              // 20%
            // Rank 3
            case  6: enchant_id =    1; break;              //  0% [ 7% ==  6, 14% == 6]
            case  7: enchant_id = 3027; break;              // 20%
            // Rank 4
            case  9: enchant_id = 3032; break;              //  0% [ 7% ==  6]
            case 10: enchant_id =  503; break;              // 14%
            case 11: enchant_id = 3031; break;              // 20%
            // Rank 5
            case 15: enchant_id = 3035; break;              // 0%
            case 16: enchant_id = 1663; break;              // 7%
            case 17: enchant_id = 3033; break;              // 14%
            case 18: enchant_id = 3034; break;              // 20%
            // Rank 6
            case 28: enchant_id = 3038; break;              // 0%
            case 29: enchant_id =  683; break;              // 7%
            case 31: enchant_id = 3036; break;              // 14%
            case 33: enchant_id = 3037; break;              // 20%
            // Rank 7
            case 40: enchant_id = 3041; break;              // 0%
            case 42: enchant_id = 1664; break;              // 7%
            case 45: enchant_id = 3039; break;              // 14%
            case 48: enchant_id = 3040; break;              // 20%
            // Rank 8
            case 49: enchant_id = 3044; break;              // 0%
            case 52: enchant_id = 2632; break;              // 7%
            case 55: enchant_id = 3042; break;              // 14%
            case 58: enchant_id = 3043; break;              // 20%
            // Rank 9
            case 62: enchant_id = 2633; break;              // 0%
            case 66: enchant_id = 3018; break;              // 7%
            case 70: enchant_id = 3019; break;              // 14%
            case 74: enchant_id = 3020; break;              // 20%
            default:
                sLog.outError("Spell::EffectEnchantItemTmp: Damage %u not handled in S'RW",enchnting_damage);
                return;
        }
    }

    if (!enchant_id)
    {
        sLog.outError("Spell %u Effect %u (SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY) have 0 as enchanting id",m_spellInfo->Id,i);
        return;
    }

    SpellItemEnchantmentEntry const *pEnchant = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
    if(!pEnchant)
    {
        sLog.outError("Spell %u Effect %u (SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY) have not existed enchanting id %u ",m_spellInfo->Id,i,enchant_id);
        return;
    }

    // select enchantment duration
    uint32 duration;

    // rogue family enchantments exception by duration
    if(m_spellInfo->Id==38615)
        duration = 1800;                                    // 30 mins
    // other rogue family enchantments always 1 hour (some have spell damage=0, but some have wrong data in EffBasePoints)
    else if(m_spellInfo->SpellFamilyName==SPELLFAMILY_ROGUE)
        duration = 3600;                                    // 1 hour
    // shaman family enchantments
    else if(m_spellInfo->SpellFamilyName==SPELLFAMILY_SHAMAN)
        duration = 1800;                                    // 30 mins
    // other cases with this SpellVisual already selected
    else if(m_spellInfo->SpellVisual[0]==215)
        duration = 1800;                                    // 30 mins
    // some fishing pole bonuses
    else if(m_spellInfo->SpellVisual[0]==563)
        duration = 600;                                     // 10 mins
    // shaman rockbiter enchantments
    else if(m_spellInfo->SpellVisual[0]==0)
        duration = 1800;                                    // 30 mins
    else if(m_spellInfo->Id==29702)
        duration = 300;                                     // 5 mins
    else if(m_spellInfo->Id==37360)
        duration = 300;                                     // 5 mins
    // default case
    else
        duration = 3600;                                    // 1 hour

    // item can be in trade slot and have owner diff. from caster
    Player* item_owner = itemTarget->GetOwner();
    if(!item_owner)
        return;

    if(item_owner!=p_caster && p_caster->GetSession()->GetSecurity() > SEC_PLAYER && sWorld.getConfig(CONFIG_GM_LOG_TRADE) )
    {
        sLog.outCommand(p_caster->GetSession()->GetAccountId(),"GM %s (Account: %u) enchanting(temp): %s (Entry: %d) for player: %s (Account: %u)",
            p_caster->GetName(),p_caster->GetSession()->GetAccountId(),
            itemTarget->GetProto()->Name1,itemTarget->GetEntry(),
            item_owner->GetName(),item_owner->GetSession()->GetAccountId());
    }

    // remove old enchanting before applying new if equipped
    item_owner->ApplyEnchantment(itemTarget,TEMP_ENCHANTMENT_SLOT,false);

    itemTarget->SetEnchantment(TEMP_ENCHANTMENT_SLOT, enchant_id, duration*1000, 0);

    // add new enchanting if equipped
    item_owner->ApplyEnchantment(itemTarget,TEMP_ENCHANTMENT_SLOT,true);
}

void Spell::EffectTameCreature(uint32 /*i*/)
{
    if(m_caster->GetPetGUID())
        return;

    if(!unitTarget)
        return;

    if(unitTarget->GetTypeId() == TYPEID_PLAYER)
        return;

    Creature* creatureTarget = (Creature*)unitTarget;

    if(creatureTarget->isPet())
        return;

    if(m_caster->getClass() != CLASS_HUNTER)
        return;

    // cast finish successfully
    //SendChannelUpdate(0);
    finish();

    Pet* pet = m_caster->CreateTamedPetFrom(creatureTarget,m_spellInfo->Id);

    // kill original creature
    creatureTarget->setDeathState(JUST_DIED);
    creatureTarget->RemoveCorpse();
    creatureTarget->SetHealth(0);                       // just for nice GM-mode view

    uint32 level = (creatureTarget->getLevel() < (m_caster->getLevel() - 5)) ? (m_caster->getLevel() - 5) : creatureTarget->getLevel();

    // prepare visual effect for levelup
    pet->SetUInt32Value(UNIT_FIELD_LEVEL, level - 1);

    // add to world
    pet->GetMap()->Add((Creature*)pet);

    // visual effect for levelup
    pet->SetUInt32Value(UNIT_FIELD_LEVEL, level);

    // caster have pet now
    m_caster->SetPet(pet);

    if(m_caster->GetTypeId() == TYPEID_PLAYER)
    {
        pet->SavePetToDB(PET_SAVE_AS_CURRENT);
        ((Player*)m_caster)->PetSpellInitialize();
    }
}

void Spell::EffectSummonPet(uint32 i)
{
    uint32 petentry = m_spellInfo->EffectMiscValue[i];

    Pet *OldSummon = m_caster->GetPet();

    // if pet requested type already exist
    if( OldSummon )
    {
        if(petentry == 0 || OldSummon->GetEntry() == petentry)
        {
            // pet in corpse state can't be summoned
            if( OldSummon->isDead() )
                return;

            OldSummon->GetMap()->Remove((Creature*)OldSummon,false);
            OldSummon->SetMapId(m_caster->GetMapId());

            float px, py, pz;
            m_caster->GetClosePoint(px, py, pz, OldSummon->GetObjectSize());

            OldSummon->Relocate(px, py, pz, OldSummon->GetOrientation());
            m_caster->GetMap()->Add((Creature*)OldSummon);

            if(m_caster->GetTypeId() == TYPEID_PLAYER && OldSummon->isControlled() )
            {
                ((Player*)m_caster)->PetSpellInitialize();
            }
            return;
        }

        if(m_caster->GetTypeId() == TYPEID_PLAYER)
            ((Player*)m_caster)->RemovePet(OldSummon,(OldSummon->getPetType()==HUNTER_PET ? PET_SAVE_AS_DELETED : PET_SAVE_NOT_IN_SLOT),false);
        else
            return;
    }

    Pet* NewSummon = new Pet;

    // petentry==0 for hunter "call pet" (current pet summoned if any)
    if(m_caster->GetTypeId() == TYPEID_PLAYER && NewSummon->LoadPetFromDB((Player*)m_caster,petentry))
    {
        if(NewSummon->getPetType()==SUMMON_PET)
        {
            // Remove Demonic Sacrifice auras (known pet)
            Unit::AuraList const& auraClassScripts = m_caster->GetAurasByType(SPELL_AURA_OVERRIDE_CLASS_SCRIPTS);
            for(Unit::AuraList::const_iterator itr = auraClassScripts.begin();itr!=auraClassScripts.end();)
            {
                if((*itr)->GetModifier()->m_miscvalue==2228)
                {
                    m_caster->RemoveAurasDueToSpell((*itr)->GetId());
                    itr = auraClassScripts.begin();
                }
                else
                    ++itr;
            }
        }

        return;
    }

    // not error in case fail hunter call pet
    if(!petentry)
    {
        delete NewSummon;
        return;
    }

    CreatureInfo const* cInfo = sCreatureStorage.LookupEntry<CreatureInfo>(petentry);

    if(!cInfo)
    {
        sLog.outError("EffectSummonPet: creature entry %u not found.",petentry);
        delete NewSummon;
        return;
    }

    Map *map = m_caster->GetMap();
    uint32 pet_number = objmgr.GeneratePetNumber();
    if(!NewSummon->Create(objmgr.GenerateLowGuid(HIGHGUID_PET), map, m_caster->GetPhaseMask(),
        petentry, pet_number))
    {
        delete NewSummon;
        return;
    }

    float px, py, pz;
    m_caster->GetClosePoint(px, py, pz, NewSummon->GetObjectSize());

    NewSummon->Relocate(px, py, pz, m_caster->GetOrientation());

    if(!NewSummon->IsPositionValid())
    {
        sLog.outError("Pet (guidlow %d, entry %d) not summoned. Suggested coordinates isn't valid (X: %f Y: %f)",
            NewSummon->GetGUIDLow(), NewSummon->GetEntry(), NewSummon->GetPositionX(), NewSummon->GetPositionY());
        delete NewSummon;
        return;
    }

    uint32 petlevel = m_caster->getLevel();
    NewSummon->setPetType(SUMMON_PET);

    uint32 faction = m_caster->getFaction();
    if(m_caster->GetTypeId() == TYPEID_UNIT)
    {
        if ( ((Creature*)m_caster)->isTotem() )
            NewSummon->GetCharmInfo()->SetReactState(REACT_AGGRESSIVE);
        else
            NewSummon->GetCharmInfo()->SetReactState(REACT_DEFENSIVE);
    }

    NewSummon->SetOwnerGUID(m_caster->GetGUID());
    NewSummon->SetCreatorGUID(m_caster->GetGUID());
    NewSummon->SetUInt32Value(UNIT_NPC_FLAGS, 0);
    NewSummon->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, faction);
    NewSummon->SetUInt32Value(UNIT_FIELD_BYTES_0, 2048);
    NewSummon->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
    NewSummon->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, time(NULL));
    NewSummon->SetUInt32Value(UNIT_FIELD_PETEXPERIENCE, 0);
    NewSummon->SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, 1000);
    NewSummon->SetUInt32Value(UNIT_CREATED_BY_SPELL, m_spellInfo->Id);

    NewSummon->GetCharmInfo()->SetPetNumber(pet_number, true);
    // this enables pet details window (Shift+P)

    // this enables popup window (pet dismiss, cancel), hunter pet additional flags set later
    if(m_caster->GetTypeId() == TYPEID_PLAYER)
        NewSummon->SetUInt32Value(UNIT_FIELD_FLAGS,UNIT_FLAG_PVP_ATTACKABLE);

    NewSummon->InitStatsForLevel(petlevel);
    NewSummon->InitPetCreateSpells();
    NewSummon->InitTalentForLevel();

    if(NewSummon->getPetType()==SUMMON_PET)
    {
        // Remove Demonic Sacrifice auras (new pet)
        Unit::AuraList const& auraClassScripts = m_caster->GetAurasByType(SPELL_AURA_OVERRIDE_CLASS_SCRIPTS);
        for(Unit::AuraList::const_iterator itr = auraClassScripts.begin();itr!=auraClassScripts.end();)
        {
            if((*itr)->GetModifier()->m_miscvalue==2228)
            {
                m_caster->RemoveAurasDueToSpell((*itr)->GetId());
                itr = auraClassScripts.begin();
            }
            else
                ++itr;
        }

        // generate new name for summon pet
        std::string new_name=objmgr.GeneratePetName(petentry);
        if(!new_name.empty())
            NewSummon->SetName(new_name);
    }
    else if(NewSummon->getPetType()==HUNTER_PET)
        NewSummon->SetByteValue(UNIT_FIELD_BYTES_2, 2, UNIT_RENAME_NOT_ALLOWED);

    NewSummon->AIM_Initialize();
    NewSummon->SetHealth(NewSummon->GetMaxHealth());
    NewSummon->SetPower(POWER_MANA, NewSummon->GetMaxPower(POWER_MANA));

    map->Add((Creature*)NewSummon);

    m_caster->SetPet(NewSummon);
    sLog.outDebug("New Pet has guid %u", NewSummon->GetGUIDLow());

    if(m_caster->GetTypeId() == TYPEID_PLAYER)
    {
        NewSummon->SavePetToDB(PET_SAVE_AS_CURRENT);
        ((Player*)m_caster)->PetSpellInitialize();
    }
}

void Spell::EffectLearnPetSpell(uint32 i)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player *_player = (Player*)m_caster;

    Pet *pet = _player->GetPet();
    if(!pet)
        return;
    if(!pet->isAlive())
        return;

    SpellEntry const *learn_spellproto = sSpellStore.LookupEntry(m_spellInfo->EffectTriggerSpell[i]);
    if(!learn_spellproto)
        return;

    pet->learnSpell(learn_spellproto->Id);

    pet->SavePetToDB(PET_SAVE_AS_CURRENT);
    _player->PetSpellInitialize();
}

void Spell::EffectTaunt(uint32 /*i*/)
{
    // this effect use before aura Taunt apply for prevent taunt already attacking target
    // for spell as marked "non effective at already attacking target"
    if(unitTarget && unitTarget->GetTypeId() != TYPEID_PLAYER)
    {
        if(unitTarget->getVictim()==m_caster)
        {
            SendCastResult(SPELL_FAILED_DONT_REPORT);
            return;
        }
    }

    // Also use this effect to set the taunter's threat to the taunted creature's highest value
    if(unitTarget->CanHaveThreatList() && unitTarget->getThreatManager().getCurrentVictim())
        unitTarget->getThreatManager().addThreat(m_caster,unitTarget->getThreatManager().getCurrentVictim()->getThreat());
}

void Spell::EffectWeaponDmg(uint32 i)
{
    if(!unitTarget)
        return;
    if(!unitTarget->isAlive())
        return;

    // multiple weapon dmg effect workaround
    // execute only the last weapon damage
    // and handle all effects at once
    for (int j = 0; j < 3; j++)
    {
        switch(m_spellInfo->Effect[j])
        {
            case SPELL_EFFECT_WEAPON_DAMAGE:
            case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
            case SPELL_EFFECT_NORMALIZED_WEAPON_DMG:
            case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
                if (j < i)                                  // we must calculate only at last weapon effect
                    return;
            break;
        }
    }

    // some spell specific modifiers
    bool customBonusDamagePercentMod = false;
    bool spellBonusNeedWeaponDamagePercentMod = false;      // if set applied weapon damage percent mode to spell bonus

    float bonusDamagePercentMod  = 1.0f;                    // applied to fixed effect damage bonus if set customBonusDamagePercentMod
    float weaponDamagePercentMod = 1.0f;                    // applied to weapon damage (and to fixed effect damage bonus if customBonusDamagePercentMod not set
    float totalDamagePercentMod  = 1.0f;                    // applied to final bonus+weapon damage
    bool normalized = false;

    int32 spell_bonus = 0;                                  // bonus specific for spell
    switch(m_spellInfo->SpellFamilyName)
    {
        case SPELLFAMILY_WARRIOR:
        {
            // Whirlwind, single only spell with 2 weapon white damage apply if have
            if(m_caster->GetTypeId()==TYPEID_PLAYER && (m_spellInfo->SpellFamilyFlags & 0x00000400000000LL))
            {
                if(((Player*)m_caster)->GetWeaponForAttack(OFF_ATTACK,true))
                    spell_bonus += m_caster->CalculateDamage (OFF_ATTACK, normalized);
            }
            // Devastate bonus and sunder armor refresh
            else if(m_spellInfo->SpellVisual[0] == 671 && m_spellInfo->SpellIconID == 1508)
            {
                uint32 stack = 0;
                // Need refresh all Sunder Armor auras from this caster
                Unit::AuraMap& suAuras = unitTarget->GetAuras();
                for(Unit::AuraMap::iterator itr = suAuras.begin(); itr != suAuras.end(); ++itr)
                {
                    SpellEntry const *spellInfo = (*itr).second->GetSpellProto();
                    if( spellInfo->SpellFamilyName == SPELLFAMILY_WARRIOR &&
                        spellInfo->SpellFamilyFlags & 0x0000000000004000LL &&
                        (*itr).second->GetCasterGUID() == m_caster->GetGUID())
                    {
                        (*itr).second->RefreshAura();
                        stack = (*itr).second->GetStackAmount();
                    }
                }
                if (stack)
                    spell_bonus += stack * CalculateDamage(2, unitTarget);
            }
            break;
        }
        case SPELLFAMILY_ROGUE:
        {
            // Ambush
            if(m_spellInfo->SpellFamilyFlags & 0x00000200LL)
            {
                customBonusDamagePercentMod = true;

                bonusDamagePercentMod = 2.75f;               // 275%
            }
            // Mutilate (for each hand)
            else if(m_spellInfo->SpellFamilyFlags & 0x600000000LL)
            {
                bool found = false;
                // fast check
                if(unitTarget->HasAuraState(AURA_STATE_DEADLY_POISON))
                    found = true;
                // full aura scan
                else
                {
                    Unit::AuraMap const& auras = unitTarget->GetAuras();
                    for(Unit::AuraMap::const_iterator itr = auras.begin(); itr!=auras.end(); ++itr)
                    {
                        if(itr->second->GetSpellProto()->Dispel == DISPEL_POISON)
                        {
                            found = true;
                            break;
                        }
                    }
                }

                if(found)
                    totalDamagePercentMod *= 1.2f;          // 120% if poisoned
            }
            break;
        }
        case SPELLFAMILY_PALADIN:
        {
            // Seal of Command - receive benefit from Spell Damage and Healing
            if(m_spellInfo->SpellFamilyFlags & 0x00000002000000LL)
            {
                spellBonusNeedWeaponDamagePercentMod = true;// apply weaponDamagePercentMod to spell_bonus (and then to all bonus, fixes and weapon already have applied)
                spell_bonus += int32(0.23f*m_caster->SpellBaseDamageBonus(GetSpellSchoolMask(m_spellInfo)));
                spell_bonus += int32(0.29f*m_caster->SpellBaseDamageBonusForVictim(GetSpellSchoolMask(m_spellInfo), unitTarget));
            }
            break;
        }
        case SPELLFAMILY_SHAMAN:
        {
            // Skyshatter Harness item set bonus
            // Stormstrike
            if(m_spellInfo->SpellFamilyFlags & 0x001000000000LL)
            {
                Unit::AuraList const& m_OverrideClassScript = m_caster->GetAurasByType(SPELL_AURA_OVERRIDE_CLASS_SCRIPTS);
                for(Unit::AuraList::const_iterator i = m_OverrideClassScript.begin(); i != m_OverrideClassScript.end(); ++i)
                {
                    // Stormstrike AP Buff
                    if ( (*i)->GetModifier()->m_miscvalue == 5634 )
                    {
                        m_caster->CastSpell(m_caster,38430,true,NULL,*i);
                        break;
                    }
                }
            }
        }
    }

    int32 fixed_bonus = 0;
    for (int j = 0; j < 3; j++)
    {
        switch(m_spellInfo->Effect[j])
        {
            case SPELL_EFFECT_WEAPON_DAMAGE:
            case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
                fixed_bonus += CalculateDamage(j,unitTarget);
                break;
            case SPELL_EFFECT_NORMALIZED_WEAPON_DMG:
                fixed_bonus += CalculateDamage(j,unitTarget);
                normalized = true;
                break;
            case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
                weaponDamagePercentMod *= float(CalculateDamage(j,unitTarget)) / 100.0f;

                // applied only to prev.effects fixed damage
                if(customBonusDamagePercentMod)
                    fixed_bonus = int32(fixed_bonus*bonusDamagePercentMod);
                else
                    fixed_bonus = int32(fixed_bonus*weaponDamagePercentMod);
                break;
            default:
                break;                                      // not weapon damage effect, just skip
        }
    }

    // apply weaponDamagePercentMod to spell bonus also
    if(spellBonusNeedWeaponDamagePercentMod)
        spell_bonus = int32(spell_bonus*weaponDamagePercentMod);

    // non-weapon damage
    int32 bonus = spell_bonus + fixed_bonus;

    // apply to non-weapon bonus weapon total pct effect, weapon total flat effect included in weapon damage
    if(bonus)
    {
        UnitMods unitMod;
        switch(m_attackType)
        {
            default:
            case BASE_ATTACK:   unitMod = UNIT_MOD_DAMAGE_MAINHAND; break;
            case OFF_ATTACK:    unitMod = UNIT_MOD_DAMAGE_OFFHAND;  break;
            case RANGED_ATTACK: unitMod = UNIT_MOD_DAMAGE_RANGED;   break;
        }

        float weapon_total_pct  = m_caster->GetModifierValue(unitMod, TOTAL_PCT);
        bonus = int32(bonus*weapon_total_pct);
    }

    // + weapon damage with applied weapon% dmg to base weapon damage in call
    bonus += int32(m_caster->CalculateDamage(m_attackType, normalized)*weaponDamagePercentMod);

    // total damage
    bonus = int32(bonus*totalDamagePercentMod);

    // prevent negative damage
    uint32 eff_damage = uint32(bonus > 0 ? bonus : 0);

    // Add melee damage bonuses (also check for negative)
    m_caster->MeleeDamageBonus(unitTarget, &eff_damage, m_attackType, m_spellInfo);
    m_damage+= eff_damage;

    // Hemorrhage
    if(m_spellInfo->SpellFamilyName==SPELLFAMILY_ROGUE && (m_spellInfo->SpellFamilyFlags & 0x2000000))
    {
        if(m_caster->GetTypeId()==TYPEID_PLAYER)
            ((Player*)m_caster)->AddComboPoints(unitTarget, 1);
    }

    // Mangle (Cat): CP
    if(m_spellInfo->SpellFamilyName==SPELLFAMILY_DRUID && (m_spellInfo->SpellFamilyFlags==0x0000040000000000LL))
    {
        if(m_caster->GetTypeId()==TYPEID_PLAYER)
            ((Player*)m_caster)->AddComboPoints(unitTarget,1);
    }

    // take ammo
    if(m_attackType == RANGED_ATTACK && m_caster->GetTypeId() == TYPEID_PLAYER)
    {
        Item *pItem = ((Player*)m_caster)->GetWeaponForAttack( RANGED_ATTACK );

        // wands don't have ammo
        if(!pItem  || pItem->IsBroken() || pItem->GetProto()->SubClass==ITEM_SUBCLASS_WEAPON_WAND)
            return;

        if( pItem->GetProto()->InventoryType == INVTYPE_THROWN )
        {
            if(pItem->GetMaxStackCount()==1)
            {
                // decrease durability for non-stackable throw weapon
                ((Player*)m_caster)->DurabilityPointLossForEquipSlot(EQUIPMENT_SLOT_RANGED);
            }
            else
            {
                // decrease items amount for stackable throw weapon
                uint32 count = 1;
                ((Player*)m_caster)->DestroyItemCount( pItem, count, true);
            }
        }
        else if(uint32 ammo = ((Player*)m_caster)->GetUInt32Value(PLAYER_AMMO_ID))
            ((Player*)m_caster)->DestroyItemCount(ammo, 1, true);
    }
}

void Spell::EffectThreat(uint32 /*i*/)
{
    if(!unitTarget || !unitTarget->isAlive() || !m_caster->isAlive())
        return;

    if(!unitTarget->CanHaveThreatList())
        return;

    // Sunder Armor (including Devastate)
    if( m_spellInfo->SpellFamilyFlags & 0x4000 )
        damage+= m_caster->GetTotalAttackPowerValue(BASE_ATTACK)*0.05f;

    unitTarget->AddThreat(m_caster, float(damage));
}

void Spell::EffectHealMaxHealth(uint32 /*i*/)
{
    if(!unitTarget)
        return;
    if(!unitTarget->isAlive())
        return;

    uint32 heal = m_caster->GetMaxHealth();

    m_healing+=heal;
}

void Spell::EffectInterruptCast(uint32 /*i*/)
{
    if(!unitTarget)
        return;
    if(!unitTarget->isAlive())
        return;

    // TODO: not all spells that used this effect apply cooldown at school spells
    // also exist case: apply cooldown to interrupted cast only and to all spells
    for (uint32 i = CURRENT_FIRST_NON_MELEE_SPELL; i < CURRENT_MAX_SPELL; i++)
    {
        if (unitTarget->m_currentSpells[i])
        {
            // check if we can interrupt spell
            if ( unitTarget->m_currentSpells[i]->m_spellInfo->InterruptFlags & SPELL_INTERRUPT_FLAG_INTERRUPT && unitTarget->m_currentSpells[i]->m_spellInfo->PreventionType == SPELL_PREVENTION_TYPE_SILENCE )
            {
                unitTarget->ProhibitSpellScholl(GetSpellSchoolMask(unitTarget->m_currentSpells[i]->m_spellInfo), GetSpellDuration(m_spellInfo));
                unitTarget->InterruptSpell(i,false);
            }
        }
    }
}

void Spell::EffectSummonObjectWild(uint32 i)
{
    uint32 gameobject_id = m_spellInfo->EffectMiscValue[i];

    GameObject* pGameObj = new GameObject;

    WorldObject* target = focusObject;
    if( !target )
        target = m_caster;

    float x,y,z;
    if(m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION)
    {
        x = m_targets.m_destX;
        y = m_targets.m_destY;
        z = m_targets.m_destZ;
    }
    else
        m_caster->GetClosePoint(x,y,z,DEFAULT_WORLD_OBJECT_SIZE);

    Map *map = target->GetMap();

    if(!pGameObj->Create(objmgr.GenerateLowGuid(HIGHGUID_GAMEOBJECT), gameobject_id, map,
        m_caster->GetPhaseMask(), x, y, z, target->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, 100, 1))
    {
        delete pGameObj;
        return;
    }

    int32 duration = GetSpellDuration(m_spellInfo);
    pGameObj->SetRespawnTime(duration > 0 ? duration/IN_MILISECONDS : 0);
    pGameObj->SetSpellId(m_spellInfo->Id);

    if(pGameObj->GetGoType() != GAMEOBJECT_TYPE_FLAGDROP)   // make dropped flag clickable for other players (not set owner guid (created by) for this)...
        m_caster->AddGameObject(pGameObj);
    map->Add(pGameObj);

    if(pGameObj->GetMapId() == 489 && pGameObj->GetGoType() == GAMEOBJECT_TYPE_FLAGDROP)  //WS
    {
        if(m_caster->GetTypeId() == TYPEID_PLAYER)
        {
            Player *pl = (Player*)m_caster;
            BattleGround* bg = ((Player *)m_caster)->GetBattleGround();
            if(bg && bg->GetTypeID()==BATTLEGROUND_WS && bg->GetStatus() == STATUS_IN_PROGRESS)
            {
                 uint32 team = ALLIANCE;

                 if(pl->GetTeam() == team)
                     team = HORDE;

                ((BattleGroundWS*)bg)->SetDroppedFlagGUID(pGameObj->GetGUID(),team);
            }
        }
    }

    if(pGameObj->GetMapId() == 566 && pGameObj->GetGoType() == GAMEOBJECT_TYPE_FLAGDROP)  //EY
    {
        if(m_caster->GetTypeId() == TYPEID_PLAYER)
        {
            BattleGround* bg = ((Player *)m_caster)->GetBattleGround();
            if(bg && bg->GetTypeID()==BATTLEGROUND_EY && bg->GetStatus() == STATUS_IN_PROGRESS)
            {
                ((BattleGroundEY*)bg)->SetDroppedFlagGUID(pGameObj->GetGUID());
            }
        }
    }

    if(uint32 linkedEntry = pGameObj->GetLinkedGameObjectEntry())
    {
        GameObject* linkedGO = new GameObject;
        if(linkedGO->Create(objmgr.GenerateLowGuid(HIGHGUID_GAMEOBJECT), linkedEntry, map,
            m_caster->GetPhaseMask(), x, y, z, target->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, 100, 1))
        {
            linkedGO->SetRespawnTime(duration > 0 ? duration/IN_MILISECONDS : 0);
            linkedGO->SetSpellId(m_spellInfo->Id);

            m_caster->AddGameObject(linkedGO);
            map->Add(linkedGO);
        }
        else
        {
            delete linkedGO;
            linkedGO = NULL;
            return;
        }
    }
}

void Spell::EffectScriptEffect(uint32 effIndex)
{
    // TODO: we must implement hunter pet summon at login there (spell 6962)

    switch(m_spellInfo->SpellFamilyName)
    {
        case SPELLFAMILY_GENERIC:
        {
            switch(m_spellInfo->Id)
            {
                // PX-238 Winter Wondervolt TRAP
                case 26275:
                {
                    uint32 spells[4] = { 26272, 26157, 26273, 26274 };

                    // check presence
                    for(int j = 0; j < 4; ++j)
                        if(unitTarget->HasAura(spells[j],0))
                            return;

                    // select spell
                    uint32 iTmpSpellId = spells[urand(0,3)];

                    // cast
                    unitTarget->CastSpell(unitTarget, iTmpSpellId, true);
                    return;
                }
                // Bending Shinbone
                case 8856:
                {
                    if(!itemTarget && m_caster->GetTypeId()!=TYPEID_PLAYER)
                        return;

                    uint32 spell_id = 0;
                    switch(urand(1,5))
                    {
                    case 1:  spell_id = 8854; break;
                    default: spell_id = 8855; break;
                    }

                    m_caster->CastSpell(m_caster,spell_id,true,NULL);
                    return;
                }
                // Brittle Armor - need remove one 24575 Brittle Armor aura
                case 24590:
                    unitTarget->RemoveSingleSpellAurasFromStack(24575);
                    return;
                // Mercurial Shield - need remove one 26464 Mercurial Shield aura
                case 26465:
                    unitTarget->RemoveSingleSpellAurasFromStack(26464);
                    return;
                // Orb teleport spells
                case 25140:
                case 25143:
                case 25650:
                case 25652:
                case 29128:
                case 29129:
                case 35376:
                case 35727:
                {
                    if(!unitTarget)
                        return;

                    uint32 spellid;
                    switch(m_spellInfo->Id)
                    {
                        case 25140: spellid =  32571; break;
                        case 25143: spellid =  32572; break;
                        case 25650: spellid =  30140; break;
                        case 25652: spellid =  30141; break;
                        case 29128: spellid =  32568; break;
                        case 29129: spellid =  32569; break;
                        case 35376: spellid =  25649; break;
                        case 35727: spellid =  35730; break;
                        default:
                            return;
                    }

                    unitTarget->CastSpell(unitTarget,spellid,false);
                    return;
                }
                // Shadow Flame (All script effects, not just end ones to prevent player from dodging the last triggered spell)
                case 22539:
                case 22972:
                case 22975:
                case 22976:
                case 22977:
                case 22978:
                case 22979:
                case 22980:
                case 22981:
                case 22982:
                case 22983:
                case 22984:
                case 22985:
                {
                    if(!unitTarget || !unitTarget->isAlive())
                        return;

                    // Onyxia Scale Cloak
                    if(unitTarget->GetDummyAura(22683))
                        return;

                    // Shadow Flame
                    m_caster->CastSpell(unitTarget, 22682, true);
                    return;
                }
                // Summon Black Qiraji Battle Tank
                case 26656:
                {
                    if(!unitTarget)
                        return;

                    // Prevent stacking of mounts
                    unitTarget->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);

                    // Two separate mounts depending on area id (allows use both in and out of specific instance)
                    if (unitTarget->GetAreaId() == 3428)
                        unitTarget->CastSpell(unitTarget, 25863, false);
                    else
                        unitTarget->CastSpell(unitTarget, 26655, false);
                    return;
                }
                // Piccolo of the Flaming Fire
                case 17512:
                {
                    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
                        return;
                    unitTarget->HandleEmoteCommand(EMOTE_STATE_DANCE);
                    return;
                }
                // Escape artist
                case 20589:
                {
                    m_caster->RemoveSpellsCausingAura(SPELL_AURA_MOD_ROOT);
                    m_caster->RemoveSpellsCausingAura(SPELL_AURA_MOD_DECREASE_SPEED);
                    return;
                }
                // Mirren's Drinking Hat
                case 29830:
                {
                    uint32 item = 0;
                    switch ( urand(1,6) )
                    {
                        case 1:case 2:case 3:
                            item = 23584;break;             // Loch Modan Lager
                        case 4:case 5:
                            item = 23585;break;             // Stouthammer Lite
                        case 6:
                            item = 23586;break;             // Aerie Peak Pale Ale
                    }
                    if (item)
                        DoCreateItem(effIndex,item);
                    break;
                }
                // Improved Sprint
                case 30918:
                {
                    // Removes snares and roots.
                    uint32 mechanic_mask = (1<<MECHANIC_ROOT) | (1<<MECHANIC_SNARE);
                    Unit::AuraMap& Auras = unitTarget->GetAuras();
                    for(Unit::AuraMap::iterator iter = Auras.begin(), next; iter != Auras.end(); iter = next)
                    {
                        next = iter;
                        ++next;
                        Aura *aur = iter->second;
                        if (!aur->IsPositive())             //only remove negative spells
                        {
                            // check for mechanic mask
                            if(GetSpellMechanicMask(aur->GetSpellProto(), aur->GetEffIndex()) & mechanic_mask)
                            {
                                unitTarget->RemoveAurasDueToSpell(aur->GetId());
                                if(Auras.empty())
                                    break;
                                else
                                    next = Auras.begin();
                            }
                        }
                    }
                    break;
                }
                // Flame Crash
                case 41126:
                {
                    if(!unitTarget)
                        return;

                    unitTarget->CastSpell(unitTarget, 41131, true);
                    break;
                }
                // Force Cast - Portal Effect: Sunwell Isle
                case 44876:
                {
                    if(!unitTarget)
                        return;

                    unitTarget->CastSpell(unitTarget, 44870, true);
                    break;
                }
                // Goblin Weather Machine
                case 46203:
                {
                    if(!unitTarget)
                        return;

                    uint32 spellId;
                    switch(rand()%4)
                    {
                        case 0: spellId = 46740; break;
                        case 1: spellId = 46739; break;
                        case 2: spellId = 46738; break;
                        case 3: spellId = 46736; break;
                    }
                    unitTarget->CastSpell(unitTarget, spellId, true);
                    break;
                }
                //5,000 Gold
                case 46642:
                {
                    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
                        return;

                    ((Player*)unitTarget)->ModifyMoney(50000000);

                    break;
                }
                // Emblazon Runeblade
                case 51770:
                {
                    if(!unitTarget)
                        return;

                    unitTarget->CastSpell(unitTarget,51771,false);
                    break;
                }
                // Death Gate
                case 52751:
                {
                    if(!unitTarget || unitTarget->getClass() != CLASS_DEATH_KNIGHT)
                        return;
                    // triggered spell is stored in m_spellInfo->EffectBasePoints[0]
                    unitTarget->CastSpell(unitTarget, damage, false);
                    break;
                }
                // random spell learn instead placeholder
                case 60893:                                 // Northrend Alchemy Research
                case 61177:                                 // Northrend Inscription Research
                case 61288:                                 // Minor Inscription Research
                case 61756:                                 // Northrend Inscription Research (FAST QA VERSION)
                {
                    if(!IsExplicitDiscoverySpell(m_spellInfo))
                    {
                        sLog.outError("Wrong explicit discovery spell %u structure, or outdated...",m_spellInfo->Id);
                        return;
                    }

                    if(m_caster->GetTypeId()!=TYPEID_PLAYER)
                        return;
                    Player* player = (Player*)m_caster;

                    // need replace effect 0 item by loot
                    uint32 reagent_id = m_spellInfo->EffectItemType[0];

                    if(!player->HasItemCount(reagent_id,1))
                        return;

                    // remove reagent
                    uint32 count = 1;
                    player->DestroyItemCount (reagent_id,count,true);

                    // create some random items
                    player->AutoStoreLoot(m_spellInfo->Id,LootTemplates_Spell);

                    // learn random explicit discovery recipe (if any)
                    if(uint32 discoveredSpell = GetExplicitDiscoverySpell(m_spellInfo->Id, player))
                        player->learnSpell(discoveredSpell,false);
                    return;
                }
            }
            break;
        }
        case SPELLFAMILY_WARLOCK:
        {
            switch(m_spellInfo->Id)
            {
                // Healthstone creating spells
                case  6201:
                case  6202:
                case  5699:
                case 11729:
                case 11730:
                case 27230:
                case 47871:
                case 47878:
                {
                    uint32 itemtype;
                    uint32 rank = 0;
                    Unit::AuraList const& mDummyAuras = unitTarget->GetAurasByType(SPELL_AURA_DUMMY);
                    for(Unit::AuraList::const_iterator i = mDummyAuras.begin();i != mDummyAuras.end(); ++i)
                    {
                        if((*i)->GetId() == 18692)
                        {
                            rank = 1;
                            break;
                        }
                        else if((*i)->GetId() == 18693)
                        {
                            rank = 2;
                            break;
                        }
                    }

                    static uint32 const itypes[8][3] = {
                        { 5512,19004,19005},                // Minor Healthstone
                        { 5511,19006,19007},                // Lesser Healthstone
                        { 5509,19008,19009},                // Healthstone
                        { 5510,19010,19011},                // Greater Healthstone
                        { 9421,19012,19013},                // Major Healthstone
                        {22103,22104,22105},                // Master Healthstone
                        {36889,36890,36891},                // Demonic Healthstone
                        {36892,36893,36894}                 // Fel Healthstone
                    };

                    switch(m_spellInfo->Id)
                    {
                        case  6201:
                            itemtype=itypes[0][rank];break; // Minor Healthstone
                        case  6202:
                            itemtype=itypes[1][rank];break; // Lesser Healthstone
                        case  5699:
                            itemtype=itypes[2][rank];break; // Healthstone
                        case 11729:
                            itemtype=itypes[3][rank];break; // Greater Healthstone
                        case 11730:
                            itemtype=itypes[4][rank];break; // Major Healthstone
                        case 27230:
                            itemtype=itypes[5][rank];break; // Master Healthstone
                        case 47871:
                            itemtype=itypes[6][rank];break; // Demonic Healthstone
                        case 47878:
                            itemtype=itypes[7][rank];break; // Fel Healthstone
                        default:
                            return;
                    }
                    DoCreateItem( effIndex, itemtype );
                    return;
                }
            }
            break;
        }
        case SPELLFAMILY_PRIEST:
        {
            switch(m_spellInfo->Id)
            {
                // Pain and Suffering
                case 47948:
                {
                    if (!unitTarget)
                        return;
                    // Refresh Shadow Word: Pain on target
                    Unit::AuraMap& auras = unitTarget->GetAuras();
                    for(Unit::AuraMap::iterator itr = auras.begin(); itr != auras.end(); ++itr)
                    {
                        SpellEntry const *spellInfo = (*itr).second->GetSpellProto();
                        if( spellInfo->SpellFamilyName == SPELLFAMILY_PRIEST &&
                            spellInfo->SpellFamilyFlags &  0x0000000000008000LL &&
                            (*itr).second->GetCasterGUID() == m_caster->GetGUID())
                        {
                            (*itr).second->RefreshAura();
                            return;
                        }
                    }
                    return;
                }
                default:
                    break;
            }
            break;
        }
        case SPELLFAMILY_HUNTER:
        {
            switch(m_spellInfo->Id)
            {
                // Chimera Shot
                case 53209:
                {
                    uint32 spellId = 0;
                    int32 basePoint = 0;
                    Unit::AuraMap& Auras = unitTarget->GetAuras();
                    for(Unit::AuraMap::iterator i = Auras.begin(); i != Auras.end(); ++i)
                    {
                        Aura *aura = (*i).second;
                        if (aura->GetCasterGUID() != m_caster->GetGUID())
                            continue;
                        // Search only Serpent Sting, Viper Sting, Scorpid Sting auras
                        uint64 familyFlag = aura->GetSpellProto()->SpellFamilyFlags;
                        if (!(familyFlag & 0x000000800000C000LL))
                            continue;
                        // Refresh aura duration
                        aura->RefreshAura();

                        // Serpent Sting - Instantly deals 40% of the damage done by your Serpent Sting.
                        if (familyFlag & 0x0000000000004000LL && aura->GetEffIndex() == 0)
                        {
                            spellId = 53353; // 53353 Chimera Shot - Serpent
                            basePoint = aura->GetModifier()->m_amount * 5 * 40 / 100;
                        }
                        // Viper Sting - Instantly restores mana to you equal to 60% of the total amount drained by your Viper Sting.
                        if (familyFlag & 0x0000008000000000LL && aura->GetEffIndex() == 0)
                        {
                            spellId = 53358; // 53358 Chimera Shot - Viper
                            basePoint = aura->GetModifier()->m_amount * 4 * 60 / 100;
                        }
                        // Scorpid Sting - Attempts to Disarm the target for 10 sec. This effect cannot occur more than once per 1 minute.
                        if (familyFlag & 0x0000000000008000LL)
                            spellId = 53359; // 53359 Chimera Shot - Scorpid
                        // ?? nothing say in spell desc (possibly need addition check)
                        //if (familyFlag & 0x0000010000000000LL || // dot
                        //    familyFlag & 0x0000100000000000LL)   // stun
                        //{
                        //    spellId = 53366; // 53366 Chimera Shot - Wyvern
                        //}
                    }
                    if (spellId)
                        m_caster->CastCustomSpell(unitTarget, spellId, &basePoint, 0, 0, false);
                    return;
                }
                default:
                    break;
            }
            break;
        }
        case SPELLFAMILY_PALADIN:
        {
            // Judgement
            if (m_spellInfo->SpellFamilyFlags & 0x0000000000800000LL)
            {
                if(!unitTarget || !unitTarget->isAlive())
                    return;
                uint32 spellId1 = 0;
                uint32 spellId2 = 0;

                // Judgement self add switch
                switch (m_spellInfo->Id)
                {
                    case 41467: break;                      // Judgement
                    case 53407: spellId1 = 20184; break;    // Judgement of Justice
                    case 20271:                             // Judgement of Light
                    case 57774: spellId1 = 20185; break;    // Judgement of Light
                    case 53408: spellId1 = 20186; break;    // Judgement of Wisdom
                    default:
                        return;
                }
				//Judgement of the wise
				int chance =0;
				int32 aur =0;
				if(m_caster->HasAura(31876)) {chance=33;aur=31876;}
				else if (m_caster->HasAura(31877)) {chance=66;aur=31877;}
				else if (m_caster->HasAura(31878)) {chance=100;aur=31878;}
				if(chance != 0)
				{
					int32 mana15 = m_caster->GetCreateMana() * 0.15;
					if(urand(1,100) <= chance) m_caster->CastCustomSpell(m_caster,31930,&mana15,false,false,true,NULL,m_caster->GetDummyAura(aur),m_caster->GetGUID());
					int countera =0;
					Group *pGroup = NULL;
              		if (m_caster->GetTypeId() == TYPEID_PLAYER)
						pGroup = ((Player*)m_caster)->GetGroup();
					float radius=100.0f;
					if( pGroup)
					{
						for(GroupReference *itr = pGroup->GetFirstMember(); itr != NULL; itr = itr->next())
						{
							Player* Target = itr->getSource();
							if(Target && Target->isAlive() &&  m_caster->IsFriendlyTo(Target) && m_caster != Target && Target->getPowerType() == POWER_MANA && countera <=10)
							{
								countera++;
								m_caster->CastSpell(Target,57669,true,NULL,m_caster->GetDummyAura(aur),m_caster->GetGUID());
							}
						}
					}
				}

                // all seals have aura dummy in 2 effect
                Unit::AuraList const& m_dummyAuras = m_caster->GetAurasByType(SPELL_AURA_DUMMY);
                for(Unit::AuraList::const_iterator itr = m_dummyAuras.begin(); itr != m_dummyAuras.end(); ++itr)
                {
                    SpellEntry const *spellInfo = (*itr)->GetSpellProto();
                    // search seal (all seals have judgement's aura dummy spell id in 2 effect
                    if ((*itr)->GetEffIndex() != 2 || !spellInfo || !IsSealSpell(spellInfo))
                        continue;
                    spellId2 = (*itr)->GetModifier()->m_amount;
                    SpellEntry const *judge = sSpellStore.LookupEntry(spellId2);
                    if (!judge)
                        continue;
                    break;
                }
                if (spellId1)
                    m_caster->CastSpell(unitTarget, spellId1, true);
                if (spellId2)
                    m_caster->CastSpell(unitTarget, spellId2, true);
                return;
            }
        }
        case SPELLFAMILY_POTION:
        {
            switch(m_spellInfo->Id)
            {
                // Dreaming Glory
                case 28698:
                {
                    if(!unitTarget)
                        return;
                    unitTarget->CastSpell(unitTarget, 28694, true);
                    break;
                }
                // Netherbloom
                case 28702:
                {
                    if(!unitTarget)
                        return;
                    // 25% chance of casting a random buff
                    if(roll_chance_i(75))
                        return;

                    // triggered spells are 28703 to 28707
                    // Note: some sources say, that there was the possibility of
                    //       receiving a debuff. However, this seems to be removed by a patch.
                    const uint32 spellid = 28703;

                    // don't overwrite an existing aura
                    for(uint8 i=0; i<5; i++)
                        if(unitTarget->HasAura(spellid+i, 0))
                            return;
                    unitTarget->CastSpell(unitTarget, spellid+urand(0, 4), true);
                    break;
                }

                // Nightmare Vine
                case 28720:
                {
                    if(!unitTarget)
                        return;
                    // 25% chance of casting Nightmare Pollen
                    if(roll_chance_i(75))
                        return;
                    unitTarget->CastSpell(unitTarget, 28721, true);
                    break;
                }
            }
            break;
        }
    }

    // normal DB scripted effect
    if(!unitTarget)
        return;

    sLog.outDebug("Spell ScriptStart spellid %u in EffectScriptEffect ", m_spellInfo->Id);
    sWorld.ScriptsStart(sSpellScripts, m_spellInfo->Id, m_caster, unitTarget);
}

void Spell::EffectSanctuary(uint32 /*i*/)
{
    if(!unitTarget)
        return;
    //unitTarget->CombatStop();

    unitTarget->CombatStop();
    unitTarget->getHostilRefManager().deleteReferences();   // stop all fighting
    // Vanish allows to remove all threat and cast regular stealth so other spells can be used
    if(m_spellInfo->SpellFamilyName == SPELLFAMILY_ROGUE && (m_spellInfo->SpellFamilyFlags & SPELLFAMILYFLAG_ROGUE_VANISH))
    {
        ((Player *)m_caster)->RemoveSpellsCausingAura(SPELL_AURA_MOD_ROOT);
    }
}

void Spell::EffectAddComboPoints(uint32 /*i*/)
{
    if(!unitTarget)
        return;

    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    if(damage <= 0)
        return;

    ((Player*)m_caster)->AddComboPoints(unitTarget, damage);
}

void Spell::EffectDuel(uint32 i)
{
    if(!m_caster || !unitTarget || m_caster->GetTypeId() != TYPEID_PLAYER || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player *caster = (Player*)m_caster;
    Player *target = (Player*)unitTarget;

    // caster or target already have requested duel
    if( caster->duel || target->duel || !target->GetSocial() || target->GetSocial()->HasIgnore(caster->GetGUIDLow()) )
        return;

    // Players can only fight a duel with each other outside (=not inside dungeons and not in capital cities)
    // Don't have to check the target's map since you cannot challenge someone across maps
    uint32 mapid = caster->GetMapId();
    if( mapid != 0 && mapid != 1 && mapid != 530 && mapid != 571 && mapid != 609)
    {
        SendCastResult(SPELL_FAILED_NO_DUELING);            // Dueling isn't allowed here
        return;
    }

    AreaTableEntry const* casterAreaEntry = GetAreaEntryByAreaID(caster->GetZoneId());
    if(casterAreaEntry && (casterAreaEntry->flags & AREA_FLAG_CAPITAL) )
    {
        SendCastResult(SPELL_FAILED_NO_DUELING);            // Dueling isn't allowed here
        return;
    }

    AreaTableEntry const* targetAreaEntry = GetAreaEntryByAreaID(target->GetZoneId());
    if(targetAreaEntry && (targetAreaEntry->flags & AREA_FLAG_CAPITAL) )
    {
        SendCastResult(SPELL_FAILED_NO_DUELING);            // Dueling isn't allowed here
        return;
    }

    //CREATE DUEL FLAG OBJECT
    GameObject* pGameObj = new GameObject;

    uint32 gameobject_id = m_spellInfo->EffectMiscValue[i];

    Map *map = m_caster->GetMap();
    if(!pGameObj->Create(objmgr.GenerateLowGuid(HIGHGUID_GAMEOBJECT), gameobject_id,
        map, m_caster->GetPhaseMask(),
        m_caster->GetPositionX()+(unitTarget->GetPositionX()-m_caster->GetPositionX())/2 ,
        m_caster->GetPositionY()+(unitTarget->GetPositionY()-m_caster->GetPositionY())/2 ,
        m_caster->GetPositionZ(),
        m_caster->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, 0, 1))
    {
        delete pGameObj;
        return;
    }

    pGameObj->SetUInt32Value(GAMEOBJECT_FACTION, m_caster->getFaction() );
    pGameObj->SetUInt32Value(GAMEOBJECT_LEVEL, m_caster->getLevel()+1 );
    int32 duration = GetSpellDuration(m_spellInfo);
    pGameObj->SetRespawnTime(duration > 0 ? duration/IN_MILISECONDS : 0);
    pGameObj->SetSpellId(m_spellInfo->Id);

    m_caster->AddGameObject(pGameObj);
    map->Add(pGameObj);
    //END

    // Send request
    WorldPacket data(SMSG_DUEL_REQUESTED, 16);
    data << pGameObj->GetGUID();
    data << caster->GetGUID();
    caster->GetSession()->SendPacket(&data);
    target->GetSession()->SendPacket(&data);

    // create duel-info
    DuelInfo *duel   = new DuelInfo;
    duel->initiator  = caster;
    duel->opponent   = target;
    duel->startTime  = 0;
    duel->startTimer = 0;
    caster->duel     = duel;

    DuelInfo *duel2   = new DuelInfo;
    duel2->initiator  = caster;
    duel2->opponent   = caster;
    duel2->startTime  = 0;
    duel2->startTimer = 0;
    target->duel      = duel2;

    caster->SetUInt64Value(PLAYER_DUEL_ARBITER,pGameObj->GetGUID());
    target->SetUInt64Value(PLAYER_DUEL_ARBITER,pGameObj->GetGUID());
}

void Spell::EffectStuck(uint32 /*i*/)
{
    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    if(!sWorld.getConfig(CONFIG_CAST_UNSTUCK))
        return;

    Player* pTarget = (Player*)unitTarget;

    sLog.outDebug("Spell Effect: Stuck");
    sLog.outDetail("Player %s (guid %u) used auto-unstuck future at map %u (%f, %f, %f)", pTarget->GetName(), pTarget->GetGUIDLow(), m_caster->GetMapId(), m_caster->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ());

    if(pTarget->isInFlight())
        return;

    // homebind location is loaded always
    pTarget->TeleportTo(pTarget->m_homebindMapId,pTarget->m_homebindX,pTarget->m_homebindY,pTarget->m_homebindZ,pTarget->GetOrientation(), (unitTarget==m_caster ? TELE_TO_SPELL : 0));

    // Stuck spell trigger Hearthstone cooldown
    SpellEntry const *spellInfo = sSpellStore.LookupEntry(8690);
    if(!spellInfo)
        return;
    Spell spell(pTarget,spellInfo,true,0);
    spell.SendSpellCooldown();
}

void Spell::EffectSummonPlayer(uint32 /*i*/)
{
    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    // Evil Twin (ignore player summon, but hide this for summoner)
    if(unitTarget->GetDummyAura(23445))
        return;

    float x,y,z;
    m_caster->GetClosePoint(x,y,z,unitTarget->GetObjectSize());

    ((Player*)unitTarget)->SetSummonPoint(m_caster->GetMapId(),x,y,z);

    WorldPacket data(SMSG_SUMMON_REQUEST, 8+4+4);
    data << uint64(m_caster->GetGUID());                    // summoner guid
    data << uint32(m_caster->GetZoneId());                  // summoner zone
    data << uint32(MAX_PLAYER_SUMMON_DELAY*IN_MILISECONDS); // auto decline after msecs
    ((Player*)unitTarget)->GetSession()->SendPacket(&data);
}

static ScriptInfo generateActivateCommand()
{
    ScriptInfo si;
    si.command = SCRIPT_COMMAND_ACTIVATE_OBJECT;
    return si;
}

void Spell::EffectActivateObject(uint32 effect_idx)
{
    if(!gameObjTarget)
        return;

    static ScriptInfo activateCommand = generateActivateCommand();

    int32 delay_secs = m_spellInfo->EffectMiscValue[effect_idx];

    sWorld.ScriptCommandStart(activateCommand, delay_secs, m_caster, gameObjTarget);
}

void Spell::EffectApplyGlyph(uint32 i)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player *player = (Player*)m_caster;

    // apply new one
    if(uint32 glyph = m_spellInfo->EffectMiscValue[i])
    {
        if(GlyphPropertiesEntry const *gp = sGlyphPropertiesStore.LookupEntry(glyph))
        {
            if(GlyphSlotEntry const *gs = sGlyphSlotStore.LookupEntry(player->GetGlyphSlot(m_glyphIndex)))
            {
                if(gp->TypeFlags != gs->TypeFlags)
                {
                    SendCastResult(SPELL_FAILED_INVALID_GLYPH);
                    return;                                 // glyph slot mismatch
                }
            }

            // remove old glyph
            if(uint32 oldglyph = player->GetGlyph(m_glyphIndex))
            {
                if(GlyphPropertiesEntry const *old_gp = sGlyphPropertiesStore.LookupEntry(oldglyph))
                {
                    player->RemoveAurasDueToSpell(old_gp->SpellId);
                    player->SetGlyph(m_glyphIndex, 0);
                }
            }

            player->CastSpell(m_caster, gp->SpellId, true);
            player->SetGlyph(m_glyphIndex, glyph);
        }
    }
}

void Spell::EffectSummonTotem(uint32 i)
{
    uint8 slot = 0;
    switch(m_spellInfo->EffectMiscValueB[i])
    {
        case SUMMON_TYPE_TOTEM_SLOT1: slot = 0; break;
        case SUMMON_TYPE_TOTEM_SLOT2: slot = 1; break;
        case SUMMON_TYPE_TOTEM_SLOT3: slot = 2; break;
        case SUMMON_TYPE_TOTEM_SLOT4: slot = 3; break;
        // Battle standard case
        case SUMMON_TYPE_TOTEM:       slot = 254; break;
        // jewelery statue case, like totem without slot
        case SUMMON_TYPE_GUARDIAN:    slot = 255; break;
        default: return;
    }

    if(slot < MAX_TOTEM)
    {
        uint64 guid = m_caster->m_TotemSlot[slot];
        if(guid != 0)
        {
            Creature *OldTotem = ObjectAccessor::GetCreature(*m_caster, guid);
            if(OldTotem && OldTotem->isTotem())
                ((Totem*)OldTotem)->UnSummon();
        }
    }

    uint32 team = 0;
    if (m_caster->GetTypeId()==TYPEID_PLAYER)
        team = ((Player*)m_caster)->GetTeam();

    Totem* pTotem = new Totem;

    if(!pTotem->Create(objmgr.GenerateLowGuid(HIGHGUID_UNIT), m_caster->GetMap(), m_caster->GetPhaseMask(),
        m_spellInfo->EffectMiscValue[i], team ))
    {
        delete pTotem;
        return;
    }

    float angle = slot < MAX_TOTEM ? M_PI/MAX_TOTEM - (slot*2*M_PI/MAX_TOTEM) : 0;

    float x,y,z;
    m_caster->GetClosePoint(x,y,z,pTotem->GetObjectSize(),2.0f,angle);

    // totem must be at same Z in case swimming caster and etc.
    if( fabs( z - m_caster->GetPositionZ() ) > 5 )
        z = m_caster->GetPositionZ();

    pTotem->Relocate(x, y, z, m_caster->GetOrientation());

    if(slot < MAX_TOTEM)
        m_caster->m_TotemSlot[slot] = pTotem->GetGUID();

    pTotem->SetOwner(m_caster->GetGUID());
    pTotem->SetTypeBySummonSpell(m_spellInfo);              // must be after Create call where m_spells initilized

    int32 duration=GetSpellDuration(m_spellInfo);
    if(Player* modOwner = m_caster->GetSpellModOwner())
        modOwner->ApplySpellMod(m_spellInfo->Id,SPELLMOD_DURATION, duration);
    pTotem->SetDuration(duration);

    if (damage)                                             // if not spell info, DB values used
    {
        pTotem->SetMaxHealth(damage);
        pTotem->SetHealth(damage);
    }

    pTotem->SetUInt32Value(UNIT_CREATED_BY_SPELL,m_spellInfo->Id);

    if(m_caster->GetTypeId() == TYPEID_PLAYER)
        pTotem->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_PVP_ATTACKABLE);

    pTotem->Summon(m_caster);

    if(slot < MAX_TOTEM && m_caster->GetTypeId() == TYPEID_PLAYER)
    {
        WorldPacket data(SMSG_TOTEM_CREATED, 1+8+4+4);
        data << uint8(slot);
        data << uint64(pTotem->GetGUID());
        data << uint32(duration);
        data << uint32(m_spellInfo->Id);
        ((Player*)m_caster)->SendDirectMessage(&data);
    }
}

void Spell::EffectEnchantHeldItem(uint32 i)
{
    // this is only item spell effect applied to main-hand weapon of target player (players in area)
    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* item_owner = (Player*)unitTarget;
    Item* item = item_owner->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

    if(!item )
        return;

    // must be equipped
    if(!item ->IsEquipped())
        return;

    if (m_spellInfo->EffectMiscValue[i])
    {
        uint32 enchant_id = m_spellInfo->EffectMiscValue[i];
        int32 duration = GetSpellDuration(m_spellInfo);          //Try duration index first ..
        if(!duration)
            duration = m_currentBasePoints[i]+1;            //Base points after ..
        if(!duration)
            duration = 10;                                  //10 seconds for enchants which don't have listed duration

        SpellItemEnchantmentEntry const *pEnchant = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
        if(!pEnchant)
            return;

        // Always go to temp enchantment slot
        EnchantmentSlot slot = TEMP_ENCHANTMENT_SLOT;

        // Enchantment will not be applied if a different one already exists
        if(item->GetEnchantmentId(slot) && item->GetEnchantmentId(slot) != enchant_id)
            return;

        // Apply the temporary enchantment
        item->SetEnchantment(slot, enchant_id, duration*IN_MILISECONDS, 0);
        item_owner->ApplyEnchantment(item,slot,true);
    }
}

void Spell::EffectDisEnchant(uint32 /*i*/)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* p_caster = (Player*)m_caster;
    if(!itemTarget || !itemTarget->GetProto()->DisenchantID)
        return;

    p_caster->UpdateCraftSkill(m_spellInfo->Id);

    ((Player*)m_caster)->SendLoot(itemTarget->GetGUID(),LOOT_DISENCHANTING);

    // item will be removed at disenchanting end
}

void Spell::EffectInebriate(uint32 /*i*/)
{
    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player *player = (Player*)unitTarget;
    uint16 currentDrunk = player->GetDrunkValue();
    uint16 drunkMod = damage * 256;
    if (currentDrunk + drunkMod > 0xFFFF)
        currentDrunk = 0xFFFF;
    else
        currentDrunk += drunkMod;
    player->SetDrunkValue(currentDrunk, m_CastItem?m_CastItem->GetEntry():0);
}

void Spell::EffectFeedPet(uint32 i)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player *_player = (Player*)m_caster;

    Item* foodItem = m_targets.getItemTarget();
    if(!foodItem)
        return;

    Pet *pet = _player->GetPet();
    if(!pet)
        return;

    if(!pet->isAlive())
        return;

    int32 benefit = pet->GetCurrentFoodBenefitLevel(foodItem->GetProto()->ItemLevel);
    if(benefit <= 0)
        return;

    uint32 count = 1;
    _player->DestroyItemCount(foodItem,count,true);
    // TODO: fix crash when a spell has two effects, both pointed at the same item target

    m_caster->CastCustomSpell(pet,m_spellInfo->EffectTriggerSpell[i],&benefit,NULL,NULL,true);
}

void Spell::EffectDismissPet(uint32 /*i*/)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Pet* pet = m_caster->GetPet();

    // not let dismiss dead pet
    if(!pet||!pet->isAlive())
        return;

    ((Player*)m_caster)->RemovePet(pet,PET_SAVE_NOT_IN_SLOT);
}

void Spell::EffectSummonObject(uint32 i)
{
    uint32 go_id = m_spellInfo->EffectMiscValue[i];

    uint8 slot = 0;
    switch(m_spellInfo->Effect[i])
    {
        case SPELL_EFFECT_SUMMON_OBJECT_SLOT1: slot = 0; break;
        case SPELL_EFFECT_SUMMON_OBJECT_SLOT2: slot = 1; break;
        case SPELL_EFFECT_SUMMON_OBJECT_SLOT3: slot = 2; break;
        case SPELL_EFFECT_SUMMON_OBJECT_SLOT4: slot = 3; break;
        default: return;
    }

    uint64 guid = m_caster->m_ObjectSlot[slot];
    if(guid != 0)
    {
        GameObject* obj = NULL;
        if( m_caster )
            obj = ObjectAccessor::GetGameObject(*m_caster, guid);

        if(obj) obj->Delete();
        m_caster->m_ObjectSlot[slot] = 0;
    }

    GameObject* pGameObj = new GameObject;

    float x,y,z;
    // If dest location if present
    if (m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION)
    {
        x = m_targets.m_destX;
        y = m_targets.m_destY;
        z = m_targets.m_destZ;
    }
    // Summon in random point all other units if location present
    else
        m_caster->GetClosePoint(x,y,z,DEFAULT_WORLD_OBJECT_SIZE);

    Map *map = m_caster->GetMap();
    if(!pGameObj->Create(objmgr.GenerateLowGuid(HIGHGUID_GAMEOBJECT), go_id, map,
        m_caster->GetPhaseMask(), x, y, z, m_caster->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, 0, 1))
    {
        delete pGameObj;
        return;
    }

    pGameObj->SetUInt32Value(GAMEOBJECT_LEVEL,m_caster->getLevel());
    int32 duration = GetSpellDuration(m_spellInfo);
    pGameObj->SetRespawnTime(duration > 0 ? duration/IN_MILISECONDS : 0);
    pGameObj->SetSpellId(m_spellInfo->Id);
    m_caster->AddGameObject(pGameObj);

    map->Add(pGameObj);
    WorldPacket data(SMSG_GAMEOBJECT_SPAWN_ANIM_OBSOLETE, 8);
    data << pGameObj->GetGUID();
    m_caster->SendMessageToSet(&data,true);

    m_caster->m_ObjectSlot[slot] = pGameObj->GetGUID();
}

void Spell::EffectResurrect(uint32 /*effIndex*/)
{
    if(!unitTarget)
        return;
    if(unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    if(unitTarget->isAlive())
        return;
    if(!unitTarget->IsInWorld())
        return;

    switch (m_spellInfo->Id)
    {
        // Defibrillate (Goblin Jumper Cables) have 33% chance on success
        case 8342:
            if (roll_chance_i(67))
            {
                m_caster->CastSpell(m_caster, 8338, true, m_CastItem);
                return;
            }
            break;
        // Defibrillate (Goblin Jumper Cables XL) have 50% chance on success
        case 22999:
            if (roll_chance_i(50))
            {
                m_caster->CastSpell(m_caster, 23055, true, m_CastItem);
                return;
            }
            break;
        default:
            break;
    }

    Player* pTarget = ((Player*)unitTarget);

    if(pTarget->isRessurectRequested())       // already have one active request
        return;

    uint32 health = pTarget->GetMaxHealth() * damage / 100;
    uint32 mana   = pTarget->GetMaxPower(POWER_MANA) * damage / 100;

    pTarget->setResurrectRequestData(m_caster->GetGUID(), m_caster->GetMapId(), m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), health, mana);
    SendResurrectRequest(pTarget);
}

void Spell::EffectAddExtraAttacks(uint32 /*i*/)
{
    if(!unitTarget || !unitTarget->isAlive())
        return;

    if( unitTarget->m_extraAttacks )
        return;

    unitTarget->m_extraAttacks = damage;
}

void Spell::EffectParry(uint32 /*i*/)
{
    if (unitTarget && unitTarget->GetTypeId() == TYPEID_PLAYER)
        ((Player*)unitTarget)->SetCanParry(true);
}

void Spell::EffectBlock(uint32 /*i*/)
{
    if (unitTarget && unitTarget->GetTypeId() == TYPEID_PLAYER)
        ((Player*)unitTarget)->SetCanBlock(true);
}

void Spell::EffectMomentMove(uint32 i)
{
    if(unitTarget->isInFlight())
        return;

    if( m_spellInfo->rangeIndex== 1)                        //self range
    {
        float dis = GetSpellRadius(sSpellRadiusStore.LookupEntry(m_spellInfo->EffectRadiusIndex[i]));

        // before caster
        float fx,fy,fz;
        unitTarget->GetClosePoint(fx,fy,fz,unitTarget->GetObjectSize(),dis);
        float ox,oy,oz;
        unitTarget->GetPosition(ox,oy,oz);

        float fx2,fy2,fz2;                                  // getObjectHitPos overwrite last args in any result case
        if(VMAP::VMapFactory::createOrGetVMapManager()->getObjectHitPos(unitTarget->GetMapId(), ox,oy,oz+0.5, fx,fy,oz+0.5,fx2,fy2,fz2, -0.5))
        {
            fx = fx2;
            fy = fy2;
            fz = fz2;
            unitTarget->UpdateGroundPositionZ(fx,fy,fz);
        }

        unitTarget->NearTeleportTo(fx, fy, fz, unitTarget->GetOrientation(),unitTarget==m_caster);
    }
}

void Spell::EffectReputation(uint32 i)
{
    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player *_player = (Player*)unitTarget;

    int32  rep_change = m_currentBasePoints[i]+1;           // field store reputation change -1

    uint32 faction_id = m_spellInfo->EffectMiscValue[i];

    FactionEntry const* factionEntry = sFactionStore.LookupEntry(faction_id);

    if(!factionEntry)
        return;

    _player->ModifyFactionReputation(factionEntry,rep_change);
}

void Spell::EffectQuestComplete(uint32 i)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player *_player = (Player*)m_caster;

    uint32 quest_id = m_spellInfo->EffectMiscValue[i];
    _player->AreaExploredOrEventHappens(quest_id);
}

void Spell::EffectSelfResurrect(uint32 i)
{
    if(!unitTarget || unitTarget->isAlive())
        return;
    if(unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;
    if(!unitTarget->IsInWorld())
        return;

    uint32 health = 0;
    uint32 mana = 0;

    // flat case
    if(damage < 0)
    {
        health = uint32(-damage);
        mana = m_spellInfo->EffectMiscValue[i];
    }
    // percent case
    else
    {
        health = uint32(damage/100.0f*unitTarget->GetMaxHealth());
        if(unitTarget->GetMaxPower(POWER_MANA) > 0)
            mana = uint32(damage/100.0f*unitTarget->GetMaxPower(POWER_MANA));
    }

    Player *plr = ((Player*)unitTarget);
    plr->ResurrectPlayer(0.0f);

    plr->SetHealth( health );
    plr->SetPower(POWER_MANA, mana );
    plr->SetPower(POWER_RAGE, 0 );
    plr->SetPower(POWER_ENERGY, plr->GetMaxPower(POWER_ENERGY) );

    plr->SpawnCorpseBones();

    plr->SaveToDB();
}

void Spell::EffectSkinning(uint32 /*i*/)
{
    if(unitTarget->GetTypeId() != TYPEID_UNIT )
        return;
    if(!m_caster || m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Creature* creature = (Creature*) unitTarget;
    int32 targetLevel = creature->getLevel();

    uint32 skill = creature->GetCreatureInfo()->GetRequiredLootSkill();

    ((Player*)m_caster)->SendLoot(creature->GetGUID(),LOOT_SKINNING);
    creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);

    int32 reqValue = targetLevel < 10 ? 0 : targetLevel < 20 ? (targetLevel-10)*10 : targetLevel*5;

    int32 skillValue = ((Player*)m_caster)->GetPureSkillValue(skill);

    // Double chances for elites
    ((Player*)m_caster)->UpdateGatherSkill(skill, skillValue, reqValue, creature->isElite() ? 2 : 1 );
}

void Spell::EffectCharge(uint32 /*i*/)
{
    if(!unitTarget || !m_caster)
        return;

    float x, y, z;
    unitTarget->GetContactPoint(m_caster, x, y, z);
    if(unitTarget->GetTypeId() != TYPEID_PLAYER)
        ((Creature *)unitTarget)->StopMoving();

    // Only send MOVEMENTFLAG_WALK_MODE, client has strange issues with other move flags
    m_caster->SendMonsterMove(x, y, z, 0, MOVEMENTFLAG_WALK_MODE, 1);

    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        m_caster->GetMap()->CreatureRelocation((Creature*)m_caster,x,y,z,m_caster->GetOrientation());

    // not all charge effects used in negative spells
    if ( !IsPositiveSpell(m_spellInfo->Id))
        m_caster->Attack(unitTarget,true);
}

void Spell::EffectSummonCritter(uint32 i)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;
    Player* player = (Player*)m_caster;

    uint32 pet_entry = m_spellInfo->EffectMiscValue[i];
    if(!pet_entry)
        return;

    Pet* old_critter = player->GetMiniPet();

    // for same pet just despawn
    if(old_critter && old_critter->GetEntry() == pet_entry)
    {
        player->RemoveMiniPet();
        return;
    }

    // despawn old pet before summon new
    if(old_critter)
        player->RemoveMiniPet();

    // summon new pet
    Pet* critter = new Pet(MINI_PET);

    Map *map = m_caster->GetMap();
    uint32 pet_number = objmgr.GeneratePetNumber();
    if(!critter->Create(objmgr.GenerateLowGuid(HIGHGUID_PET), map, m_caster->GetPhaseMask(),
        pet_entry, pet_number))
    {
        sLog.outError("Spell::EffectSummonCritter, spellid %u: no such creature entry %u", m_spellInfo->Id, pet_entry);
        delete critter;
        return;
    }

    float x,y,z;
    // If dest location if present
    if (m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION)
    {
         x = m_targets.m_destX;
         y = m_targets.m_destY;
         z = m_targets.m_destZ;
     }
     // Summon if dest location not present near caster
     else
         m_caster->GetClosePoint(x,y,z,critter->GetObjectSize());

    critter->Relocate(x,y,z,m_caster->GetOrientation());

    if(!critter->IsPositionValid())
    {
        sLog.outError("Pet (guidlow %d, entry %d) not summoned. Suggested coordinates isn't valid (X: %f Y: %f)",
            critter->GetGUIDLow(), critter->GetEntry(), critter->GetPositionX(), critter->GetPositionY());
        delete critter;
        return;
    }

    critter->SetOwnerGUID(m_caster->GetGUID());
    critter->SetCreatorGUID(m_caster->GetGUID());
    critter->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE,m_caster->getFaction());
    critter->SetUInt32Value(UNIT_CREATED_BY_SPELL, m_spellInfo->Id);

    critter->AIM_Initialize();
    critter->InitPetCreateSpells();                         // e.g. disgusting oozeling has a create spell as critter...
    critter->SetMaxHealth(1);
    critter->SetHealth(1);
    critter->SetLevel(1);

    // set timer for unsummon
    int32 duration = GetSpellDuration(m_spellInfo);
    if(duration > 0)
        critter->SetDuration(duration);

    std::string name = player->GetName();
    name.append(petTypeSuffix[critter->getPetType()]);
    critter->SetName( name );
    player->SetMiniPet(critter);

    map->Add((Creature*)critter);
}

void Spell::EffectKnockBack(uint32 i)
{
    if(!unitTarget || !m_caster)
        return;

    // Effect only works on players
    if(unitTarget->GetTypeId()!=TYPEID_PLAYER)
        return;

    float vsin = sin(m_caster->GetAngle(unitTarget));
    float vcos = cos(m_caster->GetAngle(unitTarget));

    WorldPacket data(SMSG_MOVE_KNOCK_BACK, (8+4+4+4+4+4));
    data.append(unitTarget->GetPackGUID());
    data << uint32(0);                                      // Sequence
    data << float(vcos);                                    // x direction
    data << float(vsin);                                    // y direction
    data << float(m_spellInfo->EffectMiscValue[i])/10;      // Horizontal speed
    data << float(damage/-10);                              // Z Movement speed (vertical)

    ((Player*)unitTarget)->GetSession()->SendPacket(&data);
}

void Spell::EffectSendTaxi(uint32 i)
{
    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    TaxiPathEntry const* entry = sTaxiPathStore.LookupEntry(m_spellInfo->EffectMiscValue[i]);
    if(!entry)
        return;

    std::vector<uint32> nodes;

    nodes.resize(2);
    nodes[0] = entry->from;
    nodes[1] = entry->to;

    uint32 mountid = 0;
    switch(m_spellInfo->Id)
    {
        case 31606:                                         //Stormcrow Amulet
            mountid = 17447;
            break;
        case 45071:                                         //Quest - Sunwell Daily - Dead Scar Bombing Run
        case 45113:                                         //Quest - Sunwell Daily - Ship Bombing Run
        case 45353:                                         //Quest - Sunwell Daily - Ship Bombing Run Return
            mountid = 22840;
            break;
        case 34905:                                         //Stealth Flight
            mountid = 6851;
            break;
        case 45883:                                         //Amber Ledge to Beryl Point
            mountid = 23524;
            break;
        case 46064:                                         //Amber Ledge to Coldarra
            mountid = 6371;
            break;
        case 53335:                                         //Stormwind Harbor Flight - Peaceful
            mountid = 6852;
            break;
    }

    ((Player*)unitTarget)->ActivateTaxiPathTo(nodes,mountid);

}

void Spell::EffectPlayerPull(uint32 i)
{
    if(!unitTarget || !m_caster)
        return;

    // Effect only works on players
    if(unitTarget->GetTypeId()!=TYPEID_PLAYER)
        return;

    float vsin = sin(unitTarget->GetAngle(m_caster));
    float vcos = cos(unitTarget->GetAngle(m_caster));

    WorldPacket data(SMSG_MOVE_KNOCK_BACK, (8+4+4+4+4+4));
    data.append(unitTarget->GetPackGUID());
    data << uint32(0);                                      // Sequence
    data << float(vcos);                                    // x direction
    data << float(vsin);                                    // y direction
                                                            // Horizontal speed
    data << float(damage ? damage : unitTarget->GetDistance2d(m_caster));
    data << float(m_spellInfo->EffectMiscValue[i])/-10;     // Z Movement speed

    ((Player*)unitTarget)->GetSession()->SendPacket(&data);
}

void Spell::EffectDispelMechanic(uint32 i)
{
    if(!unitTarget)
        return;

    uint32 mechanic = m_spellInfo->EffectMiscValue[i];

    Unit::AuraMap& Auras = unitTarget->GetAuras();
    for(Unit::AuraMap::iterator iter = Auras.begin(), next; iter != Auras.end(); iter = next)
    {
        next = iter;
        ++next;
        SpellEntry const *spell = sSpellStore.LookupEntry(iter->second->GetSpellProto()->Id);
        if(spell->Mechanic == mechanic || spell->EffectMechanic[iter->second->GetEffIndex()] == mechanic)
        {
            unitTarget->RemoveAurasDueToSpell(spell->Id);
            if(Auras.empty())
                break;
            else
                next = Auras.begin();
        }
    }
    return;
}

void Spell::EffectSummonDeadPet(uint32 /*i*/)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;
    Player *_player = (Player*)m_caster;
    Pet *pet = _player->GetPet();
    if(!pet)
        return;
    if(pet->isAlive())
        return;
    if(damage < 0)
        return;
    pet->SetUInt32Value(UNIT_DYNAMIC_FLAGS, 0);
    pet->RemoveFlag (UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);
    pet->setDeathState( ALIVE );
    pet->clearUnitState(UNIT_STAT_ALL_STATE);
    pet->SetHealth( uint32(pet->GetMaxHealth()*(float(damage)/100)));

    pet->AIM_Initialize();

    _player->PetSpellInitialize();
    pet->SavePetToDB(PET_SAVE_AS_CURRENT);
}

void Spell::EffectDestroyAllTotems(uint32 /*i*/)
{
    float mana = 0;
    for(int slot = 0;  slot < MAX_TOTEM; ++slot)
    {
        if(!m_caster->m_TotemSlot[slot])
            continue;

        Creature* totem = ObjectAccessor::GetCreature(*m_caster,m_caster->m_TotemSlot[slot]);
        if(totem && totem->isTotem())
        {
            uint32 spell_id = totem->GetUInt32Value(UNIT_CREATED_BY_SPELL);
            SpellEntry const* spellInfo = sSpellStore.LookupEntry(spell_id);
            if(spellInfo)
                mana += spellInfo->manaCost * damage / 100;
            ((Totem*)totem)->UnSummon();
        }
    }

    int32 gain = m_caster->ModifyPower(POWER_MANA,int32(mana));
    m_caster->SendEnergizeSpellLog(m_caster, m_spellInfo->Id, gain, POWER_MANA);
}

void Spell::EffectDurabilityDamage(uint32 i)
{
    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    int32 slot = m_spellInfo->EffectMiscValue[i];

    // FIXME: some spells effects have value -1/-2
    // Possibly its mean -1 all player equipped items and -2 all items
    if(slot < 0)
    {
        ((Player*)unitTarget)->DurabilityPointsLossAll(damage,(slot < -1));
        return;
    }

    // invalid slot value
    if(slot >= INVENTORY_SLOT_BAG_END)
        return;

    if(Item* item = ((Player*)unitTarget)->GetItemByPos(INVENTORY_SLOT_BAG_0,slot))
        ((Player*)unitTarget)->DurabilityPointsLoss(item,damage);
}

void Spell::EffectDurabilityDamagePCT(uint32 i)
{
    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    int32 slot = m_spellInfo->EffectMiscValue[i];

    // FIXME: some spells effects have value -1/-2
    // Possibly its mean -1 all player equipped items and -2 all items
    if(slot < 0)
    {
        ((Player*)unitTarget)->DurabilityLossAll(double(damage)/100.0f,(slot < -1));
        return;
    }

    // invalid slot value
    if(slot >= INVENTORY_SLOT_BAG_END)
        return;

    if(damage <= 0)
        return;

    if(Item* item = ((Player*)unitTarget)->GetItemByPos(INVENTORY_SLOT_BAG_0,slot))
        ((Player*)unitTarget)->DurabilityLoss(item,double(damage)/100.0f);
}

void Spell::EffectModifyThreatPercent(uint32 /*effIndex*/)
{
    if(!unitTarget)
        return;

    unitTarget->getThreatManager().modifyThreatPercent(m_caster, damage);
}

void Spell::EffectTransmitted(uint32 effIndex)
{
    uint32 name_id = m_spellInfo->EffectMiscValue[effIndex];

    GameObjectInfo const* goinfo = objmgr.GetGameObjectInfo(name_id);

    if (!goinfo)
    {
        sLog.outErrorDb("Gameobject (Entry: %u) not exist and not created at spell (ID: %u) cast",name_id, m_spellInfo->Id);
        return;
    }

    float fx,fy,fz;

    if(m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION)
    {
        fx = m_targets.m_destX;
        fy = m_targets.m_destY;
        fz = m_targets.m_destZ;
    }
    //FIXME: this can be better check for most objects but still hack
    else if(m_spellInfo->EffectRadiusIndex[effIndex] && m_spellInfo->speed==0)
    {
        float dis = GetSpellRadius(sSpellRadiusStore.LookupEntry(m_spellInfo->EffectRadiusIndex[effIndex]));
        m_caster->GetClosePoint(fx,fy,fz,DEFAULT_WORLD_OBJECT_SIZE, dis);
    }
    else
    {
        float min_dis = GetSpellMinRange(sSpellRangeStore.LookupEntry(m_spellInfo->rangeIndex));
        float max_dis = GetSpellMaxRange(sSpellRangeStore.LookupEntry(m_spellInfo->rangeIndex));
        float dis = rand_norm() * (max_dis - min_dis) + min_dis;

        m_caster->GetClosePoint(fx,fy,fz,DEFAULT_WORLD_OBJECT_SIZE, dis);
    }

    Map *cMap = m_caster->GetMap();

    if(goinfo->type==GAMEOBJECT_TYPE_FISHINGNODE)
    {
        if ( !cMap->IsInWater(fx,fy,fz-0.5f)) // Hack to prevent fishing bobber from failing to land on fishing hole
        { // but this is not proper, we really need to ignore not materialized objects
            SendCastResult(SPELL_FAILED_NOT_HERE);
            SendChannelUpdate(0);
            return;
        }

        // replace by water level in this case
        fz = cMap->GetWaterLevel(fx,fy);
    }
    // if gameobject is summoning object, it should be spawned right on caster's position
    else if(goinfo->type==GAMEOBJECT_TYPE_SUMMONING_RITUAL)
    {
        m_caster->GetPosition(fx,fy,fz);
    }

    GameObject* pGameObj = new GameObject;

    if(!pGameObj->Create(objmgr.GenerateLowGuid(HIGHGUID_GAMEOBJECT), name_id, cMap,
        m_caster->GetPhaseMask(), fx, fy, fz, m_caster->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, 100, 1))
    {
        delete pGameObj;
        return;
    }

    int32 duration = GetSpellDuration(m_spellInfo);

    switch(goinfo->type)
    {
        case GAMEOBJECT_TYPE_FISHINGNODE:
        {
            m_caster->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT,pGameObj->GetGUID());
            m_caster->AddGameObject(pGameObj);              // will removed at spell cancel

            // end time of range when possible catch fish (FISHING_BOBBER_READY_TIME..GetDuration(m_spellInfo))
            // start time == fish-FISHING_BOBBER_READY_TIME (0..GetDuration(m_spellInfo)-FISHING_BOBBER_READY_TIME)
            int32 lastSec;
            switch(urand(0, 3))
            {
                case 0: lastSec =  3; break;
                case 1: lastSec =  7; break;
                case 2: lastSec = 13; break;
                case 3: lastSec = 17; break;
            }

            duration = duration - lastSec*IN_MILISECONDS + FISHING_BOBBER_READY_TIME*IN_MILISECONDS;
            break;
        }
        case GAMEOBJECT_TYPE_SUMMONING_RITUAL:
        {
            if(m_caster->GetTypeId()==TYPEID_PLAYER)
            {
                pGameObj->AddUniqueUse((Player*)m_caster);
                m_caster->AddGameObject(pGameObj);          // will removed at spell cancel
            }
            break;
        }
        case GAMEOBJECT_TYPE_FISHINGHOLE:
        case GAMEOBJECT_TYPE_CHEST:
        default:
        {
            break;
        }
    }

    pGameObj->SetRespawnTime(duration > 0 ? duration/IN_MILISECONDS : 0);

    pGameObj->SetOwnerGUID(m_caster->GetGUID() );

    pGameObj->SetUInt32Value(GAMEOBJECT_LEVEL, m_caster->getLevel() );
    pGameObj->SetSpellId(m_spellInfo->Id);

    DEBUG_LOG("AddObject at SpellEfects.cpp EffectTransmitted");
    //m_caster->AddGameObject(pGameObj);
    //m_ObjToDel.push_back(pGameObj);

    cMap->Add(pGameObj);

    WorldPacket data(SMSG_GAMEOBJECT_SPAWN_ANIM_OBSOLETE, 8);
    data << uint64(pGameObj->GetGUID());
    m_caster->SendMessageToSet(&data,true);

    if(uint32 linkedEntry = pGameObj->GetLinkedGameObjectEntry())
    {
        GameObject* linkedGO = new GameObject;
        if(linkedGO->Create(objmgr.GenerateLowGuid(HIGHGUID_GAMEOBJECT), linkedEntry, cMap,
            m_caster->GetPhaseMask(), fx, fy, fz, m_caster->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, 100, 1))
        {
            linkedGO->SetRespawnTime(duration > 0 ? duration/IN_MILISECONDS : 0);
            linkedGO->SetUInt32Value(GAMEOBJECT_LEVEL, m_caster->getLevel() );
            linkedGO->SetSpellId(m_spellInfo->Id);
            linkedGO->SetOwnerGUID(m_caster->GetGUID() );

            linkedGO->GetMap()->Add(linkedGO);
        }
        else
        {
            delete linkedGO;
            linkedGO = NULL;
            return;
        }
    }
}

void Spell::EffectProspecting(uint32 /*i*/)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* p_caster = (Player*)m_caster;
    if(!itemTarget || !(itemTarget->GetProto()->BagFamily & BAG_FAMILY_MASK_MINING_SUPP))
        return;

    if(itemTarget->GetCount() < 5)
        return;

    if( sWorld.getConfig(CONFIG_SKILL_PROSPECTING))
    {
        uint32 SkillValue = p_caster->GetPureSkillValue(SKILL_JEWELCRAFTING);
        uint32 reqSkillValue = itemTarget->GetProto()->RequiredSkillRank;
        p_caster->UpdateGatherSkill(SKILL_JEWELCRAFTING, SkillValue, reqSkillValue);
    }

    ((Player*)m_caster)->SendLoot(itemTarget->GetGUID(), LOOT_PROSPECTING);
}

void Spell::EffectMilling(uint32 /*i*/)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* p_caster = (Player*)m_caster;
    if(!itemTarget || !(itemTarget->GetProto()->BagFamily & BAG_FAMILY_MASK_HERBS))
        return;

    if(itemTarget->GetCount() < 5)
        return;

    if( sWorld.getConfig(CONFIG_SKILL_MILLING))
    {
        uint32 SkillValue = p_caster->GetPureSkillValue(SKILL_INSCRIPTION);
        uint32 reqSkillValue = itemTarget->GetProto()->RequiredSkillRank;
        p_caster->UpdateGatherSkill(SKILL_INSCRIPTION, SkillValue, reqSkillValue);
    }

    ((Player*)m_caster)->SendLoot(itemTarget->GetGUID(), LOOT_MILLING);
}

void Spell::EffectSkill(uint32 /*i*/)
{
    sLog.outDebug("WORLD: SkillEFFECT");
}

void Spell::EffectSummonDemon(uint32 i)
{
    // select center of summon position
    float center_x = m_targets.m_destX;
    float center_y = m_targets.m_destY;
    float center_z = m_targets.m_destZ;

    float radius = GetSpellRadius(sSpellRadiusStore.LookupEntry(m_spellInfo->EffectRadiusIndex[i]));

    int32 amount = damage > 0 ? damage : 1;

    for(int32 count = 0; count < amount; ++count)
    {
        float px, py, pz;
        // If dest location if present
        if (m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION)
        {
            // Summon 1 unit in dest location
            if (count == 0)
            {
                px = m_targets.m_destX;
                py = m_targets.m_destY;
                pz = m_targets.m_destZ;
            }
            // Summon in random point all other units if location present
            else
                m_caster->GetRandomPoint(center_x,center_y,center_z,radius,px,py,pz);
        }
        // Summon if dest location not present near caster
        else
            m_caster->GetClosePoint(px,py,pz,3.0f);

        int32 duration = GetSpellDuration(m_spellInfo);

        Creature* Charmed = m_caster->SummonCreature(m_spellInfo->EffectMiscValue[i], px, py, pz, m_caster->GetOrientation(),TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,duration);
        if (!Charmed)                                       // something fatal, not attempt more
            return;

        // might not always work correctly, maybe the creature that dies from CoD casts the effect on itself and is therefore the caster?
        Charmed->SetLevel(m_caster->getLevel());

        // TODO: Add damage/mana/hp according to level

        if (m_spellInfo->EffectMiscValue[i] == 89)          // Inferno summon
        {
            // Enslave demon effect, without mana cost and cooldown
            m_caster->CastSpell(Charmed, 20882, true);      // FIXME: enslave does not scale with level, level 62+ minions cannot be enslaved

            // Inferno effect
            Charmed->CastSpell(Charmed, 22703, true, 0);
        }
    }
}

/* There is currently no need for this effect. We handle it in BattleGround.cpp
   If we would handle the resurrection here, the spiritguide would instantly disappear as the
   player revives, and so we wouldn't see the spirit heal visual effect on the npc.
   This is why we use a half sec delay between the visual effect and the resurrection itself */
void Spell::EffectSpiritHeal(uint32 /*i*/)
{
    /*
    if(!unitTarget || unitTarget->isAlive())
        return;
    if(unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;
    if(!unitTarget->IsInWorld())
        return;

    //m_spellInfo->EffectBasePoints[i]; == 99 (percent?)
    //((Player*)unitTarget)->setResurrect(m_caster->GetGUID(), unitTarget->GetPositionX(), unitTarget->GetPositionY(), unitTarget->GetPositionZ(), unitTarget->GetMaxHealth(), unitTarget->GetMaxPower(POWER_MANA));
    ((Player*)unitTarget)->ResurrectPlayer(1.0f);
    ((Player*)unitTarget)->SpawnCorpseBones();
    */
}

// remove insignia spell effect
void Spell::EffectSkinPlayerCorpse(uint32 /*i*/)
{
    sLog.outDebug("Effect: SkinPlayerCorpse");
    if ( (m_caster->GetTypeId() != TYPEID_PLAYER) || (unitTarget->GetTypeId() != TYPEID_PLAYER) || (unitTarget->isAlive()) )
        return;

    ((Player*)unitTarget)->RemovedInsignia( (Player*)m_caster );
}

void Spell::EffectStealBeneficialBuff(uint32 i)
{
    sLog.outDebug("Effect: StealBeneficialBuff");

    if(!unitTarget || unitTarget==m_caster)                 // can't steal from self
        return;

    std::vector <Aura *> steal_list;
    // Create dispel mask by dispel type
    uint32 dispelMask  = GetDispellMask( DispelType(m_spellInfo->EffectMiscValue[i]) );
    Unit::AuraMap const& auras = unitTarget->GetAuras();
    for(Unit::AuraMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
    {
        Aura *aur = (*itr).second;
        if (aur && (1<<aur->GetSpellProto()->Dispel) & dispelMask)
        {
            // Need check for passive? this
            if (aur->IsPositive() && !aur->IsPassive())
                steal_list.push_back(aur);
        }
    }
    // Ok if exist some buffs for dispel try dispel it
    if (!steal_list.empty())
    {
        std::list < std::pair<uint32,uint64> > success_list;
        int32 list_size = steal_list.size();
        // Dispell N = damage buffs (or while exist buffs for dispel)
        for (int32 count=0; count < damage && list_size > 0; ++count)
        {
            // Random select buff for dispel
            Aura *aur = steal_list[urand(0, list_size-1)];
            // Not use chance for steal
            // TODO possible need do it
            success_list.push_back( std::pair<uint32,uint64>(aur->GetId(),aur->GetCasterGUID()));

            // Remove buff from list for prevent doubles
            for (std::vector<Aura *>::iterator j = steal_list.begin(); j != steal_list.end(); )
            {
                Aura *stealed = *j;
                if (stealed->GetId() == aur->GetId() && stealed->GetCasterGUID() == aur->GetCasterGUID())
                {
                    j = steal_list.erase(j);
                    --list_size;
                }
                else
                    ++j;
            }
        }
        // Really try steal and send log
        if (!success_list.empty())
        {
            int32 count = success_list.size();
            WorldPacket data(SMSG_SPELLSTEALLOG, 8+8+4+1+4+count*5);
            data.append(unitTarget->GetPackGUID());  // Victim GUID
            data.append(m_caster->GetPackGUID());    // Caster GUID
            data << uint32(m_spellInfo->Id);         // Dispell spell id
            data << uint8(0);                        // not used
            data << uint32(count);                   // count
            for (std::list<std::pair<uint32,uint64> >::iterator j = success_list.begin(); j != success_list.end(); ++j)
            {
                SpellEntry const* spellInfo = sSpellStore.LookupEntry(j->first);
                data << uint32(spellInfo->Id);       // Spell Id
                data << uint8(0);                    // 0 - steals !=0 transfers
                unitTarget->RemoveAurasDueToSpellBySteal(spellInfo->Id, j->second, m_caster);
            }
            m_caster->SendMessageToSet(&data, true);
        }
    }
}

void Spell::EffectKillCredit(uint32 i)
{
    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    ((Player*)unitTarget)->KilledMonster(m_spellInfo->EffectMiscValue[i], 0);
}

void Spell::EffectQuestFail(uint32 i)
{
    if(!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    ((Player*)unitTarget)->FailQuest(m_spellInfo->EffectMiscValue[i]);
}

void Spell::EffectSpecialSummon( uint32 spell_id, uint32 i, Unit* m_caster )
{
    SpellEntry const *summon_spell = sSpellStore.LookupEntry(spell_id);
    if(!summon_spell)
        return;
    uint32 summon_entry = summon_spell->EffectMiscValue[i];
    sLog.outError(" creature ID %d in spell : %d", summon_entry, spell_id);
    if(!summon_entry)
    {
        summon_entry = 7560;
        sLog.outError("missing summoned creature ID in spell : %d", spell_id);
    }

    uint32 level = m_caster->getLevel();
    int32 duration = GetSpellDuration(summon_spell);
    // select center of summon position
    float center_x = m_caster->GetPositionX();
    float center_y = m_caster->GetPositionY();
    float center_z = m_caster->GetPositionZ();
    float px;
    float py;
    float pz;

    float radius = GetSpellRadius(sSpellRadiusStore.LookupEntry(summon_spell->EffectRadiusIndex[i]));
    if( radius = 0)//damn blizzard, sometimes is radius set in other effect
    for(uint8 j = 0; j < 3; j++)
    {
        radius = GetSpellRadius(sSpellRadiusStore.LookupEntry(summon_spell->EffectRadiusIndex[j]));
        if(radius > 0) break;
    }
   m_caster->GetRandomPoint(center_x,center_y,center_z,radius,px,py,pz);
    switch (spell_id)
   {
        case 49206://Summon Gargoyle
        {
            Pet* spawnCreature = new Pet(GUARDIAN_PET);//set as guardian

            Map *map = m_caster->GetMap();
            uint32 pet_number = objmgr.GeneratePetNumber();
		if(!spawnCreature->Create(objmgr.GenerateLowGuid(HIGHGUID_PET), map, m_caster->GetPhaseMask(),
			m_spellInfo->EffectMiscValue[i], pet_number))
            {
                sLog.outError("no such creature entry %u in database, you should add it",summon_entry);
                summon_entry = 7560;
		if(!spawnCreature->Create(objmgr.GenerateLowGuid(HIGHGUID_PET), map, m_caster->GetPhaseMask(),
			m_spellInfo->EffectMiscValue[i], pet_number))
                    return;//this should never happen
            }
            spawnCreature->Relocate(px,py,pz,m_caster->GetOrientation());
            if(duration > 0)
                spawnCreature->SetDuration(duration);
            spawnCreature->SetUInt64Value(UNIT_FIELD_SUMMONEDBY,m_caster->GetGUID());
            spawnCreature->setPowerType(POWER_MANA);
            spawnCreature->SetMaxPower(POWER_MANA,28 + 10 * level);
            spawnCreature->SetPower(   POWER_MANA,28 + 10 * level);
            spawnCreature->SetUInt32Value(UNIT_NPC_FLAGS , 0);
            spawnCreature->SetLevel(level);
            spawnCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE,m_caster->getFaction());
            spawnCreature->SetUInt32Value(UNIT_FIELD_FLAGS,0);
            spawnCreature->SetUInt32Value(UNIT_FIELD_BYTES_1,0);
            spawnCreature->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP,0);
            spawnCreature->SetUInt32Value(UNIT_FIELD_PETEXPERIENCE,0);
            spawnCreature->SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP,1000);
            spawnCreature->SetUInt64Value(UNIT_FIELD_CREATEDBY, m_caster->GetGUID());
            spawnCreature->SetUInt32Value(UNIT_CREATED_BY_SPELL, m_spellInfo->Id);
            spawnCreature->SetArmor(level * 110) ;//TODO : find correct value
            spawnCreature->SetMaxHealth( level * 110 );//TODO : find correct value
            spawnCreature->SetHealth( level * 110 );//TODO : find correct value
            spawnCreature->SetAttackTime(BASE_ATTACK, 2000); //TODO : find correct value
            spawnCreature->SetBaseWeaponDamage(BASE_ATTACK ,MAXDAMAGE, float(level * 3));
            spawnCreature->SetBaseWeaponDamage(BASE_ATTACK ,MINDAMAGE, float(level * 2));
            spawnCreature->SetFloatValue(UNIT_FIELD_MAXDAMAGE,  float(level * 3));
            spawnCreature->SetFloatValue(UNIT_FIELD_MINDAMAGE,  float(level * 2));
            spawnCreature->AIM_Initialize();//use AI (guardian AI)
            map->Add((Creature*)spawnCreature);
            return;
        }
		case 52150://Raise Dead
        {
            Pet* spawnCreature = new Pet(GUARDIAN_PET);//set as guardian

            Map *map = m_caster->GetMap();
            uint32 pet_number = objmgr.GeneratePetNumber();
		if(!spawnCreature->Create(objmgr.GenerateLowGuid(HIGHGUID_PET), map,m_caster->GetPhaseMask(),
			m_spellInfo->EffectMiscValue[i], pet_number))
            {
                sLog.outError("no such creature entry %u in database, you should add it",summon_entry);
                summon_entry = 7560;
		if(!spawnCreature->Create(objmgr.GenerateLowGuid(HIGHGUID_PET), map,m_caster->GetPhaseMask(),
			m_spellInfo->EffectMiscValue[i], pet_number))
                    return;//this should never happen
            }
            spawnCreature->Relocate(m_caster->GetPositionX(),m_caster->GetPositionY(),m_caster->GetPositionZ(),m_caster->GetOrientation());
            if(duration > 0)
            spawnCreature->SetDuration(duration);
            spawnCreature->SetUInt64Value(UNIT_FIELD_SUMMONEDBY,m_caster->GetGUID());
            spawnCreature->setPowerType(POWER_MANA);
            spawnCreature->SetMaxPower(POWER_MANA,28 + 10 * level);
            spawnCreature->SetPower(   POWER_MANA,28 + 10 * level);
            spawnCreature->SetUInt32Value(UNIT_NPC_FLAGS , 0);
            spawnCreature->SetLevel(level);
            spawnCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE,m_caster->getFaction());
            spawnCreature->SetUInt32Value(UNIT_FIELD_FLAGS,0);
            spawnCreature->SetUInt32Value(UNIT_FIELD_BYTES_1,0);
            spawnCreature->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP,0);
            spawnCreature->SetUInt32Value(UNIT_FIELD_PETEXPERIENCE,0);
            spawnCreature->SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP,1000);
            spawnCreature->SetUInt64Value(UNIT_FIELD_CREATEDBY, m_caster->GetGUID());
            spawnCreature->SetUInt32Value(UNIT_CREATED_BY_SPELL, m_spellInfo->Id);
            spawnCreature->SetArmor(level * 110);//TODO : find correct value
            spawnCreature->SetMaxHealth( level * 110 );//TODO : find correct value
            spawnCreature->SetHealth( level * 110 );//TODO : find correct value
            spawnCreature->SetAttackTime(BASE_ATTACK, 2000); //TODO : find correct value
            spawnCreature->SetBaseWeaponDamage(BASE_ATTACK ,MAXDAMAGE, float(level * 2.5));
            spawnCreature->SetBaseWeaponDamage(BASE_ATTACK ,MINDAMAGE, float(level * 2));
            spawnCreature->SetFloatValue(UNIT_FIELD_MAXDAMAGE,  float(level * 2.5));
            spawnCreature->SetFloatValue(UNIT_FIELD_MINDAMAGE,  float(level * 2));
            spawnCreature->AIM_Initialize();//use AI (guardian AI)
            map->Add((Creature*)spawnCreature);
            return;
        }
        default: return;
    }
}

void Spell::EffectActivateRune(uint32  eff_idx)
{
    if(m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player *plr = (Player*)m_caster;

    if(plr->getClass() != CLASS_DEATH_KNIGHT)
        return;

    for(uint32 j = 0; j < MAX_RUNES; ++j)
    {
        if(plr->GetRuneCooldown(j) && plr->GetCurrentRune(j) == m_spellInfo->EffectMiscValue[eff_idx])
        {
            plr->SetRuneCooldown(j, 0);
        }
    }
}

void Spell::EffectTitanGrip(uint32 /*eff_idx*/)
{
    if (unitTarget && unitTarget->GetTypeId() == TYPEID_PLAYER)
        ((Player*)unitTarget)->SetCanTitanGrip(true);
}

void Spell::EffectRenamePet(uint32 /*eff_idx*/)
{
    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_UNIT ||
        !((Creature*)unitTarget)->isPet() || ((Pet*)unitTarget)->getPetType() != HUNTER_PET)
        return;

    unitTarget->SetByteValue(UNIT_FIELD_BYTES_2, 2, UNIT_RENAME_ALLOWED);
}
