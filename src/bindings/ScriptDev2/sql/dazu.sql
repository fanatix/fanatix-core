#creature
delete from locales_creature where name_loc3='';
update creature_template, locales_creature set creature_template.name=locales_creature.name_loc3 where creature_template.entry=locales_creature.entry;
update creature_template, locales_creature set creature_template.subname=locales_creature.subname_loc3 where creature_template.entry=locales_creature.entry;

#gameobject
delete from locales_gameobject where name_loc3='';
update gameobject_template, locales_gameobject set gameobject_template.name=locales_gameobject.name_loc3 where gameobject_template.entry=locales_gameobject.entry;

#item
delete from locales_item where name_loc3='';
update item_template, locales_item set item_template.name=locales_item.name_loc3 where item_template.entry=locales_item.entry;
update item_template, locales_item set item_template.description=locales_item.description_loc3 where item_template.entry=locales_item.entry;

#npc option
delete from locales_npc_option where option_text_loc3='';
update npc_option, locales_npc_option set npc_option.option_text=locales_npc_option.option_text_loc3 where npc_option.id=locales_npc_option.entry;

#npc text
delete from locales_npc_text where text0_0_loc3='';
update npc_text, locales_npc_text set npc_text.text0_1=locales_npc_text.text0_1_loc3 where npc_text.ID=locales_npc_text.entry;
update npc_text, locales_npc_text set npc_text.text0_0=locales_npc_text.text0_0_loc3 where npc_text.ID=locales_npc_text.entry;

#page text
delete from locales_page_text where text_loc3='';
update page_text, locales_page_text set page_text.text=locales_page_text.text_loc3 where page_text.entry=locales_page_text.entry;

#quest
delete from locales_quest where Details_loc3='';
delete from locales_quest where Title_loc3='';
update quest_template, locales_quest set quest_template.Title=locales_quest.Title_loc3 where quest_template.entry=locales_quest.entry;
update quest_template, locales_quest set quest_template.Details=locales_quest.Details_loc3 where quest_template.entry=locales_quest.entry;
update quest_template, locales_quest set quest_template.Objectives=locales_quest.Objectives_loc3 where quest_template.entry=locales_quest.entry;
update quest_template, locales_quest set quest_template.OfferRewardText=locales_quest.OfferRewardText_loc3 where quest_template.entry=locales_quest.entry;
update quest_template, locales_quest set quest_template.RequestItemsText=locales_quest.RequestItemsText_loc3 where quest_template.entry=locales_quest.entry;
update quest_template, locales_quest set quest_template.EndText=locales_quest.EndText_loc3 where quest_template.entry=locales_quest.entry;
update quest_template, locales_quest set quest_template.ObjectiveText1=locales_quest.ObjectiveText1_loc3 where quest_template.entry=locales_quest.entry;
update quest_template, locales_quest set quest_template.ObjectiveText2=locales_quest.ObjectiveText2_loc3 where quest_template.entry=locales_quest.entry;
update quest_template, locales_quest set quest_template.ObjectiveText3=locales_quest.ObjectiveText3_loc3 where quest_template.entry=locales_quest.entry;
update quest_template, locales_quest set quest_template.ObjectiveText4=locales_quest.ObjectiveText4_loc3 where quest_template.entry=locales_quest.entry;
