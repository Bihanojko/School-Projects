DROP TABLE IF EXISTS titul_autor;
DROP TABLE IF EXISTS rezervace;
DROP TABLE IF EXISTS vypujcka;
DROP TABLE IF EXISTS vytisk;
DROP TABLE IF EXISTS titul;
DROP TABLE IF EXISTS autor;
DROP TABLE IF EXISTS uzivatel;


SET NAMES utf8;
SET foreign_key_checks = 0;
SET time_zone = '+01:00';


CREATE TABLE uzivatel (UC int PRIMARY KEY NOT NULL AUTO_INCREMENT, jmeno varchar(255) NOT NULL, 
prijmeni varchar(255) NOT NULL, login varchar(50) UNIQUE NOT NULL, heslo varchar(255) NOT NULL, typ_uziv int NOT NULL, 
ulice varchar(50), mesto varchar(50), psc int, email varchar(50), tel varchar(13), predplaceno_do DATE DEFAULT NULL) ENGINE=InnoDB COLLATE utf8_czech_ci;

CREATE TABLE autor (AC int PRIMARY KEY NOT NULL AUTO_INCREMENT, Ajmeno varchar(50) NOT NULL, 
Aprijmeni varchar(50) NOT NULL, datum_narozeni DATE, 
datum_umrti DATE DEFAULT NULL) ENGINE=InnoDB COLLATE utf8_czech_ci;

CREATE TABLE titul (TID int PRIMARY KEY NOT NULL AUTO_INCREMENT, Tnazev varchar(50) NOT NULL, 
jazyk varchar(20), zanr varchar(20), typ int NOT NULL, ISBN bigint DEFAULT NULL, ISSN bigint DEFAULT NULL) ENGINE=InnoDB COLLATE utf8_czech_ci;



ALTER TABLE titul ADD CONSTRAINT ConISBN 
CHECK((typ = 2 AND ISBN = NULL) OR
1000000000000 <= ISBN AND ISBN <= 9999999999999 AND MOD(
(
  (MOD(TRUNC(ISBN / 1), 10) * 1) +
  (MOD(TRUNC(ISBN / 10), 10) * 3) + 
  (MOD(TRUNC(ISBN / 100), 10) * 1) + 
  (MOD(TRUNC(ISBN / 1000), 10) * 3) + 
  (MOD(TRUNC(ISBN / 10000), 10) * 1) + 
  (MOD(TRUNC(ISBN / 100000), 10) * 3) + 
  (MOD(TRUNC(ISBN / 1000000), 10) * 1) + 
  (MOD(TRUNC(ISBN / 10000000), 10) * 3) + 
  (MOD(TRUNC(ISBN / 100000000), 10) * 1) + 
  (MOD(TRUNC(ISBN / 1000000000), 10) * 3) + 
  (MOD(TRUNC(ISBN / 10000000000), 10) * 1) + 
  (MOD(TRUNC(ISBN / 100000000000), 10) * 3) + 
  (MOD(TRUNC(ISBN / 1000000000000), 10) * 1)
), 10) = 0);


ALTER TABLE titul ADD CONSTRAINT ConISSN 
CHECK((typ = 1 AND ISSN = NULL) OR
10000000 <= ISSN AND ISSN <= 99999999 AND MOD(
(
  (MOD(TRUNC(ISSN / 1), 10) * 1) +
  (MOD(TRUNC(ISSN / 10), 10) * 2) + 
  (MOD(TRUNC(ISSN / 100), 10) * 3) + 
  (MOD(TRUNC(ISSN / 1000), 10) * 4) + 
  (MOD(TRUNC(ISSN / 10000), 10) * 5) + 
  (MOD(TRUNC(ISSN / 100000), 10) * 6) + 
  (MOD(TRUNC(ISSN / 1000000), 10) * 7) + 
  (MOD(TRUNC(ISSN / 10000000), 10) * 8)
), 11) = 0);


CREATE TABLE vytisk (prirustkove_cislo int PRIMARY KEY NOT NULL AUTO_INCREMENT, TID int NOT NULL, 
nakladatelstvi varchar(50)) ENGINE=InnoDB CHARACTER SET utf8 COLLATE utf8_czech_ci;

