/*	Projekt: Hra 2016 - Othello
	Predmet: Jazyk C++
 	Datum: 6. 5. 2016
  	Autori: 	
  		Nikola Valesova, xvales02
		Ondrej Vales, xvales03
 
 	Modul: player.cpp
*/

/** @file player.cpp
	@brief Modul operaci s hracem
 
 	Modul obsahujici implementace metod tridy hrac, jeji konstruktor a destruktor a dalsi metody
*/

#include "player.h"

/**
 * @brief      Konstruktor instance hrac
 * Konstruktor vytvori hrace s implicitnim typem
 */
player_t::player_t()
{
	type = 1;
}

/**
 * @brief      Metoda na nastaveni typu hrace
 *
 * @param[in]  type     Typ hrace
 */
void player_t::set_type(int type)
{
	this->type = type;
}

/**
 * @brief      Metoda na zjisteni typu hrace
 *
 * @return     Typ hrace
 */
int player_t::get_type()
{
	return type;
}