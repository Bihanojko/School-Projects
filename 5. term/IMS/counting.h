/*
# Autori: Ondrej Vales xvales03, Nikola Valesova xvales02
# Predmet: IMS 2016/2017
# Projekt: Simulace prubehu voleb
*/

#ifndef COUNTING_CLASS_FILE
#define COUNTING_CLASS_FILE

#include "simlib.h"

class Counting : public Process
{
	int District;	// specifies which district voters belong to
	int Counts;	// specifies which district voters belong to

public:
	Counting(int dist);		// VoterGenerator constructor with district affiliation 
	void Behavior();		// VoterGenerator simulation beahavior
};

#endif
