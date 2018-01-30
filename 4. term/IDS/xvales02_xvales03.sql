-- Predmet: Databazove systemy
-- Projekt: 4. cast - SQL skript pro vytvoreni pokrocilych objektu schematu databaze
-- Zadani: Knihovna
-- Datum: 29. 4. 2016
-- Autori: 	Ondrej Vales, xvales03
--		   	Nikola Valesova, xvales02




------------------------------------------------------
-- ZRUSENI VSECH DRIVE VYTVORENYCH OBJEKTU DATABAZE --
------------------------------------------------------
DROP SEQUENCE seq_oc;
DROP SEQUENCE seq_cc;
DROP SEQUENCE seq_ac;
DROP SEQUENCE seq_tid;
DROP SEQUENCE seq_vid;
DROP SEQUENCE seq_pc;

DROP TRIGGER zmen_stav_datum;
DROP TRIGGER nastav_prirustkove_cislo;

DROP INDEX idx;
DROP INDEX idx2;

DROP TABLE titul_zanr;
DROP TABLE titul_autor;
DROP TABLE rezervace;
DROP TABLE vypujcka;
DROP TABLE vytisk;
DROP TABLE titul;
DROP TABLE autor;
DROP TABLE ctenar;
DROP TABLE zanr;
DROP TABLE knihovnik;

connect xvales02/fj70543k;
DROP MATERIALIZED VIEW ctenari_neuzavreno;
connect xvales03/4hvxovah;


----------------------------------------------
-- VYTVORENI TABULEK A INTEGRITNICH OMEZENI --
----------------------------------------------
CREATE TABLE knihovnik (OC INT PRIMARY KEY NOT NULL, jmeno VARCHAR2(50 CHAR) NOT NULL, 
prijmeni VARCHAR2(50 CHAR) NOT NULL, pozice VARCHAR2(50 CHAR) NOT NULL, 
datum_narozeni DATE NOT NULL, ulice VARCHAR2(50 CHAR), mesto VARCHAR2(50 CHAR), 
psc INT, email VARCHAR2(50 CHAR), tel VARCHAR2(13 CHAR), vedouci INT DEFAULT NULL);

CREATE TABLE ctenar (CC INT PRIMARY KEY NOT NULL, jmeno VARCHAR2(50 CHAR) NOT NULL, 
prijmeni VARCHAR2(50 CHAR) NOT NULL, datum_narozeni DATE NOT NULL, 
ulice VARCHAR2(50 CHAR), mesto VARCHAR2(50 CHAR), psc INT, email VARCHAR2(50 CHAR), 
tel VARCHAR2(13 CHAR), predplaceno_do DATE NOT NULL);

CREATE TABLE autor (AC INT PRIMARY KEY NOT NULL, jmeno VARCHAR2(50 CHAR) NOT NULL, 
prijmeni VARCHAR2(50 CHAR) NOT NULL, datum_narozeni DATE NOT NULL, 
datum_umrti DATE DEFAULT NULL);

CREATE TABLE titul (TID INT PRIMARY KEY NOT NULL, nazev VARCHAR2(50 CHAR) NOT NULL, 
jazyk VARCHAR2(20 CHAR), typ INT NOT NULL, ISBN INT DEFAULT NULL, ISSN INT DEFAULT NULL);

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

CREATE TABLE vytisk (prirustkove_cislo INT PRIMARY KEY NOT NULL, TID INT NOT NULL, 
nakladatelstvi VARCHAR2(50 CHAR), stav VARCHAR2(50 CHAR) NOT NULL);

CREATE TABLE vypujcka (VID INT PRIMARY KEY NOT NULL, datum_vypujceni DATE NOT NULL, 
datum_vraceni DATE DEFAULT NULL, stav VARCHAR2(50 CHAR) NOT NULL, 
prirustkove_cislo INT NOT NULL, CC INT NOT NULL, OC_vytvoril INT NOT NULL, 
OC_uzavrel INT DEFAULT NULL);

CREATE TABLE zanr (zkratka VARCHAR2(10 CHAR) PRIMARY KEY NOT NULL, nazev VARCHAR2(50 CHAR));

CREATE TABLE titul_zanr (TID INT NOT NULL, zkratka VARCHAR2(10 CHAR) NOT NULL);
ALTER TABLE titul_zanr ADD PRIMARY KEY (TID, zkratka);

CREATE TABLE titul_autor (TID INT NOT NULL, AC INT NOT NULL);
ALTER TABLE titul_autor ADD PRIMARY KEY (TID, AC);

