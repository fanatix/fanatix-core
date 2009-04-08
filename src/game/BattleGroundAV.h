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

#ifndef __BATTLEGROUNDAV_H
#define __BATTLEGROUNDAV_H

class BattleGround;

#define BG_AV_MAX_NODE_DISTANCE             25              // distance in which players are still counted in range of a banner (for alliance towers this is calculated from the center of the tower)

#define BG_AV_BOSS_KILL_QUEST_SPELL         23658

#define BG_AV_CAPTIME                       240000          // 4 minutes
#define BG_AV_SNOWFALL_FIRSTCAP             300000          // 5 minutes but i also have seen 4:05

#define BG_AV_SCORE_INITIAL_POINTS          600
#define BG_AV_SCORE_NEAR_LOSE               120

// description: KILL = bonushonor kill one kill is 21honor worth at 0
// REP reputation, RES = ressources a team will lose
#define BG_AV_KILL_BOSS                     4
#define BG_AV_REP_BOSS                      350
#define BG_AV_REP_BOSS_HOLIDAY              525

#define BG_AV_KILL_CAPTAIN                  3
#define BG_AV_REP_CAPTAIN                   125
#define BG_AV_REP_CAPTAIN_HOLIDAY           185
#define BG_AV_RES_CAPTAIN                   100

#define BG_AV_KILL_TOWER                    3
#define BG_AV_REP_TOWER                     12
#define BG_AV_REP_TOWER_HOLIDAY             18
#define BG_AV_RES_TOWER                     75

#define BG_AV_KILL_GET_COMMANDER            1               // for a safely returned wingcommander TODO implement it

// bonushonor at the end
#define BG_AV_KILL_SURVIVING_TOWER          2
#define BG_AV_REP_SURVIVING_TOWER           12
#define BG_AV_REP_SURVIVING_TOWER_HOLIDAY   18

#define BG_AV_KILL_SURVIVING_CAPTAIN        2
#define BG_AV_REP_SURVIVING_CAPTAIN         125
#define BG_AV_REP_SURVIVING_CAPTAIN_HOLIDAY 175

#define BG_AV_KILL_MAP_COMPLETE             0
#define BG_AV_KILL_MAP_COMPLETE_HOLIDAY     4

#define BG_AV_REP_OWNED_GRAVE               12
#define BG_AV_REP_OWNED_GRAVE_HOLIDAY       18

#define BG_AV_REP_OWNED_MINE                24
#define BG_AV_REP_OWNED_MINE_HOLIDAY        36

enum BG_AV_Sounds
{
    // TODO: get out if there comes a sound when neutral team captures mine
/*
 * some of my findings, texts taken from wowwiki.com, there are also other, so you can find it there
8212:
    alliance grave assault
    alliance tower assault
    drek "mlanzenabschaum! In meiner Burg?! Toetet sie all" - always when aggroing - "Stormpike filth! In my keep?! Slay them all!"
8333:
    galv "sterbt fuer euch ist kein platz hier" - (when aggroing) - Die! Your kind has no place in Alterac Valley!

8332:
    bal "Verschwinde, dreckiger Abschaum! Die Allianz wird im Alteractal " - (when aggroing) - Begone, uncouth scum! The Alliance will prevail in Alterac Valley!
8174:
    horde tower assault
    horde grave assault
    van "es Sturmlanzenklans, euer General wird angegriffen! Ich fordere Unterst" - " Soldiers of Stormpike, your General is under attack! I require aid! Come! Come! Slay these mangy Frostwolf dogs."
8173:
    ally grave capture/defend
    tower destroy
    mine capture
    ally wins
8192:
    ally tower destroy(only iceblood - found a bug^^ - but fixed it here ;))
    ally tower  defend
    horde tower defend
8213
horde:
    grave defend/capture
    tower destroy
    mine capture
    horde wins
    */

    BG_AV_SOUND_NEAR_LOSE               = 8456,             // not confirmed yet

