/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Instance_Molten_Core
SD%Complete: 80
SDComment: Chest for Majordomo missing
SDCategory: Molten Core
EndScriptData */

#include "precompiled.h"
#include "def_molten_core.h"

#define ENCOUNTERS      9

#define ID_LUCIFRON     12118
#define ID_MAGMADAR     11982
#define ID_GEHENNAS     12259
#define ID_GARR         12057
#define ID_GEDDON       12056
#define ID_SHAZZRAH     12264
#define ID_GOLEMAGG     11988
#define ID_DOMO			12018
#define ID_SULFURON     12098
#define ID_RAGNAROS     11502
#define ID_FLAMEWAKERPRIEST     11662

#define ID_CACHEOFTHEFIRELORD	179703

struct MANGOS_DLL_SPEC instance_molten_core : public ScriptedInstance
{
    instance_molten_core(Map *Map) : ScriptedInstance(Map) {Initialize();}

    //uint64 RuneGUID[7];
    
    //If all necessary Bosses are dead.
    bool SulfuronDead;
	bool GeddonDead;
	bool ShazzrahDead;
	bool GolemaggDead;
	bool GarrDead;
	bool MagmadarDead;
	bool GehennasDead;

    uint64 Lucifron, Magmadar, Gehennas, Garr, Geddon, Shazzrah, Sulfuron, Golemagg, Domo, Ragnaros, FlamewakerPriest, CacheOfTheFirelordGuid;

	uint32 FireswornDead;

	GameObject *CacheOfTheFirelord;

    void Initialize()
    {
        Lucifron = 0;
        Magmadar = 0;
        Gehennas = 0;
        Garr = 0;
        Geddon = 0;
        Shazzrah = 0;
        Sulfuron = 0;
        Golemagg = 0;
        Domo = 0;
        Ragnaros = 0;
        FlamewakerPriest = 0;
		CacheOfTheFirelordGuid = 0;

        SulfuronDead = false;
		GeddonDead = false;
		ShazzrahDead = false;
		GolemaggDead = false;
		GarrDead = false;
		MagmadarDead = false;
		GehennasDead = false;

		FireswornDead = 0;

		CacheOfTheFirelord = NULL;
	}

    //Called every map update
    void Update(uint32 diff) 
    {
    }

    //Used by the map's CanEnter function.
    //This is to prevent players from entering during boss encounters.
    bool IsEncounterInProgress() const 
    {
        return false; 
    };

    //Called when a gameobject is created
    void OnObjectCreate(GameObject *obj) 
    {
		switch(obj->GetEntry())
		{
		case ID_CACHEOFTHEFIRELORD:
				CacheOfTheFirelord = obj;
				CacheOfTheFirelordGuid = obj->GetGUID();
				//CacheOfTheFirelord->SetUInt32Value(GAMEOBJECT_FLAGS, 33);
				break;
		}
    }

    //called on creature creation
    void OnCreatureCreate(Creature *creature, uint32 creature_entry)
    {
        switch (creature_entry)
        {
            case ID_LUCIFRON:
                Lucifron = creature->GetGUID();
                break;
            case ID_MAGMADAR:
                Magmadar = creature->GetGUID();
                break;
            case ID_GEHENNAS:
                Gehennas = creature->GetGUID();
                break;
            case ID_GARR:
                Garr = creature->GetGUID();
                break;
            case ID_GEDDON:
                Geddon = creature->GetGUID();
                break;
            case ID_SHAZZRAH:
                Shazzrah = creature->GetGUID();
                break;
            case ID_SULFURON:
                Sulfuron = creature->GetGUID();
                break;
            case ID_GOLEMAGG:
                Golemagg = creature->GetGUID();
                break;
            case ID_DOMO:
                Domo = creature->GetGUID();
				creature->setFaction(35);
				creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				creature->SetVisibility(VISIBILITY_OFF);
                break;
            case ID_RAGNAROS:
                Ragnaros = creature->GetGUID();
				creature->setFaction(35);
				creature->CastSpell(creature,20568,false);
                break;
            case ID_FLAMEWAKERPRIEST:
                FlamewakerPriest = creature->GetGUID();
                break;
        }
    }

    uint64 GetData64(uint32 data)
    {
        switch(data)
        {
            case DATA_SULFURON:
                return Sulfuron;
            case DATA_GOLEMAGG:
                return Golemagg;
				break;
			case DATA_CACHEOFTHEFIRELORD_GUID:
				return CacheOfTheFirelordGuid;
				break;
			default:
				return 0;
				break;
        }
    } // end GetData64

    uint32 GetData(uint32 type)
    {
        switch(type)
        {
			case DATA_ALL_BOSSES_DEAD:
				if(GeddonDead /*&& SulfuronDead && ShazzrahDead && GolemaggDead && GarrDead && MagmadarDead && GehennasDead*/)
					return 1;
				else return 0;
				break;

			case DATA_FIRESWORNISDEAD:
				return FireswornDead;
				break;
        }
		return 0;
    }


    void SetData(uint32 type, uint32 data)
    {
		switch(type)
		{
		case DATA_FIRESWORN_DEATH:
			FireswornDead++;
			break;

		case DATA_MAJORDOMO_SURRENDER:
			//CacheOfTheFirelord->SetUInt32Value(GAMEOBJECT_FLAGS, 0);
			CacheOfTheFirelord->Respawn();
			break;

		case DATA_SULFURON_DEAD:
			SulfuronDead=true;
			break;

		case DATA_GEDDON_DEAD:
			GeddonDead=true;
			break;

		case DATA_SHAZZRAH_DEAD:
			ShazzrahDead=true;
			break;

		case DATA_GOLEMAGG_DEAD:
			GolemaggDead=true;
			break;

			case DATA_GARR_DEAD:
			GarrDead=true;
			break;

		case DATA_MAGMADAR_DEAD:
			MagmadarDead=true;
			break;

		case DATA_GEHENNAS_DEAD:
			GehennasDead=true;
			break;
		}
    }
};

InstanceData* GetInstance_instance_molten_core(Map *Map)
{
    return new instance_molten_core (Map);
}

void AddSC_instance_molten_core()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_molten_core";
    newscript->GetInstanceData = &GetInstance_instance_molten_core;
    newscript->RegisterSelf();
}
