// Delete Copse Scarabs
DELETE from `creature` where `id`= 16698;
UPDATE `creature_template` SET `mindmg` = 400, `maxdmg` = 600, `attackpower` = ROUND((`mindmg` + `maxdmg`) / 4 * 7), `mindmg` = ROUND(`mindmg` - `attackpower` / 7), `maxdmg` = ROUND(`maxdmg` - `attackpower` / 7) WHERE `entry` = 16698;
// Delete Gothik adds
delete from `creature` where `id`in (16124,16125,16126,16127,16148,16149,16150);
// Faction Gothik adds
update `creature_template` set faction_A = 21, faction_H = 21 where `entry` in(16124,16125,16126,16127,16148,16149,16150);
// Grobbulus Wolke
update `creature_template` set faction_A = 21, faction_H = 21 where `entry` in (16363);
// Razevous Damage
UPDATE `creature_template` SET 
`mindmg` = 9000, 
`maxdmg` = 11000, 
`attackpower` = ROUND((`mindmg` + `maxdmg`) / 4 * 7), 
`mindmg` = ROUND(`mindmg` - `attackpower` / 7), 
`maxdmg` = ROUND(`maxdmg` - `attackpower` / 7) 
WHERE `entry` = 16061;
// several mobs
delete from `creature` where `id` in (16983,16981,16982,16984);
update `creature_template` set faction_A = 21, faction_H = 21 where `entry` in (16983,16981,16982,16984); 
// Noth Skelletions
delete from `creature` where `id` in (16286);
update `creature_template` set faction_A = 21, faction_H = 21 where `entry` in (16286); -- Lortheb Spores
// Sapphie
update `creature_template` set unit_flags = 0 where entry = 15989;
GoDoors
// Spawns
// Delete existing Spawns
delete from gameobject where id in (181126,181195,181167,181235,181197,181209,181123, 181120,181121,181124,181125,18
170,181119,181200,181201,181202,181203,181241,1812 25,181228,181496);
insert into `gameobject` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) values
-- 181126 Anub'Rekhan Door
('333000','181126','533','3202.7','-3476.08','287.061','0','0','0','0','0','6','0','1' ),
-- 181195 Anub'Rekhan Gate
('333001','181195','533','3143.53','-3572.74','286','5.49778','0','0','0.382687','-0.923878','6','0','1'),
-- 181167 Grand Widow Faerlina Door
('333002','181167','533','3120.78','-3791','273.93','0','0','0','0','0','6','0','1'),
-- 181235 Grand Widow Faerlina - Web
('333003','181235','533','3322.53','-3695.4','259','3.14159','0','0','1','3.13916e-007','6','0','0'),
-- 181209 Maexxna - Outer Web Door 
('333004','181209','533','3425.68','-3846.1','309.109','3.14159','0','0','1','3.13916e-007','6','0','1'),
-- 181197 Maexxna - Inner Web Door
('333005','181197','533','3446.72','-3860.3','308.76','3.14159','0','0','0','0','6','0' ,'1'),
-- 181124 Vaccuum - Enter Gate
('333006','181124','533','2765.75','-3384.38','267.685','0','0','0','0','1','6','0','1' ),
-- 181125 Vaccuum - Exit Gate
('333007','181125','533','2617.74','-3336.81','267.684','0','0','0','0','1','25','0','1 '),
-- 181170 Vaccuum - Combat Gate
('333008','181170','533','2692.04','-3361.4','267.684','0','0','0','0','1','6','0','1') ,
-- 181119 Deathknight Door
('333009','181119','533','2587.76','-3017.16','240.5','3.14159','0','0','1','3.13916e-007','6','0','1'),
-- 181200 Noth - Entry Door
('333010','181200','533','2737.83','-3489.78','262.107','0','0','0','0','1','25','0','0 '),
-- 181201 Noth - Exit Door
('333011','181201','533','2684.37','-3559.83','261.944','1.57079','0','0','0.707105','0 .707109','25','0','1'),
-- 181202 Heigan the Unclean - Entry Door
('333012','181202','533','2823.47','-3685.07','274.079','0','0','0','0','1','25','0','0 '),
-- 181203 Heigan the Unclean - Exit Door
('333013','181203','533','2771.42','-3737.02','273.619','1.57079','0','0','0.707105','0 .707109','25','0','1'),
-- 181496 Heigan the Unclean - Exit Gate
('333020','181496','533','2909.82','-3817.88','273.926','0','0','0','0','1','6','0','1' ),
-- 181241 Loatheb - Entrance Door
('333014','181241','533','2909.73','-3947.75','273.924','0','0','0','0','1','6','0','1' ),
-- 181123 Patchwork - Exit Door
('333015','181123','533','3318.27','-3254.42','293.786','1.57079','0','0','0.707105','0 .707109','6','0','1'),
-- 181120 Gluth - Exit Door
('333016','181120','533','3338.63','-3101.36','297.0','3.14159','0','0','1','1.26759e-006','6','0','1'),
-- 181121 Thaddius Door
('333017','181121','533','3424.77','-3014.72','295.608','0','0','0','0','1','25','0','1 '),
-- 181225 Frostwyrm Waterfall Door
('333018','181225','533','3537.32','-5160.48','143.619','4.50667','0','0','0.775973','-0.630766','25','0','1'),
-- 181228 KelThuzad Door 
('333019','181228','533','3635.34','-5090.31','143.206','1.37','0','0','0.632673','0.77 4419','6','0','1');


