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

#include "AchievementMgr.h"
#include "Common.h"
#include "Player.h"
#include "WorldPacket.h"
#include "DBCEnums.h"
#include "GameEventMgr.h"
#include "ObjectMgr.h"
#include "Guild.h"
#include "Database/DatabaseEnv.h"
#include "World.h"
#include "SpellMgr.h"
#include "ArenaTeam.h"
#include "ProgressBar.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "Language.h"

#include "Policies/SingletonImp.h"

INSTANTIATE_SINGLETON_1(AchievementGlobalMgr);

const CriteriaCastSpellRequirement AchievementGlobalMgr::m_criteriaCastSpellRequirements[CRITERIA_CAST_SPELL_REQ_COUNT] =
    {
        {5272, 3057, 0, 0},
        {5273, 2784, 0, 0},
        {5752, 9099, 0, 0},
        {5753, 8403, 0, 0},
        {5772, 0, 0, RACE_GNOME},
        {5774, 0, 0, RACE_BLOODELF},
        {5775, 0, 0, RACE_DRAENEI},
        {5776, 0, 0, RACE_DWARF},
        {5777, 0, 0, RACE_HUMAN},
        {5778, 0, 0, RACE_NIGHTELF},
        {5779, 0, 0, RACE_ORC},
        {5780, 0, 0, RACE_TAUREN},
        {5781, 0, 0, RACE_TROLL},
        {5782, 0, 0, RACE_UNDEAD_PLAYER},
        {6225, 5661, 0, 0},
        {6226, 26044, 0, 0},
        {6228, 739, 0, 0},
        {6229, 927, 0, 0},
        {6230, 1444, 0, 0},
        {6231, 8140, 0, 0},
        {6232, 5489, 0, 0},
        {6233,12336, 0, 0},
        {6234, 1351, 0, 0},
        {6235, 5484, 0, 0},
        {6236, 1182, 0, 0},
        {6237, 0, CLASS_DEATH_KNIGHT, RACE_ORC},
        {6238, 0, CLASS_WARRIOR, RACE_HUMAN},
        {6239, 0, CLASS_SHAMAN, RACE_TAUREN},
        {6240, 0, CLASS_DRUID, RACE_NIGHTELF},
        {6241, 0, CLASS_ROGUE, RACE_UNDEAD_PLAYER},
        {6242, 0, CLASS_HUNTER, RACE_TROLL},
        {6243, 0, CLASS_MAGE, RACE_GNOME},
        {6244, 0, CLASS_PALADIN, RACE_DWARF},
        {6245, 0, CLASS_WARLOCK, RACE_BLOODELF},
        {6246, 0, CLASS_PRIEST, RACE_DRAENEI},
        {6312, 0, CLASS_WARLOCK, RACE_GNOME},
        {6313, 0, CLASS_DEATH_KNIGHT, RACE_HUMAN},
        {6314, 0, CLASS_PRIEST, RACE_NIGHTELF},
        {6315, 0, CLASS_SHAMAN, RACE_ORC},
        {6316, 0, CLASS_DRUID, RACE_TAUREN},
        {6317, 0, CLASS_ROGUE, RACE_TROLL},
        {6318, 0, CLASS_WARRIOR, RACE_UNDEAD_PLAYER},
        {6319, 0, CLASS_MAGE, RACE_BLOODELF},
        {6320, 0, CLASS_PALADIN, RACE_DRAENEI},
        {6321, 0, CLASS_HUNTER, RACE_DWARF},
        {6662, 31261, 0, 0}
    };

namespace MaNGOS
{
    class AchievementChatBuilder
    {
        public:
            AchievementChatBuilder(Player const& pl, ChatMsg msgtype, int32 textId, uint32 ach_id)
                : i_player(pl), i_msgtype(msgtype), i_textId(textId), i_achievementId(ach_id) {}
            void operator()(WorldPacket& data, int32 loc_idx)
            {
                char const* text = objmgr.GetMangosString(i_textId,loc_idx);

                data << uint8(i_msgtype);
                data << uint32(LANG_UNIVERSAL);
                data << uint64(i_player.GetGUID());
                data << uint32(5);
                data << uint64(i_player.GetGUID());
                data << uint32(strlen(text)+1);
                data << text;
                data << uint8(0);
                data << uint32(i_achievementId);
            }

        private:
            Player const& i_player;
            ChatMsg i_msgtype;
            int32 i_textId;
            uint32 i_achievementId;
    };
}                                                           // namespace MaNGOS

AchievementMgr::AchievementMgr(Player *player)
{
    m_player = player;
}

AchievementMgr::~AchievementMgr()
{
}

void AchievementMgr::Reset()
{
    for(CompletedAchievementMap::iterator iter = m_completedAchievements.begin(); iter!=m_completedAchievements.end(); ++iter)
    {
        WorldPacket data(SMSG_ACHIEVEMENT_DELETED,4);
        data << uint32(iter->first);
        m_player->SendDirectMessage(&data);
    }

    for(CriteriaProgressMap::iterator iter = m_criteriaProgress.begin(); iter!=m_criteriaProgress.end(); ++iter)
    {
        WorldPacket data(SMSG_CRITERIA_DELETED,4);
        data << uint32(iter->first);
        m_player->SendDirectMessage(&data);
    }

    m_completedAchievements.clear();
    m_criteriaProgress.clear();
    DeleteFromDB(m_player->GetGUIDLow());

    // re-fill data
    CheckAllAchievementCriteria();
}

void AchievementMgr::DeleteFromDB(uint32 lowguid)
{
    CharacterDatabase.BeginTransaction ();
    CharacterDatabase.PExecute("DELETE FROM character_achievement WHERE guid = %u",lowguid);
    CharacterDatabase.PExecute("DELETE FROM character_achievement_progress WHERE guid = %u",lowguid);
    CharacterDatabase.CommitTransaction ();
}

