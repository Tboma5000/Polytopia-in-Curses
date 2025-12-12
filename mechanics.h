#ifndef MECHANICS_H
#define MECHANICS_H

struct Civs;
struct City;
struct Units;
struct GAME;
struct TribeS;


//-------------------------------------------------------------------------------------
// World Generation
//-------------------------------------------------------------------------------------

void world_generation(const int size, int** world, int CIVS);
// Generate oceans, forests, mountains, tribes, cities

void resources_generation(const int size, int** resources, int** world);
// Generate resources on titels


//-------------------------------------------------------------------------------------
// Structures
//-------------------------------------------------------------------------------------


// Return number of CIV, which this territory depend
int ReturnNumbOfCiv(struct Civs* CivS, const int x, const int y, const int sizeof_territory);

// Return City, which this territory depend
struct City* ReturnCity(struct Civs* CivS, const int x, const int y);

// Return Civ with needed number of CIV
struct Civs* ReturnCiv(struct Civs* CivS, int number);

// Return a Unit of Civ, that is on this position
struct Units* ReturnUnit(struct Civs* CivS, const int x, const int y);

// Create a Civ for the start of game
void new_Civ(struct Civs** head, int number, int x, int y);

// Create a new City - zero City
void new_City(struct Civs* CIV, struct City** head, int number, int x, int y);

// Create a new Unit in certain coordinates and with certain type
void new_Unit(struct Units** head, int number, int x, int y, int type);

// Write a list with number of civ and coordinates of capital
void civs_creation(const int size, int** world, struct Civs** CivS, int CIV);


//-----------------------
// Tribes
//-----------------------


// Write all tribes coordinates
void tribes_write(const int size, int** world, struct TribeS** head);

// Check: does any unit on tribe
void check_tribe(struct Civs* CivS, struct GAME* Game);

// Delete tribe from map and add this as zero City to Civ, which unit is here
void Capture_tribe(const int size, const int x, const int y, int** world, struct Civs* CivS, struct GAME* Game);

//-------------------------------------------------------------------------------------
// Research
//-------------------------------------------------------------------------------------


// Complete certain research
void Research(struct Civs* CivS, int tech);


//---------------------------------------------
// Movement
//---------------------------------------------


//
void clear_arr(int*** Moves, int size);

// Fill array of posible moves to unit
void check_on_move(const int size, const int x, const int y, int** world, struct Civs* CivS, int*** Moves, int sizeof_Moves);

// Move unit to right position - return 1, else return 0
int Movement(const int x1, const int y1, const int x2, const int y2, struct Civs* CivS, int** Moves, int sizeof_Moves);

// in process
// Fill array Combats of posible fights
void check_on_combat(const int size, const int x, const int y, struct Civs* CivS, int*** Combats, int sizeof_Combats);

// in process
// If all good - make combat between units, return 1, else return 0
int Combat(const int size, const int x1, const int y1, const int x2, const int y2, struct Civs* CivS, int** Combats, int sizeof_Combats);


//-------------------------------
// Resources
//-------------------------------


int TakeResource(const int x, const int y, int** resources, struct Civs* CivS);


//-----------------------
// Building
//-----------------------

int Building(const int x, const int y, int** world, int** resources, struct Civs* CivS);


#endif // MECHANICS_H