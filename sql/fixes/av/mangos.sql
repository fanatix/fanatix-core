REPLACE INTO gameobject_template VALUES (178925,1,5651,"Alliance Banner","",83,0,1,0,1479,196608,180421,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (178943,1,5652,"Horde Banner","",84,0,1,0,1479,196608,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (178940,1,5653,"Contested Banner","",83,0,1,0,1479,196608,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (179435,1,5654,"Contested Banner","",84,0,1,0,1479,196608,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"");

REPLACE INTO gameobject_template VALUES (178365,1,5771,"Alliance Banner","",83,0,1,0,1479,196608,180100,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (179286,1,5772,"Contested Banner","",83,0,1,0,1479,196608,180102,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (179287,1,5774,"Contested Banner","",84,0,1,0,1479,0,180102,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (180418,1,6211,"Snowfall Banner","",0,0,1,0,1479,196608,180100,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"");

REPLACE INTO gameobject_template VALUES (180424,0,3751,"Alterac Valley Gate","",100,0,3.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (180100,6,2232,"Alliance Banner Aura","",0,0,2,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (180102,6,266,"Neutral Banner Aura","",0,0,2,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (180101,6,1311,"Horde Banner Aura","",0,0,2,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (180421,6,2232,"Alliance Banner Aura, Large","",0,0,5,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (180423,6,266,"Neutral Banner Aura, Large","",0,0,5,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,"");
REPLACE INTO gameobject_template VALUES (180422,6,1311,"Horde Banner Aura, Large","",0,0,5,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,"");

-- removes the damage from the fire-gos (used for destroyed towers) - but this is not blizzlike TODO remove this hack
UPDATE `gameobject_template` SET `type` = '5',`data0` = '0',`data1` = '0',`data2` = '0',`data3` = '1',`data5` = '0',`data10` = '0' WHERE `gameobject_template`.`entry` =179065 LIMIT 1 ;

-- bowman apply aura entangling root (so they can't move)
DELETE FROM `creature_template_addon` WHERE `entry` IN(13358,13359);
INSERT INTO `creature_template_addon` (`entry`, `auras`) VALUES ('13358', '42716 0 42716 1');
INSERT INTO `creature_template_addon` (`entry`, `auras`) VALUES ('13359', '42716 0 42716 1');

-- following is the player loot.. it takes the id 1 - be sure that i don't delete anything
REPLACE INTO `creature_loot_template` (`entry` ,`item` ,`ChanceOrQuestChance` ,`groupid` ,`mincountOrRef` ,`maxcount` ,`lootcondition` ,`condition_value1` ,`condition_value2`)
VALUES ('1', '17306', '33', '0', '5', '15',  '6', '67', '0'),
('1', '17422', '100', '0', '20', '40', '0', '0', '0'),
('1', '17423', '33', '0', '5', '15',  '6', '469', '0'),
( '1', '17502', '30', '1', '5', '10', '6', '469', '0' ),
( '1', '17503', '30', '1', '5', '10', '6', '469', '0'),
( '1', '17504', '30', '1', '5', '10', '6', '469', '0' ),
( '1', '17326', '30', '2', '5', '10', '6', '67', '0' ),
( '1', '17327', '30', '2', '5', '10', '6', '67', '0' ),
( '1', '17328', '30', '2', '5', '10', '6', '67', '0' );

-- blizzlike mobs in the mines don't have any loot (not even gold, it doesn't show the looticon if i go with my mouse over them)
-- broken after the strange new loot-table appeared (but not important)  delete from creature_loot_template where entry in (13396,13080,13098,13078,13397,13099,13081,13079,11603,11604,11605,11677,10982,13317,13096,13087,13086,13316,13097,13089,13088);
-- TODO: write own loottable


INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-917.648,-46.8922,77.0872,5.27089);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-912.689,-45.4494,76.2277,4.60767);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-905.455,-84.5179,75.3642,3.29867);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-904.332,-111.509,75.5925,2.47837);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-904.27,-160.419,61.9876,3.61192);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-904.023,-90.4558,75.3706,3.40339);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-978.678,-37.3136,75.8364,2.84489);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-973.076,-36.5013,77.5047,1.0821);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-963.951,-87.734,81.5555,0.575959);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-961.941,-90.7252,81.6629,0.820305);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-957.623,-186.582,66.6021,1.95477);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-952.476,-179.778,78.6771,4.5204);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-950.427,-115.007,79.6127,3.68264);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-950.25,-151.95,79.4598,-1.81423);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-950.169,-188.099,66.6184,5.55015);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-949.944,-142.977,80.5382,2.70526);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-947.854,-170.5,79.7618,0.942478);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-946.738,-139.567,80.0904,2.3911);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-945.503,-65.0654,79.7907,5.02655);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-943.678,-110.986,80.2557,0.959931);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-942.993,-56.9881,79.8915,5.65487);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-938.197,-155.838,61.3111,1.65806);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-930.488,-214.524,72.1431,2.1236);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-929.947,-154.449,61.5084,1.67552);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-927.412,-135.313,61.1987,3.29867);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-920.677,-156.859,62.8033,3.15306);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-916.75,-136.094,62.2357,0.0698132);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-915.319,-132.718,62.562,1.16984);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-913.589,-146.794,76.9366,1.8675);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-907.572,-148.937,76.6898,4.76475);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-902.02,-64.6174,73.9707,1.19169);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-899.489,-61.7252,73.2498,5.09636);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-894.792,-127.141,75.3834,6.14356);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-892.408,-162.525,64.1212,2.69884);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-892.326,-123.158,76.0318,5.5676);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-888.468,-148.462,61.8012,1.65806);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-883.268,-159.738,63.5311,5.20108);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-877.76,-118.07,65.215,2.94961);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-876.792,-128.646,64.1045,3.40339);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-874.901,-36.6579,69.4246,2.00713);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-874.856,-151.351,62.7537,3.57875);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-872.135,-150.08,62.7513,3.57201);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-870.288,-149.217,62.5413,3.56624);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-870.03,-6.27443,70.3867,2.3911);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-869.023,-82.2118,69.5848,3.22886);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-866.354,-40.2455,70.842,0.0698132);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-865.305,-152.302,63.5044,4.86947);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-861.926,-79.0519,71.4178,0.20944);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-857.292,-152.277,63.2114,4.18879);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11603,30,-853.357,-0.696194,72.0655,0.994838);

-- 
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-917.648,-46.8922,77.0872,5.27089);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-912.689,-45.4494,76.2277,4.60767);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-905.455,-84.5179,75.3642,3.29867);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-904.332,-111.509,75.5925,2.47837);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-904.27,-160.419,61.9876,3.61192);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-904.023,-90.4558,75.3706,3.40339);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-978.678,-37.3136,75.8364,2.84489);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-973.076,-36.5013,77.5047,1.0821);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-963.951,-87.734,81.5555,0.575959);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-961.941,-90.7252,81.6629,0.820305);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-957.623,-186.582,66.6021,1.95477);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-952.476,-179.778,78.6771,4.5204);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-950.427,-115.007,79.6127,3.68264);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-950.25,-151.95,79.4598,-1.81423);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-950.169,-188.099,66.6184,5.55015);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-949.944,-142.977,80.5382,2.70526);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-947.854,-170.5,79.7618,0.942478);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-946.738,-139.567,80.0904,2.3911);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-945.503,-65.0654,79.7907,5.02655);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-943.678,-110.986,80.2557,0.959931);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-942.993,-56.9881,79.8915,5.65487);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-938.197,-155.838,61.3111,1.65806);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-930.488,-214.524,72.1431,2.1236);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-929.947,-154.449,61.5084,1.67552);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-927.412,-135.313,61.1987,3.29867);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-920.677,-156.859,62.8033,3.15306);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-916.75,-136.094,62.2357,0.0698132);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-915.319,-132.718,62.562,1.16984);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-913.589,-146.794,76.9366,1.8675);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-907.572,-148.937,76.6898,4.76475);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-902.02,-64.6174,73.9707,1.19169);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-899.489,-61.7252,73.2498,5.09636);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-894.792,-127.141,75.3834,6.14356);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-892.408,-162.525,64.1212,2.69884);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-892.326,-123.158,76.0318,5.5676);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-888.468,-148.462,61.8012,1.65806);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-883.268,-159.738,63.5311,5.20108);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-877.76,-118.07,65.215,2.94961);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-876.792,-128.646,64.1045,3.40339);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-874.901,-36.6579,69.4246,2.00713);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-874.856,-151.351,62.7537,3.57875);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-872.135,-150.08,62.7513,3.57201);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-870.288,-149.217,62.5413,3.56624);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-870.03,-6.27443,70.3867,2.3911);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-869.023,-82.2118,69.5848,3.22886);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-866.354,-40.2455,70.842,0.0698132);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-865.305,-152.302,63.5044,4.86947);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-861.926,-79.0519,71.4178,0.20944);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-857.292,-152.277,63.2114,4.18879);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13317,30,-853.357,-0.696194,72.0655,0.994838);