CREATE TABLE rezervace (TID INT NOT NULL, CC INT NOT NULL);
ALTER TABLE rezervace ADD PRIMARY KEY (TID, CC);


--------------------------
-- PRIDANI CIZICH KLICU --
--------------------------
ALTER TABLE knihovnik ADD FOREIGN KEY (vedouci) REFERENCES knihovnik (OC);

ALTER TABLE vytisk ADD FOREIGN KEY (TID) REFERENCES titul (TID);

ALTER TABLE vypujcka ADD FOREIGN KEY (prirustkove_cislo) REFERENCES vytisk (prirustkove_cislo);
ALTER TABLE vypujcka ADD FOREIGN KEY (CC) REFERENCES ctenar (CC);
ALTER TABLE vypujcka ADD FOREIGN KEY (OC_vytvoril) REFERENCES knihovnik (OC);
ALTER TABLE vypujcka ADD FOREIGN KEY (OC_uzavrel) REFERENCES knihovnik (OC);

ALTER TABLE titul_zanr ADD FOREIGN KEY (TID) REFERENCES titul (TID);
ALTER TABLE titul_zanr ADD FOREIGN KEY (zkratka) REFERENCES zanr (zkratka);

ALTER TABLE titul_autor ADD FOREIGN KEY (TID) REFERENCES titul (TID);
ALTER TABLE titul_autor ADD FOREIGN KEY (AC) REFERENCES autor (AC);

ALTER TABLE rezervace ADD FOREIGN KEY (TID) REFERENCES titul (TID);
ALTER TABLE rezervace ADD FOREIGN KEY (CC) REFERENCES ctenar (CC);


------------------------
-- VYTVORENI SEKVENCI --
------------------------
CREATE SEQUENCE seq_oc
MINVALUE 1
MAXVALUE 1025
START WITH 1
INCREMENT BY 1
CACHE 1024;

CREATE SEQUENCE seq_cc
MINVALUE 1
MAXVALUE 1025
START WITH 1
INCREMENT BY 1
CACHE 1024;

CREATE SEQUENCE seq_ac
MINVALUE 1
MAXVALUE 1025
START WITH 1
INCREMENT BY 1
CACHE 1024;

CREATE SEQUENCE seq_tid
MINVALUE 1
MAXVALUE 1025
START WITH 1
INCREMENT BY 1
CACHE 1024;

CREATE SEQUENCE seq_vid
MINVALUE 1
MAXVALUE 1025
START WITH 1
INCREMENT BY 1
CACHE 1024;

CREATE SEQUENCE seq_pc
MINVALUE 1
MAXVALUE 1025
START WITH 1
INCREMENT BY 1
CACHE 1024;


---------------------------------------
-- VLOZENI UKAZKOVYCH DAT TO TABULEK --
---------------------------------------
INSERT INTO knihovnik (OC, jmeno, prijmeni, pozice, datum_narozeni, ulice, mesto, psc, email, tel, vedouci)
VALUES (seq_oc.nextval, 'Jan', 'Zahradnik', 'vedouci', TO_DATE('1978-12-24', 'yyyy-mm-dd'), 'Cejl 8', 'Brno', 66401, 'jzahradnik@mail.com', '+420805624458', DEFAULT);

INSERT INTO knihovnik (OC, jmeno, prijmeni, pozice, datum_narozeni, ulice, mesto, psc, email, tel, vedouci)
VALUES (seq_oc.nextval, 'Ales', 'Musil', 'knihovnik', TO_DATE('1981-05-16', 'yyyy-mm-dd'), 'Bozetechova 18', 'Brno', 66401, 'amusil@mail.com', '+420707184259', 1);

INSERT INTO knihovnik (OC, jmeno, prijmeni, pozice, datum_narozeni, ulice, mesto, psc, email, tel, vedouci)
VALUES (seq_oc.nextval, 'Daniel', 'Dvorak', 'knihovnik', TO_DATE('1983-12-01', 'yyyy-mm-dd'), 'U Vody 39', 'Zakrany', 66482, 'ddvorak@mail.com', '+420608931556', 1);


INSERT INTO ctenar (CC, jmeno, prijmeni, datum_narozeni, ulice, mesto, psc, email, tel, predplaceno_do)
VALUES (seq_cc.nextval, 'Marie', 'Prihradska', TO_DATE('1965-08-05', 'yyyy-mm-dd'), 'Hajni 68', 'Zakrany', 66482, 'marie123@supermail.cz', NULL, TO_DATE('2016-10-27', 'yyyy-mm-dd'));

