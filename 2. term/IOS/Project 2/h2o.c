/** Soubor: h2o.c
* IOS, Projekt 2
* Datum: 3. 5. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Implementace reseni paralelne bezicich procesu 
* pri vytvareni molekuly vody
**/


#include "h2o.h"


// pocet molekul (atomu kysliku)
long int molecules;


// hlavni funkce
int main(int argc, char *argv[])
{
    Params my_params;		// struktura pro uchovani parametru
    int errnum = 0; 		// cislo chyboveho stavu
    errnum = checkparams(argc, argv, &my_params);	//zpracuj parametry

    if (errnum != 0)		// kontrola uspesnosti zpracovani
    {
	fprintf(stderr, "Spatne zadane parametry!\n");
	exit (1);
    }

    // otevreni vystupniho souboru, overeni uspesnosti
    if ((my_params.fw = fopen("h2o.out", "w")) == NULL)
    {
	fprintf(stderr, "Neuspesne otevreni souboru h2o.out.\n");
	exit (2);
    }

    // vypnuti bufferovaneho vstupu
    setbuf(my_params.fw, NULL);

    // pole pro uchovani pid procesu kysliku
    pid_t *oxygen_pid = malloc(my_params.o_count * sizeof (*oxygen_pid));
    if (oxygen_pid == NULL)
    {
        fprintf(stderr, "Neuspesna alokace pameti.\n");
        fclose(my_params.fw);
        exit (2);
    }

    // pole pro uchovani pid procesu vodiku
    pid_t *hydrogen_pid = malloc(my_params.o_count * 2 * sizeof (*hydrogen_pid));
    if (hydrogen_pid == NULL)
    {
        fprintf(stderr, "Neuspesna alokace pameti.\n");
        fclose(my_params.fw);
        free(oxygen_pid);
        exit (2);
    }

    // namapovani sdilene pameti a overeni uspesnosti
    Shared_Mem shared_memory;
    shared_memory.print_counter = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    shared_memory.o_counter = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    shared_memory.h_counter = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    shared_memory.to_bond = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    shared_memory.h_ready = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    shared_memory.finished = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_memory.print_counter == MAP_FAILED ||
        shared_memory.o_counter == MAP_FAILED ||
        shared_memory.h_counter == MAP_FAILED ||
	shared_memory.to_bond == MAP_FAILED ||
	shared_memory.h_ready == MAP_FAILED ||
	shared_memory.finished == MAP_FAILED)
    	{
	    fprintf(stderr, "Chyba pri mapovani pameti.\n");
	    munmap(shared_memory.print_counter, 6 * sizeof (long int));
	    free(oxygen_pid);
	    free(hydrogen_pid);
	    fclose(my_params.fw);
	    exit (2);
	}

    // inicializace sdilene pameti
    *shared_memory.print_counter = 1;
    *shared_memory.o_counter = 0;
    *shared_memory.h_counter = 0;
    *shared_memory.to_bond = 0;
    *shared_memory.h_ready = 0;
    *shared_memory.finished = 0;

    // inicializace struktury semaforu
    Synch_molecule synch = {.wrt = NULL, .operation = NULL,
	.oxygens = NULL, .hydrogens = NULL, .bond = NULL, 
	.done = NULL, .ending = NULL, .fw = my_params.fw};

    // namapovani sdilene pameti semaforu
    synch.operation = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, 0, 0);
    synch.hydrogens = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, 0, 0);
    synch.oxygens = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, 0, 0);
    synch.wrt = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, 0, 0);
    synch.ending = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, 0, 0);
    synch.bond = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, 0, 0);
    synch.done = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, 0, 0);
    synch.bonding = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, 0, 0);

    // overeni uspesnosti vytvoreni semaforu
    if (synch.wrt == SEM_FAILED || synch.operation == SEM_FAILED || synch.oxygens == SEM_FAILED\
	|| synch.hydrogens == SEM_FAILED || synch.bond == SEM_FAILED || synch.done == SEM_FAILED\
	||  synch.ending == SEM_FAILED || synch.bonding == SEM_FAILED)
        {
            DE_ALLOC;
            SEMDESTROY;

            fprintf(stderr, "Chyba pri vytvareni semaforu.\n");
	    exit (2);
    	}

    // inicializace semaforu
    if (sem_init(synch.operation, 1, 1) == -1 || sem_init(synch.hydrogens, 1, 0) == -1
    	|| sem_init(synch.oxygens, 1, 0) == -1 || sem_init(synch.wrt, 1, 1) == -1
    	|| sem_init(synch.ending, 1, 0) == -1 || sem_init(synch.bond, 1, 1) == -1
    	|| sem_init(synch.done, 1, 0) == -1 || sem_init(synch.bonding, 1, 1) == -1)
        {
            DE_ALLOC;
            SEMDESTROY;

            fprintf(stderr, "Chyba pri vytvareni semaforu.\n");
	    exit (2);
    	}

    pid_t pid;
    pid = fork();	// pomocny potomek na generovani dalsich potomku

    if (pid == 0)
    {
	pid_t o_pid;	// pid generovanych procesu kysliku

	for (long int i = 1; i <= my_params.o_count; i++)
	{
	    if (my_params.o_max != 0)
	        usleep(rand() % (1000 * (my_params.o_max) + 1));	// uspani procesu

	    o_pid = fork();
	    if (o_pid == 0)
	    {
		oxygen(synch, &shared_memory, my_params.bond_max, i);
		break;
	    }
 
	    else if (o_pid < 0)
    		{
		    DE_ALLOC;
		    SEMDESTROY;
    		    exit (2);
    		}
	    oxygen_pid[i-1] = pid;   //uchovame pid vsech kysliku
        } 
    }

    else if (pid > 0)
    {
        pid = fork();		// druhy pomocny potomek na generovani dalsich potomku
	if (pid == 0)
	{
	    pid_t h_pid;	// pid generovanych procesu vodiku

    	    for (long int i = 1; i <= my_params.o_count * 2; i++)
	    { 
		h_pid = fork();
		if (h_pid == 0)
		{		
		    if (my_params.h_max != 0)
		    	usleep(rand() % (1000 * (my_params.h_max) + 1));	// uspani procesu
		    hydrogen(synch, &shared_memory, my_params.bond_max, i);
		    break;
		}

		else if (pid < 0)
		    {
			fprintf(stderr, "Chyba pri vytvareni potomka vodik.\n");
		    	DE_ALLOC;
		    	SEMDESTROY;
			exit (2);
		    }
		hydrogen_pid[i-1] = pid;   //uchovame pid vsech vodiku
    	    }
	}

	else if (pid < 0)
	{
	    fprintf(stderr, "Chyba pri vytvareni potomka vodik.\n");
	    DE_ALLOC;
  	    SEMDESTROY;
	    exit (2);
	}
    }

    else
    {
	fprintf(stderr, "Chyba pri vytvareni potomka vodik.\n");
	DE_ALLOC;
	SEMDESTROY;
	exit (2);
    }


    // cekani na ukonceni vsech procesu
    for (long int i = 0; i < 2 * my_params.o_count; i++)
        waitpid(hydrogen_pid[i], NULL, 0);

    for (long int i = 0; i < my_params.o_count; i++)
        waitpid(oxygen_pid[i], NULL, 0);

    DE_ALLOC;
    SEMDESTROY;

    return 0;
}


