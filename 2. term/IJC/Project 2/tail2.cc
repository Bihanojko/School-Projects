/** Soubor: tail2.cc
* Reseni IJC-DU2, priklad 1 b)
* Datum: 24. 4. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: g++ 4.5
* Implementace POSIX prikazu tail v C++11
**/


#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <queue>


using namespace std;

const long int LINES_COUNT = 10;	//implicitni pocet tisknutych radku



//funkce vypise chybove hlaseni
void print_error(const char *hlaseni)
{
    cerr << hlaseni << endl;
}


//funkce na vypis nekolika poslednich radku
void print_lines(istream &std_input, long int count)
{
    queue<string> buffer;
    string input_line;
    long int pos = 0;

    if (count < 0)		//kontrola volby poctu radku
    {
	print_error("Zaporny argument poctu radku!");
	return;
    }

    while (getline(std_input, input_line) != 0)   		//dosazen konec souboru, ukonci nacitani
    {
        buffer.push(input_line);
        if (pos >= count)        //prekroceni poctu vypisovanych radku
            buffer.pop();     //vyhodime nejstarsi radek
	pos++;
    }

    if (pos < count)		//bylo nacteno mene radku nez chceme vypsat
	count = pos;

    for (long int i = 0; i < count; i++)
    {
	cout << buffer.front() << endl;
	buffer.pop();	
    }

    return;
}



//funkce na vypis od daneho radku
void print_from_line(istream &std_input, long int count)
{
    string input_line;
    long int i;			//pocitadlo cyklu for

    if (count < 0)		//kontrola volby poctu radku
    {
	print_error("Zaporny argument poctu radku!");
	return;
    }

    for (i = 0; i < (count - 1); )	//pro kazdy radek, ktery chceme preskocit
    {
	if (getline(std_input, input_line) == 0)
	{
	    count = i;
	    break;
	}
	else				//jinak inkrementuj pocet nactenych radku
	    i++;
    }

    while (getline(std_input, input_line) != 0)		//nacti platny radek
	cout << input_line << endl;

    return;
}


//volani funkci na zpracovani argumentu a vypis radku
int main(int argc, char *argv[])
{
    ios::sync_with_stdio(false);
    fstream fr;
    istream *std_input = &cin;
    long int count = LINES_COUNT;			//pozadovany pocet radku, implicitne 10
    string param_count;
    string params;
    bool ok = false;

    if (argc == 2)		//spusteni: tail soubor
    {
	ok = true;
	fr.open(argv[1], ios::in);
	if (!fr.is_open())
	{
	    print_error("Neuspesne otevreni vstupniho souboru!");
	    return 1;
	}
	std_input = &fr;
    }

    if (argc == 1 || argc == 2)		//spusteni: tail [soubor]
    {
        ok = true;
        print_lines(*std_input, count);
    }

    if (argc > 2)
    {
    	params = argv[1];

    	if (params.compare("-n") == 0)
    	{
    		ok = true;
		param_count = argv[2];
		istringstream ssi (param_count, istringstream::in);
		ssi >> count;
        	if (count == 0)
        	{
        	    print_error("Spatne zadany ciselny argument!");
		    return 1;
        	}

		if (argv[2][0] == '+')
		{
		    if (argc == 4)	//spusteni: tail -n +x soubor
		    {
		    	fr.open(argv[3], ios::in);
		    	if (!fr.is_open())
			{
		    	    print_error("Neuspesne otevreni vstupniho souboru!");
			    return 1;
			}
		    	std_input = &fr;
		    }
	
		    if (argc == 3 || argc == 4)	//spusteni: tail -n +x [soubor]	
		    	print_from_line(*std_input, count);
		}

		else
		{
		    if (argc == 4)	//spusteni: tail -n x soubor
		    {
			fr.open(argv[3], ios::in);
		    	if (!fr.is_open())
			{
			    print_error("Neuspesne otevreni vstupniho souboru!");
			    return 1;
			}		    
		    	std_input = &fr;
		    }
	
		    if (argc == 3 || argc == 4)	//spusteni: tail -n x [soubor]
		    	print_lines(*std_input, count);
        	}

    	}
    }

    if (!ok)					    //chybovy stav
        print_error("Spatne zadane argumenty!");

    if (std_input != &cin)    
	fr.close();					//uzavreni souboru

    return 0;
}
