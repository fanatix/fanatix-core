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

#define GOSSIP_FLIGHT "I need a ride"

bool GossipHello_npc_acherus_taxi(Player *player, Creature *_Creature)
{
player->SetTaxiCheater(true);

player->ADD_GOSSIP_ITEM(0, GOSSIP_FLIGHT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(9978,_Creature->GetGUID());
    return true;
}

bool GossipSelect_npc_acherus_taxi(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    { 
       if (player->GetPositionZ() >=316)       
        {
        player->GetSession()->SendDoFlight(28108, 1053); 
        }else{
        player->GetSession()->SendDoFlight(28108, 1054);
        }

    }
    return true;
}

void AddSC_npc_acherus_taxi()
{
    Script *newscript;

newscript = new Script;
newscript->Name="npc_acherus_taxi";
newscript->pGossipHello = &GossipHello_npc_acherus_taxi;
newscript->pGossipSelect = &GossipSelect_npc_acherus_taxi;
newscript->RegisterSelf();
}
