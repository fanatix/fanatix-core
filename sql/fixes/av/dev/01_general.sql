REPLACE INTO gameobject_template VALUES (178925,1,5651,"Alliance Banner","","",83,0,1,0,1479,196608,180421,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (178943,1,5652,"Horde Banner","","",84,0,1,0,1479,196608,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (178940,1,5653,"Contested Banner","","",83,0,1,0,1479,196608,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (179435,1,5654,"Contested Banner","","",84,0,1,0,1479,196608,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"");

REPLACE INTO gameobject_template VALUES (178365,1,5771,"Alliance Banner","","",83,0,1,0,1479,196608,180100,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (179286,1,5772,"Contested Banner","","",83,0,1,0,1479,196608,180102,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (179287,1,5774,"Contested Banner","","",84,0,1,0,1479,0,180102,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (180418,1,6211,"Snowfall Banner","","",0,0,1,0,1479,196608,180100,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"");

REPLACE INTO gameobject_template VALUES (180424,0,3751,"Alterac Valley Gate","","",100,0,3.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (180100,6,2232,"Alliance Banner Aura","","",0,0,2,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (180102,6,266,"Neutral Banner Aura","","",0,0,2,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (180101,6,1311,"Horde Banner Aura","","",0,0,2,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (180421,6,2232,"Alliance Banner Aura, Large","","",0,0,5,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (180423,6,266,"Neutral Banner Aura, Large","","",0,0,5,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (180422,6,1311,"Horde Banner Aura, Large","","",0,0,5,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,"");

-- removes the damage from the fire-gos (used for destroyed towers) - but this is not blizzlike TODO remove this hack
UPDATE `gameobject_template` SET `type` = '5',`data0` = '0',`data1` = '0',`data2` = '0',`data3` = '1',`data5` = '0',`data10` = '0' WHERE `gameobject_template`.`entry` =179065 LIMIT 1 ;

-- bowman apply aura entangling root (so they can't move)
-- INSERT INTO `creature_template_addon` (`entry`, `auras`) VALUES ('13358', '42716 0 42716 1');
-- INSERT INTO `creature_template_addon` (`entry`, `auras`) VALUES ('13359', '42716 0 42716 1');
DELETE FROM `creature_template_addon` WHERE `entry` IN(13358,13359);
INSERT INTO `creature_template_addon` (`entry`, `auras`) VALUES ('13358', '42716 0');
INSERT INTO `creature_template_addon` (`entry`, `auras`) VALUES ('13359', '42716 0');

-- delete auras from marshalls around vanndar - somehow it's possible, that
-- normal player get buffed by them and walk around with 30k health
DELETE FROM `creature_template_addon` WHERE `entry` IN(14762,14763,14764,14765);

-- following is the player loot.. it takes the id 1 - be sure that i don't delete anything
REPLACE INTO `creature_loot_template` (`entry` ,`item` ,`ChanceOrQuestChance` ,`groupid` ,`mincountOrRef` ,`maxcount` ,`lootcondition` ,`condition_value1` ,`condition_value2`)
VALUES ('1', '17306', '33', '0', '5', '15',  '6', '67', '0'),
('1', '17422', '100', '0', '20', '40', '0', '0', '0'),
('1', '17423', '33', '0', '5', '15',  '6', '469', '0'),
( '1', '17502', '30', '1', '5', '10', '6', '469', '0' ),
( '1', '17503', '30', '1', '5', '10', '6', '469', '0'),
( '1', '17504', '30', '1', '5', '10', '6', '469', '0' ),
( '1', '17326', '30', '2', '5', '10', '6', '67', '0' ),
( '1', '17327', '30', '2', '5', '10', '6', '67', '0' ),
( '1', '17328', '30', '2', '5', '10', '6', '67', '0' );

-- blizzlike mobs in the mines don't have any loot (not even gold, it doesn't show the looticon if i go with my mouse over them)
-- broken after the strange new loot-table appeared (but not important)  delete from creature_loot_template where entry in (13396,13080,13098,13078,13397,13099,13081,13079,11603,11604,11605,11677,10982,13317,13096,13087,13086,13316,13097,13089,13088);
-- TODO: write own loottable

-- fixed the reputation requirement for the av-trinket-quest
-- please look at this critical i just looked at wowhead-comments and i'm not
-- realy sure
UPDATE quest_template SET RequiredMinRepFaction=730, RequiredMinRepValue=42980 WHERE entry=7172;
UPDATE quest_template SET RequiredMinRepFaction=730, RequiredMinRepValue=42000 WHERE entry=7171;
UPDATE quest_template SET RequiredMinRepFaction=730, RequiredMinRepValue=21000 WHERE entry=7170;
UPDATE quest_template SET RequiredMinRepFaction=730, RequiredMinRepValue=9000  WHERE entry=7169;
UPDATE quest_template SET RequiredMinRepFaction=730, RequiredMinRepValue=3000  WHERE entry=7168;


UPDATE quest_template SET RequiredMinRepFaction=729, RequiredMinRepValue=42980  WHERE entry=7167;
UPDATE quest_template SET RequiredMinRepFaction=729, RequiredMinRepValue=42000  WHERE entry=7166;
UPDATE quest_template SET RequiredMinRepFaction=729, RequiredMinRepValue=21000  WHERE entry=7165;
UPDATE quest_template SET RequiredMinRepFaction=729, RequiredMinRepValue=9000   WHERE entry=7164;
UPDATE quest_template SET RequiredMinRepFaction=729, RequiredMinRepValue=3000   WHERE entry=7163;

