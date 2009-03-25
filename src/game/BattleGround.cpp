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

#include "Object.h"
#include "Player.h"
#include "BattleGround.h"
#include "BattleGroundMgr.h"
#include "Creature.h"
#include "MapManager.h"
#include "Language.h"
#include "SpellAuras.h"
#include "ArenaTeam.h"
#include "World.h"
#include "Group.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "Util.h"
#include "Formulas.h"
#include "GridNotifiersImpl.h"

namespace MaNGOS
{
    class BattleGroundChatBuilder
    {
        public:
            BattleGroundChatBuilder(ChatMsg msgtype, int32 textId, Player const* source, va_list* args = NULL)
                : i_msgtype(msgtype), i_textId(textId), i_source(source), i_args(args) {}
            void operator()(WorldPacket& data, int32 loc_idx)
            {
                char const* text = objmgr.GetMangosString(i_textId,loc_idx);

                if(i_args)
                {
                    // we need copy va_list before use or original va_list will corrupted
                    va_list ap;
                    va_copy(ap,*i_args);

                    char str [2048];
                    vsnprintf(str,2048,text, ap );
                    va_end(ap);

                    do_helper(data,&str[0]);
                }
                else
                    do_helper(data,text);
            }
        private:
            void do_helper(WorldPacket& data, char const* text)
            {
                uint64 target_guid = i_source  ? i_source ->GetGUID() : 0;

                data << uint8(i_msgtype);
                data << uint32(LANG_UNIVERSAL);
                data << uint64(target_guid);                // there 0 for BG messages
                data << uint32(0);                          // can be chat msg group or something
                data << uint64(target_guid);
                data << uint32(strlen(text)+1);
                data << text;
                data << uint8(i_source ? i_source->chatTag() : uint8(0));
            }

            ChatMsg i_msgtype;
            int32 i_textId;
            Player const* i_source;
            va_list* i_args;
    };

    class BattleGroundYellBuilder
    {
        public:
            BattleGroundYellBuilder(uint32 language, int32 textId, Creature const* source, va_list* args = NULL)
                : i_language(language), i_textId(textId), i_source(source), i_args(args) {}
            void operator()(WorldPacket& data, int32 loc_idx)
            {
                char const* text = objmgr.GetMangosString(i_textId,loc_idx);

                if(i_args)
                {
                    // we need copy va_list before use or original va_list will corrupted
                    va_list ap;
                    va_copy(ap,*i_args);

                    char str [2048];
                    vsnprintf(str,2048,text, ap );
                    va_end(ap);

                    do_helper(data,&str[0]);
                }
                else
                    do_helper(data,text);
            }
        private:
            void do_helper(WorldPacket& data, char const* text)
            {
                //copyied from BuildMonsterChat
                data << (uint8)CHAT_MSG_MONSTER_YELL;
                data << (uint32)i_language;
                data << (uint64)i_source->GetGUID();
                data << (uint32)0;                                     //2.1.0
                data << (uint32)(strlen(i_source->GetName())+1);
                data << i_source->GetName();
                data << (uint64)0;                            //Unit Target - isn't important for bgs
                data << (uint32)strlen(text)+1;
                data << text;
                data << (uint8)0;                                      // ChatTag - for bgs allways 0?
            }

            uint32 i_language;
            int32 i_textId;
            Creature const* i_source;
            va_list* i_args;
    };


    class BattleGround2ChatBuilder
    {
        public:
            BattleGround2ChatBuilder(ChatMsg msgtype, int32 textId, Player const* source, int32 arg1, int32 arg2)
                : i_msgtype(msgtype), i_textId(textId), i_source(source), i_arg1(arg1), i_arg2(arg2) {}
            void operator()(WorldPacket& data, int32 loc_idx)
            {
                char const* text = objmgr.GetMangosString(i_textId,loc_idx);
                char const* arg1str = i_arg1 ? objmgr.GetMangosString(i_arg1,loc_idx) : "";
                char const* arg2str = i_arg2 ? objmgr.GetMangosString(i_arg2,loc_idx) : "";

                char str [2048];
                snprintf(str,2048,text, arg1str, arg2str );

                uint64 target_guid = i_source  ? i_source ->GetGUID() : 0;

                data << uint8(i_msgtype);
                data << uint32(LANG_UNIVERSAL);
                data << uint64(target_guid);                // there 0 for BG messages
                data << uint32(0);                          // can be chat msg group or something
                data << uint64(target_guid);
                data << uint32(strlen(str)+1);
                data << str;
                data << uint8(i_source ? i_source->chatTag() : uint8(0));
            }
        private:

            ChatMsg i_msgtype;
            int32 i_textId;
            Player const* i_source;
            int32 i_arg1;
            int32 i_arg2;
    };

    class BattleGround2YellBuilder
    {
        public:
            BattleGround2YellBuilder(uint32 language, int32 textId, Creature const* source, int32 arg1, int32 arg2)
                : i_language(language), i_textId(textId), i_source(source), i_arg1(arg1), i_arg2(arg2) {}
            void operator()(WorldPacket& data, int32 loc_idx)
            {
                char const* text = objmgr.GetMangosString(i_textId,loc_idx);
                char const* arg1str = i_arg1 ? objmgr.GetMangosString(i_arg1,loc_idx) : "";
                char const* arg2str = i_arg2 ? objmgr.GetMangosString(i_arg2,loc_idx) : "";

                char str [2048];
                snprintf(str,2048,text, arg1str, arg2str );
                //copyied from BuildMonsterChat
                data << (uint8)CHAT_MSG_MONSTER_YELL;
                data << (uint32)i_language;
                data << (uint64)i_source->GetGUID();
                data << (uint32)0;                                     //2.1.0
                data << (uint32)(strlen(i_source->GetName())+1);
                data << i_source->GetName();
                data << (uint64)0;                            //Unit Target - isn't important for bgs
                data << (uint32)strlen(str)+1;
                data << str;
                data << (uint8)0;                                      // ChatTag - for bgs allways 0?
            }
        private:

            uint32 i_language;
            int32 i_textId;
            Creature const* i_source;
            int32 i_arg1;
            int32 i_arg2;
    };
}                                                           // namespace MaNGOS