INSERT INTO ctenar (CC, jmeno, prijmeni, datum_narozeni, ulice, mesto, psc, email, tel, predplaceno_do)
VALUES (seq_cc.nextval, 'Pavel', 'Prihradsky', TO_DATE('1966-02-14', 'yyyy-mm-dd'), 'Hajni 68', 'Zakrany', 66482, 'pavel.prihradsky@supermail.cz', '+420804337785', TO_DATE('2016-10-27', 'yyyy-mm-dd'));

INSERT INTO ctenar (CC, jmeno, prijmeni, datum_narozeni, ulice, mesto, psc, email, tel, predplaceno_do)
VALUES (seq_cc.nextval, 'Jan', 'Vojvoda', TO_DATE('1993-11-01', 'yyyy-mm-dd'), 'Cejl 15', 'Brno', 66401, 'vojvoda@mail.com', '+420606812546', TO_DATE('2016-10-27', 'yyyy-mm-dd'));

INSERT INTO ctenar (CC, jmeno, prijmeni, datum_narozeni, ulice, mesto, psc, email, tel, predplaceno_do)
VALUES (seq_cc.nextval, 'Klara', 'Dusanova', TO_DATE('1987-08-31', 'yyyy-mm-dd'), 'Brnenska 201', 'Vyskov', 66284, 'klaruska@seznam.cz', '+420733798954', TO_DATE('2016-12-24', 'yyyy-mm-dd'));

INSERT INTO ctenar (CC, jmeno, prijmeni, datum_narozeni, ulice, mesto, psc, email, tel, predplaceno_do)
VALUES (seq_cc.nextval, 'Monika', 'Ondrackova', TO_DATE('1993-02-01', 'yyyy-mm-dd'), 'Severni 666', 'Ostrava', 66284, 'monion@seznam.cz', '+420636974205', TO_DATE('2016-10-22', 'yyyy-mm-dd'));

INSERT INTO ctenar (CC, jmeno, prijmeni, datum_narozeni, ulice, mesto, psc, email, tel, predplaceno_do)
VALUES (seq_cc.nextval, 'Jan', 'Kopal', TO_DATE('1985-07-31', 'yyyy-mm-dd'), 'Rosicka 505', 'Tetcice', 66284, 'kopi@centrum.cz', '+420737007007', TO_DATE('2016-11-20', 'yyyy-mm-dd'));


INSERT INTO autor (AC, jmeno, prijmeni, datum_narozeni, datum_umrti)
VALUES (seq_ac.nextval, 'Karel', 'Capek', TO_DATE('1890-01-09', 'yyyy-mm-dd'), TO_DATE('1938-12-25', 'yyyy-mm-dd'));

INSERT INTO autor (AC, jmeno, prijmeni, datum_narozeni, datum_umrti)
VALUES (seq_ac.nextval, 'Jules', 'Verne', TO_DATE('1828-02-08', 'yyyy-mm-dd'), TO_DATE('1905-03-24', 'yyyy-mm-dd'));

INSERT INTO autor (AC, jmeno, prijmeni, datum_narozeni, datum_umrti)
VALUES (seq_ac.nextval, 'Suzane', 'Collins', TO_DATE('1962-08-10', 'yyyy-mm-dd'), DEFAULT);

INSERT INTO autor (AC, jmeno, prijmeni, datum_narozeni, datum_umrti)
VALUES (seq_ac.nextval, 'Emily', 'Bronte', TO_DATE('1818-07-30', 'yyyy-mm-dd'), TO_DATE('1848-12-19', 'yyyy-mm-dd'));

INSERT INTO autor (AC, jmeno, prijmeni, datum_narozeni, datum_umrti)
VALUES (seq_ac.nextval, 'Oscar', 'Wilde', TO_DATE('1854-10-16', 'yyyy-mm-dd'), TO_DATE('1900-11-30', 'yyyy-mm-dd'));

INSERT INTO autor (AC, jmeno, prijmeni, datum_narozeni, datum_umrti)
VALUES (seq_ac.nextval, 'George', 'Orwell', TO_DATE('1903-06-25', 'yyyy-mm-dd'), TO_DATE('1950-01-21', 'yyyy-mm-dd'));


INSERT INTO titul (TID, nazev, jazyk, typ, ISBN, ISSN)
VALUES (seq_tid.nextval, 'Krakatit', 'cestina', 1, 9788073900212, DEFAULT);