// funkce na zpracovani zadanych parametru
int checkparams(int argc, char *argv[], Params *my_params)
{
    if (argc != 5)	// kontrola poctu parametru
	return 1;

    char *endPtr;
    my_params->o_count = strtol(argv[1], &endPtr, 10);	// retezec prvniho argumentu -> cislo
    if (*endPtr != '\0' || my_params->o_count < 1)	// kontrola
	return 1;
    molecules = my_params->o_count;

    my_params->h_max = strtol(argv[2], &endPtr, 10);	// retezec druheho argumentu -> cislo
    if (*endPtr != '\0' || my_params->h_max < 0 || my_params->h_max >= max)	// kontrola
	return 1;

    my_params->o_max = strtol(argv[3], &endPtr, 10);	// retezec tretiho argumentu -> cislo
    if (*endPtr != '\0' || my_params->o_max < 0 || my_params->o_max >= max)	// kontrola
	return 1;

    my_params->bond_max = strtol(argv[4], &endPtr, 10);	// retezec ctvrteho argumentu -> cislo
    if (*endPtr != '\0' || my_params->bond_max < 0 || my_params->bond_max >= max)  // kontrola
	return 1;

    return 0;
}


// funkce tvorici atom kysliku
void oxygen(Synch_molecule synch, Shared_Mem *shared_memory, long int max_sleep, int oxygen_id)
{
    sem_wait(synch.wrt);	// uzamceni souboru a sdilene pameti
	fprintf(synch.fw, "%ld\t: O: %d\t: started\n", (*shared_memory->print_counter)++, oxygen_id);
    fflush(synch.fw);
    sem_post(synch.wrt);	// odemceni souboru a sdilene pameti

    sem_wait(synch.operation);
    (*shared_memory->o_counter)++;	// inkrementace citace atomu kysliku

    if ((*shared_memory->h_counter) >= 2)	// pokud uz mame dva atomy vodiku
    {
	sem_wait(synch.bonding);	// dokonceni tvorby jinych atomu

	sem_wait(synch.wrt);	// uzamceni souboru a sdilene pameti
	fprintf(synch.fw, "%ld\t: O: %d\t: ready\n", (*shared_memory->print_counter)++, oxygen_id);
	fflush(synch.fw);
	sem_post(synch.wrt);	// odemceni souboru a sdilene pameti

	*shared_memory->h_counter -= 2;
	sem_post(synch.hydrogens);
	sem_post(synch.hydrogens);
	*shared_memory->o_counter -= 1;
	sem_post(synch.oxygens);
    }

    else
    {
	sem_wait(synch.bonding);
	sem_post(synch.bonding);

	sem_wait(synch.wrt);	// uzamceni souboru a sdilene pameti
	fprintf(synch.fw, "%ld\t: O: %d\t: waiting\n", (*shared_memory->print_counter)++, oxygen_id);
	fflush(synch.fw);
	sem_post(synch.wrt);	// odemceni souboru a sdilene pameti

	sem_post(synch.operation);
    }

    sem_wait(synch.oxygens);

    bond(synch, shared_memory, max_sleep, oxygen_id, "O");	

    sem_wait(synch.wrt);	// uzamceni souboru a sdilene pameti
	fprintf(synch.fw, "%ld\t: O: %d\t: bonded\n", (*shared_memory->print_counter)++,  oxygen_id);
	fflush(synch.fw);
    sem_post(synch.wrt);	// odemceni souboru a sdilene pameti

    sem_wait(synch.wrt);	// vsechny tri atomy se vraci z funkce bond
    (*shared_memory->to_bond)++;
    if ((*shared_memory->to_bond) == 3)
    {
	(*shared_memory->to_bond) = 0;
	sem_post(synch.bonding);
	sem_post(synch.bond);
	sem_post(synch.bond);
	sem_post(synch.bond);
    }
    sem_post(synch.wrt);
    sem_wait(synch.bond);	// cekani na ostatni atomy ve funkci bond

    sem_post(synch.operation);

    sem_wait(synch.wrt);	// pristup ke sdilene pameti
	(*shared_memory->finished)++;	// citac spojenych atomu
	if ((*shared_memory->finished) == (3 * molecules))	// vsechny atomy spojeny
	{
		(*shared_memory->finished) = 0;
		for(int i = 0; i < 3 * molecules; i++)
			sem_post(synch.ending);
	}
    sem_post(synch.wrt);
	sem_wait(synch.ending);	// cekani na ukonceni vsech atomu

    sem_wait(synch.wrt);	// uzamceni souboru a sdilene pameti
	fprintf(synch.fw, "%ld\t: O: %d\t: finished\n", (*shared_memory->print_counter)++,  oxygen_id);
	fflush(synch.fw);
    sem_post(synch.wrt);	// odemceni souboru a sdilene pameti
}