template<class Do>
void BattleGround::BroadcastWorker(Do& _do)
{
    for(BattleGroundPlayerMap::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if(Player *plr = ObjectAccessor::FindPlayer(MAKE_NEW_GUID(itr->first, 0, HIGHGUID_PLAYER)))
            _do(plr);
}

BattleGround::BattleGround()
{
    m_TypeID            = BattleGroundTypeId(0);
    m_InstanceID        = 0;
    m_Status            = STATUS_NONE;
    m_ClientInstanceID  = 0;
    m_EndTime           = 0;
    m_LastResurrectTime = 0;
    m_QueueId           = QUEUE_ID_MAX_LEVEL_19;
    m_InvitedAlliance   = 0;
    m_InvitedHorde      = 0;
    m_ArenaType         = 0;
    m_IsArena           = false;
    m_Winner            = 2;
    m_StartTime         = 0;
    m_Events            = 0;
    m_IsRated           = false;
    m_BuffChange        = false;
    m_Name              = "";
    m_LevelMin          = 0;
    m_LevelMax          = 0;
    m_InBGFreeSlotQueue = false;
    m_SetDeleteThis     = false;

    m_MaxPlayersPerTeam = 0;
    m_MaxPlayers        = 0;
    m_MinPlayersPerTeam = 0;
    m_MinPlayers        = 0;

    m_MapId             = 0;

    m_TeamStartLocX[BG_TEAM_ALLIANCE]   = 0;
    m_TeamStartLocX[BG_TEAM_HORDE]      = 0;

    m_TeamStartLocY[BG_TEAM_ALLIANCE]   = 0;
    m_TeamStartLocY[BG_TEAM_HORDE]      = 0;

    m_TeamStartLocZ[BG_TEAM_ALLIANCE]   = 0;
    m_TeamStartLocZ[BG_TEAM_HORDE]      = 0;

    m_TeamStartLocO[BG_TEAM_ALLIANCE]   = 0;
    m_TeamStartLocO[BG_TEAM_HORDE]      = 0;

    m_ArenaTeamIds[BG_TEAM_ALLIANCE]   = 0;
    m_ArenaTeamIds[BG_TEAM_HORDE]      = 0;

    m_ArenaTeamRatingChanges[BG_TEAM_ALLIANCE]   = 0;
    m_ArenaTeamRatingChanges[BG_TEAM_HORDE]      = 0;

    m_BgRaids[BG_TEAM_ALLIANCE]         = NULL;
    m_BgRaids[BG_TEAM_HORDE]            = NULL;

    m_PlayersCount[BG_TEAM_ALLIANCE]    = 0;
    m_PlayersCount[BG_TEAM_HORDE]       = 0;

    m_PrematureCountDown = false;
    m_PrematureCountDown = 0;

    m_StartDelayTimes[BG_STARTING_EVENT_FIRST]  = BG_START_DELAY_2M;
    m_StartDelayTimes[BG_STARTING_EVENT_SECOND] = BG_START_DELAY_1M;
    m_StartDelayTimes[BG_STARTING_EVENT_THIRD]  = BG_START_DELAY_30S;
    m_StartDelayTimes[BG_STARTING_EVENT_FOURTH] = BG_START_DELAY_NONE;
    //we must set to some default existing values
    m_StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_BG_WS_START_TWO_MINUTES;
    m_StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_WS_START_ONE_MINUTE;
    m_StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_BG_WS_START_HALF_MINUTE;
    m_StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_WS_HAS_BEGUN;
}

BattleGround::~BattleGround()
{
    // remove objects and creatures
    // (this is done automatically in mapmanager update, when the instance is reset after the reset time)
    int size = m_BgCreatures.size();
    for(int i = 0; i < size; ++i)
    {
        DelCreature(i);
    }
    size = m_BgObjects.size();
    for(int i = 0; i < size; ++i)
    {
        DelObject(i);
    }

    if(GetInstanceID())                                     // not spam by useless queries in case BG templates
    {
        // delete creature and go respawn times
        WorldDatabase.PExecute("DELETE FROM creature_respawn WHERE instance = '%u'",GetInstanceID());
        WorldDatabase.PExecute("DELETE FROM gameobject_respawn WHERE instance = '%u'",GetInstanceID());
        // delete instance from db
        CharacterDatabase.PExecute("DELETE FROM instance WHERE id = '%u'",GetInstanceID());
        // remove from battlegrounds
    }

    sBattleGroundMgr.RemoveBattleGround(GetInstanceID(), GetTypeID());
    // unload map
    if(Map * map = MapManager::Instance().FindMap(GetMapId(), GetInstanceID()))
        if(map->IsBattleGroundOrArena())
            ((BattleGroundMap*)map)->SetUnload();
    // remove from bg free slot queue
    this->RemoveFromBGFreeSlotQueue();
}

void BattleGround::Update(uint32 diff)
{
    if(!GetPlayersSize() && !GetReviveQueueSize())
        //BG is empty
        return;

    // remove offline players from bg after 5 minutes
    if( !m_OfflineQueue.empty() )
    {
        BattleGroundPlayerMap::iterator itr = m_Players.find(*(m_OfflineQueue.begin()));
        if( itr != m_Players.end() )
        {
            if( itr->second.OfflineRemoveTime <= sWorld.GetGameTime() )
            {
                RemovePlayerAtLeave(itr->first, true, true);// remove player from BG
                m_OfflineQueue.pop_front();                 // remove from offline queue
                //do not use itr for anything, because it is erased in RemovePlayerAtLeave()
            }
        }
    }

    /*********************************************************/
    /***        BATTLEGROUND RESSURECTION SYSTEM           ***/
    /*********************************************************/

    //this should be handled by spell system
    m_LastResurrectTime += diff;
    if (m_LastResurrectTime >= RESURRECTION_INTERVAL)
    {
        if(GetReviveQueueSize())
        {
            for(std::map<uint64, std::vector<uint64> >::iterator itr = m_ReviveQueue.begin(); itr != m_ReviveQueue.end(); ++itr)
            {
                Creature *sh = NULL;
                for(std::vector<uint64>::iterator itr2 = (itr->second).begin(); itr2 != (itr->second).end(); ++itr2)
                {
                    Player *plr = objmgr.GetPlayer(*itr2);
                    if(!plr)
                        continue;

                    if (!sh)
                    {
                        sh = ObjectAccessor::GetCreature(*plr, itr->first);
                        // only for visual effect
                        if (sh)
                            sh->CastSpell(sh, SPELL_SPIRIT_HEAL, true);   // Spirit Heal, effect 117
                    }

                    plr->CastSpell(plr, SPELL_RESURRECTION_VISUAL, true);   // Resurrection visual
                    m_ResurrectQueue.push_back(*itr2);
                }
                (itr->second).clear();
            }

            m_ReviveQueue.clear();
            m_LastResurrectTime = 0;
        }
        else
            // queue is clear and time passed, just update last resurrection time
            m_LastResurrectTime = 0;
    }
    else if (m_LastResurrectTime > 500)    // Resurrect players only half a second later, to see spirit heal effect on NPC
    {
        for(std::vector<uint64>::iterator itr = m_ResurrectQueue.begin(); itr != m_ResurrectQueue.end(); ++itr)
        {
            Player *plr = objmgr.GetPlayer(*itr);
            if(!plr)
                continue;
            plr->ResurrectPlayer(1.0f);
            plr->CastSpell(plr, SPELL_SPIRIT_HEAL_MANA, true);
            ObjectAccessor::Instance().ConvertCorpseForPlayer(*itr);
        }
        m_ResurrectQueue.clear();
    }

    /*********************************************************/
    /***           BATTLEGROUND BALLANCE SYSTEM            ***/
    /*********************************************************/

    // if less then minimum players are in on one side, then start premature finish timer
    if(GetStatus() == STATUS_IN_PROGRESS && !isArena() && sBattleGroundMgr.GetPrematureFinishTime() && (GetPlayersCountByTeam(ALLIANCE) < GetMinPlayersPerTeam() || GetPlayersCountByTeam(HORDE) < GetMinPlayersPerTeam()))
    {
        if(!m_PrematureCountDown)
        {
            m_PrematureCountDown = true;
            m_PrematureCountDownTimer = sBattleGroundMgr.GetPrematureFinishTime();
        }
        else if(m_PrematureCountDownTimer < diff)
        {
            // time's up!
            uint32 winner = 0;
            if( GetPlayersCountByTeam(ALLIANCE) >= GetMinPlayersPerTeam() )
                winner = ALLIANCE;
            else if( GetPlayersCountByTeam(HORDE) >= GetMinPlayersPerTeam() )
                winner = HORDE;

            EndBattleGround(winner);
            m_PrematureCountDown = false;
        }
        else
        {
            uint32 newtime = m_PrematureCountDownTimer - diff;
            // announce every minute
            if( newtime > (MINUTE * IN_MILISECONDS) )
            {
                if( newtime / (MINUTE * IN_MILISECONDS) != m_PrematureCountDownTimer / (MINUTE * IN_MILISECONDS) )
                    PSendMessageToAll(LANG_BATTLEGROUND_PREMATURE_FINISH_WARNING, CHAT_MSG_SYSTEM, NULL, (uint32)(m_PrematureCountDownTimer / (MINUTE * IN_MILISECONDS)));
            }
            else
            {
                //announce every 15 seconds
                if( newtime / (15 * IN_MILISECONDS) != m_PrematureCountDownTimer / (15 * IN_MILISECONDS) )
                    PSendMessageToAll(LANG_BATTLEGROUND_PREMATURE_FINISH_WARNING_SECS, CHAT_MSG_SYSTEM, NULL, (uint32)(m_PrematureCountDownTimer / IN_MILISECONDS));
            }
            m_PrematureCountDownTimer = newtime;
        }
    }
    else if (m_PrematureCountDown)
        m_PrematureCountDown = false;

    /*********************************************************/
    /***           BATTLEGROUND STARTING SYSTEM            ***/
    /*********************************************************/

    if (GetStatus() == STATUS_WAIT_JOIN && GetPlayersSize())
    {
        ModifyStartDelayTime(diff);

        if (!(m_Events & BG_STARTING_EVENT_1))
        {
            m_Events |= BG_STARTING_EVENT_1;

            // setup here, only when at least one player has ported to the map
            if(!SetupBattleGround())
            {
                EndNow();
                return;
            }

            StartingEventCloseDoors();
            SetStartDelayTime(m_StartDelayTimes[BG_STARTING_EVENT_FIRST]);
            //first start warning - 2 or 1 minute
            SendMessageToAll(m_StartMessageIds[BG_STARTING_EVENT_FIRST], CHAT_MSG_BG_SYSTEM_NEUTRAL);
        }
        // After 1 minute or 30 seconds, warning is signalled
        else if (GetStartDelayTime() <= m_StartDelayTimes[BG_STARTING_EVENT_SECOND] && !(m_Events & BG_STARTING_EVENT_2))
        {
            m_Events |= BG_STARTING_EVENT_2;
            SendMessageToAll(m_StartMessageIds[BG_STARTING_EVENT_SECOND], CHAT_MSG_BG_SYSTEM_NEUTRAL);
        }
        // After 30 or 15 seconds, warning is signalled
        else if (GetStartDelayTime() <= m_StartDelayTimes[BG_STARTING_EVENT_THIRD] && !(m_Events & BG_STARTING_EVENT_3))
        {
            m_Events |= BG_STARTING_EVENT_3;
            SendMessageToAll(m_StartMessageIds[BG_STARTING_EVENT_THIRD], CHAT_MSG_BG_SYSTEM_NEUTRAL);
        }
        // delay expired (atfer 2 or 1 minute)
        else if (GetStartDelayTime() <= 0 && !(m_Events & BG_STARTING_EVENT_4))
        {
            m_Events |= BG_STARTING_EVENT_4;

            StartingEventOpenDoors();

            SendMessageToAll(m_StartMessageIds[BG_STARTING_EVENT_FOURTH], CHAT_MSG_BG_SYSTEM_NEUTRAL);
            SetStatus(STATUS_IN_PROGRESS);
            SetStartDelayTime(m_StartDelayTimes[BG_STARTING_EVENT_FOURTH]);

            //remove preparation
            if( isArena() )
            {
                //TODO : add arena sound PlaySoundToAll(SOUND_ARENA_START);

                for(BattleGroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
                    if(Player *plr = objmgr.GetPlayer(itr->first))
                        plr->RemoveAurasDueToSpell(SPELL_ARENA_PREPARATION);

                CheckArenaWinConditions();
            }
            else
            {

                PlaySoundToAll(SOUND_BG_START);

                for(BattleGroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
                    if(Player* plr = objmgr.GetPlayer(itr->first))
                        plr->RemoveAurasDueToSpell(SPELL_PREPARATION);
                //Announce BG starting
                if( sWorld.getConfig(CONFIG_BATTLEGROUND_QUEUE_ANNOUNCER_ENABLE) )
                {
                    sWorld.SendWorldText(LANG_BG_STARTED_ANNOUNCE_WORLD, GetName(), GetMinLevel(), GetMaxLevel());
                }
            }
        }
    }

    /*********************************************************/
    /***           BATTLEGROUND ENDING SYSTEM              ***/
    /*********************************************************/

    if(GetStatus() == STATUS_WAIT_LEAVE)
    {
        // remove all players from battleground after 2 minutes
        m_EndTime -= diff;
        if( m_EndTime <= 0)
        {
            m_EndTime = 0;
            BattleGroundPlayerMap::iterator itr, next;
            for(itr = m_Players.begin(); itr != m_Players.end(); itr = next)
            {
                next = itr;
                ++next;
                //itr is erased here!
                RemovePlayerAtLeave(itr->first, true, true);// remove player from BG
                // do not change any battleground's private variables
            }
        }
    }

    //update start time
    m_StartTime += diff;
}

void BattleGround::SetTeamStartLoc(uint32 TeamID, float X, float Y, float Z, float O)
{
    uint8 idx = GetTeamIndexByTeamId(TeamID);
    m_TeamStartLocX[idx] = X;
    m_TeamStartLocY[idx] = Y;
    m_TeamStartLocZ[idx] = Z;
    m_TeamStartLocO[idx] = O;
}

void BattleGround::SendPacketToAll(WorldPacket *packet)
{
    for(BattleGroundPlayerMap::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        Player *plr = objmgr.GetPlayer(itr->first);
        if(plr)
            plr->GetSession()->SendPacket(packet);
        else
            sLog.outError("BattleGround: Player " I64FMTD " not found!", itr->first);
    }
}

void BattleGround::SendPacketToTeam(uint32 TeamID, WorldPacket *packet, Player *sender, bool self)
{
    for(BattleGroundPlayerMap::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        Player *plr = objmgr.GetPlayer(itr->first);

        if(!plr)
        {
            sLog.outError("BattleGround: Player " I64FMTD " not found!", itr->first);
            continue;
        }

        if(!self && sender == plr)
            continue;

        uint32 team = itr->second.Team;
        if(!team) team = plr->GetTeam();

        if(team == TeamID)
            plr->GetSession()->SendPacket(packet);
    }
}

void BattleGround::PlaySoundToAll(uint32 SoundID)
{
    WorldPacket data;
    sBattleGroundMgr.BuildPlaySoundPacket(&data, SoundID);
    SendPacketToAll(&data);
}

void BattleGround::PlaySoundToTeam(uint32 SoundID, uint32 TeamID)
{
    WorldPacket data;

    for(BattleGroundPlayerMap::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        Player *plr = objmgr.GetPlayer(itr->first);

        if(!plr)
        {
            sLog.outError("BattleGround: Player " I64FMTD " not found!", itr->first);
            continue;
        }

        uint32 team = itr->second.Team;
        if(!team) team = plr->GetTeam();

        if(team == TeamID)
        {
            sBattleGroundMgr.BuildPlaySoundPacket(&data, SoundID);
            plr->GetSession()->SendPacket(&data);
        }
    }
}

void BattleGround::CastSpellOnTeam(uint32 SpellID, uint32 TeamID)
{
    for(BattleGroundPlayerMap::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        Player *plr = objmgr.GetPlayer(itr->first);

        if(!plr)
        {
            sLog.outError("BattleGround: Player " I64FMTD " not found!", itr->first);
            continue;
        }

        uint32 team = itr->second.Team;
        if(!team) team = plr->GetTeam();

        if(team == TeamID)
            plr->CastSpell(plr, SpellID, true);
    }
}

void BattleGround::RewardHonorToTeam(uint32 Honor, uint32 TeamID)
{
    for(BattleGroundPlayerMap::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        Player *plr = objmgr.GetPlayer(itr->first);

        if(!plr)
        {
            sLog.outError("BattleGround: Player " I64FMTD " not found!", itr->first);
            continue;
        }

        uint32 team = itr->second.Team;
        if(!team) team = plr->GetTeam();

        if(team == TeamID)
            UpdatePlayerScore(plr, SCORE_BONUS_HONOR, Honor);
    }
}

void BattleGround::RewardReputationToTeam(uint32 faction_id, uint32 Reputation, uint32 TeamID)
{
    FactionEntry const* factionEntry = sFactionStore.LookupEntry(faction_id);

    if(!factionEntry)
        return;

    for(BattleGroundPlayerMap::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        Player *plr = objmgr.GetPlayer(itr->first);

        if(!plr)
        {
            sLog.outError("BattleGround: Player " I64FMTD " not found!", itr->first);
            continue;
        }

        uint32 team = itr->second.Team;
        if(!team) team = plr->GetTeam();

        if(team == TeamID)
            plr->ModifyFactionReputation(factionEntry, Reputation);
    }
}

void BattleGround::UpdateWorldState(uint32 Field, uint32 Value)
{
    WorldPacket data;
    sBattleGroundMgr.BuildUpdateWorldStatePacket(&data, Field, Value);
    SendPacketToAll(&data);
}

void BattleGround::UpdateWorldStateForPlayer(uint32 Field, uint32 Value, Player *Source)
{
    WorldPacket data;
    sBattleGroundMgr.BuildUpdateWorldStatePacket(&data, Field, Value);
    Source->GetSession()->SendPacket(&data);
}

void BattleGround::EndBattleGround(uint32 winner)
{
    this->RemoveFromBGFreeSlotQueue();

    ArenaTeam * winner_arena_team = NULL;
    ArenaTeam * loser_arena_team = NULL;
    uint32 loser_rating = 0;
    uint32 winner_rating = 0;
    WorldPacket data;
    int32 winmsg_id = 0;

    if(winner == ALLIANCE)
    {
        winmsg_id = isBattleGround() ? LANG_BG_A_WINS : LANG_ARENA_GOLD_WINS;

        PlaySoundToAll(SOUND_ALLIANCE_WINS);                // alliance wins sound

        SetWinner(WINNER_ALLIANCE);
    }
    else if(winner == HORDE)
    {
        winmsg_id = isBattleGround() ? LANG_BG_H_WINS : LANG_ARENA_GREEN_WINS;

        PlaySoundToAll(SOUND_HORDE_WINS);                   // horde wins sound

        SetWinner(WINNER_HORDE);
    }
    else
    {
        SetWinner(3);
    }

    SetStatus(STATUS_WAIT_LEAVE);
    //we must set it this way, because end time is sent in packet!
    m_EndTime = TIME_TO_AUTOREMOVE;

    // arena rating calculation
    if(isArena() && isRated())
    {
        winner_arena_team = objmgr.GetArenaTeamById(GetArenaTeamIdForTeam(winner));
        loser_arena_team = objmgr.GetArenaTeamById(GetArenaTeamIdForTeam(GetOtherTeam(winner)));
        if( winner_arena_team && loser_arena_team )
        {
            loser_rating = loser_arena_team->GetStats().rating;
            winner_rating = winner_arena_team->GetStats().rating;
            int32 winner_change = winner_arena_team->WonAgainst(loser_rating);
            int32 loser_change = loser_arena_team->LostAgainst(winner_rating);
            sLog.outDebug("--- Winner rating: %u, Loser rating: %u, Winner change: %u, Losser change: %u ---", winner_rating, loser_rating, winner_change, loser_change);
            SetArenaTeamRatingChangeForTeam(winner, winner_change);
            SetArenaTeamRatingChangeForTeam(GetOtherTeam(winner), loser_change);
        }
        else
        {
            SetArenaTeamRatingChangeForTeam(ALLIANCE, 0);
            SetArenaTeamRatingChangeForTeam(HORDE, 0);
        }
    }

    for(BattleGroundPlayerMap::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        Player *plr = objmgr.GetPlayer(itr->first);
        uint32 team = itr->second.Team;

        if(!plr)
        {
            //if rated arena match - make member lost!
            if(isArena() && isRated() && winner_arena_team && loser_arena_team)
            {
                if(team == winner)
                    winner_arena_team->OfflineMemberLost(itr->first, loser_rating);
                else
                    loser_arena_team->OfflineMemberLost(itr->first, winner_rating);
            }
            sLog.outError("BattleGround: Player " I64FMTD " not found!", itr->first);
            continue;
        }

        // should remove spirit of redemption
        if(plr->HasAuraType(SPELL_AURA_SPIRIT_OF_REDEMPTION))
            plr->RemoveSpellsCausingAura(SPELL_AURA_MOD_SHAPESHIFT);

        if(!plr->isAlive())
        {
            plr->ResurrectPlayer(1.0f);
            plr->SpawnCorpseBones();
        }
        else
        {
            //needed cause else in av some creatures will kill the players at the end
            plr->CombatStop();
            plr->getHostilRefManager().deleteReferences();
        }

        //this line is obsolete - team is set ALWAYS
        //if(!team) team = plr->GetTeam();

        // per player calculation
        if(isArena() && isRated() && winner_arena_team && loser_arena_team)
        {
            if(team == winner)
                winner_arena_team->MemberWon(plr,loser_rating);
            else
                loser_arena_team->MemberLost(plr,winner_rating);
        }

        if(team == winner)
        {
            RewardMark(plr,ITEM_WINNER_COUNT);
            RewardQuest(plr);
        }
        else
        {
            RewardMark(plr,ITEM_LOSER_COUNT);
        }

        plr->CombatStopWithPets(true);

        BlockMovement(plr);

        sBattleGroundMgr.BuildPvpLogDataPacket(&data, this);
        plr->GetSession()->SendPacket(&data);

        BattleGroundQueueTypeId bgQueueTypeId = BattleGroundMgr::BGQueueTypeId(GetTypeID(), GetArenaType());
        sBattleGroundMgr.BuildBattleGroundStatusPacket(&data, this, plr->GetBattleGroundQueueIndex(bgQueueTypeId), STATUS_IN_PROGRESS, TIME_TO_AUTOREMOVE, GetStartTime(), GetArenaType());
        plr->GetSession()->SendPacket(&data);
        plr->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND, 1);
    }

    if(isArena() && isRated() && winner_arena_team && loser_arena_team)
    {
        // update arena points only after increasing the player's match count!
        //obsolete: winner_arena_team->UpdateArenaPointsHelper();
        //obsolete: loser_arena_team->UpdateArenaPointsHelper();
        // save the stat changes
        winner_arena_team->SaveToDB();
        loser_arena_team->SaveToDB();
        // send updated arena team stats to players
        // this way all arena team members will get notified, not only the ones who participated in this match
        winner_arena_team->NotifyStatsChanged();
        loser_arena_team->NotifyStatsChanged();
    }

    // inform invited players about the removal
    sBattleGroundMgr.m_BattleGroundQueues[BattleGroundMgr::BGQueueTypeId(GetTypeID(), GetArenaType())].BGEndedRemoveInvites(this);

    if(winmsg_id)
        SendMessageToAll(winmsg_id, CHAT_MSG_BG_SYSTEM_NEUTRAL);
}

uint32 BattleGround::GetBonusHonorFromKill(uint32 kills) const
{
    //variable kills means how many honorable kills you scored (so we need kills * honor_for_one_kill)
    return MaNGOS::Honor::hk_honor_at_level(GetMaxLevel(), kills);
}

uint32 BattleGround::GetBattlemasterEntry() const
{
    switch(GetTypeID())
    {
        case BATTLEGROUND_AV: return 15972;
        case BATTLEGROUND_WS: return 14623;
        case BATTLEGROUND_AB: return 14879;
        case BATTLEGROUND_EY: return 22516;
        case BATTLEGROUND_NA: return 20200;
        default:              return 0;
    }
}

void BattleGround::RewardMark(Player *plr,uint32 count)
{
    // 'Inactive' this aura prevents the player from gaining honor points and battleground tokens
    if(plr->GetDummyAura(SPELL_AURA_PLAYER_INACTIVE))
        return;
    if(!plr || !count)
        return;

    BattleGroundMarks mark;
    switch(GetTypeID())
    {
        case BATTLEGROUND_AV:
            mark = ITEM_AV_MARK_OF_HONOR;
            break;
        case BATTLEGROUND_WS:
            mark = ITEM_WS_MARK_OF_HONOR;
            break;
        case BATTLEGROUND_AB:
            mark = ITEM_AB_MARK_OF_HONOR;
            break;
        case BATTLEGROUND_EY:
            mark = ITEM_EY_MARK_OF_HONOR;
            break;
        default:
            return;
    }

    if ( objmgr.GetItemPrototype( mark ) )
    {
        ItemPosCountVec dest;
        uint32 no_space_count = 0;
        uint8 msg = plr->CanStoreNewItem( NULL_BAG, NULL_SLOT, dest, mark, count, &no_space_count );
        if( msg != EQUIP_ERR_OK )                       // convert to possible store amount
            count -= no_space_count;

        if(!dest.empty())                // can add some
            if(Item* item = plr->StoreNewItem( dest, mark, true, 0))
                plr->SendNewItem(item,count,false,true);

        if(no_space_count > 0)
            SendRewardMarkByMail(plr,mark,no_space_count);
    }
}

void BattleGround::SendRewardMarkByMail(Player *plr,uint32 mark, uint32 count)
{
    uint32 bmEntry = GetBattlemasterEntry();
    if(!bmEntry)
        return;

    ItemPrototype const* markProto = objmgr.GetItemPrototype(mark);
    if(!markProto)
        return;

    if(Item* markItem = Item::CreateItem(mark,count,plr))
    {
        // save new item before send
        markItem->SaveToDB();                               // save for prevent lost at next mail load, if send fail then item will deleted

        // item
        MailItemsInfo mi;
        mi.AddItem(markItem->GetGUIDLow(), markItem->GetEntry(), markItem);

        // subject: item name
        std::string subject = markProto->Name1;
        int loc_idx = plr->GetSession()->GetSessionDbLocaleIndex();
        if ( loc_idx >= 0 )
            if(ItemLocale const *il = objmgr.GetItemLocale(markProto->ItemId))
                if (il->Name.size() > size_t(loc_idx) && !il->Name[loc_idx].empty())
                    subject = il->Name[loc_idx];

        // text
        std::string textFormat = plr->GetSession()->GetMangosString(LANG_BG_MARK_BY_MAIL);
        char textBuf[300];
        snprintf(textBuf,300,textFormat.c_str(),GetName(),GetName());
        uint32 itemTextId = objmgr.CreateItemText( textBuf );

        WorldSession::SendMailTo(plr, MAIL_CREATURE, MAIL_STATIONERY_NORMAL, bmEntry, plr->GetGUIDLow(), subject, itemTextId , &mi, 0, 0, MAIL_CHECK_MASK_NONE);
    }
}

void BattleGround::RewardQuest(Player *plr)
{
    // 'Inactive' this aura prevents the player from gaining honor points and battleground tokens
    if(plr->GetDummyAura(SPELL_AURA_PLAYER_INACTIVE))
        return;

    uint32 quest;
    switch(GetTypeID())
    {
        case BATTLEGROUND_AV:
            quest = SPELL_AV_QUEST_REWARD;
            break;
        case BATTLEGROUND_WS:
            quest = SPELL_WS_QUEST_REWARD;
            break;
        case BATTLEGROUND_AB:
            quest = SPELL_AB_QUEST_REWARD;
            break;
        case BATTLEGROUND_EY:
            quest = SPELL_EY_QUEST_REWARD;
            break;
        default:
            return;
    }

    plr->CastSpell(plr, quest, true);
}

void BattleGround::BlockMovement(Player *plr)
{
    plr->SetClientControl(plr, 0);                          // movement disabled NOTE: the effect will be automatically removed by client when the player is teleported from the battleground, so no need to send with uint8(1) in RemovePlayerAtLeave()
}

void BattleGround::RemovePlayerAtLeave(uint64 guid, bool Transport, bool SendPacket)
{
    uint32 team = GetPlayerTeam(guid);
    bool participant = false;
    // Remove from lists/maps
    BattleGroundPlayerMap::iterator itr = m_Players.find(guid);
    if(itr != m_Players.end())
    {
        UpdatePlayersCountByTeam(team, true);               // -1 player
        m_Players.erase(itr);
        // check if the player was a participant of the match, or only entered through gm command (goname)
        participant = true;
    }

    std::map<uint64, BattleGroundScore*>::iterator itr2 = m_PlayerScores.find(guid);
    if(itr2 != m_PlayerScores.end())
    {
        delete itr2->second;                                // delete player's score
        m_PlayerScores.erase(itr2);
    }

    RemovePlayerFromResurrectQueue(guid);

    Player *plr = objmgr.GetPlayer(guid);

    // should remove spirit of redemption
    if(plr && plr->HasAuraType(SPELL_AURA_SPIRIT_OF_REDEMPTION))
        plr->RemoveSpellsCausingAura(SPELL_AURA_MOD_SHAPESHIFT);

    if(plr && !plr->isAlive())                              // resurrect on exit
    {
        plr->ResurrectPlayer(1.0f);
        plr->SpawnCorpseBones();
    }

    RemovePlayer(plr, guid);                                // BG subclass specific code

    if(participant) // if the player was a match participant, remove auras, calc rating, update queue
    {
        BattleGroundTypeId bgTypeId = GetTypeID();
        BattleGroundQueueTypeId bgQueueTypeId = BattleGroundMgr::BGQueueTypeId(GetTypeID(), GetArenaType());
        if(plr)
        {
            plr->ClearAfkReports();

            if(!team) team = plr->GetTeam();

            // if arena, remove the specific arena auras
            if(isArena())
            {
                plr->RemoveArenaAuras(true);                // removes debuffs / dots etc., we don't want the player to die after porting out
                bgTypeId=BATTLEGROUND_AA;                   // set the bg type to all arenas (it will be used for queue refreshing)

                // summon old pet if there was one and there isn't a current pet
                if(!plr->GetPet() && plr->GetTemporaryUnsummonedPetNumber())
                {
                    Pet* NewPet = new Pet;
                    if(!NewPet->LoadPetFromDB(plr, 0, (plr)->GetTemporaryUnsummonedPetNumber(), true))
                        delete NewPet;

                    (plr)->SetTemporaryUnsummonedPetNumber(0);
                }

                if(isRated() && GetStatus() == STATUS_IN_PROGRESS)
                {
                    //left a rated match while the encounter was in progress, consider as loser
                    ArenaTeam * winner_arena_team = objmgr.GetArenaTeamById(GetArenaTeamIdForTeam(GetOtherTeam(team)));
                    ArenaTeam * loser_arena_team = objmgr.GetArenaTeamById(GetArenaTeamIdForTeam(team));
                    if(winner_arena_team && loser_arena_team)
                        loser_arena_team->MemberLost(plr,winner_arena_team->GetRating());
                }
            }
            if(SendPacket)
            {
                WorldPacket data;
                sBattleGroundMgr.BuildBattleGroundStatusPacket(&data, this, plr->GetBattleGroundQueueIndex(bgQueueTypeId), STATUS_NONE, 0, 0, 0);
                plr->GetSession()->SendPacket(&data);
            }

            // this call is important, because player, when joins to battleground, this method is not called, so it must be called when leaving bg
            plr->RemoveBattleGroundQueueId(bgQueueTypeId);
        }
        else
        // removing offline participant
        {
            if(isRated() && GetStatus() == STATUS_IN_PROGRESS)
            {
                //left a rated match while the encounter was in progress, consider as loser
                ArenaTeam * others_arena_team = objmgr.GetArenaTeamById(GetArenaTeamIdForTeam(GetOtherTeam(team)));
                ArenaTeam * players_arena_team = objmgr.GetArenaTeamById(GetArenaTeamIdForTeam(team));
                if( others_arena_team && players_arena_team )
                    players_arena_team->OfflineMemberLost(guid, others_arena_team->GetRating());
            }
        }

        // remove from raid group if player is member
        if(Group *group = GetBgRaid(team))
        {
            if( !group->RemoveMember(guid, 0) )             // group was disbanded
            {
                SetBgRaid(team, NULL);
                delete group;
            }
        }
        DecreaseInvitedCount(team);
        //we should update battleground queue, but only if bg isn't ending
        if( isBattleGround() && GetStatus() < STATUS_WAIT_LEAVE )
            sBattleGroundMgr.m_BattleGroundQueues[bgQueueTypeId].Update(bgTypeId, GetQueueId());
        // Let others know
        WorldPacket data;
        sBattleGroundMgr.BuildPlayerLeftBattleGroundPacket(&data, guid);
        SendPacketToTeam(team, &data, plr, false);
    }

    if( plr )
    {
        // Do next only if found in battleground
        plr->SetBattleGroundId(0, BATTLEGROUND_TYPE_NONE);  // We're not in BG.
        // reset destination bg team
        plr->SetBGTeam(0);

        if(Transport)
            plr->TeleportTo(plr->GetBattleGroundEntryPoint());

        sLog.outDetail("BATTLEGROUND: Removed player %s from BattleGround.", plr->GetName());
    }

    if(!GetPlayersSize() && !GetInvitedCount(HORDE) && !GetInvitedCount(ALLIANCE))
    {
        // if no players left AND no invitees left, set this bg to delete in next update
        // direct deletion could cause crashes
        m_SetDeleteThis = true;
        // return to prevent addition to freeslotqueue
        return;
    }

    // a player exited the battleground, so there are free slots. add to queue
    this->AddToBGFreeSlotQueue();
}

// this method is called when no players remains in battleground
void BattleGround::Reset()
{
    SetQueueId(QUEUE_ID_MAX_LEVEL_19);
    SetWinner(WINNER_NONE);
    SetStatus(STATUS_WAIT_QUEUE);
    SetStartTime(0);
    SetEndTime(0);
    SetLastResurrectTime(0);
    SetArenaType(0);
    SetRated(false);

    m_Events = 0;

    if (m_InvitedAlliance > 0 || m_InvitedHorde > 0)
        sLog.outError("BattleGround system ERROR: bad counter, m_InvitedAlliance: %d, m_InvitedHorde: %d", m_InvitedAlliance, m_InvitedHorde);

    m_InvitedAlliance = 0;
    m_InvitedHorde = 0;
    m_InBGFreeSlotQueue = false;

    m_Players.clear();
    m_PlayerScores.clear();
}

void BattleGround::StartBattleGround()
{
    ///this method should spawn spirit guides and so on
    SetStartTime(0);

    SetLastResurrectTime(0);
}

void BattleGround::AddPlayer(Player *plr)
{
    // score struct must be created in inherited class

    uint64 guid = plr->GetGUID();
    uint32 team = plr->GetBGTeam();

    BattleGroundPlayer bp;
    bp.OfflineRemoveTime = 0;
    bp.Team = team;

    // Add to list/maps
    m_Players[guid] = bp;

    UpdatePlayersCountByTeam(team, false);                  // +1 player

    WorldPacket data;
    sBattleGroundMgr.BuildPlayerJoinedBattleGroundPacket(&data, plr);
    SendPacketToTeam(team, &data, plr, false);

    // add arena specific auras
    if(isArena())
    {
        plr->RemoveArenaSpellCooldowns();
        plr->RemoveArenaAuras();
        plr->RemoveAllEnchantments(TEMP_ENCHANTMENT_SLOT);
        if(team == ALLIANCE)                                // gold
        {
            if(plr->GetTeam() == HORDE)
                plr->CastSpell(plr, SPELL_HORDE_GOLD_FLAG,true);
            else
                plr->CastSpell(plr, SPELL_ALLIANCE_GOLD_FLAG,true);
        }
        else                                                // green
        {
            if(plr->GetTeam() == HORDE)
                plr->CastSpell(plr, SPELL_HORDE_GREEN_FLAG,true);
            else
                plr->CastSpell(plr, SPELL_ALLIANCE_GREEN_FLAG,true);
        }

        plr->DestroyConjuredItems(true);

        Pet* pet = plr->GetPet();
        if(pet)
        {
            if(pet->getPetType() == SUMMON_PET || pet->getPetType() == HUNTER_PET)
            {
                (plr)->SetTemporaryUnsummonedPetNumber(pet->GetCharmInfo()->GetPetNumber());
                (plr)->SetOldPetSpell(pet->GetUInt32Value(UNIT_CREATED_BY_SPELL));
            }
            (plr)->RemovePet(NULL,PET_SAVE_NOT_IN_SLOT);
        }
        else
            (plr)->SetTemporaryUnsummonedPetNumber(0);

        if(GetStatus() == STATUS_WAIT_JOIN)                 // not started yet
        {
            plr->CastSpell(plr, SPELL_ARENA_PREPARATION, true);

            plr->SetHealth(plr->GetMaxHealth());
            plr->SetPower(POWER_MANA, plr->GetMaxPower(POWER_MANA));
        }
    }
    else
    {
        if(GetStatus() == STATUS_WAIT_JOIN)                 // not started yet
            plr->CastSpell(plr, SPELL_PREPARATION, true);   // reduces all mana cost of spells.
    }

    // setup BG group membership
    PlayerAddedToBGCheckIfBGIsRunning(plr);
    AddOrSetPlayerToCorrectBgGroup(plr, guid, team);

    // Log
    sLog.outDetail("BATTLEGROUND: Player %s joined the battle.", plr->GetName());
}

/* this method adds player to his team's bg group, or sets his correct group if player is already in bg group */
void BattleGround::AddOrSetPlayerToCorrectBgGroup(Player *plr, uint64 plr_guid, uint32 team)
{
    Group* group = GetBgRaid(team);
    if(!group)                                      // first player joined
    {
        group = new Group;
        SetBgRaid(team, group);
        group->Create(plr_guid, plr->GetName());
    }
    else                                            // raid already exist
    {
        if(group->IsMember(plr_guid))
        {
            uint8 subgroup = group->GetMemberGroup(plr_guid);
            plr->SetGroup(group, subgroup);
        }
        else
            GetBgRaid(team)->AddMember(plr_guid, plr->GetName());
    }
}

// This method should be called when player logs into running battleground
void BattleGround::EventPlayerLoggedIn(Player* player, uint64 plr_guid)
{
    // player is correct pointer
    for(std::deque<uint64>::iterator itr = m_OfflineQueue.begin(); itr != m_OfflineQueue.end(); ++itr)
    {
        if( *itr == plr_guid )
        {
            m_OfflineQueue.erase(itr);
            break;
        }
    }
    m_Players[plr_guid].OfflineRemoveTime = 0;
    PlayerAddedToBGCheckIfBGIsRunning(player);
    // if battleground is starting, then add preparation aura
    // we don't have to do that, because preparation aura isn't removed when player logs out
}

// This method should be called when player logs out from running battleground
void BattleGround::EventPlayerLoggedOut(Player* player)
{
    // player is correct pointer, it is checked in WorldSession::LogoutPlayer()
    m_OfflineQueue.push_back(player->GetGUID());
    m_Players[player->GetGUID()].OfflineRemoveTime = sWorld.GetGameTime() + MAX_OFFLINE_TIME;
    if( GetStatus() == STATUS_IN_PROGRESS )
    {
        if( isBattleGround() )
            EventPlayerDroppedFlag(player);
        else
            CheckArenaWinConditions();
    }
}

/* This method should be called only once ... it adds pointer to queue */
void BattleGround::AddToBGFreeSlotQueue()
{
    // make sure to add only once
    if(!m_InBGFreeSlotQueue && isBattleGround())
    {
        sBattleGroundMgr.BGFreeSlotQueue[m_TypeID].push_front(this);
        m_InBGFreeSlotQueue = true;
    }
}

/* This method removes this battleground from free queue - it must be called when deleting battleground - not used now*/
void BattleGround::RemoveFromBGFreeSlotQueue()
{
    // set to be able to re-add if needed
    m_InBGFreeSlotQueue = false;
    // uncomment this code when battlegrounds will work like instances
    for (BGFreeSlotQueueType::iterator itr = sBattleGroundMgr.BGFreeSlotQueue[m_TypeID].begin(); itr != sBattleGroundMgr.BGFreeSlotQueue[m_TypeID].end(); ++itr)
    {
        if ((*itr)->GetInstanceID() == m_InstanceID)
        {
            sBattleGroundMgr.BGFreeSlotQueue[m_TypeID].erase(itr);
            return;
        }
    }
}

// get the number of free slots for team
// returns the number how many players can join battleground to MaxPlayersPerTeam
uint32 BattleGround::GetFreeSlotsForTeam(uint32 Team) const
{
    //return free slot count to MaxPlayerPerTeam
    if (GetStatus() == STATUS_WAIT_JOIN || GetStatus() == STATUS_IN_PROGRESS)
        return (GetInvitedCount(Team) < GetMaxPlayersPerTeam()) ? GetMaxPlayersPerTeam() - GetInvitedCount(Team) : 0;

    return 0;
}

bool BattleGround::HasFreeSlots() const
{
    return GetPlayersSize() < GetMaxPlayers();
}

void BattleGround::UpdatePlayerScore(Player *Source, uint32 type, uint32 value)
{
    //this procedure is called from virtual function implemented in bg subclass
    std::map<uint64, BattleGroundScore*>::iterator itr = m_PlayerScores.find(Source->GetGUID());

    if(itr == m_PlayerScores.end())                         // player not found...
        return;

    switch(type)
    {
        case SCORE_KILLING_BLOWS:                           // Killing blows
            itr->second->KillingBlows += value;
            break;
        case SCORE_DEATHS:                                  // Deaths
            itr->second->Deaths += value;
            break;
        case SCORE_HONORABLE_KILLS:                         // Honorable kills
            itr->second->HonorableKills += value;
            break;
        case SCORE_BONUS_HONOR:                             // Honor bonus
            // do not add honor in arenas
            if(isBattleGround())
            {
                // reward honor instantly
                if(Source->RewardHonor(NULL, 1, value))
                    itr->second->BonusHonor += value;
            }
            break;
            //used only in EY, but in MSG_PVP_LOG_DATA opcode
        case SCORE_DAMAGE_DONE:                             // Damage Done
            itr->second->DamageDone += value;
            break;
        case SCORE_HEALING_DONE:                            // Healing Done
            itr->second->HealingDone += value;
            break;
        default:
            sLog.outError("BattleGround: Unknown player score type %u", type);
            break;
    }
}

void BattleGround::AddPlayerToResurrectQueue(uint64 npc_guid, uint64 player_guid)
{
    m_ReviveQueue[npc_guid].push_back(player_guid);

    Player *plr = objmgr.GetPlayer(player_guid);
    if(!plr)
        return;

    plr->CastSpell(plr, SPELL_WAITING_FOR_RESURRECT, true);
    SpellEntry const *spellInfo = sSpellStore.LookupEntry( SPELL_WAITING_FOR_RESURRECT );
    if(spellInfo)
    {
        Aura *Aur = CreateAura(spellInfo, 0, NULL, plr);
        plr->AddAura(Aur);
    }
}

void BattleGround::RemovePlayerFromResurrectQueue(uint64 player_guid)
{
    for(std::map<uint64, std::vector<uint64> >::iterator itr = m_ReviveQueue.begin(); itr != m_ReviveQueue.end(); ++itr)
    {
        for(std::vector<uint64>::iterator itr2 =(itr->second).begin(); itr2 != (itr->second).end(); ++itr2)
        {
            if(*itr2 == player_guid)
            {
                (itr->second).erase(itr2);

                Player *plr = objmgr.GetPlayer(player_guid);
                if(!plr)
                    return;

                plr->RemoveAurasDueToSpell(SPELL_WAITING_FOR_RESURRECT);

                return;
            }
        }
    }
}

bool BattleGround::AddObject(uint32 type, uint32 entry, float x, float y, float z, float o, float rotation0, float rotation1, float rotation2, float rotation3, uint32 respawnTime)
{
    Map * map = MapManager::Instance().FindMap(GetMapId(),GetInstanceID());
    if(!map)
        return false;

    // must be created this way, adding to godatamap would add it to the base map of the instance
    // and when loading it (in go::LoadFromDB()), a new guid would be assigned to the object, and a new object would be created
    // so we must create it specific for this instance
    GameObject * go = new GameObject;
    if(!go->Create(objmgr.GenerateLowGuid(HIGHGUID_GAMEOBJECT),entry, map,
        PHASEMASK_NORMAL, x,y,z,o,rotation0,rotation1,rotation2,rotation3,100,1))
    {
        sLog.outErrorDb("Gameobject template %u not found in database! BattleGround not created!", entry);
        sLog.outError("Cannot create gameobject template %u! BattleGround not created!", entry);
        delete go;
        return false;
    }
/*
    uint32 guid = go->GetGUIDLow();

    // without this, UseButtonOrDoor caused the crash, since it tried to get go info from godata
    // iirc that was changed, so adding to go data map is no longer required if that was the only function using godata from GameObject without checking if it existed
    GameObjectData& data = objmgr.NewGOData(guid);

    data.id             = entry;
    data.mapid          = GetMapId();
    data.posX           = x;
    data.posY           = y;
    data.posZ           = z;
    data.orientation    = o;
    data.rotation0      = rotation0;
    data.rotation1      = rotation1;
    data.rotation2      = rotation2;
    data.rotation3      = rotation3;
    data.spawntimesecs  = respawnTime;
    data.spawnMask      = 1;
    data.animprogress   = 100;
    data.go_state       = 1;
*/
    // add to world, so it can be later looked up from HashMapHolder
    go->AddToWorld();
    m_BgObjects[type] = go->GetGUID();
    return true;
}

//some doors aren't despawned so we cannot handle their closing in gameobject::update()
//it would be nice to correctly implement GO_ACTIVATED state and open/close doors in gameobject code
void BattleGround::DoorClose(uint32 type)
{
    GameObject *obj = HashMapHolder<GameObject>::Find(m_BgObjects[type]);
    if(obj)
    {
        //if doors are open, close it
        if( obj->getLootState() == GO_ACTIVATED && !obj->GetGoState() )
        {
            //change state to allow door to be closed
            obj->SetLootState(GO_READY);
            obj->UseDoorOrButton(RESPAWN_ONE_DAY);
        }
    }
    else
    {
        sLog.outError("BattleGround: Door object not found (cannot close doors)");
    }
}

void BattleGround::DoorOpen(uint32 type)
{
    GameObject *obj = HashMapHolder<GameObject>::Find(m_BgObjects[type]);
    if(obj)
    {
        //change state to be sure they will be opened
        obj->SetLootState(GO_READY);
        obj->UseDoorOrButton(RESPAWN_ONE_DAY);
    }
    else
    {
        sLog.outError("BattleGround: Door object not found! - doors will be closed.");
    }
}

GameObject* BattleGround::GetBGObject(uint32 type)
{
    GameObject *obj = HashMapHolder<GameObject>::Find(m_BgObjects[type]);
    if(!obj)
        sLog.outError("couldn't get gameobject %i",type);
    return obj;
}

Creature* BattleGround::GetBGCreature(uint32 type)
{
    Creature *creature = HashMapHolder<Creature>::Find(m_BgCreatures[type]);
    if(!creature)
        sLog.outError("couldn't get creature %i",type);
    return creature;
}

void BattleGround::SpawnBGObject(uint64 const& guid, uint32 respawntime)
{
    GameObject *obj = HashMapHolder<GameObject>::Find(guid);
    if(!obj)
        return;
    Map * map = MapManager::Instance().FindMap(GetMapId(),GetInstanceID());
    if(!map)
        return;
    if( respawntime == 0 )
    {
        //we need to change state from GO_JUST_DEACTIVATED to GO_READY in case battleground is starting again
        if( obj->getLootState() == GO_JUST_DEACTIVATED )
            obj->SetLootState(GO_READY);
        obj->SetRespawnTime(0);
        map->Add(obj);
    }
    else
    {
        map->Add(obj);
        obj->SetRespawnTime(respawntime);
        obj->SetLootState(GO_JUST_DEACTIVATED);
    }
}

Creature* BattleGround::AddCreature(uint32 entry, uint32 type, uint32 teamval, float x, float y, float z, float o, uint32 respawntime)
{
    Map * map = MapManager::Instance().FindMap(GetMapId(),GetInstanceID());
    if(!map)
        return NULL;

    Creature* pCreature = new Creature;
    if (!pCreature->Create(objmgr.GenerateLowGuid(HIGHGUID_UNIT), map, PHASEMASK_NORMAL, entry, teamval))
    {
        sLog.outError("Can't create creature entry: %u",entry);
        delete pCreature;
        return NULL;
    }

    pCreature->Relocate(x, y, z, o);

    if(!pCreature->IsPositionValid())
    {
        sLog.outError("ERROR: Creature (guidlow %d, entry %d) not added to battleground. Suggested coordinates isn't valid (X: %f Y: %f)",pCreature->GetGUIDLow(),pCreature->GetEntry(),pCreature->GetPositionX(),pCreature->GetPositionY());
        return NULL;
    }

    pCreature->SetDBTableGuid(pCreature->GetGUID());
    CreatureData &data = objmgr.NewOrExistCreatureData(pCreature->GetDBTableGUIDLow());
    data.posX           = x;
    data.posY           = y;
    data.posZ           = z;
    data.orientation    = o;
    data.spawndist      = 15; //for randommovement,spawning

    pCreature->AIM_Initialize();

    //pCreature->SetDungeonDifficulty(0);

    map->Add(pCreature);
    m_BgCreatures[type] = pCreature->GetGUID();

    return  pCreature;
}

void BattleGround::SpawnBGCreature(uint64 const& guid, uint32 respawntime)
{
    Creature* obj = HashMapHolder<Creature>::Find(guid);
    if(!obj)
        return;
    Map * map = MapManager::Instance().FindMap(GetMapId(),GetInstanceID());
    if(!map)
        return;
    if(respawntime == 0)
    {
        obj->SetPhaseMask(PHASEMASK_NORMAL,false);
        obj->setDeathState(ALIVE);
        obj->Respawn();
        map->Add(obj);
    }
    else
    {
        obj->SetPhaseMask(0,false);
        map->Add(obj);
    }
}

bool BattleGround::DelCreature(uint32 type)
{
    if(!m_BgCreatures[type])
        return true;

    Creature *cr = HashMapHolder<Creature>::Find(m_BgCreatures[type]);
    if(!cr)
    {
        sLog.outError("Can't find Battleground creature type:%u guid:%u",type, GUID_LOPART(m_BgCreatures[type]));
        return false;
    }
    cr->CleanupsBeforeDelete();
    cr->AddObjectToRemoveList();
    m_BgCreatures[type] = 0;
    return true;
}

bool BattleGround::DelObject(uint32 type)
{
    if(!m_BgObjects[type])
        return true;

    GameObject *obj = HashMapHolder<GameObject>::Find(m_BgObjects[type]);
    if(!obj)
    {
        sLog.outError("Can't find Battleground gobject type:%u guid:%u",type, GUID_LOPART(m_BgObjects[type]));
        return false;
    }
    obj->SetRespawnTime(0);                                 // not save respawn time
    obj->Delete();
    m_BgObjects[type] = 0;
    return true;
}

bool BattleGround::AddSpiritGuide(uint32 type, float x, float y, float z, float o, uint32 team)
{
    uint32 entry = 0;

    if(team == ALLIANCE)
        entry = 13116;
    else
        entry = 13117;

    Creature* pCreature = AddCreature(entry,type,team,x,y,z,o);
    if(!pCreature)
    {
        sLog.outError("Can't create Spirit guide. BattleGround not created!");
        EndNow();
        return false;
    }

    pCreature->setDeathState(DEAD);

    pCreature->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, pCreature->GetGUID());
    // aura
    pCreature->SetVisibleAura(0, SPELL_SPIRIT_HEAL_CHANNEL);
    //pCreature->SetUInt32Value(UNIT_FIELD_AURAFLAGS, 0x00000009);
    //pCreature->SetUInt32Value(UNIT_FIELD_AURALEVELS, 0x0000003C);
    //pCreature->SetUInt32Value(UNIT_FIELD_AURAAPPLICATIONS, 0x000000FF);
    // casting visual effect
    pCreature->SetUInt32Value(UNIT_CHANNEL_SPELL, SPELL_SPIRIT_HEAL_CHANNEL);
    // correct cast speed
    pCreature->SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);

    //pCreature->CastSpell(pCreature, SPELL_SPIRIT_HEAL_CHANNEL, true);

    return true;
}

