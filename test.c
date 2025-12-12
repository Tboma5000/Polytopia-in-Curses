#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ncursesw/curses.h>
#include <locale.h>
#include <unistd.h>
#include <wchar.h>
#include <string.h>
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


enum Rows {
    // for function map()
    title_size = 9,
    additinal_retreat = 1,
    add_borders = 5,
    basic_territory = 9,

    // for function click_on_map()
    row_of_menu = 5,
    row_of_menu_actions = 10,

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

enum Const {

    //----------------------
    // Technologies cost
    //----------------------


    Tech_Tier1 = 5,
    Tech_Tier2 = 7,
    Tech_Tier3 = 10,


    //--------------------------
    // City Growth
    //--------------------------


    City_Growth_of_Tier2 = 2,
    City_Growth_of_Tier3 = 3,
    City_Growth_of_Tier4 = 4,
    City_Growth_of_Tier5 = 5,
    City_Growth_of_Tier6 = 6,
    City_Growth_of_Tier7 = 7,
    City_Growth_of_Tier8 = 8,
    City_Growth_of_Tier9 = 9,
    City_Growth_of_Tier10 = 10,
};

enum Buildings {
   Farm = 4,
   Lumber_cut = 5,
   Mine = 6,
   Port = 7,
   Sawmill = 8,
   Windmill = 9,
   Forge = 10,
   Market = 11,
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
   int combat; // can unit fight ot not
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
    int player; // it`s players CIV or AI
   int numb; // number of civ
   int stars; // how much have
   int score;  
   int income;
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


    //---------------------
    // Movement 
    //---------------------
    int sizeof_Moves;
    int** Moves;

    
    //---------------------
    // Combat
    //---------------------

    int sizeof_Combat;
    int** Combats;
};

// list of units to hit
struct PCombat {
    struct Units Unit;
    struct PCombat *next;
    struct PCombat *prev;
};

// list of tribes and their information
struct TribeS {
    int turn;
    bool Capture;
    int x;
    int y;
    struct TribeS *next;
    struct TribeS *prev;
};

// Important information for game
struct GAME {
    int turn;
    struct TribeS* Tribes;
};


//---------------------------------------------
// Declaration of functions
//---------------------------------------------

void print_world(int size, int** world);
void print_res(int size, int** resources);
void print_civs(struct Civs* CivS);
void print_tribes(struct TribeS* Tribes);


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
    int** world = (int**)malloc((size_t)size * sizeof(int*)); // array of territory type
    int** resources = (int**)malloc((size_t)size * sizeof(int*)); // array of resources on territory
    for (int i=0; i<size; i++) {
        world[i] = (int*)malloc((size_t)size * sizeof(int));
        resources[i] = (int*)malloc((size_t)size * sizeof(int));
    }
    struct Civs *CivS = NULL;

    struct ClicK* Click = malloc(sizeof(struct ClicK));
    memset(Click, 0, sizeof(struct ClicK));

    struct CXY* XY = malloc(sizeof(struct CXY));
    memset(XY, 0, sizeof(struct CXY));
    
    struct GAME* Game = malloc(sizeof(struct GAME));
    memset(Game, 0, sizeof(struct GAME));
    Game->turn = 1;


//---------------------------------------------------------
// Functions
//---------------------------------------------------------

    world_generation(size, world, CIV);
    print_world(size, world);
    printf("\n");

    //resources_generation(size, resources, world);
    //print_res(size, resources);
    //printf("\n");

    civs_creation(size, world, &CivS, CIV);
    //print_civs(CivS);
    //printf("\n");

    tribes_write(size, world, &Game->Tribes);
    print_tribes(Game->Tribes);
    printf("\n");

    //sleep(5);

    check_tribe(CivS, Game);

    

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

void print_res(int size, int** resources) {
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            printf("%d ", resources[i][j]);
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

void print_tribes(struct TribeS* Tribes) {
    struct TribeS* Tribe = Tribes;
    printf("\n Tribes: \n");
    while (Tribe != NULL) {
        printf("X: %d, Y: %d, TURN: %d, CAPTURE: %d\n", Tribe->x, Tribe->y, Tribe->turn, Tribe->Capture);
        Tribe = Tribe->next;
    }
}