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
SDName: Blackrock_Depths
SD%Complete: 50
SDComment: Quest support: 4001, 4342, 7604. Vendor Lokhtos Darkbargainer.
SDCategory: Blackrock Depths
EndScriptData */

/* ContentData
at_ring_of_law
npc_grimstone
mob_phalanx
npc_kharan_mighthammer
npc_lokhtos_darkbargainer
EndContentData */

#include "precompiled.h"
#include "def_blackrock_depths.h"
#include "../../npc/npc_escortAI.h"

#define C_GRIMSTONE         10096
#define C_THELDREN          16059

//4 or 6 in total? 1+2+1 / 2+2+2 / 3+3. Depending on this, code should be changed.
#define MOB_AMOUNT          4

uint32 RingMob[]=
{
    8925,                                                   // Dredge Worm
    8926,                                                   // Deep Stinger
    8927,                                                   // Dark Screecher
    8928,                                                   // Burrowing Thundersnout
    8933,                                                   // Cave Creeper
    8932,                                                   // Borer Beetle
};

uint32 RingBoss[]=
{
    9027,                                                   // Gorosh
    9028,                                                   // Grizzle
    9029,                                                   // Eviscerator
    9030,                                                   // Ok'thor
    9031,                                                   // Anub'shiah
    9032,                                                   // Hedrum
};

bool AreaTrigger_at_ring_of_law(Player *player, AreaTriggerEntry *at)
{
    ScriptedInstance* pInstance = ((ScriptedInstance*)player->GetInstanceData());

    if (pInstance)
    {
        if (pInstance->GetData(TYPE_RING_OF_LAW) == IN_PROGRESS || pInstance->GetData(TYPE_RING_OF_LAW) == DONE)
            return false;

        pInstance->SetData(TYPE_RING_OF_LAW,IN_PROGRESS);
        player->SummonCreature(C_GRIMSTONE,625.559,-205.618,-52.735,2.609,TEMPSUMMON_DEAD_DESPAWN,0);

        return false;
    }
    return false;
}

/*######
## npc_grimstone
######*/