void BattleGround::SendMessageToAll(int32 entry, ChatMsg type, Player const* source)
{
    MaNGOS::BattleGroundChatBuilder bg_builder(type, entry, source);
    MaNGOS::LocalizedPacketDo<MaNGOS::BattleGroundChatBuilder> bg_do(bg_builder);
    BroadcastWorker(bg_do);
}

void BattleGround::SendYellToAll(int32 entry, uint32 language, uint64 const& guid)
{
    Creature *source = HashMapHolder<Creature>::Find(guid);
    if(!source)
        return;
    MaNGOS::BattleGroundYellBuilder bg_builder(language, entry, source);
    MaNGOS::LocalizedPacketDo<MaNGOS::BattleGroundYellBuilder> bg_do(bg_builder);
    BroadcastWorker(bg_do);
}

void BattleGround::PSendMessageToAll(int32 entry, ChatMsg type, Player const* source, ...)
{
    va_list ap;
    va_start(ap, source);

    MaNGOS::BattleGroundChatBuilder bg_builder(type, entry, source, &ap);
    MaNGOS::LocalizedPacketDo<MaNGOS::BattleGroundChatBuilder> bg_do(bg_builder);
    BroadcastWorker(bg_do);

    va_end(ap);
}

void BattleGround::SendMessage2ToAll(int32 entry, ChatMsg type, Player const* source, int32 arg1, int32 arg2)
{
    MaNGOS::BattleGround2ChatBuilder bg_builder(type, entry, source, arg1, arg2);
    MaNGOS::LocalizedPacketDo<MaNGOS::BattleGround2ChatBuilder> bg_do(bg_builder);
    BroadcastWorker(bg_do);
}

