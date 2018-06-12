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

void branchAndBound(knapsack &k) {
   vector<int> best_solution;
   vector<BranchAndBoundNode> partial_solutions;
   
   vector<int> temp;
   partial_solutions.push_back(BranchAndBoundNode(temp, 0));
   temp.push_back(0);
   partial_solutions.push_back(BranchAndBoundNode(temp, 0));
   
   int highest_bound = 0;
   int highest_index = -1;
   int size = 0;
   while ((size = partial_solutions.size())) {
      // calculate upper bound on each solution in partial_solutions
      for (int i = 0; i < size; i++) {
         k.selectList(partial_solutions[i].getSelectedItems());
         if (k.bound() > highest_bound) {
            highest_index = i;
            highest_bound = k.bound();
         }
      }
      
   }
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
      branchAndBound(k);
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
