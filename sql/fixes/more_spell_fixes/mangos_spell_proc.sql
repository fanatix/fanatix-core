-- Aspect of the Viper
REPLACE INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(34074, 0, 9, 522819, 8917121, 513, 0, 0, 0, 0, 0);

-- Sonic Booster
UPDATE spell_proc_event SET ppmRate='1' WHERE entry='55018';

-- Noise Machine
UPDATE spell_proc_event SET ppmRate='1' WHERE entry='55019';

-- Sonic Shield
INSERT INTO `spell_proc_event` VALUES ('54808', '0', '0', '0', '0', '0', '0', '0', '0', '0', '60');

-- Sonic Awareness (DND)
INSERT INTO `spell_proc_event` VALUES ('54707', '0', '0', '0', '0', '0', '0', '0', '0', '0', '60');

-- Invigorating Earthsiege Diamond Passive
INSERT INTO `spell_proc_event` VALUES ('61356', '0', '0', '0', '0', '0', '0', '0', '0', '0', '45');

-- Darkmoon Card: Greatness
INSERT INTO `spell_proc_event` VALUES ('57345', '0', '0', '0', '0', '0', '0', '0', '0', '0', '45');

-- Bandit's Insignia
INSERT INTO `spell_proc_event` VALUES ('60442', '0', '0', '0', '0', '0', '0', '0', '0', '0', '45');

-- Titanium Shield Spike
INSERT INTO `spell_proc_event` VALUES ('56355', '0', '0', '0', '0', '0', '64', '0', '0', '0', '5');

-- Thundering Skyflare Diamond
INSERT INTO `spell_proc_event` VALUES ('55380', '0', '0', '0', '0', '0', '0', '0', '0', '0', '45');

-- Swordguard Embroidery
INSERT INTO `spell_proc_event` VALUES ('55776', '0', '0', '0', '0', '0', '0', '0', '0', '0', '45');

-- Darkglow Embroidery
INSERT INTO `spell_proc_event` VALUES ('55768', '0', '0', '0', '0', '0', '0', '0', '0', '0', '45');

-- Lightweave Embroidery
INSERT INTO `spell_proc_event` VALUES ('55640', '0', '0', '0', '0', '0', '0', '0', '0', '0', '45');

-- Ferocious Inspiration
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(34457, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0);

-- Frenzy
INSERT INTO `spell_proc_event` (`entry` ,`SchoolMask` ,`SpellFamilyName` ,`SpellFamilyMask0` ,`SpellFamilyMask1` ,`SpellFamilyMask2` ,`procFlags` ,`procEx` ,`ppmRate` ,`CustomChance` ,`Cooldown`)VALUES 
('20784', '0', '0', '0', '0', '0', '0', '2', '0', '0', '0'); 

-- Master Tactician
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(34506, 0, 9, 522753, 8392833, 513, 0, 0, 0, 0, 0),
(34507, 0, 9, 522753, 8392833, 513, 0, 0, 0, 0, 0),
(34508, 0, 9, 522753, 8392833, 513, 0, 0, 0, 0, 0),
(34838, 0, 9, 522753, 8392833, 513, 0, 0, 0, 0, 0),
(34839, 0, 9, 522753, 8392833, 513, 0, 0, 0, 0, 0); 

-- Hunting Party
DELETE FROM spell_proc_event WHERE entry IN (53290, 53291, 53292, 53293, 53294);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(53290, 0, 9, 0x800, 0x1, 0x200, 0, 0x2, 0, 0, 0),
(53291, 0, 9, 0x800, 0x1, 0x200, 0, 0x2, 0, 0, 0),
(53292, 0, 9, 0x800, 0x1, 0x200, 0, 0x2, 0, 0, 0),
(53293, 0, 9, 0x800, 0x1, 0x200, 0, 0x2, 0, 0, 0),
(53294, 0, 9, 0x800, 0x1, 0x200, 0, 0x2, 0, 0, 0); 

-- Infusion of Light
DELETE FROM `spell_proc_event` WHERE `entry` IN ('53569','53576'); 
INSERT INTO spell_proc_event VALUES ('53569','0','10','2097152','327680','0','0','2','0','0','0'); 
INSERT INTO spell_proc_event VALUES ('53576','0','10','2097152','327680','0','0','2','0','0','0'); 

-- Arcane Potency
DELETE FROM `spell_proc_event` WHERE `entry` IN (31571, 31572);
INSERT INTO `spell_proc_event` VALUES (31571, 0x00, 3, 0x00000000, 0x00000022, 0x00000000, 0x00004000, 0x00000000, 0.000000, 0.000000, 0)
, (31572, 0x00, 3, 0x00000000, 0x00000022, 0x00000000, 0x00004000, 0x00000000, 0.000000, 0.000000, 0);