    BG_AV_SOUND_ALLIANCE_ASSAULTS       = 8212,             // tower,grave + enemy boss if someone tries to attack him
    BG_AV_SOUND_HORDE_ASSAULTS          = 8174,
    BG_AV_SOUND_ALLIANCE_GOOD           = 8173,             // if something good happens for the team:  wins(maybe only through killing the boss), captures mine or grave, destroys tower and defends grave
    BG_AV_SOUND_HORDE_GOOD              = 8213,
    BG_AV_SOUND_BOTH_TOWER_DEFEND       = 8192,

    BG_AV_SOUND_ALLIANCE_CAPTAIN        = 8232,             // gets called when someone attacks them and at the beginning after 3min + rand(x) * 10sec (maybe buff)
    BG_AV_SOUND_HORDE_CAPTAIN           = 8333,
};

enum BG_AV_CREATURE_ENTRIES                                 // only those, which are interesting for us
{
    BG_AV_CREATURE_ENTRY_H_CAPTAIN          = 11947,
    BG_AV_CREATURE_ENTRY_A_CAPTAIN          = 11949,
    BG_AV_CREATURE_ENTRY_H_BOSS             = 11946,
    BG_AV_CREATURE_ENTRY_A_BOSS             = 11948,
    // he yells all captures/defends... to all players
    BG_AV_CREATURE_ENTRY_N_HERALD           = 11997,

    // they are needed cause they must get despawned after destroying a tower
    BG_AV_CREATURE_ENTRY_A_MARSHAL_SOUTH    = 14763,
    BG_AV_CREATURE_ENTRY_A_MARSHAL_NORTH    = 14762,
    BG_AV_CREATURE_ENTRY_A_MARSHAL_ICE      = 14764,
    BG_AV_CREATURE_ENTRY_A_MARSHAL_STONE    = 14765,
    BG_AV_CREATURE_ENTRY_H_MARSHAL_ICE      = 14773,
    BG_AV_CREATURE_ENTRY_H_MARSHAL_TOWER    = 14776,
    BG_AV_CREATURE_ENTRY_H_MARSHAL_ETOWER   = 14772,
    BG_AV_CREATURE_ENTRY_H_MARSHAL_WTOWER   = 14777,

    BG_AV_CREATURE_ENTRY_A_GRAVE_DEFENSE_1  = 12050,
    BG_AV_CREATURE_ENTRY_A_GRAVE_DEFENSE_2  = 13326,
    BG_AV_CREATURE_ENTRY_A_GRAVE_DEFENSE_3  = 13331,
    BG_AV_CREATURE_ENTRY_A_GRAVE_DEFENSE_4  = 13422,

    BG_AV_CREATURE_ENTRY_H_GRAVE_DEFENSE_1  = 12053,
    BG_AV_CREATURE_ENTRY_H_GRAVE_DEFENSE_2  = 13328,
    BG_AV_CREATURE_ENTRY_H_GRAVE_DEFENSE_3  = 13332,
    BG_AV_CREATURE_ENTRY_H_GRAVE_DEFENSE_4  = 13421,

    BG_AV_CREATURE_ENTRY_A_TOWER_DEFENSE    = 13358,
    BG_AV_CREATURE_ENTRY_H_TOWER_DEFENSE    = 13359,
};

enum BG_AV_OTHER_VALUES
{
    BG_AV_NORTH_MINE            = 0,
    BG_AV_SOUTH_MINE            = 1,
    BG_AV_MINE_TICK_TIMER       = 45000,
    BG_AV_MINE_RECLAIM_TIMER    = 1200000,                  // TODO: get the right value.. this is currently 20 minutes
    BG_AV_NEUTRAL_TEAM          = 0                         // this is the neutral owner of snowfall
};

enum BG_AV_ObjectIds
{
    BG_AV_OBJECTID_BANNER_A             = 178925,           // can only be used by horde
    BG_AV_OBJECTID_BANNER_H             = 178943,           // can only be used by alliance
    BG_AV_OBJECTID_BANNER_CONT_A        = 178940,           // can only be used by horde
    BG_AV_OBJECTID_BANNER_CONT_H        = 179435,           // can only be used by alliance

