/*	Projekt: Hra 2016 - Othello
	Predmet: Jazyk C++
 	Datum: 6. 5. 2016
  	Autori: 	
  		Nikola Valesova, xvales02
		Ondrej Vales, xvales03
 
 	Modul: interface.h
*/

/** @file interface.h
	@brief Modul s virtualnim tidou pro rozhrani
 
	Modul obsahuje virtulani rozhrani pro uzivatelska rozhrani, je vyuzit navrhovy vzor strategie
*/

#ifndef INTERFACE_H
#define INTERFACE_H

#include "interface_ai.h"
#include "board.h"

using namespace std;

/**
 * @brief      Virtualni trida rozhrani
 * Virtualni trida obsahujici metody implementovane uzivatelskymi rozhranimi
 */
class interface_t: public interface_ai_t
{
public:
	virtual void print(board_t &board, int whose_turn) = 0;
	virtual void print_end(string who) = 0;
	virtual int init(int &size, vector <player_t> &players, string &game_name) = 0;
};

#endif