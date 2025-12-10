#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ncursesw/curses.h>
#include <locale.h>
#include <unistd.h>
#include <wchar.h>
#include "mechanics.h"
#include "gui.h"

#define CIV 2

// Unit type:
// 1 - Warrior
// 2 - Raider
// 3 - Archer
// 4 - Giant
// 5 - Defender
// 6 - Catapult
// 7 - Knight


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

    int tech_screen;

    // Y
    int movement;
    int combat;
    int resources;
    int spawn;
    int build;
    int capture_tribe;

    int Technology;

    // X
    //Technology
    //branch 1
    int Hunting;
    int Archery;
    int Spiritualizm;
    int Forestry;
    int Mathematics;
    //branch 2
    int Riding;
    int Roads;
    int Trade;
    int Free_Spirit;
    int Chivarly;
    //branch 3
    int Organization;
    int Farming;
    int Construction;
    int Strategy;
    int Diplomacy;
    //branch 4
    int Climbing;
    int Mining;
    int Smithery;
    int Meditation;
    int Philosophy;
    //branch 5
    int Fishing;
    int Ramming;
    int Aquatism;
    int Sailing;
    int Navigation;


    //------------------------------------
    // Tech
    //------------------------------------


    int retreat;


    //---------------------
    // Movement 
    //---------------------


    int** Moves;
};

// list of units to hit
struct PCombat {
    struct Units Unit;
    struct PCombat *next;
    struct PCombat *prev;
};


//---------------------------------------------
// Declaration of functions
//---------------------------------------------

void print_world(int size, int** world);
void print_res(int size, float** resources);
void print_civs(struct Civs* CivS);


//---------------------------------------------------------
//Functions
//---------------------------------------------------------


int main() {
//---------------------------------------------------------
// Rules
//---------------------------------------------------------


    setlocale(LC_ALL, "en_US.UTF-8");
    srand((unsigned int)time(NULL));


//---------------------------------------------------------
// Variables
// --------------------------------------------------------



    int size = 10;
    size_t sz = (size_t)size;
    int** world = (int**)malloc(sz * sizeof(int*)); // array of territory type
    float** resources = (float**)malloc(sz * sizeof(float*)); // array of resources on territory
    for (int i=0; i<size; i++) {
        world[i] = (int*)malloc(sz * sizeof(int));
        resources[i] = (float*)malloc(sz * sizeof(float));
    }
    struct Civs *CivS = NULL;
    struct ClicK* Click = malloc(sizeof(struct ClicK));
    struct CXY* XY = malloc(sizeof(struct CXY));
    

//---------------------------------------------------------
// Functions
//---------------------------------------------------------

    world_generation(size, world, CIV);
    //print_world(size, world);

    //printf("\n");

    resources_generation(size, resources, world);
    //print_res(size, resources);

    //printf("\n");

    civs_creation(size, world, &CivS, CIV);
    //print_civs(CivS);

    //printf("\n");

    //sleep(5);

    initscr();
    start_color(); 
    clear();
    cbreak();
    noecho();
    keypad(stdscr, 1);
    curs_set(0);

    mousemask(ALL_MOUSE_EVENTS, NULL);

    MEVENT event;

    WINDOW* Technologies = newwin(height, width, 0, size*size+1);
    map(size, world, resources, CivS, Technologies);

    while (1) {
        int ch = getch();

        if (ch == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                mvprintw(43, 10, "CLICK: x=%d y=%d                      ", event.x, event.y);
                if (event.x >= 10 && event.x <= 100 && event.y >= 1 && event.y <= 41) {
                    convertor(size, event.x, event.y, Click);
                    mvprintw(44, 10, "CONVERT: x=%d y=%d                         ", Click->map_x, Click->map_y);
                    refresh();
                    click_on_map(size, world, resources, CivS, Click->map_x, Click->map_y, XY);
                    map(size, world, resources, CivS, Technologies);
                } else {
                    Click->action_x = event.x -101;
                    Click->action_y = event.y -0;
                    mvprintw(44, 10, "OUT OF MAP: x=%d y=%d                         ", Click->action_x, Click->action_y);
                    int chose = click_action(size, Click->action_x, Click->action_y, XY, CivS, Technologies, &event, Click);
                    switch (chose) {
                        
                        //-----------------------------------------
                        // Menu actions
                        //-----------------------------------------


                        case 1: 
                            //int DX = Click->map_x;
                            //int DY = Click->map_y;
                            map(size, world, resources, CivS, Technologies);
                            /*
                            while(1) {
                                int h = getch();
                                if (h == KEY_MOUSE) {
                                    if (getmouse(&event) == OK) {
                                        convertor(size, event.x, event.y, Click);
                                        int move = Movement(DX, DY, Click->map_x, Click->map_y, CivS, XY->Moves);
                                        if (move == 0) {
                                            mvprintw(0, 10, " False Position! ");
                                        }
                                        map(size, world, resources, CivS, Technologies);
                                        break;
                                    }
                                }
                            }
                            */
                            break;

                        case 2: mvprintw(20, 110, "Combat was chosen!                                "); break;
                        case 3: mvprintw(20, 110, "Grab Resources was chosen!                                "); break;
                        case 4: mvprintw(20, 110, "Spawn unit was chosen!                                "); break;
                        case 5: mvprintw(20, 110, "Buiding was chosen!                                "); break;
                        case 6: mvprintw(20, 110, "Capture tribe was chosen!                                "); break;

                    }

                }
            }
        }
    }

    getchar();
    endwin();

    return 0;
}


//-------------------------------------------------------
// Initialisation of functions
//-------------------------------------------------------


void print_world(int size, int** world) {
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            printf("%d ", world[i][j]);
        }
        printf("\n");
    }
}

void print_res(int size, float** resources) {
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            printf("%.1f ", resources[i][j]);
        }
        printf("\n");
    }
}

void print_civs(struct Civs* CivS) {
    struct Civs *n = CivS;
    while(n) {
        printf("CIV numb %d, score %d, stars: %d\n", n->numb, n->score, n->stars);
        printf("Territory: ");
        for (int i=0; i<9; i++) {
            printf("[%d, %d]", n->territory[i][0], n->territory[i][1]);
        }
        printf("\n");
        struct City *s = n->Cities;
        while(s) {
            printf("City numb: %d, x: %d, y: %d ", s->numb, s->x, s->y);
            printf("Growth: %d, Population: %d, Production: %d\n", s->growth, s->population, s->production);
            printf("Territory: ");
            for (int i=0; i<9; i++) {
                printf("[%d, %d]", s->territory[i][0], s->territory[i][1]);
            }
            printf("\n");
            s = s->next;
        }
        struct Units *d = n->Unites;
        while(d) {
            printf("Unit numb: %d, name: %s, type: %d, x: %d, y: %d, hp: %d, dmg: %d, def: %d, move: %d, range: %d\n", d->numb, d->stat.name, d->type, d->x, d->y, d->stat.hp, d->stat.dmg, d->stat.def, d->stat.movement, d->stat.range);
            d = d->next;
        }
        n = n->next;
    }
}