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
SDName: The_Barrens
SD%Complete: 92
SDComment: Quest support: 863, 1719, 2458, 4921, 6981
SDCategory: Barrens
EndScriptData */

/* ContentData
npc_beaten_corpse
npc_sputtervalve
npc_taskmaster_fizzule
npc_twiggy_flathead
at_twiggy_flathead
npc_wizzlecrank_shredder
EndContentData */

#include "precompiled.h"
#include "../../npc/npc_escortAI.h"

/*######
## npc_beaten_corpse
######*/

bool GossipHello_npc_beaten_corpse(Player *player, Creature *_Creature)
{
    if( player->GetQuestStatus(4921) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(4921) == QUEST_STATUS_COMPLETE)
        player->ADD_GOSSIP_ITEM(0,"Examine corpse in detail...",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);

    player->SEND_GOSSIP_MENU(3557,_Creature->GetGUID());
    return true;
}

bool GossipSelect_npc_beaten_corpse(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if(action == GOSSIP_ACTION_INFO_DEF +1)
    {
        player->SEND_GOSSIP_MENU(3558,_Creature->GetGUID());
        player->KilledMonster( 10668,_Creature->GetGUID() );
    }
    return true;
}

/*######
## npc_sputtervalve
######*/

bool GossipHello_npc_sputtervalve(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if( player->GetQuestStatus(6981) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM(0,"Can you tell me about this shard?",GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF);

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(),_Creature->GetGUID());
    return true;
}

bool GossipSelect_npc_sputtervalve(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if(action == GOSSIP_ACTION_INFO_DEF)
    {
        player->SEND_GOSSIP_MENU(2013,_Creature->GetGUID());
        player->AreaExploredOrEventHappens(6981);
    }
    return true;
}

/*######
## npc_taskmaster_fizzule
######*/

enum
{
    FACTION_FRIENDLY_F  = 35,
    SPELL_FLARE         = 10113,
    SPELL_FOLLY         = 10137,
};

struct MANGOS_DLL_DECL npc_taskmaster_fizzuleAI : public ScriptedAI
{
    npc_taskmaster_fizzuleAI(Creature* c) : ScriptedAI(c)
    {
        factionNorm = c->getFaction();
        Reset();
    }

    uint32 factionNorm;
    bool IsFriend;
    uint32 Reset_Timer;
    uint8 FlareCount;

    void Reset()
    {
        IsFriend = false;
        Reset_Timer = 120000;
        FlareCount = 0;
        m_creature->setFaction(factionNorm);
    }

    void DoFriend()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();

        m_creature->StopMoving();
        m_creature->GetMotionMaster()->MoveIdle();