void AchievementMgr::SaveToDB()
{
    if(!m_completedAchievements.empty())
    {
        bool need_execute = false;
        std::ostringstream ssdel;
        std::ostringstream ssins;
        for(CompletedAchievementMap::iterator iter = m_completedAchievements.begin(); iter!=m_completedAchievements.end(); ++iter)
        {
            if(!iter->second.changed)
                continue;

            /// first new/changed record prefix
            if(!need_execute)
            {
                ssdel << "DELETE FROM character_achievement WHERE guid = " << GetPlayer()->GetGUIDLow() << " AND achievement IN (";
                ssins << "INSERT INTO character_achievement (guid, achievement, date) VALUES ";
                need_execute = true;
            }
            /// next new/changed record prefix
            else
            {
                ssdel << ", ";
                ssins << ", ";
            }

            // new/changed record data
            ssdel << iter->first;
            ssins << "("<<GetPlayer()->GetGUIDLow() << ", " << iter->first << ", " << uint64(iter->second.date) << ")";

            /// mark as saved in db
            iter->second.changed = false;
        }

        if(need_execute)
            ssdel << ")";

        if(need_execute)
        {
            CharacterDatabase.Execute( ssdel.str().c_str() );
            CharacterDatabase.Execute( ssins.str().c_str() );
        }
    }

    if(!m_criteriaProgress.empty())
    {
        /// prepare deleting and insert
        bool need_execute_del = false;
        bool need_execute_ins = false;
        std::ostringstream ssdel;
        std::ostringstream ssins;
        for(CriteriaProgressMap::iterator iter = m_criteriaProgress.begin(); iter!=m_criteriaProgress.end(); ++iter)
        {
            if(!iter->second.changed)
                continue;

            // deleted data (including 0 progress state)
            {
                /// first new/changed record prefix (for any counter value)
                if(!need_execute_del)
                {
                    ssdel << "DELETE FROM character_achievement_progress WHERE guid = " << GetPlayer()->GetGUIDLow() << " AND criteria IN (";
                    need_execute_del = true;
                }
                /// next new/changed record prefix
                else
                    ssdel << ", ";

                // new/changed record data
                ssdel << iter->first;
            }

            // store data only for real progress
            if(iter->second.counter != 0)
            {
                /// first new/changed record prefix
                if(!need_execute_ins)
                {
                    ssins << "INSERT INTO character_achievement_progress (guid, criteria, counter, date) VALUES ";
                    need_execute_ins = true;
                }
                /// next new/changed record prefix
                else
                    ssins << ", ";

                // new/changed record data
                ssins << "(" << GetPlayer()->GetGUIDLow() << ", " << iter->first << ", " << iter->second.counter << ", " << iter->second.date << ")";
            }

            /// mark as updated in db
            iter->second.changed = false;
        }

        if(need_execute_del)                                // DELETE ... IN (.... _)_
            ssdel << ")";

        if(need_execute_del || need_execute_ins)
        {
            if(need_execute_del)
                CharacterDatabase.Execute( ssdel.str().c_str() );
            if(need_execute_ins)
                CharacterDatabase.Execute( ssins.str().c_str() );
        }
    }
}

void AchievementMgr::LoadFromDB(QueryResult *achievementResult, QueryResult *criteriaResult)
{
    if(achievementResult)
    {
        do
        {
            Field *fields = achievementResult->Fetch();
            CompletedAchievementData& ca = m_completedAchievements[fields[0].GetUInt32()];
            ca.date = time_t(fields[1].GetUInt64());
            ca.changed = false;
        } while(achievementResult->NextRow());
        delete achievementResult;
    }

    if(criteriaResult)
    {
        do
        {
            Field *fields = criteriaResult->Fetch();

            uint32 id      = fields[0].GetUInt32();
            uint32 counter = fields[1].GetUInt32();
            time_t date    = time_t(fields[2].GetUInt64());

            AchievementCriteriaEntry const* criteria = sAchievementCriteriaStore.LookupEntry(id);
            if (!criteria || (criteria->timeLimit && time_t(date + criteria->timeLimit) < time(NULL)))
                continue;

            CriteriaProgress& progress = m_criteriaProgress[id];
            progress.counter = counter;
            progress.date    = date;
            progress.changed = false;
        } while(criteriaResult->NextRow());
        delete criteriaResult;
    }

}

void AchievementMgr::SendAchievementEarned(AchievementEntry const* achievement)
{
    if(GetPlayer()->GetSession()->PlayerLoading())
        return;

    #ifdef MANGOS_DEBUG
    if((sLog.getLogFilter() & LOG_FILTER_ACHIEVEMENT_UPDATES)==0)
        sLog.outDebug("AchievementMgr::SendAchievementEarned(%u)", achievement->ID);
    #endif

    if(Guild* guild = objmgr.GetGuildById(GetPlayer()->GetGuildId()))
    {
        MaNGOS::AchievementChatBuilder say_builder(*GetPlayer(), CHAT_MSG_GUILD_ACHIEVEMENT, LANG_ACHIEVEMENT_EARNED,achievement->ID);
        MaNGOS::LocalizedPacketDo<MaNGOS::AchievementChatBuilder> say_do(say_builder);
        guild->BroadcastWorker(say_do,GetPlayer());
    }

    if(achievement->flags & (ACHIEVEMENT_FLAG_REALM_FIRST_KILL|ACHIEVEMENT_FLAG_REALM_FIRST_REACH))
    {
        // broadcast realm first reached
        WorldPacket data(SMSG_SERVER_FIRST_ACHIEVEMENT, strlen(GetPlayer()->GetName())+1+8+4+4);
        data << GetPlayer()->GetName();
        data << uint64(GetPlayer()->GetGUID());
        data << uint32(achievement->ID);
        data << uint32(0);                                  // 1=link supplied string as player name, 0=display plain string
        sWorld.SendGlobalMessage(&data);
    }
    else
    {
        CellPair p = MaNGOS::ComputeCellPair(GetPlayer()->GetPositionX(), GetPlayer()->GetPositionY());

        Cell cell(p);
        cell.data.Part.reserved = ALL_DISTRICT;
        cell.SetNoCreate();

        MaNGOS::AchievementChatBuilder say_builder(*GetPlayer(), CHAT_MSG_ACHIEVEMENT, LANG_ACHIEVEMENT_EARNED,achievement->ID);
        MaNGOS::LocalizedPacketDo<MaNGOS::AchievementChatBuilder> say_do(say_builder);
        MaNGOS::PlayerDistWorker<MaNGOS::LocalizedPacketDo<MaNGOS::AchievementChatBuilder> > say_worker(GetPlayer(),sWorld.getConfig(CONFIG_LISTEN_RANGE_SAY),say_do);
        TypeContainerVisitor<MaNGOS::PlayerDistWorker<MaNGOS::LocalizedPacketDo<MaNGOS::AchievementChatBuilder> >, WorldTypeMapContainer > message(say_worker);
        CellLock<GridReadGuard> cell_lock(cell, p);
        cell_lock->Visit(cell_lock, message, *GetPlayer()->GetMap());
    }

    WorldPacket data(SMSG_ACHIEVEMENT_EARNED, 8+4+8);
    data.append(GetPlayer()->GetPackGUID());
    data << uint32(achievement->ID);
    data << uint32(secsToTimeBitFields(time(NULL)));
    data << uint32(0);
    GetPlayer()->SendMessageToSetInRange(&data, sWorld.getConfig(CONFIG_LISTEN_RANGE_SAY), true);
}

void AchievementMgr::SendCriteriaUpdate(uint32 id, CriteriaProgress const* progress)
{
    WorldPacket data(SMSG_CRITERIA_UPDATE, 8+4+8);
    data << uint32(id);

    // the counter is packed like a packed Guid
    data.appendPackGUID(progress->counter);

    data.append(GetPlayer()->GetPackGUID());
    data << uint32(0);
    data << uint32(secsToTimeBitFields(progress->date));
    data << uint32(0);  // timer 1
    data << uint32(0);  // timer 2
    GetPlayer()->SendDirectMessage(&data);
}

/**
 * called at player login. The player might have fulfilled some achievements when the achievement system wasn't working yet
 */
void AchievementMgr::CheckAllAchievementCriteria()
{
    // suppress sending packets
    for(uint32 i=0; i<ACHIEVEMENT_CRITERIA_TYPE_TOTAL; ++i)
        UpdateAchievementCriteria(AchievementCriteriaTypes(i));
}

