#ifndef INCLUDE_ATOM_SS_H_
#define INCLUDE_ATOM_SS_H_

#include <iostream>
#include <cstdlib>
#include <thread>
#include <cmath>
#include <semaphore>
#include <queue>
#include <map>

#define NUM_OF_ATOM_TO_GENERATE 20

#include "CommonTypes.h"

class ChemicalReaction_Semaphore
{
public:
	ChemicalReaction_Semaphore(int generationRate = 100);
	~ChemicalReaction_Semaphore();
	void Start();

private:
	inline static int atomID = 0;
	int generationRate; // Generation interval for each atom
	std::string composedMolecule;

	std::binary_semaphore criticalSection {1};
	std::binary_semaphore criticalSectionDisplay{ 1 };

	std::binary_semaphore co2_generator {1};
	std::binary_semaphore no2_generator {0};
	std::binary_semaphore so2_generator {0};
	std::binary_semaphore tho2_generator {0};

	std::counting_semaphore<NUM_OF_ATOM_TO_GENERATE> c {0};
	std::counting_semaphore<NUM_OF_ATOM_TO_GENERATE> n {0};
	std::counting_semaphore<NUM_OF_ATOM_TO_GENERATE> s {0};
	std::counting_semaphore<NUM_OF_ATOM_TO_GENERATE> th {0};
	std::counting_semaphore<NUM_OF_ATOM_TO_GENERATE * 2> o {0};

	std::queue<atom> c_atoms;
	std::queue<atom> n_atoms;
	std::queue<atom> s_atoms;
	std::queue<atom> th_atoms;
	std::queue<atom> o_atoms;

	bool isNO2produced = false;

	//void Sleep();
	void Sleep(int defaultTime = -1);
	void Compose_CO2();
	void Compose_NO2();
	void Compose_SO2();
	void Compose_THO2();
	void Produce_C();
	void Produce_N();
	void Produce_S();
	void Produce_TH();
	void Produce_O();
	void DisplayComposedMolecule(TYPE producedType, atom* ax = nullptr);

};

#endif