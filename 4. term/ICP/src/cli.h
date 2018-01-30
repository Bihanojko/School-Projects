/*	Projekt: Hra 2016 - Othello
	Predmet: Jazyk C++
 	Datum: 6. 5. 2016
  	Autori: 	
  		Nikola Valesova, xvales02
		Ondrej Vales, xvales03
 
 	Modul: cli.h
*/

/** @file cli.h
	@brief Modul definujici tridu pro hrani aplikace s textovym uzivatelskym rozhranim
 
 	Tento modul obsahuje definici tridy cli, ktera implementuje metody pro aplikaci s textovym uzivatelskym rozhranim
*/

#ifndef CLI_H
#define CLI_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <cctype>

#include "interface.h"
#include "board.h"

using namespace std;

/**
 * @brief      Trida obsahujici udaje o textovem rozhrani
 */
class cli : public interface_t
{
public:	
	void print(board_t &board, int whose_turn);
	void print_end(string who);
	int init(int &size, vector <player_t> &players, string &game_name);
	void get_move(board_t &board, vector<int> &next_move, vector <vector <int>> &possible_positions);
};

#endif
