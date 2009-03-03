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

#define LANG_BG_AV_A_CAPTAIN_BUFF       "Begone. Uncouth scum! The Alliance shall prevail in Alterac Valley!"
#define LANG_BG_AV_H_CAPTAIN_BUFF       "Now is the time to attack! For the Horde!"
#define LANG_BG_AV_S_MINE_BOSS_CLAIMS   "Snivvle is here! Snivvle claims the Coldtooth Mine!"

#define BG_AV_CAPTIME                    240000  //4:00
#define BG_AV_SNOWFALL_FIRSTCAP          300000  //5:00 but i also have seen 4:05

#define BG_AV_SCORE_INITIAL_POINTS       600
#define SEND_MSG_NEAR_LOSE               120

#define BG_AV_KILL_BOSS                 4
#define BG_AV_REP_BOSS                  350

#define BG_AV_KILL_CAPTAIN              3
#define BG_AV_REP_CAPTAIN               125
#define BG_AV_RES_CAPTAIN               100

#define BG_AV_KILL_TOWER                3
#define BG_AV_REP_TOWER                 12
#define BG_AV_RES_TOWER                 75

#define BG_AV_GET_COMMANDER            1 //for a safely returned wingcommander
//bonushonor at the end
#define BG_AV_KILL_SURVIVING_TOWER      2
#define BG_AV_REP_SURVIVING_TOWER       12

#define BG_AV_KILL_SURVIVING_CAPTAIN    2
#define BG_AV_REP_SURVIVING_CAPTAIN     125

enum BG_AV_Sounds
{ //TODO: get out if there comes a sound when neutral team captures mine

/*
8212:
    alliance grave assault
    alliance tower assault
    drek "mlanzenabschaum! In meiner Burg?! Toetet sie all" - nicht immer der sound
8333:
    galv "sterbt fuer euch ist kein platz hier"

8332:
    bal "Verschwinde, dreckiger Abschaum! Die Allianz wird im Alteractal "
8174:
    horde tower assault
    horde grave assault
    van "es Sturmlanzenklans, euer General wird angegriffen! Ich fordere Unterst"
8173:
    ally grave capture/defend
    tower destroy
    mine capture
    ally wins
8192:
    ally tower destroy(only iceblood - found a bug^^)
    ally tower  defend
    horde tower defend
8213
horde:
    grave defend/capture
    tower destroy
    mine capture
    horde wins
    */

    AV_SOUND_NEAR_VICTORY                   = 8456, //not confirmed yet

    AV_SOUND_ALLIANCE_ASSAULTS              = 8212, //tower,grave + enemy boss if someone tries to attack him
    AV_SOUND_HORDE_ASSAULTS                 = 8174,
    AV_SOUND_ALLIANCE_GOOD                  = 8173, //if something good happens for the team:  wins(maybe only through killing the boss), captures mine or grave, destroys tower and defends grave
    AV_SOUND_HORDE_GOOD                     = 8213,
    AV_SOUND_BOTH_TOWER_DEFEND              = 8192,

    AV_SOUND_ALLIANCE_CAPTAIN               = 8232, //gets called when someone attacks them and at the beginning after 3min+rand(x)*10sec (maybe buff)
    AV_SOUND_HORDE_CAPTAIN                  = 8333,


};

enum BG_AV_CREATURE_ENTRIES //only those, whoe are interesting for us
{
    AV_CREATURE_ENTRY_H_CAPTAIN     = 11947,
    AV_CREATURE_ENTRY_A_CAPTAIN     = 11949,
    AV_CREATURE_ENTRY_H_BOSS        = 11946,
    AV_CREATURE_ENTRY_A_BOSS        = 11948,
    AV_CREATURE_ENTRY_N_HERALD      = 11997,

    //TODO implement the following ones
    AV_CREATURE_ENTRY_H_L1          = 13179,            //H-lieutnant guse
    AV_CREATURE_ENTRY_H_D_B         = 13236,            //H-shaman-boss
    AV_CREATURE_ENTRY_H_D_C         = 13284,            //h-shaman-creep
    AV_CREATURE_ENTRY_A_L1          = 13438,            //A-lieutnant slidore
    AV_CREATURE_ENTRY_A_D_B         = 13442,            //a-druid boss
    AV_CREATURE_ENTRY_A_D_C         = 13443,             //a-druid creep

    AV_CREATURE_ENTRY_NM_N_B        = 11657,
    AV_CREATURE_ENTRY_NM_A_B        = 13078,
    AV_CREATURE_ENTRY_NM_H_B        = 13079,
    AV_CREATURE_ENTRY_SM_N_B        = 11677,
    AV_CREATURE_ENTRY_SM_A_B        = 13086,
    AV_CREATURE_ENTRY_SM_H_B        = 13088,

    AV_CREATURE_ENTRY_A_MARSHAL_SOUTH       = 14763,
    AV_CREATURE_ENTRY_A_MARSHAL_NORTH       = 14762,
    AV_CREATURE_ENTRY_A_MARSHAL_ICE         = 14764,
    AV_CREATURE_ENTRY_A_MARSHAL_STONE       = 14765,
    AV_CREATURE_ENTRY_H_MARSHAL_ICE         = 14773,
    AV_CREATURE_ENTRY_H_MARSHAL_TOWER       = 14776,
    AV_CREATURE_ENTRY_H_MARSHAL_ETOWER      = 14772,
    AV_CREATURE_ENTRY_H_MARSHAL_WTOWER      = 14777,


};


enum BG_AV_OTHER_VALUES
{
    AV_STATICCPLACE_MAX        = 123,
    AV_NORTH_MINE              = 0,
    AV_SOUTH_MINE              = 1,
    AV_MINE_TICK_TIMER         = 45000,
    AV_MINE_RECLAIM_TIMER      = 1200000, //TODO: get the right value.. this is currently 20 minutes
    AV_NEUTRAL_TEAM            = 0 //this is the neutral owner of snowfall
};

enum BG_AV_ObjectIds
{
    //cause the mangos-system is a bit different, we don't use the right go-ids for every node.. if we want to be 100% like another big server, we must take one object for every node
    //snowfall 4flags as eyecandy 179424 (alliance neutral)
    //Banners - stolen from battleground_AB.h ;-)
    BG_AV_OBJECTID_BANNER_A             = 178925, // can only be used by horde
    BG_AV_OBJECTID_BANNER_H             = 178943, // can only be used by alliance
    BG_AV_OBJECTID_BANNER_CONT_A        = 178940, // can only be used by horde
    BG_AV_OBJECTID_BANNER_CONT_H        = 179435, // can only be used by alliance

