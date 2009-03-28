/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

/* ScriptData
SDName: Npc_EscortAI
SD%Complete: 100
SDComment:
SDCategory: Npc
EndScriptData */

#include "precompiled.h"
#include "npc_escortAI.h"

#define WP_LAST_POINT   -1
#define MAX_PLAYER_DISTANCE 50

bool npc_escortAI::IsVisible(Unit* who) const
{
    if (!who)
        return false;

    return (m_creature->GetDistance(who) < VISIBLE_RANGE) && who->isVisibleForOrDetect(m_creature,true);
}

void npc_escortAI::AttackStart(Unit *who)
{
    if (!who)
        return;

    if (IsBeingEscorted && !Defend)
        return;

    if (m_creature->Attack(who, true))
    {
        m_creature->AddThreat(who, 0.0f);
        m_creature->SetInCombatWith(who);
        who->SetInCombatWith(m_creature);

        if (!InCombat)
        {
            InCombat = true;

            if (IsBeingEscorted)
            {
                //Store last position
                m_creature->GetPosition(LastPos.x, LastPos.y, LastPos.z);
                debug_log("SD2: EscortAI has entered combat and stored last location.");
            }

            Aggro(who);
        }

        m_creature->GetMotionMaster()->MovementExpired();
        m_creature->GetMotionMaster()->MoveChase(who);
    }
}

void npc_escortAI::MoveInLineOfSight(Unit *who)
{
    if (IsBeingEscorted && !Attack)
        return;

    if (!m_creature->hasUnitState(UNIT_STAT_STUNNED) && who->isTargetableForAttack() && 
        m_creature->IsHostileTo(who) && who->isInAccessablePlaceFor(m_creature))
    {
        if (!m_creature->canFly() && m_creature->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
            return;

        float attackRadius = m_creature->GetAttackDistance(who);
        if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->IsWithinLOSInMap(who))
        {
            if (!m_creature->getVictim())
            {
                AttackStart(who);
                who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
            }
            else if (m_creature->GetMap()->IsDungeon())
            {
                who->SetInCombatWith(m_creature);
                m_creature->AddThreat(who, 0.0f);
            }
        }
    }
}

void npc_escortAI::JustRespawned()
{
    InCombat = false;
    IsBeingEscorted = false;
    IsOnHold = false;

    //Re-Enable gossip
    m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

    Reset();
}

void npc_escortAI::EnterEvadeMode()
{
    InCombat = false;

    m_creature->RemoveAllAuras();
    m_creature->DeleteThreatList();
    m_creature->CombatStop();
    m_creature->SetLootRecipient(NULL);

    if (IsBeingEscorted)
    {
        debug_log("SD2: EscortAI has left combat and is now returning to last point.");
        Returning = true;
        m_creature->GetMotionMaster()->MovementExpired();
        m_creature->GetMotionMaster()->MovePoint(WP_LAST_POINT, LastPos.x, LastPos.y, LastPos.z);

    }else
    {
        m_creature->GetMotionMaster()->MovementExpired();
        m_creature->GetMotionMaster()->MoveTargetedHome();
    }

    Reset();
}