//TODO: implement quest part of event (different end boss)
struct MANGOS_DLL_DECL npc_grimstoneAI : public npc_escortAI
{
    npc_grimstoneAI(Creature *c) : npc_escortAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        MobSpawnId = rand()%6;
        Reset();
    }

    ScriptedInstance* pInstance;

    uint8 EventPhase;
    uint32 Event_Timer;

    uint8 MobSpawnId;
    uint8 MobCount;
    uint32 MobDeath_Timer;

    uint64 RingMobGUID[4];
    uint64 RingBossGUID;

    bool CanWalk;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        EventPhase = 0;
        Event_Timer = 1000;

        MobCount = 0;
        MobDeath_Timer = 0;

        for(uint8 i = 0; i < MOB_AMOUNT; i++)
            RingMobGUID[i] = 0;

        RingBossGUID = 0;

        CanWalk = false;
    }

    void Aggro(Unit *who) { }

    void DoGate(uint32 id, uint32 state)
    {
        if (GameObject *go = GameObject::GetGameObject(*m_creature,pInstance->GetData64(id)))
            go->SetGoState(state);

        debug_log("SD2: npc_grimstone, arena gate update state.");
    }

    //TODO: move them to center
    void SummonRingMob()
    {
        if (Creature* tmp = m_creature->SummonCreature(RingMob[MobSpawnId],608.960,-235.322,-53.907,1.857,TEMPSUMMON_DEAD_DESPAWN,0))
            RingMobGUID[MobCount] = tmp->GetGUID();

        ++MobCount;

        if (MobCount == MOB_AMOUNT)
            MobDeath_Timer = 2500;
    }

    //TODO: move them to center
    void SummonRingBoss()
    {
        if (Creature* tmp = m_creature->SummonCreature(RingBoss[rand()%6],644.300,-175.989,-53.739,3.418,TEMPSUMMON_DEAD_DESPAWN,0))
            RingBossGUID = tmp->GetGUID();

        MobDeath_Timer = 2500;
    }

    void WaypointReached(uint32 i)
    {
        switch(i)
        {
            case 0:
                DoScriptText(-1000000, m_creature);//2
                CanWalk = false;
                Event_Timer = 5000;
                break;
            case 1:
                DoScriptText(-1000000, m_creature);//4
                CanWalk = false;
                Event_Timer = 5000;
                break;
            case 2:
                CanWalk = false;
                break;
            case 3:
                DoScriptText(-1000000, m_creature);//5
                break;
            case 4:
                DoScriptText(-1000000, m_creature);//6
                CanWalk = false;
                Event_Timer = 5000;
                break;
            case 5:
                if (pInstance)
                {
                    pInstance->SetData(TYPE_RING_OF_LAW,DONE);
                    debug_log("SD2: npc_grimstone: event reached end and set complete.");
                }
                break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!pInstance)
            return;

        if (MobDeath_Timer)
        {
            if (MobDeath_Timer <= diff)
            {
                MobDeath_Timer = 2500;

                if (RingBossGUID)
                {
                    Creature *boss = (Creature*)Unit::GetUnit(*m_creature,RingBossGUID);
                    if (boss && !boss->isAlive() && boss->isDead())
                    {
                        RingBossGUID = 0;
                        Event_Timer = 5000;
                        MobDeath_Timer = 0;
                        return;
                    }
                    return;
                }

                for(uint8 i = 0; i < MOB_AMOUNT; i++)
                {
                    Creature *mob = (Creature*)Unit::GetUnit(*m_creature,RingMobGUID[i]);
                    if (mob && !mob->isAlive() && mob->isDead())
                    {
                        RingMobGUID[i] = 0;
                        --MobCount;

                        //seems all are gone, so set timer to continue and discontinue this
                        if (!MobCount)
                        {
                            Event_Timer = 5000;
                            MobDeath_Timer = 0;
                        }
                    }
                }
            }else MobDeath_Timer -= diff;
        }

        if (Event_Timer)
        {
            if (Event_Timer <= diff)
            {
                switch(EventPhase)
                {
                    case 0:
                        DoScriptText(-1000000, m_creature);//1
                        DoGate(DATA_ARENA4,1);
                        Start(false, false, false);
                        CanWalk = true;
                        Event_Timer = 0;
                        break;
                    case 1:
                        CanWalk = true;
                        Event_Timer = 0;
                        break;
                    case 2:
                        Event_Timer = 2000;
                        break;
                    case 3:
                        DoGate(DATA_ARENA1,0);
                        Event_Timer = 3000;
                        break;
                    case 4:
                        CanWalk = true;
                        m_creature->SetVisibility(VISIBILITY_OFF);
                        SummonRingMob();
                        Event_Timer = 8000;
                        break;
                    case 5:
                        SummonRingMob();
                        SummonRingMob();
                        Event_Timer = 8000;
                        break;
                    case 6:
                        SummonRingMob();
                        Event_Timer = 0;
                        break;
                    case 7:
                        m_creature->SetVisibility(VISIBILITY_ON);
                        DoGate(DATA_ARENA1,1);
                        DoScriptText(-1000000, m_creature);//4
                        CanWalk = true;
                        Event_Timer = 0;
                        break;
                    case 8:
                        DoGate(DATA_ARENA2,0);
                        Event_Timer = 5000;
                        break;
                    case 9:
                        m_creature->SetVisibility(VISIBILITY_OFF);
                        SummonRingBoss();
                        Event_Timer = 0;
                        break;
                    case 10:
                        //if quest, complete
                        DoGate(DATA_ARENA2,1);
                        DoGate(DATA_ARENA3,0);
                        DoGate(DATA_ARENA4,0);
                        CanWalk = true;
                        Event_Timer = 0;
                        break;
                }
                ++EventPhase;
            }else Event_Timer -= diff;
        }

        if (CanWalk)
            npc_escortAI::UpdateAI(diff);
    }
};

CreatureAI* GetAI_npc_grimstone(Creature *_Creature)
{
    npc_grimstoneAI* Grimstone_AI = new npc_grimstoneAI(_Creature);

    Grimstone_AI->FillPointMovementListForCreature();

    return (CreatureAI*)Grimstone_AI;
}

/*######
## mob_phalanx
######*/

#define SPELL_THUNDERCLAP       15588
#define SPELL_FIREBALLVOLLEY    15285
#define SPELL_MIGHTYBLOW        14099

struct MANGOS_DLL_DECL mob_phalanxAI : public ScriptedAI
{
    mob_phalanxAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 ThunderClap_Timer;
    uint32 FireballVolley_Timer;
    uint32 MightyBlow_Timer;

    void Reset()
    {
        ThunderClap_Timer = 12000;
        FireballVolley_Timer = 0;
        MightyBlow_Timer = 15000;
    }

    void Aggro(Unit *who)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //ThunderClap_Timer
        if (ThunderClap_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_THUNDERCLAP);
            ThunderClap_Timer = 10000;
        }else ThunderClap_Timer -= diff;

        //FireballVolley_Timer
        if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 51)
        {
            if (FireballVolley_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_FIREBALLVOLLEY);
                FireballVolley_Timer = 15000;
            }else FireballVolley_Timer -= diff;
        }

        //MightyBlow_Timer
        if (MightyBlow_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_MIGHTYBLOW);
            MightyBlow_Timer = 10000;
        }else MightyBlow_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_phalanx(Creature *_Creature)
{
    return new mob_phalanxAI (_Creature);
}