    BG_AV_OBJECTID_BANNER_A_B           = 178365,
    BG_AV_OBJECTID_BANNER_H_B           = 178364,
    BG_AV_OBJECTID_BANNER_CONT_A_B      = 179286,
    BG_AV_OBJECTID_BANNER_CONT_H_B      = 179287,
    BG_AV_OBJECTID_BANNER_SNOWFALL_N    = 180418,

    //snowfall eyecandy banner:
    BG_AV_OBJECTID_SNOWFALL_CANDY_A     = 179044,
    BG_AV_OBJECTID_SNOWFALL_CANDY_PA    = 179424,
    BG_AV_OBJECTID_SNOWFALL_CANDY_H     = 179064,
    BG_AV_OBJECTID_SNOWFALL_CANDY_PH    = 179425,

    //banners on top of towers:
    BG_AV_OBJECTID_TOWER_BANNER_A  = 178927, //[PH] Alliance A1 Tower Banner BIG
    BG_AV_OBJECTID_TOWER_BANNER_H  = 178955, //[PH] Horde H1 Tower Banner BIG
    BG_AV_OBJECTID_TOWER_BANNER_PA = 179446, //[PH] Alliance H1 Tower Pre-Banner BIG
    BG_AV_OBJECTID_TOWER_BANNER_PH = 179436, //[PH] Horde A1 Tower Pre-Banner BIG

    //Auras
    BG_AV_OBJECTID_AURA_A               = 180421,
    BG_AV_OBJECTID_AURA_H               = 180422,
    BG_AV_OBJECTID_AURA_N               = 180423,
    BG_AV_OBJECTID_AURA_A_S             = 180100,
    BG_AV_OBJECTID_AURA_H_S             = 180101,
    BG_AV_OBJECTID_AURA_N_S             = 180102,

    BG_AV_OBJECTID_GATE_A               = 180424,
    BG_AV_OBJECTID_GATE_H               = 180424,

    //mine supplies
    BG_AV_OBJECTID_MINE_N        = 178785,
    BG_AV_OBJECTID_MINE_S       = 178784,

    BG_AV_OBJECTID_FIRE                 = 179065,
    BG_AV_OBJECTID_SMOKE                 = 179066
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

    BG_AV_NODES_MAX                     = 15
};

enum BG_AV_ObjectTypes
{
    BG_AV_OBJECT_FLAG_A_FIRSTAID_STATION    = 0,
    BG_AV_OBJECT_FLAG_A_STORMPIKE_GRAVE     = 1,
    BG_AV_OBJECT_FLAG_A_STONEHEART_GRAVE    = 2,
    BG_AV_OBJECT_FLAG_A_SNOWFALL_GRAVE      = 3,
    BG_AV_OBJECT_FLAG_A_ICEBLOOD_GRAVE      = 4,
    BG_AV_OBJECT_FLAG_A_FROSTWOLF_GRAVE     = 5,
    BG_AV_OBJECT_FLAG_A_FROSTWOLF_HUT       = 6,
    BG_AV_OBJECT_FLAG_A_DUNBALDAR_SOUTH     = 7,
    BG_AV_OBJECT_FLAG_A_DUNBALDAR_NORTH     = 8,
    BG_AV_OBJECT_FLAG_A_ICEWING_BUNKER      = 9,
    BG_AV_OBJECT_FLAG_A_STONEHEART_BUNKER   = 10,

    BG_AV_OBJECT_FLAG_C_A_FIRSTAID_STATION    = 11,
    BG_AV_OBJECT_FLAG_C_A_STORMPIKE_GRAVE     = 12,
    BG_AV_OBJECT_FLAG_C_A_STONEHEART_GRAVE    = 13,
    BG_AV_OBJECT_FLAG_C_A_SNOWFALL_GRAVE      = 14,
    BG_AV_OBJECT_FLAG_C_A_ICEBLOOD_GRAVE      = 15,
    BG_AV_OBJECT_FLAG_C_A_FROSTWOLF_GRAVE     = 16,
    BG_AV_OBJECT_FLAG_C_A_FROSTWOLF_HUT       = 17,
    BG_AV_OBJECT_FLAG_C_A_ICEBLOOD_TOWER      = 18,
    BG_AV_OBJECT_FLAG_C_A_TOWER_POINT         = 19,
    BG_AV_OBJECT_FLAG_C_A_FROSTWOLF_ETOWER    = 20,
    BG_AV_OBJECT_FLAG_C_A_FROSTWOLF_WTOWER    = 21,

    BG_AV_OBJECT_FLAG_C_H_FIRSTAID_STATION    = 22,
    BG_AV_OBJECT_FLAG_C_H_STORMPIKE_GRAVE     = 23,
    BG_AV_OBJECT_FLAG_C_H_STONEHEART_GRAVE    = 24,
    BG_AV_OBJECT_FLAG_C_H_SNOWFALL_GRAVE      = 25,
    BG_AV_OBJECT_FLAG_C_H_ICEBLOOD_GRAVE      = 26,
    BG_AV_OBJECT_FLAG_C_H_FROSTWOLF_GRAVE     = 27,
    BG_AV_OBJECT_FLAG_C_H_FROSTWOLF_HUT       = 28,
    BG_AV_OBJECT_FLAG_C_H_DUNBALDAR_SOUTH     = 29,
    BG_AV_OBJECT_FLAG_C_H_DUNBALDAR_NORTH     = 30,
    BG_AV_OBJECT_FLAG_C_H_ICEWING_BUNKER      = 31,
    BG_AV_OBJECT_FLAG_C_H_STONEHEART_BUNKER   = 32,

    BG_AV_OBJECT_FLAG_H_FIRSTAID_STATION    = 33,
    BG_AV_OBJECT_FLAG_H_STORMPIKE_GRAVE     = 34,
    BG_AV_OBJECT_FLAG_H_STONEHEART_GRAVE    = 35,
    BG_AV_OBJECT_FLAG_H_SNOWFALL_GRAVE      = 36,
    BG_AV_OBJECT_FLAG_H_ICEBLOOD_GRAVE      = 37,
    BG_AV_OBJECT_FLAG_H_FROSTWOLF_GRAVE     = 38,
    BG_AV_OBJECT_FLAG_H_FROSTWOLF_HUT       = 39,
    BG_AV_OBJECT_FLAG_H_ICEBLOOD_TOWER      = 40,
    BG_AV_OBJECT_FLAG_H_TOWER_POINT         = 41,
    BG_AV_OBJECT_FLAG_H_FROSTWOLF_ETOWER    = 42,
    BG_AV_OBJECT_FLAG_H_FROSTWOLF_WTOWER    = 43,

