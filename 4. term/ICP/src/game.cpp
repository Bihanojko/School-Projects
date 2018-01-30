/*	Projekt: Hra 2016 - Othello
	Predmet: Jazyk C++
 	Datum: 6. 5. 2016
  	Autori: 	
  		Nikola Valesova, xvales02
		Ondrej Vales, xvales03
 
 	Modul: game.cpp
*/

/** @file game.cpp
	@brief Implementace rozhrani tridy game
 
	Modul obsahujici implementaci metod tridy game, jako napriklad nacitani a ukladani historie a pohyb v ni
*/

#include "game.h"
#include "ai.h"
#include "player.h"

using namespace std;

/**
 * @brief      Konstruktor pro vytvoreni nove hry
 * Konstruktor vytvori novou hru zadaneho jmena, velikosti a naplni ukazatele na uzivatelske rozhrani a rozhrani umele inteligence
 *
 * @param[in]  interface_instance             Ukazatel na instanci uzivatelskeho rozhrani
 * @param[in]  ai                             Trida obsahujici umele inteligence
 * @param[in]  size                           Udava velikost hraci desky
 * @param[in]  game_name                      Udava nezev hry
 */
game_t::game_t(interface_t *interface_instance, ais &ai, int size, string game_name): 
interface(interface_instance), board(size), name(game_name)
{
	whose_turn = 0;
	interface_ai[0] = (interface_ai_t *)interface_instance;
	for (int i = 1; i <= NO_OF_AI; i++)
		interface_ai[i] = ai.get_pointer(i - 1);
}

/**
 * @brief      Konstruktor pro vytvoreni hry, ktera bude nactena z historie
 * Konstruktor vytvori hru zadaneho jmena a naplni ukazatele na uzivatelske rozhrani a rozhrani umele inteligence, velikost je pozdeji nactena ze souboru
 *
 * @param[in]  interface_instance             Ukazatel na instanci uzivatelskeho rozhrani
 * @param[in]  ai                             Trida obsahujici umele inteligence
 * @param[in]  game_name                      Udava nezev hry
 */
game_t::game_t(interface_t *interface_instance, ais &ai, string game_name): 
interface(interface_instance), name(game_name)
{
	interface_ai[0] = (interface_ai_t *)interface_instance;
	for (int i = 1; i <= NO_OF_AI; i++)
		interface_ai[i] = ai.get_pointer(i - 1);
	this->load(game_name);
}

/**
 * @brief      Metoda ve ktere je hrana hra
 * Metoda opakovane pocita mozne tahy, zobrazuje hraci desku a pta se na dalsi tah
 *
 * @param[in]  players             Udava typy hracu, kteri hru hraji
 * @param  newg                Udava zda se jedna o novou nebo nactenou hru
 */
void game_t::play(vector <player_t> &players, bool newg)
{
	next_move.reserve(2);

	// synchronizace typu hracu po nacteni hry
	if (newg)
	{
		this->players[0].set_type(players[0].get_type());
		this->players[1].set_type(players[1].get_type());
	}

	while (true)
	{
		// synchronizace typu hracu po nacteni hry
		players[0].set_type(this->players[0].get_type());
		players[1].set_type(this->players[1].get_type());

		// vypocet moznych pozic
		board.set_possible_positions(whose_turn % 2, possible_positions);
		// zobrazeni hraci desky
		interface->print(board, whose_turn % 2);

		// neni kam umistit dalsi kamen, hra zkoncila
		if (possible_positions.size() == 0)
		{
			interface->print_end((board.get_score()[0] > board.get_score()[1]) ? "1" : "2");
			break;
		}

		// novou pozici pridej do historie, opakovanou ne
		if (newg)
		{
			this->add_to_history(whose_turn);
		}
		if (!newg)
		{
			newg = !newg;
		}

		// uloz soucasny stav hryS
		this->save(name);

		// zjisti dalsi tah (v zavislosti na typu hrace na tahu volano bud umela inteligence 1, umela inteligence 2 nebo user interface)
		interface_ai[players[whose_turn % 2].get_type()]->get_move(board, next_move, possible_positions);

		// provedeni tahu
		if (next_move[0] >= 0)
		{
			board.get_changing_stones(next_move, whose_turn % 2, to_be_changed);
			board.change_stones(to_be_changed, whose_turn % 2);
			board.make_move(next_move, whose_turn % 2);
			whose_turn++;
		} else 		// 	zpet
		if (next_move[0] == -1)
		{
			this->undo();
			newg = !newg;

		} else 		// vpred
		if (next_move[0] == -2)
		{
			this->redo();
			newg = !newg;

		} else 		// konec
		if (next_move[0] == -5)
		{
			break;
		}
	}
}

