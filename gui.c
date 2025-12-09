#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncursesw/curses.h>
#include <stdbool.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include "mechanics.h"

#define RETREAT 10


//-----------------------------------------
// Constants
//-----------------------------------------


enum Const {
    // for function map()
    title_size = 9,
    additinal_retreat = 1,
    add_borders = 5,
    basic_territory = 9,

    // for function click_on_map()
    row_of_menu = 5,

    // for new windows
    width = 82,
    height = 44,

    // for function tech_tree()
    row_of_Tech_branch1 = 7,
    row_of_Tech_branch2 = row_of_Tech_branch1*2,
    row_of_Tech_branch3 = row_of_Tech_branch1*3,
    row_of_Tech_branch4 = row_of_Tech_branch1*4,
    row_of_Tech_branch5 = row_of_Tech_branch1*5,
    row_of_close = 40,
    arrow_len = 7,
    spaced_arrow_len = 10,

    // for function technology()
    close_button = row_of_menu+12,
    research_button = row_of_menu+12,
};


//-----------------------------------------
// Textures
//-----------------------------------------


// 0 - sea +-
// 1 - land +
// 2 - forest +
// 3 - mountain +
// 4 - tribe +-
// 5 - city +-


//----------------------------------------
// Structures
//----------------------------------------


// charakteristics of unit
struct Charakteristics {
   char* name;
   int cost;
   int hp;
   int dmg;
   int def;
   int movement;
   int range;
};

// list of all units
struct Units {
   // number of civ
   int numb;
   // coordinates of unit on map
   int x;
   int y;
   int type;
   struct Charakteristics stat;
   struct Units *next; // connection with next unit
   struct Units *prev; // connection with previos unit
};

// list of all cities
struct City {
   int numb; // number of civ
   int x;
   int y;
   int population;
   int growth;
   int production;
   int** territory;
   struct City *next; // connection with next city
   struct City *prev; // connection with previous city
};

// list of Technologies of Branch1
struct Branch1 {
    bool Hunting;
    bool Archery;
    bool Forestry;
    bool Spiritualizm;
    bool Mathematics;
};

// list of Technologies of Branch1
struct Branch2 {
    bool Riding;
    bool Roads;
    bool Free_Spirit;
    bool Trade;
    bool Chivalry;
};

// list of Technologies of Branch1
struct Branch3 {
    bool Organization;
    bool Farming;
    bool Strategy;
    bool Construction;
    bool Diplomacy;
};

// list of Technologies of Branch1
struct Branch4 {
    bool Climbing;
    bool Mining;
    bool Meditation;
    bool Smithery;
    bool Philosophy;
};

// list of Technologies of Branch1
struct Branch5 {
    bool Fishing;
    bool Sailing;
    bool Ramming;
    bool Navigation;
    bool Aquatism;
};

// list of unlocked technologies
struct Technology {
   int Tier1;
   int Tier2;
   int Tier3;
   struct Branch1 branch1;
   struct Branch2 branch2;
   struct Branch3 branch3;
   struct Branch4 branch4;
   struct Branch5 branch5;
};

// list of information of one civ
struct Civs {
   int numb; // number of civ
   int stars; // how much have
   int score;   
   int** territory;
   struct City* Cities; // list of cities under control of this civ
   struct Units* Unites; // list of units under control of this civ
   struct Technology Tech; // technologys this civ
   struct Civs *next; // connection with next civ
   struct Civs *prev; // connection with previos civ
};

// information of click
struct ClicK {
    // position on a map
    int map_x;
    int map_y;
    // position out of map
    int action_x;
    int action_y;
};

// P - means posibilities!
// list of posibilities of player in this title
struct PTitle {
    bool movement;
    bool combat;
    bool resources;
    bool spawn;
    bool build;
    bool capture_tribe;
};

// list of X and Y coordinate of screens features (Coordinate X and Y)
struct CXY {

    //------------------------------------
    // Y
    //------------------------------------


    int movement;
    int combat;
    int resources;
    int spawn;
    int build;
    int capture_tribe;

    //Technology
    int Technology;



    //---------------------------------
    // X
    //---------------------------------


    //---------------------------------
    // Branch 1
    //---------------------------------


    int Hunting;
    int Archery;
    int Spiritualizm;
    int Forestry;
    int Mathematics;
    

    //---------------------------------
    // Branch 2
    //---------------------------------


    int Riding;
    int Roads;
    int Trade;
    int Free_Spirit;
    int Chivarly;
    

    //---------------------------------
    // Branch 3
    //---------------------------------


    int Organization;
    int Farming;
    int Construction;
    int Strategy;
    int Diplomacy;
    

    //---------------------------------
    // Branch 4
    //---------------------------------


    int Climbing;
    int Mining;
    int Smithery;
    int Meditation;
    int Philosophy;
    

    //---------------------------------
    // Branch 5
    //---------------------------------


    int Fishing;
    int Ramming;
    int Aquatism;
    int Sailing;
    int Navigation;


    //------------------------------------
    // Tech
    //------------------------------------


    int retreat;
};

// list of units to hit
struct PCombat {
    struct Units Unit;
    struct PCombat *next;
    struct PCombat *prev;
};

//-------------------------------------------
// Functions
//-------------------------------------------