    BG_AV_OBJECT_FLAG_N_SNOWFALL_GRAVE      = 44,

    BG_AV_OBJECT_DOOR_H                     = 45,
    BG_AV_OBJECT_DOOR_A                     = 46,
//auras for graveyards (3auras per graveyard neutral,alliance,horde)
    BG_AV_OBJECT_AURA_N_FIRSTAID_STATION    = 47,
    BG_AV_OBJECT_AURA_A_FIRSTAID_STATION    = 48,
    BG_AV_OBJECT_AURA_H_FIRSTAID_STATION    = 49,
    BG_AV_OBJECT_AURA_N_STORMPIKE_GRAVE     = 50,
    BG_AV_OBJECT_AURA_A_STORMPIKE_GRAVE     = 51,
    BG_AV_OBJECT_AURA_H_STORMPIKE_GRAVE     = 52,
    BG_AV_OBJECT_AURA_N_STONEHEART_GRAVE    = 53,
    BG_AV_OBJECT_AURA_A_STONEHEART_GRAVE    = 54,
    BG_AV_OBJECT_AURA_H_STONEHEART_GRAVE    = 55,
    BG_AV_OBJECT_AURA_N_SNOWFALL_GRAVE      = 56,
    BG_AV_OBJECT_AURA_A_SNOWFALL_GRAVE      = 57,
    BG_AV_OBJECT_AURA_H_SNOWFALL_GRAVE      = 58,
    BG_AV_OBJECT_AURA_N_ICEBLOOD_GRAVE      = 59,
    BG_AV_OBJECT_AURA_A_ICEBLOOD_GRAVE      = 60,
    BG_AV_OBJECT_AURA_H_ICEBLOOD_GRAVE      = 61,
    BG_AV_OBJECT_AURA_N_FROSTWOLF_GRAVE     = 62,
    BG_AV_OBJECT_AURA_A_FROSTWOLF_GRAVE     = 63,
    BG_AV_OBJECT_AURA_H_FROSTWOLF_GRAVE     = 64,
    BG_AV_OBJECT_AURA_N_FROSTWOLF_HUT       = 65,
    BG_AV_OBJECT_AURA_A_FROSTWOLF_HUT       = 66,
    BG_AV_OBJECT_AURA_H_FROSTWOLF_HUT       = 67,

    //big flags on top of towers 2 flags on each (contested,(alliance | horde)) + 2 auras
    BG_AV_OBJECT_TFLAG_A_DUNBALDAR_SOUTH     = 67,
    BG_AV_OBJECT_TFLAG_H_DUNBALDAR_SOUTH     = 68,
    BG_AV_OBJECT_TFLAG_A_DUNBALDAR_NORTH     = 69,
    BG_AV_OBJECT_TFLAG_H_DUNBALDAR_NORTH     = 70,
    BG_AV_OBJECT_TFLAG_A_ICEWING_BUNKER      = 71,
    BG_AV_OBJECT_TFLAG_H_ICEWING_BUNKER      = 72,
    BG_AV_OBJECT_TFLAG_A_STONEHEART_BUNKER   = 73,
    BG_AV_OBJECT_TFLAG_H_STONEHEART_BUNKER   = 74,
    BG_AV_OBJECT_TFLAG_A_ICEBLOOD_TOWER      = 75,
    BG_AV_OBJECT_TFLAG_H_ICEBLOOD_TOWER      = 76,
    BG_AV_OBJECT_TFLAG_A_TOWER_POINT         = 77,
    BG_AV_OBJECT_TFLAG_H_TOWER_POINT         = 78,
    BG_AV_OBJECT_TFLAG_A_FROSTWOLF_ETOWER    = 79,
    BG_AV_OBJECT_TFLAG_H_FROSTWOLF_ETOWER    = 80,
    BG_AV_OBJECT_TFLAG_A_FROSTWOLF_WTOWER    = 81,
    BG_AV_OBJECT_TFLAG_H_FROSTWOLF_WTOWER    = 82,
    BG_AV_OBJECT_TAURA_A_DUNBALDAR_SOUTH     = 83,
    BG_AV_OBJECT_TAURA_H_DUNBALDAR_SOUTH     = 84,
    BG_AV_OBJECT_TAURA_A_DUNBALDAR_NORTH     = 85,
    BG_AV_OBJECT_TAURA_H_DUNBALDAR_NORTH     = 86,
    BG_AV_OBJECT_TAURA_A_ICEWING_BUNKER      = 87,
    BG_AV_OBJECT_TAURA_H_ICEWING_BUNKER      = 88,
    BG_AV_OBJECT_TAURA_A_STONEHEART_BUNKER   = 89,
    BG_AV_OBJECT_TAURA_H_STONEHEART_BUNKER   = 90,
    BG_AV_OBJECT_TAURA_A_ICEBLOOD_TOWER      = 91,
    BG_AV_OBJECT_TAURA_H_ICEBLOOD_TOWER      = 92,
    BG_AV_OBJECT_TAURA_A_TOWER_POINT         = 93,
    BG_AV_OBJECT_TAURA_H_TOWER_POINT         = 94,
    BG_AV_OBJECT_TAURA_A_FROSTWOLF_ETOWER    = 95,
    BG_AV_OBJECT_TAURA_H_FROSTWOLF_ETOWER    = 96,
    BG_AV_OBJECT_TAURA_A_FROSTWOLF_WTOWER    = 97,
    BG_AV_OBJECT_TAURA_H_FROSTWOLF_WTOWER    = 98,

