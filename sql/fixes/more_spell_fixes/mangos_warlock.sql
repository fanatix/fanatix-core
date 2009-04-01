-- Fel Stamina
DELETE FROM `spell_bonus_data` WHERE `entry` = 18790;
INSERT INTO `spell_bonus_data` (`entry`,`direct_bonus`,`dot_bonus`,`ap_bonus`,`comments`) VALUES
('18790','0','0','0','Warlock - Fel Stamina');