/*
SQLyog Ultimate v12.08 (64 bit)
MySQL - 10.1.11-MariaDB : Database - nost_logs
*********************************************************************
*/


/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`nost_logs` /*!40100 DEFAULT CHARACTER SET latin1 */;

USE `nost_logs`;

/*Table structure for table `logs_anticheat` */

DROP TABLE IF EXISTS `logs_anticheat`;

CREATE TABLE `logs_anticheat` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `time` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `realm` int(10) unsigned NOT NULL,
  `account` int(10) unsigned NOT NULL,
  `ip` varchar(16) NOT NULL,
  `actionMask` int(10) unsigned DEFAULT NULL,
  `player` varchar(32) NOT NULL,
  `info` varchar(512) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `account` (`account`),
  KEY `ip` (`ip`),
  KEY `time` (`time`),
  KEY `realm` (`realm`)
) ENGINE=InnoDB AUTO_INCREMENT=197 DEFAULT CHARSET=utf8;

/*Table structure for table `logs_battleground` */

DROP TABLE IF EXISTS `logs_battleground`;

CREATE TABLE `logs_battleground` (
  `time` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `bgid` int(11) DEFAULT NULL,
  `bgtype` int(11) DEFAULT NULL,
  `bgteamcount` int(11) DEFAULT NULL,
  `bgduration` int(11) DEFAULT NULL,
  `playerGuid` int(11) DEFAULT NULL,
  `team` int(11) DEFAULT NULL,
  `deaths` int(11) DEFAULT NULL,
  `honorBonus` int(11) DEFAULT NULL,
  `honorableKills` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `logs_behavior` */

DROP TABLE IF EXISTS `logs_behavior`;

CREATE TABLE `logs_behavior` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `account` int(10) unsigned NOT NULL,
  `detection` varchar(255) NOT NULL,
  `data` varchar(255) NOT NULL,
  KEY `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `logs_characters` */

DROP TABLE IF EXISTS `logs_characters`;

CREATE TABLE `logs_characters` (
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `type` enum('LostSocket','Create','Delete','Login','Logout','') NOT NULL DEFAULT '',
  `guid` int(11) NOT NULL DEFAULT '0',
  `account` int(11) NOT NULL DEFAULT '0',
  `name` varchar(255) NOT NULL DEFAULT '',
  `ip` varchar(255) NOT NULL DEFAULT '',
  `clientHash` varchar(255) NOT NULL DEFAULT '',
  KEY `guid` (`guid`),
  KEY `ip` (`ip`),
  KEY `clientHash` (`clientHash`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `logs_chat` */

DROP TABLE IF EXISTS `logs_chat`;

CREATE TABLE `logs_chat` (
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `type` enum('Say','Whisp','Group','Guild','Officer','Raid','BG','Chan','') NOT NULL DEFAULT '',
  `guid` int(11) NOT NULL DEFAULT '0',
  `target` int(11) NOT NULL DEFAULT '0',
  `channelId` int(11) NOT NULL DEFAULT '0',
  `channelName` varchar(255) NOT NULL DEFAULT '',
  `message` varchar(255) NOT NULL DEFAULT '',
  KEY `guid` (`guid`),
  KEY `target` (`target`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `logs_spamdetect` */

DROP TABLE IF EXISTS `logs_spamdetect`;

CREATE TABLE `logs_spamdetect` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `realm` int(10) unsigned NOT NULL,
  `accountId` int(11) DEFAULT '0',
  `fromGuid` int(11) unsigned NOT NULL DEFAULT '0',
  `fromIP` varchar(16) NOT NULL,
  `fromFingerprint` int(10) unsigned NOT NULL,
  `comment` varchar(512) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`),
  KEY `guid` (`fromGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `logs_trade` */

DROP TABLE IF EXISTS `logs_trade`;

CREATE TABLE `logs_trade` (
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `type` enum('AuctionBid','AuctionBuyout','SellItem','GM','Mail','QuestMaxLevel','Quest','Loot','Trade','') NOT NULL DEFAULT '',
  `sender` int(11) unsigned NOT NULL DEFAULT '0',
  `senderType` int(11) unsigned NOT NULL DEFAULT '0',
  `senderEntry` int(11) unsigned NOT NULL DEFAULT '0',
  `receiver` int(11) unsigned NOT NULL DEFAULT '0',
  `amount` int(11) NOT NULL DEFAULT '0',
  `data` int(11) NOT NULL DEFAULT '0',
  KEY `sender` (`sender`),
  KEY `receiver` (`receiver`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `logs_transactions` */

DROP TABLE IF EXISTS `logs_transactions`;

CREATE TABLE `logs_transactions` (
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `type` enum('Bid','Buyout','PlaceAuction','Trade','Mail','MailCOD') DEFAULT NULL,
  `guid1` int(11) unsigned NOT NULL DEFAULT '0',
  `money1` int(11) unsigned NOT NULL DEFAULT '0',
  `spell1` int(11) unsigned NOT NULL DEFAULT '0',
  `items1` varchar(255) NOT NULL DEFAULT '',
  `guid2` int(11) unsigned NOT NULL DEFAULT '0',
  `money2` int(11) unsigned NOT NULL DEFAULT '0',
  `spell2` int(11) unsigned NOT NULL DEFAULT '0',
  `items2` varchar(255) NOT NULL DEFAULT '',
  KEY `guid2` (`guid2`),
  KEY `guid1` (`guid1`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `logs_trashcharacters` */

DROP TABLE IF EXISTS `logs_trashcharacters`;

CREATE TABLE `logs_trashcharacters` (
  `guid` int(10) unsigned NOT NULL,
  `data` varchar(255) NOT NULL,
  `cluster` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Table structure for table `migrations` */

DROP TABLE IF EXISTS `migrations`;

CREATE TABLE `migrations` (
  `id` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Table structure for table `smartlog_creature` */

DROP TABLE IF EXISTS `smartlog_creature`;

CREATE TABLE `smartlog_creature` (
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `type` enum('Death','LongCombat','ScriptInfo','') NOT NULL DEFAULT '',
  `entry` int(11) NOT NULL DEFAULT '0',
  `guid` int(11) NOT NULL DEFAULT '0',
  `specifier` varchar(255) NOT NULL DEFAULT '',
  `combatTime` int(11) NOT NULL DEFAULT '0',
  `content` varchar(255) NOT NULL DEFAULT '',
  KEY `entry` (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
