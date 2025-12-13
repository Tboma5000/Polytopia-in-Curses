#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include <ncursesw/curses.h>

#define OCEAN 2
#define FOREST 20
#define MOUNTAIN 20
#define TRIBE 7
#define RESOURCE 66

#define BASIC_TERRITORY 9

//------------------
// Textures
//------------------
// 0 - sea +-
// 1 - land +
// 2 - forest +
// 3 - mountain +
// 4 - tribe +-
// 5 - city +-


// Unit type:
// 1 - Warrior
// 2 - Raider
// 3 - Archer
// 4 - Giant
// 5 - Defender
// 6 - Catapult
// 7 - Knight
// 8 - Swordsman


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
   int siege_turn;
   bool Capture_City;
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
   int countCity;
   int countCity_prev;
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
    bool capture_city;
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
    int capture_city;

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


    //---------------------
    // Spawn
    //---------------------

    int spawn_Warrior;
    int spawn_Archer;
    int spawn_Catapult;
    int spawn_Rider;
    int spawn_Defender;
    int spawn_Knight;
    int spawn_Swordsman;
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


//-------------------------------------------
// Functions
//-------------------------------------------


//-------------------------------------------
// World
//-------------------------------------------


int check_for_distance(const int size, int** world, int x, int y, int n) {
    for (int dx = -2; dx <= 2; dx++) {
         for (int dy = -2; dy <= 2; dy++) {
            int nx = x + dx;
            int ny = y + dy;

            if (nx < 0 || nx >= size || ny < 0 || ny >= size) 
                continue;

            if (world[nx][ny] == n && nx != x && ny != y) {
               world[nx][ny] = 1;
               return 1;
            }
      }
   }
   return 0;
}

void world_generation(const int size, int**  world, int CIVS) {
   // Fill world by land
   for (int i=0; i<size; i++) {
      for (int j=0; j<size; j++) {
         world[i][j] = 1;
      }
   }

   // Create oceans - need corection!
   int ocean = rand() % OCEAN +2; // count of oceans
   for (int i=0; i<ocean; i++) {
      int rand_water = rand() % ((size*size) * 20 / 100) +10; // count of water titels in ocean
      //coordinates to spawn an ocean
      int rand_x, rand_y = 0;
      do {
         rand_x = rand() % size;
         rand_y = rand() % size;
      } while (world[rand_x][rand_y] == 0);
      world[rand_x][rand_y] = 0;

      int move_x, move_y = 0;
      for (int c = 0; c < rand_water; c++) {
         int span = (rand() % 2) ? 1 : -1;
         int orr  = rand() % 3;

         move_x = rand_x;
         move_y = rand_y;

         while (1) {
            if (orr == 0) {
               move_x += span;
            } else if (orr == 1) {
               move_y += span;
            } else {
               move_x += span;
               move_y += span;
            }

            if (move_x < 0 || move_x >= size || move_y < 0 || move_y >= size) {
               break;
            }

            if (world[move_x][move_y] != 0) {
               world[move_x][move_y] = 0;
               break;
            }  
         }
      }
   }

   // Create forests
   int forest = rand() % ((size*size) * FOREST / 100) +1; // forest take 20% of all land`
   for (int i=0; i<forest; i++) {
      int rand_forest = rand() % 4 +1; // titels of forest in one forest
      
      int rand_x, rand_y = 0; // Coordinates of start forest
      do {
         rand_x = rand() % size;
         rand_y = rand() % size;
      } while (world[rand_x][rand_y] != 1);

      int move_x, move_y = 0;
      for (int c = 0; c < rand_forest; c++) {
         int span = (rand() % 2) ? 1 : -1;
         int orr  = rand() % 3;

         move_x = rand_x;
         move_y = rand_y;

         while (1) {
            if (orr == 0) {
               move_x += span;
            } else if (orr == 1) {
               move_y += span;
            } else {
               move_x += span;
               move_y += span;
            }

            if (move_x < 0 || move_x >= size || move_y < 0 || move_y >= size) {
               break;
            }

            if (world[move_x][move_y] == 1) {
               world[move_x][move_y] = 2;
               break;
            }  
         }
      }
   }

   // Create mountains
   int mountains = rand() % ((size * size) * MOUNTAIN / 100) +1; // mountains takes 20% of land
   for (int i=0; i<mountains; i++) {
      int rand_mountain = rand() % 3 +1; // titels of mountains

      int rand_x, rand_y = 0;
      do {
         rand_x = rand() % size;
         rand_y = rand() % size;
      } while (world[rand_x][rand_y] != 1);

      int move_x, move_y = 0;
      for (int c=0; c<rand_mountain; c++) {
         int span = (rand() % 2) ? 1 : -1;
         int orr  = rand() % 3;

         move_x = rand_x;
         move_y = rand_y;

         while (1) {
            if (orr == 0) {
               move_x += span;
            } else if (orr == 1) {
               move_y += span;
            } else {
               move_x += span;
               move_y += span;
            }

            if (move_x < 0 || move_x >= size || move_y < 0 || move_y >= size) {
               break;
            }

            if (world[move_x][move_y] == 1) {
               world[move_x][move_y] = 3;
               break;
            }  
         }
      }
   }
   
   // Create tribes - need corection!
   int tribe = rand() % ((size * size) * TRIBE / 100) +2*CIVS;
   for (int i=0; i<tribe; i++) {
      int rand_x, rand_y, check = 0;
      do {
         rand_x = rand() % size;
         rand_y = rand() % size;
      } while (world[rand_x][rand_y] != 1);

      int move_x, move_y = 0;
      move_x = rand_x;
      move_y = rand_y;

      if (world[move_x][move_y] == 1) {
         world[move_x][move_y] = 4;
      }

      check = check_for_distance(size, world, move_x, move_y, 4);
      if (check != 0) {
         i++;
      }
   }

   // Creation of Civilization
   for (int i=0; i<CIVS; i++) {
      int rand_x, rand_y, check = 0;
      do {
         rand_x = rand() %size +i;
         rand_y = rand() %size +i;
      } while (!(rand_x > 0 && rand_x < size && rand_y > 0 && rand_y < size));
      if (rand_x == 0) {
         rand_x++;
      }
      if (rand_y == 0) {
         rand_y++;
      } 
      if (rand_x == size-1) {
         rand_x--;
      }
      if (rand_y == size-1) {
         rand_y--;
      }
      world[rand_x][rand_y] = 5;
      check = check_for_distance(size, world, rand_x, rand_y, 5);
      if (check == 1) {
         i--;
      }
   }
}