    BG_AV_OBJECTID_BANNER_A_B           = 178365,
    BG_AV_OBJECTID_BANNER_H_B           = 178364,
    BG_AV_OBJECTID_BANNER_CONT_A_B      = 179286,
    BG_AV_OBJECTID_BANNER_CONT_H_B      = 179287,
    BG_AV_OBJECTID_BANNER_SNOWFALL_N    = 180418,

    // mine supplies
    BG_AV_OBJECTID_MINE_N               = 178785,
    BG_AV_OBJECTID_MINE_S               = 178784,
};

enum BG_AV_Nodes
{
    BG_AV_NODES_FIRSTAID_STATION        = 0,
    BG_AV_NODES_STORMPIKE_GRAVE         = 1,
    BG_AV_NODES_STONEHEART_GRAVE        = 2,
    BG_AV_NODES_SNOWFALL_GRAVE          = 3,
    BG_AV_NODES_ICEBLOOD_GRAVE          = 4,
    BG_AV_NODES_FROSTWOLF_GRAVE         = 5,
    BG_AV_NODES_FROSTWOLF_HUT           = 6,
    BG_AV_NODES_DUNBALDAR_SOUTH         = 7,
    BG_AV_NODES_DUNBALDAR_NORTH         = 8,
    BG_AV_NODES_ICEWING_BUNKER          = 9,
    BG_AV_NODES_STONEHEART_BUNKER       = 10,
    BG_AV_NODES_ICEBLOOD_TOWER          = 11,
    BG_AV_NODES_TOWER_POINT             = 12,
    BG_AV_NODES_FROSTWOLF_ETOWER        = 13,
    BG_AV_NODES_FROSTWOLF_WTOWER        = 14,
    BG_AV_NODES_ERROR                   = 255,
};
#define BG_AV_NODES_MAX                 15

/// the last dimension 2 is for 0=assaulted, 1=controlled
/// neutral node (snowfall) will get into an extra enum)
/// destroyed will be handled as controlled by opponent

/* we use (node * 4) + (2 * bg_team_id) + controlled + 1 instead of this array
 * but i think with this array it's more clear which id does what, thats why i
 * dont delete it
const uint8 BG_AV_NodeEventIndexes[BG_AV_NODES_MAX][BG_TEAMS_COUNT][2] = {
    { {1, 2}, {3, 4} },                                     // FIRSTAID_STATION
    { {5, 6}, {7,  8} },                                    // STORMPIKE_GRAVE
    { {9, 10}, {11, 12} },                                  // STONEHEART_GRAVE
    { {13, 14}, {15, 16} },                                 // SNOWFALL_GRAVE
    { {17, 18}, {19, 20} },                                 // ICEBLOOD_GRAVE
    { {21, 22}, {23, 24} },                                 // FROSTWOLF_GRAVE
    { {25, 26}, {27, 28} },                                 // FROSTWOLF_HUT
    { {29, 30}, {31, 32} },                                 // DUNBALDAR_SOUTH
    { {33, 34}, {35, 36} },                                 // DUNBALDAR_NORTH
    { {37, 38}, {39, 40} },                                 // ICEWING_BUNKER
    { {41, 42}, {43, 44} },                                 // STONEHEART_BUNKER
    { {45, 46}, {47, 48} },                                 // ICEBLOOD_TOWER
    { {49, 50}, {51, 52} },                                 // TOWER_POINT
    { {53, 54}, {55, 56} },                                 // FROSTWOLF_ETOWER
    { {57, 58}, {59, 60} }                                  // FROSTWOLF_WTOWER
};
*/
// cause snowfall is the only neutral one, i will give it an extra variable, and
// don't add neutral state to this array
#define BG_AV_NodeEventSnowfall 61                          // neutral state of snowfall
#define BG_AV_MAX_NODE_EVENTS   62