--
--
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-917.648,-46.8922,77.0872,5.27089);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-912.689,-45.4494,76.2277,4.60767);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-905.455,-84.5179,75.3642,3.29867);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-904.332,-111.509,75.5925,2.47837);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-904.27,-160.419,61.9876,3.61192);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-904.023,-90.4558,75.3706,3.40339);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-978.678,-37.3136,75.8364,2.84489);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-973.076,-36.5013,77.5047,1.0821);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-963.951,-87.734,81.5555,0.575959);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-961.941,-90.7252,81.6629,0.820305);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-957.623,-186.582,66.6021,1.95477);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-952.476,-179.778,78.6771,4.5204);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-950.427,-115.007,79.6127,3.68264);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-950.25,-151.95,79.4598,-1.81423);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-950.169,-188.099,66.6184,5.55015);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-949.944,-142.977,80.5382,2.70526);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-947.854,-170.5,79.7618,0.942478);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-946.738,-139.567,80.0904,2.3911);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-945.503,-65.0654,79.7907,5.02655);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-943.678,-110.986,80.2557,0.959931);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-942.993,-56.9881,79.8915,5.65487);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-938.197,-155.838,61.3111,1.65806);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-930.488,-214.524,72.1431,2.1236);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-929.947,-154.449,61.5084,1.67552);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-927.412,-135.313,61.1987,3.29867);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-920.677,-156.859,62.8033,3.15306);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-916.75,-136.094,62.2357,0.0698132);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-915.319,-132.718,62.562,1.16984);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-913.589,-146.794,76.9366,1.8675);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-907.572,-148.937,76.6898,4.76475);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-902.02,-64.6174,73.9707,1.19169);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-899.489,-61.7252,73.2498,5.09636);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-894.792,-127.141,75.3834,6.14356);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-892.408,-162.525,64.1212,2.69884);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-892.326,-123.158,76.0318,5.5676);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-888.468,-148.462,61.8012,1.65806);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-883.268,-159.738,63.5311,5.20108);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-877.76,-118.07,65.215,2.94961);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-876.792,-128.646,64.1045,3.40339);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-874.901,-36.6579,69.4246,2.00713);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-874.856,-151.351,62.7537,3.57875);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-872.135,-150.08,62.7513,3.57201);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-870.288,-149.217,62.5413,3.56624);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-870.03,-6.27443,70.3867,2.3911);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-869.023,-82.2118,69.5848,3.22886);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-866.354,-40.2455,70.842,0.0698132);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-865.305,-152.302,63.5044,4.86947);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-861.926,-79.0519,71.4178,0.20944);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-857.292,-152.277,63.2114,4.18879);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13316,30,-853.357,-0.696194,72.0655,0.994838);

-- special
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-954.622,-110.958,80.7911,6.24828);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-951.477,-53.9647,80.0235,5.32325);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-946.812,-126.04,78.8601,5.15265);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-940.689,-140.707,79.9225,2.79253);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-933.954,-159.632,60.778,2.56563);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-922.537,-130.291,61.3756,4.95674);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-915.862,-151.74,76.9427,0.942478);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-888.321,-159.831,62.5303,1.20428);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-874.361,-42.4751,69.4316,0.785398);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-873.19,-50.4899,70.0568,-2.41288);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-868.511,-148.386,62.3547,3.57875);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-868.44,-121.649,64.5056,3.33358);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-868.324,-77.7196,71.4768,5.41052);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-859.846,-19.6549,70.7304,1.97222);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-828.05,-150.508,62.2019,2.14675);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-826.254,-58.6911,72.0041,3.68264);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-976.086,-44.1775,76.029,1.46608);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-971.864,-87.4223,81.4954,5.8294);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-966.551,-74.1111,80.0243,4.2129);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-958.509,-173.652,77.9013,6.24828);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-951.511,-181.242,65.529,4.39823);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-940.967,-186.243,77.698,1.28164);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-930.004,-65.0898,79.077,0.0581657);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-920.864,-40.2009,78.256,5.16617);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-919.089,-148.021,62.0317,2.59327);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-901.516,-116.329,75.6876,0.471239);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-897.864,-84.4348,74.083,3.00197);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-897.617,-52.0457,71.9503,4.36332);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-894.891,-153.951,61.6827,3.23569);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-893.933,-111.625,75.6591,4.22536);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-883.265,-152.854,61.8384,0.0941087);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-868.293,-147.243,62.1097,3.2056);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-867.501,-11.8709,70.018,6.14356);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-866.699,-147.54,62.1646,3.57878);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-866.566,-91.1916,67.4414,4.56707);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-857.272,-141.142,61.7356,4.17134);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-847.446,-98.0061,68.5131,3.24631);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-837.026,-140.729,62.5141,5.51524);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11604,30,-824.204,-65.053,72.3381,3.01942);
 
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-954.622,-110.958,80.7911,6.24828);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-951.477,-53.9647,80.0235,5.32325);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-946.812,-126.04,78.8601,5.15265);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-940.689,-140.707,79.9225,2.79253);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-933.954,-159.632,60.778,2.56563);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-922.537,-130.291,61.3756,4.95674);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-915.862,-151.74,76.9427,0.942478);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-888.321,-159.831,62.5303,1.20428);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-874.361,-42.4751,69.4316,0.785398);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-873.19,-50.4899,70.0568,-2.41288);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-868.511,-148.386,62.3547,3.57875);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-868.44,-121.649,64.5056,3.33358);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-868.324,-77.7196,71.4768,5.41052);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-859.846,-19.6549,70.7304,1.97222);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-828.05,-150.508,62.2019,2.14675);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-826.254,-58.6911,72.0041,3.68264);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-976.086,-44.1775,76.029,1.46608);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-971.864,-87.4223,81.4954,5.8294);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-966.551,-74.1111,80.0243,4.2129);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-958.509,-173.652,77.9013,6.24828);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-951.511,-181.242,65.529,4.39823);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-940.967,-186.243,77.698,1.28164);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-930.004,-65.0898,79.077,0.0581657);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-920.864,-40.2009,78.256,5.16617);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-919.089,-148.021,62.0317,2.59327);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-901.516,-116.329,75.6876,0.471239);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-897.864,-84.4348,74.083,3.00197);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-897.617,-52.0457,71.9503,4.36332);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-894.891,-153.951,61.6827,3.23569);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-893.933,-111.625,75.6591,4.22536);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-883.265,-152.854,61.8384,0.0941087);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-868.293,-147.243,62.1097,3.2056);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-867.501,-11.8709,70.018,6.14356);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-866.699,-147.54,62.1646,3.57878);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-866.566,-91.1916,67.4414,4.56707);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-857.272,-141.142,61.7356,4.17134);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-847.446,-98.0061,68.5131,3.24631);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-837.026,-140.729,62.5141,5.51524);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13096,30,-824.204,-65.053,72.3381,3.01942);
 
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-954.622,-110.958,80.7911,6.24828);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-951.477,-53.9647,80.0235,5.32325);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-946.812,-126.04,78.8601,5.15265);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-940.689,-140.707,79.9225,2.79253);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-933.954,-159.632,60.778,2.56563);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-922.537,-130.291,61.3756,4.95674);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-915.862,-151.74,76.9427,0.942478);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-888.321,-159.831,62.5303,1.20428);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-874.361,-42.4751,69.4316,0.785398);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-873.19,-50.4899,70.0568,-2.41288);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-868.511,-148.386,62.3547,3.57875);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-868.44,-121.649,64.5056,3.33358);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-868.324,-77.7196,71.4768,5.41052);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-859.846,-19.6549,70.7304,1.97222);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-828.05,-150.508,62.2019,2.14675);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-826.254,-58.6911,72.0041,3.68264);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-976.086,-44.1775,76.029,1.46608);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-971.864,-87.4223,81.4954,5.8294);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-966.551,-74.1111,80.0243,4.2129);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-958.509,-173.652,77.9013,6.24828);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-951.511,-181.242,65.529,4.39823);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-940.967,-186.243,77.698,1.28164);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-930.004,-65.0898,79.077,0.0581657);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-920.864,-40.2009,78.256,5.16617);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-919.089,-148.021,62.0317,2.59327);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-901.516,-116.329,75.6876,0.471239);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-897.864,-84.4348,74.083,3.00197);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-897.617,-52.0457,71.9503,4.36332);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-894.891,-153.951,61.6827,3.23569);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-893.933,-111.625,75.6591,4.22536);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-883.265,-152.854,61.8384,0.0941087);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-868.293,-147.243,62.1097,3.2056);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-867.501,-11.8709,70.018,6.14356);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-866.699,-147.54,62.1646,3.57878);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-866.566,-91.1916,67.4414,4.56707);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-857.272,-141.142,61.7356,4.17134);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-847.446,-98.0061,68.5131,3.24631);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-837.026,-140.729,62.5141,5.51524);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13097,30,-824.204,-65.053,72.3381,3.01942);

-- vermins
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10982,30,-951.955,-197.5,77.212,5.63741);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10982,30,-944.837,-199.608,77.0737,4.97419);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10982,30,-933.494,-209.063,73.7803,5.88176);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10982,30,-929.666,-201.308,73.7032,5.02655);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10982,30,-978.997,-249.356,65.4345,5.05464);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10982,30,-974.565,-224.828,69.5858,4.88846);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10982,30,-946.514,-259.239,66.0874,3.78132);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10982,30,-918.402,-250.439,69.5271,2.21352);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10982,30,-910.14,-229.959,72.9279,0.27677);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10982,30,-851.563,-88.6527,68.5983,3.61896);