void resources_generation(const int size, int** resourses, int** world) {
   // Set 0;
   for (int i=0; i<size; i++) {
      for (int j=0; j<size; j++) {
         resourses[i][j] = 0;
      }
   }

   int resources_rand = rand() % ((size*size) * RESOURCE / 100) +RESOURCE*2;
   for (int i=0; i<resources_rand; i++) {
      int rand_x, rand_y = 0;
      int type = 0;
      do {
         rand_x = rand() % size;
         rand_y = rand() % size;
      } while (world[rand_x][rand_y] == 4 || world[rand_x][rand_y] == 5);

      do {
         type = rand() % 3;
         if (type == 0) {
            type = 1;
         } else if (type == 1) {
            type = 2;
         } else if (type == 2) {
            type = 3;
         }
      } while (type == 3 && world[rand_x][rand_y] != 2);
      
      resourses[rand_x][rand_y] = type;
   }
}


//------------------------------------------
// Structs
//------------------------------------------

void clear_arr(int*** ARR, int size) {
   if (!ARR || !*ARR) return;

   for (int i=0; i<size; i++) {
      free((*ARR)[i]);
      (*ARR)[i] = NULL;
   }
   free(*ARR);
   *ARR = NULL;
}

int ReturnNumbOfCiv(struct Civs* CivS, const int x, const int y) {
   struct Civs* CV = CivS;
   while (CV != NULL) {
      for (int i=0; i<CV->countCity*BASIC_TERRITORY; i++) {
         if (CV->territory[i][0] == x && CV->territory[i][1] == y) {
            return CV->numb;
         }
      }
      CV = CV->next;
   }
   return -1;
}

struct Civs* ReturnCiv(struct Civs* CivS, int number) {
   struct Civs* CV = CivS;
   while (CV != NULL) {
      if (CV->numb == number) {
         return CV;
      }
      CV = CV->next;
   }
   return NULL;
}

struct City* ReturnCity(struct Civs* CivS, const int x, const int y) {
   struct Civs* CIV = CivS;
   while (CIV != NULL) {
      struct City* CITY = CIV->Cities;
      while (CITY != NULL) {
         for (int i=0; i<basic_territory; i++) {
            if (CITY->territory[i] != NULL) {
               if (CITY->territory[i][0] == x && CITY->territory[i][1] == y) {
                  return CITY;
               }
            }
         }
         CITY = CITY->next;
      }
      CIV = CIV->next;
   }
   return NULL;
   /*
   struct City* CitY = Cit;
   while (CitY != NULL) {
      for (int i=0; i<basic_territory; i++) {
         if (CitY->territory[i][0] == x && CitY->territory[i][1] == y) {
            return CitY;
         }
      }
      CitY = CitY->next;
   }
   return NULL;
   */
}

struct Units* ReturnUnit(struct Civs* CivS, const int x, const int y) {
   struct Civs* CIV = CivS;
   while (CIV != NULL) {
      struct Units* Unit = CIV->Unites;
      while (Unit != NULL) {
         if (Unit->x == x && Unit->y == y) {
            return Unit;
         }
      Unit = Unit->next;
      }
      CIV = CIV->next;
   }
   
   return NULL;
}

// Add territory to City, compare with territory other CivS and expand territory of CIV
void Expansion(struct Civs* CivS, struct Civs* CIV, struct City* CITY, const int size, const int x, const int y) {
   if (CITY == NULL || CivS == NULL || CIV == NULL) return;
   CITY->territory = malloc(BASIC_TERRITORY * sizeof(int*));
   int pos = 0;
   for (int i=0; i<BASIC_TERRITORY; i++) {
      CITY->territory[i] = malloc(2 * sizeof(int));
   }
   for (int dx=x-1; dx<=x+1; dx++) {
      for (int dy=y-1; dy<=y+1; dy++) {
         if (pos > BASIC_TERRITORY) break;

         if (dx < 0 || dx >= size || dy < 0 || dy >= size) {
            CITY->territory[pos][0] = -1;
            CITY->territory[pos][1] = -1;
            pos++;
            continue;
         }
         CITY->territory[pos][0] = dx;
         CITY->territory[pos][1] = dy;
         pos++;
      }
   }

   struct Civs* CIVI = CivS;
   while (CIVI != NULL) {
      for (int i=0; i<BASIC_TERRITORY; i++) {
         for (int j=0; j<CIVI->countCity*BASIC_TERRITORY; j++) {
            if (CITY->territory[i][0] == CIVI->territory[j][0] && CITY->territory[i][1] == CIVI->territory[i][1]) {
               CITY->territory[i][0] = -1;
               CITY->territory[i][1] = -1;
            }
         }
      }
      CIVI = CIVI->next;
   }

   

   int oldCountCity = CIV->countCity;
   CIV->countCity += 1;
   CIV->countCity_prev += 1;

   int totalTerritore = CIV->countCity * BASIC_TERRITORY;

   int** tmp = realloc(CIV->territory, (size_t)totalTerritore * sizeof(int*));
   if (!tmp) return;

   CIV->territory = tmp;

   int start = oldCountCity * BASIC_TERRITORY;

   for (int i=start; i<totalTerritore; i++) {
      CIV->territory[i] = malloc(2 * sizeof(int));
   }

   for (int i=0; i<BASIC_TERRITORY; i++) {
      if (CIV->territory[start+i] != NULL) {
         CIV->territory[start+i][0] = CITY->territory[i][0];
         CIV->territory[start+i][1] = CITY->territory[i][1];
      }
   }
}