void npc_escortAI::UpdateAI(const uint32 diff)
{
    //Waypoint Updating
    if (IsBeingEscorted && !InCombat && WaitTimer && !Returning)
    {
        if (WaitTimer <= diff)
        {
            if (ReconnectWP)
            {
                //Correct movement speed
                if (Run)
                    m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);
                else
                    m_creature->AddUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);

                //Continue with waypoints
                if (!IsOnHold)
                {
                    if (CurrentWP != WaypointList.end())
                    {
                        m_creature->GetMotionMaster()->MovePoint(CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z );
                        debug_log("SD2: EscortAI Reconnect WP is: %u, %f, %f, %f", CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z);

                        WaitTimer = 0;
                        ReconnectWP = false;
                        return;
                    }
                    else
                        debug_log("SD2: EscortAI Reconnected to end of WP list");
                }
            }

            //End of the line, Despawn self then immediatly respawn
            if (CurrentWP == WaypointList.end())
            {
                debug_log("SD2: EscortAI reached end of waypoints");

                m_creature->setDeathState(JUST_DIED);
                m_creature->SetHealth(0);
                m_creature->CombatStop();
                m_creature->DeleteThreatList();
                m_creature->Respawn();
                m_creature->GetMotionMaster()->Clear(true);

                //Re-Enable gossip
                m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                IsBeingEscorted = false;
                WaitTimer = 0;
                return;
            }

            if (!IsOnHold)
            {
                m_creature->GetMotionMaster()->MovePoint(CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z );
                debug_log("SD2: EscortAI Next WP is: %u, %f, %f, %f", CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z);
                WaitTimer = 0;
            }
        }else WaitTimer -= diff;
    }

    //Check if player is within range
    if (IsBeingEscorted && !InCombat && PlayerGUID)
    {
        if (PlayerTimer < diff)
        {
            Unit* p = Unit::GetUnit(*m_creature, PlayerGUID);

            if (!p || m_creature->GetDistance(p) > MAX_PLAYER_DISTANCE)
            {
                JustDied(m_creature);
                IsBeingEscorted = false;

                debug_log("SD2: EscortAI Evaded back to spawn point because player was to far away or not found");

                m_creature->setDeathState(JUST_DIED);
                m_creature->SetHealth(0);
                m_creature->CombatStop();
                m_creature->DeleteThreatList();
                m_creature->Respawn();
                m_creature->GetMotionMaster()->Clear(true);

                //Re-Enable gossip
                m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

            PlayerTimer = 1000;
        }else PlayerTimer -= diff;
    }

    //Check if we have a current target
    if (m_creature->isAlive() && m_creature->SelectHostilTarget() && m_creature->getVictim())
    {
        //If we are within range melee the target
        if (m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
        {
            if (m_creature->isAttackReady())
            {
                m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();
            }
        }
    }
}

void npc_escortAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE || !IsBeingEscorted)
        return;

    //Original position reached, continue waypoint movement
    if (id == WP_LAST_POINT)
    {
        debug_log("SD2: EscortAI has returned to original position before combat");
        ReconnectWP = true;
        Returning = false;
        WaitTimer = 1;

    }else
    {
        //Make sure that we are still on the right waypoint
        if (CurrentWP->id != id)
        {
            debug_log("SD2 ERROR: EscortAI reached waypoint out of order %d, expected %d", id, CurrentWP->id);
            return;
        }

        debug_log("SD2: EscortAI Waypoint %d reached", CurrentWP->id);

        //Call WP function
        WaypointReached(CurrentWP->id);

        WaitTimer = CurrentWP->WaitTimeMs + 1;

        ++CurrentWP;
    }
}

void npc_escortAI::AddWaypoint(uint32 id, float x, float y, float z, uint32 WaitTimeMs)
{
    Escort_Waypoint t(id, x, y, z, WaitTimeMs);

    WaypointList.push_back(t);
}

void npc_escortAI::FillPointMovementListForCreature()
{
    UNORDERED_MAP<uint32, std::vector<PointMovement> >::iterator pPointsEntries = PointMovementMap.find(m_creature->GetEntry());

    if (pPointsEntries != PointMovementMap.end())
    {
        std::vector<PointMovement>::iterator itr;

        for (itr = pPointsEntries->second.begin(); itr != pPointsEntries->second.end(); ++itr)
        {
            Escort_Waypoint pPoint(itr->m_uiPointId,itr->m_fX,itr->m_fY,itr->m_fZ,itr->m_uiWaitTime);
            WaypointList.push_back(pPoint);
        }
    }
}

void npc_escortAI::Start(bool bAttack, bool bDefend, bool bRun, uint64 pGUID)
{
    if (InCombat)
    {
        debug_log("SD2 ERROR: EscortAI attempt to Start while in combat");
        return;
    }

    if (WaypointList.empty())
    {
        debug_log("SD2 ERROR: Call to escortAI::Start with 0 waypoints");
        return;
    }

    Attack = bAttack;
    Defend = bDefend;
    Run = bRun;
    PlayerGUID = pGUID;

    debug_log("SD2: EscortAI started with %d waypoints. Attack = %d, Defend = %d, Run = %d, PlayerGUID = %d", WaypointList.size(), Attack, Defend, Run, PlayerGUID);

    CurrentWP = WaypointList.begin();

    //Set initial speed
    if (Run)
        m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);
    else m_creature->AddUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);

    //Start WP
    m_creature->GetMotionMaster()->MovePoint(CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z );
    debug_log("SD2: EscortAI Next WP is: %d, %f, %f, %f", CurrentWP->id, CurrentWP->x, CurrentWP->y, CurrentWP->z);
    IsBeingEscorted = true;
    ReconnectWP = false;
    Returning = false;
    IsOnHold = false;

    //Disable gossip
    m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
}