        m_creature->setFaction(FACTION_FRIENDLY_F);
        m_creature->HandleEmoteCommand(EMOTE_ONESHOT_SALUTE);
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if (spell->Id == SPELL_FLARE || spell->Id == SPELL_FOLLY)
        {
            ++FlareCount;

            if (FlareCount >= 2)
                IsFriend = true;
        }
    }

    void Aggro(Unit* who) { }

    void UpdateAI(const uint32 diff)
    {
        if (IsFriend)
        {
            if (Reset_Timer < diff)
            {
                EnterEvadeMode();
            } else Reset_Timer -= diff;
        }

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_taskmaster_fizzule(Creature* pCreature)
{
    return new npc_taskmaster_fizzuleAI(pCreature);
}

bool ReciveEmote_npc_taskmaster_fizzule(Player* pPlayer, Creature* pCreature, uint32 emote)
{
    if (emote == TEXTEMOTE_SALUTE)
    {
        if (((npc_taskmaster_fizzuleAI*)pCreature->AI())->FlareCount >= 2)
        {
            if (pCreature->getFaction() == FACTION_FRIENDLY_F)
                return true;
            else
                ((npc_taskmaster_fizzuleAI*)pCreature->AI())->DoFriend();
        }
    }
    return true;
}

/*#####
## npc_twiggy_flathead
#####*/

#define SAY_BIG_WILL_READY                  -1000123
#define SAY_TWIGGY_BEGIN                    -1000124
#define SAY_TWIGGY_FRAY                     -1000125
#define SAY_TWIGGY_DOWN                     -1000126
#define SAY_TWIGGY_OVER                     -1000127

#define C_TWIGGY                            6248
#define C_BIG_WILL                          6238
#define C_AFFRAY_CHALLENGER                 6240
#define QUEST_AFFRAY                        1719

float AffrayChallengerLoc[6][4]=
{
    {-1683, -4326, 2.79, 0},
    {-1682, -4329, 2.79, 0},
    {-1683, -4330, 2.79, 0},
    {-1680, -4334, 2.79, 1.49},
    {-1674, -4326, 2.79, 3.49},
    {-1677, -4334, 2.79, 1.66}
};

struct MANGOS_DLL_DECL npc_twiggy_flatheadAI : public ScriptedAI
{
    npc_twiggy_flatheadAI(Creature *c) : ScriptedAI(c) {Reset();}

    bool EventInProgress;

    uint32 Event_Timer;
    uint32 Step;
    uint32 Challenger_Count;
    uint32 ChallengerDeath_Timer;

    uint64 PlayerGUID;
    uint64 BigWillGUID;
    uint64 AffrayChallenger[6];

    void Reset()
    {
        EventInProgress = false;

        Event_Timer = 2000;
        Step = 0;
        Challenger_Count = 0;
        ChallengerDeath_Timer = 0;

        PlayerGUID = 0;
        BigWillGUID = 0;

        for(uint8 i = 0; i < 6; i++)
            AffrayChallenger[i] = 0;
    }

    void Aggro(Unit *who) { }

    bool CanStartEvent(Player *player)
    {
        if (!EventInProgress)
        {
            EventInProgress = true;
            PlayerGUID = player->GetGUID();
            DoScriptText(SAY_TWIGGY_BEGIN, m_creature, player);
            return true;
        }

        debug_log("SD2: npc_twiggy_flathead event already in progress, need to wait.");
        return false;
    }

    void SetChallengers()
    {
        for(uint8 i = 0; i < 6; i++)
        {
            Creature* pCreature = m_creature->SummonCreature(C_AFFRAY_CHALLENGER, AffrayChallengerLoc[i][0], AffrayChallengerLoc[i][1], AffrayChallengerLoc[i][2], AffrayChallengerLoc[i][3], TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000);
            if (!pCreature)
            {
                debug_log("SD2: npc_twiggy_flathead event cannot summon challenger as expected.");
                continue;
            }

            pCreature->setFaction(35);
            pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            pCreature->HandleEmoteCommand(EMOTE_ONESHOT_ROAR);
            AffrayChallenger[i] = pCreature->GetGUID();
        }
    }

    void SetChallengerReady(Unit *pUnit)
    {
        pUnit->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pUnit->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pUnit->HandleEmoteCommand(EMOTE_ONESHOT_ROAR);
        pUnit->setFaction(14);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!EventInProgress)
            return;

        if (ChallengerDeath_Timer)
        {
            if (ChallengerDeath_Timer < diff)
            {
                for(uint8 i = 0; i < 6; i++)
                {
                    Creature *challenger = (Creature*)Unit::GetUnit(*m_creature,AffrayChallenger[i]);
                    if (challenger && !challenger->isAlive() && challenger->isDead())
                    {
                        DoScriptText(SAY_TWIGGY_DOWN, m_creature);
                        challenger->RemoveCorpse();
                        AffrayChallenger[i] = 0;
                        continue;
                    }
                }
                ChallengerDeath_Timer = 2500;
            } else ChallengerDeath_Timer -= diff;
        }

        if (Event_Timer < diff)
        {
            Player *player = (Player*)Unit::GetUnit(*m_creature,PlayerGUID);
            if (!player || player->isDead())
                Reset();

            switch(Step)
            {
                case 0:
                    SetChallengers();
                    ChallengerDeath_Timer = 2500;
                    Event_Timer = 5000;
                    ++Step;
                    break;
                case 1:
                    DoScriptText(SAY_TWIGGY_FRAY, m_creature);
                    if (Unit *challenger = Unit::GetUnit(*m_creature,AffrayChallenger[Challenger_Count]))
                        SetChallengerReady(challenger);
                    else Reset();
                    ++Challenger_Count;
                    Event_Timer = 25000;
                    if (Challenger_Count == 6)
                        ++Step;
                    break;
                case 2:
                    if (Unit *temp = m_creature->SummonCreature(C_BIG_WILL,-1713.79,-4342.09,6.05,6.15,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,300000))
                    {
                        BigWillGUID = temp->GetGUID();
                        temp->setFaction(35);
                        temp->GetMotionMaster()->MovePoint(0,-1682.31,-4329.68,2.78);
                    }
                    Event_Timer = 15000;
                    ++Step;
                    break;
                case 3:
                    if (Unit *will = Unit::GetUnit(*m_creature,BigWillGUID))
                    {
                        will->setFaction(32);
                        DoScriptText(SAY_BIG_WILL_READY, will, player);
                    }
                    Event_Timer = 5000;
                    ++Step;
                    break;
                case 4:
                    Unit *will = Unit::GetUnit(*m_creature,BigWillGUID);
                    if (will && will->isDead())
                    {
                        DoScriptText(SAY_TWIGGY_OVER, m_creature);
                        Reset();
                    } else if (!will)
                        Reset();
                    Event_Timer = 5000;
                    break;
            }
        } else Event_Timer -= diff;
    }
};