void map(const int size, int** world, float** resources, struct Civs* CivS, WINDOW* Technologies) {

    //--------------------------------------------------------------------------------------------------------
    // Colors
    //--------------------------------------------------------------------------------------------------------

    
    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_WHITE, COLOR_GREEN);
    init_pair(4, COLOR_WHITE, COLOR_YELLOW);


    //--------------------------------------------
    // Drawing a world
    //--------------------------------------------


    for (int x=0; x<size; x++) {
        int row = x*4+2;
        mvprintw(1, RETREAT, "+");
        for (int y=0; y<size+add_borders; y++) {
            printw("-+-+-+");
        }
        mvprintw(row, RETREAT, "|");
        mvprintw(row+1, RETREAT, "|");
        mvprintw(row+2, RETREAT, "|");
        for (int y=0; y<size; y++) {
            if (world[x][y] == 0) {
                mvprintw(row, RETREAT+y*title_size+additinal_retreat, " 🌊🌊🌊 |");
                mvprintw(row+1, RETREAT+y*title_size+additinal_retreat, " 🌊🌊🌊 |");
                mvprintw(row+2, RETREAT+y*title_size+additinal_retreat, " 🌊🌊🌊 |");
            } else if (world[x][y] == 1) {
                mvprintw(row, RETREAT+y*title_size+additinal_retreat, " 🟩🟩🟩 |");
                mvprintw(row+1, RETREAT+y*title_size+additinal_retreat, " 🟩🟩🟩 |");
                mvprintw(row+2, RETREAT+y*title_size+additinal_retreat, " 🟩🟩🟩 |");
            } else if (world[x][y] == 2){
                mvprintw(row, RETREAT+y*title_size+additinal_retreat, " 🌲🌲🌲 |");
                mvprintw(row+1, RETREAT+y*title_size+additinal_retreat, " 🌲🌲🌲 |");
                mvprintw(row+2, RETREAT+y*title_size+additinal_retreat, " 🌲🌲🌲 |");
            } else if (world[x][y] == 3){
                mvprintw(row, RETREAT+y*title_size+additinal_retreat, " ⛰️ ⛰️ ⛰️  |");
                mvprintw(row+1, RETREAT+y*title_size+additinal_retreat, " ⛰️ ⛰️ ⛰️  |");
                mvprintw(row+2, RETREAT+y*title_size+additinal_retreat, " ⛰️ ⛰️ ⛰️  |");
            } else if (world[x][y] == 4){
                mvprintw(row, RETREAT+y*title_size+additinal_retreat, " 🟩🟩🟩 |");
                mvprintw(row+1, RETREAT+y*title_size+additinal_retreat, " 🟩⛺🟩 |");
                mvprintw(row+2, RETREAT+y*title_size+additinal_retreat, " 🟩🟩🟩 |");
            } else if (world[x][y] == 5){ 
                mvprintw(row, RETREAT+y*title_size+additinal_retreat, " 🟩🟩🟩 |");
                mvprintw(row+1, RETREAT+y*title_size+additinal_retreat, " 🟩🏰🟩 |");
                mvprintw(row+2, RETREAT+y*title_size+additinal_retreat, " 🟩🟩🟩 |");
            }
        }
        mvprintw(x*4+5, RETREAT, "+");
        for (int y=0; y<size+add_borders; y++) {
            printw("-+-+-+");
        }
    }


    //-----------------------------------------------
    // Color territory
    //-----------------------------------------------


    struct Civs* CIV = CivS;
    while (CIV!=NULL) {
        for (int x=0; x<size; x++) {
            int row = x*4+2;
            for (int y=0; y<size; y++) {
                for (int i=0; i<basic_territory; i++) {
                    if (CIV->territory[i][0] == x && CIV->territory[i][1] == y) {
                        mvchgat(row-1, RETREAT+y*title_size+additinal_retreat-1, title_size+1, A_NORMAL, CIV->numb, NULL);
                        mvchgat(row, RETREAT+y*title_size+additinal_retreat-1, title_size+1, A_NORMAL, CIV->numb, NULL);
                        mvchgat(row+1, RETREAT+y*title_size+additinal_retreat-1, title_size+1, A_NORMAL, CIV->numb, NULL);
                        mvchgat(row+2, RETREAT+y*title_size+additinal_retreat-1, title_size+1, A_NORMAL, CIV->numb, NULL);
                        mvchgat(row+3, RETREAT+y*title_size+additinal_retreat-1, title_size+1, A_NORMAL, CIV->numb, NULL);
                    }
                }
            }
        }
        CIV = CIV->next;
    }


    //-----------------------------------------------
    // Drawing resources
    //-----------------------------------------------


    for (int x=0; x<size; x++) {
        int row = x*4+2;
        char* symb = (char*)calloc(6, sizeof(char));
        for (int y=0; y<size; y++) {
            if (world[x][y] == 0 && resources[x][y] == 0.5) {
                symb = "🐬";
            } else if (world[x][y] == 1 && resources[x][y] == 0.5) {
                symb = "🫐";
            } else if (world[x][y] == 1 && resources[x][y] == 1) {
                symb = "🌾";
            } else if (world[x][y] == 2 && resources[x][y] == 1.5) {
                symb = "🐗";
            } else if (world[x][y] == 3 && resources[x][y] == 1) {
                symb = "🪙";
            }


            if (world[x][y] == 0 && resources[x][y] == 0.5) {
                mvprintw(row, RETREAT+y*title_size+additinal_retreat+1, "%s", symb);
                mvprintw(row, RETREAT+y*title_size+additinal_retreat+4, "%s", symb);
                mvprintw(row+1, RETREAT+y*title_size+additinal_retreat+3, "%s", symb);
                mvprintw(row+2, RETREAT+y*title_size+additinal_retreat+1, "%s", symb);
                mvprintw(row+2, RETREAT+y*title_size+additinal_retreat+4, "%s", symb);
            } else if (world[x][y] == 1 && resources[x][y] == 0.5) {
                mvprintw(row, RETREAT+y*title_size+additinal_retreat+1, "%s", symb);
                mvprintw(row, RETREAT+y*title_size+additinal_retreat+4, "%s", symb);
                mvprintw(row+1, RETREAT+y*title_size+additinal_retreat+3, "%s", symb);
                mvprintw(row+2, RETREAT+y*title_size+additinal_retreat+1, "%s", symb);
                mvprintw(row+2, RETREAT+y*title_size+additinal_retreat+4, "%s", symb);
            } else if (world[x][y] == 1 && resources[x][y] == 1) {
                mvprintw(row, RETREAT+y*title_size+additinal_retreat+1, "%s", symb);
                mvprintw(row, RETREAT+y*title_size+additinal_retreat+4, "%s", symb);
                mvprintw(row+1, RETREAT+y*title_size+additinal_retreat+3, "%s", symb);
                mvprintw(row+2, RETREAT+y*title_size+additinal_retreat+1, "%s", symb);
                mvprintw(row+2, RETREAT+y*title_size+additinal_retreat+4, "%s", symb);
            } else if (world[x][y] == 2 && resources[x][y] == 1.5) {
                mvprintw(row, RETREAT+y*title_size+additinal_retreat+1, "%s", symb);
                mvprintw(row, RETREAT+y*title_size+additinal_retreat+4, "%s", symb);
                mvprintw(row+1, RETREAT+y*title_size+additinal_retreat+3, "%s", symb);
                mvprintw(row+2, RETREAT+y*title_size+additinal_retreat+1, "%s", symb);
                mvprintw(row+2, RETREAT+y*title_size+additinal_retreat+4, "%s", symb);
            } else if (world[x][y] == 3 && resources[x][y] == 1) {
                mvprintw(row, RETREAT+y*title_size+additinal_retreat+1, "%s", symb);
                mvprintw(row, RETREAT+y*title_size+additinal_retreat+5, "%s", symb);
                mvprintw(row+1, RETREAT+y*title_size+additinal_retreat+3, "%s", symb);
                mvprintw(row+2, RETREAT+y*title_size+additinal_retreat+1, "%s", symb);
                mvprintw(row+2, RETREAT+y*title_size+additinal_retreat+5, "%s", symb);
            }
        }
    }


    //-----------------------------------------------
    // Drawing units
    //-----------------------------------------------


    struct Civs* CV = CivS;
    for (int x=0; x<size; x++) {
        int row = x*4+2;
        char* symb = calloc(6, 1);
        for (int y=0; y<size; y++) {
            if (CV->Unites->x == x && CV->Unites->y == y) {
                //⚔️  🐎  🏹  👹  🛡️  🏗️  🏇  🗡️

                switch(CV->Unites->type) {
                    case 1: symb = "⚔️"; break;
                    case 2: symb = "🐎"; break;
                    case 3: symb = "🏹"; break;
                    case 4: symb = "👹"; break;
                    case 5: symb = "🛡️"; break;
                    case 6: symb = "🏗️"; break;
                    case 7: symb = "🏇"; break;
                    case 8: symb = "🗡️"; break;
                }

                //symb = "⚔️";
                if (world[x][y] != 4 && world[x][y] != 5) {
                    mvprintw(row+1, RETREAT+y*title_size+additinal_retreat+3, ("%s "), symb);
                } else {
                    mvprintw(row+1, RETREAT+y*title_size+additinal_retreat+1, ("%s "), symb);
                }
                if (CV->next!=NULL) {
                    CV = CV->next;
                }
            }
        }
    }


    //-----------------------------------------------
    // Drawing Technology 'Button'
    //-----------------------------------------------


    mvwprintw(Technologies, 2, 8, " Technology");

    
    refresh();
    wrefresh(Technologies);
}