CREATE TABLE vypujcka (VID int PRIMARY KEY NOT NULL AUTO_INCREMENT, datum_vypujceni DATE NOT NULL, 
datum_vraceni DATE DEFAULT NULL, stav int NOT NULL, 
prirustkove_cislo int NOT NULL, CC int NOT NULL, OC_vytvoril int NOT NULL, 
OC_uzavrel int DEFAULT NULL) ENGINE=InnoDB CHARACTER SET utf8 COLLATE utf8_czech_ci; 

CREATE TABLE titul_autor (TID int NOT NULL, AC int NOT NULL) ENGINE=InnoDB CHARACTER SET utf8 COLLATE utf8_czech_ci; 
ALTER TABLE titul_autor ADD PRIMARY KEY (TID, AC);

CREATE TABLE rezervace (TID int NOT NULL, CC int NOT NULL) ENGINE=InnoDB CHARACTER SET utf8 COLLATE utf8_czech_ci; 
ALTER TABLE rezervace ADD PRIMARY KEY (TID, CC);

ALTER TABLE uzivatel ADD CONSTRAINT Contype 
CHECK(typ_uziv >= 0 AND typ_uziv < 3);

ALTER TABLE vytisk ADD FOREIGN KEY (TID) REFERENCES titul (TID);

ALTER TABLE vypujcka ADD FOREIGN KEY (prirustkove_cislo) REFERENCES vytisk (prirustkove_cislo);
ALTER TABLE vypujcka ADD FOREIGN KEY (CC) REFERENCES uzivatel (UC);
ALTER TABLE vypujcka ADD FOREIGN KEY (OC_vytvoril) REFERENCES uzivatel (UC);
ALTER TABLE vypujcka ADD FOREIGN KEY (OC_uzavrel) REFERENCES uzivatel (UC);

ALTER TABLE titul_autor ADD FOREIGN KEY (TID) REFERENCES titul (TID);
ALTER TABLE titul_autor ADD FOREIGN KEY (AC) REFERENCES autor (AC);

ALTER TABLE rezervace ADD FOREIGN KEY (TID) REFERENCES titul (TID);
ALTER TABLE rezervace ADD FOREIGN KEY (CC) REFERENCES uzivatel (UC);


INSERT INTO uzivatel (jmeno, prijmeni, login, heslo, typ_uziv, ulice, mesto, psc, email, tel, predplaceno_do) VALUES 
('Jan', 'Zahradník', 'honzaz', '$1$Mz.XMWcF$JQO7Kr8N9.lvxSY.75uCI/', 2, 'Cejl 8', 'Brno', 66401, 'jzahradnik@mail.com', '+420805624458', NULL),
('Aleš', 'Musil', 'amusil', '$1$N1d18vOF$/CqWMjXtp5iNVREQKewR0/', 1, 'Božetěchova 18', 'Brno', 66401, 'amusil@mail.com', '+420707184259', NULL),
('Daniel', 'Dvořák', 'dan', '$1$vTj6YaN5$LvCjuEodCmmyfpcLDoVrI.', 1, 'U Vody 39', 'Zakřany', 66482, 'ddvorak@mail.com', '+420608931556', NULL),
('Karel', 'Stromecký', 'stromek', '$1$duvEA/QL$8uDDw0c8.5QQNVAswbZLQ.', 1, 'Polní 98', 'Rosice', 66501, 'ks@mail.com', '+420608931556', NULL),
('Monika', 'Rovná', 'monule', '$1$w24fUDTr$oEL3LXnjcEMQA.a4ZKdul1', 0, 'U lesa 12', 'Tetčice', 66494, 'monule@supermail.cz', NULL, STR_TO_DATE('2016-12-29', '%Y-%m-%d')),
('Marie', 'Přihradská', 'mp', '$1$kbaoVzBG$8yELfrECbHfTF8aqRP3TJ0', 0, 'Hajní 68', 'Zakřany', 66482, 'marie123@supermail.cz', NULL, STR_TO_DATE('2016-12-15', '%Y-%m-%d')),
('Pavel', 'Přihradský', 'pavlik', '$1$VSTgtGW4$8fzcajq.aAo6VUIphlkZ3/', 0, 'Hajní 68', 'Zakřany', 66482, 'pavel.prihradsky@supermail.cz', '+420804337785', STR_TO_DATE('2016-10-27', '%Y-%m-%d')),
('Jan', 'Vojvoda', 'janv', '$1$rcmvmaDQ$cD66ECXGs5Pbano.geOZD1', 0, 'Cejl 15', 'Brno', 66401, 'vojvoda@mail.com', '+420606812546', STR_TO_DATE('2016-10-27', '%Y-%m-%d'));