CreatureAI* GetAI_npc_twiggy_flathead(Creature *_Creature)
{
    return new npc_twiggy_flatheadAI (_Creature);
}

Creature* SelectCreatureInGrid(Unit *pUnit, uint32 entry, float range)
{
    Creature* pCreature = NULL;

    CellPair pair(MaNGOS::ComputeCellPair(pUnit->GetPositionX(), pUnit->GetPositionY()));
    Cell cell(pair);
    cell.data.Part.reserved = ALL_DISTRICT;
    cell.SetNoCreate();

    MaNGOS::NearestCreatureEntryWithLiveStateInObjectRangeCheck creature_check(*pUnit, entry, true, range);
    MaNGOS::CreatureLastSearcher<MaNGOS::NearestCreatureEntryWithLiveStateInObjectRangeCheck> searcher(pUnit, pCreature, creature_check);

    TypeContainerVisitor<MaNGOS::CreatureLastSearcher<MaNGOS::NearestCreatureEntryWithLiveStateInObjectRangeCheck>, GridTypeMapContainer> creature_searcher(searcher);

    CellLock<GridReadGuard> cell_lock(cell, pair);
    cell_lock->Visit(cell_lock, creature_searcher,*(pUnit->GetMap()));

    return pCreature;
}

bool AreaTrigger_at_twiggy_flathead(Player *player, AreaTriggerEntry *at)
{
    if (!player->isDead() && player->GetQuestStatus(QUEST_AFFRAY) == QUEST_STATUS_INCOMPLETE)
    {
        if (uint16 slot = player->FindQuestSlot(QUEST_AFFRAY))
        {
            //we don't want player to start event if failed already.
            if (player->GetQuestSlotState(slot) == QUEST_STATE_FAIL)
                return true;
        }

        Creature* Twiggy = SelectCreatureInGrid(player, C_TWIGGY, 30);

        if (!Twiggy)
            return true;

        if (((npc_twiggy_flatheadAI*)Twiggy->AI())->CanStartEvent(player))
            return false;                                   //ok to let mangos process further
        else
            return true;
    }
    return true;
}
/*#####
## npc_wizzlecrank_shredder
#####*/
#define SAY_PROGRESS_1  -1000272
#define SAY_PROGRESS_2  -1000273
#define SAY_PROGRESS_3  -1000274

#define SAY_MERCENARY_4 -1000275

#define SAY_PROGRESS_5  -1000276
#define SAY_PROGRESS_6  -1000277
#define SAY_PROGRESS_7  -1000278
#define SAY_PROGRESS_8  -1000279

#define QUEST_ESCAPE    863
#define NPC_PILOT       3451
#define MOB_MERCENARY   3282

struct MANGOS_DLL_DECL npc_wizzlecrank_shredderAI : public npc_escortAI
{
    npc_wizzlecrank_shredderAI(Creature* c) : npc_escortAI(c) {Reset();}

    bool Completed;

    void WaypointReached(uint32 i)
    {
        Unit* player = Unit::GetUnit((*m_creature), PlayerGUID);

        if(!player)
            return;

        switch(i)
        {
        case 0: DoScriptText(SAY_PROGRESS_1, m_creature);
            m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE); break;
        case 1: DoScriptText(SAY_PROGRESS_2, m_creature); break;
        case 10: DoScriptText(SAY_PROGRESS_3, m_creature, player);
            m_creature->AddUnitMovementFlag(MOVEMENTFLAG_WALK_MODE); break;
        case 20:{
            Unit* Mercenary = SelectCreatureInGrid(m_creature, MOB_MERCENARY, 99);
            if(Mercenary)
            {
                DoScriptText(SAY_MERCENARY_4, Mercenary);
                ((Creature*)Mercenary)->AI()->AttackStart(m_creature);
                AttackStart(Mercenary);
            }
                }break;
        case 21: DoScriptText(SAY_PROGRESS_5, m_creature);
            m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE); break;
        case 28: DoScriptText(SAY_PROGRESS_6, m_creature); break;
        case 29: DoScriptText(SAY_PROGRESS_7, m_creature); break;
        case 30: DoScriptText(SAY_PROGRESS_8, m_creature); break;
        case 31: m_creature->SummonCreature(NPC_PILOT, 1088.77, -2985.39, 91.84, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 300000);
            m_creature->setDeathState(JUST_DIED);
            Completed = true;
            if (player && player->GetTypeId() == TYPEID_PLAYER)
                    ((Player*)player)->GroupEventHappens(QUEST_ESCAPE, m_creature);
            break;
        }
    }

    void Reset()
    {
        m_creature->setDeathState(ALIVE);
        Completed = false;
        m_creature->setFaction(69);
    }

    void Aggro(Unit* who){}

    void JustDied(Unit* killer)
    {
        if (PlayerGUID && !Completed)
        {
            Unit* player = Unit::GetUnit((*m_creature), PlayerGUID);
            if (player)
                ((Player*)player)->FailQuest(QUEST_ESCAPE);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        npc_escortAI::UpdateAI(diff);
    }
};