void convertor(const int size, const int screen_x, const int screen_y, struct ClicK* Click) {
    for (int x=0; x<size; x++) {
        int row = x*4+2;
        for (int y=0; y<size; y++) {
            int column = RETREAT+y*title_size+additinal_retreat;
            if (screen_x >= column && screen_x <= column+9 && screen_y >= row && screen_y <= row+2) {
                Click->map_x = x;
                Click->map_y = y;
                return;
            }
        }
    }
}


void add_Combat(struct PCombat** head, struct Units* EnemyUnit) {
    struct PCombat* newCombat = malloc(sizeof(struct PCombat));
    if (!newCombat) return;
    newCombat->Unit = *EnemyUnit;

    if (*head == NULL) {
        *head = newCombat;
    } else {
        struct PCombat* temp = *head;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newCombat;
        newCombat->prev = temp;
    }
}

// !!! require improvement P.S.: a lot
void click_on_map(const int size, int** world, float** resources, struct Civs* CivS, const int x, const int y, struct CXY* XY) {

    // delete this in future
    clear();


    //---------------------------------------------
    // Null all cordinates
    //---------------------------------------------
    
    
    XY->build = 0;
    XY->capture_tribe = 0;
    XY->combat = 0;
    XY->movement = 0;
    XY->resources = 0;
    XY->spawn = 0;
    XY->Technology = 2;

    //--------------------------------------------
    // False all posibilities
    //--------------------------------------------


    struct PTitle* TitlE = malloc(sizeof(struct PTitle));
    TitlE->build = false;
    TitlE->capture_tribe = false;
    TitlE->combat = false;
    TitlE->movement = false;
    TitlE->resources = false;
    TitlE->spawn = false;


    struct Civs* CV = CivS;
    struct PCombat* Combat; // write units, that can atack
    struct Units* Unit;
    int check_on_CV = 0; // to stop unneeded checks
    while(CV!=NULL) {
        struct Units* UN = CV->Unites;
        while(UN!=NULL) {
            if (UN->x == x && UN->y == y) {
                check_on_CV = 1;
                Unit = UN;
                

                //----------------------------
                // Movement 
                //----------------------------
                TitlE->movement = true;
                

                //---------
                // here must be code, that indentify where unit can move
                // need already complete mechanics of movement, technologies and buildings
                //---------


                //----------------------------
                // Combat
                //----------------------------


                struct Civs* Enemy = CV;
                Enemy = Enemy->next;
                while(Enemy!=NULL) {
                    struct Units* EnemyUnit = Enemy->Unites;
                    while (EnemyUnit!=NULL) {
                        for (int dx=x-UN->stat.range; dx <=x+UN->stat.range; dx++) {
                            for (int dy=y-UN->stat.range; dy <= y+UN->stat.range; dy++) {
                                if (dx == EnemyUnit->x && dy == EnemyUnit->y) {
                                    add_Combat(&Combat, EnemyUnit);
                                    TitlE->combat = true;
                                }
                            }
                        }
                        EnemyUnit = EnemyUnit->next;
                    }
                    Enemy = Enemy->next;
                }
            }
            if (check_on_CV == 1) break;
            UN = UN->next;
        }
        if (check_on_CV == 1) break;
        CV = CV->next;
    }


    //-----------------------------
    // Grab resources
    //-----------------------------


    struct Civs* CIV = CivS;
    int check_for_CV = 0;
    while(CIV!=NULL) {
        for (int x=0; x<basic_territory; x++) {
            if (CIV->territory[x][0] == x && CIV->territory[x][1] == y) {
                check_for_CV = 1;
                if (resources[x][y] == 0.5) {
                    if (world[x][y] == 0) {
                        if (CIV->Tech.branch5.Fishing == true) {
                            TitlE->resources = true;
                        }
                    }
                    if (world[x][y] == 2) {
                        if (CIV->Tech.branch3.Organization == true) {
                            TitlE->resources = true;
                        }
                    }
                } else if (resources[x][y] == 1.5) {
                    if (CIV->Tech.branch1.Hunting == true) {
                        TitlE->resources = true;
                    }
                } else {
                    TitlE->resources = false;
                }
                break;
            }
        }
        if (check_for_CV == 1) {
            break;
        }
        CIV = CIV->next;
    }

    



    //-----------------------------
    // Spawn unit
    //-----------------------------


    if (world[x][y] == 5) {
        TitlE->spawn = true;
    }


    // !!!
    //-----------------------------
    // Buildings
    //-----------------------------


    //here must be code to identify - can player build something here or not
    //need already complete mechanics of technologies and buildings


    // !!!
    //-----------------------------
    // Capture Tribe
    //-----------------------------


    if (world[x][y] == 4 && Unit->stat.movement != 0) {
        TitlE->capture_tribe = true;
    }


    // !!!
    //-----------------------------
    // Capture City
    //-----------------------------


    //here must be code to identify - can player capture this city or not
    //need already complete mechanics of capturing city


    //-----------------------------
    // Drawing menu of actions
    //-----------------------------


    int count = 0; // count of actions
    mvprintw(row_of_menu, size*size+RETREAT, "+-+-+-+-+-+-+-+-+-+-+");
    if (TitlE->movement == true) {
        count++;
        mvprintw(row_of_menu+count, size*size+RETREAT, "%d. Movement", count);
        XY->movement = row_of_menu+count;
    } 
    if (TitlE->combat == true) {
        count++;
        mvprintw(row_of_menu+count, size*size+RETREAT, "%d. Combat", count);
        XY->combat = row_of_menu+count;
    }
    if (TitlE->resources == true) {
        count++;
        mvprintw(row_of_menu+count, size*size+RETREAT, "%d. Grab Resources", count);
        XY->resources = row_of_menu+count;
    }
    if (TitlE->spawn == true) {
        count++;
        mvprintw(row_of_menu+count, size*size+RETREAT, "%d. Spawn Unit", count);
        XY->spawn = row_of_menu+count;
    }
    if (TitlE->build == true) {
        count++;
        mvprintw(row_of_menu+count, size*size+RETREAT, "%d. Build", count);
        XY->build = row_of_menu+count;
    }
    if (TitlE->capture_tribe == true) {
        count++;
        mvprintw(row_of_menu+count, size*size+RETREAT, "%d. Capture Tribe", count);
        XY->capture_tribe = row_of_menu+count;
    }
    /* Capture city
    if (TitlE->combat == true) {
        count++;
        mvprintw(row_of_menu+count, size*size+RETREAT, "%d. Combat", count);
    }
    */
    mvprintw(row_of_menu+count+1, size*size+RETREAT, "+-+-+-+-+-+-+-+-+-+-+");


    refresh();
}

