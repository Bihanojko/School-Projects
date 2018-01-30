/*	Projekt: Hra 2016 - Othello
	Predmet: Jazyk C++
 	Datum: 6. 5. 2016
  	Autori: 	
  		Nikola Valesova, xvales02
		Ondrej Vales, xvales03
 
 	Modul: ai.h
*/

/** @file ai.h
	@brief Modul rozhrani umele inteligence
 
 	Tento modul obsahuje prototypy metod spojenych s umelou inteligenci
 	a definici dvou trid, ktere dedi od obecneho rozrani umelych inteligenci
*/

#ifndef AI_H
#define AI_H

#include <time.h>
#include <stdlib.h>

#include "interface_ai.h"
#include "board.h"

using namespace std;

#define NO_OF_AI 2

/**
 * @brief      Trida implementujici umelou inteligenci cislo 1
 */
class ai1_t : public interface_ai_t
{
public:	
	void get_move(board_t &board, vector<int> &next_move, vector <vector <int>> &possible_positions);
};

/**
 * @brief      Trida implementujici umelou inteligenci cislo 2
 */
class ai2_t : public interface_ai_t
{
public:	
	void get_move(board_t &board, vector<int> &next_move, vector <vector <int>> &possible_positions);
};

/**
 * @brief      Trida obsahujici instance umelych inteligenci
 */
class ais
{
	ai1_t ai1;
	ai2_t ai2;
	interface_ai_t *interface_ai[NO_OF_AI];
public:
	ais();
	interface_ai_t *get_pointer(int i);
};

#endif