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

#include "precompiled.h"
long long int money;
int costo;

bool GossipHello_telefratz(Player *player, Creature *_Creature)
{
	if ( player->GetTeam() == ALLIANCE ) 
	{
		player->ADD_GOSSIP_ITEM( 5, "Darnassus - 5 Silber"		, GOSSIP_SENDER_MAIN, 1203);
		player->ADD_GOSSIP_ITEM( 5, "Exodar - 5 Silber"			, GOSSIP_SENDER_MAIN, 1216);
		player->ADD_GOSSIP_ITEM( 5, "Sturmwind - 5 Silber"		, GOSSIP_SENDER_MAIN, 1206);
		player->ADD_GOSSIP_ITEM( 5, "Eisenschmiede - 5 Silber"		, GOSSIP_SENDER_MAIN, 1224);
		player->ADD_GOSSIP_ITEM( 5, "Shattrath - 5 Silber"            , GOSSIP_SENDER_MAIN, 1287);
		player->ADD_GOSSIP_ITEM( 5, "Flammenschlund - 10 Silber"			 , GOSSIP_SENDER_MAIN, 1248);
	}
    if ( player->GetTeam() == HORDE ) 
	{
		player->ADD_GOSSIP_ITEM( 5, "Orgrimmar - 5 Silber"		, GOSSIP_SENDER_MAIN, 1215);
		player->ADD_GOSSIP_ITEM( 5, "Silbermond - 5 Silber"		, GOSSIP_SENDER_MAIN, 1217);
		player->ADD_GOSSIP_ITEM( 5, "Unterstadt - 5 Silber"		, GOSSIP_SENDER_MAIN, 1213);
		player->ADD_GOSSIP_ITEM( 5, "Donnerfels - 5 Silber"	, GOSSIP_SENDER_MAIN, 1225);
		player->ADD_GOSSIP_ITEM( 5, "Shattrath - 5 Silber"            , GOSSIP_SENDER_MAIN, 1287);
		player->ADD_GOSSIP_ITEM( 5, "Das Verliess - 10 Silver"			 , GOSSIP_SENDER_MAIN, 1253);
	}
	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
	return true;
}

void SendDefaultMenu_telefratz(Player *player, Creature *_Creature, uint32 action )
{
	if(!player->getAttackers().empty())
	{
		player->CLOSE_GOSSIP_MENU();
		_Creature->MonsterSay("Du bist im Kampfmodus!", LANG_UNIVERSAL, NULL);
		return;
    }
	if( player->getLevel() < 0  ) 
    {
	player->CLOSE_GOSSIP_MENU();
    _Creature->MonsterSay("Du musst mindestens level 8 sein.", LANG_UNIVERSAL, NULL);
		return;
	}
	switch(action)
	{	
		money = player-> GetMoney();
		costo = 500;

		if (money < costo ) 
		{
			player->CLOSE_GOSSIP_MENU();
			_Creature->MonsterSay("Du hast nicht genug Gold!", LANG_UNIVERSAL, NULL);
			return;
		}
		// Teleport to Stormwind
		case 1206:
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(0, -8960.14f, 516.266f, 96.3568f, 3.560470f);
		player->ModifyMoney(-1*costo);
		return;

		// Teleport to Eisenschmiede
		case 1224:
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(0, -4924.07f, -951.95f, 501.55f, 5.40f);
		player->ModifyMoney(-1*costo);
		break;

		// Teleport to Exodar
		case 1216:
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(530, -4073.03f, -12020.4f, -1.47f, 0.0f);
		player->ModifyMoney(-1*costo);
		break;

		case 1203: // Teleport to Darnassus
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(1, 9947.52f, 2482.73f, 1316.21f, 0.0f);
		player->ModifyMoney(-1*costo);
		break;

		//teleport player to the Stockade
		case 1253:
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(0, -8769.76f,813.08f,97.63f,2.26f);
		player->ModifyMoney(-2*costo);	
		break;

		// Teleport to Understadt
		case 1213:
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(0, 1819.71f, 238.79f, 60.5321f, 0.0f);
		player->ModifyMoney(-1*costo);
		break;

		// Teleport to Orgrimmar
		case 1215:
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(1, 1552.5f, -4420.66f, 8.94802f, 0.409934f);
		player->ModifyMoney(-1*costo);
		break;

		// Teleport to Silbermond
		case 1217:
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(530, 9338.74f, -7277.27f, 13.7895f, 0.0f);
		player->ModifyMoney(-1*costo);
		break;

		// Teleport to Donnerfels
		case 1225:
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(1, -1280.19f,127.21f,131.35f,5.16f); 
		player->ModifyMoney(-1*costo);
		break;

		//teleport player to Flammenschlund
		case 1248:
		player->CLOSE_GOSSIP_MENU();
		player->TeleportTo(1, 1800.53f,-4394.68f,-17.93f,5.49f);
		player->ModifyMoney(-2*costo);
		break;
		// Shattrath City
		case 1287:
			if( player->getLevel() >= 65)
			{
				player->CLOSE_GOSSIP_MENU();
				player->TeleportTo(530, -1850.209961f, 5435.821777f, -10.961435f, 3.403913f);
				player->ModifyMoney(-1*costo);
			} 
			else 
			{
				player->CLOSE_GOSSIP_MENU();
				_Creature->MonsterSay("Du musst mindestens level 65 sein!", LANG_UNIVERSAL, NULL);
			}
		break;
	}
}

bool GossipSelect_telefratz(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
	if (sender == GOSSIP_SENDER_MAIN)
		SendDefaultMenu_telefratz(player, _Creature, action );
		return true;
}

void AddSC_telefratz()
{
Script *newscript;

newscript = new Script;
newscript->Name = "telefratz";
newscript->pGossipHello = &GossipHello_telefratz;
newscript->pGossipSelect = &GossipSelect_telefratz;
newscript->RegisterSelf();
}
