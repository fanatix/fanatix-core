delete from mangos_string where entry>10746 and entry<10781;
INSERT INTO mangos_string VALUES
(10747,'Alliance',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10748,'Horde',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10749,'%s was destroyed by the %s!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10750,'The %s is under attack! If left unchecked, the %s will destroy it!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10751,'The %s was taken by the %s!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10752,'The %s was taken by the %s!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10753,'The %s was taken by the %s!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10754,'The %s is under attack! If left unchecked, the %s will capture it!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10755,'The %s has taken the %s! Its supplies will now be used for reinforcements!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10756,'Irondeep Mine',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10757,'Coldtooth Mine',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10758,'Stormpike Aid Station',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10759,'Dun Baldar South Bunker',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10760,'Dun Baldar North Bunker',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10761,'Stormpike Graveyard',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10762,'Icewing Bunker',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10763,'Stonehearth Graveyard',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10764,'Stonehearth Bunker',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10765,'Snowfall Graveyard',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10766,'Iceblood Tower',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10767,'Iceblood Graveyard',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10768,'Tower Point',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10769,'Frostwolf Graveyard',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10770,'East Frostwolf Tower',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10771,'West Frostwolf Tower',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10772,'Frostwolf Relief Hut',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10773,'2 minute until the battle for Alterac Valley begins.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10774,'1 minute until the battle for Alterac Valley begins.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10775,'30 seconds until the battle for Alterac Valley begins. Prepare yourselves!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10776,'The battle for Alterac Valley has begun!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10777,'The Alliance Team is running out of reinforcements!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10778,'The Horde Team is running out of reinforcements!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10779,'The Frostwolf General is Dead!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(10780,'The Stormpike General is Dead!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL)
;

UPDATE mangos_string SET content_loc3='Allianz' WHERE entry=10747;
UPDATE mangos_string SET content_loc3='Horde' WHERE entry=10748;
UPDATE mangos_string SET content_loc3='%s wurde von der %s zerstört!' WHERE entry=10749;
UPDATE mangos_string SET content_loc3='%s wird angegriffen! Wenn er unbewacht bleibt, wird die %s ihn zerstören!' WHERE entry=10750;
-- Wenn _sie_ unbewacht bleibt wird allianz _sie_ TODO: don't store towers and gravenames, store complete strings with the names inside it, else i don't know how to implement this feature

UPDATE mangos_string SET content_loc3='%s wurde von der %s erobert!' WHERE entry=10751;
UPDATE mangos_string SET content_loc3='%s wurde von der %s erobert!' WHERE entry=10752;
UPDATE mangos_string SET content_loc3='%s wurde von der %s erobert!' WHERE entry=10753;

UPDATE mangos_string SET content_loc3='%s wird angegriffen! Wenn er unbewacht bleibt, wird die %s ihn erobern!' WHERE entry=10754;
-- Wenn _sie_ unbewacht bleibt ... TODO...

UPDATE mangos_string SET content_loc3='Die %s hat die %s eingenommen! Die dort befindlichen Vorräte werden zur Verstärkung eingesetzt!' WHERE entry=10755;

UPDATE mangos_string SET content_loc3='Eisenschachtmine' WHERE entry=10756;
UPDATE mangos_string SET content_loc3='Eisbeißermine' WHERE entry=10757;
UPDATE mangos_string SET content_loc3='Das Lazarett der Sturmlanzen' WHERE entry=10758;
UPDATE mangos_string SET content_loc3='Der Südbunker von Dun Baldar' WHERE entry=10759;
UPDATE mangos_string SET content_loc3='Der Nordbunker von Dun Baldar' WHERE entry=10760;
UPDATE mangos_string SET content_loc3='Der Friedhof der Sturmlanzen' WHERE entry=10761;
UPDATE mangos_string SET content_loc3='Der Eisschwingenbunker' WHERE entry=10762;
UPDATE mangos_string SET content_loc3='Der Steinbruchfriedhof' WHERE entry=10763;
UPDATE mangos_string SET content_loc3='Der Steinbruchbunker' WHERE entry=10764;
UPDATE mangos_string SET content_loc3='Der Schneewehenfriedhof' WHERE entry=10765;
UPDATE mangos_string SET content_loc3='Der Eisblutturm' WHERE entry=10766;
UPDATE mangos_string SET content_loc3='Der Eisblutfriedhof' WHERE entry=10767;
UPDATE mangos_string SET content_loc3='Die Turmstellung' WHERE entry=10768;
UPDATE mangos_string SET content_loc3='Der Friedhof der Frostwölfe' WHERE entry=10769;
UPDATE mangos_string SET content_loc3='Der östliche Frostwolfturm' WHERE entry=10770;
UPDATE mangos_string SET content_loc3='Der westliche Frostwolfturm' WHERE entry=10771;
UPDATE mangos_string SET content_loc3='Die Heilerhütte der Frostwölfe' WHERE entry=10772;
UPDATE mangos_string SET content_loc3='Der Kampf um das Alteractal beginnt in 2 Minuten.' WHERE entry=10773;
UPDATE mangos_string SET content_loc3='Der Kampf um das Alteractal beginnt in 1 Minute.' WHERE entry=10774;
UPDATE mangos_string SET content_loc3='Der Kampf um das Alteractal beginnt in 30 Sekunden.' WHERE entry=10775;
UPDATE mangos_string SET content_loc3='Der Kampf um das Alteractal hat begonnen!' WHERE entry=10776;
UPDATE mangos_string SET content_loc3='Die Allianz hat nurnoch wenige Verstärkungspunkte' WHERE entry=10777; -- TODO find right msg
UPDATE mangos_string SET content_loc3='Die Horde hat nurnoch wenige Verstärkungspunkte' WHERE entry=10778; -- TODO same here
UPDATE mangos_string SET content_loc3='Der General der Allianz ist tot!' WHERE entry=10779;
UPDATE mangos_string SET content_loc3='Der General der Horde ist tot!' WHERE entry=10780;


-- CHAT_MSG_BG_SYSTEM_ALLIANCE', 'txt': '$N hat den Schneewehenfriedhof besetzt! Wenn niemand eingreift, erlangt die Allianz die Kontrolle \xc3\xbcber ihn