#define BG_AV_NodeEventDoors 0
#define BG_AV_NodeEventCaptainDead_A 63
#define BG_AV_NodeEventCaptainDead_H 64

/// stores x,y-position from the center of the node (for graves and horde-towers, the
/// node-banner-position), (for alliance-towers the bigbanner/bigaura position)
const float BG_AV_NodePositions[BG_AV_NODES_MAX][2] = {
    {638.592f,-32.422f },                                   // firstaid station
    {669.007f,-294.078f },                                  // stormpike
    {77.8013f,-404.7f },                                    // stone grave
    {-202.581f,-112.73f },                                  // snowfall
    {-611.962f,-396.17f },                                  // iceblood grave
    {-1082.45f,-346.823f },                                 // frostwolf grave
    {-1402.21f,-307.431f },                                 // frostwolf hut
    {555.848f,-84.4151f },                                  // dunbaldar south
    {679.339f,-136.468f },                                  // dunbaldar north
    {208.973f,-365.971f },                                  // icewing bunker
    {-155.832f,-449.401f },                                 // stoneheart bunker
    {-571.88f,-262.777f },                                  // ice tower
    {-768.907f,-363.71f },                                  // tower point
    {-1302.9f,-316.981f },                                  // frostwolf etower
    {-1297.5f,-266.767f }                                   // frostwolf wtower
};

enum BG_AV_DB_Creatures
{
    BG_AV_CREATURE_HERALD      = 1,
    BG_AV_CREATURE_A_BOSS      = 2,
    BG_AV_CREATURE_H_BOSS      = 3,
    BG_AV_CREATURE_SNIFFLE     = 4,
    BG_AV_CREATURE_MARSHAL     = 5,                         // 4alliance marshals + 4 horde marshals their ids are similar to those of the tower - ids (only here)
    BG_AV_DB_CREATURE_MAX      = 13
};

enum BG_AV_MineCreature_Entries
{
    // North_Mine_..._X 4 is always boss
    BG_AV_NORTH_MINE_NEUTRAL_1  = 10987,
    BG_AV_NORTH_MINE_NEUTRAL_2  = 11600,
    BG_AV_NORTH_MINE_NEUTRAL_3  = 11602,
    BG_AV_NORTH_MINE_NEUTRAL_4  = 11657,
    BG_AV_NORTH_MINE_ALLIANCE_1 = 13396,
    BG_AV_NORTH_MINE_ALLIANCE_2 = 13080,
    BG_AV_NORTH_MINE_ALLIANCE_3 = 13098,
    BG_AV_NORTH_MINE_ALLIANCE_4 = 13078,
    BG_AV_NORTH_MINE_HORDE_1    = 13397,
    BG_AV_NORTH_MINE_HORDE_2    = 13099,
    BG_AV_NORTH_MINE_HORDE_3    = 13081,
    BG_AV_NORTH_MINE_HORDE_4    = 13079,
    // South_Mine_..._X 4 is always boss
    BG_AV_SOUTH_MINE_NEUTRAL_1  = 11603,
    BG_AV_SOUTH_MINE_NEUTRAL_2  = 11604,
    BG_AV_SOUTH_MINE_NEUTRAL_3  = 11605,
    BG_AV_SOUTH_MINE_NEUTRAL_4  = 11677,
    BG_AV_SOUTH_MINE_NEUTRAL_5  = 10982,
    BG_AV_SOUTH_MINE_ALLIANCE_1 = 13317,
    BG_AV_SOUTH_MINE_ALLIANCE_2 = 13096,
    BG_AV_SOUTH_MINE_ALLIANCE_3 = 13087,
    BG_AV_SOUTH_MINE_ALLIANCE_4 = 13086,
    BG_AV_SOUTH_MINE_HORDE_1    = 13316,
    BG_AV_SOUTH_MINE_HORDE_2    = 13097,
    BG_AV_SOUTH_MINE_HORDE_3    = 13089,
    BG_AV_SOUTH_MINE_HORDE_4    = 13088
};

