#ifndef MECHANICS_H
#define MECHANICS_H

struct Civs;
struct City;
struct Units;

void world_generation(const int size, int** world, int CIVS);
// Generate oceans, forests, mountains, tribes, cities

void resources_generation(const int size, float** resources, int** world);
// Generate resources on titels

// Good stuff with structs
struct Civs* ReturnCiv(struct Civs* CivS, int number);
void new_Civ(struct Civs** head, int number, int x, int y);
void new_City(struct City** head, int number, int x, int y);
void new_Unit(struct Units** head, int number, int x, int y, int type);

void civs_creation(const int size, int** world, struct Civs** CivS, int CIV);
// Write a list with number of civ and coordinates of capital


#endif // MECHANICS_H