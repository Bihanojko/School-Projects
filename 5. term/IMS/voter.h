/*
# Autori: Ondrej Vales xvales03, Nikola Valesova xvales02
# Predmet: IMS 2016/2017
# Projekt: Simulace prubehu voleb
*/

#ifndef VOTER_CLASS_FILE
#define VOTER_CLASS_FILE

#include <vector>

#include "simlib.h"

class Voter : public Process
{
	int District;		// specifies which district voter belongs to

public:
	Voter(int dist);	// Voter constructor with district affiliation 
	void Behavior();	// Voter simulation beahavior
};

class VoterGenerator : public Process
{
	int District;			// specifies which district voters belong to
	double Arrivals;		// mean interval between arrivals (average)
	double ArrivalsHigh;	// mean interval between arrivals in bussy hours
	double ArrivalsLow;		// mean interval between arrivals elsewhere

public:
	VoterGenerator(int dist, double arv);		// VoterGenerator constructor with district affiliation 
	void Behavior();							// VoterGenerator simulation beahavior
	double Delay();								// VoterGenerator interval between arrivals
};

#endif
