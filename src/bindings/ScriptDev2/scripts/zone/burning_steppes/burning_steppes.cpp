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
SDName: Burning_Steppes
SD%Complete: 100
SDComment: Quest support: 4224, 4866, 4726
SDCategory: Burning Steppes
EndScriptData */

/* ContentData
npc_ragged_john
npc_broodling
EndContentData */

#include "precompiled.h"

/*######
## npc_ragged_john
######*/

struct MANGOS_DLL_DECL npc_ragged_johnAI : public ScriptedAI
{
    npc_ragged_johnAI(Creature *c) : ScriptedAI(c) { Reset(); }

    void Reset() {}

    void MoveInLineOfSight(Unit *who)
    {
        if( who->HasAura(16468,0) )
        {
            if( who->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(who, 15) && who->isInAccessablePlaceFor(m_creature) )
            {
                DoCast(who,16472);
                ((Player*)who)->AreaExploredOrEventHappens(4866);
            }
        }

        if( !m_creature->getVictim() && who->isTargetableForAttack() && ( m_creature->IsHostileTo( who )) && who->isInAccessablePlaceFor(m_creature) )
        {
            if (!m_creature->canFly() && m_creature->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
                return;

            float attackRadius = m_creature->GetAttackDistance(who);
            if( m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->IsWithinLOSInMap(who) )
            {
                who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                AttackStart(who);
            }
        }
    }

    void Aggro(Unit *who) {}
};

CreatureAI* GetAI_npc_ragged_john(Creature *_Creature)
{
    return new npc_ragged_johnAI (_Creature);
}

bool GossipHello_npc_ragged_john(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if (player->GetQuestStatus(4224) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM( 0, "Official buisness, John. I need some information about Marsha Windsor. Tell me about the last time you saw him.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    player->SEND_GOSSIP_MENU(2713, _Creature->GetGUID());
    return true;
}

bool GossipSelect_npc_ragged_john(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF:
            player->ADD_GOSSIP_ITEM( 0, "So what did you do?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(2714, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            player->ADD_GOSSIP_ITEM( 0, "Start making sense, dwarf. I don't want to have anything to do with your cracker, your pappy, or any sort of 'discreditin'.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->SEND_GOSSIP_MENU(2715, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            player->ADD_GOSSIP_ITEM( 0, "Ironfoe?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->SEND_GOSSIP_MENU(2716, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            player->ADD_GOSSIP_ITEM( 0, "Interesting... continue John.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->SEND_GOSSIP_MENU(2717, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            player->ADD_GOSSIP_ITEM( 0, "So that's how Windsor died...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->SEND_GOSSIP_MENU(2718, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            player->ADD_GOSSIP_ITEM( 0, "So how did he die?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->SEND_GOSSIP_MENU(2719, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+6:
            player->ADD_GOSSIP_ITEM( 0, "Ok so where the hell is he? Wait a minute! Are you drunk?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->SEND_GOSSIP_MENU(2720, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+7:
            player->ADD_GOSSIP_ITEM( 0, "WHY is he in Blackrock Depths?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
            player->SEND_GOSSIP_MENU(2721, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+8:
            player->ADD_GOSSIP_ITEM( 0, "300? So the Dark Irons killed him and dragged him into the Depths?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
            player->SEND_GOSSIP_MENU(2722, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+9:
            player->ADD_GOSSIP_ITEM( 0, "Ahh... Ironfoe", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
            player->SEND_GOSSIP_MENU(2723, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+10:
            player->ADD_GOSSIP_ITEM( 0, "Thanks, Ragged John. Your story was very uplifting and informative", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
            player->SEND_GOSSIP_MENU(2725, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+11:
            player->CLOSE_GOSSIP_MENU();
            player->AreaExploredOrEventHappens(4224);
            break;
    }
    return true;
}
/*######
## npc_broodling
######*/

#define BROODLING_ESSENCE_QUEST          4726
#define DRACO_INCARCINATRIX_900_SPELL    16007
#define CREATE_BROODLING_ESSENCE_SPELL   16027

bool mDracoCasted;

struct MANGOS_DLL_DECL npc_broodlingAI : public ScriptedAI
{
    npc_broodlingAI(Creature* c) : ScriptedAI(c)
    {
        Reset();
    }
    void Reset() 
    {
        mDracoCasted = false;
    }
    void Aggro(Unit* mVictim) {}
    void SpellHit(Unit* mCaster, const SpellEntry* mSpell)
    {
        if(mSpell->Id == DRACO_INCARCINATRIX_900_SPELL)
        {
            mDracoCasted = true;
        }
    }
    void JustDied(Unit* mKiller)
    {
        if(((Player*)mKiller)->IsActiveQuest(BROODLING_ESSENCE_QUEST) && mDracoCasted)
            m_creature->CastSpell(m_creature, CREATE_BROODLING_ESSENCE_SPELL, true);
    }
};

CreatureAI* GetAI_npc_broodling(Creature* _Creature)
{
    return new npc_broodlingAI(_Creature);
}

void AddSC_burning_steppes()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_ragged_john";
    newscript->GetAI = &GetAI_npc_ragged_john;
    newscript->pGossipHello =  &GossipHello_npc_ragged_john;
    newscript->pGossipSelect = &GossipSelect_npc_ragged_john;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_broodling";
    newscript->GetAI = &GetAI_npc_broodling;
    newscript->RegisterSelf();
}
