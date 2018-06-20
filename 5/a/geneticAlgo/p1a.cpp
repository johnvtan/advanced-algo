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
#include "Genetic.h"

knapsack globalK;

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
  if ( !v.empty() ) {
    out << '[';
    std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
    out << "\b\b]";
  }
  return out;
}

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

// determines the fitness level of the knapsack based on the vector passed in
// return -1 if the knapsack solution is not feasible, getValue() otherwise
int fitness(vector<int> p) {
   vector<bool> selectedItems;

   // assign the value to the int vector to the value of the bool vector (0 -> false = not selected)
   for (unsigned int i = 0; i < p.size(); i++) {
      selectedItems.push_back(p[i]);
   }

   globalK.unSelectAll();
   globalK.selectList(selectedItems);

   if (globalK.getCost() > globalK.getCostLimit()) {
      return -1;
   }

   return globalK.getValue();
}

// performs a genetic algorithm on the knapsack to try and find a best solution
void geneticAlgorithm(int t) {
   clock_t startTime;
   startTime = clock();
   double totalTime = 0.0;
   time_t seed = time(0);

   int maxValue = 2;
   int maxPopulationSize = globalK.getNumObjects();
   int minInitialPopulationSize = globalK.getNumObjects() / 2;
   int individualSize = globalK.getNumObjects();
   double survivalRate = 0.25;
   bool minimize = false;

   Genetic g(maxValue, maxPopulationSize, minInitialPopulationSize, individualSize, survivalRate, minimize, fitness);

   // run our genetic algorithm for t seconds
   while (totalTime <= t) {
      globalK = g.nextGeneration();

      // check to ensure we haven't gone over the time limit
      totalTime = (clock() - startTime) / (double) CLOCKS_PER_SEC;
   }

   globalK.printSolution();
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
      globalK = knapsack(fin);

      geneticAlgorithm(300);
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
