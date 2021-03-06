/*
# Autori: Ondrej Vales xvales03, Nikola Valesova xvales02
# Predmet: IMS 2016/2017
# Projekt: Simulace prubehu voleb
*/

#ifndef SIM_FILE
#define SIM_FILE

#include "defines.h"

// District populations
unsigned int Population[DISTRICT_COUNT] =
{
	153227,
	47796,
	74514,
	57340,
	40920,
	56755,
	83305,
	87302,
	306752,
	170267,
	94234,
	128451,
	73308,
	92166,
	71679,
	91799,
	73455,
	76802,
	90175,
	58422,
	91528,
	96244,
	131381,
	63808,
	89626,
	63527,
	96270,
	82746,
	71793,
	135649,
	60079,
	77994,
	173941,
	210893,
	123968,
	145063,
	266034,
	32489,
	189383,
	88798,
	107671,
	100137,
	84437,
	134199,
	84427,
	110438,
	48385,
	69900,
	146802,
	49543,
	62444,
	38355,
	41287,
	77545,
	70103,
	128762,
	78202,
	60337,
	83214,
	94943,
	75475,
	122206,
	99754,
	92238,
	44022,
	104983,
	99326,
	95353,
	69641,
	90777,
	101998,
	94522,
	87777,
	117671,
	117855,
	157154
};

// Booths stores capacities
unsigned long Capacity[DISTRICT_COUNT] =
{
	307,
	79,
	215,
	147,
	109,
	196,
	207,
	196,
	350,
	250,
	115,
	161,
	132,
	200,
	112,
	140,
	95,
	211,
	218,
	194,
	248,
	266,
	252,
	180,
	185,
	144,
	165,
	149,
	120,
	194,
	118,
	141,
	212,
	256,
	166,
	206,
	338,
	49,
	305,
	180,
	208,
	190,
	217,
	260,
	188,
	211,
	138,
	231,
	208,
	177,
	147,
	101,
	85,
	217,
	127,
	211,
	167,
	162,
	146,
	222,
	158,
	176,
	128,
	226,
	120,
	151,
	118,
	217,
	167,
	124,
	131,
	138,
	166,
	151,
	151,
	217
};

// Facilities names
const char *Name[DISTRICT_COUNT] =
{
	"České Budějovice",
	"Český Krumlov",
	"Jindřichův Hradec",
	"Písek",
	"Prachatice",
	"Strakonice",
	"Tábor",
	"Blansko",
	"Brno-město",
	"Brno-venkov",
	"Břeclav",
	"Hodonín",
	"Vyškov",
	"Znojmo",
	"Cheb",
	"Karlovy Vary",
	"Sokolov",
	"Havlíčkův Brod",
	"Jihlava",
	"Pelhřimov",
	"Třebíč",
	"Žďár nad Sázavou",
	"Hradec Králové",
	"Jičín",
	"Náchod",
	"Rychnov nad Kněžnou",
	"Trutnov",
	"Česká Lípa",
	"Jablonec nad Nisou",
	"Liberec",
	"Semily",
	"Bruntál",
	"Frýdek-Místek",
	"Karviná",
	"Nový Jičín",
	"Opava",
	"Ostrava-město",
	"Jeseník",
	"Olomouc",
	"Prostějov",
	"Přerov",
	"Šumperk",
	"Chrudim",
	"Pardubice",
	"Svitavy",
	"Ústí nad Orlicí",
	"Domažlice",
	"Klatovy",
	"Plzeň-město",
	"Plzeň-jih",
	"Plzeň-sever",
	"Rokycany",
	"Tachov",
	"Benešov",
	"Beroun",
	"Kladno",
	"Kolín",
	"Kutná Hora",
	"Mělník",
	"Mladá Boleslav",
	"Nymburk",
	"Praha-východ",
	"Praha-západ",
	"Příbram",
	"Rakovník",
	"Děčín",
	"Chomutov",
	"Litoměřice",
	"Louny",
	"Most",
	"Teplice",
	"Ústí nad Labem",
	"Kroměříž",
	"Uherské Hradiště",
	"Vsetín",
	"Zlín"
};

#endif
