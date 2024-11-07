#include "ChemicalReaction.h"

ChemicalReaction::ChemicalReaction(int C, int N, int S, int Th, int O, int generationRate) 
{
    this->generationRate = generationRate;
    totalAtomNumber = C + N + S + Th + O;
    targetAtomNumbers[0] = C;
    targetAtomNumbers[1] = N;
    targetAtomNumbers[2] = S;
    targetAtomNumbers[3] = Th;
    targetAtomNumbers[4] = O;
}

void ChemicalReaction::Start() 
{
    std::thread co2_comp(&ChemicalReaction::Compose_CO2, this);
    std::thread no2_comp(&ChemicalReaction::Compose_NO2, this);
    std::thread so2_comp(&ChemicalReaction::Compose_SO2, this);
    std::thread tho2_comp(&ChemicalReaction::Compose_THO2, this);

    std::unique_lock<std::recursive_mutex> lck(mtx, std::defer_lock);

    for (int i = 0; i < totalAtomNumber; i++) 
    {
        int randAtomType = std::rand() % TOTAL_NUMBER_OF_ATOM_TYPE;

        while (targetAtomNumbers[randAtomType] == 0) 
        {
            randAtomType = std::rand() % TOTAL_NUMBER_OF_ATOM_TYPE;
        }

        atom ax;
        lck.lock();
        ax.atomID = atomID++;
        ax.atomType = atomName[randAtomType];
        currentAtomNumbers[randAtomType] += 1;
        lck.unlock();

        targetAtomNumbers[randAtomType] -= 1;
        std::cout << ax.atomType << " with ID: " << ax.atomID << " is created." << std::endl;
        cv.notify_all();

        Sleep();
    }

    co2_comp.join();
    no2_comp.join();
    so2_comp.join();
    tho2_comp.join();
}

void ChemicalReaction::Sleep() 
{
    double sTime = (-1.0 / 100.0) * log(1.0 - ((double)std::rand() / RAND_MAX));
    std::this_thread::sleep_for(std::chrono::microseconds(static_cast<int>(sTime * 1000000)));
}

void ChemicalReaction::Compose_CO2() 
{
    while (true) 
    {
        std::unique_lock<std::recursive_mutex> lck(mtx);
        while ((orderOfGeneration != 1 && orderOfGeneration != 3) || currentAtomNumbers[0] < 1 || currentAtomNumbers[4] < 2)
        {
            cv.wait(lck);
        }

        currentAtomNumbers[0] -= 1;
        currentAtomNumbers[4] -= 2;
        composedMolecule = "CO2";
        DisplayComposedMolecule();

        // Move to the next molecule in the sequence
        orderOfGeneration = (orderOfGeneration == 1) ? 2 : 4;
    }
}

void ChemicalReaction::Compose_NO2() 
{
    while (true) 
    {
        std::unique_lock<std::recursive_mutex> lck(mtx);
        while (orderOfGeneration != 2 || currentAtomNumbers[1] < 1 || currentAtomNumbers[4] < 2) 
        {
            cv.wait(lck);
        }

        currentAtomNumbers[1] -= 1;
        currentAtomNumbers[4] -= 2;
        composedMolecule = "NO2";
        DisplayComposedMolecule();

        // Move to the next molecule in the sequence
        orderOfGeneration = 3;
    }
}

void ChemicalReaction::Compose_SO2() 
{
    while (true) 
    {
        std::unique_lock<std::recursive_mutex> lck(mtx);
        while (orderOfGeneration != 4 || currentAtomNumbers[2] < 1 || currentAtomNumbers[4] < 2) 
        {
            cv.wait(lck);
        }

        currentAtomNumbers[2] -= 1;
        currentAtomNumbers[4] -= 2;
        composedMolecule = "SO2";
        DisplayComposedMolecule();

        // Move to the next molecule in the sequence
        orderOfGeneration = 5;
    }
}

void ChemicalReaction::Compose_THO2() 
{
    while (true) 
    {
        std::unique_lock<std::recursive_mutex> lck(mtx);
        while (orderOfGeneration != 5 || currentAtomNumbers[3] < 1 || currentAtomNumbers[4] < 2) 
        {
            cv.wait(lck);
        }

        currentAtomNumbers[3] -= 1;
        currentAtomNumbers[4] -= 2;
        composedMolecule = "ThO2";
        DisplayComposedMolecule();

        // Reset to the start of the cycle
        orderOfGeneration = 1;
    }
}

void ChemicalReaction::DisplayComposedMolecule() 
{
    std::unique_lock<std::recursive_mutex> lck(mtx);
    std::cout << "\nComposed Molecule: " << composedMolecule << "\n" << std::endl;
}