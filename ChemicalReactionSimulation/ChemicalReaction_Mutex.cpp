#include "ChemicalReaction_Mutex.h"

ChemicalReaction_Mutex::ChemicalReaction_Mutex(int C, int N, int S, int Th, int O, int generationRate) 
{
    this->generationRate = generationRate;
    totalAtomNumber = C + N + S + Th + O;
    targetAtomNumbers[0] = C;
    targetAtomNumbers[1] = N;
    targetAtomNumbers[2] = S;
    targetAtomNumbers[3] = Th;
    targetAtomNumbers[4] = O;
}

ChemicalReaction_Mutex::~ChemicalReaction_Mutex()
{
    std::unique_lock<std::recursive_mutex> lck(mtx);

    std::cout << std::endl;
    while (c_atoms.empty() == false)
    {
        std::cout << c_atoms.front().atomType << " with ID: " << c_atoms.front().atomID << " is wasted." << std::endl;
        c_atoms.pop();
    }

    while (n_atoms.empty() == false)
    {
        std::cout << n_atoms.front().atomType << " with ID: " << n_atoms.front().atomID << " is wasted." << std::endl;
        n_atoms.pop();
    }

    while (s_atoms.empty() == false)
    {
        std::cout << s_atoms.front().atomType << " with ID: " << s_atoms.front().atomID << " is wasted." << std::endl;
        s_atoms.pop();
    }

    while (th_atoms.empty() == false)
    {
        std::cout << th_atoms.front().atomType << " with ID: " << th_atoms.front().atomID << " is wasted." << std::endl;
        th_atoms.pop();
    }

    while (o_atoms.empty() == false)
    {
        std::cout << o_atoms.front().atomType << " with ID: " << o_atoms.front().atomID << " is wasted." << std::endl;
        o_atoms.pop();
    }
}

void ChemicalReaction_Mutex::Start() 
{
    std::thread co2_comp(&ChemicalReaction_Mutex::Compose_CO2, this);
    std::thread no2_comp(&ChemicalReaction_Mutex::Compose_NO2, this);
    std::thread so2_comp(&ChemicalReaction_Mutex::Compose_SO2, this);
    std::thread tho2_comp(&ChemicalReaction_Mutex::Compose_THO2, this);

    std::unique_lock<std::recursive_mutex> lck(mtx, std::defer_lock);
    for (int i = 0; i < totalAtomNumber; i++) 
    {
        int randAtomType = std::rand() % TOTAL_NUMBER_OF_ATOM_TYPE;

        while (targetAtomNumbers[randAtomType] == 0) 
        {
            randAtomType = std::rand() % TOTAL_NUMBER_OF_ATOM_TYPE;
        }

        lck.lock();
        ax.atomID = atomID++;
        ax.atomType = atomName[randAtomType];

        switch (randAtomType)
        {
        case 0:
            c_atoms.push(ax);
            break;
        case 1:
            n_atoms.push(ax);
            break;
        case 2:
            s_atoms.push(ax);
            break;
        case 3:
            th_atoms.push(ax);
            break;
        case 4:
            o_atoms.push(ax);
            break;
        }

        lck.unlock();

        targetAtomNumbers[randAtomType] -= 1;
        std::cout << ax.atomType << " with ID: " << ax.atomID << " is created." << std::endl;
        cv.notify_all();

        Sleep();
    }

    co2_comp.detach();
    no2_comp.detach();
    so2_comp.detach();
    tho2_comp.detach();

    Sleep(3);
}

/*
void ChemicalReaction_Mutex::Sleep() 
{
    double sTime = (-1.0 / 100.0) * log(1.0 - ((double)std::rand() / RAND_MAX));
    std::this_thread::sleep_for(std::chrono::microseconds(static_cast<int>(sTime * 1000000)));
}
*/

void ChemicalReaction_Mutex::Sleep(int defaultTime)
{
    if (defaultTime < 0)
    {
        double sTime = (-1.0 / 100.0) * log(1.0 - ((double)std::rand() / RAND_MAX));
        std::this_thread::sleep_for(std::chrono::microseconds(static_cast<int>(sTime * 1000000)));
    }
    else
    {
        std::this_thread::sleep_for(std::chrono::seconds(defaultTime));
    }
}

void ChemicalReaction_Mutex::Compose_CO2() 
{
    while (true) 
    {
        std::unique_lock<std::recursive_mutex> lck(mtx);
        while ((orderOfGeneration != 1 && orderOfGeneration != 3) || c_atoms.size() < 1 || o_atoms.size() < 2)
        {
            cv.wait(lck);
        }

        c_atoms.pop();
        o_atoms.pop();
        o_atoms.pop();
        composedMolecule = "CO2";
        DisplayComposedMolecule(TYPE::MOLECULE);

        // Move to the next molecule in the sequence
        orderOfGeneration = (orderOfGeneration == 1) ? 2 : 4;
    }
}

void ChemicalReaction_Mutex::Compose_NO2() 
{
    while (true) 
    {
        std::unique_lock<std::recursive_mutex> lck(mtx);
        while (orderOfGeneration != 2 || n_atoms.size() < 1 || o_atoms.size() < 2)
        {
            cv.wait(lck);
        }

        n_atoms.pop();
        o_atoms.pop();
        o_atoms.pop();
        composedMolecule = "NO2";
        DisplayComposedMolecule(TYPE::MOLECULE);

        // Move to the next molecule in the sequence
        orderOfGeneration = 3;
    }
}

void ChemicalReaction_Mutex::Compose_SO2() 
{
    while (true) 
    {
        std::unique_lock<std::recursive_mutex> lck(mtx);
        while (orderOfGeneration != 4 || s_atoms.size() < 1 || o_atoms.size() < 2)
        {
            cv.wait(lck);
        }

        s_atoms.pop();
        o_atoms.pop();
        o_atoms.pop();
        composedMolecule = "SO2";
        DisplayComposedMolecule(TYPE::MOLECULE);

        // Move to the next molecule in the sequence
        orderOfGeneration = 5;
    }
}

void ChemicalReaction_Mutex::Compose_THO2() 
{
    while (true) 
    {
        std::unique_lock<std::recursive_mutex> lck(mtx);
        while (orderOfGeneration != 5 || th_atoms.size() < 1 || o_atoms.size() < 2)
        {
            cv.wait(lck);
        }

        th_atoms.pop();
        o_atoms.pop();
        o_atoms.pop();
        composedMolecule = "ThO2";
        DisplayComposedMolecule(TYPE::MOLECULE);

        // Reset to the start of the cycle
        orderOfGeneration = 1;
    }
}

void ChemicalReaction_Mutex::DisplayComposedMolecule(TYPE producedType)
{
    std::unique_lock<std::recursive_mutex> lck(mtx);

    if (producedType == TYPE::ATOM)
    {
        std::cout << ax.atomType << " with ID: " << ax.atomID << " is created." << std::endl;
    }
    else if(producedType == TYPE::MOLECULE)
    {
        std::cout << "\nComposed Molecule: " << composedMolecule << "\n" << std::endl;
    }
}