-- bosses
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13088,30,-848.902,-92.931,68.6325,3.33350);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13086,30,-848.902,-92.931,68.6325,3.33350);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11677,30,-848.902,-92.931,68.6325,3.33350);
UPDATE creature c, creature_template t SET c.curhealth=t.minhealth WHERE c.map=30 AND c.id=t.entry;
UPDATE creature SET spawndist=0 WHERE MovementType=0 AND map=30;
DELETE FROM creature WHERE map=30;
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(2225,30,-1235.31, -340.777, 60.5088, 3.31613);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(3343,30,-1244.02, -323.795, 61.0485, 5.21853);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(3625,30,-1235.16, -332.302, 60.2985, 2.96706);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(4255,30,587.303, -42.8257, 37.5615, 5.23599);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(4257,30,643.635, -58.3987, 41.7405, 4.72984);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(5134,30,591.464, -44.452, 37.6166, 5.65487);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(5135,30,608.515, -33.3935, 42.0003, 5.41052);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(5139,30,617.656, -32.0701, 42.7168, 4.06662);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10364,30,-1183.76, -268.295, 72.8233, 3.28122);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10367,30,-1187.86, -275.31, 73.0481, 3.63028);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10981,30,-1008.42, -368.006, 55.3426, 5.95647);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10981,30,-1091.92, -424.28, 53.0139, 2.93958);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10981,30,-558.455, -198.768, 58.1755, 4.97946);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10981,30,-861.247, -312.51, 55.1427, 3.35382);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10981,30,-1003.81, -395.913, 50.4736, 2.85631);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10981,30,-904.5, -289.815, 65.1222, 5.7847);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10981,30,-1064.41, -438.839, 51.3614, 1.88857);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10986,30,258.814, 76.2017, 18.6468, 6.19052);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10986,30,265.838, -315.846, -16.5429, 3.15917);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10986,30,426.485, -51.1927, -5.66286, 1.60347);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10986,30,452.044, -33.9594, -0.044651, 2.72815);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10986,30,266.032, -315.639, -16.5429, 4.67962);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10986,30,532.64, -54.5863, 20.7024, 2.93215);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,295.183, -299.908, -34.6123, 0.135851);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,421.08, -225.006, -23.73, 0.166754);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,-55.7766, -192.498, 20.4352, 6.12221);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,527.887, -477.223, 62.3559, 0.170935);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,389.144, -346.508, -30.334, 4.14117);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,108.121, -322.248, 37.5655, 4.46788);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,507.479, -67.9403, 10.3571, 3.26304);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,329.071, -185.016, -29.1542, 0.356943);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,252.449, -422.313, 35.1404, 4.53771);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,358.882, -118.061, -24.9119, 2.29257);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,487.151, -174.229, 14.7558, 4.73192);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,449.652, -123.561, 6.14273, 6.12029);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,272.419, -261.802, -41.8835, 3.66559);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,359.021, -210.954, -29.3483, 4.31339);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,450.598, -318.048, -37.7548, 0.655219);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,509.333, -218.2, 3.05439, 3.66292);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,485.771, -223.613, -1.53, 2.04862);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,486.636, -452.172, 39.6592, 2.3341);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,702.783, -257.494, 25.9777, 1.68329);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,460.942, -199.263, -6.0149, 0.380506);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,483.108, -115.307, 10.1056, 3.69701);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,471.601, -154.174, 14.0702, 5.5807);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,213.938, -420.793, 41.2549, 5.71394);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,289.387, -294.685, -33.9073, 0.555494);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,155.649, -402.891, 43.3915, 5.94838);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,517.184, -295.105, -9.78195, 6.05668);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,102.334, -332.165, 38.9812, 3.31445);

UPDATE creature SET movementtype=1 WHERE id IN (10981,10986,10990) AND map=30; -- frostwolf, harpy, ram

INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10990,30,320.244, -107.793, -42.6357, -1.00311);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11675,30,217.976, 110.774, 15.7603, 4.56793);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11675,30,269.872, 6.66684, 20.7592, 0.381212);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11675,30,313.528, -319.041, -27.2373, 0.554098);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11675,30,435.441, -39.9289, -0.169651, 0.549454);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11675,30,315.115, -317.62, -29.1123, 0.90111);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11678,30,428.091, -122.731, 3.40332, 6.05901);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11678,30,235.05, 85.5705, 18.3079, -0.914255);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11839,30,-1553.04, -344.342, 64.4163, 6.09933);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11947,30,-545.23, -165.35, 57.7886, 3.01145);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11948,30,722.43, -10.9982, 50.7046, 3.42085);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11949,30,-57.7891, -286.597, 15.6479, 6.02139);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11997,30,930.498, -520.755, 93.7334, 1.8326);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(12051,30,-776.092, -345.161, 67.4092, 1.89257);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(12051,30,-1224.63, -308.144, 65.0087, 4.01139);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(12051,30,-713.039, -442.515, 82.8638, 0.68724);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(12051,30,-711.783, -444.061, 82.7039, 0.683494);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(12096,30,587.633, -45.9816, 37.5438, 5.81195);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(12097,30,-1293.79, -194.407, 72.4398, 5.84685);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(12127,30,446.163, -377.119, -1.12725, 0.209526);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(12127,30,549.348, -399.254, 53.3537, 3.24729);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(12127,30,549.801, -401.217, 53.8305, 3.24729);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(12127,30,192.704, -406.874, 42.9183, 6.10696);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(12127,30,441.305, -435.765, 28.2385, 2.14472);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(12127,30,192.982, -404.891, 43.0132, 6.1061);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(12127,30,355.342, -391.989, -0.486707, 3.00643);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(12127,30,446.035, -375.104, -1.12725, 0.21033);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(12127,30,697.864, -433.238, 62.7914, 1.65776);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(12127,30,610.74, -331.585, 30.8021, 5.14253);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(12127,30,609.815, -329.775, 30.9271, -2.38829);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(12127,30,695.874, -433.434, 62.8543, 1.65776);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(12127,30,443.337, -435.283, 28.6842, 2.13768);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13176,30,-1251.5, -316.327, 62.6565, 5.02655);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13179,30,-1332.0, -331.243, 91.2631, 1.50098);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13216,30,569.983, -94.9992, 38.0325, 1.39626);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13218,30,-1244.92, -308.916, 63.2525, 1.62316);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13236,30,-1319.56, -342.675, 60.3404, 1.20428);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13257,30,647.61, -61.1548, 41.7405, 4.24115);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13284,30,-1321.64, -343.73, 60.4833, 1.01229);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13284,30,-1317.61, -342.853, 60.3726, 2.47837);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13284,30,-1319.31, -344.475, 60.3825, 1.72788);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13438,30,569.963, -42.0218, 37.7581, 4.27606);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13442,30,729.2, -78.812, 51.6335, 3.97935);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13443,30,729.118, -82.8713, 51.6335, 2.53073);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13443,30,725.554, -79.4973, 51.6335, 5.27089);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13443,30,724.768, -84.1642, 51.6335, 0.733038);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13447,30,596.68, -83.0633, 39.0051, 6.24828);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13577,30,600.032, -2.92475, 42.0788, 5.00909);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13617,30,610.239, -21.8454, 43.272, 4.90438);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13797,30,613.422, -150.764, 33.4517, 5.55015);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13798,30,-1213.91, -370.619, 56.4455, 0.837758);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13816,30,704.35, -22.9071, 50.2187, 0.785398);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(14185,30,-1271.24, -335.766, 62.3971, 5.75959);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(14186,30,-1268.64, -332.688, 62.6171, 5.28835);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(14187,30,648.363, -65.2233, 41.7405, 3.12414);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(14188,30,648.238, -67.8931, 41.7405, 2.60054);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(14282,30,-1223.44, -309.833, 64.9331, 4.0131);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(14282,30,-1226.4, -307.136, 64.9706, 4.0145);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(14283,30,356.001, -389.969, -0.438796, 3.0334);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(14283,30,355.835, -394.005, -0.60149, 3.02498);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(14283,30,882.266, -496.378, 96.7707, 4.83248);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(14283,30,878.649, -495.917, 96.6171, 4.67693);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(14283,30,932.851, -511.017, 93.6748, 3.61004);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(14283,30,935.806, -513.983, 93.7436, 3.61788);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(14284,30,947.412, -509.982, 95.1098, 2.82743);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(14284,30,934.557, -512.395, 93.662, 3.61004);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(14284,30,939.42, -502.777, 94.5887, 5.14872);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(14284,30,854.276, -494.241, 96.8017, 5.44543);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(14284,30,776.621, -487.775, 99.4049, 3.50811);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(14284,30,880.169, -495.699, 96.6204, 4.8325);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(14284,30,773.651, -497.482, 99.0408, 2.11185);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(14284,30,949.1, -506.913, 95.4237, 3.31613);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11946,30,-1370.9, -219.793, 98.4258, 5.04381);

-- marshals, who stand around the boss
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation)
VALUES(14763,30,721.104, -7.64155, 50.7046, 3.45575);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation)
VALUES(14762,30,723.058, -14.1548, 50.7046, 3.40339);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation)
VALUES(14764,30,715.691, -4.72233, 50.2187, 3.47321);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation)
VALUES(14765,30,720.046, -19.9413, 50.2187, 3.36849);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation)
VALUES(14773,30,-1363.99, -221.99, 98.4053, 4.93012);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation)
VALUES(14776,30,-1370.96, -223.532, 98.4266, 4.93012);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation)
VALUES(14772,30,-1378.37, -228.614, 99.3546, 5.38565);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation)
VALUES(14777,30,-1358.02, -228.998, 98.868, 3.87768);

