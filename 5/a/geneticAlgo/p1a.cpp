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