static const uint32 achievIdByArenaSlot[MAX_ARENA_SLOT] = { 1057, 1107, 1108 };
static const uint32 achievIdForDangeon[][4] =
{
    // ach_cr_id,is_dungeon,is_raid,is_heroic_dungeon
    { 321,       true,      true,   true  },
    { 916,       false,     true,   false },
    { 917,       false,     true,   false },
    { 918,       true,      false,  false },
    { 2219,      false,     false,  true  },
    { 0,         false,     false,  false }
};

/**
 * this function will be called whenever the user might have done a criteria relevant action
 */
void AchievementMgr::UpdateAchievementCriteria(AchievementCriteriaTypes type, uint32 miscvalue1, uint32 miscvalue2, Unit *unit, uint32 time)
{
    if((sLog.getLogFilter() & LOG_FILTER_ACHIEVEMENT_UPDATES)==0)
        sLog.outDetail("AchievementMgr::UpdateAchievementCriteria(%u, %u, %u, %u)", type, miscvalue1, miscvalue2, time);

    if (!sWorld.getConfig(CONFIG_GM_ALLOW_ACHIEVEMENT_GAINS) && m_player->GetSession()->GetSecurity() > SEC_PLAYER)
        return;

    AchievementCriteriaEntryList const& achievementCriteriaList = achievementmgr.GetAchievementCriteriaByType(type);
    for(AchievementCriteriaEntryList::const_iterator i = achievementCriteriaList.begin(); i!=achievementCriteriaList.end(); ++i)
    {
        AchievementCriteriaEntry const *achievementCriteria = (*i);

        // don't update already completed criteria
        if(IsCompletedCriteria(achievementCriteria))
            continue;

        if(achievementCriteria->groupFlag & ACHIEVEMENT_CRITERIA_GROUP_NOT_IN_GROUP && GetPlayer()->GetGroup())
            continue;

        AchievementEntry const *achievement = sAchievementStore.LookupEntry(achievementCriteria->referredAchievement);
        if(!achievement)
            continue;

        if ((achievement->factionFlag == ACHIEVEMENT_FACTION_FLAG_HORDE    && GetPlayer()->GetTeam() != HORDE) ||
            (achievement->factionFlag == ACHIEVEMENT_FACTION_FLAG_ALLIANCE && GetPlayer()->GetTeam() != ALLIANCE))
            continue;

        switch (type)
        {
            // std. case: increment at 1
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST:
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if(!miscvalue1)
                    continue;
                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            // std case: increment at miscvalue1
            case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TALENTS:
            case ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_QUEST_REWARD:
            case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TRAVELLING:
            case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_AT_BARBER:
            case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_MAIL:
            case ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY:
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if(!miscvalue1)
                    continue;
                SetCriteriaProgress(achievementCriteria, miscvalue1, PROGRESS_ACCUMULATE);
                break;
            // std case: high value at miscvalue1
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_BID:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_SOLD: /* FIXME: for online player only currently */
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if(!miscvalue1)
                    continue;
                SetCriteriaProgress(achievementCriteria, miscvalue1, PROGRESS_HIGHEST);
                break;

            // specialized cases

            case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE:
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if(!miscvalue1)
                    continue;
                if(achievementCriteria->kill_creature.creatureID != miscvalue1)
                    continue;

                // LOT achievement->ID required special custom checks
                switch(achievement->ID)
                {
                    // Just heroic
                    case 489: case 490: case 491: case 492: case 493: case 494: case 495:
                    case 496: case 497: case 498: case 499: case 500: case 563: case 565:
                    case 567: case 569: case 573: case 575: case 577: case 623: case 625:
                    case 667: case 668: case 669: case 670: case 671: case 672: case 673:
                    case 674: case 675: case 676: case 677: case 678: case 679: case 680:
                    case 681: case 682: case 1367: case 1368: case 1378: case 1379:
                    case 1380: case 1381: case 1382: case 1383: case 1384: case 1385:
                    case 1386: case 1387: case 1388: case 1389: case 1390: case 1393:
                    case 1394: case 1400: case 1402: case 1504: case 1505: case 1506:
                    case 1507: case 1508: case 1509: case 1510: case 1511: case 1512:
                    case 1513: case 1514: case 1515: case 1721: case 1754: case 1756:
                    case 1768: case 1817: case 1865:
                        if(GetPlayer()->GetDifficulty()!=DIFFICULTY_HEROIC)
                            continue;
                        break;
                    // Heroic + other
                    case 579: case 1296: case 1297: case 1816: case 1834: case 1857: case 1859:
                    case 1860: case 1861: case 1862: case 1864: case 1866: case 1867: case 1868:
                    case 1870: case 1871: case 1872: case 1873: case 1875: case 1877: case 1919:
                    case 2036: case 2037: case 2038: case 2039: case 2040: case 2041: case 2042:
                    case 2043: case 2044: case 2045: case 2046: case 2048: case 2052: case 2053:
                    case 2054: case 2056: case 2057: case 2058: case 2139: case 2140: case 2147:
                    case 2149: case 2150: case 2151: case 2152: case 2154: case 2155: case 2156:
                    case 2157: case 2179: case 2181: case 2183: case 2185: case 2186:
                        if(GetPlayer()->GetDifficulty()!=DIFFICULTY_HEROIC)
                            continue;
                        // FIX ME: mark as fail always until implement
                        continue;
                    // Normal + other
                    case 578: case 624: case 1790: case 1856: case 1858: case 1869: case 1874:
                    case 1996: case 1997: case 2047: case 2049: case 2050: case 2051: case 2146:
                    case 2148: case 2153: case 2178: case 2180: case 2182: case 2184: case 2187:
                        if(GetPlayer()->GetDifficulty()!=DIFFICULTY_NORMAL)
                            continue;
                        // FIX ME: mark as fail always until implement
                        continue;
                    // Just Normal
                    default:
                        if(GetPlayer()->GetDifficulty()!=DIFFICULTY_NORMAL)
                            continue;
                        break;
                };

                SetCriteriaProgress(achievementCriteria, miscvalue2, PROGRESS_ACCUMULATE);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL:
                SetCriteriaProgress(achievementCriteria, GetPlayer()->getLevel());
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL:
                // update at loading or specific skill update
                if(miscvalue1 && miscvalue1 != achievementCriteria->reach_skill_level.skillID)
                    continue;
                if(uint32 skillvalue = GetPlayer()->GetBaseSkillValue(achievementCriteria->reach_skill_level.skillID))
                    SetCriteriaProgress(achievementCriteria, skillvalue);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL:
                // update at loading or specific skill update
                if(miscvalue1 && miscvalue1 != achievementCriteria->learn_skill_level.skillID)
                    continue;
                if(uint32 maxSkillvalue = GetPlayer()->GetPureMaxSkillValue(achievementCriteria->learn_skill_level.skillID))
                    SetCriteriaProgress(achievementCriteria, maxSkillvalue);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT:
                if(m_completedAchievements.find(achievementCriteria->complete_achievement.linkedAchievement) != m_completedAchievements.end())
                    SetCriteriaProgress(achievementCriteria, 1);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT:
            {
                uint32 counter =0;
                for(QuestStatusMap::iterator itr = GetPlayer()->getQuestStatusMap().begin(); itr!=GetPlayer()->getQuestStatusMap().end(); itr++)
                    if(itr->second.m_rewarded)
                        counter++;
                SetCriteriaProgress(achievementCriteria, counter);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE:
            {
                uint32 counter =0;
                for(QuestStatusMap::iterator itr = GetPlayer()->getQuestStatusMap().begin(); itr!=GetPlayer()->getQuestStatusMap().end(); itr++)
                {
                    Quest const* quest = objmgr.GetQuestTemplate(itr->first);
                    if(itr->second.m_rewarded && quest->GetZoneOrSort() >= 0 && uint32(quest->GetZoneOrSort()) == achievementCriteria->complete_quests_in_zone.zoneID)
                        counter++;
                }
                SetCriteriaProgress(achievementCriteria, counter);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND:
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if(!miscvalue1)
                    continue;
                if(GetPlayer()->GetMapId() != achievementCriteria->complete_battleground.mapID)
                    continue;
                SetCriteriaProgress(achievementCriteria, miscvalue1, PROGRESS_ACCUMULATE);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP:
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if(!miscvalue1)
                    continue;
                if(GetPlayer()->GetMapId() != achievementCriteria->death_at_map.mapID)
                    continue;
                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_DEATH:
            {
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if(!miscvalue1)
                    continue;
                // skip wrong arena achievements, if not achievIdByArenaSlot then normal totla death counter
                bool notfit = false;
                for(int i = 0; i < MAX_ARENA_SLOT; ++i)
                {
                    if(achievIdByArenaSlot[i] == achievement->ID)
                    {
                        BattleGround* bg = GetPlayer()->GetBattleGround();
                        if(!bg || ArenaTeam::GetSlotByType(bg->GetArenaType())!=i)
                            notfit = true;

                        break;
                    }
                }
                if(notfit)
                    continue;

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON:
            {
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if(!miscvalue1)
                    continue;

                Map const* map = GetPlayer()->GetMap();
                if(!map->IsDungeon())
                    continue;

                // search case
                bool found = false;
                for(int i = 0; achievIdForDangeon[i][0]; ++i)
                {
                    if(achievIdForDangeon[i][0] == achievement->ID)
                    {
                        if(map->IsRaid())
                        {
                            // if raid accepted (ignore difficulty)
                            if(!achievIdForDangeon[i][2])
                                break;                      // for
                        }
                        else if(GetPlayer()->GetDifficulty()==DIFFICULTY_NORMAL)
                        {
                            // dungeon in normal mode accepted
                            if(!achievIdForDangeon[i][1])
                                break;                      // for
                        }
                        else
                        {
                            // dungeon in heroic mode accepted
                            if(!achievIdForDangeon[i][3])
                                break;                      // for
                        }

                        found = true;
                        break;                              // for
                    }
                }
                if(!found)
                    continue;

                //FIXME: work only for instances where max==min for players
                if(((InstanceMap*)map)->GetMaxPlayers() != achievementCriteria->death_in_dungeon.manLimit)
                    continue;
                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;

            }
            case ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE:
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if(!miscvalue1)
                    continue;
                if(miscvalue1 != achievementCriteria->killed_by_creature.creatureEntry)
                    continue;
                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_PLAYER:
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if(!miscvalue1)
                    continue;

                // if team check required: must kill by opposition faction
                if(achievement->ID==318 && miscvalue2==GetPlayer()->GetTeam())
                    continue;

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING:
            {
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if(!miscvalue1)
                    continue;
                if(achievement->ID == 1260)
                {
                    if(Player::GetDrunkenstateByValue(GetPlayer()->GetDrunkValue()) != DRUNKEN_SMASHED)
                        continue;
                    if(!IsHolidayActive(HOLIDAY_BREWFEST))
                        continue;
                }
                // miscvalue1 is the ingame fallheight*100 as stored in dbc
                SetCriteriaProgress(achievementCriteria, miscvalue1);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM:
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if(!miscvalue1)
                    continue;
                if(miscvalue2 != achievementCriteria->death_from.type)
                    continue;
                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:
                if(GetPlayer()->GetQuestRewardStatus(achievementCriteria->complete_quest.questID))
                    SetCriteriaProgress(achievementCriteria, 1);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET:
            case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2:
                if (!miscvalue1 || miscvalue1 != achievementCriteria->be_spell_target.spellID)
                    continue;
                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL:
                if (!miscvalue1 || miscvalue1 != achievementCriteria->cast_spell.spellID)
                    continue;
                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL:
                // spell always provide and at login spell learning.
                if(miscvalue1 && miscvalue1!=achievementCriteria->learn_spell.spellID)
                    continue;
                if(GetPlayer()->HasSpell(miscvalue1))
                    SetCriteriaProgress(achievementCriteria, 1);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM:
                // speedup for non-login case
                if(miscvalue1 && achievementCriteria->own_item.itemID != miscvalue1)
                    continue;
                SetCriteriaProgress(achievementCriteria, GetPlayer()->GetItemCount(achievementCriteria->own_item.itemID, true));
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM:
                // AchievementMgr::UpdateAchievementCriteria might also be called on login - skip in this case
                if(!miscvalue1)
                    continue;
                if(achievementCriteria->use_item.itemID != miscvalue1)
                    continue;
                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM:
                // You _have_ to loot that item, just owning it when logging in does _not_ count!
                if(!miscvalue1)
                    continue;
                if(miscvalue1 != achievementCriteria->own_item.itemID)
                    continue;
                SetCriteriaProgress(achievementCriteria, miscvalue2, PROGRESS_ACCUMULATE);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA:
            {
                WorldMapOverlayEntry const* worldOverlayEntry = sWorldMapOverlayStore.LookupEntry(achievementCriteria->explore_area.areaReference);
                if(!worldOverlayEntry)
                    break;

                bool matchFound = false;
                for (int i = 0; i < 3; ++i)
                {
                    int32 exploreFlag = GetAreaFlagByAreaID(worldOverlayEntry->areatableID[i]);
                    if(exploreFlag < 0)
                        break;

                    uint32 playerIndexOffset = uint32(exploreFlag) / 32;
                    uint32 mask = 1<< (uint32(exploreFlag) % 32);

                    if(GetPlayer()->GetUInt32Value(PLAYER_EXPLORED_ZONES_1 + playerIndexOffset) & mask)
                    {
                        matchFound = true;
                        break;
                    }
                }

                if(matchFound)
                    SetCriteriaProgress(achievementCriteria, 1);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT:
                SetCriteriaProgress(achievementCriteria, GetPlayer()->GetByteValue(PLAYER_BYTES_2, 2)+1);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION:
            {
                // skip faction check only at loading
                if (miscvalue1 && miscvalue1 != achievementCriteria->gain_reputation.factionID)
                    continue;

                int32 reputation = GetPlayer()->GetReputationMgr().GetReputation(achievementCriteria->gain_reputation.factionID);
                if (reputation > 0)
                    SetCriteriaProgress(achievementCriteria, reputation);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION:
            {
                SetCriteriaProgress(achievementCriteria, GetPlayer()->GetReputationMgr().GetExaltedFactionCount());
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP:
            {
                // skip for login case
                if(!miscvalue1)
                    continue;
                SetCriteriaProgress(achievementCriteria, 1);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT:
            case ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT:
            {
                // miscvalue1 = itemid
                // miscvalue2 = diced value
                if(!miscvalue1)
                    continue;
                if(miscvalue2 != achievementCriteria->roll_greed_on_loot.rollValue)
                    continue;
                ItemPrototype const *pProto = objmgr.GetItemPrototype( miscvalue1 );

                uint32 requiredItemLevel = 0;
                if (achievementCriteria->ID == 2412 || achievementCriteria->ID == 2358)
                    requiredItemLevel = 185;

                if(!pProto || pProto->ItemLevel <requiredItemLevel)
                    continue;
                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE:
            {
                // miscvalue1 = emote
                // miscvalue2 = achievement->ID for special requirement
                if(!miscvalue1)
                    continue;
                if(miscvalue1 != achievementCriteria->do_emote.emoteID)
                    continue;
                if(achievementCriteria->do_emote.count)
                {
                    // harcoded case
                    if(achievement->ID==247)
                    {
                        if (!unit || unit->GetTypeId() != TYPEID_PLAYER ||
                            unit->isAlive() || ((Player*)unit)->GetDeathTimer() == 0)
                            continue;
                    }
                    // expected as scripted case
                    else if(!miscvalue2 || !achievement->ID != miscvalue2)
                        continue;
                }

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM:
                // miscvalue1 = item_id
                if(!miscvalue1)
                    continue;
                if(miscvalue1 != achievementCriteria->equip_item.itemID)
                    continue;

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS:
            {
                // spell always provide and at login spell learning.
                if(!miscvalue1)
                    continue;
                // rescan only when change possible
                SkillLineAbilityMap::const_iterator skillIter0 = spellmgr.GetBeginSkillLineAbilityMap(miscvalue1);
                if(skillIter0 == spellmgr.GetEndSkillLineAbilityMap(miscvalue1))
                    continue;
                if(skillIter0->second->skillId != achievementCriteria->learn_skilline_spell.skillLine)
                    continue;

                uint32 spellCount = 0;
                for (PlayerSpellMap::const_iterator spellIter = GetPlayer()->GetSpellMap().begin();
                    spellIter != GetPlayer()->GetSpellMap().end();
                    ++spellIter)
                {
                    for(SkillLineAbilityMap::const_iterator skillIter = spellmgr.GetBeginSkillLineAbilityMap(spellIter->first);
                        skillIter != spellmgr.GetEndSkillLineAbilityMap(spellIter->first);
                        ++skillIter)
                    {
                        if(skillIter->second->skillId == achievementCriteria->learn_skilline_spell.skillLine)
                            spellCount++;
                    }
                }
                SetCriteriaProgress(achievementCriteria, spellCount);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REVERED_REPUTATION:
                SetCriteriaProgress(achievementCriteria, GetPlayer()->GetReputationMgr().GetReveredFactionCount());
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_GAIN_HONORED_REPUTATION:
                SetCriteriaProgress(achievementCriteria, GetPlayer()->GetReputationMgr().GetHonoredFactionCount());
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_KNOWN_FACTIONS:
                SetCriteriaProgress(achievementCriteria, GetPlayer()->GetReputationMgr().GetVisibleFactionCount());
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2:
            {
                if (!miscvalue1 || miscvalue1 != achievementCriteria->cast_spell.spellID)
                    continue;

                // those requirements couldn't be found in the dbc
                if (CriteriaCastSpellRequirement const* requirement = AchievementGlobalMgr::GetCriteriaCastSpellRequirement(achievementCriteria))
                {
                    if (!unit)
                        continue;

                    if (requirement->creatureEntry && unit->GetEntry() != requirement->creatureEntry)
                        continue;

                    if (requirement->playerRace && (unit->GetTypeId() != TYPEID_PLAYER || unit->getRace()!=requirement->playerRace))
                        continue;

                    if (requirement->playerClass && (unit->GetTypeId() != TYPEID_PLAYER || unit->getClass()!=requirement->playerClass))
                        continue;
                }

                SetCriteriaProgress(achievementCriteria, 1, PROGRESS_ACCUMULATE);
                break;
            }
            // std case: not exist in DBC, not triggered in code as result
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEALTH:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_SPELLPOWER:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_ARMOR:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_POWER:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_STAT:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_RATING:
                break;
            // FIXME: not triggered in code as result, need to implement
            case ACHIEVEMENT_CRITERIA_TYPE_WIN_BG:
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY:
            case ACHIEVEMENT_CRITERIA_TYPE_DAMAGE_DONE:
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID:
            case ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE:
            case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA:
            case ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA:
            case ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA:
            case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL:
            case ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING:
            case ACHIEVEMENT_CRITERIA_TYPE_REACH_TEAM_RATING:
            case ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK:
            case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM:
            case ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS:
            case ACHIEVEMENT_CRITERIA_TYPE_HK_RACE:
            case ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE:
            case ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS:
            case ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_VENDORS:
            case ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_TALENT_RESETS:
            case ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT:
            case ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL:
            case ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT:
            case ACHIEVEMENT_CRITERIA_TYPE_EARNED_PVP_TITLE:
            case ACHIEVEMENT_CRITERIA_TYPE_LOSE_DUEL:
            case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE:
            case ACHIEVEMENT_CRITERIA_TYPE_GOLD_EARNED_BY_AUCTIONS:
            case ACHIEVEMENT_CRITERIA_TYPE_CREATE_AUCTION:
            case ACHIEVEMENT_CRITERIA_TYPE_WON_AUCTIONS:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_GOLD_VALUE_OWNED:
            case ACHIEVEMENT_CRITERIA_TYPE_LOOT_EPIC_ITEM:
            case ACHIEVEMENT_CRITERIA_TYPE_RECEIVE_EPIC_ITEM:
            case ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED:
            case ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_DEALT:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_RECEIVED:
            case ACHIEVEMENT_CRITERIA_TYPE_TOTAL_DAMAGE_RECEIVED:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEAL_CASTED:
            case ACHIEVEMENT_CRITERIA_TYPE_TOTAL_HEALING_RECEIVED:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEALING_RECEIVED:
            case ACHIEVEMENT_CRITERIA_TYPE_QUEST_ABANDONED:
            case ACHIEVEMENT_CRITERIA_TYPE_FLIGHT_PATHS_TAKEN:
            case ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE:
            case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE:
            case ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL:
            case ACHIEVEMENT_CRITERIA_TYPE_ACCEPTED_SUMMONINGS:
            case ACHIEVEMENT_CRITERIA_TYPE_TOTAL:
                break;                                   // Not implemented yet :(
        }
        if(IsCompletedCriteria(achievementCriteria))
            CompletedCriteria(achievementCriteria);
    }
}

static const uint32 achievIdByClass[MAX_CLASSES] = { 0, 459, 465 , 462, 458, 464, 461, 467, 460, 463, 0, 466 };
static const uint32 achievIdByRace[MAX_RACES]    = { 0, 1408, 1410, 1407, 1409, 1413, 1411, 1404, 1412, 0, 1405, 1406 };

bool AchievementMgr::IsCompletedCriteria(AchievementCriteriaEntry const* achievementCriteria)
{
    AchievementEntry const* achievement = sAchievementStore.LookupEntry(achievementCriteria->referredAchievement);
    if(!achievement)
        return false;

    // counter can never complete
    if(achievement->flags & ACHIEVEMENT_FLAG_COUNTER)
        return false;

    if(achievement->flags & (ACHIEVEMENT_FLAG_REALM_FIRST_REACH | ACHIEVEMENT_FLAG_REALM_FIRST_KILL))
    {
        // someone on this realm has already completed that achievement
        if(achievementmgr.IsRealmCompleted(achievement))
            return false;
    }

    CriteriaProgressMap::const_iterator itr = m_criteriaProgress.find(achievementCriteria->ID);
    if(itr == m_criteriaProgress.end())
        return false;

    CriteriaProgress const* progress = &itr->second;

    switch(achievementCriteria->requiredType)
    {
        case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE:
            return progress->counter >= achievementCriteria->kill_creature.creatureCount;
        case ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL:
        {
            // skip wrong class achievements
            for(int i = 1; i < MAX_CLASSES; ++i)
                if(achievIdByClass[i] == achievement->ID && i != GetPlayer()->getClass())
                    return false;

            // skip wrong race achievements
            for(int i = 1; i < MAX_RACES; ++i)
                if(achievIdByRace[i] == achievement->ID && i != GetPlayer()->getRace())
                    return false;

            // appropriate class/race or not class/race specific
            return progress->counter >= achievementCriteria->reach_level.level;
        }
        case ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL:
            return progress->counter >= achievementCriteria->reach_skill_level.skillLevel;
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL:
            return progress->counter >= (achievementCriteria->learn_skill_level.skillLevel * 75);
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT:
            return progress->counter >= 1;
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT:
            return progress->counter >= achievementCriteria->complete_quest_count.totalQuestCount;
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE:
            return progress->counter >= achievementCriteria->complete_quests_in_zone.questCount;
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST:
            return progress->counter >= achievementCriteria->complete_daily_quest.questCount;
        case ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING:
            return progress->counter >= achievementCriteria->fall_without_dying.fallHeight;
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:
            return progress->counter >= 1;
        case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET:
        case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2:
            return progress->counter >= achievementCriteria->be_spell_target.spellCount;
        case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL:
        case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2:
            return progress->counter >= achievementCriteria->cast_spell.castCount;
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL:
            return progress->counter >= 1;
        case ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM:
            return progress->counter >= achievementCriteria->own_item.itemCount;
        case ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM:
            return progress->counter >= achievementCriteria->use_item.itemCount;
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM:
            return progress->counter >= achievementCriteria->loot_item.itemCount;
        case ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA:
            return progress->counter >= 1;
        case ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT:
            return progress->counter >= achievementCriteria->buy_bank_slot.numberOfSlots;
        case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION:
            return progress->counter >= achievementCriteria->gain_reputation.reputationAmount;
        case ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION:
            return progress->counter >= achievementCriteria->gain_exalted_reputation.numberOfExaltedFactions;
        case ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP:
            return progress->counter >= achievementCriteria->visit_barber.numberOfVisits;
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT:
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT:
            return progress->counter >= achievementCriteria->roll_greed_on_loot.count;
        case ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE:
            return progress->counter >= achievementCriteria->do_emote.count;
        case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM:
            return progress->counter >= achievementCriteria->equip_item.count;
        case ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_QUEST_REWARD:
            return progress->counter >= achievementCriteria->quest_reward_money.goldInCopper;
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY:
            return progress->counter >= achievementCriteria->loot_money.goldInCopper;
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS:
            return progress->counter >= achievementCriteria->learn_skilline_spell.spellCount;

        // handle all statistic-only criteria here
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND:
        case ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP:
        case ACHIEVEMENT_CRITERIA_TYPE_DEATH:
        case ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON:
        case ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE:
        case ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_PLAYER:
        case ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM:
        case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TALENTS:
        case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_AT_BARBER:
        case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_MAIL:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_BID:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_SOLD:
        case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REVERED_REPUTATION:
        case ACHIEVEMENT_CRITERIA_TYPE_GAIN_HONORED_REPUTATION:
        case ACHIEVEMENT_CRITERIA_TYPE_KNOWN_FACTIONS:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEALTH:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_SPELLPOWER:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_ARMOR:
            return false;
    }
    return false;
}

void AchievementMgr::CompletedCriteria(AchievementCriteriaEntry const* criteria)
{
    AchievementEntry const* achievement = sAchievementStore.LookupEntry(criteria->referredAchievement);
    if(!achievement)
        return;
    // counter can never complete
    if(achievement->flags & ACHIEVEMENT_FLAG_COUNTER)
        return;

    if(criteria->completionFlag & ACHIEVEMENT_CRITERIA_COMPLETE_FLAG_ALL || GetAchievementCompletionState(achievement)==ACHIEVEMENT_COMPLETED_COMPLETED_NOT_STORED)
    {
        CompletedAchievement(achievement);
    }
}

// TODO: achievement 705 requires 4 criteria to be fulfilled
AchievementCompletionState AchievementMgr::GetAchievementCompletionState(AchievementEntry const* entry)
{
    if(m_completedAchievements.find(entry->ID)!=m_completedAchievements.end())
        return ACHIEVEMENT_COMPLETED_COMPLETED_STORED;

    bool foundOutstanding = false;
    for (uint32 entryId = 0; entryId<sAchievementCriteriaStore.GetNumRows(); entryId++)
    {
         AchievementCriteriaEntry const* criteria = sAchievementCriteriaStore.LookupEntry(entryId);
         if(!criteria || criteria->referredAchievement!= entry->ID)
             continue;

         if(IsCompletedCriteria(criteria) && criteria->completionFlag & ACHIEVEMENT_CRITERIA_COMPLETE_FLAG_ALL)
             return ACHIEVEMENT_COMPLETED_COMPLETED_NOT_STORED;

         // found an umcompleted criteria, but DONT return false yet - there might be a completed criteria with ACHIEVEMENT_CRITERIA_COMPLETE_FLAG_ALL
         if(!IsCompletedCriteria(criteria))
             foundOutstanding = true;
    }
    if(foundOutstanding)
        return ACHIEVEMENT_COMPLETED_NONE;
    else
        return ACHIEVEMENT_COMPLETED_COMPLETED_NOT_STORED;
}

void AchievementMgr::SetCriteriaProgress(AchievementCriteriaEntry const* entry, uint32 changeValue, ProgressType ptype)
{
    if((sLog.getLogFilter() & LOG_FILTER_ACHIEVEMENT_UPDATES)==0)
        sLog.outDetail("AchievementMgr::SetCriteriaProgress(%u, %u) for (GUID:%u)", entry->ID, changeValue, m_player->GetGUIDLow());

    CriteriaProgress *progress = NULL;

    CriteriaProgressMap::iterator iter = m_criteriaProgress.find(entry->ID);

    if(iter == m_criteriaProgress.end())
    {
        // not create record for 0 counter
        if(changeValue == 0)
            return;

        progress = &m_criteriaProgress[entry->ID];
        progress->counter = changeValue;
        progress->date = time(NULL);
    }
    else
    {
        progress = &iter->second;

        uint32 newValue;
        switch(ptype)
        {
            case PROGRESS_SET:
                newValue = changeValue;
                break; 
            case PROGRESS_ACCUMULATE:
            {
                // avoid overflow
                uint32 max_value = std::numeric_limits<uint32>::max();
                newValue = max_value - progress->counter > changeValue ? progress->counter + changeValue : max_value;
                break; 
            }
            case PROGRESS_HIGHEST:
                newValue = progress->counter < changeValue ? changeValue : progress->counter;
                break; 
        }

        // not update (not mark as changed) if counter will have same value
        if(progress->counter == newValue)
            return;

        progress->counter = newValue;
    }

    progress->changed = true;

    if(entry->timeLimit)
    {
        time_t now = time(NULL);
        if(time_t(progress->date + entry->timeLimit) < now)
            progress->counter = 1;

        // also it seems illogical, the timeframe will be extended at every criteria update
        progress->date = now;
    }
    SendCriteriaUpdate(entry->ID,progress);
}

void AchievementMgr::CompletedAchievement(AchievementEntry const* achievement)
{
    sLog.outDetail("AchievementMgr::CompletedAchievement(%u)", achievement->ID);
    if(achievement->flags & ACHIEVEMENT_FLAG_COUNTER || m_completedAchievements.find(achievement->ID)!=m_completedAchievements.end())
        return;

    SendAchievementEarned(achievement);
    CompletedAchievementData& ca =  m_completedAchievements[achievement->ID];
    ca.date = time(NULL);
    ca.changed = true;

    // don't insert for ACHIEVEMENT_FLAG_REALM_FIRST_KILL since otherwise only the first group member would reach that achievement
    // TODO: where do set this instead?
    if(!(achievement->flags & ACHIEVEMENT_FLAG_REALM_FIRST_KILL))
        achievementmgr.SetRealmCompleted(achievement);

    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT);

    // reward items and titles if any
    AchievementReward const* reward = achievementmgr.GetAchievementReward(achievement);

    // no rewards
    if(!reward)
        return;

    // titles
    if(uint32 titleId = reward->titleId[GetPlayer()->GetTeam() == HORDE?0:1])
    {
        if(CharTitlesEntry const* titleEntry = sCharTitlesStore.LookupEntry(titleId))
            GetPlayer()->SetTitle(titleEntry);
    }

    // mail
    if(reward->sender)
    {
        Item* item = reward->itemId ? Item::CreateItem(reward->itemId,1,GetPlayer ()) : NULL;

        MailItemsInfo mi;
        if(item)
        {
            // save new item before send
            item->SaveToDB();                               // save for prevent lost at next mail load, if send fail then item will deleted

            // item
            mi.AddItem(item->GetGUIDLow(), item->GetEntry(), item);
        }

        int loc_idx = GetPlayer()->GetSession()->GetSessionDbLocaleIndex();

        // subject and text
        std::string subject = reward->subject;
        std::string text = reward->text;
        if ( loc_idx >= 0 )
        {
            if(AchievementRewardLocale const* loc = achievementmgr.GetAchievementRewardLocale(achievement))
            {
                if (loc->subject.size() > size_t(loc_idx) && !loc->subject[loc_idx].empty())
                    subject = loc->subject[loc_idx];
                if (loc->text.size() > size_t(loc_idx) && !loc->text[loc_idx].empty())
                    text = loc->text[loc_idx];
            }
        }

        uint32 itemTextId = objmgr.CreateItemText( text );

        WorldSession::SendMailTo(GetPlayer(), MAIL_CREATURE, MAIL_STATIONERY_NORMAL, reward->sender, GetPlayer()->GetGUIDLow(), subject, itemTextId , &mi, 0, 0, MAIL_CHECK_MASK_NONE);
    }
}

void AchievementMgr::SendAllAchievementData()
{
    // since we don't know the exact size of the packed GUIDs this is just an approximation
    WorldPacket data(SMSG_ALL_ACHIEVEMENT_DATA, 4*2+m_completedAchievements.size()*4*2+m_completedAchievements.size()*7*4);
    BuildAllDataPacket(&data);
    GetPlayer()->GetSession()->SendPacket(&data);
}

void AchievementMgr::SendRespondInspectAchievements(Player* player)
{
    // since we don't know the exact size of the packed GUIDs this is just an approximation
    WorldPacket data(SMSG_RESPOND_INSPECT_ACHIEVEMENTS, 4+4*2+m_completedAchievements.size()*4*2+m_completedAchievements.size()*7*4);
    data.append(GetPlayer()->GetPackGUID());
    BuildAllDataPacket(&data);
    player->GetSession()->SendPacket(&data);
}

/**
 * used by both SMSG_ALL_ACHIEVEMENT_DATA  and SMSG_RESPOND_INSPECT_ACHIEVEMENT
 */
void AchievementMgr::BuildAllDataPacket(WorldPacket *data)
{
    for(CompletedAchievementMap::const_iterator iter = m_completedAchievements.begin(); iter!=m_completedAchievements.end(); ++iter)
    {
        *data << uint32(iter->first);
        *data << uint32(secsToTimeBitFields(iter->second.date));
    }
    *data << int32(-1);

    for(CriteriaProgressMap::const_iterator iter = m_criteriaProgress.begin(); iter!=m_criteriaProgress.end(); ++iter)
    {
        *data << uint32(iter->first);
        data->appendPackGUID(iter->second.counter);
        data->append(GetPlayer()->GetPackGUID());
        *data << uint32(0);
        *data << uint32(secsToTimeBitFields(iter->second.date));
        *data << uint32(0);
        *data << uint32(0);
    }

    *data << int32(-1);
}

//==========================================================
AchievementCriteriaEntryList const& AchievementGlobalMgr::GetAchievementCriteriaByType(AchievementCriteriaTypes type)
{
    return m_AchievementCriteriasByType[type];
}

void AchievementGlobalMgr::LoadAchievementCriteriaList()
{
    if(sAchievementCriteriaStore.GetNumRows()==0)
    {
        barGoLink bar(1);
        bar.step();

        sLog.outString();
        sLog.outErrorDb(">> Loaded 0 achievement criteria.");
        return;
    }

    barGoLink bar( sAchievementCriteriaStore.GetNumRows() );
    for (uint32 entryId = 0; entryId<sAchievementCriteriaStore.GetNumRows(); entryId++)
    {
        bar.step();

        AchievementCriteriaEntry const* criteria = sAchievementCriteriaStore.LookupEntry(entryId);
        if(!criteria)
            continue;

        m_AchievementCriteriasByType[criteria->requiredType].push_back(criteria);
    }

    sLog.outString();
    sLog.outString(">> Loaded %lu achievement criteria.",(unsigned long)m_AchievementCriteriasByType->size());
}


void AchievementGlobalMgr::LoadCompletedAchievements()
{
    QueryResult *result = CharacterDatabase.Query("SELECT achievement FROM character_achievement GROUP BY achievement");

    if(!result)
    {
        barGoLink bar(1);
        bar.step();

        sLog.outString();
        sLog.outString(">> Loaded 0 realm completed achievements . DB table `character_achievement` is empty.");
        return;
    }

    barGoLink bar(result->GetRowCount());
    do
    {
        bar.step();
        Field *fields = result->Fetch();
        m_allCompletedAchievements.insert(fields[0].GetUInt32());
    } while(result->NextRow());

    delete result;

    sLog.outString();
    sLog.outString(">> Loaded %lu realm completed achievements.",(unsigned long)m_allCompletedAchievements.size());
}

void AchievementGlobalMgr::LoadRewards()
{
    m_achievementRewards.clear();                             // need for reload case

    //                                                0      1        2        3     4       5        6
    QueryResult *result = WorldDatabase.Query("SELECT entry, title_A, title_H, item, sender, subject, text FROM achievement_reward");

    if(!result)
    {
        barGoLink bar(1);

        bar.step();

        sLog.outString();
        sLog.outErrorDb(">> Loaded 0 achievement rewards. DB table `achievement_reward` is empty.");
        return;
    }

    barGoLink bar(result->GetRowCount());

    do
    {
        bar.step();

        Field *fields = result->Fetch();
        uint32 entry = fields[0].GetUInt32();
        if (!sAchievementStore.LookupEntry(entry))
        {
            sLog.outErrorDb( "Table `achievement_reward` has wrong achievement (Entry: %u), ignore", entry);
            continue;
        }

        AchievementReward reward;
        reward.titleId[0] = fields[1].GetUInt32();
        reward.titleId[1] = fields[2].GetUInt32();
        reward.itemId     = fields[3].GetUInt32();
        reward.sender     = fields[4].GetUInt32();
        reward.subject    = fields[5].GetCppString();
        reward.text       = fields[6].GetCppString();

        if ((reward.titleId[0]==0)!=(reward.titleId[1]==0))
            sLog.outErrorDb( "Table `achievement_reward` (Entry: %u) has title (A: %u H: %u) only for one from teams.", entry, reward.titleId[0], reward.titleId[1]);

        // must be title or mail at least
        if (!reward.titleId[0] && !reward.titleId[1] && !reward.sender)
        {
            sLog.outErrorDb( "Table `achievement_reward` (Entry: %u) not have title or item reward data, ignore.", entry);
            continue;
        }

        if (reward.titleId[0])
        {
            CharTitlesEntry const* titleEntry = sCharTitlesStore.LookupEntry(reward.titleId[0]);
            if (!titleEntry)
            {
                sLog.outErrorDb( "Table `achievement_reward` (Entry: %u) has invalid title id (%u) in `title_A`, set to 0", entry, reward.titleId[0]);
                reward.titleId[0] = 0;
            }
        }

        if (reward.titleId[1])
        {
            CharTitlesEntry const* titleEntry = sCharTitlesStore.LookupEntry(reward.titleId[1]);
            if (!titleEntry)
            {
                sLog.outErrorDb( "Table `achievement_reward` (Entry: %u) has invalid title id (%u) in `title_A`, set to 0", entry, reward.titleId[1]);
                reward.titleId[1] = 0;
            }
        }

        //check mail data before item for report including wrong item case
        if (reward.sender)
        {
            if (!objmgr.GetCreatureTemplate(reward.sender))
            {
                sLog.outErrorDb( "Table `achievement_reward` (Entry: %u) has invalid creature entry %u as sender, mail reward skipped.", entry, reward.sender);
                reward.sender = 0;
            }
        }
        else
        {
            if (reward.itemId)
                sLog.outErrorDb( "Table `achievement_reward` (Entry: %u) not have sender data but have item reward, item will not rewarded", entry);

            if (!reward.subject.empty())
                sLog.outErrorDb( "Table `achievement_reward` (Entry: %u) not have sender data but have mail subject.", entry);

            if (!reward.text.empty())
                sLog.outErrorDb( "Table `achievement_reward` (Entry: %u) not have sender data but have mail text.", entry);
        }

        if (reward.itemId)
        {
            if (!objmgr.GetItemPrototype(reward.itemId))
            {
                sLog.outErrorDb( "Table `achievement_reward` (Entry: %u) has invalid item id %u, reward mail will be without item.", entry, reward.itemId);
                reward.itemId = 0;
            }
        }

        m_achievementRewards[entry] = reward;

    } while (result->NextRow());

    delete result;

    sLog.outString();
    sLog.outString( ">> Loaded %lu achievement reward locale strings", (unsigned long)m_achievementRewardLocales.size() );
}

void AchievementGlobalMgr::LoadRewardLocales()
{
    m_achievementRewardLocales.clear();                       // need for reload case

    QueryResult *result = WorldDatabase.Query("SELECT entry,subject_loc1,text_loc1,subject_loc2,text_loc2,subject_loc3,text_loc3,subject_loc4,text_loc4,subject_loc5,text_loc5,subject_loc6,text_loc6,subject_loc7,text_loc7,subject_loc8,text_loc8 FROM locales_achievement_reward");

    if(!result)
    {
        barGoLink bar(1);

        bar.step();

        sLog.outString();
        sLog.outString(">> Loaded 0 achievement reward locale strings. DB table `locales_achievement_reward` is empty.");
        return;
    }

    barGoLink bar(result->GetRowCount());

    do
    {
        Field *fields = result->Fetch();
        bar.step();

        uint32 entry = fields[0].GetUInt32();

        if(m_achievementRewards.find(entry)==m_achievementRewards.end())
        {
            sLog.outErrorDb( "Table `locales_achievement_reward` (Entry: %u) has locale strings for not existed achievement reward .", entry);
            continue;
        }

        AchievementRewardLocale& data = m_achievementRewardLocales[entry];

        for(int i = 1; i < MAX_LOCALE; ++i)
        {
            std::string str = fields[1+2*(i-1)].GetCppString();
            if(!str.empty())
            {
                int idx = objmgr.GetOrNewIndexForLocale(LocaleConstant(i));
                if(idx >= 0)
                {
                    if(data.subject.size() <= size_t(idx))
                        data.subject.resize(idx+1);

                    data.subject[idx] = str;
                }
            }
            str = fields[1+2*(i-1)+1].GetCppString();
            if(!str.empty())
            {
                int idx = objmgr.GetOrNewIndexForLocale(LocaleConstant(i));
                if(idx >= 0)
                {
                    if(data.text.size() <= size_t(idx))
                        data.text.resize(idx+1);

                    data.text[idx] = str;
                }
            }
        }
    } while (result->NextRow());

    delete result;

    sLog.outString();
    sLog.outString( ">> Loaded %lu achievement reward locale strings", (unsigned long)m_achievementRewardLocales.size() );
}