INSERT INTO autor (Ajmeno, Aprijmeni, datum_narozeni, datum_umrti) VALUES
('George', 'Orwell', STR_TO_DATE('1903-06-25', '%Y-%m-%d'), STR_TO_DATE('1950-01-21', '%Y-%m-%d')),
('Jules', 'Verne', STR_TO_DATE('1828-02-08', '%Y-%m-%d'), STR_TO_DATE('1905-03-24', '%Y-%m-%d')),
('George R. R.', 'Martin', STR_TO_DATE('1948-09-20', '%Y-%m-%d'), DEFAULT),
('Oscar', 'Wilde', STR_TO_DATE('1854-10-16', '%Y-%m-%d'), STR_TO_DATE('1900-11-30', '%Y-%m-%d')),
('Karel', 'Čapek', STR_TO_DATE('1890-01-09', '%Y-%m-%d'), STR_TO_DATE('1938-12-25', '%Y-%m-%d')),
('Alois', 'Jirásek', STR_TO_DATE('1851-08-23', '%Y-%m-%d'), STR_TO_DATE('1930-03-12', '%Y-%m-%d')),
('Jane', 'Austen', STR_TO_DATE('1775-12-16', '%Y-%m-%d'), STR_TO_DATE('1817-07-18', '%Y-%m-%d')),
('Emily', 'Brontë', STR_TO_DATE('1818-07-30', '%Y-%m-%d'), STR_TO_DATE('1848-12-19', '%Y-%m-%d')),
('Božena', 'Němcová', STR_TO_DATE('1820-02-04', '%Y-%m-%d'), STR_TO_DATE('1862-01-21', '%Y-%m-%d')),
('Joseph', 'Heller', STR_TO_DATE('1923-05-01', '%Y-%m-%d'), STR_TO_DATE('1999-12-12', '%Y-%m-%d')),
('Ray', 'Bradbury', STR_TO_DATE('1920-08-22', '%Y-%m-%d'), STR_TO_DATE('2012-06-05', '%Y-%m-%d')),
('Francis Scott', 'Fitzgerald', STR_TO_DATE('1896-09-24', '%Y-%m-%d'), STR_TO_DATE('1940-12-21', '%Y-%m-%d'));


INSERT INTO titul (Tnazev, jazyk, zanr, typ, ISBN, ISSN) VALUES
('1984', 'čeština', 'antiutopie', 1, 9788483310397, DEFAULT),
('Cesta k Wigan Pier', 'čeština', 'fejetony', 1, 9788073900625, DEFAULT),
('The Song of Ice and Fire', 'english', 'fantasy', 1, 9780545586177, DEFAULT),
('Vesmír', 'čeština', 'naučné', 2, DEFAULT, 12144029),
('Hra o trůny', 'čeština', 'fantasy', 1, 9788071974123, DEFAULT),
('Obraz Doriana Graye', 'čeština', 'román', 1, 9781619490581, DEFAULT),
('Strašidlo cantervillské', 'čeština', 'román', 1, 9781447405436, DEFAULT),
('Krakatit', 'čeština', 'román', 1, 9788020016782, DEFAULT),
('Bílá nemoc', 'čeština', 'drama', 1, 9788086216478, DEFAULT),
('R. U. R.', 'čeština', 'drama', 1, 9788831720946, DEFAULT),
('Psohlavci', 'čeština', 'román', 1, 9788073901295, DEFAULT),
('Pýcha a předsudek', 'čeština', 'román', 1, 9788073900212, DEFAULT),
('Na větrné hůrce', 'čeština', 'román', 1, 9788073351939, DEFAULT),
('Divá Bára', 'čeština', 'román', 1, 9788000005027, DEFAULT),
('Babička', 'čeština', 'román', 1, 9789061006947, DEFAULT),
('Hlava XXII', 'čeština', 'válečné', 1, 9780099536017, DEFAULT),
('451 stupňů Fahrenheita', 'čeština','fantasy', 1, 9789703707034, DEFAULT),
('Velký Gatsby', 'čeština', 'román', 1, 9781597226769, DEFAULT),
('Farma zvířat', 'čeština', 'román', 1, 9780736692427, DEFAULT);


