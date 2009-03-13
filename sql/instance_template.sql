/*
MySQL Data Transfer
Source Host: 89.163.144.238
Source Database: worked
Target Host: 89.163.144.238
Target Database: worked
Date: 28.01.2009 23:21:14
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for instance_template
-- ----------------------------
DROP TABLE IF EXISTS `instance_template`;
CREATE TABLE `instance_template` (
  `map` smallint(5) unsigned NOT NULL,
  `parent` int(10) unsigned NOT NULL,
  `levelMin` tinyint(3) unsigned NOT NULL default '0',
  `levelMax` tinyint(3) unsigned NOT NULL default '0',
  `maxPlayers` tinyint(3) unsigned NOT NULL default '0',
  `maxPlayersHeroic` tinyint(3) unsigned NOT NULL default '0',
  `reset_delay` int(10) unsigned NOT NULL default '0',
  `startLocX` float default NULL,
  `startLocY` float default NULL,
  `startLocZ` float default NULL,
  `startLocO` float default NULL,
  `script` varchar(128) NOT NULL default '',
  PRIMARY KEY  (`map`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `instance_template` VALUES ('33', '0', '22', '30', '10', '10', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('34', '0', '24', '32', '10', '10', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('36', '0', '15', '20', '10', '10', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('43', '0', '15', '21', '10', '10', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('47', '0', '29', '38', '10', '10', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('48', '0', '24', '32', '10', '10', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('70', '0', '35', '47', '10', '10', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('90', '0', '29', '38', '10', '10', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('109', '0', '45', '55', '10', '10', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('129', '0', '37', '46', '10', '10', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('189', '0', '34', '45', '10', '10', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('209', '0', '44', '54', '10', '10', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('229', '0', '58', '0', '10', '10', '120000', '78.5083', '-225.044', '49.839', '5.1', '');
INSERT INTO `instance_template` VALUES ('230', '0', '52', '0', '5', '5', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('249', '0', '60', '0', '40', '40', '432000', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('289', '0', '57', '0', '5', '5', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('309', '0', '60', '0', '20', '20', '259200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('329', '0', '58', '60', '5', '5', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('349', '0', '46', '55', '10', '10', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('389', '0', '13', '18', '10', '10', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('409', '0', '60', '0', '40', '40', '604800', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('429', '0', '55', '60', '5', '5', '7200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('469', '0', '60', '0', '40', '40', '604800', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('509', '0', '60', '0', '20', '20', '259200', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('531', '0', '60', '0', '40', '40', '604800', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('533', '0', '80', '0', '10', '25', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('615', '0', '80', '0', '10', '25', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('616', '0', '80', '0', '10', '25', '0', null, null, null, null, '');
INSERT INTO `instance_template` VALUES ('624', '0', '80', '0', '10', '25', '0', null, null, null, null, '');