enum BG_AV_Graveyards
{
    BG_AV_GRAVE_STORM_AID          = 751,
    BG_AV_GRAVE_STORM_GRAVE        = 689,
    BG_AV_GRAVE_STONE_GRAVE        = 729,
    BG_AV_GRAVE_SNOWFALL           = 169,
    BG_AV_GRAVE_ICE_GRAVE          = 749,
    BG_AV_GRAVE_FROSTWOLF          = 690,
    BG_AV_GRAVE_FROST_HUT          = 750,
    BG_AV_GRAVE_MAIN_ALLIANCE      = 611,
    BG_AV_GRAVE_MAIN_HORDE         = 610
};

const uint32 BG_AV_GraveyardIds[9]= {
    BG_AV_GRAVE_STORM_AID,
    BG_AV_GRAVE_STORM_GRAVE,
    BG_AV_GRAVE_STONE_GRAVE,
    BG_AV_GRAVE_SNOWFALL,
    BG_AV_GRAVE_ICE_GRAVE,
    BG_AV_GRAVE_FROSTWOLF,
    BG_AV_GRAVE_FROST_HUT,
    BG_AV_GRAVE_MAIN_ALLIANCE,
    BG_AV_GRAVE_MAIN_HORDE
};

enum BG_AV_States
{
    POINT_NEUTRAL               = 0,
    POINT_ASSAULTED             = 1,
    POINT_DESTROYED             = 2,
    POINT_CONTROLLED            = 3
};