UPDATE creature c, creature_template t SET c.curhealth=t.minhealth WHERE c.map=30 AND c.id=t.entry;
UPDATE creature SET spawndist=0 WHERE MovementType=0 AND map=30;
REPLACE INTO creature_template
   (`entry`, `heroic_entry`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `baseattacktime`, `rangeattacktime`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`)
VALUES
   (11946, 0, 11894, 0, 11894, 0, 'Drek\'Thar', 'Frostwolf General', '', 72, 72, 220000, 250000, 0, 0, 0, 1214, 1214, 1, 2, 1, 3, 5000, 8000, 0, 15000, 2000, 0, 4096, 0, 0, 0, 0, 0, 0, 203.77, 289.86, 100, 7, 4, 0, 11946, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 1, 3, 0, 1, 994, 80428891, 1, 'mob_eventai');

REPLACE INTO creature_template
   (`entry`, `heroic_entry`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `baseattacktime`, `rangeattacktime`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`)
VALUES
   (11947, 0, 11895, 0, 11895, 0, 'Captain Galvangar', 'Frostwolf Captain', '', 71, 71, 180000, 200000, 0, 0, 0, 1214, 1214, 0, 2, 1, 3, 4000, 4000, 0, 8000, 2000, 0, 4160, 0, 0, 0, 0, 0, 0, 172.1, 240.07, 100, 7, 4, 11947, 11947, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24055, 31418, '', 1, 3, 0, 1, 1389, 80428891, 1, 'mob_eventai');

REPLACE INTO creature_template
   (`entry`, `heroic_entry`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `baseattacktime`, `rangeattacktime`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`)
VALUES
   (11948, 0, 11896, 0, 11896, 0, 'Vanndar Stormpike', 'Stormpike General', '', 73, 73, 220000, 250000, 0, 0, 0, 1216, 1216, 0, 2, 1, 3, 5000, 8000, 0, 15000, 1158, 1274, 0, 0, 0, 0, 0, 0, 0, 203.77, 289.86, 100, 7, 4, 11948, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 1, 3, 0, 1, 0, 80428891, 1, 'mob_eventai');

REPLACE INTO creature_template
   (`entry`, `heroic_entry`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `baseattacktime`, `rangeattacktime`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`)
VALUES
   (11949, 0, 12823, 0, 12823, 0, 'Captain Balinda Stonehearth', 'Stormpike Captain', '', 71, 71, 180000, 200000, 38772, 38772, 0, 1216, 1216, 0, 2, 1, 3, 1000, 2000, 0, 2000, 2000, 0, 4096, 0, 0, 0, 0, 0, 0, 172.1, 240.07, 100, 7, 4, 11949, 11949, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19805, 25868, '', 1, 3, 0, 1, 1364, 80428891, 1, 'mob_eventai');

REPLACE INTO creature_template
   (`entry`, `heroic_entry`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `baseattacktime`, `rangeattacktime`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`)
VALUES
   (13358, 0, 13390, 0, 13393, 0, 'Stormpike Bowman', '', '', 69, 70, 4500, 10000, 0, 0, 0, 1216, 1216, 0, 1.27, 1, 0, 65, 135, 0, 696, 2000, 0, 4608, 0, 0, 0, 0, 0, 0, 200, 600, 1000, 7, 0, 13358, 13358, 0, 0, 0, 0, 0, 0, 0, 22121, 0, 0, 0, 0, 260, 344, '', 0, 3, 0, 1, 8, 0, 0, 'mob_eventai');

REPLACE INTO creature_template
   (`entry`, `heroic_entry`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `baseattacktime`, `rangeattacktime`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`)
VALUES
   (13359, 0, 13398, 0, 13401, 0, 'Frostwolf Bowman', '', '', 69, 70, 4500, 10000, 0, 0, 0, 1214, 1214, 0, 1.27, 1, 0, 66, 137, 0, 712, 2000, 0, 4608, 0, 0, 0, 0, 0, 0, 200, 600, 1000, 7, 0, 13359, 13359, 0, 0, 0, 0, 0, 0, 0, 22121, 0, 0, 0, 0, 262, 347, '', 0, 3, 0, 1, 6, 0, 0, 'mob_eventai');

REPLACE INTO creature_template
   (`entry`, `heroic_entry`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `baseattacktime`, `rangeattacktime`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`)
VALUES
   (14770, 0, 14797, 0, 14798, 0, 'Dun Baldar North Warmaster', '', '', 70, 70, 139780, 139780, 0, 0, 0, 1214, 1214, 0, 2, 1, 3, 3000, 5000, 0, 2175, 2000, 0, 4096, 0, 0, 0, 0, 0, 0, 62.016, 85.272, 100, 7, 0, 14770, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 0, 1, 1416, 0, 0, 'mob_eventai');

REPLACE INTO creature_template
   (`entry`, `heroic_entry`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `baseattacktime`, `rangeattacktime`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`)
VALUES
   (14771, 0, 14798, 0, 14799, 0, 'Dun Baldar South Warmaster', '', '', 70, 70, 36600, 36600, 0, 0, 0, 1214, 1214, 0, 2, 1, 3, 3000, 6000, 0, 10000, 2000, 0, 4096, 0, 0, 0, 0, 0, 0, 62.016, 85.272, 100, 7, 0, 14771, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 0, 1, 1416, 0, 0, 'mob_eventai');

REPLACE INTO creature_template
   (`entry`, `heroic_entry`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `baseattacktime`, `rangeattacktime`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`)
VALUES
   (14772, 0, 14798, 0, 14800, 0, 'East Frostwolf Warmaster', '', '', 70, 70, 139362, 139362, 0, 0, 0, 1214, 1214, 0, 2, 1, 3, 3000, 6000, 0, 10000, 2000, 0, 4096, 0, 0, 0, 0, 0, 0, 59.9488, 82.4296, 100, 7, 0, 14772, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6049, 7904, '', 1, 3, 0, 1, 1416, 0, 0, 'mob_eventai');

REPLACE INTO creature_template
   (`entry`, `heroic_entry`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `baseattacktime`, `rangeattacktime`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`)
VALUES
   (14773, 0, 14797, 0, 14797, 0, 'Iceblood Warmaster', '', '', 70, 70, 141144, 141144, 0, 0, 0, 1214, 1214, 0, 2, 1, 3, 3000, 6000, 0, 10000, 2000, 0, 4096, 0, 0, 0, 0, 0, 0, 62.016, 85.272, 100, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 1, 3, 0, 1, 1416, 0, 0, 'mob_eventai');

REPLACE INTO creature_template
   (`entry`, `heroic_entry`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `baseattacktime`, `rangeattacktime`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`)
VALUES
   (14774, 0, 14800, 0, 14800, 0, 'Icewing Warmaster', '', '', 70, 70, 122300, 122300, 0, 0, 0, 1214, 1214, 0, 2, 1, 3, 3000, 6000, 0, 10000, 2000, 0, 4096, 0, 0, 0, 0, 0, 0, 62.016, 85.272, 100, 7, 0, 14774, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6049, 7904, '', 1, 3, 0, 1, 1416, 0, 0, 'mob_eventai');

REPLACE INTO creature_template
   (`entry`, `heroic_entry`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `baseattacktime`, `rangeattacktime`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`)
VALUES
   (14775, 0, 14798, 0, 14800, 0, 'Stonehearth Warmaster', '', '', 70, 70, 140204, 140204, 0, 0, 0, 1214, 1214, 0, 2, 1, 3, 3000, 6000, 0, 10000, 2000, 0, 4096, 0, 0, 0, 0, 0, 0, 62.016, 85.272, 100, 7, 0, 14775, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6049, 7904, '', 1, 3, 0, 1, 1416, 0, 0, 'mob_eventai');

REPLACE INTO creature_template
   (`entry`, `heroic_entry`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `baseattacktime`, `rangeattacktime`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`)
VALUES
   (14776, 0, 14798, 0, 14800, 0, 'Tower Point Warmaster', '', '', 70, 70, 131536, 131536, 0, 0, 0, 1214, 1214, 0, 2, 1, 3, 3000, 6000, 0, 10000, 2000, 0, 4096, 0, 0, 0, 0, 0, 0, 62.016, 85.272, 100, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 1, 3, 0, 1, 1416, 0, 0, 'mob_eventai');

REPLACE INTO creature_template
   (`entry`, `heroic_entry`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `baseattacktime`, `rangeattacktime`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`)
VALUES
   (14777, 0, 14797, 0, 14798, 0, 'West Frostwolf Warmaster', '', '', 70, 70, 138920, 138920, 0, 0, 0, 1214, 1214, 0, 2, 1, 3, 3000, 6000, 0, 10000, 2000, 0, 4096, 0, 0, 0, 0, 0, 0, 59.9488, 82.4296, 100, 7, 0, 14777, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 1, 3, 0, 1, 1416, 0, 0, 'mob_eventai');

