UPDATE creature c, creature_template t SET c.curhealth=t.minhealth WHERE c.map=30 AND c.id=t.entry;
UPDATE creature c, creature_template t SET c.curmana=t.minmana WHERE c.map=30 AND c.id=t.entry;
UPDATE creature SET spawndist=0 WHERE MovementType=0 AND map=30;

-- horde and ally captains should never respawn:
UPDATE creature SET spawntimesecs=86400 where id in (11949,11947);
-- stonehewer a npc next to van respawns slow
UPDATE creature SET spawntimesecs=1200 where id in (13816);

-- delete to much reputation-gains from av-creatures
DELETE FROM creature_onkill_reputation WHERE RewOnKillRepFaction1 IN (729,730) AND RewOnKillRepValue1 > 100;