enum BG_AV_WorldStates
{
    BG_AV_Alliance_Score        = 3127,
    BG_AV_Horde_Score           = 3128,
    BG_AV_SHOW_H_SCORE          = 3133,
    BG_AV_SHOW_A_SCORE          = 3134,

/*
    //the comments behind the state shows which icon overlaps the other.. but is, until now, unused and maybe not a good solution (but give few performance (: )

// Graves
    // Alliance
    //Stormpike first aid station
    AV_AID_A_C                      = 1325,
    AV_AID_A_A                      = 1326,
    AV_AID_H_C                      = 1327,
    AV_AID_H_A                      = 1328,
    //Stormpike Graveyard
    AV_PIKEGRAVE_A_C                = 1333,
    AV_PIKEGRAVE_A_A                = 1335,
    AV_PIKEGRAVE_H_C                = 1334,
    AV_PIKEGRAVE_H_A                = 1336,
    //Stoneheart Grave
    AV_STONEHEART_A_C               = 1302,
    AV_STONEHEART_A_A               = 1304, //over hc
    AV_STONEHEART_H_C               = 1301, //over ac
    AV_STONEHEART_H_A               = 1303, //over aa
    //Neutral
    //Snowfall Grave
*/
    AV_SNOWFALL_N                   = 1966, //over aa
/*
    AV_SNOWFALL_A_C                 = 1341, //over hc
    AV_SNOWFALL_A_A                 = 1343, //over ha
    AV_SNOWFALL_H_C                 = 1342,
    AV_SNOWFALL_H_A                 = 1344, //over ac
    //Horde
    //Iceblood grave
    AV_ICEBLOOD_A_C                 = 1346, //over hc
    AV_ICEBLOOD_A_A                 = 1348, //over ac
    AV_ICEBLOOD_H_C                 = 1347,
    AV_ICEBLOOD_H_A                 = 1349, //over aa
    //Frostwolf Grave
    AV_FROSTWOLF_A_C                = 1337, //over hc
    AV_FROSTWOLF_A_A                = 1339, //over ac
    AV_FROSTWOLF_H_C                = 1338,
    AV_FROSTWOLF_H_A                = 1340, //over aa
    //Frostwolf Hut
    AV_FROSTWOLFHUT_A_C             = 1329, //over hc
    AV_FROSTWOLFHUT_A_A             = 1331, //over ha
    AV_FROSTWOLFHUT_H_C             = 1330,
    AV_FROSTWOLFHUT_H_A             = 1332, //over ac

//Towers
    //Alliance
    //Dunbaldar South Bunker
    AV_DUNS_CONTROLLED              = 1361,
    AV_DUNS_DESTROYED               = 1370,
    AV_DUNS_ASSAULTED               = 1378,
    //Dunbaldar North Bunker
    AV_DUNN_CONTROLLED              = 1362,
    AV_DUNN_DESTROYED               = 1371,
    AV_DUNN_ASSAULTED               = 1379,
    //Icewing Bunker
    AV_ICEWING_CONTROLLED           = 1363,
    AV_ICEWING_DESTROYED            = 1372,
    AV_ICEWING_ASSAULTED            = 1380,
    //Stoneheart Bunker
    AV_STONEH_CONTROLLED            = 1364,
    AV_STONEH_DESTROYED             = 1373,
    AV_STONEH_ASSAULTED             = 1381,
    //Horde
    //Iceblood Tower
    AV_ICEBLOOD_CONTROLLED          = 1385,
    AV_ICEBLOOD_DESTROYED           = 1368,
    AV_ICEBLOOD_ASSAULTED           = 1390,
    //Tower Point
    AV_TOWERPOINT_CONTROLLED        = 1384,
    AV_TOWERPOINT_DESTROYED         = 1367, //goes over controlled
    AV_TOWERPOINT_ASSAULTED         = 1389, //goes over destroyed
    //Frostwolf West
    AV_FROSTWOLFW_CONTROLLED        = 1382,
    AV_FROSTWOLFW_DESTROYED         = 1365, //over controlled
    AV_FROSTWOLFW_ASSAULTED         = 1387, //over destroyed
    //Frostwolf East
    AV_FROSTWOLFE_CONTROLLED        = 1383,
    AV_FROSTWOLFE_DESTROYED         = 1366,
    AV_FROSTWOLFE_ASSAULTED         = 1388,

//mines
    AV_N_MINE_N                     = 1360,
    AV_N_MINE_A                     = 1358,
    AV_N_MINE_H                     = 1359,

    AV_S_MINE_N                     = 1357,
    AV_S_MINE_A                     = 1355,
    AV_S_MINE_H                     = 1356,

//towers assaulted by own team (unused)
    AV_STONEH_UNUSED                = 1377,
    AV_ICEWING_UNUSED               = 1376,
    AV_DUNS_UNUSED                  = 1375,
    AV_DUNN_UNUSED                  = 1374,

    AV_ICEBLOOD_UNUSED              = 1395,
    AV_TOWERPOINT_UNUSED            = 1394,
    AV_FROSTWOLFE_UNUSED            = 1393,
    AV_FROSTWOLFW_UNUSED            = 1392
*/
};

// alliance_control neutral_control horde_control
const uint32 BG_AV_MineWorldStates[2][3] = {
    {1358, 1360,1359},
    {1355, 1357,1356}
};

// alliance_control alliance_assault h_control h_assault
const uint32 BG_AV_NodeWorldStates[BG_AV_NODES_MAX][4] = {
    // Stormpike first aid station
    {1325,1326,1327,1328},
    // Stormpike Graveyard
    {1333,1335,1334,1336},
    // Stoneheart Grave
    {1302,1304,1301,1303},
    // Snowfall Grave
    {1341,1343,1342,1344},
    // Iceblood grave
    {1346,1348,1347,1349},
    // Frostwolf Grave
    {1337,1339,1338,1340},
    // Frostwolf Hut
    {1329,1331,1330,1332},
    // Dunbaldar South Bunker
    {1361,1375,1370,1378},
    // Dunbaldar North Bunker
    {1362,1374,1371,1379},
    // Icewing Bunker
    {1363,1376,1372,1380},
    // Stoneheart Bunker
    {1364,1377,1373,1381},
    // Iceblood Tower
    {1368,1390,1385,1395},
    // Tower Point
    {1367,1389,1384,1394},
    // Frostwolf East
    {1366,1388,1383,1393},
    // Frostwolf West
    {1365,1387,1382,1392},
};

