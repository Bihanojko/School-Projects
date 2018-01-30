/*	Projekt: Hra 2016 - Othello
	Predmet: Jazyk C++
 	Datum: 6. 5. 2016
  	Autori: 	
  		Nikola Valesova, xvales02
		Ondrej Vales, xvales03
 
 	Modul: board.h
*/

/** @file board.h
	@brief Modul rozhrani hraci desky
 
 	Tento modul obsahuje definici tridy hraci deska
*/


#ifndef BOARD_H
#define BOARD_H

#include "player.h"

#include <iostream>
#include <vector>

using namespace std;

/**
 * @brief      Trida udaju o hraci desce
 */
class board_t
{
	int size;
	vector<int> game_board;
public:
	board_t(int value);
	board_t();
	int get_size();
	const vector<int> &get_board();
	const vector<int> get_score();
	void set_possible_positions(int player, vector <vector <int>> &possible_positions);
	void copy(const board_t &board);
	void copy(const int s, const vector<int> &gb);
	void make_move(const vector<int> &next_move, const int &player);
	void get_changing_stones(const vector<int> &next_move, const int &player, vector <vector<int>> &to_be_changed);
	void change_stones(const vector <vector<int>> &to_be_changed, const int &player);
};

#endif