    BG_AV_OBJECT_BURN_DUNBALDAR_SOUTH        = 99,
    BG_AV_OBJECT_BURN_DUNBALDAR_NORTH        = 109,
    BG_AV_OBJECT_BURN_ICEWING_BUNKER         = 119,
    BG_AV_OBJECT_BURN_STONEHEART_BUNKER      = 129,
    BG_AV_OBJECT_BURN_ICEBLOOD_TOWER         = 139,
    BG_AV_OBJECT_BURN_TOWER_POINT            = 149,
    BG_AV_OBJECT_BURN_FROSTWOLF_ETWOER       = 159,
    BG_AV_OBJECT_BURN_FROSTWOLF_WTOWER       = 169,
    BG_AV_OBJECT_BURN_BUILDING_ALLIANCE      = 179,
    BG_AV_OBJECT_BURN_BUILDING_HORDE         = 189,

    BG_AV_OBJECT_MAX                          = 199
};



enum BG_AV_OBJECTS
{
    AV_OPLACE_FIRSTAID_STATION              = 0,
    AV_OPLACE_STORMPIKE_GRAVE               = 1,
    AV_OPLACE_STONEHEART_GRAVE              = 2,
    AV_OPLACE_SNOWFALL_GRAVE                = 3,
    AV_OPLACE_ICEBLOOD_GRAVE                = 4,
    AV_OPLACE_FROSTWOLF_GRAVE               = 5,
    AV_OPLACE_FROSTWOLF_HUT                 = 6,
    AV_OPLACE_DUNBALDAR_SOUTH               = 7,
    AV_OPLACE_DUNBALDAR_NORTH               = 8,
    AV_OPLACE_ICEWING_BUNKER                = 9,
    AV_OPLACE_STONEHEART_BUNKER             = 10,
    AV_OPLACE_ICEBLOOD_TOWER                = 11,
    AV_OPLACE_TOWER_POINT                   = 12,
    AV_OPLACE_FROSTWOLF_ETOWER              = 13,
    AV_OPLACE_FROSTWOLF_WTOWER              = 14,
    AV_OPLACE_BIGBANNER_DUNBALDAR_SOUTH     = 15,
    AV_OPLACE_BIGBANNER_DUNBALDAR_NORTH     = 16,
    AV_OPLACE_BIGBANNER_ICEWING_BUNKER      = 17,
    AV_OPLACE_BIGBANNER_STONEHEART_BUNKER   = 18,
    AV_OPLACE_BIGBANNER_ICEBLOOD_TOWER      = 19,
    AV_OPLACE_BIGBANNER_TOWER_POINT         = 20,
    AV_OPLACE_BIGBANNER_FROSTWOLF_ETOWER    = 21,
    AV_OPLACE_BIGBANNER_FROSTWOLF_WTOWER    = 22,

    AV_OPLACE_BURN_DUNBALDAR_SOUTH          = 23,
    AV_OPLACE_BURN_DUNBALDAR_NORTH          = 33,
    AV_OPLACE_BURN_ICEWING_BUNKER           = 43,
    AV_OPLACE_BURN_STONEHEART_BUNKER        = 53,
    AV_OPLACE_BURN_ICEBLOOD_TOWER           = 63,
    AV_OPLACE_BURN_TOWER_POINT              = 73,
    AV_OPLACE_BURN_FROSTWOLF_ETOWER         = 83,
    AV_OPLACE_BURN_FROSTWOLF_WTOWER         = 93,
    AV_OPLACE_BURN_BUILDING_A               = 103,
    AV_OPLACE_BURN_BUILDING_H               = 113,

