// Project 1a: Solving knapsack using exhaustive search
//

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <stack>
#include <vector>
#include <time.h>
#include <cstdlib>
#include <algorithm>

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include "knapsack.h"
#include "subset.h"

int randomFunc(int i) { return rand() % i; }

// randomly selects or not selects each item in the knapsack
// so long as the knapsack has a viable cost
void initializeRandomKnapsack(knapsack &k) {
   k.unSelectAll();

   vector<int> itemOrder;
   for (int i = 0; i < k.getNumObjects(); i++) {
      itemOrder.push_back(i);
   }

   // order the items in a random order
   random_shuffle(itemOrder.begin(), itemOrder.end(), randomFunc);

   for (int i = 0; i < k.getNumObjects(); i++) {
      // if we choose a random number between 0 and 1, and it's 1, select the item
      if (rand() % 2) {
         if (k.getCost() + k.getCost(itemOrder[i]) <= k.getCostLimit()) {
            k.select(itemOrder[i]);
         }
      }
   }
}

// gets neighboring solutions to the current knapsack by
// switching on/off every item in the knapsack
vector< vector<int> > getNeighbors(knapsack &k) {
   vector< vector<int> > neighbors;

   for (int i = 0; i < k.getNumObjects(); i++) {
      if (k.isSelected(i)) {
         k.unSelect(i);
         neighbors.push_back(k.getSelectedItems());
         k.select(i);
      }
      else {
         if (k.getCost() + k.getCost(i) <= k.getCostLimit()) {
            k.select(i);
            neighbors.push_back(k.getSelectedItems());
            k.unSelect(i);
         }
      }
   }

   return neighbors;
}

// performs steepest descent search on the knapsack until 
// no better neighbor exists
void steepestDescent(knapsack &k, int t) {
   clock_t startTime;
   startTime = clock();
   double totalTime = 0.0;
   time_t seed = time(0);
   cout << "Seed is " << seed << endl;

   srand(1529355919);
   initializeRandomKnapsack(k);

   knapsack bestSolution = k;

   bool bestSolutionChanged = true;

   // keep improving our solution until we reach a local maximum
   while (bestSolutionChanged) {
      bestSolutionChanged = false;
      vector< vector<int> > neighbors = getNeighbors(k);

      // check all neighbors to see if there is a better neighbor
      // if so, choose the best neighbor among them
      for (int i = 0; i < neighbors.size(); i++) {
         k.unSelectAll();
         k.selectList(neighbors[i]);

         if (k.getValue() > bestSolution.getValue()) {
            bestSolutionChanged = true;
            bestSolution = k;
         }
      }

      // check to ensure we haven't gone over the time limit
      totalTime = (clock() - startTime) / (double) CLOCKS_PER_SEC;
      if (totalTime >= t) {
         break;
      }
   }

   vector< vector<int> > neighbors = getNeighbors(bestSolution);
   for (int i = 0; i < neighbors.size(); i++) {
      cout << "Neighbor " << i << ": ";
      for (int k = 0; k < neighbors[i].size(); k++) {
         cout << neighbors[i][k] << " ";
      }
      cout << endl;
   }


   bestSolution.printSolution();
}

int main(int argc, char **argv)
{
   ifstream fin;
   stack <int> moves;
   string fileName;

   if (2 != argc) 
   {
      cout << "Error: need to include path to input as argument.\n"
           << "\tp1a.out <input-file>\n";
      exit(1);
   }

   fin.open(argv[1]);
   if (!fin)
   {
      cerr << "Cannot open " << fileName << endl;
      exit(1);
   }

   try
   {
      //cout << "Reading knapsack instance" << endl;
      knapsack k(fin);

      steepestDescent(k, 300);
   }    

   catch (indexRangeError &ex) 
   { 
      cout << ex.what() << endl; exit(1);
   }
   catch (rangeError &ex)
   {
      cout << ex.what() << endl; exit(1);
   }
}
