/*	Projekt: Hra 2016 - Othello
	Predmet: Jazyk C++
 	Datum: 6. 5. 2016
  	Autori: 	
  		Nikola Valesova, xvales02
		Ondrej Vales, xvales03
 
 	Modul: cli.cpp
*/

/** @file cli.cpp
	@brief Modul metod pro textove uzivatelske rozhrani
 
	Modul obsahujci implementace metod pro textove uzivatelske rozhrani
*/

#include "cli.h"

using namespace std;

/**
 * @brief      Metoda na vypsani aktualniho stavu hraci desky, skore a hrace, ktery je na tahu
 *
 * @param      board  Obsah hraci desky
 * @param 	   whose_turn  Identifikace, ktery hrac je na tahu
 */
void cli::print(board_t &board, int whose_turn)
{
	cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
	cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
	cout << endl << endl << "SKÓRE:\tHráč 1 : Hráč 2\t= " << board.get_score()[0] << " : " << board.get_score()[1] << endl;
	cout << "Na tahu je hráč: " << whose_turn + 1 << endl << endl;

	int size = board.get_size();
	cout << "\t";
	for (int i = -1; i < size; i++)
	{
		for (int j = -1; j < size; j++)
		{
			if (i == -1)
			{
				if (j < size - 1)
					cout << " " << static_cast<char>(j + 66);
			}
			else if (j == -1)
			{
				cout << i + 1 << "\t";
			}
			else if (board.get_board()[i * size + j] == 0)
				cout << "|" << " ";
			else if (board.get_board()[i * size + j] == 1)
				cout << "|" << "X";
			else if (board.get_board()[i * size + j] == 2)
				cout << "|" << "O";
			else if (board.get_board()[i * size + j] == 3)
				cout << "|" << ".";
		}
		if (i > -1)
			cout << "|" << endl;
		else
			cout << endl;
	}
}

/**
 * @brief      Metoda na vypsani konce hry
 *
 * @param      who  Udava kdo vyhral
 */
void cli::print_end(string who)
{
	cout << endl << "KONEC HRY" << endl;
	cout << "Vyhral hrac cislo " << who << "!";
	cin.ignore();
}

/**
 * @brief      Metoda obsahujici inicializacni dialog s hracem a nacteni preferenci
 *
 * @param      size      Velikost hraci plochy zadana uzivatelem
 * @param      opponent  Preferovana moznost protihrace
 */
int cli::init(int &size, vector <player_t> &players, string &game_name)
{
	// retezec na ulozeni vstupu od uzivatele
	string line;
	// prommena na ulozeni volby, zda chce uzivatel nacist hru, nebo vytvorit novou; 
	// promenna na ulozeni volby protihrace
	int game_type, opponent;

	// volba nacteni nebo zapoceti nove hry
	cout << "Chcete zacit novou hru, nebo nacist jiz ulozenou? (1: nova hra\t2: nacist hru)" << endl;
	while(1)
	{
		getline(cin, line);
		istringstream ss(line);
		if (!(ss >> game_type) || (game_type < 1) || (game_type > 2)) 
		{
			cout << "Neplatna moznost" << endl;
			cout << "Zvolte jinou (1, 2):" << endl;
		}
		else
			break;
	}

	// volba nazvu hry
	cout << endl << "Zadejte nazev hry:\t";
	while(1)
	{
		getline(cin, game_name);
		int i;
		for (i = 0; i < game_name.size(); i++)
		{
			if (!(isalnum(game_name[i]))) 
			{
				cout << "Nazev muze obsahovat pouze alfanumericke znaky, zadejte jiny nazev:" << endl;
				break;
			}
		}

		if (i == game_name.size())
		{
			if (game_type == 2)
			{
				ifstream f;
				f.open("./examples/" + game_name + ".save");

				if (!f.is_open())
				{
					f.close();
					cout << "Neni ulozena zadna hra s timto nazvem, zadejte jiny nazev:";
				}
				else
				{
					f.close();
					return game_type;
				}	
			}
			else
				break;
		}
	}

	// volba velikosti hraci desky
	cout << endl << "Zadejte velikost hraci desky:\t";
	while(1)
	{
		getline(cin, line);
		if (line == "")
		{
			size = 8;
			break;
		}
		else
		{
			istringstream ss(line);
			if (!(ss >> size) || (size < 6) || (size > 12) || (size % 2)) 
			{
				cout << "Neplatna velikost desky" << endl;
				cout << "Zvolte jinou (6, 8, 10, 12):" << endl;
			}
			else
				break;
		}
	}

	// volba soupere
	cout << endl << "Zvolte soupere:" << endl;
	cout << "1: druhy hrac\t2: pocitac (lehci obtiznost)\t3: pocitac (vyssi obtiznost)" << endl;
	while(1)
	{
		getline(cin, line);
		istringstream ss(line);
		if (!(ss >> opponent) || (opponent < 1) || (opponent > 3)) 
		{
			cout << "Neplatna moznost" << endl;
			cout << "Zvolte jinou (1, 2, 3):" << endl;
		}
		else
			break;
	}

	// nastaveni hodnot typu obou hracu
	players[0].set_type((int) 0);
	players[1].set_type(opponent - 1);

	return game_type;
}

/**
 * @brief      Metoda na nacteni dalsiho tahu hrace
 *
 * @param      board      Obsah hraci desky
 * @param      next_move  Zvolena pozice nasledujiciho tahu
 */
void cli::get_move(board_t &board, vector<int> &next_move, vector <vector <int>> &possible_positions)
{
	// promenna pro nacteni souradnic pozadovaneho tahu
	next_move.reserve(2);
	// promenna na nacteni vstupu od uzivatele
	string line;

	// volba tahu
	while (1)
	{
		cout << "Zadejte souradnici x (nebo pismeno 'O' pro volbu jine operace):\t";
		while(1)
		{
			getline(cin, line);
			if (line.length() == 1)
			{
				if (static_cast<int>(line[0]) >= 'A' && static_cast<int>(line[0]) < 'A' + board.get_size())
				{
					next_move[0] = static_cast<int>(line[0]) - 'A';
					break;
				}

				else if (line[0] == 'O')
				{
					cout << "Zvolte pozadovanou operaci:" << endl;
					cout << "1: krok zpet" << endl;
					cout << "2: krok vpred" << endl;
					cout << "3: konec" << endl;

					while(1)
					{
						getline(cin, line);
						istringstream ss(line);
						if (!(ss >> next_move[0]) || (next_move[0] < 1) || (next_move[0] > 3))
							cout << "Neplatna moznost, zvolte jinou:\t";
						else
							break;
					}

					if (next_move[0] == 3)
						next_move[0] = 5;

					next_move[0] *= -1;
					return;
				}
			}
			cout << "Neplatna moznost, zvolte jinou:\t";
		}

		cout << "Zadejte souradnici y:\t";
		while(1)
		{
			getline(cin, line);
			istringstream ss(line);
			if (!(ss >> next_move[1]) || (next_move[1] < 1) || (next_move[1] > board.get_size())) 
			{
				cout << "Neplatna moznost, zvolte jinou:\t";
			}
			else
			{
				next_move[1]--;
				break;
			}
		}

		for (const vector<int> &v : possible_positions)
		{
			if (v[0] == next_move[1] && v[1] == next_move[0])
				return;
		}

	    cout << "Na tuto pozici nelze vlozit kamen, zvolte jinou!" << endl;
	}
}