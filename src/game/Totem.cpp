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

#include "Totem.h"
#include "WorldPacket.h"
#include "Log.h"
#include "Group.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"

Totem::Totem() : Creature()
{
    m_isTotem = true;
    m_duration = 0;
    m_type = TOTEM_PASSIVE;
}

void Totem::Update( uint32 time )
{
    Unit *owner = GetOwner();
    if (!owner || !owner->isAlive() || !isAlive())
    {
        UnSummon();                                         // remove self
        return;
    }

    if (m_duration <= time)
    {
        UnSummon();                                         // remove self
        return;
    }
    else
        m_duration -= time;

        if(owner->IsPvP())
             SetPvP(true);


    Creature::Update( time );
}

void Totem::Summon(Unit* owner)
{
    sLog.outDebug("AddObject at Totem.cpp line 49");

    SetInstanceId(owner->GetInstanceId());
    owner->GetMap()->Add((Creature*)this);

    // select totem model in dependent from owner team
    CreatureInfo const *cinfo = GetCreatureInfo();
    if(owner->GetTypeId()==TYPEID_PLAYER && cinfo)
    {
        if(((Player*)owner)->GetTeam()==HORDE)
            SetDisplayId(cinfo->DisplayID_H);
        else
            SetDisplayId(cinfo->DisplayID_A);
    }

    WorldPacket data(SMSG_GAMEOBJECT_SPAWN_ANIM_OBSOLETE, 8);
    data << GetGUID();
    SendMessageToSet(&data,true);

    AIM_Initialize();

    switch(m_type)
    {
        case TOTEM_PASSIVE: CastSpell(this, GetSpell(), true); break;
        case TOTEM_STATUE:  CastSpell(GetOwner(), GetSpell(), true); break;
        default: break;
    }
}

void Totem::UnSummon()
{
    SendObjectDeSpawnAnim(GetGUID());

    CombatStop();
    RemoveAurasDueToSpell(GetSpell());
    Unit *owner = GetOwner();
    if (owner)
    {
        // clear owenr's totem slot
        for(int i = 0; i < MAX_TOTEM; ++i)
        {
            if(owner->m_TotemSlot[i]==GetGUID())
            {
                owner->m_TotemSlot[i] = 0;
                break;
            }
        }

        owner->RemoveAurasDueToSpell(GetSpell());

        //remove aura all party members too
        Group *pGroup = NULL;
        if (owner->GetTypeId() == TYPEID_PLAYER)
        {
            // Not only the player can summon the totem (scripted AI)
            pGroup = ((Player*)owner)->GetGroup();
            if (pGroup)
            {
                for(GroupReference *itr = pGroup->GetFirstMember(); itr != NULL; itr = itr->next())
                {
                    Player* Target = itr->getSource();
                    if(Target && pGroup->SameSubGroup((Player*)owner, Target))
                        Target->RemoveAurasDueToSpell(GetSpell());
                }
            }
        }
    }

    CleanupsBeforeDelete();
    AddObjectToRemoveList();
}

void Totem::SetOwner(uint64 guid)
{
    SetCreatorGUID(guid);
    SetOwnerGUID(guid);
    if (Unit *owner = GetOwner())
    {
        setFaction(owner->getFaction());
        SetLevel(owner->getLevel());
               if(owner->IsPvP())
                    SetPvP(true);
    }
}

Unit *Totem::GetOwner()
{
    uint64 ownerid = GetOwnerGUID();
    if(!ownerid)
        return NULL;
    return ObjectAccessor::GetUnit(*this, ownerid);
}

void Totem::SetTypeBySummonSpell(SpellEntry const * spellProto)
{
    // Get spell casted by totem
    SpellEntry const * totemSpell = sSpellStore.LookupEntry(GetSpell());
    if (totemSpell)
    {
        // If spell have cast time -> so its active totem
        if (GetSpellCastTime(totemSpell))
            m_type = TOTEM_ACTIVE;
    }
    if(spellProto->SpellIconID==2056)
        m_type = TOTEM_STATUE;                              //Jewelery statue
}

bool Totem::IsImmunedToSpellEffect(SpellEntry const* spellInfo, uint32 index) const
{
    // TODO: possibly all negative auras immuned?
    switch(spellInfo->EffectApplyAuraName[index])
    {
        case SPELL_AURA_PERIODIC_DAMAGE:
        case SPELL_AURA_PERIODIC_LEECH:
        case SPELL_AURA_MOD_FEAR:
        case SPELL_AURA_TRANSFORM:
            return true;
        default:
            break;
    }
    return Creature::IsImmunedToSpellEffect(spellInfo, index);
}