void tech_tree(const int size, struct Civs* CivS, struct CXY* XY, WINDOW* Technologies) {

    //---------------------------------
    // Colors
    //---------------------------------


    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_WHITE, COLOR_GREEN);
    init_pair(4, COLOR_WHITE, COLOR_YELLOW);


    //--------------------------------
    // Create a new window
    //--------------------------------


    


    //---------------------------------
    // Drawing a tree
    //---------------------------------


    mvwprintw(Technologies, 2, 8, " Technologies ");


    //-------------------------------------
    // Branch 1
    //-------------------------------------


    mvwprintw(Technologies, row_of_Tech_branch1-2, RETREAT+strlen("Hunting ")+2, "--->   ");
    if (CivS->Tech.branch1.Archery == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Archery ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Archery ");
    }
    XY->Archery = RETREAT+strlen("Hunting ")+2+arrow_len;
    wprintw(Technologies, "   --->   ");
    if (CivS->Tech.branch1.Spiritualizm == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, "Spiritualizm");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Spiritualizm ");
    }
    XY->Spiritualizm = RETREAT+strlen("Hunting ")+2+arrow_len+strlen(" Archery ")+spaced_arrow_len;
    mvwprintw(Technologies, row_of_Tech_branch1-1, RETREAT+strlen("Hunting ")+1, "/");
    wmove(Technologies, row_of_Tech_branch1, RETREAT);
    if (CivS->Tech.branch1.Hunting == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Hunting ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Hunting ");
    }
    XY->Hunting = RETREAT;
    mvwprintw(Technologies, row_of_Tech_branch1+1, RETREAT+strlen("Hunting ")+1, "\\");
    mvwprintw(Technologies, row_of_Tech_branch1+2, RETREAT+strlen("Hunting ")+2, "--->   ");
    if (CivS->Tech.branch1.Forestry == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Forestry ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Forestry ");
    }
    XY->Forestry = RETREAT+strlen("Hunting ")+2+arrow_len;
    wprintw(Technologies, "   --->   ");
    if (CivS->Tech.branch1.Mathematics == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Mathematics ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Mathematics ");
    }
    XY->Mathematics = RETREAT+strlen("Hunting ")+2+arrow_len+strlen(" Forestry ")+spaced_arrow_len;


    //-------------------------------------
    // Branch 2
    //-------------------------------------


    mvwprintw(Technologies, row_of_Tech_branch2-2, RETREAT+strlen("Riding ")+2, "--->   ");
    if (CivS->Tech.branch2.Roads == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Roads ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Roads ");
    }
    XY->Roads = RETREAT+strlen("Riding ")+2+arrow_len;
    wprintw(Technologies, "   --->   ");
    if (CivS->Tech.branch2.Trade == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Trade ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Trade ");
    }
    XY->Trade = RETREAT+strlen("Riding ")+2+arrow_len+strlen(" Roads ")+spaced_arrow_len;
    mvwprintw(Technologies, row_of_Tech_branch2-1, RETREAT+strlen("Riding ")+1, "/");
    wmove(Technologies, row_of_Tech_branch2, RETREAT);
    if (CivS->Tech.branch2.Riding == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Riding ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Riding ");
    }
    XY->Riding = RETREAT;
    mvwprintw(Technologies, row_of_Tech_branch2+1, RETREAT+strlen("Riding ")+1, "\\");
    mvwprintw(Technologies, row_of_Tech_branch2+2, RETREAT+strlen("Riding ")+2, "--->   ");
    if (CivS->Tech.branch2.Free_Spirit == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Free Spirit ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Free Spirit ");
    }
    XY->Free_Spirit = RETREAT+strlen("Riding ")+2+arrow_len;
    wprintw(Technologies, "   --->   ");
    if (CivS->Tech.branch2.Chivalry == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Chivalry ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Chivalry ");
    }
    XY->Chivarly = RETREAT+strlen("Riding ")+2+arrow_len+strlen(" Free Spirit ")+spaced_arrow_len;


    //-------------------------------------
    // Branch 3
    //-------------------------------------


    mvwprintw(Technologies, row_of_Tech_branch3-2, RETREAT+strlen("Organization ")+2, "--->   ");
    if (CivS->Tech.branch3.Farming == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Farming ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Farming ");
    }
    XY->Farming = RETREAT+strlen("Organization ")+2+arrow_len;
    wprintw(Technologies, "   --->   ");
    if (CivS->Tech.branch3.Construction == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, "Construction");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Construction ");
    }
    XY->Construction = RETREAT+strlen("Organization ")+2+arrow_len+strlen(" Farming ")+spaced_arrow_len;
    mvwprintw(Technologies, row_of_Tech_branch3-1, RETREAT+strlen("Organization ")+1, "/");
    wmove(Technologies, row_of_Tech_branch3, RETREAT);
    if (CivS->Tech.branch3.Organization == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Organization ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Organization ");
    }
    XY->Organization = RETREAT;
    mvwprintw(Technologies, row_of_Tech_branch3+1, RETREAT+strlen("Organization ")+1, "\\");
    mvwprintw(Technologies, row_of_Tech_branch3+2, RETREAT+strlen("Organization ")+2, "--->   ");
    if (CivS->Tech.branch3.Strategy == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Strategy ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Strategy ");
    }
    XY->Strategy = RETREAT+strlen("Organization ")+2+arrow_len;
    wprintw(Technologies, "   --->   ");
    if (CivS->Tech.branch3.Diplomacy == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Diplomacy ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Diplomacy ");
    }
    XY->Diplomacy = RETREAT+strlen("Organization ")+2+arrow_len+strlen(" Strategy ")+spaced_arrow_len;


    //-------------------------------------
    // Branch 4
    //-------------------------------------


    mvwprintw(Technologies, row_of_Tech_branch4-2, RETREAT+strlen("Climbing ")+2, "--->   ");
    if (CivS->Tech.branch4.Mining == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Mining ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Mining ");
    }
    XY->Mining = RETREAT+strlen("Climbing ")+2+arrow_len;
    wprintw(Technologies, "   --->   ");
    if (CivS->Tech.branch4.Smithery == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Smithery ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Smithery ");
    }
    XY->Smithery = RETREAT+strlen("Climbing ")+2+arrow_len+strlen(" Mining ")+spaced_arrow_len;
    mvwprintw(Technologies, row_of_Tech_branch4-1, RETREAT+strlen("Climbing ")+1, "/");
    wmove(Technologies, row_of_Tech_branch4, RETREAT);
    if (CivS->Tech.branch4.Climbing == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Climbing ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Climbing ");
    }
    XY->Climbing = RETREAT;
    mvwprintw(Technologies, row_of_Tech_branch4+1, RETREAT+strlen("Climbing ")+1, "\\");
    mvwprintw(Technologies, row_of_Tech_branch4+2, RETREAT+strlen("Climbing ")+2, "--->   ");
    if (CivS->Tech.branch4.Meditation == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Meditation ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Meditation ");
    }
    XY->Meditation = RETREAT+strlen("Climbing ")+2+arrow_len;
    wprintw(Technologies, "   --->   ");
    if (CivS->Tech.branch4.Philosophy == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Philosophy ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Philosophy ");
    }
    XY->Philosophy = RETREAT+strlen("Climbing ")+2+arrow_len+strlen(" Meditation ")+spaced_arrow_len;


    //-------------------------------------
    // Branch 5
    //-------------------------------------


    mvwprintw(Technologies, row_of_Tech_branch5-2, RETREAT+strlen("Fishing ")+2, "--->   ");
    if (CivS->Tech.branch5.Ramming == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Ramming ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Ramming ");
    }
    XY->Ramming = RETREAT+strlen("Fishing ")+2+arrow_len;
    wprintw(Technologies, "   --->   ");
    if (CivS->Tech.branch5.Aquatism == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Aquatism ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Aquatism ");
    }
    XY->Aquatism = RETREAT+strlen("Fishing ")+2+arrow_len+strlen(" Ramming ")+spaced_arrow_len;
    mvwprintw(Technologies, row_of_Tech_branch5-1, RETREAT+strlen("Fishing ")+1, "/");
    wmove(Technologies, row_of_Tech_branch5, RETREAT);
    if (CivS->Tech.branch5.Fishing == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Fishing ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Fishing ");
    }
    XY->Fishing = RETREAT;
    mvwprintw(Technologies, row_of_Tech_branch5+1, RETREAT+strlen("Fishing ")+1, "\\");
    mvwprintw(Technologies, row_of_Tech_branch5+2, RETREAT+strlen("Fishing ")+2, "--->   ");
    if (CivS->Tech.branch5.Sailing == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Sailing ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Sailing ");
    }
    XY->Sailing = RETREAT+strlen("Fishing ")+2+arrow_len;
    wprintw(Technologies, "   --->   ");
    if (CivS->Tech.branch5.Navigation == true) {
        wattron(Technologies, COLOR_PAIR(3));
        wprintw(Technologies, " Navigation ");
        wattroff(Technologies, COLOR_PAIR(3));
    } else {
        wprintw(Technologies, " Navigation ");
    }
    XY->Navigation = RETREAT+strlen("Fishing ")+2+arrow_len+strlen(" Sailing ")+spaced_arrow_len;


    //---------------------------
    // Button Close
    //---------------------------


    mvwprintw(Technologies, row_of_close, RETREAT, "Close Window");
    

    wrefresh(Technologies);
}

