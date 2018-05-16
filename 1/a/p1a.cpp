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

/*
void exhaustiveKnapsackRecursive(vector <int> items, int max) {
   //int startItem = items.back();
   for (int k = 0; k < items.size(); k++) {
         cout << items[k] << " ";
   }
   cout << endl;
   for (int i = items.back() + 1; i < max; i++) {
      items.push_back(i);
      doCombinationRecursive(items, max);
      items.pop_back();
   }
}

void exhaustiveKnapsack(knapsack &k, int t) {
   vector <int> items(1, 0);
   int max = k.getNumObjects();
   for (int i = 0; i < max; i++) {
      items[0] =  i;
      doCombinationRecursive(items, max);
   }
}
*/

void printVector(vector <int> v) {
   for (unsigned int i = 0; i < v.size(); i++) {
      cout << v[i] << " ";
   }
   cout << endl;
}

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
      // take everything out of knapsack
      k.unSelectAll();
      s = subset.nextSubset();
      k.selectList(s);

      if (k.getCost() <= k.getCostLimit() && k.getValue() > best_value) {
         best_combination = s;
         best_value = k.getValue();
      }
      total_time = (clock() - start_time) / (double) CLOCKS_PER_SEC;
      if (total_time >= t) {
         break;
      }

   }  
   k.unSelectAll();
   k.selectList(best_combination);
   k.printSolution();
   cout << "Total Time: " << total_time << "seconds\n";
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
      cout << "Reading knapsack instance" << endl;
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
