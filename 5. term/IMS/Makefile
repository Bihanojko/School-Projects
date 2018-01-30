
# Autori: Ondrej Vales xvales03, Nikola Valesova xvales02
# Predmet: IMS 2016/2017
# Projekt: Simulace prubehu voleb

CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11
OFILES = sim.o voter.o counting.o
HFILES = sim.h voter.h defines.h counting.h

all: sim

sim: $(OFILES) $(HFILES)
	$(CXX) $(CXXFLAGS) $(OFILES) -g -O2 -lsimlib -lm -o sim
	
sim.o: sim.cpp $(HFILES)
	$(CXX) $(CXXFLAGS) -c sim.cpp

voter.o: voter.cpp counting.cpp voter.h counting.h defines.h
	$(CXX) $(CXXFLAGS) -c voter.cpp

counting.o: counting.cpp counting.h
	$(CXX) $(CXXFLAGS) -c counting.cpp

run:
	./sim 0.3

pack:
	zip ims.zip *.cpp *.h Makefile

clean:
	rm -f *.o *.log sim