// funkce tvorici atom vodiku, obdobne jako u kysliku
void hydrogen(Synch_molecule synch, Shared_Mem *shared_memory, long int max_sleep, int hydrogen_id)
{
	sem_wait(synch.wrt);	// uzamceni souboru a sdilene pameti
            fprintf(synch.fw, "%ld\t: H: %d\t: started\n", (*shared_memory->print_counter)++, hydrogen_id);
	    fflush(synch.fw);
        sem_post(synch.wrt);	// odemceni souboru a sdilene pameti

	sem_wait(synch.operation);
    	(*shared_memory->h_counter)++;

     	if (((*shared_memory->h_counter) >= 2) && ((*shared_memory->o_counter) >= 1))
    	{
	    sem_wait(synch.bonding);

	    sem_wait(synch.wrt);	// uzamceni souboru a sdilene pameti
            	fprintf(synch.fw, "%ld\t: H: %d\t: ready\n", (*shared_memory->print_counter)++, hydrogen_id);
	    	fflush(synch.fw);
            sem_post(synch.wrt);	// odemceni souboru a sdilene pameti

	    (*shared_memory->h_counter) -= 2;
	    sem_post(synch.hydrogens);
	    sem_post(synch.hydrogens);
	    (*shared_memory->o_counter) -= 1;
	    sem_post(synch.oxygens);
	}	

	else
	{
	    sem_wait(synch.bonding);
	    sem_post(synch.bonding);

	    sem_wait(synch.wrt);	// uzamceni souboru a sdilene pameti
            	fprintf(synch.fw, "%ld\t: H: %d\t: waiting\n", (*shared_memory->print_counter)++, hydrogen_id);
	    	fflush(synch.fw);
	    sem_post(synch.wrt);	// odemceni souboru a sdilene pameti

	    sem_post(synch.operation);
	}

	sem_wait(synch.hydrogens);

	bond(synch, shared_memory, max_sleep, hydrogen_id, "H");

    	sem_wait(synch.wrt);	// uzamceni souboru a sdilene pameti
	    fprintf(synch.fw, "%ld\t: H: %d\t: bonded\n", (*shared_memory->print_counter)++,  hydrogen_id);
	    fflush(synch.fw);
    	sem_post(synch.wrt);	// odemceni souboru a sdilene pameti

	sem_wait(synch.wrt);	// cekani na navrat ostatnich atomu z funce bond	
	(*shared_memory->to_bond)++;
    	if ((*shared_memory->to_bond) == 3)
    	{
	    (*shared_memory->to_bond) = 0;
	    sem_post(synch.bonding);
	    sem_post(synch.bond);
	    sem_post(synch.bond);
	    sem_post(synch.bond);
    	}

    	sem_post(synch.wrt);
    	sem_wait(synch.bond);

    	sem_wait(synch.wrt);
	(*shared_memory->finished)++;	// cekani na spojeni vsech atomu
	if ((*shared_memory->finished) == (3 * molecules))
	{
	    (*shared_memory->finished) = 0;
	    for(int i = 0; i < 3 * molecules; i++)
		sem_post(synch.ending);
	}
    	sem_post(synch.wrt);

	sem_wait(synch.ending);

	sem_wait(synch.wrt);	// uzamceni souboru a sdilene pameti
	fprintf(synch.fw, "%ld\t: H: %d\t: finished\n", (*shared_memory->print_counter)++,  hydrogen_id);
	fflush(synch.fw);
	sem_post(synch.wrt);	// odemceni souboru a sdilene pameti
}


// funkce na spojeni tri atomu v molekulu
void bond(Synch_molecule synch, Shared_Mem *shared_memory, long int max_sleep, int my_id, char *me)
{
    	sem_wait(synch.wrt);		// uzamceni souboru a sdilene pameti
            fprintf(synch.fw, "%ld\t: %c: %d\t: begin bonding\n", (*shared_memory->print_counter)++, *me, my_id);
	    fflush(synch.fw);
        sem_post(synch.wrt);		// odemceni souboru a sdilene pameti

	if (max_sleep != 0)
            usleep(rand() % (1000 * (max_sleep) + 1));	// uspani procesu

	sem_wait(synch.wrt);		// cekani na vsechny tri atomy
	    (*shared_memory->h_ready)++;
	    if ((*shared_memory->h_ready) == 3)
	    {
		(*shared_memory->h_ready) = 0;
		sem_post(synch.done);
		sem_post(synch.done);
		sem_post(synch.done);
	    }
    	sem_post(synch.wrt);

	sem_wait(synch.done);
}

