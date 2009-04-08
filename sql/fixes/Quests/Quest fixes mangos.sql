UPDATE `creature_template` SET `ScriptName`='npc_clintar_dreamwalker' WHERE `entry`='22834';
UPDATE `creature_template` SET `ScriptName`='npc_clintar_spirit' WHERE `entry`='22916';
UPDATE `creature_template` SET `ScriptName` = `npc_deathstalker_erland` where `entry`='1978';
UPDATE `gameobject_template` SET `ScriptName`='go_cat_figurine' WHERE `entry`=13359;

-- Whizzlecrank's Shredder
UPDATE `creature_template` SET `ScriptName`='npc_wizzlecrank_shredder' WHERE `entry`='3439';

-- Nestlewood Owlkin (16518)
UPDATE `creature_template` SET `ScriptName`='mob_nestlewood_owlkin' WHERE `entry`='16518';

-- Ruul Snowhoof (12818)
UPDATE `creature_template` SET `ScriptName` = 'npc_ruul_snowhoof' WHERE `entry` = 12818;

UPDATE `quest_template` SET `specialflags` = `specialflags`|2 WHERE `entry` = '665';
UPDATE `creature_template` SET `ScriptName` = 'npc_professor_phizzlethorpe' WHERE `entry` = '2768';

-- Khadgar's Servant
UPDATE `creature_template` SET  `speed` = '1.7', `ScriptName` = 'npc_kservant' WHERE `entry` = '19685';
DELETE FROM `quest_start_scripts` WHERE `id`='10211';
UPDATE `quest_template` SET `StartScript`='0' WHERE entry=10211;

-- Khadgar
UPDATE `creature_template` SET `ScriptName`='npc_khadgar' WHERE `entry` = '18166';

UPDATE 'creature_template' SET 'ScriptName' = 'npc_broodling' WHERE 'entry' = '7047';
UPDATE 'creature_template' SET 'ScriptName' = 'npc_broodling' WHERE 'entry' = '7048';
UPDATE 'creature_template' SET 'ScriptName' = 'npc_broodling' WHERE 'entry' = '7049';


