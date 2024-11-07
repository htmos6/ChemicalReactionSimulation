#include "ChemicalReaction_Semaphore.h"

ChemicalReaction_Semaphore::ChemicalReaction_Semaphore(int generationRate)
{
    this->generationRate = generationRate;
}

ChemicalReaction_Semaphore::~ChemicalReaction_Semaphore()
{
    criticalSection.acquire();

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

    criticalSection.release();
}

void ChemicalReaction_Semaphore::Start()
{
    std::thread co2_comp(&ChemicalReaction_Semaphore::Compose_CO2, this);
    std::thread no2_comp(&ChemicalReaction_Semaphore::Compose_NO2, this);
    std::thread so2_comp(&ChemicalReaction_Semaphore::Compose_SO2, this);
    std::thread tho2_comp(&ChemicalReaction_Semaphore::Compose_THO2, this);
    std::thread c_produce(&ChemicalReaction_Semaphore::Produce_C, this);
    std::thread n_produce(&ChemicalReaction_Semaphore::Produce_N, this);
    std::thread s_produce(&ChemicalReaction_Semaphore::Produce_S, this);
    std::thread th_produce(&ChemicalReaction_Semaphore::Produce_TH, this);
    std::thread o_produce(&ChemicalReaction_Semaphore::Produce_O, this);

    co2_comp.detach();
    no2_comp.detach();
    so2_comp.detach();
    tho2_comp.detach();
    c_produce.detach();
    n_produce.detach();
    s_produce.detach();
    th_produce.detach();
    o_produce.detach();

    Sleep(10);
}

/*
void ChemicalReaction_Semaphore::Sleep()
{
    double sTime = (-1.0 / 100.0) * log(1.0 - ((double)std::rand() / RAND_MAX));
    std::this_thread::sleep_for(std::chrono::microseconds(static_cast<int>(sTime * 1000000)));
}
*/

void ChemicalReaction_Semaphore::Sleep(int defaultTime)
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

void ChemicalReaction_Semaphore::Produce_C()
{
    for (int i = 0; i < NUM_OF_ATOM_TO_GENERATE; i++)
    {
        atom ax;
        
        criticalSection.acquire();
        ax.atomID = atomID++;
        criticalSection.release();

        ax.atomType = atomName[0];

        criticalSection.acquire();
        c_atoms.push(ax);
        DisplayComposedMolecule(TYPE::ATOM, &ax);
        criticalSection.release();

        c.release();
        Sleep();
    }
}

void ChemicalReaction_Semaphore::Produce_N()
{
    for (int i = 0; i < NUM_OF_ATOM_TO_GENERATE; i++)
    {
        atom ax;

        criticalSection.acquire();
        ax.atomID = atomID++;
        criticalSection.release();

        ax.atomType = atomName[1];

        criticalSection.acquire();
        n_atoms.push(ax);
        DisplayComposedMolecule(TYPE::ATOM, &ax);
        criticalSection.release();

        n.release();
        Sleep();
    }
}

void ChemicalReaction_Semaphore::Produce_S()
{
    for (int i = 0; i < NUM_OF_ATOM_TO_GENERATE; i++)
    {
        atom ax;

        criticalSection.acquire();
        ax.atomID = atomID++;
        criticalSection.release();

        ax.atomType = atomName[2];

        criticalSection.acquire();
        s_atoms.push(ax);
        DisplayComposedMolecule(TYPE::ATOM, &ax);
        criticalSection.release();

        s.release();
        Sleep();
    }
}

void ChemicalReaction_Semaphore::Produce_TH()
{
    for (int i = 0; i < NUM_OF_ATOM_TO_GENERATE; i++)
    {
        atom ax;

        criticalSection.acquire();
        ax.atomID = atomID++;
        criticalSection.release();

        ax.atomType = atomName[3];

        criticalSection.acquire();
        th_atoms.push(ax);
        DisplayComposedMolecule(TYPE::ATOM, &ax);
        criticalSection.release();

        th.release();
        Sleep();
    }
}

void ChemicalReaction_Semaphore::Produce_O()
{
    for (int i = 0; i < NUM_OF_ATOM_TO_GENERATE * 2; i++)
    {
        atom ax;

        criticalSection.acquire();
        ax.atomID = atomID++;
        criticalSection.release();

        ax.atomType = atomName[4];

        criticalSection.acquire();
        o_atoms.push(ax);
        DisplayComposedMolecule(TYPE::ATOM, &ax);
        criticalSection.release();

        o.release();
        Sleep();
    }
}

void ChemicalReaction_Semaphore::Compose_CO2()
{
    while (true)
    {
        co2_generator.acquire();

        c.acquire();
        o.acquire();
        o.acquire();

        criticalSection.acquire();
        c_atoms.pop();
        o_atoms.pop();
        o_atoms.pop();
        composedMolecule = "CO2";
        DisplayComposedMolecule(TYPE::MOLECULE);
        criticalSection.release();

        isNO2produced == false ? no2_generator.release() : so2_generator.release();
    }
}

void ChemicalReaction_Semaphore::Compose_NO2()
{
    while (true)
    {
        no2_generator.acquire();

        n.acquire();
        o.acquire();
        o.acquire();

        criticalSection.acquire();
        n_atoms.pop();
        o_atoms.pop();
        o_atoms.pop();
        composedMolecule = "NO2";
        DisplayComposedMolecule(TYPE::MOLECULE);
        criticalSection.release();

        isNO2produced = true;
        co2_generator.release();
    }
}

void ChemicalReaction_Semaphore::Compose_SO2()
{
    while (true)
    {
        so2_generator.acquire();

        s.acquire();
        o.acquire();
        o.acquire();

        criticalSection.acquire();
        s_atoms.pop();
        o_atoms.pop();
        o_atoms.pop();
        composedMolecule = "SO2";
        DisplayComposedMolecule(TYPE::MOLECULE);
        criticalSection.release();

        isNO2produced = false;
        tho2_generator.release();
    }
}

void ChemicalReaction_Semaphore::Compose_THO2()
{
    while (true)
    {
        tho2_generator.acquire();

        th.acquire();
        o.acquire();
        o.acquire();

        criticalSection.acquire();
        th_atoms.pop();
        o_atoms.pop();
        o_atoms.pop();
        composedMolecule = "ThO2";
        DisplayComposedMolecule(TYPE::MOLECULE);
        criticalSection.release();

        co2_generator.release();
    }
}

void ChemicalReaction_Semaphore::DisplayComposedMolecule(TYPE producedType, atom* ax)
{
    criticalSectionDisplay.acquire();
    
    if (producedType == TYPE::MOLECULE && ax == nullptr)
    {
        std::cout << "\nComposed Molecule: " << composedMolecule << "\n" << std::endl;
    }
    else if (producedType == TYPE::ATOM && ax != nullptr)
    {
        std::cout << ax->atomType << " with ID: " << ax->atomID << " is created." << std::endl;
    }

    criticalSectionDisplay.release();
}