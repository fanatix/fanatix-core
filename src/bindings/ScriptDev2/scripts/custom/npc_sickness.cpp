#include "precompiled.h"

bool GossipHello_npc_sickness(Player *player, Creature *_Creature)
{

if ( player->GetTeam() == ALLIANCE ) {

player->ADD_GOSSIP_ITEM( 4, "Entferne Wiederbelungsnachwirkung", GOSSIP_SENDER_MAIN, 1214);
player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());

    }  else {

player->ADD_GOSSIP_ITEM( 4, "Entferne Wiederbelungsnachwirkung", GOSSIP_SENDER_MAIN, 1215);
player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());

    }

return true;

}

void SendDefaultMenu_npc_sickness(Player *player, Creature *_Creature, uint32 action)


{


   if(!player->getAttackers().empty())
   {
    _Creature->MonsterSay("Du befindest dich in einem Kampf", LANG_UNIVERSAL, NULL);
   return;
   }

   if ( (player->isDead()) ) // Is player Dead

   {
      _Creature->MonsterSay("Du bist Tot", LANG_UNIVERSAL, NULL);
      return;
   }

   if ( (player->isInFlight()) || player->IsInWater() || player->GetInstanceId() ) //Is player in flight/water
   {
      _Creature->MonsterSay("Das kannst du nicht", LANG_UNIVERSAL, NULL);
      return;
   }

   if ( (player->IsMounted()) ) //Is player mounted
   {
      _Creature->MonsterSay("Du musst dich vorher dismounten", LANG_UNIVERSAL, NULL);
      return;
   }

     player->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);


switch(action)
{

case 1214://Remove Res Sickness
    if(!player->HasAura(SPELL_ID_PASSIVE_RESURRECTION_SICKNESS,0)) {
       _Creature->MonsterWhisper("Du hast keine Wiederbelebungsnachwirkung", LANG_UNIVERSAL, NULL);
        player->CLOSE_GOSSIP_MENU();
        return;
    }

    player->RemoveAurasDueToSpell(SPELL_ID_PASSIVE_RESURRECTION_SICKNESS);
    player->TeleportTo(0, -6.595490f, -324.676941f, 131.170364f, 4.164785f);
    player->CLOSE_GOSSIP_MENU();
break;

case 1215://Remove Res Sickness
    if(!player->HasAura(15007,0)) {
        _Creature->MonsterWhisper("Du hast keine Wiederbelebungsnachwirkung", LANG_UNIVERSAL, NULL);
        player->CLOSE_GOSSIP_MENU();
        return;
    }

    player->RemoveAurasDueToSpell(15007);
    player->TeleportTo(0, 383.854584f, -997.048035f, 110.134575f, 2.596171f);
    player->CLOSE_GOSSIP_MENU();
break;


}
 }

bool GossipSelect_npc_sickness(Player *player, Creature *_Creature, uint32 sender, uint32 action )

{
// Main menu
if (sender == GOSSIP_SENDER_MAIN)
SendDefaultMenu_npc_sickness(player, _Creature, action   );

return true;
}
void AddSC_npc_sickness()

{

Script *newscript;

newscript = new Script;
newscript->Name="npc_sickness";
newscript->pGossipHello = &GossipHello_npc_sickness;
newscript->pGossipSelect = &GossipSelect_npc_sickness;

newscript->RegisterSelf();

}