void BattleGround::SendYell2ToAll(int32 entry, uint32 language, uint64 const& guid, int32 arg1, int32 arg2)
{
    Creature *source = HashMapHolder<Creature>::Find(guid);
    if(!source)
        return;
    MaNGOS::BattleGround2YellBuilder bg_builder(language, entry, source, arg1, arg2);
    MaNGOS::LocalizedPacketDo<MaNGOS::BattleGround2YellBuilder> bg_do(bg_builder);
    BroadcastWorker(bg_do);
}

void BattleGround::EndNow()
{
    RemoveFromBGFreeSlotQueue();
    SetStatus(STATUS_WAIT_LEAVE);
    SetEndTime(0);
    // inform invited players about the removal
    sBattleGroundMgr.m_BattleGroundQueues[BattleGroundMgr::BGQueueTypeId(GetTypeID(), GetArenaType())].BGEndedRemoveInvites(this);
}

/*
important notice:
buffs aren't spawned/despawned when players captures anything
buffs are in their positions when battleground starts
*/
void BattleGround::HandleTriggerBuff(uint64 const& go_guid)
{
    GameObject *obj = HashMapHolder<GameObject>::Find(go_guid);
    if(!obj || obj->GetGoType() != GAMEOBJECT_TYPE_TRAP || !obj->isSpawned())
        return;

    //change buff type, when buff is used:
    int32 index = m_BgObjects.size() - 1;
    while (index >= 0 && m_BgObjects[index] != go_guid)
        index--;
    if (index < 0)
    {
        sLog.outError("BattleGround (Type: %u) has buff gameobject (Guid: %u Entry: %u Type:%u) but it hasn't that object in its internal data",GetTypeID(),GUID_LOPART(go_guid),obj->GetEntry(),obj->GetGoType());
        return;
    }

    //randomly select new buff
    uint8 buff = urand(0, 2);
    uint32 entry = obj->GetEntry();
    if( m_BuffChange && entry != Buff_Entries[buff] )
    {
        //despawn current buff
        SpawnBGObject(m_BgObjects[index], RESPAWN_ONE_DAY);
        //set index for new one
        for (uint8 currBuffTypeIndex = 0; currBuffTypeIndex < 3; ++currBuffTypeIndex)
        {
            if( entry == Buff_Entries[currBuffTypeIndex] )
            {
                index -= currBuffTypeIndex;
                index += buff;
            }
        }
    }

    SpawnBGObject(m_BgObjects[index], BUFF_RESPAWN_TIME);
}

