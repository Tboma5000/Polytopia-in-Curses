#ifndef MECHANICS_H
#define MECHANICS_H

struct Civs;
struct City;
struct Units;


//-------------------------------------------------------------------------------------
// World Generation
//-------------------------------------------------------------------------------------

void world_generation(const int size, int** world, int CIVS);
// Generate oceans, forests, mountains, tribes, cities

void resources_generation(const int size, float** resources, int** world);
// Generate resources on titels


//-------------------------------------------------------------------------------------
// Structures
//-------------------------------------------------------------------------------------


// Return Civ with needed number of CIV
struct Civs* ReturnCiv(struct Civs* CivS, int number);

// Return a Unit of Civ, that is on this position
struct Units* ReturnUnit(struct Civs* CivS, const int x, const int y);

// Create a Civ for the start of game
void new_Civ(struct Civs** head, int number, int x, int y);

// Create a new City - zero City
void new_City(struct City** head, int number, int x, int y);

// Create a new Unit in certain coordinates and with certain type
void new_Unit(struct Units** head, int number, int x, int y, int type);

// Write a list with number of civ and coordinates of capital
void civs_creation(const int size, int** world, struct Civs** CivS, int CIV);


//-------------------------------------------------------------------------------------
// Research
//-------------------------------------------------------------------------------------


// Complete certain research
void Research(struct Civs* CivS, int tech);


//---------------------------------------------
// Movement
//---------------------------------------------

// Fill array of posible moves to unit
void check_on_move(const int x, const int y, int** world, struct Civs* CivS, int*** Moves);

// Move unit to right position - return 1, else return 0
int Movement(const int x1, const int y1, const int x2, const int y2, struct Civs* CivS, int** Moves);
#endif // MECHANICS_H