void new_Unit(struct Units** head, int number, int x, int y, int type) {
   struct Units* newUnit = malloc(sizeof(struct Units));
   if (!newUnit) return;
   memset(newUnit, 0, sizeof(struct Units));

   newUnit->x = x;
   newUnit->y = y;
   newUnit->numb = number;
   newUnit->type = type;
   newUnit->combat = 1;
   newUnit->next = NULL;
   newUnit->prev = NULL;
   if (type == 1) {
      struct Charakteristics Warrior;
      //Warrior.name = malloc(8);
      //Warrior.name = "Warrior";
      Warrior.cost = 2;
      Warrior.hp = 10;
      Warrior.dmg = 3;
      Warrior.def = 2;
      Warrior.movement = 2;
      Warrior.range = 1;
      newUnit->stat = Warrior;
   } else if (type == 2) {
      struct Charakteristics Rider;
      //Rider.name = malloc(6);
      //Rider.name = "Rider";
      Rider.cost = 3;
      Rider.hp = 10;
      Rider.dmg = 3;
      Rider.def = 1;
      Rider.movement = 2;
      Rider.range = 1;
      newUnit->stat = Rider;
   } else if (type == 3) {
      struct Charakteristics Archer;
      //Archer.name = malloc(7);
      //Archer.name = "Archer";
      Archer.cost = 3;
      Archer.hp = 10;
      Archer.dmg = 3;
      Archer.def = 1;
      Archer.movement = 1;
      Archer.range = 2;
      newUnit->stat = Archer;
   } else if (type == 4) {
      struct Charakteristics Giant;
      //Giant.name = malloc(6);
      //Giant.name = "Giant";
      Giant.cost = 0;
      Giant.hp = 40;
      Giant.dmg = 7;
      Giant.def = 4;
      Giant.movement = 1;
      Giant.range = 1;
      newUnit->stat = Giant;
   } else if (type == 5) {
      struct Charakteristics Defender;
      //Defender.name = malloc(9);
      //Defender.name = "Defender";
      Defender.cost = 3;
      Defender.hp = 15;
      Defender.dmg = 1;
      Defender.def = 3;
      Defender.movement = 1;
      Defender.range = 1;
      newUnit->stat = Defender;
   } else if (type == 6) {
      struct Charakteristics Catapult;
      //Catapult.name = malloc(9);
      //Catapult.name = "Catapult";
      Catapult.cost = 8;
      Catapult.hp = 10;
      Catapult.dmg = 7;
      Catapult.def = 0;
      Catapult.movement = 1;
      Catapult.range = 3;
      newUnit->stat = Catapult;
   } else if (type == 7) {
      struct Charakteristics Knight;
      //Knight.name = malloc(7);
      //Knight.name = "Knight";
      Knight.cost = 8;
      Knight.hp = 10;
      Knight.dmg = 6; // 3.5
      Knight.def = 1;
      Knight.movement = 3;
      Knight.range = 1;
      newUnit->stat = Knight;
   } else if (type == 8) {
      struct Charakteristics Swordsman;
      //Swordsman.name = malloc(10);
      //Swordsman.name = "Swordsman";
      Swordsman.cost = 5;
      Swordsman.hp = 15;
      Swordsman.dmg = 7;
      Swordsman.def = 3;
      Swordsman.movement = 1;
      Swordsman.range = 1;
      newUnit->stat = Swordsman;
   }


   if (*head == NULL) {
      *head = newUnit;
      return;
   }

   struct Units* temp = *head;
   while (temp->next != NULL) {
      temp = temp->next;
   }

   temp->next = newUnit;
   newUnit->prev = temp;
}

// Delete Unit
void delete_Unit(struct Civs* CivS, const int x, const int y) {
   if (CivS == NULL) return;

   struct Units* Unit = ReturnUnit(CivS, x, y);
   if (Unit == NULL) return;

   struct Civs* CIV = ReturnCiv(CivS, Unit->numb);
   if (CIV == NULL) return;

   if (Unit->prev) {
      Unit->prev->next = Unit->next;
   } else {
      CIV->Unites = Unit->next;
   }

   if (Unit->next) {
      Unit->next->prev = Unit->prev;
   }

   free(Unit);
}

