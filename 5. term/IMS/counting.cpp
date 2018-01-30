/*
# Autori: Ondrej Vales xvales03, Nikola Valesova xvales02
# Predmet: IMS 2016/2017
# Projekt: Simulace prubehu voleb
*/

#include <iostream>

#include "defines.h"
#include "counting.h"

extern Store Register[];
extern Histogram FinishTime;
extern Histogram ShippingTime;
extern Histogram FinishCountingTime;
extern Histogram TotalCounts;

extern unsigned int ClosedVotingRooms;

Counting::Counting(int dist): District(dist), Counts(0)
{		
}

void Counting::Behavior()
{
	double VotesPerRoom = Register[District].tstat.Number() / Register[District].Capacity();
	do
	{
		// time needed to count votes
		Wait(Uniform(VotesPerRoom * 0.25, VotesPerRoom * 0.75));
		Counts++;
	} while (Random() < 0.02); 					// error was made, recount
	FinishCountingTime(Time - VOTING_TIME);		// log time at end of count

	double aux = Time;	
	Wait(20 + Exponential(70));		// ship results

	ShippingTime(Time - aux);		// log time needen to ship resuts
	FinishTime(Time - VOTING_TIME);	// log total time
	TotalCounts(Counts);			// log number of errors made in counting
	
	ClosedVotingRooms++;			// district finished

	if (ClosedVotingRooms == DISTRICT_COUNT_USED)		// last district to end counting, stop sim
	{
		Stop();
	}
}
