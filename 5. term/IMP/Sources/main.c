// Predmet: Mikroprocesorove a vestavene systemy
// Projekt: Simulace v CW: Svetelne noviny
// Autor: Nikola Valesova (xvales02)
// Datum: 22. 10. 2016
// Original



#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */



// pole ulozenych hodnot na zobrazeni loginu ulozeneho po sloupcich
unsigned char login[64] = {0,33,18,12,12,18,33,0,0,48,12,3,12,48,0,0,0,30,33,33,34,30,1,0,0,0,0,255,0,0,0,0,0,28,42,41,41,41,26,0,0,18,41,37,37,18,0,0,0,60,66,129,129,66,60,0,0,67,133,137,145,97,1};
// pocatecni adresa mapovani diod displeje
unsigned char *display = (char *) 0x00B0;
// namapovani rychlosti dane posuvnikem
unsigned char *speed = (char *) 0x00D0;
// napamovani tlacitka init pro inicializaci
unsigned char *init = (char *) 0x00D1;
// napamovani tlacitka horiz pro horizontalni rotaci
unsigned char *horiz = (char *) 0x00D2;
// napamovani tlacitka vert pro vertikalni rotaci
unsigned char *vert = (char *) 0x00D3;



// inicializace globalnich promennych, tlacitka nejsou stiskla, posuvnik rychlosti je uprostred
void set_global_variables() {
  *speed = 127;
  *init = 0;    
  *horiz = 0;
  *vert = 0;
}


// funkce na inicializaci promennych
void initialize_variables(unsigned int *coord_x, unsigned int *coord_y) {
    // vynulovani aktualniho poctu bitu rotace v obou smerech
    *coord_x = 0;
    *coord_y = 0;
}


// funkce na zobrazeni casti loginu po inicialiazaci
void reset_display() {
  // citac pro cyklus for
  int i;
  // pocatecni zobrazeni casti loginu, 32 bitu je pocet sloupcu diod na displeji
  for (i = 0;i < 32; i++) {
    display[i] = login[i]; 
  }
}


// funkce detekujici stisk tlacitka a ulozeni informace o novem stavu
// pokud nedoslo ke stisku zadneho z tlacitek, smer zustava beze zmeny
void get_direction(unsigned int *direction) {
  if (*init & 1) {
    *direction = 0;
  }
  else if (*horiz & 1) {
    *direction = 1;
  }
  else if (*vert & 1) {
    *direction = 2;
  } 
}


// funkce, ktera na zaklade smeru rotace zvysi pocet bitu, o ktere je treba login orotovat
void increment_rotation_bits(unsigned int direction, unsigned int *coord_x, unsigned int *coord_y) {
  // horizontalni rotace - inkrementace poctu bitu rotace na ose x
  if (direction == 1) {
    (*coord_x)++;
  }
  // vertikalni rotace - inkrementace poctu bitu rotace na ose y  
  else if (direction == 2) {
    (*coord_y)++;
  }         
}     


// funkce na rotaci loginu
void rotate_login(unsigned int coord_x, unsigned int coord_y) {
  int i;
  // pro kazdy sloupec diod na displeji proved oba typy rotaci
  for (i = 0; i < 32; i++) {
    // horizontalni rotace
    display[i] = login[(i - (coord_x % 64) + 64) % 64];
    // vertikalni rotace
    display[i] = (display[i] << (coord_y % 8)) | (display[i] >> (8 - (coord_y % 8))); 
  }
}


// funkce umoznujici urceni rychlosti rotace pomoci tahla
void delay_next_rotation() {
  // citac pro realizaci zpozdeni a urcovani rychlosti rotace loginu
  unsigned int counter;
  // delka cekani pred orotovanim loginu, neprimo umerna rychlosti ziskane z posuvniku
  unsigned int wait;
  
  // zpomaleni / zrychleni na zaklade hodnoty ziskane z tahla
  // koeficienty 256 a 10 ziskany empiricky ve snaze najit idealni pomer mezi pomalou a rychlou rotaci 
  counter = 0;
  wait = 256 - (unsigned int)*speed;
  while (counter < wait * 10) {
    counter++;
  }
}


// MAIN
void main(void) {
  // urceni smeru, kterym se login pohybuje - vertikalni, horizontalni, nebo zadny
  unsigned int direction = 0;
  // citac, o kolik bitu ma orotovat login ve vertikalnim smeru
  unsigned int coord_x = 0;
  // citac, o kolik bitu ma orotovat login v horizontalnim smeru
  unsigned int coord_y = 0;

  // inicializace globalnich promennych
  set_global_variables();

  // stav inicializace
  for (;;) {
    // inicializace promennych 
    initialize_variables(&coord_x, &coord_y);
    
    // zobrazeni inicializacni casti loginu na displeji
    reset_display();

    // cekani na stisk nektereho z tlacitek, po stisku horiz nebo vert rotace  
    for (;;) {
      __RESET_WATCHDOG(); /* feeds the dog */

      // ziskani smeru rotace podle stisknuteho tlacitka
      get_direction(&direction);
     
      // pokud bylo stisknuto tlacitko init, vraceni na zacatek predchoziho cyklu a provedeni inicializace
      if (direction == 0) {
        break;
      }
     
      // zvyseni poctu bitu, o ktere je treba login orotovat
      increment_rotation_bits(direction, &coord_x, &coord_y);

      // rotace loginu a jeho zobrazeni
      rotate_login(coord_x, coord_y);

      // zpozdeni programu pred opetovnou rotaci loginu
      delay_next_rotation();
    }
  }
}