bool QuestAccept_npc_wizzlecrank_shredder(Player* player, Creature* creature, Quest const* quest)
{
    if (quest->GetQuestId() == QUEST_ESCAPE)
    {
        ((npc_escortAI*)(creature->AI()))->Start(true, true, false, player->GetGUID());
        creature->setFaction(113);
    }
    return true;
}

CreatureAI* GetAI_npc_wizzlecrank_shredderAI(Creature *_Creature)
{
    npc_wizzlecrank_shredderAI* thisAI = new npc_wizzlecrank_shredderAI(_Creature);

    thisAI->AddWaypoint(0, 1109.15, -3104.11, 82.41, 6000);
    thisAI->AddWaypoint(1, 1105.39, -3102.86, 82.74, 2000);
    thisAI->AddWaypoint(2, 1104.97, -3108.52, 83.10, 1000);
    thisAI->AddWaypoint(3, 1110.01, -3110.48, 82.81, 1000);
    thisAI->AddWaypoint(4, 1111.72, -3103.03, 82.21, 1000);
    thisAI->AddWaypoint(5, 1106.98, -3099.44, 82.18, 1000);
    thisAI->AddWaypoint(6, 1103.74, -3103.29, 83.05, 1000);
    thisAI->AddWaypoint(7, 1112.55, -3106.56, 82.31, 1000);
    thisAI->AddWaypoint(8, 1108.12, -3111.04, 82.99, 1000);
    thisAI->AddWaypoint(9, 1109.32, -3100.39, 82.08, 1000);
    thisAI->AddWaypoint(10, 1109.32, -3100.39, 82.08, 6000);
    thisAI->AddWaypoint(11, 1098.92, -3095.14, 82.97);
    thisAI->AddWaypoint(12, 1100.94, -3082.60, 82.83);
    thisAI->AddWaypoint(13, 1101.12, -3068.83, 82.53);
    thisAI->AddWaypoint(14, 1096.97, -3051.99, 82.50);
    thisAI->AddWaypoint(15, 1094.06, -3036.79, 82.70);
    thisAI->AddWaypoint(16, 1098.22, -3027.84, 83.79);
    thisAI->AddWaypoint(17, 1109.51, -3015.92, 85.73);
    thisAI->AddWaypoint(18, 1119.87, -3007.21, 87.08);
    thisAI->AddWaypoint(19, 1130.23, -3002.49, 91.27, 5000);
    thisAI->AddWaypoint(20, 1130.23, -3002.49, 91.27, 3000);
    thisAI->AddWaypoint(21, 1130.23, -3002.49, 91.27, 4000);
    thisAI->AddWaypoint(22, 1129.73, -2985.89, 92.46);
    thisAI->AddWaypoint(23, 1124.10, -2983.29, 92.81);
    thisAI->AddWaypoint(24, 1111.74, -2992.38, 91.59);
    thisAI->AddWaypoint(25, 1111.06, -2976.54, 91.81);
    thisAI->AddWaypoint(26, 1099.91, -2991.17, 91.67);
    thisAI->AddWaypoint(27, 1096.32, -2981.55, 91.73);
    thisAI->AddWaypoint(28, 1091.28, -2985.82, 91.74, 4000);
    thisAI->AddWaypoint(29, 1091.28, -2985.82, 91.74, 3000);
    thisAI->AddWaypoint(30, 1091.28, -2985.82, 91.74, 7000);
    thisAI->AddWaypoint(31, 1091.28, -2985.82, 91.74, 3000);

    return (CreatureAI*)thisAI;
}

void AddSC_the_barrens()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_beaten_corpse";
    newscript->pGossipHello = &GossipHello_npc_beaten_corpse;
    newscript->pGossipSelect = &GossipSelect_npc_beaten_corpse;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_sputtervalve";
    newscript->pGossipHello = &GossipHello_npc_sputtervalve;
    newscript->pGossipSelect = &GossipSelect_npc_sputtervalve;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_taskmaster_fizzule";
    newscript->GetAI = &GetAI_npc_taskmaster_fizzule;
    newscript->pReceiveEmote = &ReciveEmote_npc_taskmaster_fizzule;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_twiggy_flathead";
    newscript->GetAI = &GetAI_npc_twiggy_flathead;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "at_twiggy_flathead";
    newscript->pAreaTrigger = &AreaTrigger_at_twiggy_flathead;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_wizzlecrank_shredder";
    newscript->GetAI = &GetAI_npc_wizzlecrank_shredderAI;
    newscript->pQuestAccept = &QuestAccept_npc_wizzlecrank_shredder;
    newscript->RegisterSelf();
}
