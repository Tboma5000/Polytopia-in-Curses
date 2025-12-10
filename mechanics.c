#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

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

void resources_generation(const int size, float** resourses, int** world) {
   // Set 0;
   for (int i=0; i<size; i++) {
      for (int j=0; j<size; j++) {
         resourses[i][j] = 0;
      }
   }

   int resources_rand = rand() % ((size*size) * RESOURCE / 100) +RESOURCE*2;
   for (int i=0; i<resources_rand; i++) {
      int rand_x, rand_y = 0;
      float type = 0;
      do {
         rand_x = rand() % size;
         rand_y = rand() % size;
      } while (world[rand_x][rand_y] == 4 || world[rand_x][rand_y] == 5);

      do {
         type = (float)(rand() % 3);
         if (type == 0) {
            type = 0.5;
         } else if (type == 1) {
            type = 1;
         } else if (type == 2) {
            type = 1.5;
         }
      } while (type == 1.5 && world[rand_x][rand_y] != 2);
      
      resourses[rand_x][rand_y] = type;
   }
}


//------------------------------------------
// Structs
//------------------------------------------


struct Civs* ReturnCiv(struct Civs* CivS, int number) {
   struct Civs* CV = CivS;
   while (CV && CV->numb != number) {
      CV = CV->next;
   }
   return CV;
}

struct Units* ReturnUnit(struct Civs* CivS, const int x, const int y) {
   struct Civs* CV = CivS;
   struct Units* UN = CV->Unites;
   while (CV!=NULL) {
      while (UN!=NULL) {
         if (UN->x == x && UN->y == y) {
            return UN;
         }
         UN = UN->next;
      }
      CV = CV->next;
   }
   return UN; 

}