INSERT INTO titul (TID, nazev, jazyk, typ, ISBN, ISSN)
VALUES (seq_tid.nextval, 'R.U.R.', 'cestina', 1, 9788073900625, DEFAULT);

INSERT INTO titul (TID, nazev, jazyk, typ, ISBN, ISSN)
VALUES (seq_tid.nextval, 'Catching Fire', 'english', 1, 9780545586177, DEFAULT);

INSERT INTO titul (TID, nazev, jazyk, typ, ISBN, ISSN)
VALUES (seq_tid.nextval, 'Vesmir', 'cestina', 2, DEFAULT, 12144029);

INSERT INTO titul (TID, nazev, jazyk, typ, ISBN, ISSN)
VALUES (seq_tid.nextval, 'Na vetrne hurce', 'cestina', 1, 8790545586177, DEFAULT);

INSERT INTO titul (TID, nazev, jazyk, typ, ISBN, ISSN)
VALUES (seq_tid.nextval, 'Obraz Doriana Graye', 'cestina', 1, 7790545586178, DEFAULT);

INSERT INTO titul (TID, nazev, jazyk, typ, ISBN, ISSN)
VALUES (seq_tid.nextval, '1984', 'english', 1, 8790545587167, DEFAULT);


INSERT INTO vytisk (prirustkove_cislo, TID, nakladatelstvi, stav)
VALUES (seq_pc.nextval, 1, 'Dobrovsky', 'dobry');

INSERT INTO vytisk (prirustkove_cislo, TID, nakladatelstvi, stav)
VALUES (seq_pc.nextval, 1, 'Dobrovsky', 'dobry');

INSERT INTO vytisk (prirustkove_cislo, TID, nakladatelstvi, stav)
VALUES (seq_pc.nextval, 1, 'Dobrovsky', 'poskozeno');

INSERT INTO vytisk (prirustkove_cislo, TID, nakladatelstvi, stav)
VALUES (seq_pc.nextval, 2, 'Dobrovsky', 'novy');

INSERT INTO vytisk (prirustkove_cislo, TID, nakladatelstvi, stav)
VALUES (seq_pc.nextval, 2, 'Dobrovsky', 'vyrazeno');

INSERT INTO vytisk (prirustkove_cislo, TID, nakladatelstvi, stav)
VALUES (seq_pc.nextval, 2, 'Dobrovsky', 'vyrazeno');

INSERT INTO vytisk (prirustkove_cislo, TID, nakladatelstvi, stav)
VALUES (seq_pc.nextval, 3, 'Scholastic', 'dobry');

INSERT INTO vytisk (prirustkove_cislo, TID, nakladatelstvi, stav)
VALUES (seq_pc.nextval, 5, 'Odeon', 'dobry');

INSERT INTO vytisk (prirustkove_cislo, TID, nakladatelstvi, stav)
VALUES (seq_pc.nextval, 6, 'Kosmas', 'dobry');

INSERT INTO vytisk (prirustkove_cislo, TID, nakladatelstvi, stav)
VALUES (seq_pc.nextval, 7, 'Odeon', 'dobry');


INSERT INTO vypujcka (VID, datum_vypujceni, datum_vraceni, stav, prirustkove_cislo, CC, OC_vytvoril, OC_uzavrel)
VALUES (seq_vid.nextval, TO_DATE('2016-02-28', 'yyyy-mm-dd'), DEFAULT, 'vypujceno', 1, 2, 2, DEFAULT);

INSERT INTO vypujcka (VID, datum_vypujceni, datum_vraceni, stav, prirustkove_cislo, CC, OC_vytvoril, OC_uzavrel)
VALUES (seq_vid.nextval, TO_DATE('2016-01-28', 'yyyy-mm-dd'), TO_DATE('2016-02-21', 'yyyy-mm-dd'), 'vraceno', 3, 1, 1, 2);

INSERT INTO vypujcka (VID, datum_vypujceni, datum_vraceni, stav, prirustkove_cislo, CC, OC_vytvoril, OC_uzavrel)
VALUES (seq_vid.nextval, TO_DATE('2016-03-12', 'yyyy-mm-dd'), DEFAULT, 'vypujceno', 7, 2, 3, DEFAULT);

INSERT INTO vypujcka (VID, datum_vypujceni, datum_vraceni, stav, prirustkove_cislo, CC, OC_vytvoril, OC_uzavrel)
VALUES (seq_vid.nextval, TO_DATE('2014-08-12', 'yyyy-mm-dd'), TO_DATE('2014-09-01', 'yyyy-mm-dd'), 'vraceno', 1, 2, 3, 2);