void BattleGround::HandleKillPlayer( Player *player, Player *killer )
{
    //keep in mind that for arena this will have to be changed a bit

    // add +1 deaths
    UpdatePlayerScore(player, SCORE_DEATHS, 1);

    // add +1 kills to group and +1 killing_blows to killer
    if( killer )
    {
        UpdatePlayerScore(killer, SCORE_HONORABLE_KILLS, 1);
        UpdatePlayerScore(killer, SCORE_KILLING_BLOWS, 1);

        for(BattleGroundPlayerMap::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        {
            Player *plr = objmgr.GetPlayer(itr->first);

            if(!plr || plr == killer)
                continue;

            if( plr->GetTeam() == killer->GetTeam() && plr->IsAtGroupRewardDistance(player) )
                UpdatePlayerScore(plr, SCORE_HONORABLE_KILLS, 1);
        }
    }

    // to be able to remove insignia
    player->SetFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE );
}

void BattleGround::SetHoliday(bool is_holiday)
{
    //if(is_holiday)
        //m_HonorMode = BG_HOLIDAY;
    //else
        //m_HonorMode = BG_NORMAL;
}

int32 BattleGround::GetObjectType(uint64 const& guid)
{
    for(uint32 i = 0;i <= m_BgObjects.size(); i++)
        if(m_BgObjects[i] == guid)
            return i;
    sLog.outError("BattleGround: cheating? a player used a gameobject which isnt supposed to be a usable object!");
    return -1;
}