void new_Unit(struct Units** head, int number, int x, int y, int type) {
   struct Units* newUnit = malloc(sizeof(struct Units));
   if (!newUnit) return;

   newUnit->x = x;
   newUnit->y = y;
   newUnit->numb = number;
   newUnit->type = type;
   newUnit->next = NULL;
   newUnit->prev = NULL;
   if (type == 1) {
      struct Charakteristics Warrior;
      Warrior.name = malloc(8);
      Warrior.name = "Warrior";
      Warrior.cost = 2;
      Warrior.hp = 10;
      Warrior.dmg = 2;
      Warrior.def = 2;
      Warrior.movement = 1;
      Warrior.range = 1;
      newUnit->stat = Warrior;
   } else if (type == 2) {
      struct Charakteristics Rider;
      Rider.name = malloc(6);
      Rider.name = "Rider";
      Rider.cost = 3;
      Rider.hp = 10;
      Rider.dmg = 2;
      Rider.def = 1;
      Rider.movement = 2;
      Rider.range = 1;
      newUnit->stat = Rider;
   } else if (type == 3) {
      struct Charakteristics Archer;
      Archer.name = malloc(7);
      Archer.name = "Archer";
      Archer.cost = 3;
      Archer.hp = 10;
      Archer.dmg = 2;
      Archer.def = 1;
      Archer.movement = 1;
      Archer.range = 2;
      newUnit->stat = Archer;
   } else if (type == 4) {
      struct Charakteristics Giant;
      Giant.name = malloc(6);
      Giant.name = "Giant";
      Giant.cost = 0;
      Giant.hp = 40;
      Giant.dmg = 5;
      Giant.def = 4;
      Giant.movement = 1;
      Giant.range = 1;
      newUnit->stat = Giant;
   } else if (type == 5) {
      struct Charakteristics Defender;
      Defender.name = malloc(9);
      Defender.name = "Defender";
      Defender.cost = 3;
      Defender.hp = 15;
      Defender.dmg = 1;
      Defender.def = 3;
      Defender.movement = 1;
      Defender.range = 1;
      newUnit->stat = Defender;
   } else if (type == 6) {
      struct Charakteristics Catapult;
      Catapult.name = malloc(9);
      Catapult.name = "Catapult";
      Catapult.cost = 8;
      Catapult.hp = 10;
      Catapult.dmg = 4;
      Catapult.def = 0;
      Catapult.movement = 1;
      Catapult.range = 3;
      newUnit->stat = Catapult;
   } else if (type == 7) {
      struct Charakteristics Knight;
      Knight.name = malloc(7);
      Knight.name = "Knight";
      Knight.cost = 8;
      Knight.hp = 10;
      Knight.dmg = 4; // 3.5
      Knight.def = 1;
      Knight.movement = 3;
      Knight.range = 1;
      newUnit->stat = Knight;
   } else if (type == 8) {
      struct Charakteristics Swordsman;
      Swordsman.name = malloc(10);
      Swordsman.name = "Swordsman";
      Swordsman.cost = 5;
      Swordsman.hp = 15;
      Swordsman.dmg = 3;
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

void new_City(struct City** head, int number, int x, int y) {
   struct City* newCity = malloc(sizeof(struct City));
   if (!newCity) return;

   newCity->numb = number;
   newCity->x = x;
   newCity->y = y;
   newCity->growth = 1;
   newCity->population = 0;
   newCity->production = 1;
   newCity->territory = malloc(BASIC_TERRITORY * sizeof(int*));
   int pos = 0;
   for (int dx=x-1; dx<=x+1; dx++) {
      for (int dy=y-1; dy<=y+1; dy++) {
         newCity->territory[pos] = malloc(2 * sizeof(int));
         newCity->territory[pos][0] = dx;
         newCity->territory[pos][1] = dy;
         pos++;
      }
   }
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

void new_Civ(struct Civs** head, int number, int x, int y) {
   struct Civs* newCiv = malloc(sizeof(struct Civs));
   if(!newCiv) return;

   newCiv->numb = number;
   newCiv->next = NULL;
   newCiv->prev = NULL;
   newCiv->Cities = NULL;
   //newCiv->Unites = NULL;
   newCiv->score = 0;
   newCiv->stars = 5;


   //---------------------------------------------
   // Technologies
   //---------------------------------------------


   newCiv->Tech.Tier1 = 5;
   newCiv->Tech.Tier2 = 7;
   newCiv->Tech.Tier3 = 10;


   //---------------------------------------------
   // Falst all technologies
   //---------------------------------------------

   
   //---------------------------------------------
   // Branch 1
   //---------------------------------------------


   newCiv->Tech.branch1.Hunting = false;
   newCiv->Tech.branch1.Archery = false;
   newCiv->Tech.branch1.Forestry = false;
   newCiv->Tech.branch1.Spiritualizm = false;
   newCiv->Tech.branch1.Mathematics = false;


   //---------------------------------------------
   // Branch 2
   //---------------------------------------------


   newCiv->Tech.branch2.Riding = false;
   newCiv->Tech.branch2.Roads = false;
   newCiv->Tech.branch2.Free_Spirit = false;
   newCiv->Tech.branch2.Trade = false;
   newCiv->Tech.branch2.Chivalry = false;


   //---------------------------------------------
   // Branch 3
   //---------------------------------------------

   newCiv->Tech.branch3.Organization = false;
   newCiv->Tech.branch3.Farming = false;
   newCiv->Tech.branch3.Strategy = false;
   newCiv->Tech.branch3.Construction = false;
   newCiv->Tech.branch3.Diplomacy = false;


   //---------------------------------------------
   // Branch 4
   //---------------------------------------------


   newCiv->Tech.branch4.Climbing = false;
   newCiv->Tech.branch4.Mining = false;
   newCiv->Tech.branch4.Meditation = false;
   newCiv->Tech.branch4.Smithery = false;
   newCiv->Tech.branch4.Philosophy = false;


   //---------------------------------------------
   // Branch 5
   //---------------------------------------------

   newCiv->Tech.branch5.Fishing = false;
   newCiv->Tech.branch5.Ramming = false;
   newCiv->Tech.branch5.Sailing = false;
   newCiv->Tech.branch5.Aquatism = false;
   newCiv->Tech.branch5.Navigation = false;

   
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
   new_City(&newCiv->Cities, number, x, y);
   new_Unit(&newCiv->Unites, number, x, y, 1);

   // Mark territory of capital - CIV
   newCiv->territory = newCiv->Cities->territory;
}

void civs_creation(const int size, int** world, struct Civs** CivS, int CIV) {
   int number = 1;
   for (int x=0; x<size; x++) {
      for (int y=0; y<size; y++) {
         if (world[x][y] == 5) {
            new_Civ(CivS, number, x, y);
            struct Civs* temp = *CivS;
            while(temp) {
               temp->Cities->production = 2;
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

void check_on_move(const int x, const int y, int** world, struct Civs* CivS, int*** Moves) {
   if (Moves!=NULL && *Moves!=NULL) {
      for (int i=0; i<24; i++) {
         if ((*Moves)[i] != NULL) {
            free((*Moves)[i]);
         }
      }
   }
   int** newMove = realloc(*Moves, 24 * sizeof(int*));
   *Moves = newMove;
   struct Units* Unit = ReturnUnit(CivS, x, y);
   for (int i=0; i<24; i++) {
      (*Moves)[i] = NULL;
   }
   for (int i=0; i<=Unit->stat.movement*8; i++) {
      (*Moves)[i] = malloc(2 * sizeof(int));
   }
   int pos = 0;
   if (Unit->stat.movement != 0) {
      for (int dx = x-Unit->stat.movement; dx<=x+Unit->stat.movement; dx++) {
         for (int dy = y-Unit->stat.movement; dy<=y+Unit->stat.movement; dy++) {
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
                  break;
            }
         }
      }
   }
}

int Movement(const int x1, const int y1, const int x2, const int y2, struct Civs* CivS, int** Moves) {
   struct Units* Unit = ReturnUnit(CivS, x1, y1);
   if (!Unit) return 0;
   if (Moves != NULL) {
      for (int i=0; i<24; i++) {
         if (Moves[i] != NULL) {
            if (x2 == Moves[i][0] && y2 == Moves[i][1]) {
               Unit->x = x2;
               Unit->y = y2;
               return 1;
            }
         }
      }
   } else {
      return 0;
   }
   return 0;
}