REPLACE INTO creature_template
   (`entry`, `heroic_entry`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `baseattacktime`, `rangeattacktime`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`)
VALUES
   (25040, 0, 4606, 0, 4606, 0, 'Greater Water Elemental', '', '', 1, 1, 1, 1, 0, 0, 0, 35, 35, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 0, 1, 0, 0, 0, 'mob_eventai');


UPDATE creature_template SET minlevel=50 , maxlevel=51 WHERE entry=10981;
UPDATE creature_template SET minlevel=50 , maxlevel=51 WHERE entry=10990;
UPDATE creature_template SET minlevel=52 , maxlevel=53 WHERE entry=10986;
UPDATE creature_template SET minlevel=52 , maxlevel=53 WHERE entry=11678;
UPDATE creature_template SET minlevel=53 , maxlevel=53 WHERE entry=11675;
UPDATE creature_template SET minlevel=53 , maxlevel=54 WHERE entry=14282;
UPDATE creature_template SET minlevel=53 , maxlevel=54 WHERE entry=14283;
UPDATE creature_template SET minlevel=55 , maxlevel=55 WHERE entry=10364;
UPDATE creature_template SET minlevel=55 , maxlevel=55 WHERE entry=10367;
UPDATE creature_template SET minlevel=55 , maxlevel=55 WHERE entry=12096;
UPDATE creature_template SET minlevel=55 , maxlevel=55 WHERE entry=12097;
UPDATE creature_template SET minlevel=55 , maxlevel=55 WHERE entry=2225;
UPDATE creature_template SET minlevel=55 , maxlevel=55 WHERE entry=3343;
UPDATE creature_template SET minlevel=55 , maxlevel=55 WHERE entry=3625;
UPDATE creature_template SET minlevel=55 , maxlevel=55 WHERE entry=4255;
UPDATE creature_template SET minlevel=55 , maxlevel=55 WHERE entry=4257;
UPDATE creature_template SET minlevel=55 , maxlevel=55 WHERE entry=5134;
UPDATE creature_template SET minlevel=55 , maxlevel=55 WHERE entry=5135;
UPDATE creature_template SET minlevel=55 , maxlevel=55 WHERE entry=5139;
UPDATE creature_template SET minlevel=56 , maxlevel=56 WHERE entry=11839;
UPDATE creature_template SET minlevel=57 , maxlevel=57 WHERE entry=12051;
UPDATE creature_template SET minlevel=57 , maxlevel=57 WHERE entry=12127;
UPDATE creature_template SET minlevel=58 , maxlevel=58 WHERE entry=13216;
UPDATE creature_template SET minlevel=58 , maxlevel=58 WHERE entry=13218;
UPDATE creature_template SET minlevel=58 , maxlevel=58 WHERE entry=13284;
UPDATE creature_template SET minlevel=58 , maxlevel=58 WHERE entry=13438;
UPDATE creature_template SET minlevel=58 , maxlevel=58 WHERE entry=13447;
UPDATE creature_template SET minlevel=59 , maxlevel=59 WHERE entry=13179;
UPDATE creature_template SET minlevel=59 , maxlevel=59 WHERE entry=14185;
UPDATE creature_template SET minlevel=59 , maxlevel=59 WHERE entry=14188;
UPDATE creature_template SET minlevel=60 , maxlevel=60 WHERE entry=11997;
UPDATE creature_template SET minlevel=60 , maxlevel=60 WHERE entry=13176;
UPDATE creature_template SET minlevel=60 , maxlevel=60 WHERE entry=13236;
UPDATE creature_template SET minlevel=60 , maxlevel=60 WHERE entry=13257;
UPDATE creature_template SET minlevel=60 , maxlevel=60 WHERE entry=13442;
UPDATE creature_template SET minlevel=60 , maxlevel=60 WHERE entry=13443;
UPDATE creature_template SET minlevel=60 , maxlevel=60 WHERE entry=13577;
UPDATE creature_template SET minlevel=60 , maxlevel=60 WHERE entry=13617;
UPDATE creature_template SET minlevel=60 , maxlevel=60 WHERE entry=14186;
UPDATE creature_template SET minlevel=60 , maxlevel=60 WHERE entry=14187;
UPDATE creature_template SET minlevel=60 , maxlevel=61 WHERE entry=13797;
UPDATE creature_template SET minlevel=60 , maxlevel=61 WHERE entry=13798;
UPDATE creature_template SET minlevel=61 , maxlevel=61 WHERE entry=11947;
UPDATE creature_template SET minlevel=61 , maxlevel=61 WHERE entry=11949;
UPDATE creature_template SET minlevel=61 , maxlevel=61 WHERE entry=13816;
UPDATE creature_template SET minlevel=61 , maxlevel=61 WHERE entry=14284;
UPDATE creature_template SET minlevel=63 , maxlevel=63 WHERE entry=11946;
UPDATE creature_template SET minlevel=63 , maxlevel=63 WHERE entry=11948;

UPDATE creature_template SET minlevel=54 , maxlevel=54 WHERE entry IN (10987,11603,13317,13396,13397,13316);
UPDATE creature_template SET minlevel=54 , maxlevel=55 WHERE entry IN (11600,13080,13099,11604,13096,13097);
UPDATE creature_template SET minlevel=54 , maxlevel=55 WHERE entry IN (11602,13098,13081,11605,13087,13089);
UPDATE creature_template SET minlevel=58 , maxlevel=58 WHERE entry IN (11657,13078,13079,11677,13086,13088);
UPDATE creature_template SET minlevel=54 , maxlevel=55 WHERE entry=10982;

-- following gameobjects are static
DELETE FROM gameobject WHERE map=30;
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (2061,30,-1423.16,-318.436,89.1136,2.35619,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (2061,30,-1290.27,-172.471,72.1853,3.06305,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (2061,30,-1286.25,-184.48,71.8334,-2.05076,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (2061,30,-1282.21,-284.083,87.256,-0.0610855,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (2061,30,-1280.17,-220.537,72.2686,1.62316,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (2061,30,-1256.55,-280.278,73.9473,-0.0610855,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (2061,30,-1241.25,-345.115,59.6867,0.357794,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (2061,30,-1215.59,-371.946,56.5293,0.357794,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (2061,30,-1202.8,-271.599,72.5805,0.357794,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (2061,30,-1140.82,-343.392,50.9077,-2.7838,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (2061,30,-1139.68,-356.288,51.264,-0.706858,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (2061,30,-1099.21,-266.231,57.8849,-2.28638,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (2061,30,-1082.6,-266.681,57.8575,2.40855,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (2061,30,30.4168,-428.853,41.528,-2.59181,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (2061,30,31.2216,-428.08,41.528,0.549779,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (2061,30,50.6401,-421.166,44.7325,-0.00875192,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (2066,30,-743.427,-398.242,76.4266,0.872664,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (3832,30,-1235.57,-241.478,73.4377,1.48353,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (3833,30,-1248.85,-254.06,73.4377,1.48353,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (3834,30,-1248.15,-245.599,73.4377,-0.0523605,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (3835,30,-1237.21,-260.168,73.4377,1.48353,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (3836,30,-1260.37,-248.767,77.9454,1.48353,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (3837,30,-1249.32,-244.907,92.3372,0.401426,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (3838,30,-1250.09,-254.604,92.3015,0.148352,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (22205,30,50.6401,-421.166,44.7325,-0.00875192,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (22207,30,30.4168,-428.853,41.528,-2.59181,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (22208,30,31.2216,-428.08,41.528,0.549779,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (28048,30,41.1672,-426.866,44.6828,2.7838,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (28049,30,39.0988,-425.746,44.688,-0.619592,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (28605,30,25.2482,-433.104,47.6369,2.38237,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (28606,30,25.758,-425.837,47.6369,-1.98095,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (28607,30,27.6786,-427.69,47.6369,-2.67908,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (29784,30,-1338.28,-297.628,91.4867,0.248971,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (31442,30,-1338.28,-297.628,91.4867,0.248971,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (50984,30,-1250.39,-310.191,61.185,-1.10828,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (51704,30,-1245.2,-307.059,63.3199,3.07959,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (51705,30,-1258.23,-310.977,63.2015,0.862906,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (177261,30,-1290.27,-172.471,72.1853,3.06305,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (177262,30,-1286.25,-184.48,71.8334,-2.05076,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (177263,30,-1280.17,-220.537,72.2686,1.62316,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (177292,30,-1099.21,-266.231,57.8849,-2.28638,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (177293,30,-1082.6,-266.681,57.8575,2.40855,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (177405,30,-1140.82,-343.392,50.9077,-2.7838,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (177406,30,-1139.68,-356.288,51.264,-0.706858,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (177408,30,-743.427,-398.242,76.4266,0.872664,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (177409,30,-1215.59,-371.946,56.5293,0.357794,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (177410,30,-1241.25,-345.115,59.6867,0.357794,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (177411,30,-1202.8,-271.599,72.5805,0.357794,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (177412,30,-1282.21,-284.083,87.256,-0.0610855,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (177413,30,-1256.55,-280.278,73.9473,-0.0610855,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178684,30,649.265,-59.1102,41.5476,-2.68781,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178685,30,646.207,-57.2428,41.6587,-0.157079,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179384,30,-145.341,-444.846,26.4163,-0.0523596,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179384,30,560.834,-75.4266,37.9558,0.785398,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179385,30,-155.405,-440.24,33.2862,2.34747,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179385,30,550.678,-79.8234,44.8257,-3.09796,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179386,30,-150.787,-459.829,26.4163,0.558507,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179386,30,568.326,-89.4992,37.9558,1.39626,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179387,30,-153.748,-438.639,33.2862,-2.88852,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179387,30,550.597,-77.5213,44.8257,-2.05076,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179388,30,-149.057,-461.089,26.4163,1.38754,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179388,30,570.419,-89.0567,37.9558,2.2253,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179389,30,-168.342,-458.4,33.2862,-0.445059,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179389,30,555.517,-101.589,44.8257,0.392699,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179390,30,-142.968,-444.076,26.4163,-2.23402,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179390,30,561.851,-73.1481,37.9558,-1.39626,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179391,30,-172.363,-452.824,33.2796,0.62832,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179391,30,548.682,-100.846,44.8191,1.46608,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179392,30,-171.282,-456.892,33.2796,1.63188,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179392,30,552.429,-102.764,44.8191,2.46964,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179393,30,-172.356,-453.88,33.2796,0.322886,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179393,30,549.472,-101.547,44.8191,1.16064,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179394,30,-171.882,-454.632,33.2796,0.759218,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179394,30,550.347,-101.698,44.8191,1.59698,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179395,30,-171.656,-455.671,33.2796,0.0610861,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179395,30,551.271,-102.226,44.8191,0.898843,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179396,30,-170.699,-458.41,33.2796,-1.02974,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179396,30,553.947,-103.347,44.8191,-0.191986,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179397,30,-170.14,-457.609,33.2796,-0.680678,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179397,30,553.725,-102.396,44.8191,0.15708,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179419,30,-1423.16,-318.436,89.1136,2.35619,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179437,30,618.748,-52.1126,42.1122,-0.0698131,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179438,30,-1181.1,-370.747,53.6246,2.68781,60);

-- nest 2 gos were inserted by hand..
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179024,30,40.0913,0.0153897,-4.0962,0.96131,120);
INSERT INTO gameobject (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179025,30,-1552.55, -359.738, 66.948,2.8875,120);

-- irondeep supplies
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178785,30,870.899,-388.434,61.6406,-1.22173,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178785,30,825.214,-320.174,63.712,-2.82743,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178785,30,837.117,-452.556,47.2331,-3.12414,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178785,30,869.755,-448.867,52.5448,-0.855212,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178785,30,949.877,-458.198,56.4874,0.314159,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178785,30,900.35,-479.024,58.3553,0.122173,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178785,30,854.449,-442.255,50.6589,0.401426,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178785,30,886.685,-442.358,54.6962,-1.22173,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178785,30,817.509,-457.331,48.4666,2.07694,60);

-- coldtooth supplies
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178784,30,-934.212,-57.3517,80.277,-0.0174535,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178784,30,-916.281,-36.8579,77.0227,0.122173,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178784,30,-902.73,-103.868,75.4378,-1.58825,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178784,30,-900.514,-143.527,75.9686,1.8675,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178784,30,-862.882,-0.353299,72.1526,-2.51327,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178784,30,-854.932,-85.9184,68.6056,-2.04204,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178784,30,-851.833,-118.959,63.8672,-0.0698131,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178784,30,-849.832,-20.8421,70.4672,-1.81514,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178784,30,-844.25,-60.0374,72.1031,-2.19912,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178784,30,-820.644,-136.043,63.1977,2.40855,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178784,30,-947.642,-208.807,77.0101,1.36136,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (178784,30,-951.394,-193.695,67.634,0.802851,60);

-- snowfall eyecandy:
-- alliance
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179044,30,-191.153,-129.868,78.5595,-1.25664,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179044,30,-201.282,-134.319,78.6753,-0.942478,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179044,30,-215.981,-91.4101,80.8702,-1.74533,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179044,30,-200.465,-96.418,79.7587,1.36136,60);
-- alliance assault
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179424,30,-191.153,-129.868,78.5595,-1.25664,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179424,30,-201.282,-134.319,78.6753,-0.942478,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179424,30,-215.981,-91.4101,80.8702,-1.74533,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179424,30,-200.465,-96.418,79.7587,1.36136,60);
-- horde
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179064,30,-191.153,-129.868,78.5595,-1.25664,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179064,30,-201.282,-134.319,78.6753,-0.942478,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179064,30,-215.981,-91.4101,80.8702,-1.74533,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179064,30,-200.465,-96.418,79.7587,1.36136,60);
-- horde assault
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179425,30,-191.153,-129.868,78.5595,-1.25664,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179425,30,-201.282,-134.319,78.6753,-0.942478,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179425,30,-215.981,-91.4101,80.8702,-1.74533,60);
INSERT INTO `gameobject` (`id` ,`map`,position_x,position_y,position_z,orientation,spawntimesecs) VALUES (179425,30,-200.465,-96.418,79.7587,1.36136,60);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,971.671, -442.657, 57.6951, 3.1765);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,969.979, -457.148, 58.1119, 4.5204);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,958.692, -333.477, 63.2276, 5.77704);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,957.113, -325.92, 61.7589, 1.13446);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,948.25, -448.268, 56.9009, 5.60251);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,934.727, -385.802, 63.0344, 3.75246);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,931.751, -403.458, 59.6737, 5.63741);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,931.146, -359.666, 66.0294, 3.9619);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,929.702, -412.401, 56.8776, 5.89921);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,926.849, -379.074, 63.5286, 2.0944);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,921.972, -358.597, 66.4313, 2.93215);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,921.449, -341.981, 67.1264, 3.4383);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,921.1, -395.812, 60.4615, 2.71695);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,919.274, -394.986, 60.3478, 2.71696);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,916.852, -393.891, 60.1726, 2.71695);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,914.568, -326.21, 66.1733, 2.25147);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,913.064, -395.773, 60.1364, 4.41568);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,909.246, -474.576, 58.2067, 0.226893);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,909.246, -474.576, 58.2901, 0.226893);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,907.209, -428.267, 59.8065, 1.8675);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,905.973, -459.528, 58.7594, 1.37189);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,905.067, -396.074, 60.2085, 5.07891);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,901.809, -457.709, 59.0116, 3.52557);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,900.962, -427.44, 59.0842, 1.50098);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,897.929, -471.742, 59.7729, 2.54818);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,893.376, -343.171, 68.1499, 5.35816);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,890.584, -406.049, 61.1925, 5.67232);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,888.208, -332.564, 68.148, 1.93732);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,887.647, -391.537, 61.8734, 1.37881);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,885.109, -343.338, 67.0867, 3.78979);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,881.618, -419.948, 53.5228, 0.593412);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,878.675, -345.36, 66.1052, 3.45651);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,877.127, -351.8, 66.5296, 5.74213);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,876.778, -345.97, 65.7724, 3.45262);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,874.577, -414.786, 52.7817, 1.67552);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,868.247, -343.136, 64.9894, 1.6057);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,859.03, -367.231, 47.4655, 0.0174533);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,857.513, -351.817, 65.1867, 4.39823);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,852.632, -372.416, 48.1657, 3.66519);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,849.86, -340.944, 66.2447, 0.401426);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,847.99, -386.287, 60.9277, 2.32374);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,847.601, -423.072, 50.0852, 4.57276);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,847.135, -411.307, 50.2106, 1.5708);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,835.077, -379.418, 48.2755, 5.93412);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,834.87, -453.304, 47.9075, 0.226893);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,834.634, -365.981, 62.8801, 1.32645);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,834.354, -355.526, 48.1491, 6.07375);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,833.702, -327.506, 65.0439, 0.331613);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,833.151, -374.228, 63.0938, 3.66519);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(10987,30,831.711, -346.785, 47.2975, 0.226893);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,971.671, -442.657, 57.6951, 3.1765);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,969.979, -457.148, 58.1119, 4.5204);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,958.692, -333.477, 63.2276, 5.77704);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,957.113, -325.92, 61.7589, 1.13446);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,948.25, -448.268, 56.9009, 5.60251);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,934.727, -385.802, 63.0344, 3.75246);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,931.751, -403.458, 59.6737, 5.63741);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,931.146, -359.666, 66.0294, 3.9619);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,929.702, -412.401, 56.8776, 5.89921);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,926.849, -379.074, 63.5286, 2.0944);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,921.972, -358.597, 66.4313, 2.93215);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,921.449, -341.981, 67.1264, 3.4383);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,921.1, -395.812, 60.4615, 2.71695);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,919.274, -394.986, 60.3478, 2.71696);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,916.852, -393.891, 60.1726, 2.71695);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,914.568, -326.21, 66.1733, 2.25147);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,913.064, -395.773, 60.1364, 4.41568);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,909.246, -474.576, 58.2067, 0.226893);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,909.246, -474.576, 58.2901, 0.226893);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,907.209, -428.267, 59.8065, 1.8675);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,905.973, -459.528, 58.7594, 1.37189);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,905.067, -396.074, 60.2085, 5.07891);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,901.809, -457.709, 59.0116, 3.52557);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,900.962, -427.44, 59.0842, 1.50098);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,897.929, -471.742, 59.7729, 2.54818);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,893.376, -343.171, 68.1499, 5.35816);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,890.584, -406.049, 61.1925, 5.67232);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,888.208, -332.564, 68.148, 1.93732);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,887.647, -391.537, 61.8734, 1.37881);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,885.109, -343.338, 67.0867, 3.78979);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,881.618, -419.948, 53.5228, 0.593412);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,878.675, -345.36, 66.1052, 3.45651);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,877.127, -351.8, 66.5296, 5.74213);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,876.778, -345.97, 65.7724, 3.45262);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,874.577, -414.786, 52.7817, 1.67552);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,868.247, -343.136, 64.9894, 1.6057);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,859.03, -367.231, 47.4655, 0.0174533);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,857.513, -351.817, 65.1867, 4.39823);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,852.632, -372.416, 48.1657, 3.66519);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,849.86, -340.944, 66.2447, 0.401426);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,847.99, -386.287, 60.9277, 2.32374);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,847.601, -423.072, 50.0852, 4.57276);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,847.135, -411.307, 50.2106, 1.5708);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,835.077, -379.418, 48.2755, 5.93412);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,834.87, -453.304, 47.9075, 0.226893);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,834.634, -365.981, 62.8801, 1.32645);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,834.354, -355.526, 48.1491, 6.07375);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,833.702, -327.506, 65.0439, 0.331613);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,833.151, -374.228, 63.0938, 3.66519);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13396,30,831.711, -346.785, 47.2975, 0.226893);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,971.671, -442.657, 57.6951, 3.1765);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,969.979, -457.148, 58.1119, 4.5204);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,958.692, -333.477, 63.2276, 5.77704);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,957.113, -325.92, 61.7589, 1.13446);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,948.25, -448.268, 56.9009, 5.60251);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,934.727, -385.802, 63.0344, 3.75246);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,931.751, -403.458, 59.6737, 5.63741);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,931.146, -359.666, 66.0294, 3.9619);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,929.702, -412.401, 56.8776, 5.89921);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,926.849, -379.074, 63.5286, 2.0944);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,921.972, -358.597, 66.4313, 2.93215);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,921.449, -341.981, 67.1264, 3.4383);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,921.1, -395.812, 60.4615, 2.71695);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,919.274, -394.986, 60.3478, 2.71696);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,916.852, -393.891, 60.1726, 2.71695);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,914.568, -326.21, 66.1733, 2.25147);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,913.064, -395.773, 60.1364, 4.41568);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,909.246, -474.576, 58.2067, 0.226893);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,909.246, -474.576, 58.2901, 0.226893);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,907.209, -428.267, 59.8065, 1.8675);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,905.973, -459.528, 58.7594, 1.37189);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,905.067, -396.074, 60.2085, 5.07891);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,901.809, -457.709, 59.0116, 3.52557);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,900.962, -427.44, 59.0842, 1.50098);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,897.929, -471.742, 59.7729, 2.54818);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,893.376, -343.171, 68.1499, 5.35816);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,890.584, -406.049, 61.1925, 5.67232);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,888.208, -332.564, 68.148, 1.93732);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,887.647, -391.537, 61.8734, 1.37881);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,885.109, -343.338, 67.0867, 3.78979);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,881.618, -419.948, 53.5228, 0.593412);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,878.675, -345.36, 66.1052, 3.45651);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,877.127, -351.8, 66.5296, 5.74213);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,876.778, -345.97, 65.7724, 3.45262);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,874.577, -414.786, 52.7817, 1.67552);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,868.247, -343.136, 64.9894, 1.6057);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,859.03, -367.231, 47.4655, 0.0174533);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,857.513, -351.817, 65.1867, 4.39823);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,852.632, -372.416, 48.1657, 3.66519);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,849.86, -340.944, 66.2447, 0.401426);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,847.99, -386.287, 60.9277, 2.32374);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,847.601, -423.072, 50.0852, 4.57276);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,847.135, -411.307, 50.2106, 1.5708);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,835.077, -379.418, 48.2755, 5.93412);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,834.87, -453.304, 47.9075, 0.226893);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,834.634, -365.981, 62.8801, 1.32645);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,834.354, -355.526, 48.1491, 6.07375);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,833.702, -327.506, 65.0439, 0.331613);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,833.151, -374.228, 63.0938, 3.66519);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13397,30,831.711, -346.785, 47.2975, 0.226893);


-- following are shaman and skullthumber, but i only spawn the first on, this
-- can later be done with creature-pools
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,955.812, -440.302, 55.3411, 3.19395);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,937.378, -377.816, 65.3919, 3.56047);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,925.059, -331.347, 65.7564, 3.66519);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,922.918, -396.634, 60.3942, 2.71695);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,909.99, -462.154, 59.0811, 3.7001);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,907.893, -388.787, 61.7923, 5.74213);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,898.801, -437.105, 58.5266, 0.959931);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,884.237, -407.597, 61.566, 0.820305);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,880.744, -344.683, 66.4086, 3.4644);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,876.047, -341.857, 65.8743, 4.45059);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,874.674, -402.077, 61.7573, 0.26341);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,871.914, -404.209, 62.1269, 6.06163);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,871.606, -403.665, 62.0795, 0.765774);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,871.561, -404.114, 62.1297, 0.00981727);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,871.528, -404.248, 62.1455, 0.498032);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,871.493, -404.122, 62.1331, 5.65727);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,871.282, -403.843, 62.1108, 0.788382);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,868.294, -392.395, 61.4772, 4.38685);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,868.256, -392.363, 61.4803, 0.732738);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,867.804, -392.51, 61.5089, 2.30167);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,867.612, -392.371, 61.524, 2.86149);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,858.593, -439.614, 50.2184, 0.872665);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,851.471, -362.52, 47.314, 4.06662);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,846.939, -347.279, 66.2876, 0.942478);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,842.08, -421.775, 48.2659, 1.0821);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,838.358, -371.212, 63.3299, 4.04916);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,827.57, -417.483, 48.4538, 1.49237);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,827.012, -457.397, 48.9331, 2.35619);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,825.535, -322.373, 63.9357, 4.76475);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,867.635, -443.605, 51.3347, 1.38626);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,957.293, -455.039, 56.7395, 5.79449);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,950.077, -326.672, 61.6552, 5.48033);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,936.692, -356.78, 65.9835, 2.75762);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,926.475, -419.345, 56.1833, 2.0944);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,924.729, -397.453, 60.213, 2.71695);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,902.195, -475.891, 58.312, 1.39626);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,897.464, -338.758, 68.1715, 2.94961);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,884.237, -407.597, 61.566, 0.820305);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,882.517, -344.111, 66.7887, 3.46962);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,881.437, -400.254, 61.2028, 0.263427);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,880.156, -400.678, 61.3113, 3.41373);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,877.989, -418.051, 52.9753, 4.46804);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,871.212, -404.12, 62.1433, 3.6554);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,871.036, -404.119, 62.2237, 4.50295);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,857.396, -395.766, 61.263, 4.78684);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,857.276, -395.395, 61.2418, 0.0845553);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,857.231, -394.577, 61.2174, 1.96817);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,857.108, -395.682, 61.2317, 4.87022);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,856.709, -395.28, 61.1814, 2.54913);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,850.922, -390.399, 60.8771, 2.85405);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,847.556, -388.228, 60.9438, 2.56872);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,842.031, -384.663, 61.6028, 2.56871);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,832.035, -389.301, 47.5567, 2.11185);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,827.415, -419.468, 48.3322, 1.49232);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,826.402, -349.454, 47.2722, 1.51844);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,817.83, -455.715, 48.4207, 0.925025);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11600,30,808.953, -325.964, 52.4043, 3.01942);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,955.812, -440.302, 55.3411, 3.19395);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,937.378, -377.816, 65.3919, 3.56047);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,925.059, -331.347, 65.7564, 3.66519);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,922.918, -396.634, 60.3942, 2.71695);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,909.99, -462.154, 59.0811, 3.7001);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,907.893, -388.787, 61.7923, 5.74213);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,898.801, -437.105, 58.5266, 0.959931);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,884.237, -407.597, 61.566, 0.820305);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,880.744, -344.683, 66.4086, 3.4644);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,876.047, -341.857, 65.8743, 4.45059);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,874.674, -402.077, 61.7573, 0.26341);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,871.914, -404.209, 62.1269, 6.06163);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,871.606, -403.665, 62.0795, 0.765774);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,871.561, -404.114, 62.1297, 0.00981727);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,871.528, -404.248, 62.1455, 0.498032);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,871.493, -404.122, 62.1331, 5.65727);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,871.282, -403.843, 62.1108, 0.788382);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,868.294, -392.395, 61.4772, 4.38685);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,868.256, -392.363, 61.4803, 0.732738);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,867.804, -392.51, 61.5089, 2.30167);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,867.612, -392.371, 61.524, 2.86149);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,858.593, -439.614, 50.2184, 0.872665);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,851.471, -362.52, 47.314, 4.06662);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,846.939, -347.279, 66.2876, 0.942478);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,842.08, -421.775, 48.2659, 1.0821);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,838.358, -371.212, 63.3299, 4.04916);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,827.57, -417.483, 48.4538, 1.49237);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,827.012, -457.397, 48.9331, 2.35619);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,825.535, -322.373, 63.9357, 4.76475);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,867.635, -443.605, 51.3347, 1.38626);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,957.293, -455.039, 56.7395, 5.79449);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,950.077, -326.672, 61.6552, 5.48033);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,936.692, -356.78, 65.9835, 2.75762);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,926.475, -419.345, 56.1833, 2.0944);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,924.729, -397.453, 60.213, 2.71695);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,902.195, -475.891, 58.312, 1.39626);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,897.464, -338.758, 68.1715, 2.94961);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,884.237, -407.597, 61.566, 0.820305);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,882.517, -344.111, 66.7887, 3.46962);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,881.437, -400.254, 61.2028, 0.263427);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,880.156, -400.678, 61.3113, 3.41373);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,877.989, -418.051, 52.9753, 4.46804);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,871.212, -404.12, 62.1433, 3.6554);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,871.036, -404.119, 62.2237, 4.50295);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,857.396, -395.766, 61.263, 4.78684);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,857.276, -395.395, 61.2418, 0.0845553);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,857.231, -394.577, 61.2174, 1.96817);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,857.108, -395.682, 61.2317, 4.87022);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,856.709, -395.28, 61.1814, 2.54913);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,850.922, -390.399, 60.8771, 2.85405);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,847.556, -388.228, 60.9438, 2.56872);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,842.031, -384.663, 61.6028, 2.56871);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,832.035, -389.301, 47.5567, 2.11185);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,827.415, -419.468, 48.3322, 1.49232);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,826.402, -349.454, 47.2722, 1.51844);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,817.83, -455.715, 48.4207, 0.925025);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13080,30,808.953, -325.964, 52.4043, 3.01942);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,955.812, -440.302, 55.3411, 3.19395);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,937.378, -377.816, 65.3919, 3.56047);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,925.059, -331.347, 65.7564, 3.66519);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,922.918, -396.634, 60.3942, 2.71695);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,909.99, -462.154, 59.0811, 3.7001);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,907.893, -388.787, 61.7923, 5.74213);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,898.801, -437.105, 58.5266, 0.959931);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,884.237, -407.597, 61.566, 0.820305);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,880.744, -344.683, 66.4086, 3.4644);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,876.047, -341.857, 65.8743, 4.45059);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,874.674, -402.077, 61.7573, 0.26341);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,871.914, -404.209, 62.1269, 6.06163);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,871.606, -403.665, 62.0795, 0.765774);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,871.561, -404.114, 62.1297, 0.00981727);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,871.528, -404.248, 62.1455, 0.498032);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,871.493, -404.122, 62.1331, 5.65727);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,871.282, -403.843, 62.1108, 0.788382);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,868.294, -392.395, 61.4772, 4.38685);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,868.256, -392.363, 61.4803, 0.732738);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,867.804, -392.51, 61.5089, 2.30167);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,867.612, -392.371, 61.524, 2.86149);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,858.593, -439.614, 50.2184, 0.872665);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,851.471, -362.52, 47.314, 4.06662);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,846.939, -347.279, 66.2876, 0.942478);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,842.08, -421.775, 48.2659, 1.0821);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,838.358, -371.212, 63.3299, 4.04916);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,827.57, -417.483, 48.4538, 1.49237);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,827.012, -457.397, 48.9331, 2.35619);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,825.535, -322.373, 63.9357, 4.76475);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,867.635, -443.605, 51.3347, 1.38626);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,957.293, -455.039, 56.7395, 5.79449);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,950.077, -326.672, 61.6552, 5.48033);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,936.692, -356.78, 65.9835, 2.75762);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,926.475, -419.345, 56.1833, 2.0944);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,924.729, -397.453, 60.213, 2.71695);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,902.195, -475.891, 58.312, 1.39626);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,897.464, -338.758, 68.1715, 2.94961);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,884.237, -407.597, 61.566, 0.820305);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,882.517, -344.111, 66.7887, 3.46962);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,881.437, -400.254, 61.2028, 0.263427);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,880.156, -400.678, 61.3113, 3.41373);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,877.989, -418.051, 52.9753, 4.46804);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,871.212, -404.12, 62.1433, 3.6554);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,871.036, -404.119, 62.2237, 4.50295);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,857.396, -395.766, 61.263, 4.78684);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,857.276, -395.395, 61.2418, 0.0845553);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,857.231, -394.577, 61.2174, 1.96817);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,857.108, -395.682, 61.2317, 4.87022);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,856.709, -395.28, 61.1814, 2.54913);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,850.922, -390.399, 60.8771, 2.85405);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,847.556, -388.228, 60.9438, 2.56872);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,842.031, -384.663, 61.6028, 2.56871);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,832.035, -389.301, 47.5567, 2.11185);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,827.415, -419.468, 48.3322, 1.49232);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,826.402, -349.454, 47.2722, 1.51844);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,817.83, -455.715, 48.4207, 0.925025);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13099,30,808.953, -325.964, 52.4043, 3.01942);


-- next are bosses
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(11657,30,865.554, -438.735, 50.7333, 2.12431);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13078,30,865.554, -438.735, 50.7333, 2.12431);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13079,30,865.554, -438.735, 50.7333, 2.12431);

UPDATE creature c, creature_template t SET c.curhealth=t.minhealth WHERE c.map=30 AND c.id=t.entry;
UPDATE creature SET spawndist=0 WHERE MovementType=0 AND map=30;
delete from mangos_string where entry>736 and entry<770;
INSERT INTO mangos_string VALUES
(747,'Alliance',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(748,'Horde',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(749,'%s was destroyed by the %s!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(750,'The %s is under attack! If left unchecked, the %s will destroy it!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(751,'The %s was taken by the %s!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(752,'The %s was taken by the %s!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(753,'The %s was taken by the %s!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(754,'The %s is under attack! If left unchecked, the %s will capture it!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(755,'The %s has taken the %s! Its supplies will now be used for reinforcements!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(756,'Irondeep Mine',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(757,'Coldtooth Mine',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(758,'Stormpike Aid Station',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(759,'Dun Baldar South Bunker',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(760,'Dun Baldar North Bunker',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(761,'Stormpike Graveyard',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(762,'Icewing Bunker',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(763,'Stonehearth Graveyard',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(764,'Stonehearth Bunker',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(765,'Snowfall Graveyard',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(766,'Iceblood Tower',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(767,'Iceblood Graveyard',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(768,'Tower Point',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(769,'Frostwolf Graveyard',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(770,'East Frostwolf Tower',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(771,'West Frostwolf Tower',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(772,'Frostwolf Relief Hut',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(773,'The Battle for Alterac Valley begins in 1 minute.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(774,'The Battle for Alterac Valley begins in 30 seconds. Prepare yourselves!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(775,'The Battle for Alterac Valley has begun!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(776,'The Alliance Team is running out of reinforcements!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(777,'The Horde Team is running out of reinforcements!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(778,'The Frostwolf General is Dead!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(779,'The Stormpike General is Dead!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL)
;

UPDATE mangos_string SET content_loc3='Allianz' WHERE entry=747;
UPDATE mangos_string SET content_loc3='Horde' WHERE entry=748;
UPDATE mangos_string SET content_loc3='%s wurde von der %s zerstört!' WHERE entry=749;
UPDATE mangos_string SET content_loc3='%s wird angegriffen! Wenn er unbewacht bleibt, wird die %s ihn zerstören!' WHERE entry=750;
-- Wenn _sie_ unbewacht bleibt wird allianz _sie_ TODO: don't store towers and gravenames, store complete strings with the names inside it, else i don't know how to implement this feature

UPDATE mangos_string SET content_loc3='%s wurde von der %s erobert!' WHERE entry=751;
UPDATE mangos_string SET content_loc3='%s wurde von der %s erobert!' WHERE entry=752;
UPDATE mangos_string SET content_loc3='%s wurde von der %s erobert!' WHERE entry=753;

UPDATE mangos_string SET content_loc3='%s wird angegriffen! Wenn er unbewacht bleibt, wird die %s ihn erobern!' WHERE entry=754;
-- Wenn _sie_ unbewacht bleibt ... TODO...

UPDATE mangos_string SET content_loc3='Die %s hat die %s eingenommen! Die dort befindlichen Vorräte werden zur Verstärkung eingesetzt!' WHERE entry=755;

UPDATE mangos_string SET content_loc3='Eisenschachtmine' WHERE entry=756;
UPDATE mangos_string SET content_loc3='Eisbeißermine' WHERE entry=757;
UPDATE mangos_string SET content_loc3='Das Lazarett der Sturmlanzen' WHERE entry=758;
UPDATE mangos_string SET content_loc3='Der Südbunker von Dun Baldar' WHERE entry=759;
UPDATE mangos_string SET content_loc3='Der Nordbunker von Dun Baldar' WHERE entry=760;
UPDATE mangos_string SET content_loc3='Der Friedhof der Sturmlanzen' WHERE entry=761;
UPDATE mangos_string SET content_loc3='Der Eisschwingenbunker' WHERE entry=762;
UPDATE mangos_string SET content_loc3='Der Steinbruchfriedhof' WHERE entry=763;
UPDATE mangos_string SET content_loc3='Der Steinbruchbunker' WHERE entry=764;
UPDATE mangos_string SET content_loc3='Der Schneewehenfriedhof' WHERE entry=765;
UPDATE mangos_string SET content_loc3='Der Eisblutturm' WHERE entry=766;
UPDATE mangos_string SET content_loc3='Der Eisblutfriedhof' WHERE entry=767;
UPDATE mangos_string SET content_loc3='Die Turmstellung' WHERE entry=768;
UPDATE mangos_string SET content_loc3='Der Friedhof der Frostwölfe' WHERE entry=769;
UPDATE mangos_string SET content_loc3='Der östliche Frostwolfturm' WHERE entry=770;
UPDATE mangos_string SET content_loc3='Der westliche Frostwolfturm' WHERE entry=771;
UPDATE mangos_string SET content_loc3='Die Heilerhütte der Frostwölfe' WHERE entry=772;
UPDATE mangos_string SET content_loc3='Der Kampf um das Alteractal beginnt in 1 Minute.' WHERE entry=773;
UPDATE mangos_string SET content_loc3='Der Kampf um das Alteractal beginnt in 30 Sekunden.' WHERE entry=774;
UPDATE mangos_string SET content_loc3='Der Kampf um das Alteractal hat begonnen!' WHERE entry=775;
UPDATE mangos_string SET content_loc3='Die Allianz hat nurnoch wenige Verstärkungspunkte' WHERE entry=776; -- TODO find right msg
UPDATE mangos_string SET content_loc3='Die Horde hat nurnoch wenige Verstärkungspunkte' WHERE entry=777; -- TODO same here
UPDATE mangos_string SET content_loc3='Der General der Allianz ist tot!' WHERE entry=778;
UPDATE mangos_string SET content_loc3='Der General der Horde ist tot!' WHERE entry=779;


-- CHAT_MSG_BG_SYSTEM_ALLIANCE', 'txt': '$N hat den Schneewehenfriedhof besetzt! Wenn niemand eingreift, erlangt die Allianz die Kontrolle \xc3\xbcber ihn
