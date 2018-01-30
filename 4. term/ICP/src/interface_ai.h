/*	Projekt: Hra 2016 - Othello
	Predmet: Jazyk C++
 	Datum: 6. 5. 2016
  	Autori: 	
  		Nikola Valesova, xvales02
		Ondrej Vales, xvales03
 
 	Modul: interface_ai.h
*/

/** @file interface_ai.h
	@brief Modul s virtualnim tidou pro umele inteligence
 
	Modul obsahuje virtulani rozhrani pro umele inteligence, je vyuzit navrhovy vzor strategie
*/

#ifndef INTERFACE_AI_H
#define INTERFACE_AI_H

#include "board.h"

using namespace std;

/**
 * @brief      Virtualni trida pro rozhrani umele inteligence
 * Virtualni trida obsahujici metody implementovane umelymi inteligencemi
 */
class interface_ai_t
{
public:
	virtual void get_move(board_t &board, vector<int> &next_move, vector <vector <int>> &possible_positions) = 0;
};

#endif