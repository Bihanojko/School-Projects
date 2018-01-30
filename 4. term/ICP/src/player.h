/*	Projekt: Hra 2016 - Othello
	Predmet: Jazyk C++
 	Datum: 6. 5. 2016
  	Autori: 	
  		Nikola Valesova, xvales02
		Ondrej Vales, xvales03
 
 	Modul: player.h
*/

/** @file player.h
	@brief Modul se tridou hrace
 
 	Tento modul obsahuje definici tridy harce
*/

#ifndef PLAYER_H
#define PLAYER_H

#include <string>

using namespace std;

/**
 * @brief      Trida udaju o hraci
 */
class player_t
{
	int type;
public:
	player_t();
	void set_type(int type);
	int get_type();
};

#endif