    AV_OPLACE_MAX                           = 123
};
const float BG_AV_ObjectPos[AV_OPLACE_MAX][4] = {
    {638.592f,-32.422f,46.0608f,-1.62316f },//firstaid station
    {669.007f,-294.078f,30.2909f,2.77507f },//stormpike
    {77.8013f,-404.7f,46.7549f,-0.872665f },//stone grave
    {-202.581f,-112.73f,78.4876f,-0.715585f },//snowfall
    {-611.962f,-396.17f,60.8351f,2.53682f},  //iceblood grave
    {-1082.45f,-346.823f,54.9219f,-1.53589f },//frostwolf grave
    {-1402.21f,-307.431f,89.4424f,0.191986f },//frostwolf hut
    {553.779f,-78.6566f,51.9378f,-1.22173f }, //dunnbaldar south
    {674.001f,-143.125f,63.6615f,0.994838f }, //dunbaldar north
    {203.281f,-360.366f,56.3869f,-0.925024f }, //icew
    {-152.437f,-441.758f,40.3982f,-1.95477f }, //stone
    {-571.88f,-262.777f,75.0087f,-0.802851f }, //ice tower
    {-768.907f,-363.71f,90.8949f,1.07991f},  //tower point
    {-1302.9f,-316.981f,113.867f,2.00713f }, //frostwolf etower
    {-1297.5f,-266.767f,114.15f,3.31044f},   //frostwolf wtower
    //bigbanner:
    {555.848f,-84.4151f,64.4397f,3.12414f }, //duns
    {679.339f,-136.468f,73.9626f,-2.16421f }, //dunn
    {208.973f,-365.971f,66.7409f,-0.244346f }, //icew
    {-155.832f,-449.401f,52.7306f,0.610865f }, //stone
    {-572.329f,-262.476f,88.6496f,-0.575959f }, //icetower
    {-768.199f,-363.105f,104.537f,0.10472f }, //towerp
    {-1302.84f,-316.582f,127.516f,0.122173f }, //etower
    {-1297.87f,-266.762f,127.796f,0.0698132f }, //wtower
    //burning auras towers have 9*179065 captain-buildings have 5*179066+5*179065
    //dunns
    {562.632f,-88.1815f,61.993f,0.383972f },
    {562.523f,-74.5028f,37.9474f,-0.0523599f },
    {558.097f,-70.9842f,52.4876f,0.820305f },
    {578.167f,-71.8191f,38.1514f,2.72271f },
    {556.028f,-94.9242f,44.8191f,3.05433f },
    {572.451f,-94.3655f,37.9443f,-1.72788f },
    {549.263f,-79.3645f,44.8191f,0.436332f },
    {543.513f,-94.4006f,52.4819f,0.0349066f },
    {572.149f,-93.7862f,52.5726f,0.541052f },
    {582.162f,-81.2375f,37.9216f,0.0872665f },
    //dunn
    {664.797f,-143.65f,64.1784f,-0.453786f},
    {664.505f,-139.452f,49.6696f,-0.0349067f},
    {676.067f,-124.319f,49.6726f,-1.01229f},
    {693.004f,-144.025f,64.1755f,2.44346f},
    {661.175f,-117.691f,49.645f,1.91986f},
    {684.423f,-146.582f,63.6662f,0.994838f},
    {682.791f,-127.769f,62.4155f,1.09956f},
    {674.576f,-147.101f,56.5425f,-1.6057f},
    {655.719f,-126.673f,49.8138f,2.80998f},
    {0,0,0,0},
    //icew
    {231.503f,-356.688f,42.3704f,0.296706f},
    {224.989f,-348.175f,42.5607f,1.50098f},
    {205.782f,-351.335f,56.8998f,1.01229f},
    {196.605f,-369.187f,56.3914f,2.46091f},
    {210.619f,-376.938f,49.2677f,2.86234f},
    {209.647f,-352.632f,42.3959f,-0.698132f},
    {220.65f,-368.132f,42.3978f,-0.2618f},
    {224.682f,-374.031f,57.0679f,0.541052f},
    {200.26f,-359.968f,49.2677f,-2.89725f},
    {196.619f,-378.016f,56.9131f,1.01229f},
    //stone
    {-155.488f,-437.356f,33.2796f,2.60054f},
    {-163.441f,-454.188f,33.2796f,1.93732f},
    {-143.977f,-445.148f,26.4097f,-1.8675f},
    {-135.764f,-464.708f,26.3823f,2.25147f},
    {-154.076f,-466.929f,41.0636f,-1.8675f},
    {-149.908f,-460.332f,26.4083f,-2.09439f},
    {-151.638f,-439.521f,40.3797f,0.436332f},
    {-131.301f,-454.905f,26.5771f,2.93215f},
    {-171.291f,-444.684f,40.9211f,2.30383f},
    {-143.591f,-439.75f,40.9275f,-1.72788f},
    //iceblood
    {-572.667f,-267.923f,56.8542f,2.35619f},
    {-561.021f,-262.689f,68.4589f,1.37881f},
    {-572.538f,-262.649f,88.6197f,1.8326f},
    {-574.77f,-251.45f,74.9422f,-1.18682f},
    {-578.625f,-267.571f,68.4696f,0.506145f},
    {-571.476f,-257.234f,63.3223f,3.10669f},
    {-566.035f,-273.907f,52.9582f,-0.890118f},
    {-580.948f,-259.77f,68.4696f,1.46608f},
    {-568.318f,-267.1f,75.0008f,1.01229f},
    {-559.621f,-268.597f,52.8986f,0.0523599f},
    //towerp
    {-776.072f,-368.046f,84.3558f,2.63545f},
    {-777.564f,-368.521f,90.6701f,1.72788f},
    {-765.461f,-357.711f,90.888f,0.314159f},
    {-768.763f,-362.735f,104.612f,1.81514f},
    {-760.356f,-358.896f,84.3558f,2.1293f},
    {-771.967f,-352.838f,84.3484f,1.74533f},
    {-773.333f,-364.653f,79.2351f,-1.64061f},
    {-764.109f,-366.069f,70.0934f,0.383972f},
    {-767.103f,-350.737f,68.7933f,2.80998f},
    {-760.115f,-353.845f,68.8633f,1.79769f},
    //froste
    {-1304.87f,-304.525f,91.8366f,-0.680679f},
    {-1301.77f,-310.974f,95.8252f,0.907571f},
    {-1305.58f,-320.625f,102.166f,-0.558505f},
    {-1294.27f,-323.468f,113.893f,-1.67552f},
    {-1302.65f,-317.192f,127.487f,2.30383f},
    {-1293.89f,-313.478f,107.328f,1.6057f},
    {-1312.41f,-312.999f,107.328f,1.5708f},
    {-1311.57f,-308.08f,91.7666f,-1.85005f},
    {-1314.7f,-322.131f,107.36f,0.645772f},
    {-1304.6f,-310.754f,113.859f,-0.401426f},
    //frostw
    {-1308.24f,-273.26f,92.0514f,-0.139626f},
    {-1302.26f,-262.858f,95.9269f,0.418879f},
    {-1297.28f,-267.773f,126.756f,2.23402f},
    {-1299.08f,-256.89f,114.108f,-2.44346f},
    {-1303.41f,-268.237f,114.151f,-1.23918f},
    {-1304.43f,-273.682f,107.612f,0.244346f},
    {-1309.53f,-265.951f,92.1418f,-2.49582f},
    {-1295.55f,-263.865f,105.033f,0.925024f},
    {-1294.71f,-281.466f,107.664f,-1.50098f},
    {-1289.69f,-259.521f,107.612f,-2.19912f},

    //the two buildings of the captains
    //alliance
    {-64.4987f,-289.33f,33.4616f,-2.82743f},
    {-5.98025f,-326.144f,38.8538f,0},
    {-2.67893f,-306.998f,33.4165f,0},
    {-60.25f,-309.232f,50.2408f,-1.46608f},
    {-48.7941f,-266.533f,47.7916f,2.44346f},
    {-3.40929f,-306.288f,33.34f,0},
    {-48.619f,-266.917f,47.8168f,0},
    {-62.9474f,-286.212f,66.7288f,0},
    {-5.05132f,-325.323f,38.8536f,0},
    {-64.2677f,-289.412f,33.469f,0},
//horde
    {-524.276f,-199.6f,82.8733f,-1.46608f},
    {-518.196f,-173.085f,102.43f,0},
    {-500.732f,-145.358f,88.5337f,2.44346f},
    {-501.084f,-150.784f,80.8506f,0},
    {-518.309f,-163.963f,102.521f,2.96706f},
    {-517.053f,-200.429f,80.759f,0},
    {-514.361f,-163.864f,104.163f,0},
    {-568.04f,-188.707f,81.55f,0},
    {-501.775f,-151.581f,81.2027f,0},
    {-509.975f,-191.652f,83.2978f,0},

};

const float BG_AV_DoorPositons[2][4] = {
    {780.487f, -493.024f, 99.9553f, 3.0976f},   //alliance
    {-1375.193f, -538.981f, 55.2824f, 0.72178f} //horde
};