void new_City(struct Civs* CivS, struct Civs* CIV, struct City** head, int number, int x, int y, int size) {
   struct City* newCity = malloc(sizeof(struct City));
   if (!newCity) return;
   memset(newCity, 0, sizeof(struct City));

   newCity->numb = number;
   newCity->x = x;
   newCity->y = y;
   newCity->growth = 1;
   newCity->population = 0;
   newCity->production = 1;
   CIV->income += newCity->production;
   Expansion(CivS, CIV, newCity, size, x, y);

   newCity->next = NULL;
   newCity->prev = NULL;

   if (*head == NULL) {
      *head = newCity;
      return;
   }

   struct City* temp = *head;
   while (temp->next != NULL) {
      temp = temp->next;
   }

   temp->next = newCity;
   newCity->prev = temp;
}

void new_Civ(struct Civs** head, int number, int x, int y, int size) {
   struct Civs* newCiv = malloc(sizeof(struct Civs));
   if(!newCiv) return;
   memset(newCiv, 0, sizeof(struct Civs));

   newCiv->numb = number;
   newCiv->next = NULL;
   newCiv->prev = NULL;
   newCiv->Cities = NULL;
   newCiv->Unites = NULL;
   newCiv->score = 0;
   newCiv->stars = 5;
   newCiv->countCity = 0;
   newCiv->countCity_prev = 0;


   //---------------------------------------------
   // Technologies
   //---------------------------------------------

   newCiv->Tech.branch4.Climbing = true;
   newCiv->Tech.branch1.Archery = true;
   newCiv->Tech.branch1.Mathematics = true;
   newCiv->Tech.branch2.Riding = true;
   newCiv->Tech.branch2.Chivalry = true;
   newCiv->Tech.branch3.Strategy = true;
   newCiv->Tech.branch4.Smithery = true;

   /*
   newCiv->Tech.Tier1 = 5;
   newCiv->Tech.Tier2 = 7;
   newCiv->Tech.Tier3 = 10;
   */

   
   //---------------------------------------------
   // Add new element in the end of list
   //---------------------------------------------


   if (*head == NULL) {
      *head = newCiv;
   } else {
      struct Civs* temp = *head;
      while (temp->next != NULL) temp = temp->next;
      temp->next = newCiv;
      newCiv->prev = temp;
   }

   // Add capital to CIV and basic Unit
   struct Civs* CivS = *head;
   new_City(CivS, newCiv, &newCiv->Cities, number, x, y, size);
   new_Unit(&newCiv->Unites, number, x, y, 1);
}

void civs_creation(const int size, int** world, struct Civs** CivS, int CIV) {
   int number = 1;
   for (int x=0; x<size; x++) {
      for (int y=0; y<size; y++) {
         if (world[x][y] == 5) {
            new_Civ(CivS, number, x, y, size);
            struct Civs* temp = *CivS;
            while(temp) {
               temp->Cities->production = 2;
               temp->income = temp->Cities->production;
               temp = temp->next;
            }
            number++;

            if (number >= CIV) {
               break;
            }
         }
      }
   }
}

// add all Cities territory to territory CIV
void Territory(struct Civs* CivS) {
   if (CivS == NULL) return;

   struct Civs* CIV = CivS;
   while (CIV != NULL) {
      int sizeofTerritory = CIV->countCity*basic_territory;
      int pos = 0;

      clear_arr(&CIV->territory, CIV->countCity_prev*basic_territory);

      //int** territory = malloc((size_t)sizeofTerritory * sizeof(int*));
      //if (!territory) return;

      CIV->territory = malloc((size_t)sizeofTerritory * sizeof(int*)); //territory;
      if (!CIV->territory) return;
      
      for (int i=0; i<sizeofTerritory; i++) {
         CIV->territory[i] = calloc(2, sizeof(int));
         if (!CIV->territory[i]) {
            for (int j=0; j<i; j++) {
               free(CIV->territory[j]);
            }
            free(CIV->territory);
            CIV->territory = NULL;
            return;
         }
      }

      struct City* CITY = CIV->Cities;
      while (CITY != NULL) {
         for (int j=0; j<basic_territory; j++) {
            if (CITY->territory[j] != NULL) {
               CIV->territory[pos][0] = CITY->territory[j][0];
               CIV->territory[pos][1] = CITY->territory[j][1];
               pos++;
            }
         }
         CITY = CITY->next;
      }
      CIV = CIV->next;
   }
}

//------------------------
// Tribes
//------------------------


void tribes_write(const int size, int** world, struct TribeS** head) {
   for (int x=0; x<size; x++) {
      for (int y=0; y<size; y++) {
         if (world[x][y] == 4) {
            struct TribeS* newTribe = malloc(sizeof(struct TribeS));
            if (!newTribe) return;
            memset(newTribe, 0, sizeof(struct TribeS));

            newTribe->x = x;
            newTribe->y = y;
            newTribe->turn = 0;
            newTribe->next = NULL;
            newTribe->prev = NULL;

            if (*head == NULL) {
               *head = newTribe;
            } else {
               struct TribeS* tmp = *head;
               while (tmp->next != NULL) tmp = tmp->next;
               tmp->next = newTribe;
               newTribe->prev = tmp;
            }
         }
      }
   }
}

// Check: does any unit on tribe
void check_tribe(struct Civs* CivS, struct GAME* Game, const int x, const int y) {
   struct TribeS* T = Game->Tribes;
   while (T != NULL) {
      if (T->turn != 0) {
         T->turn = 0;
      }
      T = T->next;
   }

   struct Units* Unit = ReturnUnit(CivS, x, y);
   if (Unit == NULL) return;

   struct TribeS* Tribe = Game->Tribes;
   if (Tribe == NULL) return;

   while (Tribe != NULL) {
      if (Unit->x == Tribe->x && Unit->y == Tribe->y) {
         Tribe->turn = Game->turn;
      }
      Tribe = Tribe->next;
   }
}

