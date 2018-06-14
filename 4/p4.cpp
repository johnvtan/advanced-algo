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

void branchAndBound(knapsack &k) {
   cout << "Running...\n";
   vector<BranchAndBoundNode> partial_solutions;
   
   // set up the node list with choosing to not select, then select the first item in the knapsack
   vector<int> temp;
   //cout << "test" << endl;
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
         if (partial_solutions[i].getUpperBound() > highest_upper_bound_value) {
            highest_upper_bound_index = i;
            highest_upper_bound_value = partial_solutions[i].getUpperBound();
         }
      }

      highest_upper_bound_solution = partial_solutions[highest_upper_bound_index];

      // check to see if we have a complete solution 
      if (highest_upper_bound_solution.getLayer() == k.getNumObjects() - 1) {
         if (highest_upper_bound_solution.getSelectedItemsValue() > best_solution.getSelectedItemsValue()) {
            best_solution = highest_upper_bound_solution;
            //cout << "New Best Solution found : " << highest_upper_bound_index << " Value = " << best_solution.getSelectedItemsValue() << endl;
            //best_solution.printItems();
         } else {
            //cout << "Index " << highest_upper_bound_index << " erased because value <= best value" << endl;
            //cout << "Erased value: " << partial_solutions[highest_upper_bound_index].getSelectedItemsValue() << " Best value : " << best_solution.getSelectedItemsValue() << endl;
            //partial_solutions[highest_upper_bound_index].printItems();
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

      // remove partial solutions whose upper bounds are less than or equal to our current best solution's value
      size = partial_solutions.size();
      for (int i = 0; i < size; i++) {
         if (partial_solutions[i].getUpperBound() <= best_solution.getSelectedItemsValue()) {
            partial_solutions.erase(partial_solutions.begin() + i);
            i--;
            size--;
         }
      }
   }

   k.unSelectAll();
   k.selectList(best_solution.getSelectedItems());
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
      branchAndBound(k);

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
