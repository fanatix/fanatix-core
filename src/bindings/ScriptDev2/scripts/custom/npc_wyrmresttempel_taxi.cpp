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
#include "WorldPacket.h"

#define GOSSIP_UP      "My Lord, I must go to the upper floor of the temple."
#define GOSSIP_DOWN    "I would like to take a flight to the ground, Lord Afrasastrasz."
#define GOSSIP_MIDDLE  "Can you spare a drake to travel to Lord Afrasastrasz, in the middle of the temple?"
#define GOSSIP_TOP     "Please, Let me take one of these dragons to the top floor of the temple."
#define GOSSIP_BOTTOM  "Yes, Please. I would like to return to the ground floor of the temple."
#define GOSSIP_ONEDOWN "I would like to see Lord Afrasastrasz, in the middle of the temple."

bool GossipHello_npc_wyrmresttempel_middle_taxi(Player *player, Creature *_Creature)
{
player->SetTaxiCheater(true);

player->ADD_GOSSIP_ITEM(0, GOSSIP_UP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
player->ADD_GOSSIP_ITEM(0, GOSSIP_DOWN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->SEND_GOSSIP_MENU(12887,_Creature->GetGUID()); 
        
    return true;
}

bool GossipSelect_npc_wyrmresttempel_middle_taxi(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {          
        player->GetSession()->SendDoFlight(6376, 881);
    }
    

    if (action == GOSSIP_ACTION_INFO_DEF + 2)
    {
      player->GetSession()->SendDoFlight(6376, 882);

    }
return true;
}

bool GossipHello_npc_wyrmresttempel_bottom_taxi(Player *player, Creature *_Creature)
{
player->SetTaxiCheater(true);

player->ADD_GOSSIP_ITEM(0, GOSSIP_TOP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
player->ADD_GOSSIP_ITEM(0, GOSSIP_MIDDLE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->SEND_GOSSIP_MENU(12713,_Creature->GetGUID());
        
    return true;
}



bool GossipSelect_npc_wyrmresttempel_bottom_taxi(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_INFO_DEF + 4)
    {         
        player->GetSession()->SendDoFlight(6376, 878);
    }
    

    if (action == GOSSIP_ACTION_INFO_DEF + 3)
    {
      player->GetSession()->SendDoFlight(6376, 883);

    }
return true;
}

bool GossipHello_npc_wyrmresttempel_top_taxi(Player *player, Creature *_Creature)
{
player->SetTaxiCheater(true);

player->ADD_GOSSIP_ITEM(0, GOSSIP_BOTTOM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
player->ADD_GOSSIP_ITEM(0, GOSSIP_ONEDOWN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
        player->SEND_GOSSIP_MENU(12714,_Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_wyrmresttempel_top_taxi(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_INFO_DEF + 5)
    {      
        player->GetSession()->SendDoFlight(6376, 879);
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 6)

    {
      player->GetSession()->SendDoFlight(6376, 880);

    }
return true;
}

void AddSC_npc_wyrmresttempel_taxi()
{
    Script *newscript;

newscript = new Script;
newscript->Name="npc_wyrmresttempelmiddle_taxi";
newscript->pGossipHello = &GossipHello_npc_wyrmresttempel_middle_taxi;
newscript->pGossipSelect = &GossipSelect_npc_wyrmresttempel_middle_taxi;
newscript->RegisterSelf();

newscript = new Script;
newscript->Name="npc_wyrmresttempelbottom_taxi";
newscript->pGossipHello = &GossipHello_npc_wyrmresttempel_bottom_taxi;
newscript->pGossipSelect = &GossipSelect_npc_wyrmresttempel_bottom_taxi;
newscript->RegisterSelf();

newscript = new Script;
newscript->Name="npc_wyrmresttempeltop_taxi";
newscript->pGossipHello = &GossipHello_npc_wyrmresttempel_top_taxi;
newscript->pGossipSelect = &GossipSelect_npc_wyrmresttempel_top_taxi;
newscript->RegisterSelf();

}