INSERT INTO vypujcka (VID, datum_vypujceni, datum_vraceni, stav, prirustkove_cislo, CC, OC_vytvoril, OC_uzavrel)
VALUES (seq_vid.nextval, TO_DATE('2016-04-04', 'yyyy-mm-dd'), DEFAULT, 'vypujceno', 8, 4, 3, DEFAULT);

INSERT INTO vypujcka (VID, datum_vypujceni, datum_vraceni, stav, prirustkove_cislo, CC, OC_vytvoril, OC_uzavrel)
VALUES (seq_vid.nextval, TO_DATE('2016-04-15', 'yyyy-mm-dd'), DEFAULT, 'vypujceno', 9, 5, 2, DEFAULT);

INSERT INTO vypujcka (VID, datum_vypujceni, datum_vraceni, stav, prirustkove_cislo, CC, OC_vytvoril, OC_uzavrel)
VALUES (seq_vid.nextval, TO_DATE('2016-04-26', 'yyyy-mm-dd'), DEFAULT, 'vypujceno', 10, 6, 1, DEFAULT);


INSERT INTO zanr (zkratka , nazev)
VALUES ('UT', 'utopie');

INSERT INTO zanr (zkratka , nazev)
VALUES ('SF', 'science fiction');

INSERT INTO zanr (zkratka , nazev)
VALUES ('RO', 'romanticke');


INSERT INTO titul_zanr (TID, zkratka)
VALUES (1, 'UT');

INSERT INTO titul_zanr (TID, zkratka)
VALUES (2, 'SF');

INSERT INTO titul_zanr (TID, zkratka)
VALUES (3, 'UT');

INSERT INTO titul_zanr (TID, zkratka)
VALUES (3, 'SF');

INSERT INTO titul_zanr (TID, zkratka)
VALUES (5, 'RO');

INSERT INTO titul_zanr (TID, zkratka)
VALUES (7, 'UT');

INSERT INTO titul_zanr (TID, zkratka)
VALUES (7, 'SF');


INSERT INTO titul_autor (TID, AC)
VALUES (1, 1);

INSERT INTO titul_autor (TID, AC)
VALUES (2, 1);

INSERT INTO titul_autor (TID, AC)
VALUES (3, 3);

INSERT INTO titul_autor (TID, AC)
VALUES (4, 3);

INSERT INTO titul_autor (TID, AC)
VALUES (5, 4);

INSERT INTO titul_autor (TID, AC)
VALUES (6, 5);

INSERT INTO titul_autor (TID, AC)
VALUES (7, 6);



INSERT INTO rezervace (TID, CC)
VALUES (1, 3);

INSERT INTO rezervace (TID, CC)
VALUES (2, 3);

INSERT INTO rezervace (TID, CC)
VALUES (3, 1);

INSERT INTO rezervace (TID, CC)
VALUES (3, 5);


------------------------
-- VYTVORENI TRIGGERU -- 
------------------------
CREATE OR REPLACE TRIGGER zmen_stav_datum
	BEFORE UPDATE OF OC_uzavrel ON vypujcka
	REFERENCING OLD AS o NEW AS n
	FOR EACH ROW
BEGIN
  SELECT 'vraceno'
  INTO :n.stav
  FROM DUAL;
  SELECT SYSDATE
  INTO :n.datum_vraceni
  FROM DUAL;
END zmen_stav_datum;
/

CREATE OR REPLACE TRIGGER nastav_prirustkove_cislo
  BEFORE INSERT ON vytisk
  REFERENCING NEW AS n
  FOR EACH ROW
BEGIN
  SELECT seq_pc.nextval
  INTO :n.prirustkove_cislo
  FROM DUAL;
END nastav_prirustkove_cislo;
/


------------------------
-- VYTVORENI PROCEDUR --
------------------------
CREATE OR REPLACE PROCEDURE odstran_stare (datum vypujcka.datum_vraceni%TYPE) AS
CURSOR v_cur IS
SELECT VID, datum_vraceni
FROM vypujcka;
radek v_cur%ROWTYPE;
BEGIN
  OPEN v_cur;
  FETCH v_cur INTO radek;  
  WHILE v_cur%found LOOP
    IF radek.datum_vraceni < datum THEN
      DELETE FROM vypujcka WHERE radek.VID=vypujcka.VID;
    END IF;
    FETCH v_cur INTO radek;
  END LOOP; 
  CLOSE v_cur;
END odstran_stare;
/

