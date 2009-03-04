-- Spawn Majordomo
DELETE FROM `creature` WHERE `id`=12018;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES (125874, 12018, 409, 1, 0, 0, 750.745178, -1226.584961, -119.617752, 1.791157, 30, 0, 0, 3900, 21000, 0, 0);

-- Spawn Cache of the Firelord
DELETE FROM `gameobject` WHERE `id`=179703;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES (100001, 179703, 409, 1, 750.745178, -1226.584961, -119.617752, 1.791157, 0, 0, 0, 0, -3600, 100, 1);

-- Loot for Cache of the Firelord
DELETE FROM `gameobject_loot_template` WHERE (`entry`=16719)
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `groupid`, `mincountOrRef`, `maxcount`, `lootcondition`, `condition_value1`, `condition_value2`) VALUES 
(16719, 18646, 90, 1, 1, 1, 0, 0, 0),
(16719, 18703, 86, 1, 1, 1, 0, 0, 0),
(16719, 18803, 37, 1, 1, 1, 0, 0, 0),
(16719, 18805, 37, 1, 1, 1, 0, 0, 0),
(16719, 18806, 34, 1, 1, 1, 0, 0, 0),
(16719, 18808, 36, 1, 1, 1, 0, 0, 0),
(16719, 18809, 36, 1, 1, 1, 0, 0, 0),
(16719, 18810, 36, 1, 1, 1, 0, 0, 0),
(16719, 18811, 34, 1, 1, 1, 0, 0, 0),
(16719, 18812, 36, 1, 1, 1, 0, 0, 0),
(16719, 19139, 33, 1, 1, 1, 0, 0, 0),
(16719, 19140, 35, 1, 1, 1, 0, 0, 0);