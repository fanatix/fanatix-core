update `creature_template` set `ScriptName`='boss_alar' where `entry`='19514';
update `creature_template` set `ScriptName`='mob_ember_of_alar' where `entry`='19551';
update `creature_template` set `ScriptName`='mob_flame_patch_alar' where `entry`='20602';

REPLACE INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
('34269','550','422.083','23.279','20.1795','0'),
('34270','550','412.892','45.4676','20.1799','0'),
('34271','550','398.272','64.5214','20.1794','0'),
('34272','550','379.218','79.1419','19.8359','0'),
('34273','550','357.029','88.3328','20.0122','0'),
('34274','550','333.218','91.4676','20.3589','0'),
('34275','550','309.407','88.3328','20.1796','0'),
('34276','550','287.218','79.1419','20.1796','0'),
('34277','550','268.164','64.5214','20.1797','0'),
('34278','550','253.544','45.4676','20.1798','0'),
('34279','550','244.353','23.279','21.2296','0'),
('34280','550','241.218','-0.5324','27.1245','0'),
('34281','550','244.353','-24.3438','20.8583','0'),
('34282','550','253.544','-46.5324','20.1799','0'),
('34283','550','268.164','-65.5862','20.1798','0'),
('34284','550','287.218','-80.2067','20.1798','0'),
('34285','550','309.407','-89.3976','20.1796','0'),
('34286','550','333.218','-92.5324','20.3633','0'),
('34287','550','357.029','-89.3976','20.0277','0'),
('34288','550','379.218','-80.2067','19.8224','0'),
('34289','550','398.272','-65.5862','20.1794','0'),
('34314','550','412.892','-46.5324','20.1797','0'),
('34315','550','422.083','-24.3438','20.1794','0'),
('34316','550','425.218','-0.5324','20.1794','0');