//creaturestuff starts here
//is related to BG_AV_CreaturePos
enum BG_AV_CreaturePlace
{
    AV_CPLACE_SPIRIT_STORM_AID      = 0,
    AV_CPLACE_SPIRIT_STORM_GRAVE    = 1,
    AV_CPLACE_SPIRIT_STONE_GRAVE    = 2,
    AV_CPLACE_SPIRIT_SNOWFALL       = 3,
    AV_CPLACE_SPIRIT_ICE_GRAVE      = 4,
    AV_CPLACE_SPIRIT_FROSTWOLF      = 5,
    AV_CPLACE_SPIRIT_FROST_HUT      = 6,
    AV_CPLACE_SPIRIT_MAIN_ALLIANCE  = 7,
    AV_CPLACE_SPIRIT_MAIN_HORDE     = 8,
//i don't will add for all 4 positions a variable.. i think one is enough to compute the rest
    AV_CPLACE_DEFENSE_STORM_AID      = 9,
    AV_CPLACE_DEFEMSE_STORM_GRAVE    = 13,
    AV_CPLACE_DEFENSE_STONE_GRAVE    = 17,
    AV_CPLACE_DEFENSE_SNOWFALL       = 21,
    AV_CPLACE_DEFENSE_FROSTWOLF      = 25,
    AV_CPLACE_DEFENSE_ICE_GRAVE      = 29,
    AV_CPLACE_DEFENSE_FROST_HUT      = 33,

    AV_CPLACE_DEFENSE_DUN_S          = 37,
    AV_CPLACE_DEFENSE_DUN_N          = 41,
    AV_CPLACE_DEFENSE_ICEWING        = 45,
    AV_CPLACE_DEFENSE_STONE_TOWER    = 49,
    AV_CPLACE_DEFENSE_ICE_TOWER      = 53,
    AV_CPLACE_DEFENSE_TOWERPOINT     = 57,
    AV_CPLACE_DEFENSE_FROST_E        = 61,
    AV_CPLACE_DEFENSE_FROST_t        = 65,

    AV_CPLACE_MAX = 69
};

//x, y, z, o
const float BG_AV_CreaturePos[AV_CPLACE_MAX][4] = {
    //spiritguides
    {643.000000f,44.000000f,69.740196f,-0.001854f},
    {676.000000f,-374.000000f,30.000000f,-0.001854f},
    {73.417755f,-496.433105f,48.731918f,-0.001854f},
    {-157.409195f,31.206272f,77.050598f,-0.001854f},
    {-531.217834f,-405.231384f,49.551376f,-0.001854f},
    {-1090.476807f,-253.308670f,57.672371f,-0.001854f},
    {-1496.065063f,-333.338409f,101.134804f,-0.001854f},
    {873.001770f,-491.283630f,96.541931f,-0.001854f},
    {-1437.670044f,-610.088989f,51.161900f,-0.001854f},
 //grave
 //firstaid
    {635.17f,-29.5594f,46.5056f,4.81711f},
    {642.488f,-32.9437f,46.365f,4.67748f},
    {642.326f,-27.9442f,46.9211f,4.59022f},
    {635.945f,-33.6171f,45.7164f,4.97419f},
   //stormpike
    {669.272f,-297.304f,30.291f,4.66604f},
    {674.08f,-292.328f,30.4817f,0.0918785f},
    {667.01f,-288.532f,29.8809f,1.81583f},
    {664.153f,-294.042f,30.2851f,3.28531f},
  //stone
    {81.7027f,-406.135f,47.7843f,0.598464f},
    {78.1431f,-409.215f,48.0401f,5.05953f},
    {73.4135f,-407.035f,46.7527f,3.34736f},
    {78.2258f,-401.859f,46.4202f,2.05852f},
  //snowfall
    {-207.412f,-110.616f,78.7959f,2.43251f},
    {-197.95f,-112.205f,78.5686f,6.22441f},
    {-202.709f,-116.829f,78.4358f,5.13742f},
    {-202.059f,-108.314f,78.5783f,5.91968f},
  //ice
    {-615.501f,-393.802f,60.4299f,3.06147f},
    {-608.513f,-392.717f,62.5724f,2.06323f},
    {-609.769f,-400.072f,60.7174f,5.22367f},
    {-616.093f,-398.293f,60.5628f,3.73613f},
  //frost
    {-1077.7f,-340.21f,55.4682f,6.25569f},
    {-1082.74f,-333.821f,54.7962f,2.05459f},
    {-1090.66f,-341.267f,54.6768f,3.27746f},
    {-1081.58f,-344.63f,55.256f,4.75636f},
  //frost hut
    {-1408.95f,-311.69f,89.2536f,4.49954f},
    {-1407.15f,-305.323f,89.1993f,2.86827f},
    {-1400.64f,-304.3f,89.7008f,1.0595f},
    {-1400.4f,-311.35f,89.3028f,4.99434f},
  //towers
  //dun south - OK
    {569.395f,-101.064f,52.8296f,2.34974f},
    {574.85f,-92.9842f,52.5869f,3.09325f},
    {575.411f,-83.597f,52.3626f,6.26573f},
    {571.352f,-75.6582f,52.479f,0.523599f},
    //dun north - OK
    {668.60f,-122.53f,64.12f,2.34f}, //not 100% ok
    {662.253f,-129.105f,64.1794f,2.77507f},
    {661.209f,-138.877f,64.2251f,3.38594f},
    {665.481f,-146.857f,64.1271f,3.75246f},
    //icewing - OK
    {225.228f,-368.909f,56.9983f,6.23806f},
    {191.36f,-369.899f,57.1524f,3.24631f},
    {215.518f,-384.019f,56.9889f,5.09636f},
    {199.625f,-382.177f,56.8691f,4.08407f},
    //stone
    {-172.851f,-452.366f,40.8725f,3.31829f},
    {-147.147f,-435.053f,40.8022f,0.599238f},
    {-169.456f,-440.325f,40.985f,2.59101f},
    {-163.494f,-434.904f,41.0725f,1.84174f},
    //ice - OK
    {-573.522f,-271.854f,75.0078f,3.9619f},
    {-565.616f,-269.051f,74.9952f,5.02655f},
    {-562.825f,-261.087f,74.9898f,5.95157f},
    {-569.176f,-254.446f,74.8771f,0.820305f},
    //towerpoint
    {-763.04f,-371.032f,90.7933f,5.25979f},
    {-759.764f,-358.264f,90.8681f,0.289795f},
    {-768.808f,-353.056f,90.8811f,1.52601f},
    {-775.944f,-362.639f,90.8949f,2.59573f},
    //frost etower
    {-1294.13f,-313.045f,107.328f,0.270162f},
    {-1306.5f,-308.105f,113.767f,1.78755f},
    {-1294.78f,-319.966f,113.79f,5.94545f},
    {-1294.83f,-312.241f,113.799f,0.295293f},
    //frost wtower
    {-1300.96f,-275.111f,114.058f,4.12804f},
    {-1302.41f,-259.256f,114.065f,1.67602f},
    {-1287.97f,-262.087f,114.165f,6.18264f},
    {-1291.59f,-271.166f,114.151f,5.28257f}

};