void remove_tribe(struct GAME* Game, const int x, const int y) {
   struct TribeS* Tribe = Game->Tribes;

   while (Tribe != NULL) {
      if (Tribe->x == x && Tribe->y == y) {
         break;
      }
      Tribe = Tribe->next;
   }
   if (Tribe == NULL) return;

   if (Tribe->prev) {
      Tribe->prev->next = Tribe->next;
   } else {
      Game->Tribes = Tribe->next;
   }

   if (Tribe->next) {
      Tribe->next->prev = Tribe->prev;
   }

   free(Tribe);
   
}

// Delete tribe from map and add this as zero City to Civ, which unit is here
void Capture_tribe(const int size, const int x, const int y, int** world, struct Civs* CivS, struct GAME* Game) {
   if (world[x][y] != 4) return;

   struct Units* Unit = ReturnUnit(CivS, x, y);
   if (Unit == NULL) return;

   if (Unit->stat.movement == 0) return;

   struct Civs* CIV = ReturnCiv(CivS, Unit->numb);
   if (CIV == NULL) return;

   world[x][y] = 5;
   //Unit->stat.movement = 0;
   new_City(CivS, CIV, &CIV->Cities, Unit->numb, x, y, size);

   remove_tribe(Game, x, y);
}

//-----------------------------------------------
// Research
//-----------------------------------------------


