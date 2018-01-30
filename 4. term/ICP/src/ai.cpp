/*	Projekt: Hra 2016 - Othello
	Predmet: Jazyk C++
 	Datum: 6. 5. 2016
  	Autori: 	
  		Nikola Valesova, xvales02
		Ondrej Vales, xvales03
 
 	Modul: ai.cpp
*/

/** @file ai.cpp
	@brief Modul implementace umele inteligence
 
 	V tomto modulu se nachazi definice metod, ktere implementuji umelou inteligenci 
 	a jeji zpusob volby nasledujiciho tahu.
*/

#include "ai.h"

/**
 * @brief      Metoda na urceni polohy dalsiho tahu umelou inteligenci cislo 1
 * Ta umistuje kamen na nahodne vybranou pozici ze vsech moznych
 *
 * @param      board               adresa tridy obsahujici herni desku a operace s ni
 * @param      next_move           vektor dvou integeru, ve kterem se vraci pozice vybraneho tahu
 * @param 	   possible_positions  vektor pozic, na ktere jde umistit kamen
 */
void ai1_t::get_move(board_t &board, vector<int> &next_move, vector <vector <int>> &possible_positions)
{
	// pocet policek, na ktera jde aktualne umistit kamen
	int count = possible_positions.size();
	
	// urceni pozice tahu
	// vypocteni nahodneho cisla a prevedeni do rozsahu 0 - pocet moznych tahu
	srand(time(NULL));
	int pos = rand() % count;

	// urceni presnych souradnic pos-teho mozneho tahu
	next_move[0] = possible_positions[pos][1];
	next_move[1] = possible_positions[pos][0];
}

/**
 * @brief      Metoda na urceni polohy dalsiho tahu umelou inteligenci cislo 2
 * Ta umistuje kamen na takove policko, kterym zmeni nejvic souperovych kamenu ve sve
 *
 * @param      board      adresa tridy obsahujici herni desku a operace s ni
 * @param      next_move  vektor dvou integeru, ve kterem se vraci pozice vybraneho tahu
 * @param      possible_positions  vektor vsech pozic, na ktere lze umistit kamen
 */
void ai2_t::get_move(board_t &board, vector<int> &next_move, vector <vector <int>> &possible_positions)
{
	// vektor pozic kamenu, ktere by po danem tahu bylo potreba zmenit
	vector <vector<int>> to_be_changed;
	// kolikaty tah zmeni nejvice kamenu
	int pos;
	// nejvyssi pocet kamenu, ktere je treba zmenit
	int max_count = 0;
	// pocitadlo pro urceni pozice tahu
	int counter = 0;

	// pro kazdy mozny tah vyzkousi, kolik kamenu by zmenil, a uchova ten "nejlepsi"
	for (int i = 0; i < possible_positions.size(); i++)
	{
		board.get_changing_stones(possible_positions[i], 1, to_be_changed);
		if (to_be_changed.size() > max_count)
		{
			max_count = to_be_changed.size();
			pos = i;
		}
	}

	// nalezeni pos-teho mozneho tahu a vraceni jeho souradnic
	for (const vector<int> &v : possible_positions)
	{
		if (counter == pos)
		{
			next_move[0] = v[1];
			next_move[1] = v[0];
			return;
		}
		counter++;
	}
}


/**
 * @brief      Konstruktor tridy ais
 * Konstruktor naplni pole ukazatelu na umele inteligence
 */
ais::ais()
{
	interface_ai[0] = &ai1;
	interface_ai[1] = &ai2;
}

/**
 * @brief      Metoda poskytujici ukazatele na instance umelych inteligenci
 * Metoda vraci ukazatel na instanci umele inteligence specifikovane indexem
 *
 * @param      i             index umele inteligence
 * @return		             ukazatel na instanci umele inteligence
 */
interface_ai_t * ais::get_pointer(int i)
{
	return interface_ai[i];
}