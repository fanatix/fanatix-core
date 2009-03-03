-- Updating coefficients of Holy Light, Flash of Light and  Holy Shock healing spells and also Sacred Shield absorb
DELETE FROM `spell_bonus_data` WHERE `entry` IN ('19750', '635', '25914', '25913', '25903', '27175', '33074', '48820', '48821', '58597');
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `comments`) VALUES
('19750', '1', '0', '0', 'Paladin - Flash of Light'),
('635', '1.66', '0', '0', 'Paladin - Holy Light'),
('25914', '0.81', '0', '0', 'Paladin - Holy Shock Triggered Heal Rank 1'),
('25913', '0.81', '0', '0', 'Paladin - Holy Shock Triggered Heal Rank 2'),
('25903', '0.81', '0', '0', 'Paladin - Holy Shock Triggered Heal Rank 3'),
('27175', '0.81', '0', '0', 'Paladin - Holy Shock Triggered Heal Rank 4'),
('33074', '0.81', '0', '0', 'Paladin - Holy Shock Triggered Heal Rank 5'),
('48820', '0.81', '0', '0', 'Paladin - Holy Shock Triggered Heal Rank 6'),
('48821', '0.81', '0', '0', 'Paladin - Holy Shock Triggered Heal Rank 7'),
('58597', '0.75', '0', '0', 'Paladin - Sacred Shield');