void Research(struct Civs* CivS, int tech) {
   switch(tech) {

      //------------------------------------------------
      // Branch 1
      //------------------------------------------------


      case 7: 
         if (CivS->Tech.branch1.Archery == false && CivS->Tech.branch1.Hunting == true && CivS->stars >= CivS->Tech.Tier2) {
             CivS->Tech.branch1.Archery = true;
             CivS->stars -= CivS->Tech.Tier2;
         }
         break;
      case 8: 
         if (CivS->Tech.branch1.Spiritualizm == false && CivS->Tech.branch1.Archery == true && CivS->stars >= CivS->Tech.Tier3) {
             CivS->Tech.branch1.Spiritualizm = true;
             CivS->stars -= CivS->Tech.Tier3;
         }
         break;
      case 9: 
         if (CivS->Tech.branch1.Hunting == false && CivS->stars >= CivS->Tech.Tier1) {
             CivS->Tech.branch1.Hunting = true;
             CivS->stars -= CivS->Tech.Tier1;
         }
         break;
      case 10: 
         if (CivS->Tech.branch1.Forestry == false && CivS->Tech.branch1.Hunting == true && CivS->stars >= CivS->Tech.Tier2) {
             CivS->Tech.branch1.Forestry = true;
             CivS->stars -= CivS->Tech.Tier2;
         }
         break;
      case 11: 
         if (CivS->Tech.branch1.Mathematics == false && CivS->Tech.branch1.Forestry == true && CivS->stars >= CivS->Tech.Tier3) {
             CivS->Tech.branch1.Mathematics = true;
             CivS->stars -= CivS->Tech.Tier3;
         }
         break;


      //------------------------------------------------
      // Branch 2
      //------------------------------------------------


      case 12: 
         if (CivS->Tech.branch2.Roads == false && CivS->Tech.branch2.Riding == true && CivS->stars >= CivS->Tech.Tier2) {
             CivS->Tech.branch2.Roads = true;
             CivS->stars -= CivS->Tech.Tier2;
         }
         break;
      case 13: 
         if (CivS->Tech.branch2.Trade == false && CivS->Tech.branch2.Roads == true && CivS->stars >= CivS->Tech.Tier3) {
             CivS->Tech.branch2.Trade = true;
             CivS->stars -= CivS->Tech.Tier3;
         }
         break;
      case 14: 
         if (CivS->Tech.branch2.Riding == false && CivS->stars >= CivS->Tech.Tier1) {
             CivS->Tech.branch2.Riding = true;
             CivS->stars -= CivS->Tech.Tier1;
         }
         break;
      case 15: 
         if (CivS->Tech.branch2.Free_Spirit == false && CivS->Tech.branch2.Riding == true && CivS->stars >= CivS->Tech.Tier2) {
             CivS->Tech.branch2.Free_Spirit = true;
             CivS->stars -= CivS->Tech.Tier2;
         }
         break;
      case 16: 
         if (CivS->Tech.branch2.Chivalry == false && CivS->Tech.branch2.Free_Spirit == true && CivS->stars >= CivS->Tech.Tier3) {
             CivS->Tech.branch2.Chivalry = true;
             CivS->stars -= CivS->Tech.Tier3;
         }
         break;


      //------------------------------------------------
      // Branch 3
      //------------------------------------------------


      case 17: 
         if (CivS->Tech.branch3.Farming == false && CivS->Tech.branch3.Organization && CivS->stars >= CivS->Tech.Tier2) {
             CivS->Tech.branch3.Farming = true;
             CivS->stars -= CivS->Tech.Tier2;
         }
         break;
      case 18: 
         if (CivS->Tech.branch3.Construction == false && CivS->Tech.branch3.Farming && CivS->stars >= CivS->Tech.Tier3) {
             CivS->Tech.branch3.Construction = true;
             CivS->stars -= CivS->Tech.Tier3;
         }
         break;
      case 19: 
         if (CivS->Tech.branch3.Organization == false && CivS->stars >= CivS->Tech.Tier1) {
             CivS->Tech.branch3.Organization = true;
             CivS->stars -= CivS->Tech.Tier1;
         }
         break;
      case 20: 
         if (CivS->Tech.branch3.Strategy == false && CivS->Tech.branch3.Organization && CivS->stars >= CivS->Tech.Tier2) {
             CivS->Tech.branch3.Strategy = true;
             CivS->stars -= CivS->Tech.Tier2;
         }
         break;
      case 21: 
         if (CivS->Tech.branch3.Diplomacy == false && CivS->Tech.branch3.Strategy && CivS->stars >= CivS->Tech.Tier3) {
             CivS->Tech.branch3.Diplomacy = true;
             CivS->stars -= CivS->Tech.Tier3;
         }
         break;

      
      //------------------------------------------------
      // Branch 4
      //------------------------------------------------


      case 22: 
         if (CivS->Tech.branch4.Mining == false && CivS->Tech.branch4.Climbing && CivS->stars >= CivS->Tech.Tier2) {
             CivS->Tech.branch4.Mining = true;
             CivS->stars -= CivS->Tech.Tier2;
         }
         break;
      case 23: 
         if (CivS->Tech.branch4.Smithery == false && CivS->Tech.branch4.Mining && CivS->stars >= CivS->Tech.Tier3) {
             CivS->Tech.branch4.Smithery = true;
             CivS->stars -= CivS->Tech.Tier3;
         }
         break;
      case 24: 
         if (CivS->Tech.branch4.Climbing == false && CivS->stars >= CivS->Tech.Tier1) {
             CivS->Tech.branch4.Climbing = true;
             CivS->stars -= CivS->Tech.Tier1;
         }
         break;
      case 25: 
         if (CivS->Tech.branch4.Meditation == false && CivS->Tech.branch4.Climbing && CivS->stars >= CivS->Tech.Tier2) {
             CivS->Tech.branch4.Meditation = true;
             CivS->stars -= CivS->Tech.Tier2;
         }
         break;
      case 26: 
         if (CivS->Tech.branch4.Philosophy == false && CivS->Tech.branch4.Meditation && CivS->stars >= CivS->Tech.Tier3) {
             CivS->Tech.branch4.Philosophy = true;
             CivS->stars -= CivS->Tech.Tier3;
         }
         break;


      //------------------------------------------------
      // Branch 5
      //------------------------------------------------


      case 27: 
         if (CivS->Tech.branch5.Ramming == false && CivS->Tech.branch5.Fishing == true && CivS->stars >= CivS->Tech.Tier2) {
             CivS->Tech.branch5.Ramming = true;
             CivS->stars -= CivS->Tech.Tier2;
         }
         break;
      case 28: 
         if (CivS->Tech.branch5.Aquatism == false && CivS->Tech.branch5.Ramming == true && CivS->stars >= CivS->Tech.Tier3) {
             CivS->Tech.branch5.Aquatism = true;
             CivS->stars -= CivS->Tech.Tier3;
         }
         break;
      case 29: 
         if (CivS->Tech.branch5.Fishing == false && CivS->stars >= CivS->Tech.Tier1) {
             CivS->Tech.branch5.Fishing = true;
             CivS->stars -= CivS->Tech.Tier1;
         }
         break;
      case 30: 
         if (CivS->Tech.branch5.Sailing == false && CivS->Tech.branch5.Fishing == true && CivS->stars >= CivS->Tech.Tier2) {
             CivS->Tech.branch5.Sailing = true;
             CivS->stars -= CivS->Tech.Tier2;
         }
         break;
      case 31: 
         if (CivS->Tech.branch5.Navigation == false && CivS->Tech.branch5.Sailing == true && CivS->stars >= CivS->Tech.Tier3) {
             CivS->Tech.branch5.Navigation = true;
             CivS->stars -= CivS->Tech.Tier3;
         }
         break;
   }
}


//------------------------------------------------
// Movement
//------------------------------------------------


int check_on_move(const int size, const int x, const int y, int** world, struct Civs* CivS, int*** Moves, int sizeof_Moves) {
   *Moves = calloc((size_t)sizeof_Moves, sizeof(int*));
   if (!(*Moves)) return 0;

   for (int i=0; i<sizeof_Moves; i++) {
      (*Moves)[i] = NULL;
   }

   for (int i=0; i<sizeof_Moves; i++) {
      if ((*Moves)[i] == NULL) {
         (*Moves)[i] = calloc(2, sizeof(int));
         if (!(*Moves)[i]) {
            for (int j=0; j<i; j++) {
               free((*Moves)[j]);
            }
            free(*Moves);
            *Moves = NULL;
            return 0;
         }
      }
   }

   struct Units* Unit = ReturnUnit(CivS, x, y);
   if (Unit == NULL) return 0;

   int pos = 0;

   for (int dx = x-Unit->stat.movement; dx <= x+Unit->stat.movement; dx++) {
      for (int dy = y-Unit->stat.movement; dy <= y+Unit->stat.movement; dy++) {
         if (pos >= sizeof_Moves) return 1;
         if (dx == x && dy == y) continue;
         if (dx < 0 || dx >= size || dy < 0 || dy >= size) continue;

         struct Units* UN = ReturnUnit(CivS, dx, dy);
         if (UN != NULL) continue;

         switch(world[dx][dy]) {
            case 0:
               break;
            case 1:
               (*Moves)[pos][0] = dx;
               (*Moves)[pos][1] = dy;
               pos++;
               break;
            case 2:
               (*Moves)[pos][0] = dx;
               (*Moves)[pos][1] = dy;
               pos++;
               break;
            case 3:
               if (CivS->Tech.branch4.Climbing == true) {
                  (*Moves)[pos][0] = dx;
                  (*Moves)[pos][1] = dy;
                  pos++;
               }
               break;
            case 4:
               (*Moves)[pos][0] = dx;
               (*Moves)[pos][1] = dy;
               pos++;
               break;
            case 5:
               struct Units* Enemy = ReturnUnit(CivS, dx, dy);
               if (Enemy == NULL) {
                  (*Moves)[pos][0] = dx;
                  (*Moves)[pos][1] = dy;
                  pos++;
               }
               break;
         }
      }
   }
   if (pos != 0) {
      return 1;
   }
   return 0;
}

