#ifndef GUI_H
#define GUI_H

struct Civs;
struct ClicK;
struct CXY;

// map drawing
void map(const int size, int** world, float** resources, struct Civs* CivS, WINDOW* Technologies);

// convert coordinates of screen into coordinates of arrays
void convertor(const int size, const int screen_x, const int screen_y, struct ClicK* Click);

// draw the list of action allowed to player in this title
void click_on_map(const int size, int** world, float** resources, struct Civs* CivS, const int x, const int y, struct CXY* XY);

// Show Technology tree !!!of player CIV - get a player CIV as parametr
void tech_tree(const int size, struct Civs* CivS, struct CXY* XY, WINDOW* Technologies);

// Show a description of choosen technology and show a button to unlock it
void technology(const int size, struct Civs* CivS, int tech, struct CXY* XY, WINDOW* Technologies);

// takes coordinates and return which function(mechanic) need to lauch
int click_action(const int size, const int x, const int y, struct CXY* XY, struct Civs* CivS, WINDOW* Technologies, MEVENT* event, struct ClicK* Click);

#endif // GUI_H