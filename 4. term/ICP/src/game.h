/*	Projekt: Hra 2016 - Othello
	Predmet: Jazyk C++
 	Datum: 6. 5. 2016
  	Autori: 	
  		Nikola Valesova, xvales02
		Ondrej Vales, xvales03
 
 	Modul: game.h
*/

/** @file game.h
	@brief Definice tridy game
 
	Modul obsahujici definici tridy game, ktera obsahuje hlavni metody pro princip hrani hry
*/

#ifndef GAME_H
#define GAME_H

#include <fstream>

#include "interface.h"
#include "ai.h"
#include "board.h"
#include "player.h"

using namespace std;

/**
 * @brief      Trida jedne rozehrane hry
 */
class game_t
{
	string name;
	board_t board;
	interface_t *interface;
	interface_ai_t *interface_ai[NO_OF_AI + 1];
	int whose_turn;
	player_t players[2];
	vector<int> next_move;
	vector<board_t> history;
	vector <vector <int>> possible_positions;
	vector <vector <int>> to_be_changed;
public:
	game_t(interface_t *interface_instance, ais &ai, int size, string game_name);
	game_t(interface_t *interface_instance, ais &ai, string game_name);
	void play(vector <player_t> &players, bool newg);
	void add_to_history(int whose_turn);
	int save(string file);
	int load(string file);
	int undo();
	int redo();
	inline bool is_PvP();
};

#endif
