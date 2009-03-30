    -- towers
    -- dun south - OK
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13358,30,569.395,-101.064,52.8296,2.34974);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13358,30,574.85,-92.9842,52.5869,3.09325);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13358,30,575.411,-83.597,52.3626,6.26573);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13358,30,571.352,-75.6582,52.479,0.523599);
INSERT INTO creature_battleground (guid,eventIndex) SELECT guid, 30 FROM creature ORDER BY guid DESC LIMIT 4;
    -- dun north - OK
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13358,30,668.60,-122.53,64.12,2.34);                        -- not 100% ok
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13358,30,662.253,-129.105,64.1794,2.77507);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13358,30,661.209,-138.877,64.2251,3.38594);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13358,30,665.481,-146.857,64.1271,3.75246);
INSERT INTO creature_battleground (guid,eventIndex) SELECT guid, 34 FROM creature ORDER BY guid DESC LIMIT 4;
    -- icewing - OK
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13358,30,225.228,-368.909,56.9983,6.23806);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13358,30,191.36,-369.899,57.1524,3.24631);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13358,30,215.518,-384.019,56.9889,5.09636);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13358,30,199.625,-382.177,56.8691,4.08407);
INSERT INTO creature_battleground (guid,eventIndex) SELECT guid, 38 FROM creature ORDER BY guid DESC LIMIT 4;
    -- stone
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13358,30,-172.851,-452.366,40.8725,3.31829);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13358,30,-147.147,-435.053,40.8022,0.599238);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13358,30,-169.456,-440.325,40.985,2.59101);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13358,30,-163.494,-434.904,41.0725,1.84174);
INSERT INTO creature_battleground (guid,eventIndex) SELECT guid, 42 FROM creature ORDER BY guid DESC LIMIT 4;
    -- ice - OK
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13359,30,-573.522,-271.854,75.0078,3.9619);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13359,30,-565.616,-269.051,74.9952,5.02655);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13359,30,-562.825,-261.087,74.9898,5.95157);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13359,30,-569.176,-254.446,74.8771,0.820305);
INSERT INTO creature_battleground (guid,eventIndex) SELECT guid, 48 FROM creature ORDER BY guid DESC LIMIT 4;

    -- towerpoint
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13359,30,-763.04,-371.032,90.7933,5.25979);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13359,30,-759.764,-358.264,90.8681,0.289795);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13359,30,-768.808,-353.056,90.8811,1.52601);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13359,30,-775.944,-362.639,90.8949,2.59573);
INSERT INTO creature_battleground (guid,eventIndex) SELECT guid, 52 FROM creature ORDER BY guid DESC LIMIT 4;
    -- frost etower
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13359,30,-1294.13,-313.045,107.328,0.270162);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13359,30,-1306.5,-308.105,113.767,1.78755);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13359,30,-1294.78,-319.966,113.79,5.94545);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13359,30,-1294.83,-312.241,113.799,0.295293);
INSERT INTO creature_battleground (guid,eventIndex) SELECT guid, 56 FROM creature ORDER BY guid DESC LIMIT 4;
    -- frost wtower
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13359,30,-1300.96,-275.111,114.058,4.12804);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13359,30,-1302.41,-259.256,114.065,1.67602);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13359,30,-1287.97,-262.087,114.165,6.18264);
INSERT INTO creature(id,map,position_x,position_y,position_z,orientation) VALUES(13359,30,-1291.59,-271.166,114.151,5.28257);
INSERT INTO creature_battleground (guid,eventIndex) SELECT guid, 60 FROM creature ORDER BY guid DESC LIMIT 4;