// return the player's team based on battlegroundplayer info
// used in same faction arena matches mainly
uint32 BattleGround::GetPlayerTeam(uint64 guid)
{
    BattleGroundPlayerMap::const_iterator itr = m_Players.find(guid);
    if(itr!=m_Players.end())
        return itr->second.Team;
    return 0;
}

uint32 BattleGround::GetOtherTeam(uint32 teamId)
{
    return (teamId) ? ((teamId == ALLIANCE) ? HORDE : ALLIANCE) : 0;
}

bool BattleGround::IsPlayerInBattleGround(uint64 guid)
{
    BattleGroundPlayerMap::const_iterator itr = m_Players.find(guid);
    if(itr != m_Players.end())
        return true;
    return false;
}

void BattleGround::PlayerAddedToBGCheckIfBGIsRunning(Player* plr)
{
    if(GetStatus() != STATUS_WAIT_LEAVE)
        return;

    WorldPacket data;
    BattleGroundQueueTypeId bgQueueTypeId = BattleGroundMgr::BGQueueTypeId(GetTypeID(), GetArenaType());

    BlockMovement(plr);

    sBattleGroundMgr.BuildPvpLogDataPacket(&data, this);
    plr->GetSession()->SendPacket(&data);

    sBattleGroundMgr.BuildBattleGroundStatusPacket(&data, this, plr->GetBattleGroundQueueIndex(bgQueueTypeId), STATUS_IN_PROGRESS, GetEndTime(), GetStartTime(), GetArenaType());
    plr->GetSession()->SendPacket(&data);
}

