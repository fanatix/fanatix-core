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
SDName: Ghostlands
SD%Complete: 100
SDComment: Quest support: 9212, 9692. Obtain Budd's Guise of Zul'aman. Vendor Rathis Tomber
SDCategory: Ghostlands
EndScriptData */

/* ContentData
npc_blood_knight_dawnstar
npc_budd_nedreck
npc_ranger_lilatha
npc_rathis_tomber
EndContentData */

#include "precompiled.h"
#include "../../npc/npc_escortAI.h"

/*######
## npc_blood_knight_dawnstar
######*/

#define GOSSIP_ITEM_INSIGNIA    "Take Blood Knight Insignia"

bool GossipHello_npc_blood_knight_dawnstar(Player *player, Creature *_Creature)
{
    if (player->GetQuestStatus(9692) == QUEST_STATUS_INCOMPLETE && !player->HasItemCount(24226,1,true))
        player->ADD_GOSSIP_ITEM(0,GOSSIP_ITEM_INSIGNIA,GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_blood_knight_dawnstar(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
    {
        ItemPosCountVec dest;
        uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 24226, 1, false);
        if (msg == EQUIP_ERR_OK)
        {
            player->StoreNewItem( dest, 24226, 1, true);
            player->PlayerTalkClass->ClearMenus();
        }
    }
    return true;
}

/*######
## npc_budd_nedreck
######*/

#define GOSSIP_ITEM_DISGUISE        "You gave the crew disguises?"

bool GossipHello_npc_budd_nedreck(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID());

    if (player->GetQuestStatus(11166) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM(0,GOSSIP_ITEM_DISGUISE,GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF);

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(),_Creature->GetGUID());
    return true;
}

bool GossipSelect_npc_budd_nedreck(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action==GOSSIP_ACTION_INFO_DEF)
    {
        player->CLOSE_GOSSIP_MENU();
        _Creature->CastSpell(player, 42540, false);
    }
    return true;
}

/*######
## npc_ranger_lilatha
######*/

enum
{
    SAY_START           = -1000140,
    SAY_PROGRESS1       = -1000141,
    SAY_PROGRESS2       = -1000142,
    SAY_PROGRESS3       = -1000143,
    SAY_END1            = -1000144,
    SAY_END2            = -1000145,
    CAPTAIN_ANSWER      = -1000146,

    QUEST_CATACOMBS     = 9212,
    GO_CAGE             = 181152,
    NPC_CAPTAIN_HELIOS  = 16220,
    FACTION_SMOON_E     = 1603,
};

struct MANGOS_DLL_DECL npc_ranger_lilathaAI : public npc_escortAI
{
    npc_ranger_lilathaAI(Creature *c) : npc_escortAI(c)
    {
        m_uiNormFaction = c->getFaction();
        m_uiGoCageGUID = 0;
        Reset();
    }

    uint32 m_uiNormFaction;
    uint64 m_uiGoCageGUID;

    GameObject* GetGoCage()
    {
        GameObject* pGo = NULL;

        CellPair pair(MaNGOS::ComputeCellPair(m_creature->GetPositionX(), m_creature->GetPositionY()));
        Cell cell(pair);
        cell.data.Part.reserved = ALL_DISTRICT;
        cell.SetNoCreate();

        MaNGOS::NearestGameObjectEntryInObjectRangeCheck go_check(*m_creature, GO_CAGE, 10);
        MaNGOS::GameObjectLastSearcher<MaNGOS::NearestGameObjectEntryInObjectRangeCheck> searcher(m_creature, pGo, go_check);

        TypeContainerVisitor<MaNGOS::GameObjectLastSearcher<MaNGOS::NearestGameObjectEntryInObjectRangeCheck>, GridTypeMapContainer> go_searcher(searcher);

        CellLock<GridReadGuard> cell_lock(cell, pair);
        cell_lock->Visit(cell_lock, go_searcher,*(m_creature->GetMap()));

        return pGo;
    }

    void CaptainAnswer()
    {
        Creature* pCreature = NULL;

        CellPair pair(MaNGOS::ComputeCellPair(m_creature->GetPositionX(), m_creature->GetPositionY()));
        Cell cell(pair);
        cell.data.Part.reserved = ALL_DISTRICT;
        cell.SetNoCreate();

        MaNGOS::NearestCreatureEntryWithLiveStateInObjectRangeCheck creature_check(*m_creature, NPC_CAPTAIN_HELIOS, true, 30);
        MaNGOS::CreatureLastSearcher<MaNGOS::NearestCreatureEntryWithLiveStateInObjectRangeCheck> searcher(m_creature, pCreature, creature_check);

        TypeContainerVisitor<MaNGOS::CreatureLastSearcher<MaNGOS::NearestCreatureEntryWithLiveStateInObjectRangeCheck>, GridTypeMapContainer> creature_searcher(searcher);

        CellLock<GridReadGuard> cell_lock(cell, pair);
        cell_lock->Visit(cell_lock, creature_searcher,*(m_creature->GetMap()));

        if (pCreature)
            DoScriptText(CAPTAIN_ANSWER, pCreature);
        else
            error_log("SD2: npc_ranger_lilatha: Captain Helios not found!");
    }