int Movement(const int x1, const int y1, const int x2, const int y2, struct Civs* CivS, int** Moves, int sizeof_Moves) {
   struct Units* Unit = ReturnUnit(CivS, x1, y1);
   if (!Unit) return 0;

   if (Moves == NULL) return 0;

   if (Moves != NULL) {
      for (int i=0; i<sizeof_Moves; i++) {
         if (Moves[i] != NULL) {
            if (Moves[i][0] == 0 && Moves[i][1] == 0) {
               if ((x1 < 0 || x1 > 1) || (y1 < 0 || y1 > 1)) {
                  continue;
               }
            }
            if (x2 == Moves[i][0] && y2 == Moves[i][1]) {
               Unit->x = x2;
               Unit->y = y2;
               /*
               if (x1-x2 == 0) {
                  Unit->stat.movement -= abs(y1-y2);
               } else if (y1-y2 == 0) {
                  Unit->stat.movement -= abs(x1-x2);
               } else {
                  int temp_x = abs(x1-x2);
                  int temp_y = abs(y1-y2);
                  if (temp_x > temp_y) {
                     Unit->stat.movement -= temp_x;
                  } else if (temp_y > temp_x) {
                     Unit->stat.movement -= temp_y;
                  } else if (temp_x == temp_y) {
                     Unit->stat.movement -= temp_x;
                  }
               }
               */
               return 1;
            }
         }
      }
   } else {
      return 0;
   }
   return 0;
}

int check_on_combat(const int size, const int x, const int y, struct Civs* CivS, int*** Combats, int sizeof_Combats) {
   *Combats = calloc((size_t)sizeof_Combats, sizeof(int*));
   if (!(*Combats)) return 0;

   for (int i=0; i<sizeof_Combats; i++) {
      (*Combats)[i] = NULL;
   }

   for (int i=0; i<sizeof_Combats; i++) {
      if ((*Combats)[i] == NULL) {
         (*Combats)[i] = calloc(2, sizeof(int));
         if (!(*Combats)[i]) {
            for (int j=0; j<i; j++) {
               free((*Combats)[j]);
            } 
            free(*Combats);
            *Combats = NULL;
            return 0;
         }
      }
   }
   
   struct Units* Unit = ReturnUnit(CivS, x ,y);
   if (Unit == NULL) return 0;

   int pos = 0;

   for (int dx = x-Unit->stat.range; dx <= x+Unit->stat.range; dx++) {
      for (int dy = y-Unit->stat.range; dy <= y+Unit->stat.range; dy++) {
         if (pos >= sizeof_Combats) return 1;
         if (dx == x && dy == y) continue;
         if (dx < 0 || dx >= size || dy < 0 || dy >= size) continue;

         struct Units* Enemy = ReturnUnit(CivS, dx, dy);
         if (Enemy == NULL) continue;

         if (Enemy->numb == Unit->numb) continue;

         (*Combats)[pos][0] = dx;
         (*Combats)[pos][1] = dy;
         pos++;
      }
   }
   if (pos != 0) {
      return 1;
   }
   return 0;
}

int defence(struct Units* Unit, struct Civs* CivS, int** world) {
   if (!Unit) return 0;

   struct Civs* CIV = ReturnCiv(CivS, Unit->numb);
   if (CIV == NULL) return 0;

   int defence = Unit->stat.def;

   switch(world[Unit->x][Unit->y]) {
      case 0:
         if (CIV->Tech.branch5.Aquatism == true) {
            defence += 1;
         }
         break;
      case 1:
         break;
      case 2:
         if (CIV->Tech.branch1.Archery == true) {
            defence += 1;
         }
         break;
      case 3:
         if (CIV->Tech.branch4.Climbing == true) {
            defence += 1;
         }
      case 4:
         break;
      case 5:
         defence += 1;
         break;
   }
   return defence;
}

