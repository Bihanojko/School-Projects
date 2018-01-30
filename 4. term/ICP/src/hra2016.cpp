/*	Projekt: Hra 2016 - Othello
	Predmet: Jazyk C++
 	Datum: 6. 5. 2016
  	Autori: 	
  		Nikola Valesova, xvales02
		Ondrej Vales, xvales03
 
 	Modul: hra2016.cpp
*/

/** @file hra2016.cpp
	@brief Zdrojovy soubor hry s cli rozhranim
 
	Obsahuje vytvoreni rozhrani cli a instance hry	
*/

#include <iostream>
#include <vector>

#include "game.h"
#include "cli.h"

using namespace std;


/**
 * @brief      Funkce rozehrani hry s cli rozhranim
 */
int main()
{
	int size, type;
	string game_name;
	vector <player_t> players;
	players.reserve(2);

	cli interface_cli;
	ais ai;
	// uvodni rozhovor
	type = interface_cli.init(size, players, game_name);

	if (type == 1)	// nova hra
	{
		game_t game (&interface_cli, ai, size, game_name);
		game.play(players, true);
	}
	else	// nacteni hry
	{
		game_t game (&interface_cli, ai, game_name);
		game.play(players, false);
	}

	return 0;
}