enum BG_AV_QuestIds
{
    BG_AV_QUEST_A_SCRAPS1       = 7223,                     // first quest
    BG_AV_QUEST_A_SCRAPS2       = 6781,                     // repeatable
    BG_AV_QUEST_H_SCRAPS1       = 7224,
    BG_AV_QUEST_H_SCRAPS2       = 6741,
    BG_AV_QUEST_A_COMMANDER1    = 6942,                     // soldier
    BG_AV_QUEST_H_COMMANDER1    = 6825,
    BG_AV_QUEST_A_COMMANDER2    = 6941,                     // leutnant
    BG_AV_QUEST_H_COMMANDER2    = 6826,
    BG_AV_QUEST_A_COMMANDER3    = 6943,                     // commander
    BG_AV_QUEST_H_COMMANDER3    = 6827,
    BG_AV_QUEST_A_BOSS1         = 7386,                     // 5 cristal/blood
    BG_AV_QUEST_H_BOSS1         = 7385,
    BG_AV_QUEST_A_BOSS2         = 6881,                     // 1
    BG_AV_QUEST_H_BOSS2         = 6801,
    BG_AV_QUEST_A_NEAR_MINE     = 5892,                     // the mine near start location of team
    BG_AV_QUEST_H_NEAR_MINE     = 5893,
    BG_AV_QUEST_A_OTHER_MINE    = 6982,                     // the other mine ;)
    BG_AV_QUEST_H_OTHER_MINE    = 6985,
    BG_AV_QUEST_A_RIDER_HIDE    = 7026,
    BG_AV_QUEST_H_RIDER_HIDE    = 7002,
    BG_AV_QUEST_A_RIDER_TAME    = 7027,
    BG_AV_QUEST_H_RIDER_TAME    = 7001
};

struct BG_AV_NodeInfo
{
    uint32       TotalOwner;
    uint32       Owner;
    uint32       PrevOwner;
    BG_AV_States State;
    BG_AV_States PrevState;
    int          Timer;
    bool         Tower;
};

inline BG_AV_Nodes &operator++(BG_AV_Nodes &i)
{
    return i = BG_AV_Nodes(i + 1);
}

class BattleGroundAVScore : public BattleGroundScore
{
    public:
        BattleGroundAVScore() : GraveyardsAssaulted(0), GraveyardsDefended(0), TowersAssaulted(0), TowersDefended(0), SecondaryObjectives(0) {};
        virtual ~BattleGroundAVScore() {};
        uint32 GraveyardsAssaulted;
        uint32 GraveyardsDefended;
        uint32 TowersAssaulted;
        uint32 TowersDefended;
        uint32 SecondaryObjectives;
};

class BattleGroundAV : public BattleGround
{
    friend class BattleGroundMgr;

    public:
        BattleGroundAV();
        ~BattleGroundAV();
        void Update(uint32 diff);

        /* inherited from BattlegroundClass */
        virtual void AddPlayer(Player *plr);
        virtual void OnObjectDBLoad(Creature* creature);
        virtual void OnObjectDBLoad(GameObject* obj);
        virtual void OnCreatureRespawn(Creature* creature);

        virtual void StartingEventCloseDoors();
        virtual void StartingEventOpenDoors();
        // world states
        virtual void FillInitialWorldStates(WorldPacket& data);

        void RemovePlayer(Player *plr,uint64 guid);
        void HandleAreaTrigger(Player *Source, uint32 Trigger);
        bool SetupBattleGround();
        virtual void Reset();

        /*general stuff*/
        void UpdateScore(uint32 team, int32 points);
        void UpdatePlayerScore(Player *Source, uint32 type, uint32 value);

        /*handle stuff*/ // these are functions which get called from extern scripts
        virtual void EventPlayerClickedOnFlag(Player *source, GameObject* target_obj);
        void HandleKillPlayer(Player* player, Player *killer);
        void HandleKillUnit(Creature *unit, Player *killer);
        void HandleQuestComplete(uint32 questid, Player *player);
        bool PlayerCanDoMineQuest(int32 GOId,uint32 team);

