DELETE FROM `command` WHERE `name` IN ('bg update','bg end','bg list');
INSERT INTO `command` VALUES
('bg update',3,'Syntax: .bg update $seconds [$shift-bglink]\r\nif $shift-bglink isn\'t used it takes the bg of the targeted player or bg from current player\r\nthen bg gets an "BattleGround::Update"-call with $seconds as diff \r\nso a .bg update 120 at the beginning, will let your bg instantly start.'),

('bg end',3,'Syntax .bg end $winner [$shift-bglink]\r\nif $shift-bglink isn\'t used it takes the bg of the targeted player or current player\r\nthen bg gets ended with a $winner as winner\r\nWinnner=0->Horde,Winner=1->Alliance, else ->Draw'),

('bg list',3,'Syntax .bg list [bg-shiftlink]\r\nLists all current open Battlegrounds and arenas - with some information about them\r\nif the optional shiftlink-parameter is given, it just shows bgs/arenas of the same type\r\nfor arenas it means, shows only 2v2,3v3,5v5 and for bgs it shows av,ab,ey,wsg'),

DELETE FROM `command` WHERE `name` IN ('debug bg');
DELETE FROM `command` WHERE `name` IN ('bg debug');
INSERT INTO `command` VALUES
('bg debug',3,'Syntax: .bg debug\r\nToggle debug mode for battlegrounds. In debug mode GM can start battleground with single player.');
