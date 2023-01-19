-- phpMyAdmin SQL Dump
-- version 5.0.2
-- https://www.phpmyadmin.net/
--
-- Hôte : 127.0.0.1:3306
-- Généré le : mar. 17 jan. 2023 à 19:52
-- Version du serveur :  5.7.31
-- Version de PHP : 7.3.21

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de données : transfert_db
--

-- --------------------------------------------------------

--
-- Structure de la table transfert_tb
--

CREATE TABLE IF NOT EXISTS transfert_tb (
  code bigint(20) UNSIGNED NOT NULL,
  nomExpe varchar(50) NOT NULL,
  adresseExpe varchar(30) NOT NULL,
  nomBene varchar(50) NOT NULL,
  adresseBene varchar(30) DEFAULT NULL,
  carteIdBene bigint(20) UNSIGNED NOT NULL,
  dateExpe varchar(14) NOT NULL,
  dateRetrait varchar(14) DEFAULT NULL,
  dateAnnul varchar(14) DEFAULT NULL,
  montant bigint(20) UNSIGNED NOT NULL,
  retirer tinyint(1) DEFAULT '0',
  annuler tinyint(1) DEFAULT '0',
  PRIMARY KEY (code)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Déchargement des données de la table transfert_tb
--

INSERT INTO transfert_tb VALUES(418932835, 'Aliou', 'Mamou Telico', 'Cire', 'Labe Daka', 123456789110, '10/01/2023', '14/01/2023', NULL, 2560000, 1, 0);
INSERT INTO transfert_tb VALUES(545404204, 'Hamidou', 'Conakry Cimenterie', 'Amadou', 'Kindia Kassia', 123456789105, '08/01/2023', NULL, '15/01/2023', 155000, 0, 1);
INSERT INTO transfert_tb VALUES(581869302, 'Ibrahima', 'Conakry/Cite', 'Habi', 'Conakry/Kissosso', 239487083497, '15/01/2023', NULL, NULL, 78540000, 0, 0);
INSERT INTO transfert_tb VALUES(809094426, 'Lamine', 'Conakry Bambeto', 'Karim', 'Conakry Sonfonia', 549824649856, '15/01/2023', NULL, NULL, 6540000, 0, 0);
INSERT INTO transfert_tb VALUES(949333985, 'Sekou', 'Coyah Bentourayah', 'Mamoudou', 'Coyah Gomboyah', 123456789108, '08/01/2023', NULL, '15/01/2023', 450000, 0, 1);
INSERT INTO transfert_tb VALUES(1196140740, 'Amadou', 'Conakry Lambanyi', 'Aliou', 'Kankan mo', 123456789102, '13/01/2023', NULL, '14/01/2023', 6450000, 0, 1);
INSERT INTO transfert_tb VALUES(2348838239, 'Oumar', 'Conakry Tombolia', 'Kaba', 'Conakry Cosa', 654683241648, '15/01/2023', NULL, NULL, 12545000, 0, 0);
INSERT INTO transfert_tb VALUES(2350294565, 'Boubacar', 'Conakry Matoto', 'Ilyassou', 'Conakry Dabompa', 524692135462, '10/01/2023', NULL, NULL, 825000, 0, 0);
INSERT INTO transfert_tb VALUES(2715962298, 'Seydouba', 'Conakry T7', 'Stone', 'Conakry T8', 123456789109, '08/01/2023', '14/01/2023', NULL, 690000, 1, 0);
INSERT INTO transfert_tb VALUES(3499211612, 'Siradiou', 'Conakry/T6', 'Issa', 'Labe/Konkola', 123456789101, '15/01/2023', NULL, NULL, 545000, 0, 0);
INSERT INTO transfert_tb VALUES(3586334585, 'Djamal', 'Conakry Enta', 'AK', 'Conakry Sonfonia', 123456789104, '08/01/2023', NULL, NULL, 8690000, 0, 0);
INSERT INTO transfert_tb VALUES(3890346734, 'Ibrahima', 'Conakry Sonfonia', 'Baillo', 'Conakry Sangoyah', 123456789103, '08/01/2023', NULL, NULL, 2450000, 0, 0);
INSERT INTO transfert_tb VALUES(3922919429, 'Oumou', 'Labe Konkola', 'Aissatou', 'Labe Maleyah', 123456789107, '08/01/2023', NULL, NULL, 250000, 0, 0);
INSERT INTO transfert_tb VALUES(4161255391, 'Saliou', 'Conakry Aviation', 'Lamine', 'Conakry Tombolia', 123456789106, '08/01/2023', NULL, NULL, 670000, 0, 0);
INSERT INTO transfert_tb VALUES(4264392720, 'Simpson', 'Conakry Lambanyi', 'Ramadane', 'Conakry Wanindara', 654941654689, '15/01/2023', NULL, NULL, 7455000, 0, 0);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
