    -- the two buildings of the captains
    -- alliance
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(179066,30,-64.4987,-289.33,33.4616,-2.82743);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(179066,30,-5.98025,-326.144,38.8538,0);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(179066,30,-2.67893,-306.998,33.4165,0);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(179066,30,-60.25,-309.232,50.2408,-1.46608);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(179066,30,-48.7941,-266.533,47.7916,2.44346);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(179065,30,-3.40929,-306.288,33.34,0);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(179065,30,-48.619,-266.917,47.8168,0);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(179065,30,-62.9474,-286.212,66.7288,0);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(179065,30,-5.05132,-325.323,38.8536,0);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(179065,30,-64.2677,-289.412,33.469,0);
INSERT INTO gameobject_battleground(guid,eventIndex) SELECT guid, 63 FROM gameobject ORDER BY guid DESC LIMIT 10;
    -- horde
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(179066,30,-524.276,-199.6,82.8733,-1.46608);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(179066,30,-518.196,-173.085,102.43,0);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(179066,30,-500.732,-145.358,88.5337,2.44346);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(179066,30,-501.084,-150.784,80.8506,0);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(179066,30,-518.309,-163.963,102.521,2.96706);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(179065,30,-517.053,-200.429,80.759,0);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(179065,30,-514.361,-163.864,104.163,0);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(179065,30,-568.04,-188.707,81.55,0);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(179065,30,-501.775,-151.581,81.2027,0);
INSERT INTO gameobject(id,map,position_x,position_y,position_z,orientation) VALUES(179065,30,-509.975,-191.652,83.2978,0);
INSERT INTO gameobject_battleground(guid,eventIndex) SELECT guid, 64 FROM gameobject ORDER BY guid DESC LIMIT 10;