/**
 * @brief      Metoda pro ukladani historie tahu
 * Metoda ulozi soucasny stav hraci desky do historie
 *
 * @param[in]  whose_turn             Udava cislo tahu
 */
void game_t::add_to_history(int whose_turn)
{
	history.resize(whose_turn, board);
	history.emplace_back();
	history[whose_turn].copy(board);
}

/**
 * @brief      Metoda pro ukladani historie do souboru
 * Metoda ulozi soucasnou historii do souboru
 *
 * @param[in]  file            Udava jemno souboru v ./examples
 * @return                     Pokud se podarilo zapsat 0 jinak 1 
 */
int game_t::save(string file)
{
	ofstream f;
	f.open("./examples/" + file + ".save");
	if (!f.is_open())
		return 1;

	f << whose_turn << endl;
	f << players[0].get_type() << endl;
	f << players[1].get_type() << endl;
	f << history.size() << endl;
	for (int i = 0; i < history.size(); i++)
	{
		f << history[i].get_size() << endl;
		for (auto j: history[i].get_board())
			f << j << " ";
		f << endl;
	}
	f.close();
	return 0;
}

/**
 * @brief      Metoda pro nacitani historie ze souboru
 * Metoda nacte historii ze souboru
 *
 * @param[in]  file            Udava jemno souboru v ./examples
 * @return                     Pokud se podarilo zapsat 0 jinak 1 
 */
int game_t::load(string file)
{
	ifstream f;
	if (file.find("/") != string::npos)
		f.open(file + ".save");
	else
		f.open("./examples/" + file + ".save");

	if (!f.is_open())
		return 1;

	f >> whose_turn;

	string auxs;
	int auxi;

	// nacteni typu hracu
	f >> auxi;
	players[0].set_type(auxi);
	f >> auxi;
	players[1].set_type(auxi);

	// nacteni historie
	f >> auxi;
	history.resize(0, board);
	for (int i = 0; i < auxi; i++)
	{
		int s, k;
		vector <int> gb;
		f >> s;	
		for (int j = 0; j < s * s; j++)
		{
			f >> k;
			gb.push_back(k);
		}
		history.emplace_back();
		history[i].copy(s, gb);
	}
	f.close();

	// nacten tah umele inteligence vedouci ke konci hry
	if (players[whose_turn % 2].get_type() != 0)
	{
		whose_turn--;
	}

	// nastaveni aktualniho stavu
	board.copy(history[whose_turn]);

	return 0;
}

/**
 * @brief      Metoda pro navraceni o tah zpet
 * Metoda se vrati na predchozi tah hrace, pokud se nelze vratit, jedna se o prazdnou operaci
 *
 * @return     Vraci 0 pokud se slo vratit jinak 1
 */
int game_t::undo()
{
	int stride = 1;
	if (!this->is_PvP())
		stride = 2;
	if (whose_turn < stride)
		return 1;

	board.copy(history[whose_turn - stride]);
	whose_turn -= stride;
	return 0;
}

/**
 * @brief      Metoda pro posun o tah vpred
 * Metoda se posune na nasledujici tah hrace, pokud se nelze posunout, jedna se o prazdnou operaci
 *
 * @return     Vraci 0 pokud se slo posunout jinak 1
 */
int game_t::redo()
{
	int stride = 1;
	if (!this->is_PvP())
		stride = 2;
	if (whose_turn + stride >= history.size())
		return 1;

	board.copy(history[whose_turn + stride]);
	whose_turn += stride;
	return 0;
}

/**
 * @brief      Metoda urcujici jestli hraji dva hraci nebo hrac s pocitacem
 * Metoda zjisti jestli hru hraji dva hraci nebo ne
 *
 * @return     Vraci true pokud se jedna o hru dvou hracu, jinak false
 */
inline bool game_t::is_PvP()
{
	return players[0].get_type() == players[1].get_type() ? true : false;
}