uint32 BattleGround::GetAlivePlayersCountByTeam(uint32 Team) const
{
    int count = 0;
    for(BattleGroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        if(itr->second.Team == Team)
        {
            Player * pl = objmgr.GetPlayer(itr->first);
            if(pl && pl->isAlive())
                ++count;
        }
    }
    return count;
}

void BattleGround::CheckArenaWinConditions()
{
    if( !GetAlivePlayersCountByTeam(ALLIANCE) && GetPlayersCountByTeam(HORDE) )
        EndBattleGround(HORDE);
    else if( GetPlayersCountByTeam(ALLIANCE) && !GetAlivePlayersCountByTeam(HORDE) )
        EndBattleGround(ALLIANCE);
}

void BattleGround::SetBgRaid( uint32 TeamID, Group *bg_raid )
{
    Group* &old_raid = TeamID == ALLIANCE ? m_BgRaids[BG_TEAM_ALLIANCE] : m_BgRaids[BG_TEAM_HORDE];
    if(old_raid) old_raid->SetBattlegroundGroup(NULL);
    if(bg_raid) bg_raid->SetBattlegroundGroup(this);
    old_raid = bg_raid;
}

WorldSafeLocsEntry const* BattleGround::GetClosestGraveYard( Player* player )
{
    return objmgr.GetClosestGraveYard( player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetMapId(), player->GetTeam() );
}
