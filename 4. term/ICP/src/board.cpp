/*	Projekt: Hra 2016 - Othello
	Predmet: Jazyk C++
 	Datum: 6. 5. 2016
  	Autori: 	
  		Nikola Valesova, xvales02
		Ondrej Vales, xvales03
 
 	Modul: board.cpp
*/

/** @file board.cpp
	@brief Modul operaci s hraci deskou
 
 	Modul obsahujici implementace metod tridy hraci deska, jeji konstruktor a destruktor a dalsi metody
*/

#include "board.h"

using namespace std;

/**
 * @brief      Konstruktor pro novou hraci desku
 * Konstruktor vytvori hraci desku v pocatecnim stavu
 *
 * @param[in]  value              Udava velikost hraci desky
 */
board_t::board_t(int value)
{
	size = value;
	for (int i = 0; i < size * size; i++)
	{
		game_board.push_back(0);
	}
	int half = size / 2;
	game_board[half * size + half] = 1;
	game_board[half * size + half - 1] = 2;
	game_board[(half - 1) * size + half] = 2;
	game_board[(half - 1) * size + half - 1] = 1;
}

/**
 * @brief      Konstruktor pro prazdnou hraci desku
 * Konstruktor vytvori prazdnou hraci, do krete je potom nacten obsah z historie
 */
board_t::board_t()
{	
}

/**
 * @brief      Metoda na zjisteni velikosti hraci desky
 *
 * @return     Velikost hraci desky
 */
int board_t::get_size()
{
	return size;
}

/**
 * @brief      Metoda na ziskani obsahu hraci desky
 *
 * @return     Obsah hraci desky
 */
const vector<int> & board_t::get_board()
{
	return game_board;
}

/**
 * @brief      Metoda na ziskani aktualniho poctu kamenu prvniho i druheho hrace
 *
 * @return     Vektor obsahujici pocet kamenu obou hracu
 */
const vector<int> board_t::get_score()
{
	// vektor dvou integeru na vypocteni skore
	vector<int> game_score;
	game_score.reserve(2);

	// inicializace
	game_score[0] = 0;
	game_score[1] = 0;

	// pro kazde policko hraci desky
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			// pokud se na nem nachazi kamen prvniho hrace, inkrementujeme jeho skore
			if (game_board[i * size + j] == 1)
				game_score[0]++;
			// pokud se na nem nachazi kamen protihrace, inkrementujeme jeho skore
			else if (game_board[i * size + j] == 2)
				game_score[1]++;			
		}
	}

	return game_score;
}

/**
 * @brief      Metoda, ktera urci pozice na hraci desce, na ktere muze hrac umistit svuj dalsi tah
 *
 * @param[in]  player              Identifikace hrace, ktery je prave na tahu
 * @param[in]  possible_positions  Vektor souradnic, na ktere jde v nadchazejicim tahu umistit kamen
 */
void board_t::set_possible_positions(int player, vector <vector <int>> &possible_positions)
{
	// pole obsahujici vsechny mozne smery, ve kterych lze po hraci desce vyhledavat
	int directions[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};
	// ciselne oznaceni kamenu hrace na tahu, ciselne oznaceni kamenu protihrace
	// prubezna x-ova souradnice, prubezna y-ova souradnice
	int my_stone, opp_stone, x_coord, y_coord;

	possible_positions.clear();
	vector<int> tmp;

	if (player == 0)
	{
		my_stone = 1;
		opp_stone = 2;
	}
	else
	{
		my_stone = 2;
		opp_stone = 1;
	}

	// zruseni oznacenych pozic, na ktere lze umistit kamen, z minuleho tahu
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (game_board[i * size + j] == 3)
				game_board[i * size + j] = 0;
		}
	}

	// nalezeni novych pozic pro aktualni tah
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			// pokud je na teto pozici na herni desce kamen hrace na tahu
			if (game_board[i * size + j] == my_stone)
			{
				// prohleda se okoli tohoto kamene v kazdem smeru 
				for (int k = 0; k < 8; k++)
				{
					x_coord = i + directions[k][0];
					y_coord = j + directions[k][1];

					// kontrola mezi
					if (x_coord < 0 || y_coord < 0 || x_coord >= size || y_coord >= size)
						break;

					// pokud je hned vedle kamen protihrace
					if (game_board[x_coord * size + y_coord] == opp_stone)
					{
						// dokud jsou dalsi kameny v danem smeru take protihracovy, postupuje se dale 
						while (game_board[x_coord * size + y_coord] == opp_stone)
						{
							x_coord += directions[k][0];
							y_coord += directions[k][1];

							if (x_coord < 0 || y_coord < 0 || x_coord >= size || y_coord >= size)
								break;
						}
						
						// pokud cyklus neskoncil prekrocenim mezi a je-li dalsi pozice volna,
						// nastavi se jako mozny tah
						if (x_coord >= 0 && y_coord >= 0 && x_coord < size && y_coord < size)
						{
							if (game_board[x_coord * size + y_coord] == 0)
							{
								game_board[x_coord * size + y_coord] = 3;
								tmp.clear();
								tmp.push_back(x_coord);
								tmp.push_back(y_coord);
								possible_positions.push_back(tmp);
							}
						}
					}
				}
			}
		}
	}
}

