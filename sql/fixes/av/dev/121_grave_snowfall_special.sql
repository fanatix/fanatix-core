-- snowfall eyecandy:
-- alliance
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179044,30,-191.153,-129.868,78.5595,-1.25664,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179044,30,-201.282,-134.319,78.6753,-0.942478,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179044,30,-215.981,-91.4101,80.8702,-1.74533,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179044,30,-200.465,-96.418,79.7587,1.36136,60);
INSERT INTO gameobject_battleground(guid,eventIndex) SELECT guid, 14 FROM gameobject ORDER BY guid DESC LIMIT 4;
-- alliance assault
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179424,30,-191.153,-129.868,78.5595,-1.25664,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179424,30,-201.282,-134.319,78.6753,-0.942478,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179424,30,-215.981,-91.4101,80.8702,-1.74533,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179424,30,-200.465,-96.418,79.7587,1.36136,60);
INSERT INTO gameobject_battleground(guid,eventIndex) SELECT guid, 13 FROM gameobject ORDER BY guid DESC LIMIT 4;
-- horde
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179064,30,-191.153,-129.868,78.5595,-1.25664,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179064,30,-201.282,-134.319,78.6753,-0.942478,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179064,30,-215.981,-91.4101,80.8702,-1.74533,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179064,30,-200.465,-96.418,79.7587,1.36136,60);
INSERT INTO gameobject_battleground(guid,eventIndex) SELECT guid, 16 FROM gameobject ORDER BY guid DESC LIMIT 4;
-- horde assault
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179425,30,-191.153,-129.868,78.5595,-1.25664,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179425,30,-201.282,-134.319,78.6753,-0.942478,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179425,30,-215.981,-91.4101,80.8702,-1.74533,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179425,30,-200.465,-96.418,79.7587,1.36136,60);
INSERT INTO gameobject_battleground(guid,eventIndex) SELECT guid, 15 FROM gameobject ORDER BY guid DESC LIMIT 4;


-- neutral banner+aura
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(180418,30,-202.581,-112.73,78.4876,-0.715585);              -- snowfall
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(180423,30,-202.581,-112.73,78.4876,-0.715585);              -- snowfall
INSERT INTO gameobject_battleground(guid,eventIndex) SELECT guid, 61 FROM gameobject ORDER BY guid DESC LIMIT 2;
