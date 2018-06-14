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
#include "BranchAndBoundNode.h"

void branchAndBound(knapsack &k, int t) {
   cout << "Running...\n";

   clock_t start_time;
   start_time = clock();
   double total_time = 0;

   vector<BranchAndBoundNode> partial_solutions;
   
   // set up the node list with choosing to not select, then select the first item in the knapsack
   vector<int> temp;
   partial_solutions.push_back(BranchAndBoundNode(temp, 0, k));
   temp.push_back(0);
   partial_solutions.push_back(BranchAndBoundNode(temp, 0, k));

   // temporarily set our best solution
   BranchAndBoundNode best_solution = partial_solutions[0];
   
   int highest_upper_bound_value = 0;
   int highest_upper_bound_index = -1;
   int size = 0;
   BranchAndBoundNode highest_upper_bound_solution;
   while ((size = partial_solutions.size())) {
      highest_upper_bound_index = -1;
      highest_upper_bound_value = 0;

      // check the upper bound on each solution in partial_solutions and see which is the highest so far
      for (int i = 0; i < size; i++) {

         // remove a partial soltuion if its upper bound is less than our best solution so far
         if (partial_solutions[i].getUpperBound() <= best_solution.getSelectedItemsValue()) {
            partial_solutions.erase(partial_solutions.begin() + i);
            i--;
            size--;
            highest_upper_bound_index--;
         }

         // update the highest upper bound so far if applicable
         else if (partial_solutions[i].getUpperBound() > highest_upper_bound_value) {
            highest_upper_bound_index = i;
            highest_upper_bound_value = partial_solutions[i].getUpperBound();
         }
      }

      highest_upper_bound_solution = partial_solutions[highest_upper_bound_index];

      // check to see if we have a complete solution 
      if (highest_upper_bound_solution.getLayer() == k.getNumObjects() - 1) {
         if (highest_upper_bound_solution.getSelectedItemsValue() > best_solution.getSelectedItemsValue()) {
            best_solution = highest_upper_bound_solution;
         } else {
            partial_solutions.erase(partial_solutions.begin() + highest_upper_bound_index);
         }
      }
      // if we're not at the final layer of branching, we remove the node we just explored and branch its children
      else {
         partial_solutions.erase(partial_solutions.begin() + highest_upper_bound_index);

         int curr_layer = highest_upper_bound_solution.getLayer() + 1;

         vector<int> temp = highest_upper_bound_solution.getSelectedItems();
         partial_solutions.push_back(BranchAndBoundNode(temp, curr_layer, k));

         temp.push_back(curr_layer);
         k.unSelectAll();
         k.selectList(temp);
         if (k.getCost() <= k.getCostLimit()) {
            partial_solutions.push_back(BranchAndBoundNode(temp, curr_layer, k));
         }       
      }

      total_time = (clock() - start_time) / (double) CLOCKS_PER_SEC;
      if (total_time >= t) {
         break;
      }
   }

   k.unSelectAll();
   k.selectList(best_solution.getSelectedItems());
   cout << "Total time: " << total_time << endl;
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

      branchAndBound(k, 600);

      k.printSolution();
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
