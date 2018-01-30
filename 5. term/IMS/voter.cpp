/*
# Autori: Ondrej Vales xvales03, Nikola Valesova xvales02
# Predmet: IMS 2016/2017
# Projekt: Simulace prubehu voleb
*/

#include <iostream>

#include "defines.h"
#include "voter.h"
#include "counting.h"

#define PEAK_1_START 120
#define PEAK_1_END 270
#define PEAK_2_START 600
#define PEAK_2_END 720

extern Store Register[];
extern Store Booths[];
extern Store Urn[];

extern Stat VotersCount[];
extern Histogram ArrivalsHist;

extern bool Closed[DISTRICT_COUNT];
extern unsigned int CurrentVotersCount[DISTRICT_COUNT];

const double high = 270;		// number of bussy hours fri 16:00 - 18:30, sat 10:00 - 12:00
const double low = 570;			// number of not bussy hours
const double markup = 3;		// in bussy hours voters arrive approx. 3 times more often

Voter::Voter(int dist): District(dist)
{
}

void Voter::Behavior()
{
	Enter(Register[District], 1);	// voter registers
	Wait(Uniform(0.15, 0.33));		// register delay
	Leave(Register[District], 1);	// registered

	Enter(Booths[District], 1);		// voter enters voting booth
	Wait(Exponential(2));			// in booth delay
	Leave(Booths[District], 1);		// leave booth

	Enter(Urn[District], 1);
	Wait(0.1);					// submit vote
	Leave(Urn[District], 1);	// leave

	CurrentVotersCount[District]--;		// update count of voters in voting room

	if (Closed[District] && CurrentVotersCount[District] == 0)		// last voter leaves start counting
	{
		(new Counting(District))->Activate();
	}
}

VoterGenerator::VoterGenerator(int dist, double arv): District(dist), Arrivals(arv)
{
	double tmp = (high * markup + low) * Arrivals / VOTING_TIME;
	ArrivalsHigh = tmp / markup;
	ArrivalsLow	= tmp;
}

void VoterGenerator::Behavior()
{
	if (Time < VOTING_TIME)
	{
		(new Voter(District))->Activate();			// new voter
		ArrivalsHist(Time);
		CurrentVotersCount[District]++;
		VotersCount[District](Register[District].QueueLen () + Booths[District].QueueLen () + Urn[District].QueueLen ());		//log length of queue
		(new VoterGenerator(District, Arrivals))->Activate(Time + Delay());		// schedule next voter
	}
	else		// voting time is out, if room is empty start counting
	{
		Closed[District] = true;
		if (CurrentVotersCount[District] == 0)
		{
			(new Counting(District))->Activate();
		}
	}
}

double VoterGenerator::Delay()		// calculate delay baset on time of day
{
	if ((Time >= PEAK_1_START && Time <= PEAK_1_END) || (Time >= PEAK_2_START && Time <= PEAK_2_END))
		return Exponential(ArrivalsHigh);

	return Exponential(ArrivalsLow);
}