INSERT INTO vytisk (TID, nakladatelstvi) VALUES
(1, 'Dobrovský'),
(1, 'Albatros'),
(1, 'Dobrovský'),
(2, 'Scholastic'),
(3, 'Albatros'),
(4, 'Dobrovský'),
(5, 'Albatros'),
(6, 'Scholastic'),
(7, 'Dobrovský'),
(8, 'Albatros'),
(9, 'Dobrovský'),
(10, 'Scholastic'),
(11, 'Dobrovský'),
(12, 'Albatros'),
(13, 'Dobrovský'),
(14, 'Dobrovský'),
(15, 'Albatros'),
(16, 'Dobrovský'),
(17, 'Dobrovský'),
(18, 'Albatros'),
(19, 'Scholastic');


INSERT INTO vypujcka (datum_vypujceni, datum_vraceni, stav, prirustkove_cislo, CC, OC_vytvoril, OC_uzavrel) VALUES
(STR_TO_DATE('07-05-2015', '%d-%m-%Y'), STR_TO_DATE('2015-07-01', '%Y-%m-%d'), 0, 1, 1, 1, 1),
(STR_TO_DATE('19-01-2015', '%d-%m-%Y'), STR_TO_DATE('2015-08-21', '%Y-%m-%d'), 0, 2, 2, 2, 2),
(STR_TO_DATE('28-01-2016', '%d-%m-%Y'), STR_TO_DATE('2016-09-07', '%Y-%m-%d'), 0, 3, 3, 2, 3),
(STR_TO_DATE('07-01-2015', '%d-%m-%Y'), STR_TO_DATE('2015-10-01', '%Y-%m-%d'), 0, 8, 5, 2, 3),
(STR_TO_DATE('19-01-2015', '%d-%m-%Y'), STR_TO_DATE('2015-11-21', '%Y-%m-%d'), 0, 9, 6, 2, 2),
(STR_TO_DATE('28-08-2016', '%d-%m-%Y'), STR_TO_DATE('2016-12-07', '%Y-%m-%d'), 0, 10, 4, 2, 3),
(STR_TO_DATE('07-04-2015', '%d-%m-%Y'), STR_TO_DATE('2015-05-01', '%Y-%m-%d'), 0, 1, 7, 1, 1),
(STR_TO_DATE('19-08-2015', '%d-%m-%Y'), STR_TO_DATE('2015-09-21', '%Y-%m-%d'), 0, 2, 8, 2, 2),
(STR_TO_DATE('28-03-2016', '%d-%m-%Y'), STR_TO_DATE('2016-04-07', '%Y-%m-%d'), 0, 3, 4, 2, 3),
(STR_TO_DATE('07-07-2015', '%d-%m-%Y'), STR_TO_DATE('2015-08-01', '%Y-%m-%d'), 0, 8, 5, 2, 3),
(STR_TO_DATE('19-05-2015', '%d-%m-%Y'), STR_TO_DATE('2015-06-21', '%Y-%m-%d'), 0, 9, 6, 2, 2),
(STR_TO_DATE('28-06-2016', '%d-%m-%Y'), STR_TO_DATE('2016-07-07', '%Y-%m-%d'), 0, 10, 4, 2, 3),
(STR_TO_DATE('28-02-2016', '%d-%m-%Y'), DEFAULT, 1, 2, 1, 1, DEFAULT),
(STR_TO_DATE('09-10-2016', '%d-%m-%Y'), DEFAULT, 1, 1, 3, 2, DEFAULT),
(STR_TO_DATE('28-08-2016', '%d-%m-%Y'), DEFAULT, 1, 3, 6, 2, DEFAULT),
(STR_TO_DATE('12-03-2016', '%d-%m-%Y'), DEFAULT, 1, 4, 2, 3, DEFAULT);


INSERT INTO titul_autor (TID, AC) VALUES 
(1, 1),
(2, 1),
(3, 3),
(4, 10),
(5, 3),
(6, 4),
(7, 4),
(8, 5),
(9, 5),
(10, 5),
(11, 6),
(12, 7),
(13, 8),
(14, 9),
(15, 9),
(16, 10),
(17, 11),
(18, 12),
(19, 1);


INSERT INTO rezervace (TID, CC) VALUES
(1, 1),
(2, 2),
(3, 3),
(4, 4),
(5, 5),
(6, 6),
(7, 7),
(8, 8),
(3, 5),
(4, 6),
(5, 7),
(6, 2),
(7, 5),
(9, 1);