enum BG_AV_CreatureIds
{

    AV_NPC_A_GRAVEDEFENSE0 = 0,     // stormpike Defender
    AV_NPC_A_GRAVEDEFENSE1 = 1,     // seasoned defender
    AV_NPC_A_GRAVEDEFENSE2 = 2,     // veteran defender
    AV_NPC_A_GRAVEDEFENSE3 = 3,     // champion defender
    AV_NPC_A_TOWERDEFENSE  = 4,     // stormpike bowman

    AV_NPC_H_GRAVEDEFENSE0 = 5,     // frostwolf guardian
    AV_NPC_H_GRAVEDEFENSE1 = 6,     // seasoned guardian
    AV_NPC_H_GRAVEDEFENSE2 = 7,     // veteran guardian
    AV_NPC_H_GRAVEDEFENSE3 = 8,    // champion guardian
    AV_NPC_H_TOWERDEFENSE  = 9,    // frostwolf bowman

    AV_NPC_INFO_MAX        = 10

};

//entry, team, minlevel, maxlevel
//TODO this array should be removed, the only needed things are the entrys (for spawning(?) and handlekillunit)
const uint32 BG_AV_CreatureInfo[AV_NPC_INFO_MAX][4] = {
    { 12050, 1216, 58, 58 }, //Stormpike Defender
    { 13326, 1216, 59, 59 }, //Seasoned Defender
    { 13331, 1216, 60, 60 }, //Veteran Defender
    { 13422, 1216, 61, 61 }, //Champion Defender
    { 13358, 1216, 59, 60 }, //Stormpike Bowman //i think its 60,61 and 69,70.. but this is until now not possible TODO look if this is ok
    { 12053, 1214, 58, 58 }, //Frostwolf Guardian
    { 13328, 1214, 59, 59 }, //Seasoned Guardian
    { 13332, 1214, 60, 60 }, //Veteran Guardian
    { 13421, 1214, 61, 61 }, //Champion Guardian
    { 13359, 1214, 59, 60 }, //Frostwolf Bowman

};

enum BG_AV_DB_Creatures
{
    AV_CREATURE_A_CAPTAIN   = 1,
    AV_CREATURE_H_CAPTAIN   = 2,
    AV_CREATURE_HERALD      = 3,
    AV_CREATURE_A_BOSS      = 4,
    AV_CREATURE_H_BOSS      = 5,
    AV_CREATURE_SNIFFLE     = 6,
    AV_CREATURE_MARSHAL     = 7,//4alliance marshals + 4 horde marshals their ids are similar to those of the tower-ids
    AV_DB_CREATURE_MAX      = 15
};

enum BG_AV_MineCreature_Entries
{
    //North_Mine_Neutral_X 4 is always boss
    N_M_N_1 = 10987,
    N_M_N_2 = 11600,
    N_M_N_3 = 11602,
    N_M_N_4 = 11657,
    N_M_A_1 = 13396,
    N_M_A_2 = 13080,
    N_M_A_3 = 13098,
    N_M_A_4 = 13078,
    N_M_H_1 = 13397,
    N_M_H_2 = 13099,
    N_M_H_3 = 13081,
    N_M_H_4 = 13079,
    //South_Mine_Neutral_X 4 is always boss
    S_M_N_1 = 11603,
    S_M_N_2 = 11604,
    S_M_N_3 = 11605,
    S_M_N_4 = 11677,
    S_M_N_5 = 10982,
    S_M_A_1 = 13317,
    S_M_A_2 = 13096,
    S_M_A_3 = 13087,
    S_M_A_4 = 13086,
    S_M_H_1 = 13316,
    S_M_H_2 = 13097,
    S_M_H_3 = 13089,
    S_M_H_4 = 13088
};

enum BG_AV_Graveyards
{
    AV_GRAVE_STORM_AID         = 751,
    AV_GRAVE_STORM_GRAVE       = 689,
    AV_GRAVE_STONE_GRAVE       = 729,
    AV_GRAVE_SNOWFALL          = 169,
    AV_GRAVE_ICE_GRAVE         = 749,
    AV_GRAVE_FROSTWOLF         = 690,
    AV_GRAVE_FROST_HUT         = 750,
    AV_GRAVE_MAIN_ALLIANCE     = 611,
    AV_GRAVE_MAIN_HORDE        = 610
};



const uint32 BG_AV_GraveyardIds[9]= {
  AV_GRAVE_STORM_AID,
  AV_GRAVE_STORM_GRAVE,
  AV_GRAVE_STONE_GRAVE,
  AV_GRAVE_SNOWFALL,
  AV_GRAVE_ICE_GRAVE,
  AV_GRAVE_FROSTWOLF,
  AV_GRAVE_FROST_HUT,
  AV_GRAVE_MAIN_ALLIANCE,
  AV_GRAVE_MAIN_HORDE
};

enum BG_AV_BUFF
{ //TODO add all other buffs here
    AV_BUFF_ARMOR = 21163,
    AV_BUFF_A_CAPTAIN = 23693, //the buff which the alliance captain does
    AV_BUFF_H_CAPTAIN = 22751 //the buff which the horde captain does
};
enum BG_AV_States
{
    POINT_NEUTRAL              =  0,
    POINT_ASSAULTED            =  1,
    POINT_DESTROYED            =  2,
    POINT_CONTROLED            =  3
};

enum BG_AV_WorldStates
{
    AV_Alliance_Score               = 3127,
    AV_Horde_Score                  = 3128,
    AV_SHOW_H_SCORE                 = 3133,
    AV_SHOW_A_SCORE                 = 3134,

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

