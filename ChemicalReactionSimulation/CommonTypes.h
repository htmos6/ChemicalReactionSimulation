#pragma once

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