/*######
## npc_kharan_mighthammer
######*/

#define QUEST_4001      4001
#define QUEST_4342      4342

#define GOSSIP_ITEM_KHARAN_1    "I need to know where the princess are, Kharan!"
#define GOSSIP_ITEM_KHARAN_2    "All is not lost, Kharan!"

#define GOSSIP_ITEM_KHARAN_3    "Gor'shak is my friend, you can trust me."
#define GOSSIP_ITEM_KHARAN_4    "Not enough, you need to tell me more."
#define GOSSIP_ITEM_KHARAN_5    "So what happened?"
#define GOSSIP_ITEM_KHARAN_6    "Continue..."
#define GOSSIP_ITEM_KHARAN_7    "So you suspect that someone on the inside was involved? That they were tipped off?"
#define GOSSIP_ITEM_KHARAN_8    "Continue with your story please."
#define GOSSIP_ITEM_KHARAN_9    "Indeed."
#define GOSSIP_ITEM_KHARAN_10   "The door is open, Kharan. You are a free man."

bool GossipHello_npc_kharan_mighthammer(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu(_Creature->GetGUID());

    if (player->GetQuestStatus(QUEST_4001) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_KHARAN_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    if (player->GetQuestStatus(4342) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_KHARAN_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);

    if (player->GetTeam() == HORDE)
        player->SEND_GOSSIP_MENU(2473, _Creature->GetGUID());
    else
        player->SEND_GOSSIP_MENU(2474, _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_kharan_mighthammer(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_KHARAN_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            player->SEND_GOSSIP_MENU(2475, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_KHARAN_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            player->SEND_GOSSIP_MENU(2476, _Creature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF+3:
            player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_KHARAN_5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
            player->SEND_GOSSIP_MENU(2477, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_KHARAN_6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            player->SEND_GOSSIP_MENU(2478, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_KHARAN_7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+6);
            player->SEND_GOSSIP_MENU(2479, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+6:
            player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_KHARAN_8, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+7);
            player->SEND_GOSSIP_MENU(2480, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+7:
            player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_KHARAN_9, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+8);
            player->SEND_GOSSIP_MENU(2481, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+8:
            player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_KHARAN_10, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+9);
            player->SEND_GOSSIP_MENU(2482, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+9:
            player->CLOSE_GOSSIP_MENU();
            if (player->GetTeam() == HORDE)
                player->AreaExploredOrEventHappens(QUEST_4001);
            else
                player->AreaExploredOrEventHappens(QUEST_4342);
            break;
    }
    return true;
}

/*######
## npc_lokhtos_darkbargainer
######*/

#define ITEM_THRORIUM_BROTHERHOOD_CONTRACT               18628
#define ITEM_SULFURON_INGOT                              17203
#define QUEST_A_BINDING_CONTRACT                         7604
#define SPELL_CREATE_THORIUM_BROTHERHOOD_CONTRACT_DND    23059

#define GOSSIP_ITEM_SHOW_ACCESS     "Show me what I have access to, Lothos."
#define GOSSIP_ITEM_GET_CONTRACT    "Get Thorium Brotherhood Contract"

bool GossipHello_npc_lokhtos_darkbargainer(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu(_Creature->GetGUID());

    if (_Creature->isVendor() && player->GetReputationRank(59) >= REP_FRIENDLY)
        player->ADD_GOSSIP_ITEM( 1, GOSSIP_ITEM_SHOW_ACCESS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

    if (player->GetQuestRewardStatus(QUEST_A_BINDING_CONTRACT) != 1 &&
        !player->HasItemCount(ITEM_THRORIUM_BROTHERHOOD_CONTRACT, 1, true) &&
        player->HasItemCount(ITEM_SULFURON_INGOT, 1))
    {
        player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_GET_CONTRACT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    }

    if (player->GetReputationRank(59) < REP_FRIENDLY)
        player->SEND_GOSSIP_MENU(3673, _Creature->GetGUID());
    else
        player->SEND_GOSSIP_MENU(3677, _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_lokhtos_darkbargainer(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
        player->CLOSE_GOSSIP_MENU();
        player->CastSpell(player, SPELL_CREATE_THORIUM_BROTHERHOOD_CONTRACT_DND, false);
    }

    if (action == GOSSIP_ACTION_TRADE)
        player->SEND_VENDORLIST(_Creature->GetGUID());

    return true;
}

/*######
## npc_rocknot
######*/

#define SAY_GOT_BEER        -1230000
#define SPELL_DRUNKEN_RAGE  14872
#define QUEST_ALE           4295

struct MANGOS_DLL_DECL npc_rocknotAI : public npc_escortAI
{
    npc_rocknotAI(Creature *c) : npc_escortAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 BreakKeg_Timer;
    uint32 BreakDoor_Timer;

    void Reset()
    {
        if (IsBeingEscorted)
            return;

        BreakKeg_Timer = 0;
        BreakDoor_Timer = 0;
    }

    void Aggro(Unit *who) { }

    void DoGo(uint32 id, uint32 state)
    {
        if (GameObject *go = GameObject::GetGameObject(*m_creature,pInstance->GetData64(id)))
            go->SetGoState(state);
    }

    void WaypointReached(uint32 i)
    {
        if (!pInstance)
            return;

        switch(i)
        {
            case 1:
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_KICK);
                break;
            case 2:
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_ATTACKUNARMED);
                break;
            case 3:
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_ATTACKUNARMED);
                break;
            case 4:
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_KICK);
                break;
            case 5:
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_KICK);
                BreakKeg_Timer = 2000;
                break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!pInstance)
            return;

        if (BreakKeg_Timer)
        {
            if (BreakKeg_Timer <= diff)
            {
                DoGo(DATA_GO_BAR_KEG,0);
                BreakKeg_Timer = 0;
                BreakDoor_Timer = 1000;
            }else BreakKeg_Timer -= diff;
        }

        if (BreakDoor_Timer)
        {
            if (BreakDoor_Timer <= diff)
            {
                DoGo(DATA_GO_BAR_DOOR,2);
                DoGo(DATA_GO_BAR_KEG_TRAP,0);               //doesn't work very well, leaving code here for future
                                                            //spell by trap has effect61, this indicate the bar go hostile

                if (Unit *tmp = Unit::GetUnit(*m_creature,pInstance->GetData64(DATA_PHALANX)))
                    tmp->setFaction(14);

                //for later, this event(s) has alot more to it.
                //optionally, DONE can trigger bar to go hostile.
                pInstance->SetData(TYPE_BAR,DONE);

                BreakDoor_Timer = 0;
            }else BreakDoor_Timer -= diff;
        }

        npc_escortAI::UpdateAI(diff);
    }
};

CreatureAI* GetAI_npc_rocknot(Creature *_Creature)
{
    npc_rocknotAI* Rocknot_AI = new npc_rocknotAI(_Creature);

    Rocknot_AI->FillPointMovementListForCreature();

    return (CreatureAI*)Rocknot_AI;
}

bool ChooseReward_npc_rocknot(Player *player, Creature *_Creature, const Quest *_Quest, uint32 item)
{
    ScriptedInstance* pInstance = ((ScriptedInstance*)_Creature->GetInstanceData());

    if (!pInstance)
        return true;

    if (pInstance->GetData(TYPE_BAR) == DONE || pInstance->GetData(TYPE_BAR) == SPECIAL)
        return true;

    if (_Quest->GetQuestId() == QUEST_ALE)
    {
        if (pInstance->GetData(TYPE_BAR) != IN_PROGRESS)
            pInstance->SetData(TYPE_BAR,IN_PROGRESS);

        pInstance->SetData(TYPE_BAR,SPECIAL);

        //keep track of amount in instance script, returns SPECIAL if amount ok and event in progress
        if (pInstance->GetData(TYPE_BAR) == SPECIAL)
        {
            DoScriptText(SAY_GOT_BEER, _Creature);
            _Creature->CastSpell(_Creature,SPELL_DRUNKEN_RAGE,false);
            ((npc_escortAI*)(_Creature->AI()))->Start(false, false, false);
        }
    }

    return true;
}

void AddSC_blackrock_depths()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "at_ring_of_law";
    newscript->pAreaTrigger = &AreaTrigger_at_ring_of_law;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_grimstone";
    newscript->GetAI = &GetAI_npc_grimstone;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_phalanx";
    newscript->GetAI = &GetAI_mob_phalanx;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_kharan_mighthammer";
    newscript->pGossipHello =  &GossipHello_npc_kharan_mighthammer;
    newscript->pGossipSelect = &GossipSelect_npc_kharan_mighthammer;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_lokhtos_darkbargainer";
    newscript->pGossipHello =  &GossipHello_npc_lokhtos_darkbargainer;
    newscript->pGossipSelect = &GossipSelect_npc_lokhtos_darkbargainer;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_rocknot";
    newscript->GetAI = &GetAI_npc_rocknot;
    newscript->pChooseReward = &ChooseReward_npc_rocknot;
    newscript->RegisterSelf();
}