    AV_N_MINE_N              = 1360,
    AV_N_MINE_A              = 1358,
    AV_N_MINE_H              = 1359,

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

//alliance_control neutral_control horde_control
const uint32 BG_AV_MineWorldStates[2][3] = {
    {1358, 1360,1359},
    {1355, 1357,1356}
};


//alliance_control alliance_assault h_control h_assault
const uint32 BG_AV_NodeWorldStates[16][4] = {
    //Stormpike first aid station
    {1325, 1326,1327,1328},
    //Stormpike Graveyard
    {1333,1335,1334,1336},
    //Stoneheart Grave
    {1302,1304,1301,1303},
    //Snowfall Grave
    {1341,1343,1342,1344},
    //Iceblood grave
    {1346,1348,1347,1349},
    //Frostwolf Grave
    {1337,1339,1338,1340},
    //Frostwolf Hut
    {1329,1331,1330,1332},
    //Dunbaldar South Bunker
    {1361,1375,1370,1378},
    //Dunbaldar North Bunker
    {1362,1374,1371,1379},
    //Icewing Bunker
    {1363,1376,1372,1380},
    //Stoneheart Bunker
    {1364,1377,1373,1381},
    //Iceblood Tower
    {1368,1390,1385,1395},
    //Tower Point
    {1367,1389,1384,1394},
    //Frostwolf East
    {1366,1388,1383,1393},
    //Frostwolf West
    {1365,1387,1382,1392},
};

enum BG_AV_QuestIds
{
    AV_QUEST_A_SCRAPS1      = 7223,
    AV_QUEST_A_SCRAPS2      = 6781,
    AV_QUEST_H_SCRAPS1      = 7224,
    AV_QUEST_H_SCRAPS2      = 6741,
    AV_QUEST_A_COMMANDER1   = 6942, //soldier
    AV_QUEST_H_COMMANDER1   = 6825,
    AV_QUEST_A_COMMANDER2   = 6941, //leutnant
    AV_QUEST_H_COMMANDER2   = 6826,
    AV_QUEST_A_COMMANDER3   = 6943, //commander
    AV_QUEST_H_COMMANDER3   = 6827,
    AV_QUEST_A_BOSS1        = 7386, // 5 cristal/blood
    AV_QUEST_H_BOSS1        = 7385,
    AV_QUEST_A_BOSS2        = 6881, // 1
    AV_QUEST_H_BOSS2        = 6801,
    AV_QUEST_A_NEAR_MINE    = 5892, //the mine near start location of team
    AV_QUEST_H_NEAR_MINE    = 5893,
    AV_QUEST_A_OTHER_MINE   = 6982, //the other mine ;)
    AV_QUEST_H_OTHER_MINE   = 6985,
    AV_QUEST_A_RIDER_HIDE   = 7026,
    AV_QUEST_H_RIDER_HIDE   = 7002,
    AV_QUEST_A_RIDER_TAME   = 7027,
    AV_QUEST_H_RIDER_TAME   = 7001
};

struct BG_AV_NodeInfo
{
    uint16       TotalOwner;
    uint16       Owner;
    uint16       PrevOwner;
    BG_AV_States State;
    BG_AV_States PrevState;
    int          Timer;
    bool         Tower;
};

inline BG_AV_Nodes &operator++(BG_AV_Nodes &i){ return i = BG_AV_Nodes(i + 1); }

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
        virtual void OnCreatureCreate(Creature* creature);
        virtual void OnObjectCreate(GameObject* obj);

        void RemovePlayer(Player *plr,uint64 guid);
        void HandleAreaTrigger(Player *Source, uint32 Trigger);
        bool SetupBattleGround();
        virtual void ResetBGSubclass();

        /*general stuff*/
        void UpdateScore(uint16 team, int16 points);
       void UpdatePlayerScore(Player *Source, uint32 type, uint32 value);

        /*handlestuff*/ //these are functions which get called from extern
        virtual void EventPlayerClickedOnFlag(Player *source, GameObject* target_obj);
        void HandleKillPlayer(Player* player, Player *killer);
        void HandleKillUnit(Creature *unit, Player *killer);
        void HandleQuestComplete(uint32 questid, Player *player);
        bool PlayerCanDoMineQuest(int32 GOId,uint32 team);


        void EndBattleGround(uint32 winner);

        virtual WorldSafeLocsEntry const* GetClosestGraveYard(float x, float y, float z, uint32 team);

    private:
        /* Nodes occupying */
        void EventPlayerAssaultsPoint(Player* player, uint32 object);
        void EventPlayerDefendsPoint(Player* player, uint32 object);
        void EventPlayerDestroyedPoint(BG_AV_Nodes node);

        void AssaultNode(BG_AV_Nodes node,uint16 team);
        void DestroyNode(BG_AV_Nodes node);
        void InitNode(BG_AV_Nodes node, uint16 team, bool tower);
        void DefendNode(BG_AV_Nodes node, uint16 team);

        void PopulateNode(BG_AV_Nodes node);
        void DePopulateNode(BG_AV_Nodes node);

        const BG_AV_Nodes GetNodeThroughObject(uint32 object);
        const uint32 GetObjectThroughNode(BG_AV_Nodes node);
        const char* GetNodeName(BG_AV_Nodes node);
        const bool IsTower(BG_AV_Nodes node) {   return m_Nodes[node].Tower; }


        /*mine*/
        void ChangeMineOwner(uint8 mine, uint32 team);

        /*worldstates*/
        void FillInitialWorldStates(WorldPacket& data);
        const uint8 GetWorldStateType(uint8 state, uint16 team);
        void SendMineWorldStates(uint32 mine);
        void UpdateNodeWorldState(BG_AV_Nodes node);

        /*general */
        Creature* AddAVCreature(uint16 cinfoid, uint16 type);
        const uint16 GetBonusHonor(uint8 kills); //TODO remove this when mangos handles this right

        /*variables */
        int32 m_Team_Scores[2];
        uint32 m_Team_QuestStatus[2][9]; //[x][y] x=team y=questcounter

        BG_AV_NodeInfo m_Nodes[BG_AV_NODES_MAX];

        uint32 m_Mine_Owner[2];
        uint32 m_Mine_PrevOwner[2]; //only for worldstates needed
        int32 m_Mine_Timer; //ticks for both teams
        uint32 m_Mine_Reclaim_Timer[2];
        uint32 m_CaptainBuffTimer[2];

        bool m_IsInformedNearVictory[2];

        Creature* m_DB_Creature[AV_DB_CREATURE_MAX];

        typedef std::vector<GameObject*> ObjectVector;
        typedef std::vector<Creature*> CreatureVector;
        ObjectVector m_SnowfallEyecandy[4];
        CreatureVector m_MineCreatures[2][3];
};

#endif
