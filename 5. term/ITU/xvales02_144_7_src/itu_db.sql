DROP TABLE IF EXISTS Recenze;
DROP TABLE IF EXISTS Titul;
DROP TABLE IF EXISTS Autor;

CREATE TABLE Autor
(
	IDAutor int PRIMARY KEY NOT NULL AUTO_INCREMENT,
	Jmeno varchar(255) NOT NULL,
	Prijmeni varchar(255) NOT NULL
) ENGINE=InnoDB;

CREATE TABLE Titul
(
	IDTitul int PRIMARY KEY NOT NULL AUTO_INCREMENT,
	Nazev varchar(255) NOT NULL,
	IDAutor int NOT NULL,
	Zanr varchar(255) NOT NULL,
	FOREIGN KEY (IDAutor) REFERENCES Autor (IDAutor)
) ENGINE=InnoDB; 

CREATE TABLE Recenze
(
	IDRecenze int PRIMARY KEY NOT NULL AUTO_INCREMENT,
	Obsah varchar(255) NOT NULL,
	IDTitul int NOT NULL,
	FOREIGN KEY (IDTitul) REFERENCES Titul (IDTitul)
) ENGINE=InnoDB; 



INSERT INTO Autor (Jmeno, Prijmeni) VALUES
('George R. R.', 'Martin'),
('Oscar', 'Wilde'),
('Karel', 'Čapek'),
('Alois', 'Jirásek'),
('Jane', 'Austen'),
('Emily', 'Brontë'),
('Božena', 'Němcová'),
('Joseph', 'Heller'),
('Ray', 'Bradbury'),
('Francis Scott', 'Fitzgerald'),
('George', 'Orwell');

INSERT INTO Titul (Nazev, IDAutor, Zanr) VALUES
('Hra o trůny', '1', 'fantasy'),
('Obraz Doriana Graye', '2', 'román'),
('Strašidlo cantervillské', '2', 'román'),
('Krakatit', '3', 'román'),
('Bílá nemoc', '3', 'drama'),
('R. U. R.', '3', 'drama'),
('Psohlavci', '4', 'román'),
('Pýcha a předsudek', '5', 'román'),
('Na větrné hůrce', '6', 'román'),
('Karla', '7', 'román'),
('Babička', '7', 'román'),
('Hlava XXII', '8', 'válečné'),
('451 stupňů Fahrenheita', '9', 'fantasy'),
('Velký Gatsby', '10', 'román'),
('1984', '11', 'antiutopie'),
('Farma zvířat', '11', 'román');

INSERT INTO Recenze (Obsah, IDTitul) VALUES
('Hodně syrově podaná fantasy.', '1'),
('Skvělá kniha. Začala jsem číst další díl.', '1'),
('Obraz Doriana Graye asi nikdy nepřestanu doporučovat. Pro Wildeovo slovo a jazyk jakožto nástroj umění, pro brilantní závěr, pro mistrovskou formu i obsah.', '2'),
('Zajímavá pohádka s hororovými prvky a obrázky, ve stylu Oscara Wildea, který mě opět nezklamal.', '3'),
('Krakatit se mi četl velmi těžko. Chtěla jsem vědět, jak to dopadne, tak jsem dočetla (ostatně já dočítám všechny knihy), ale podruhé už určitě nikdy.', '4'),
('Krásná kniha stejně jako ostatní díla. V době Čapka to byla utopie a za pár desítek let to bude realita.', '4'),
('Fajnovka z povinné četby. Knihu doporučuji i když se spoustu lidem moc nelíbí, nedocení ji.', '5'),
('Patří mezi mé nejvzácnější, mám ji od babičky.', '6'),
('Velmi dobrý příběh, nečekala jsem, že mě to bude tak bavit. Měla jsem to přečtené opravdu rychle.', '7'),
('Nejsem žádný milovník romantických filmů ani knih, ale tato kniha je nádherná, živá a poutavá.', '8'),
('Moje oblíbená kniha, jedna z těch, na kterou se nedá zapomenout.', '9'),
('Nádherná kniha pěti povídek od Boženy Němcové, kterou jsem zdědila po svém dědečkovi.', '10'),
('Babička je kniha, která se hned po prvním přečtení stala mojí srdcovkou a rozhodně si ji přečtu znovu. ', '11'),
('Krásně a úsměvně popsaná absurdita války. ', '12'),
('Kniha napěchovaná hlubšími významy a myšlenkami, nad kterými se člověk občas musel pozastavit.', '13'),
('Krásný čtivý a napínavý příběh na jehož konci nechcete uvěřit, co se všechno stalo.', '14'),
('Nádherně tísnivý kousek. Kupodivu se většinou mluví jen o její první polovině, kde je popisováno uspořádání totalitního mlýnku na lidi.', '15'),
('Knížku jsem nemohla (a ani nechtěla) odložit.', '16');