CREATE OR REPLACE PROCEDURE rezervuj (id_knihy rezervace.TID%TYPE, ctenarske_cislo rezervace.CC%TYPE) AS
BEGIN
  INSERT INTO rezervace (TID, CC)
  VALUES (id_knihy, ctenarske_cislo);
EXCEPTION
  WHEN DUP_VAL_ON_INDEX THEN
    DELETE FROM rezervace
    WHERE rezervace.TID = id_knihy AND rezervace.CC = ctenarske_cislo;
END rezervuj;
/


---------------------------------
-- VYTVORENI PRISTUPOVYCH PRAV --
---------------------------------
GRANT ALL ON titul_zanr TO xvales02;
GRANT ALL ON titul_autor TO xvales02;
GRANT ALL ON rezervace TO xvales02;
GRANT ALL ON vypujcka TO xvales02;
GRANT ALL ON vytisk TO xvales02;
GRANT ALL ON titul TO xvales02;
GRANT ALL ON autor TO xvales02;
GRANT ALL ON ctenar TO xvales02;
GRANT ALL ON zanr TO xvales02;
GRANT ALL ON knihovnik TO xvales02;


---------------------------------------------------------------------
-- VYTVORENI MATERIALIZOVANEHO POHLEDU PATRICIHO DRUHEMU UZIVATELI --
---------------------------------------------------------------------
connect xvales02/fj70543k;
CREATE MATERIALIZED VIEW ctenari_neuzavreno
BUILD IMMEDIATE
REFRESH COMPLETE
AS
   SELECT DISTINCT ctenar.jmeno, ctenar.prijmeni
   FROM xvales03.CTENAR JOIN xvales03.VYPUJCKA ON ctenar.CC = vypujcka.CC
   WHERE stav = 'vypujceno';
   
GRANT ALL ON ctenari_neuzavreno TO xvales03;

connect xvales03/4hvxovah;


---------------------------------------
-- VYUZITI MATERIALIZOVANEHO POHLEDU --
---------------------------------------
-- zobrazeni
SELECT * 
FROM xvales02.ctenari_neuzavreno;

-- vlozeni dat
INSERT INTO vypujcka (VID, datum_vypujceni, datum_vraceni, stav, prirustkove_cislo, CC, OC_vytvoril, OC_uzavrel)
VALUES (seq_vid.nextval, TO_DATE('2016-04-29', 'yyyy-mm-dd'), DEFAULT, 'vypujceno', 3, 1, 1, DEFAULT);

-- opetovne zobrazeni
SELECT * 
FROM xvales02.ctenari_neuzavreno;

-- aktualizace obsahu pohledu
connect xvales02/fj70543k;
EXECUTE DBMS_SNAPSHOT.REFRESH('ctenari_neuzavreno','COMPLETE');
connect xvales03/4hvxovah;

-- finalni zobrazeni
SELECT * 
FROM xvales02.ctenari_neuzavreno;


---------------------------
-- EXPLAIN PLAN A INDEXY --
---------------------------
EXPLAIN PLAN FOR
SELECT jmeno, prijmeni, COUNT(datum_vypujceni) pocet_vypujcek
FROM CTENAR LEFT JOIN VYPUJCKA USING (CC)
WHERE CC = 2
GROUP BY CC, jmeno, prijmeni;
select plan_table_output
from table(dbms_xplan.display('plan_table',null,'advanced'));

CREATE INDEX idx ON ctenar (CC, jmeno, prijmeni);
CREATE INDEX idx2 ON vypujcka (CC);

EXPLAIN PLAN FOR
SELECT jmeno, prijmeni, COUNT(datum_vypujceni) pocet_vypujcek
FROM CTENAR LEFT JOIN VYPUJCKA USING (CC)
WHERE CC = 2
GROUP BY CC, jmeno, prijmeni;
select plan_table_output
from table(dbms_xplan.display('plan_table',null,'advanced'));


---------------------------------
-- VYUZITI PROCEDUR A TRIGGERU --
---------------------------------
-- PRVNI TRIGGER
UPDATE vypujcka
SET OC_uzavrel = 1
WHERE VID = 1;

-- DRUHY TRIGGER
INSERT INTO vytisk (prirustkove_cislo, TID, nakladatelstvi, stav)
VALUES (null, 5, 'Odeon', 'dobry');

-- PRVNI PROCEDURA
BEGIN
  ODSTRAN_STARE(SYSDATE);
END;
/

-- DRUHA PROCEDURA
BEGIN
  REZERVUJ(3,3);
END;
/
