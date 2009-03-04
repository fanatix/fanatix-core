/*
MySQL Data Transfer
Source Host: localhost
Source Database: world
Target Host: localhost
Target Database: world
Date: 04.02.2009 13:18:40
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for instance_template
-- ----------------------------
DROP TABLE IF EXISTS `instance_template`;
CREATE TABLE `instance_template` (
  `map` smallint(5) unsigned NOT NULL,
  `parent` int(10) unsigned NOT NULL,
  `levelMin` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `levelMax` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `maxPlayers` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `reset_delay` int(10) unsigned NOT NULL DEFAULT '0',
  `startLocX` float DEFAULT NULL,
  `startLocY` float DEFAULT NULL,
  `startLocZ` float DEFAULT NULL,
  `startLocO` float DEFAULT NULL,
  `script` varchar(128) NOT NULL DEFAULT '',
  PRIMARY KEY (`map`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `instance_template` VALUES ('33', '0', '14', '30', '10', '0', null, null, null, null, 'instance_shadowfang_keep');
INSERT INTO `instance_template` VALUES ('34', '0', '15', '32', '10', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('36', '0', '10', '20', '10', '0', null, null, null, null, 'instance_deadmines');
INSERT INTO `instance_template` VALUES ('43', '0', '10', '21', '10', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('47', '0', '17', '38', '10', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('48', '0', '19', '32', '10', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('70', '0', '30', '47', '10', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('90', '0', '15', '38', '10', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('109', '0', '35', '55', '10', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('129', '0', '25', '46', '10', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('189', '0', '20', '45', '10', '0', null, null, null, null, 'instance_scarlet_monastery');
INSERT INTO `instance_template` VALUES ('209', '0', '35', '54', '10', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('229', '0', '45', '0', '10', '0', '78', '-225', '49', '5', '');
INSERT INTO `instance_template` VALUES ('230', '0', '40', '0', '5', '0', null, null, null, null, 'instance_blackrock_depths');
INSERT INTO `instance_template` VALUES ('249', '0', '50', '0', '40', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('269', '0', '66', '0', '5', '0', null, null, null, null, 'instance_dark_portal');
INSERT INTO `instance_template` VALUES ('289', '0', '45', '0', '5', '0', null, null, null, null, 'instance_scholomance');
INSERT INTO `instance_template` VALUES ('309', '0', '50', '0', '20', '0', null, null, null, null, 'instance_zulgurub');
INSERT INTO `instance_template` VALUES ('329', '0', '45', '60', '5', '0', null, null, null, null, 'instance_stratholme');
INSERT INTO `instance_template` VALUES ('349', '0', '30', '55', '10', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('389', '0', '8', '18', '10', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('409', '230', '50', '0', '40', '0', null, null, null, null, 'instance_molten_core');
INSERT INTO `instance_template` VALUES ('429', '0', '45', '60', '5', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('469', '229', '60', '0', '40', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('509', '0', '50', '0', '20', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('531', '0', '50', '0', '40', '0', null, null, null, null, 'instance_temple_of_ahnqiraj');
INSERT INTO `instance_template` VALUES ('532', '0', '68', '0', '10', '0', null, null, null, null, 'instance_karazhan');
INSERT INTO `instance_template` VALUES ('533', '0', '80', '0', '10', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('534', '0', '70', '0', '25', '0', null, null, null, null, 'instance_hyjal');
INSERT INTO `instance_template` VALUES ('540', '0', '55', '0', '5', '0', null, null, null, null, 'instance_shattered_halls');
INSERT INTO `instance_template` VALUES ('542', '0', '55', '0', '5', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('543', '0', '55', '0', '5', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('544', '0', '65', '0', '25', '0', null, null, null, null, 'instance_magtheridons_lair');
INSERT INTO `instance_template` VALUES ('545', '0', '55', '0', '5', '0', null, null, null, null, 'instance_steam_vault');
INSERT INTO `instance_template` VALUES ('546', '0', '55', '0', '5', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('547', '0', '55', '0', '5', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('548', '0', '68', '0', '25', '0', null, null, null, null, 'instance_serpent_shrine');
INSERT INTO `instance_template` VALUES ('550', '0', '68', '0', '25', '0', null, null, null, null, 'instance_the_eye');
INSERT INTO `instance_template` VALUES ('552', '0', '68', '0', '5', '0', null, null, null, null, 'instance_arcatraz');
INSERT INTO `instance_template` VALUES ('553', '0', '68', '0', '5', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('554', '0', '68', '0', '5', '0', null, null, null, null, 'instance_mechanar');
INSERT INTO `instance_template` VALUES ('555', '0', '65', '0', '5', '0', null, null, null, null, 'instance_shadow_labyrinth');
INSERT INTO `instance_template` VALUES ('556', '0', '55', '0', '5', '0', null, null, null, null, 'instance_sethekk_halls');
INSERT INTO `instance_template` VALUES ('557', '0', '55', '0', '5', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('558', '0', '55', '0', '5', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('560', '0', '66', '0', '5', '0', null, null, null, null, 'instance_old_hillsbrad');
INSERT INTO `instance_template` VALUES ('564', '0', '70', '0', '25', '0', null, null, null, null, 'instance_black_temple');
INSERT INTO `instance_template` VALUES ('565', '0', '65', '0', '25', '0', null, null, null, null, 'instance_gruuls_lair');
INSERT INTO `instance_template` VALUES ('568', '0', '68', '70', '10', '0', null, null, null, null, 'instance_zulaman');
INSERT INTO `instance_template` VALUES ('580', '0', '70', '0', '25', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('585', '0', '65', '0', '5', '0', null, null, null, null, 'instance_magisters_terrace');
INSERT INTO `instance_template` VALUES ('489', '0', '10', '0', '50', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('30', '0', '10', '0', '50', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('529', '0', '10', '0', '50', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('566', '0', '10', '0', '50', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('559', '0', '10', '0', '50', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('562', '0', '10', '0', '50', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('572', '0', '10', '0', '50', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('574', '0', '65', '0', '5', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('575', '0', '75', '0', '5', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('576', '0', '66', '0', '5', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('578', '0', '75', '0', '5', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('616', '0', '80', '0', '10', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('615', '0', '80', '0', '10', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('608', '0', '70', '0', '5', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('604', '0', '71', '0', '5', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('602', '0', '75', '0', '5', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('599', '0', '72', '0', '5', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('601', '0', '67', '0', '5', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('619', '0', '68', '0', '5', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('600', '0', '69', '0', '5', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('595', '0', '75', '0', '5', '0', null, null, null, null, '');