// aendere size ... Tuer Passt nicht und man konnte vorher an den Rand vorbei
update gameobject_template set size = 1.2 where entry = 181201;


update gameobject_template set faction = 14 where entry in (181126,181195,181167,181235,181197,181209,181123, 181120,181121,181124,181125,18
170,181119,181200,181201,181202,181203,181241,1812 25,181228,181496,181366);</div>

// Naxxramas
update `creature_template` set `scriptname` = 'boss_anubrekhan' where `entry`= 15956;
update `creature_template` set `scriptname` = 'mob_crypt_guard' where `entry`= 16573;
update `creature_template` set `scriptname` = 'boss_faerlina' where `entry`= 15953;
update `creature_template` set `scriptname` = 'boss_maexxna' where `entry`= 15952;
update `creature_template` set `scriptname` = 'mob_webwrap' where `entry`= 16486;
update `creature_template` set `scriptname` = 'boss_patchwerk' where `entry`= 16028;
update `creature_template` set `scriptname` = 'boss_grobbulus' where `entry`= 15931;
update `creature_template` set `scriptname` = 'mob_grobbulus_poisoncloud' where `entry`= 16363;
update `creature_template` set `scriptname` = 'boss_gluth' where `entry`= 15932;
update `creature_template` set `scriptname` = 'boss_thaddius' where `entry`= 15928;
update `creature_template` set `scriptname` = 'mob_feugen' where `entry`= 15930;
update `creature_template` set `scriptname` = 'mob_stalagg' where `entry`= 15929;
update `creature_template` set `scriptname` = 'boss_razuvious' where `entry`= 16061;
update `creature_template` set `scriptname` = 'boss_gothik' where `entry`= 16060;
update `creature_template` set `scriptname` = 'mob_u_trainee' where `entry`= 16124;
update `creature_template` set `scriptname` = 'mob_u_deathknight' where `entry`= 16125;
update `creature_template` set `scriptname` = 'mob_u_rider' where `entry`= 16126;
update `creature_template` set `scriptname` = 'boss_thane_korthazz' where `entry`= 16064;
update `creature_template` set `scriptname` = 'boss_lady_blaumeux' where `entry`= 16065;
update `creature_template` set `scriptname` = 'boss_highlord_mograine' where `entry`= 16062;
update `creature_template` set `scriptname` = 'boss_sir_zeliek' where `entry`= 16063;
update `creature_template` set `scriptname` = 'boss_noth' where `entry`= 15954;
update `creature_template` set `scriptname` = 'boss_heigan' where `entry`= 15936;
update `creature_template` set `scriptname` = 'mob_eye_stalks' where `entry`= 16236;
update `creature_template` set `scriptname` = 'boss_loatheb' where `entry`= 16011;
update `creature_template` set `scriptname` = 'mob_loatheb_spores' where `entry`= 16286;
update `creature_template` set `scriptname` = 'boss_sapphiron' where `entry`= 15989;
update `creature_template` set `scriptname` = 'boss_kelthuzad' where `entry`= 15990;
update `instance_template` set `script` = 'instance_naxxramas' where `map`= 533;