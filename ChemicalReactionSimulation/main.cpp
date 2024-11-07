// ChemicalReactionChemicalReaction.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "ChemicalReaction_Semaphore.h"
#include "ChemicalReaction_Mutex.h"

int main()
{
    ChemicalReaction_Mutex s(20,20,20,20,20,100);
    s.Start();
    
}