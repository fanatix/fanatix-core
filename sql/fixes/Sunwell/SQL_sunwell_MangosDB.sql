DELETE FROM `gameobject` WHERE `id` = 185483 and map = 580;
DELETE FROM `gameobject` WHERE `id` = 175946 and map = 580;
DELETE FROM `gameobject` WHERE `id` = 188118 and map = 580;
DELETE FROM `gameobject` WHERE `id` = 186261 and map = 580;
DELETE FROM `gameobject` WHERE `id` = 186262 and map = 580;
DELETE FROM `gameobject` WHERE `id` = 187990 and map = 580;

INSERT INTO `gameobject` (`id`, `map`, `spawnMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES 
(185483, 580, 1, 1701.3, 761.89, 53.4243, 6.21725, 0, 0, 0.0329632, -0.999457, 25, 0, 1),
(175946, 580, 1, 1798.8, 578.744, 50.6908, 1.29281, 0, 0, 0.602319, 0.798255, 25, 0, 1),
(188118, 580, 1, 1774.93, 668.507, 71.1901, 2.0515, 0, 0, 0.855104, 0.518456, 25, 0, 1),
(186261, 580, 1, 1703.85, 890.729, 53.0768, 6.25825, 0, 0, 0.0124655, -0.999922, 25, 0, 0),
(186262, 580, 1, 1704.92, 965.248, 53.0768, 3.27453, 0, 0, 0.997792, -0.0664176, 25, 0, 0),
(187990, 580, 1, 1832.48, 670.337, 34.208, 4.3802, 0, 0, 0.814283, -0.580469, 25, 0, 1);

DELETE FROM `creature` WHERE `id` = 24892 and map = 580;
DELETE FROM `creature` WHERE `id` = 25840 and map = 580;
DELETE FROM `creature` WHERE `id` = 25038 and map = 580;

INSERT INTO `creature` (`id`, `map`, `spawnMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES 
(25038, 580, 1, 0, 0, 1469.05, 539.074, 22.3918, 1.49274, 25, 0, 0, 6100000, 4164, 0, 0),
(24892, 580, 1, 0, 0, 1685.75, 958.533, 53.0775, 5.35723, 25, 0, 0, 953471, 1693500, 0, 0),
(25840, 580, 1, 0, 0, 1815.54, 625.973, 69.6038, 5.61844, 25, 0, 0, 2050000, 0, 0, 0);

UPDATE `creature_template` SET `ScriptName` = 'boss_kalecgos' WHERE `entry` = '24850';
UPDATE `creature_template` SET `ScriptName` = 'boss_brutallus' WHERE `entry` = '24882';
UPDATE `creature_template` SET `ScriptName` = 'boss_sathrovarr' WHERE `entry` = '24892';
UPDATE `creature_template` SET `ScriptName` = 'boss_felmist' WHERE `entry` = '25038';
UPDATE `creature_template` SET `ScriptName` = 'boss_Sacrolash' WHERE `entry` = '25165';
UPDATE `creature_template` SET `ScriptName` = 'boss_Alythess' WHERE `entry` = '25166';
UPDATE `creature_template` SET `ScriptName` = 'boss_KJ' WHERE `entry` = '25315';
UPDATE `creature_template` SET `ScriptName` = 'mob_kiljaeden_controller' WHERE `entry` = '25608';
UPDATE `creature_template` SET `ScriptName` = 'boss_muru' WHERE `entry` = '25741';
UPDATE `creature_template` SET `ScriptName` = 'boss_entropius' WHERE `entry` = '25840';
UPDATE `instance_template` SET `Script` = 'instance_sunwell_plateau' Where map=580;
update creature_template set unit_flags=0 where entry=24882;

