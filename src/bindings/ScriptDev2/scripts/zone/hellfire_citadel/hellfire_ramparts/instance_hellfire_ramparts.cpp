/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
SDName: Instance_Hellfire_Ramparts
SD%Complete: 0
SDComment: 
SDCategory: Hellfire Citadel, Hellfire Ramparts
EndScriptData */

#include "precompiled.h"
#include "def_hellfire_ramparts.h"

#define ENCOUNTERS 2

struct MANGOS_DLL_DECL instance_hellfire_ramparts : public ScriptedInstance
{
    instance_hellfire_ramparts(Map *map) : ScriptedInstance(map) {Initialize();};

    uint32 Encounter[ENCOUNTERS];
    uint64 Chest_Normal_GUID;
    uint64 Chest_Heroic_GUID;

    void Initialize()
    {
        Chest_Normal_GUID = NULL;
        Chest_Heroic_GUID = NULL;

        for(uint8 i = 0; i < ENCOUNTERS; i++)
            Encounter[i] = NOT_STARTED;
    }

    void OnObjectCreate(GameObject *go)
    {
        switch(go->GetEntry())
        {
            case 185168: Chest_Normal_GUID = go->GetGUID(); break; //Chest normal
            case 185169: Chest_Heroic_GUID = go->GetGUID(); break; //Chest heroic
        }
    }

    Player* GetPlayerInMap()
    {
        Map::PlayerList const& players = instance->GetPlayers();

        if (!players.isEmpty())
        {
            for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                if (Player* plr = itr->getSource())
                    return plr;
            }
        }

        debug_log("SD2: Instance Hellfire Ramparts: GetPlayerInMap, but PlayerList is empty!");
        return NULL;
    }

     void HandleGameObjectSetFlag(uint64 guid, uint64 flag)
    {
        Player *player = GetPlayerInMap();

        if (!player || !guid)
        {
            debug_log("SD2: Hellfire Ramparts: HandleGameObject fail");
            return;
        }

        if (GameObject *go = GameObject::GetGameObject(*player,guid))
            go->SetUInt32Value (GAMEOBJECT_FLAGS, flag);
    }

    void SetData(uint32 type, uint32 data)
    {
        switch(type)
        {
            case TYPE_VAZRUDEN_DATA:
                Encounter[0] = data;
                break;
            case TYPE_NAZAN_DATA:
                Encounter[1] = data;
                break;
        }
        if (data == DONE)
        {
            if (Encounter[0] == DONE && Encounter[1] == DONE)
            {
                if (Chest_Normal_GUID)
                    HandleGameObjectSetFlag(Chest_Normal_GUID, 0);
                if (Chest_Heroic_GUID)
                    HandleGameObjectSetFlag(Chest_Heroic_GUID, 0);
            }
        }
    }

    uint32 GetData(uint32 type)
    {
        switch( type )
        {
            case TYPE_VAZRUDEN_DATA:
                return Encounter[0];
            case TYPE_NAZAN_DATA:
                return Encounter[1];
        }
        return 0;
    }
};

InstanceData* GetInstanceData_instance_hellfire_ramparts(Map* map)
{
    return new instance_hellfire_ramparts(map);
}

void AddSC_instance_hellfire_ramparts()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_hellfire_ramparts";
    newscript->GetInstanceData = &GetInstanceData_instance_hellfire_ramparts;
    newscript->RegisterSelf();
}