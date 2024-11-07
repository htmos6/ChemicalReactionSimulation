#ifndef INCLUDE_ATOM_H_
#define INCLUDE_ATOM_H_

#include <iostream>
#include <cstdlib>
#include <thread>
#include <cmath>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <map>

#define TOTAL_NUMBER_OF_ATOM_TYPE 5

enum class TYPE
{
	ATOM, 
	MOLECULE
};

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
	~ChemicalReaction();
	void Start();

private:
	inline static int atomID = 0;
	int totalAtomNumber = 0;
	int orderOfGeneration = 1; // 1 2 3 4 5 .... (*)
	int generationRate; // Generation interval for each atom
	int targetAtomNumbers[5]; // Update at each production (*)

	std::condition_variable_any cv;
	std::recursive_mutex mtx;
	std::string composedMolecule;
	atom ax;

	std::queue<atom> c_atoms;
	std::queue<atom> n_atoms;
	std::queue<atom> s_atoms;
	std::queue<atom> th_atoms;
	std::queue<atom> o_atoms;

	//void Sleep();
	void Sleep(int defaultTime = -1);
	void Compose_CO2();
	void Compose_NO2();
	void Compose_SO2();
	void Compose_THO2();
	void DisplayComposedMolecule(TYPE producedType);

};

#endif