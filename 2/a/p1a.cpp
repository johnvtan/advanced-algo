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

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include "knapsack.h"
#include "subset.h"

// exhaustively searches knapsack for up to t seconds 
// then prints out best solution
void exhaustiveKnapsack(knapsack &k, int t) {
   Subset subset(k.getNumObjects());
   vector <int> s;
   int best_value = 0;
   vector <int> best_combination;
   clock_t start_time;
   start_time = clock();
   double total_time = 0;

   while (subset.hasNext()) {
      // take everything out of knapsack on each iteration
      k.unSelectAll();

      // generate a new subset and put those items into the (empty) knapsack
      s = subset.nextSubset();
      k.selectList(s);

      if (k.getCost() <= k.getCostLimit() && k.getValue() > best_value) {
         // if current selection is under cost limit and has better value
         // update our stored bests
         best_combination = s;
         best_value = k.getValue();
      }
      total_time = (clock() - start_time) / (double) CLOCKS_PER_SEC;
      if (total_time >= t) {
         break;
      }

   }  
   // in order to print the solution, we empty the knapsack and re-select
   // out best combination and use the knapsack's printSolution
   k.unSelectAll();
   k.selectList(best_combination);
   k.printSolution();
   //cout << "Total Time: " << total_time << "seconds\n";
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

      //exhaustiveKnapsack(k, 600);

      //cout << endl << "Best solution" << endl;
      //k.printSolution();
      exhaustiveKnapsack(k, 600);
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
