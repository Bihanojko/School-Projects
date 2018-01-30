/*
# Autori: Ondrej Vales xvales03, Nikola Valesova xvales02
# Predmet: IMS 2016/2017
# Projekt: Simulace prubehu voleb
*/

#include <ctime>
#include <iostream>
#include <string>

#include "simlib.h"
#include "sim.h"
#include "voter.h"

Store Register[DISTRICT_COUNT];
Store Booths[DISTRICT_COUNT];
Store Urn[DISTRICT_COUNT];

Stat VotersCount[DISTRICT_COUNT];
Histogram FinishCountingTime(0.0, 30, 20);
Histogram ShippingTime(0.0, 30, 20);
Histogram FinishTime(0.0, 30, 20);
Histogram TotalCounts(0.0, 1, 10);
Histogram MaxQueue(0.0, 25.0, 20);
Histogram ArrivalsHist(0.0, 60, 15);

unsigned int Closed[DISTRICT_COUNT] = {false};
unsigned int CurrentVotersCount[DISTRICT_COUNT] = {0};
unsigned int ClosedVotingRooms = 0;

void Output(int Districts);
double Args(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	double Turnout = Args(argc, argv);
	int Districts = DISTRICT_COUNT_USED;	// set total number of districts considered in simulation

	SetCalendar("cq");
	RandomSeed(std::time(nullptr));		// new random seed based on timestamp
	Init(0);							

	for (int i = 0; i < Districts; i++)		//initialize facilities and stores in districts
	{
		Register[i].SetName(Name[i]);			// set voter registration name (for statistics)
		Register[i].SetCapacity(Capacity[i]);	// set voter registration capacity

		Booths[i].SetName(Name[i]);				// set voting booths name (for statistics)
		Booths[i].SetCapacity((int)(Capacity[i] * (Turnout * 40 + 0.21)));	// set voting booths capacity

		Urn[i].SetName(Name[i]);				// set voting urn name (for statistics)
		Urn[i].SetCapacity(Capacity[i]);		// set voting urn capacity

		(new VoterGenerator(i, (VOTING_TIME / (Population[i] * Turnout))))->Activate();		// district i voter generator
	}

	Run();					// simulation
	Output(Districts);		// output statistics

	return 0;

}

void Output(int Districts)				// output statistics
{
	SetOutput("MaxQueue.log");
	for (int i = 0; i < Districts; i++)
	{
		MaxQueue(VotersCount[i].Max());
	}
	MaxQueue.Output();

	SetOutput("FinishCountingTime.log");
	FinishCountingTime.Output();

	SetOutput("ShippingTime.log");
	ShippingTime.Output();

	SetOutput("FinishTime.log");
	FinishTime.Output();

	SetOutput("TotalCounts.log");
	TotalCounts.Output();

	SetOutput("ArrivalsHist.log");
	ArrivalsHist.Output();

	SetOutput("");

	return;
}

double Args(int argc, char *argv[])
{
	if (argc == 2)
	{
		size_t end = 0;
		double Turnout = std::stod(argv[1], &end);
		if (argv[1][end] == '\0' && Turnout > 0.0 && Turnout < 1.0)
			return Turnout;
	}

	std::cerr << "Chyba argumentu" << std::endl;
	exit(-1);
}
