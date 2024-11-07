#ifndef INCLUDE_ATOM_H_
#define INCLUDE_ATOM_H_

#include <iostream>
#include <cstdlib>
#include <thread>
#include <cmath>
#include <condition_variable>
#include <mutex>
#include <map>

#define TOTAL_NUMBER_OF_ATOM_TYPE 5

struct atom
{
	int atomID;
	std::string atomType; // C, N, S, or 
};
	
inline std::string atomName[5] = { "C", "N", "S", "Th", "O" };

class ChemicalReaction
{
public:
	ChemicalReaction(int C, int N, int S, int Th, int O, int generationRate);
	~ChemicalReaction() = default;
	void Start();

private:
	inline static int atomID = 0;
	int totalAtomNumber = 0;
	int orderOfGeneration = 1; // 1 2 3 4 5 .... (*)
	int generationRate; // Generation interval for each atom
	int targetAtomNumbers[5]; // Update at each production (*)
	int currentAtomNumbers[5] = { 0, 0, 0, 0, 0 }; // Update at each production (*)

	std::condition_variable_any cv;
	std::recursive_mutex mtx;
	std::string composedMolecule;

	void Sleep();
	void Compose_CO2();
	void Compose_NO2();
	void Compose_SO2();
	void Compose_THO2();
	void DisplayComposedMolecule();

};

#endif