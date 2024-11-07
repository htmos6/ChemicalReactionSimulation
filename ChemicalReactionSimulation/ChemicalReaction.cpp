#include "ChemicalReaction.h"

ChemicalReaction::ChemicalReaction(int C, int N, int S, int Th, int O, int genRate) 
{
    this->genRate = genRate;
    totalAtomNum = C + N + S + Th + O;
    targetAtomNum[0] = C;
    targetAtomNum[1] = N;
    targetAtomNum[2] = S;
    targetAtomNum[3] = Th;
    targetAtomNum[4] = O;
}

void ChemicalReaction::Start() 
{
    std::thread co2_comp(&ChemicalReaction::Compose_CO2, this);
    std::thread no2_comp(&ChemicalReaction::Compose_NO2, this);
    std::thread so2_comp(&ChemicalReaction::Compose_SO2, this);
    std::thread tho2_comp(&ChemicalReaction::Compose_THO2, this);

    std::unique_lock<std::recursive_mutex> lck(mtx, std::defer_lock);

    for (int i = 0; i < totalAtomNum; i++) 
    {
        int randAtomType = std::rand() % TOTAL_NUMBER_OF_ATOM_TYPE;

        while (targetAtomNum[randAtomType] == 0) 
        {
            randAtomType = std::rand() % TOTAL_NUMBER_OF_ATOM_TYPE;
        }

        targetAtomNum[randAtomType] -= 1;

        lck.lock();
        initAtomNum[randAtomType] += 1;
        std::cout << atomName[randAtomType] << " with ID: " << atomID++ << " is created." << std::endl;
        lck.unlock();
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
        while ((controller != 1 && controller != 3) || initAtomNum[0] < 1 || initAtomNum[4] < 2)
        {
            cv.wait(lck);
        }

        initAtomNum[0] -= 1;
        initAtomNum[4] -= 2;
        composedMolecule = "CO2";
        DisplayComposedMolecule();

        // Move to the next molecule in the sequence
        controller = (controller == 1) ? 2 : 4;
    }
}

void ChemicalReaction::Compose_NO2() 
{
    while (true) 
    {
        std::unique_lock<std::recursive_mutex> lck(mtx);
        while (controller != 2 || initAtomNum[1] < 1 || initAtomNum[4] < 2) 
        {
            cv.wait(lck);
        }

        initAtomNum[1] -= 1;
        initAtomNum[4] -= 2;
        composedMolecule = "NO2";
        DisplayComposedMolecule();

        // Move to the next molecule in the sequence
        controller = 3;
    }
}

void ChemicalReaction::Compose_SO2() 
{
    while (true) 
    {
        std::unique_lock<std::recursive_mutex> lck(mtx);
        while (controller != 4 || initAtomNum[2] < 1 || initAtomNum[4] < 2) 
        {
            cv.wait(lck);
        }

        initAtomNum[2] -= 1;
        initAtomNum[4] -= 2;
        composedMolecule = "SO2";
        DisplayComposedMolecule();

        // Move to the next molecule in the sequence
        controller = 5;
    }
}

void ChemicalReaction::Compose_THO2() 
{
    while (true) 
    {
        std::unique_lock<std::recursive_mutex> lck(mtx);
        while (controller != 5 || initAtomNum[3] < 1 || initAtomNum[4] < 2) 
        {
            cv.wait(lck);
        }

        initAtomNum[3] -= 1;
        initAtomNum[4] -= 2;
        composedMolecule = "ThO2";
        DisplayComposedMolecule();

        // Reset to the start of the cycle
        controller = 1;
    }
}

void ChemicalReaction::DisplayComposedMolecule() 
{
    std::unique_lock<std::recursive_mutex> lck(mtx);
    std::cout << "\nComposed Molecule: " << composedMolecule << "\n" << std::endl;
}