// If all good - make combat between units, return 1, else return 0
int Combat(const int x1, const int y1, const int x2, const int y2, struct Civs* CivS, int** Combat, int sizeof_Combats, int** world) {
   if (Combat == NULL) return 0;

   struct Units* Unit = ReturnUnit(CivS, x1, y1);
   if (Unit == NULL) return 0;

   struct Units* Enemy = ReturnUnit(CivS, x2, y2);
   if (Enemy == NULL) return 0;

   if (Combat != NULL) {
      for (int i=0; i<sizeof_Combats; i++) {
         if (Combat[i] != NULL) {
            if (Combat[i][0] == 0 && Combat[i][1] == 0) {
               if ((x1 < 0 || x1 > 1) || (y1 < 0 || y1 > 1)) {
                  continue;
               }
            }

            if (Combat[i][0] == Enemy->x && Combat[i][1] == Enemy->y) {

               int EHp = Enemy->stat.hp;
               int EDef = defence(Enemy, CivS, world);
               Enemy->stat.hp = EHp - Unit->stat.dmg + EDef;
               if (Enemy->stat.hp >= EHp) {
                  Enemy->stat.hp = EHp -1;
               }

               Unit->stat.hp = Unit->stat.hp - Enemy->stat.def;

               /*
               if (Unit->type != 7) {
                  Unit->combat = 0;
               }

               if (Unit->type != 2 && Unit->type != 7) {
                  Unit->stat.movement = 0;
               }
               */

               if (Enemy->stat.hp <= 0) {
                  delete_Unit(CivS, Enemy->x, Enemy->y);
               }
               if (Unit->stat.hp <= 0) {
                  delete_Unit(CivS, Unit->x, Unit->y);
               }

               return 1;
            }
         }
      }
   }
   return 0;
}

//-------------------------------
// Resources
//-------------------------------


int TakeResource(const int x, const int y, int** resources, struct Civs* CivS) {
   if (resources[x][y] == 0 || resources[x][y] == 2) {
      return 0;
   }

   int Numb = ReturnNumbOfCiv(CivS, x, y);
   if (Numb == -1) return 1;

   struct Civs* Civ = ReturnCiv(CivS, Numb);
   if (Civ == NULL) return 2;

   struct City* CitY = ReturnCity(CivS, x, y);
   if (CitY == NULL) return 3;

   if (resources[x][y] == 1 || resources[x][y] == 3) {
      resources[x][y] -= 1;
   }
   return 0;
}


//---------------------------
// Building
//---------------------------


int Building(const int x, const int y, int** world, int** resources, struct Civs* CivS) {
   int Numb = ReturnNumbOfCiv(CivS, x, y);
   if (Numb == -1) return 1;

   struct Civs* Civ = ReturnCiv(CivS, Numb);
   if (Civ == NULL) return 2;

   struct City* CitY = ReturnCity(CivS, x, y);
   if (CitY == NULL) return 3;

   if (world[x][y] == 0) {
      resources[x][y] = Port;
      return Port;
   } else if (world[x][y] == 1 && resources[x][y] == 2) {
      resources[x][y] = Farm;
      return Farm;
   } else if (world[x][y] == 2) {
      resources[x][y] = Lumber_cut;
      return Lumber_cut;
   } else if (world[x][y] == 3 && resources[x][y] == 2) {
      resources[x][y] = Mine;
      return Mine;
   } else if (world[x][y] == 4) {
      return 0;
   } else if (world[x][y] == 5) {
      return 0;
   }
   return 0;
}


//---------------------------
// Capture City
//---------------------------

// Check: does any city under siege
void check_capturing_city(struct Civs* CivS, struct GAME* Game) {
   if (CivS == NULL) return;

   struct Civs* CIV = CivS;
   if (CIV == NULL) return;

   while (CIV != NULL) {
      struct City* CITY = CIV->Cities;
      while (CITY != NULL) {
         CITY->siege_turn = 0;
         CITY->Capture_City = false;

         CITY = CITY->next;
      }
      CIV = CIV->next;
   }

   CIV = CivS;
   while (CIV != NULL) {
      struct City* CITY = CIV->Cities;
      while (CITY != NULL) {
         struct Units* Unit = ReturnUnit(CivS, CITY->x, CITY->y);
         if (Unit != NULL) {
            if (Unit->numb != CITY->numb) {
               CITY->siege_turn = Game->turn;
            }
         }
         CITY = CITY->next;
      }
      CIV = CIV->next;
   }
}

// Remove city from CIV1 and Add to CIV2
void Capture_City(struct Civs* CivS, const int x, const int y, const int NumCIV2) {
   if (CivS == NULL) return;
   
   struct City* CITY = ReturnCity(CivS, x, y);
   if (CITY == NULL) return;

   int NumCIV1 = ReturnNumbOfCiv(CivS, x, y);
   if (NumCIV1 == -1) return;

   struct Civs* CIV1 = ReturnCiv(CivS, NumCIV1);
   if (CIV1 == NULL) return;

   struct Civs* CIV2 = ReturnCiv(CivS, NumCIV2);
   if (CIV2 == NULL) return;

   if (CITY->prev) {
      CITY->prev->next = CITY->next;
   } else {
      CIV1->Cities = CITY->next;
   }

   if (CITY->next) {
      CITY->next->prev = CITY->prev;
   }


   struct City* CitiesCV2 = CIV2->Cities;
   if (CitiesCV2 == NULL) return;

   while (CitiesCV2->next != NULL) {
      CitiesCV2 = CitiesCV2->next;
   }

   CitiesCV2->next = CITY;
   CITY->prev = CitiesCV2;

   CITY->numb = NumCIV2;
   CIV1->countCity -= 1;
   CIV2->countCity += 1;
   
   /*
   int countOfCityCV = 0;
   struct City* CTCV1 = CIV1->Cities;
   while (CTCV1 != NULL) {
      countOfCityCV++;
      CTCV1 = CTCV1->next;
   }
   CIV1->countCity = countOfCityCV;

   countOfCityCV = 0;
   struct City* CTCV2 = CIV2->Cities;
   while (CTCV2 != NULL) {
      countOfCityCV++;
      CTCV2 = CTCV2->next;
   }
   CIV2->countCity = countOfCityCV;
   //CIV2->countCity += 1;
   */
}