void technology(const int size, struct Civs* CivS, int tech, struct CXY* XY, WINDOW* Technologies) {

    wclear(Technologies);

    //---------------------------------
    // Colors
    //---------------------------------


    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_WHITE, COLOR_GREEN);
    init_pair(4, COLOR_WHITE, COLOR_YELLOW);    


    //----------------------------------
    // Nulling all XY coordinates
    //----------------------------------


    XY->Archery = 0;
    XY->Spiritualizm = 0;
    XY->Hunting = 0;
    XY->Forestry = 0;
    XY->Mathematics = 0;

    XY->Roads = 0;
    XY->Trade = 0;
    XY->Riding = 0;
    XY->Free_Spirit = 0;
    XY->Chivarly = 0;

    XY->Farming = 0;
    XY->Construction = 0;
    XY->Organization = 0;
    XY->Strategy = 0;
    XY->Diplomacy = 0;

    XY->Mining = 0;
    XY->Smithery = 0;
    XY->Climbing = 0;
    XY->Meditation = 0;
    XY->Philosophy = 0;

    XY->Ramming = 0;
    XY->Aquatism = 0;
    XY->Fishing = 0;
    XY->Sailing = 0;
    XY->Navigation = 0;


    //---------------------------------
    // Drawing technology menu
    //---------------------------------


    //---------------------------------
    // Drawing a title - name of technology
    //---------------------------------


    mvwprintw(Technologies, row_of_menu+1, RETREAT, "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+"); // ?
    wmove(Technologies, row_of_menu+2, RETREAT+25);
    switch(tech) {

        
        //---------------------------------
        // Branch 1
        //---------------------------------


        case 7: 
            if (CivS->Tech.branch1.Archery == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Archery ");
            if (CivS->Tech.branch1.Archery == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;
        case 8: 
            if (CivS->Tech.branch1.Spiritualizm == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Spiritualizm ");
            if (CivS->Tech.branch1.Spiritualizm == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;
        case 9: 
            if (CivS->Tech.branch1.Hunting == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Hunting ");
            if (CivS->Tech.branch1.Hunting == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;
        case 10: 
            if (CivS->Tech.branch1.Forestry == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Forestry ");
            if (CivS->Tech.branch1.Forestry == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;
        case 11: 
            if (CivS->Tech.branch1.Mathematics == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Mathematics ");
            if (CivS->Tech.branch1.Mathematics == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;


        //---------------------------------
        // Branch 2
        //---------------------------------


        case 12: 
            if (CivS->Tech.branch2.Roads == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Roads ");
            if (CivS->Tech.branch2.Roads == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;
        case 13: 
            if (CivS->Tech.branch2.Trade == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Trade ");
            if (CivS->Tech.branch2.Trade == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;
        case 14: 
            if (CivS->Tech.branch2.Riding == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Riding ");
            if (CivS->Tech.branch2.Riding == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;
        case 15: 
            if (CivS->Tech.branch2.Free_Spirit == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Free Spirit ");
            if (CivS->Tech.branch2.Free_Spirit == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;
        case 16: 
            if (CivS->Tech.branch2.Chivalry == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Chivalry ");
            if (CivS->Tech.branch2.Chivalry == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;


        //---------------------------------
        // Branch 3
        //---------------------------------


        case 17: 
            if (CivS->Tech.branch3.Farming == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Farming ");
            if (CivS->Tech.branch3.Farming == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;
        case 18: 
            if (CivS->Tech.branch3.Construction == true) {
               wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Construction ");
            if (CivS->Tech.branch3.Construction == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;
        case 19: 
            if (CivS->Tech.branch3.Organization == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Organization ");
            if (CivS->Tech.branch3.Organization == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;
        case 20: 
            if (CivS->Tech.branch3.Strategy == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Strategy ");
            if (CivS->Tech.branch3.Strategy == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;
        case 21: 
            if (CivS->Tech.branch3.Diplomacy == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Diplomacy ");
            if (CivS->Tech.branch3.Diplomacy == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;


        //---------------------------------
        // Branch 4
        //---------------------------------


        case 22: 
            if (CivS->Tech.branch4.Mining == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Mining ");
            if (CivS->Tech.branch4.Mining == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;
        case 23: 
            if (CivS->Tech.branch4.Smithery == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Smithery ");
            if (CivS->Tech.branch4.Smithery == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;
        case 24: 
            if (CivS->Tech.branch4.Climbing == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Climbing ");
            if (CivS->Tech.branch4.Climbing == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;
        case 25: 
            if (CivS->Tech.branch4.Meditation == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Meditation ");
            if (CivS->Tech.branch4.Meditation == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;
        case 26: 
            if (CivS->Tech.branch4.Philosophy == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Philosophy ");
            if (CivS->Tech.branch4.Philosophy == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;


        //---------------------------------
        // Branch 5
        //---------------------------------


        case 27: 
            if (CivS->Tech.branch5.Ramming == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Ramming ");
            if (CivS->Tech.branch5.Ramming == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;
        case 28: 
            if (CivS->Tech.branch5.Aquatism == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Aquatism ");
            if (CivS->Tech.branch5.Aquatism == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;
        case 29: 
            if (CivS->Tech.branch5.Fishing == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Fishing ");
            if (CivS->Tech.branch5.Fishing == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;
        case 30: 
            if (CivS->Tech.branch5.Sailing == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Sailing ");
            if (CivS->Tech.branch5.Sailing == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;
        case 31: 
            if (CivS->Tech.branch5.Navigation == true) {
                wattron(Technologies, COLOR_PAIR(3));
            }
            wprintw(Technologies, " Navigation ");
            if (CivS->Tech.branch5.Navigation == true) {
                wattroff(Technologies, COLOR_PAIR(3));
            }
            break;
    }


    //-------------------------------
    // Draw alarm if stars is lower that cost
    //-------------------------------


    wmove(Technologies, row_of_menu+4, RETREAT);
    wattron(Technologies, COLOR_PAIR(1));
    switch(tech) {

        //-----------------------------
        // Branch 1
        //-----------------------------


        case 7: 
            if (CivS->Tech.branch1.Archery == false && CivS->stars < CivS->Tech.Tier2) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;
        case 8: 
            if (CivS->Tech.branch1.Spiritualizm == false && CivS->stars < CivS->Tech.Tier3) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;
        case 9: 
            if (CivS->Tech.branch1.Hunting == false && CivS->stars < CivS->Tech.Tier1) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;
        case 10: 
            if (CivS->Tech.branch1.Forestry == false && CivS->stars < CivS->Tech.Tier2) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;
        case 11: 
            if (CivS->Tech.branch1.Mathematics == false && CivS->stars < CivS->Tech.Tier3) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;


        //-----------------------------
        // Branch 2
        //-----------------------------


        case 12:
            if (CivS->Tech.branch2.Roads == false && CivS->stars < CivS->Tech.Tier2) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;
        case 13:
            if (CivS->Tech.branch2.Trade == false && CivS->stars < CivS->Tech.Tier3) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;
        case 14:
            if (CivS->Tech.branch2.Riding == false && CivS->stars < CivS->Tech.Tier1) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;
        case 15:
            if (CivS->Tech.branch2.Free_Spirit == false && CivS->stars < CivS->Tech.Tier2) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;
        case 16:
            if (CivS->Tech.branch2.Chivalry == false && CivS->stars < CivS->Tech.Tier3) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;


        //-----------------------------
        // Branch 3
        //-----------------------------


        case 17: 
            if (CivS->Tech.branch3.Farming == false && CivS->stars < CivS->Tech.Tier2) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;
        case 18: 
            if (CivS->Tech.branch3.Construction == false && CivS->stars < CivS->Tech.Tier3) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;
        case 19: 
            if (CivS->Tech.branch3.Organization == false && CivS->stars < CivS->Tech.Tier1) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;
        case 20: 
            if (CivS->Tech.branch3.Strategy == false && CivS->stars < CivS->Tech.Tier2) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;
        case 21: 
            if (CivS->Tech.branch3.Diplomacy == false && CivS->stars < CivS->Tech.Tier3) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;


        //-----------------------------
        // Branch 4
        //-----------------------------


        case 22:
            if (CivS->Tech.branch4.Mining == false && CivS->stars < CivS->Tech.Tier2) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;
        case 23:
            if (CivS->Tech.branch4.Smithery == false && CivS->stars < CivS->Tech.Tier3) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;
        case 24:
            if (CivS->Tech.branch4.Climbing == false && CivS->stars < CivS->Tech.Tier1) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;
        case 25:
            if (CivS->Tech.branch4.Meditation == false && CivS->stars < CivS->Tech.Tier2) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;
        case 26:
            if (CivS->Tech.branch4.Philosophy == false && CivS->stars < CivS->Tech.Tier3) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;


        //-----------------------------
        // Branch 5
        //-----------------------------


        case 27:
            if (CivS->Tech.branch5.Ramming == false && CivS->stars < CivS->Tech.Tier2) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;
        case 28:
            if (CivS->Tech.branch5.Aquatism == false && CivS->stars < CivS->Tech.Tier3) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;
        case 29:
            if (CivS->Tech.branch5.Fishing == false && CivS->stars < CivS->Tech.Tier1) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;
        case 30:
            if (CivS->Tech.branch5.Sailing == false && CivS->stars < CivS->Tech.Tier2) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;
        case 31:
            if (CivS->Tech.branch5.Navigation == false && CivS->stars < CivS->Tech.Tier3) {
                wprintw(Technologies, " You don't have enoght stars to unlock it! ");
            }
            break;

    }
    wattroff(Technologies, COLOR_PAIR(1));


    //-------------------------------
    // Draw description
    //-------------------------------

    int retreat = 8;
    mvwprintw(Technologies, row_of_menu+6, RETREAT, "Description: ");
    wmove(Technologies, row_of_menu+7, RETREAT);
    switch(tech) {

        //------------------------
        // Branch 1
        //------------------------


        case 7: 
            wprintw(Technologies, " Unlock the unit Archer and give 1 def to every unit in forest ");
            break;
        case 8: 
            wprintw(Technologies, " Unlock an ability to plant forest and build Forest Temples ");
            mvwprintw(Technologies, row_of_menu+retreat, RETREAT, " --- in process --- ");
            retreat++;
            break;
        case 9: 
            wprintw(Technologies, " Unlock an ability to take animals from the forest ");
            mvwprintw(Technologies, row_of_menu+retreat, RETREAT, " as resources to grow up your cities ");
            retreat++;
            break;
        case 10: 
            wprintw(Technologies, " Unlock an ability to build forest camp to grow up your cities ");
            break;
        case 11: 
            wprintw(Technologies, " Unlock the unit Capapult and an ability to build sawmill ");
            mvwprintw(Technologies, row_of_menu+retreat, RETREAT, " Sawmill give 1 population for every abjacent forest camp ");
            retreat++;
            break;


        //------------------------
        // Branch 2
        //------------------------


        case 12:
            wprintw(Technologies, " Unlock an ability to build roads. ");
            mvwprintw(Technologies, row_of_menu+retreat, RETREAT, " On roads units move 2x faster --- in process ");
            retreat++;
            break;
        case 13:
            wprintw(Technologies, " Unlock an ability to build markets, that gives 1 star per turn ");
            mvwprintw(Technologies, row_of_menu+retreat, RETREAT, " for every abjacent building: sawmill, windmill, forge --- in process");
            retreat++;
            break;
        case 14:
            wprintw(Technologies, " Unlock the unit Rider, that have more movement than warrior ");
            break;
        case 15:
            wprintw(Technologies, " Unlock an ability to disband your unit and get stars for it ");
            mvprintw(row_of_menu+retreat, size*size+RETREAT, " --- in process --- ");
            mvprintw(row_of_menu+retreat+1, size*size+RETREAT, " Also give permition to build temples --- in process");
            retreat += 2;
            break;
        case 16:
            wprintw(Technologies, " Unlock the unit Knight, that can destroy enemies units one by one! ");
            mvwprintw(Technologies, row_of_menu+retreat, RETREAT, " Also give an ability to destroy your buildings --- in process ");
            retreat++;
            break;

        
        //------------------------
        // Branch 3
        //------------------------


        case 17:
            wprintw(Technologies, " Unlock an ability to build farms on wheat, that give a 2 population ");
            break;
        case 18:
            wprintw(Technologies, " Unlock an ability to build windmills that ");
            mvwprintw(Technologies, row_of_menu+retreat, RETREAT, " Windmills give 1 population for every abjacent farm ");
            mvwprintw(Technologies, row_of_menu+retreat+1, RETREAT, " Also give ability to destroy forests ");
            retreat += 2;
            break;
        case 19:
            wprintw(Technologies, " Unlock an ability to take resources from land - berries ");
            mvwprintw(Technologies, row_of_menu+retreat, RETREAT, " It crease your sity population by 1 ");
            retreat++;
            break;
        case 20:
            wprintw(Technologies, " Unlock the unit Defender, that have much more defensive than others ");
            mvwprintw(Technologies, row_of_menu+retreat, RETREAT, " Also give an ability to write a Peace Treaty with others CIVs ");
            mvwprintw(Technologies, row_of_menu+retreat+1, RETREAT, " --- in process --- ");
            retreat += 2;
            break;
        case 21:
            wprintw(Technologies, " Unlock the unit Cloak, that work as spy ");
            mvwprintw(Technologies, row_of_menu+retreat, RETREAT, " Unlock Embassy, that can be build in others CIVs capital ");
            mvwprintw(Technologies, row_of_menu+retreat+1, RETREAT, " and give 2 stars per turn for each side. ");
            mvwprintw(Technologies, row_of_menu+retreat+2, RETREAT, " Also give 4 stars if between sides writen a Peace Treaty ");
            mvwprintw(Technologies, row_of_menu+retreat+3, RETREAT, " Gives capital vision --- all in process ");
            retreat += 4;
            break;


        //------------------------
        // Branch 4
        //------------------------


        case 22:
            wprintw(Technologies, " Unlock an ability to build mine, that ones gives 2 population ");
            break;
        case 23:
            wprintw(Technologies, " Unlock the unit Swordsman, that stronger than warrior ");
            mvwprintw(Technologies, row_of_menu+retreat, RETREAT, " Also unlock an ability to build forge ");
            mvwprintw(Technologies, row_of_menu+retreat, RETREAT, " Forge gives 2 population for every abjacent mine ");
            retreat += 2;
            break;
        case 24:
            wprintw(Technologies, " Unlock an ability to move to mountains ");
            break;
        case 25:
            wprintw(Technologies, " Unlock an ability to build a Mountain temples --- in process ");
            break;
        case 26:
            wprintw(Technologies, " Unlock the unit Mind Bender, that can heal yours units ---in process");
            break;


        //------------------------
        // Branch 5
        //------------------------


        case 27:
            wprintw(Technologies, " Unlock an ability to upgrade raft into Rammer - strong naval unit ");
            mvwprintw(Technologies, row_of_menu+retreat, RETREAT, " --- in process ");
            retreat++;
            break;
        case 28:
            wprintw(Technologies, " Unlock an ability to build Water temples --- in process ");
            mvwprintw(Technologies, row_of_menu+retreat, RETREAT, " Also give additional defence on water titels ");
            retreat++;
            break;
        case 29:
            wprintw(Technologies, " Unlock an ability to take fish as resources to grow up your city ");
            mvwprintw(Technologies, row_of_menu+retreat, RETREAT, " Also unlock building Port ");
            mvwprintw(Technologies, row_of_menu+retreat+1, RETREAT, " Port allows your units move on water and give 1 population ");
            mvwprintw(Technologies, row_of_menu+retreat+2, RETREAT, " Unlock the raft - common unit on water --- in process ");
            retreat += 3;
            break;
        case 30:
            wprintw(Technologies, " Unlock an ability to upgrade raft into Scout - range unit on water ");
            mvwprintw(Technologies, row_of_menu+retreat, RETREAT, "--- in process ");
            mvwprintw(Technologies, row_of_menu+retreat+1, RETREAT, " Also give an ability to sail on deep water --- in process ");
            retreat += 2;
            break;
        case 31:
            wprintw(Technologies, " Unlock the unit Bomber - very strong range naval unit, ");
            mvwprintw(Technologies, row_of_menu+retreat, RETREAT, "  Bomber attack enemies on splash --- in process ");
            mvwprintw(Technologies, row_of_menu+retreat+1, RETREAT, " Also gives an ability to take water stars as stars --- in process ");
            retreat += 2;
            break;

    }

    //--------------------------
    // Draw price
    //--------------------------


    mvwprintw(Technologies, row_of_menu+retreat+2, RETREAT, "Price: ");
    switch (tech) {

        //----------------------------
        // Branch 1
        //----------------------------


        case 7: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier2);
            break;
        case 8: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier3);
            break;
        case 9: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier1);
            break;
        case 10: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier2);
            break;
        case 11: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier3);
            break;


        //----------------------------
        // Branch 2
        //----------------------------


        case 12: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier2);
            break;
        case 13: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier3);
            break;
        case 14: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier1);
            break;
        case 15: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier2);
            break;
        case 16: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier3);
            break;


        //----------------------------
        // Branch 3
        //----------------------------


        case 17: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier2);
            break;
        case 18: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier3);
            break;
        case 19: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier1);
            break;
        case 20: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier2);
            break;
        case 21: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier3);
            break;


        //----------------------------
        // Branch 4
        //----------------------------


        case 22: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier2);
            break;
        case 23: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier3);
            break;
        case 24: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier1);
            break;
        case 25: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier2);
            break;
        case 26: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier3);
            break;


        //----------------------------
        // Branch 5
        //----------------------------


        case 27: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier2);
            break;
        case 28: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier3);
            break;
        case 29: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier1);
            break;
        case 30: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier2);
            break;
        case 31: 
            wprintw(Technologies, "%d ", CivS->Tech.Tier3);
            break;
    }
   wprintw(Technologies, "🌟");


    //--------------------------
    // Button Close and button Research
    //--------------------------
    
    
    mvwprintw(Technologies, row_of_menu+retreat+4, RETREAT, " Close ");
    mvwprintw(Technologies, row_of_menu+retreat+4, RETREAT+RETREAT, " Research ");
    XY->retreat = row_of_menu+retreat+4;
    

    wrefresh(Technologies);
}

int click_action(const int size, const int x, const int y, struct CXY* XY, struct Civs* CivS, WINDOW* Technologies) {

    //--------------------------------
    // Menu actions
    //--------------------------------


    if (x >= 110 && x <= 130 && y >= 5 && y <= 13) {
        if (XY->movement == y) {
            return 1; // movement
        } else if (XY->combat == y) {
            return 2; // combat
        } else if (XY->resources == y) {
            return 3; // resources
        } else if (XY->spawn == y) {
            return 4; // spawn
        } else if (XY->build == y) {
            return 5; // build
        } else if (XY->capture_tribe == y) {
            return 6; // capture tribe
        }
    }


    //--------------------------------
    // Tech tree
    //--------------------------------


    //--------------------------------
    // Show it
    //--------------------------------


    if (x>=8 && x<= 8+strlen(" Technologies ") && y == 2) {
        tech_tree(size, CivS, XY, Technologies);
        return 0;
    }

    //--------------------------------
    // Close it
    //--------------------------------


    if (x >= RETREAT && x <= RETREAT+strlen("Close Window") && y == row_of_close) {
        wclear(Technologies);
        mvwprintw(Technologies, 2, 8, " Technology");
        wrefresh(Technologies);
        return 0;
    }

    
    //--------------------------------
    // Tech Tree Actions
    //--------------------------------


    //--------------------------------
    // Show it
    //--------------------------------


    //--------------------------------
    // Branch 1
    //--------------------------------


    if (x >= XY->Archery && x<= XY->Archery+strlen("Archery ") && y == row_of_Tech_branch1-2) {
        technology(size, CivS, 7, XY, Technologies); // Archery
        return 0;
    } else if (x >= XY->Spiritualizm && x<= XY->Spiritualizm+strlen("Spiritualizm ") && y == row_of_Tech_branch1-2) {
        technology(size, CivS, 8, XY, Technologies); // Spiritualizm
        return 0;
    } else if (x >= XY->Hunting && x<= XY->Hunting+strlen("Hunting ") && y == row_of_Tech_branch1) {
        technology(size, CivS, 9, XY, Technologies); // Hunting
        return 0;
    } else if (x >= XY->Forestry && x<= XY->Forestry+strlen("Forestry ") && y == row_of_Tech_branch1+2) {
        technology(size, CivS, 10, XY, Technologies); // Forestry
        return 0;
    } else if (x >= XY->Mathematics && x<= XY->Mathematics+strlen("Mathematics ") && y == row_of_Tech_branch1+2) {
        technology(size, CivS, 11, XY, Technologies); // Mathematics
        return 0;
    } 


    //--------------------------------
    // Branch 2
    //--------------------------------

    
    if (x >= XY->Roads && x<= XY->Roads+strlen("Roads ") && y == row_of_Tech_branch2-2) {
        technology(size, CivS, 12, XY, Technologies); // Roads
        return 0;
    } else if (x >= XY->Trade && x<= XY->Trade+strlen("Trade ") && y == row_of_Tech_branch2-2) {
        technology(size, CivS, 13, XY, Technologies); // Trade
        return 0;
    } else if (x >= XY->Riding && x<= XY->Riding+strlen("Riding ") && y == row_of_Tech_branch2) {
        technology(size, CivS, 14, XY, Technologies); // Riding
        return 0;
    } else if (x >= XY->Free_Spirit && x<= XY->Free_Spirit+strlen("Free Spirit ") && y == row_of_Tech_branch2+2) {
        technology(size, CivS, 15, XY, Technologies); // Free Spirit
        return 0;
    } else if (x >= XY->Chivarly && x<= XY->Chivarly+strlen("Chivarly ") && y == row_of_Tech_branch2+2) {
        technology(size, CivS, 16, XY, Technologies); // Chivarly
        return 0;
    } 


    //--------------------------------
    // Branch 3
    //--------------------------------

    
    if (x >= XY->Farming && x<= XY->Farming+strlen("Farming ") && y == row_of_Tech_branch3-2) {
        technology(size, CivS, 17, XY, Technologies); // Farming
        return 0;
    } else if (x >= XY->Construction && x<= XY->Construction+strlen("Construction ") && y == row_of_Tech_branch3-2) {
        technology(size, CivS, 18, XY, Technologies); // Construction
        return 0;
    } else if (x >= XY->Organization && x<= XY->Organization+strlen("Organization ") && y == row_of_Tech_branch3) {
        technology(size, CivS, 19, XY, Technologies); // Organization
        return 0;
    } else if (x >= XY->Strategy && x<= XY->Strategy+strlen("Strategy ") && y == row_of_Tech_branch3+2) {
        technology(size, CivS, 20, XY, Technologies); // Strategy
        return 0;
    } else if (x >= XY->Diplomacy && x<= XY->Diplomacy+strlen("Diplomacy ") && y == row_of_Tech_branch3+2) {
        technology(size, CivS, 21, XY, Technologies); // Diplomacy
        return 0;
    } 


    //--------------------------------
    // Branch 4
    //--------------------------------

    
    if (x >= XY->Mining && x<= XY->Mining+strlen("Mining ") && y == row_of_Tech_branch4-2) {
        technology(size, CivS, 22, XY, Technologies); // Mining
        return 0;
    } else if (x >= XY->Smithery && x<= XY->Smithery+strlen("Smithery ") && y == row_of_Tech_branch4-2) {
        technology(size, CivS, 23, XY, Technologies); // Smithery
        return 0;
    } else if (x >= XY->Climbing && x<= XY->Climbing+strlen("Climbing ") && y == row_of_Tech_branch4) {
        technology(size, CivS, 24, XY, Technologies); // Climbing
        return 0;
    } else if (x >= XY->Meditation && x<= XY->Meditation+strlen("Meditation ") && y == row_of_Tech_branch4+2) {
        technology(size, CivS, 25, XY, Technologies); // Meditation
        return 0;
    } else if (x >= XY->Philosophy && x<= XY->Philosophy+strlen("Philosophy ") && y == row_of_Tech_branch4+2) {
        technology(size, CivS, 26, XY, Technologies); // Philosophy
        return 0;
    } 


    //--------------------------------
    // Branch 5
    //--------------------------------

    
    if (x >= XY->Ramming && x<= XY->Ramming+strlen("Ramming ") && y == row_of_Tech_branch5-2) {
        technology(size, CivS, 27, XY, Technologies); // Ramming
        return 0;
    } else if (x >= XY->Aquatism && x<= XY->Aquatism+strlen("Aquatism ") && y == row_of_Tech_branch5-2) {
        technology(size, CivS, 28, XY, Technologies); // Aquatism
        return 0;
    } else if (x >= XY->Fishing && x<= XY->Fishing+strlen("Fishing ") && y == row_of_Tech_branch5) {
        technology(size, CivS, 29, XY, Technologies); // Fishing
        return 0;
    } else if (x >= XY->Sailing && x<= XY->Sailing+strlen("Sailing ") && y == row_of_Tech_branch5+2) {
        technology(size, CivS, 30, XY, Technologies); // Sailing
        return 0;
    } else if (x >= XY->Navigation && x<= XY->Navigation+strlen("Navigation ") && y == row_of_Tech_branch5+2) {
        technology(size, CivS, 31, XY, Technologies); // Navigation
        return 0;
    } 


    //------------------------------------
    // Close window
    //------------------------------------


    if (x >= RETREAT && x <= RETREAT+strlen(" Close ") && y == XY->retreat) {
        wclear(Technologies);
        tech_tree(size, CivS, XY, Technologies);
        return 0;
    }

    //------------------------------------
    // Reseach
    //------------------------------------


    if (x >= RETREAT+RETREAT && x <= RETREAT+RETREAT+strlen(" Research ") && y == XY->retreat) {
        return 0;
    }


    return 0;
}