    void WaypointReached(uint32 i)
    {
        Unit* pPlayer = Unit::GetUnit((*m_creature), PlayerGUID);

        if (!pPlayer || pPlayer->GetTypeId() != TYPEID_PLAYER)
            return;

        switch(i)
        {
            case 0:
                if (GameObject* pGoTemp = GetGoCage())
                    m_uiGoCageGUID = pGoTemp->GetGUID();

                m_creature->SetStandState(UNIT_STAND_STATE_STAND);

                if (GameObject* pGo = GameObject::GetGameObject(*m_creature,m_uiGoCageGUID))
                    pGo->SetGoState(0);

                DoScriptText(SAY_START, m_creature, pPlayer);
                break;
            case 1:
                if (GameObject* pGo = GameObject::GetGameObject(*m_creature,m_uiGoCageGUID))
                    pGo->SetGoState(1);
                break;
            case 5:
                DoScriptText(SAY_PROGRESS1, m_creature, pPlayer);
                break;
            case 11:
                DoScriptText(SAY_PROGRESS2, m_creature, pPlayer); 
                break;
            case 18:
                DoScriptText(SAY_PROGRESS3, m_creature, pPlayer);
                if (Creature* pSum1 = m_creature->SummonCreature(16342, 7627.083984, -7532.538086, 152.128616, 1.082733, TEMPSUMMON_DEAD_DESPAWN, 0))
                    pSum1->AI()->AttackStart(m_creature);
                if (Creature* pSum2 = m_creature->SummonCreature(16343, 7620.432129, -7532.550293, 152.454865, 0.827478, TEMPSUMMON_DEAD_DESPAWN, 0))
                    pSum2->AI()->AttackStart(pPlayer);
                break;
            case 19:
                m_creature->SetSpeed(MOVE_RUN, 1.5f);
                break; 
            case 25:
                m_creature->SetSpeed(MOVE_WALK, 1.0f);
                break;
            case 30:
                ((Player*)pPlayer)->GroupEventHappens(QUEST_CATACOMBS,m_creature);
                break;
            case 32:
                DoScriptText(SAY_END1, m_creature, pPlayer);
                break;
            case 33:
                DoScriptText(SAY_END2, m_creature, pPlayer);
                CaptainAnswer();
                break;
        }
    }

    void Reset()
    {
        if (!IsBeingEscorted)
        {
            m_creature->setFaction(m_uiNormFaction);
            m_uiGoCageGUID = 0;
        }
    }

    void Aggro(Unit* who) {}

    void JustDied(Unit* killer)
    {
        if (IsBeingEscorted && PlayerGUID)
        {
            if (Unit* pPlayer = Unit::GetUnit((*m_creature), PlayerGUID))
                ((Player*)pPlayer)->FailQuest(QUEST_CATACOMBS);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        npc_escortAI::UpdateAI(diff);
    }
};

CreatureAI* GetAI_npc_ranger_lilathaAI(Creature* pCreature)
{
    npc_ranger_lilathaAI* ranger_lilathaAI = new npc_ranger_lilathaAI(pCreature);

    ranger_lilathaAI->FillPointMovementListForCreature();

    return (CreatureAI*)ranger_lilathaAI;
}

bool QuestAccept_npc_ranger_lilatha(Player* pPlayer, Creature* pCreature, Quest const* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_CATACOMBS)
    {
        pCreature->setFaction(FACTION_SMOON_E);
        ((npc_escortAI*)(pCreature->AI()))->Start(true, true, false, pPlayer->GetGUID());
    }
    return true;
}

/*######
## npc_rathis_tomber
######*/

bool GossipHello_npc_rathis_tomber(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu(_Creature->GetGUID());

    if (_Creature->isVendor() && player->GetQuestRewardStatus(9152))
    {
        player->ADD_GOSSIP_ITEM(1, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
        player->SEND_GOSSIP_MENU(8432,_Creature->GetGUID());
    }else
        player->SEND_GOSSIP_MENU(8431,_Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_rathis_tomber(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_TRADE)
        player->SEND_VENDORLIST(_Creature->GetGUID());

    return true;
}

void AddSC_ghostlands()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_blood_knight_dawnstar";
    newscript->pGossipHello = &GossipHello_npc_blood_knight_dawnstar;
    newscript->pGossipSelect = &GossipSelect_npc_blood_knight_dawnstar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_budd_nedreck";
    newscript->pGossipHello = &GossipHello_npc_budd_nedreck;
    newscript->pGossipSelect = &GossipSelect_npc_budd_nedreck;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_ranger_lilatha";
    newscript->GetAI = &GetAI_npc_ranger_lilathaAI;
    newscript->pQuestAccept = &QuestAccept_npc_ranger_lilatha;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_rathis_tomber";
    newscript->pGossipHello = &GossipHello_npc_rathis_tomber;
    newscript->pGossipSelect = &GossipSelect_npc_rathis_tomber;
    newscript->RegisterSelf();
}