        void EndBattleGround(uint32 winner);

        virtual WorldSafeLocsEntry const* GetClosestGraveYard(Player *plr);

    private:
        /* Nodes occupying */
        void EventPlayerAssaultsPoint(Player* player);
        void EventPlayerDefendsPoint(Player* player);
        void EventPlayerDestroyedPoint(BG_AV_Nodes node);

        void AssaultNode(BG_AV_Nodes node, uint32 team);
        void DestroyNode(BG_AV_Nodes node);
        void InitNode(BG_AV_Nodes node, uint32 team, bool tower);
        void DefendNode(BG_AV_Nodes node, uint32 team);

        void PopulateNode(BG_AV_Nodes node);
        void DePopulateNode(BG_AV_Nodes node);

        const BG_AV_Nodes GetNodeThroughPlayerPosition(Player* plr);
        uint32 GetNodeName(BG_AV_Nodes node);
        const bool IsTower(BG_AV_Nodes node) { return (node == BG_AV_NODES_ERROR)? false : m_Nodes[node].Tower; }
        const bool IsGrave(BG_AV_Nodes node) { return (node == BG_AV_NODES_ERROR)? false : !m_Nodes[node].Tower; }
        BG_AV_Nodes GetNodeThroughNodeEvent(uint8 event);
        uint8 GetNodeEventThroughNode(BG_AV_Nodes node);
        bool IsActiveNodeEvent(uint8 event);

        /*mine*/
        void ChangeMineOwner(uint8 mine, uint32 team);

        /*worldstates*/
        uint8 GetWorldStateType(uint8 state, uint32 team) const;
        void SendMineWorldStates(uint32 mine);
        void UpdateNodeWorldState(BG_AV_Nodes node);

        /*variables */
        int32 m_TeamScores[BG_TEAMS_COUNT];
        uint32 m_Team_QuestStatus[BG_TEAMS_COUNT][9];       // [x][y] x=team y=questcounter

        BG_AV_NodeInfo m_Nodes[BG_AV_NODES_MAX];

        uint32 m_Mine_Owner[BG_TEAMS_COUNT];
        uint32 m_Mine_PrevOwner[BG_TEAMS_COUNT];            // only for worldstates needed
        int32 m_Mine_Timer;                                 // one timer ticks for both teams
        uint32 m_Mine_Reclaim_Timer[BG_TEAMS_COUNT];

        bool m_IsInformedNearLose[BG_TEAMS_COUNT];
        bool m_captainAlive[BG_TEAMS_COUNT];

        uint64 m_DB_Creature[BG_AV_DB_CREATURE_MAX];

        BG_AV_Nodes m_assault_in_progress;                  // this node gets currently assaulted and must be ignored for getclosestgrave
        BGCreatures m_MineCreatures[2][3];                  // 2 mines, 3 factions (neutral, alliance, horde)

        // 7 grave yards 2kinds: alliance/horde-captured, 4 different creaturetypes (there is a quest where you can improve those creatures)
        BGCreatures m_GraveCreatures[7][BG_TEAMS_COUNT][4];

        struct BG_AV_NodeObjects
        {
            BGObjects gameobjects;
            BGCreatures creatures;
        };
        BG_AV_NodeObjects m_NodeObjects[BG_AV_MAX_NODE_EVENTS];

        BGObjects m_DeadCaptainBurning[BG_TEAMS_COUNT];
        BGObjects m_Doors;                                  // alliance and horde will go in this one

        uint32 m_HonorMapComplete;
        uint32 m_RepTowerDestruction;
        uint32 m_RepCaptain;
        uint32 m_RepBoss;
        uint32 m_RepOwnedGrave;
        uint32 m_RepOwnedMine;
        uint32 m_RepSurviveCaptain;
        uint32 m_RepSurviveTower;
};

#endif