/**
 * @brief      Metoda, ktera naplni instanci tridy board_t daty
 * Jako zdroj dat pouziva jinou instanci teze tridy, pouziva se pro kopirovani obsahu historie
 *
 * @param[in]  board  Instance hraci desky, ze ktere jsou zkopirovana data
 */
void board_t::copy(const board_t &board)
{
	size = board.size;
	game_board.resize(0);
	for (int i = 0; i < size * size; i++)
		game_board.push_back(board.game_board[i]);
}

/**
 * @brief      Metoda, ktera naplni instanci tridy board_t daty
 * Jako zdroj dat pouziva jinou instanci teze tridy, pouziva se pro nacitani historie ze souboru
 *
 * @param[in]  s  		Velikost hraci desky
 * @param[in]  gp     	Obsah hraci desky
 */
void board_t::copy(const int s, const vector<int> &gb)
{
	size = s;
	game_board.resize(0);
	for (int i = 0; i < size * size; i++)
		game_board.push_back(gb[i]);
}

/**
 * @brief      Metoda, ktera provede pozadovany tah
 *
 * @param[in]  next_move  Souradnice pozadovaneho tahu
 * @param[in]  player     Identifikace hrace, ktery tah provadi
 */
void board_t::make_move(const vector<int> &next_move, const int &player)
{
	// oznaceni kamene hrace na tahu
	int my_stone;
	// prirazeni identifikace kamenu tohoto hrace
	if (player == 0)
		my_stone = 1;
	else
		my_stone = 2;

	// pridani kamene
	game_board[next_move[1] * size + next_move[0]] = my_stone;
}

/**
 * @brief      Metoda na urceni pozic kamenu mezi nove pridavanym a dalsimi kameny tehoz hrace, ktere maji byt zmeneny
 *
 * @param[in]  next_move  Vektor integeru obsahujici zvolenou pozici nasledujiciho tahu
 * @param[in]  player 	  Identifikace hrace, ktery je prave na tahu
 * @param[in]  to_be_changed  Vektor pozic, na kterych ma byt zmenen kamen
 */
void board_t::get_changing_stones(const vector<int> &next_move, const int &player, vector <vector<int>> &to_be_changed)
{
	// pole smeru, ve kterych lze vyhledavat
	int directions[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};
	// identifikace kamenu hrace na tahu, protihrace, prubezna x-ova a y-ova souradnice
	int my_stone, opp_stone, x_coord, y_coord;
	to_be_changed.clear();
	// vektor souradnic, na kterych bude potencionalne potreba zmenit kamen
	vector <vector<int>> possible_change;
	vector <int> tmp;

	// identifikace oznaceni kamenu hrace na tahu a jeho protihrace
	if (player == 0)
	{
		my_stone = 1;
		opp_stone = 2;
	}
	else
	{
		my_stone = 2;
		opp_stone = 1;
	}

	// v kazdem moznem smeru od vkladaneho kamene se posuneme o jedno policko 
	for (int i = 0; i < 8; i++)
	{
		x_coord = next_move[0] + directions[i][0];
		y_coord = next_move[1] + directions[i][1];

		// kontrola mezi
		if (!(x_coord < 0 || y_coord < 0 || x_coord >= size || y_coord >= size))
		{
			// jestlize je na aktualni pozici protihracuv kamen
			if (game_board[y_coord * size + x_coord] == opp_stone)
			{
				possible_change.clear();
				// posunuj se v danem smeru tak dlouho, dokud bude na aktualni pozici souperuv kamen 
				while (game_board[y_coord * size + x_coord] == opp_stone)
				{
					tmp.clear();
					tmp.push_back(x_coord);
					tmp.push_back(y_coord);
					possible_change.push_back(tmp);

					x_coord += directions[i][0];
					y_coord += directions[i][1];

					// kontrola mezi
					if (x_coord < 0 || y_coord < 0 || x_coord >= size || y_coord >= size)
						break;
				}
				
				// jestlize predchozi cyklus neskoncil prekrocenim mezi
				if (x_coord >= 0 && y_coord >= 0 && x_coord < size && y_coord < size)
				{
					// jestlize se na poli za souperovym kamenem nachazi kamen hrace na tahu
					if (game_board[y_coord * size + x_coord] == my_stone)
					{
						// uloz pozice souperovych kamenu mezi dvema kameny hrace na tahu
						for (int j = 0; j < possible_change.size(); j++)
							to_be_changed.push_back(possible_change[j]);
					}
				}
			}
		}
	}
}

/**
 * @brief      Metoda na zmeneni kamenu mezi nove pridavanym a dalsimi kameny tehoz hrace
 *
 * @param[in]  to_be_changed  Vektor pozic, na kterych ma byt zmenen kamen
 * @param[in]  player 	  Identifikace hrace, ktery je prave na tahu  
 */
void board_t::change_stones(const vector <vector<int>> &to_be_changed, const int &player)
{
	// identifikace kamene hrace na tahu
	int my_stone;
	if (player == 0)
		my_stone = 1;
	else
		my_stone = 2;

	// na vsech pozicich zmen vlastnika kamenu na hrace na tahu
	for (int i = 0; i < to_be_changed.size(); i++)
		game_board[to_be_changed[i][1] * size + to_be_changed[i][0]] = my_stone;
}
