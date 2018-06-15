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
   
   int next_value = 0;
   int next_index = -1;
   int size = 0;
   bool new_best_solution = false;
   bool find_new_partial_solution = true;
   BranchAndBoundNode next_solution;
   int child_1_index, child_2_index;
   while ((size = partial_solutions.size())) {
      child_1_index = -1;
      child_2_index = -1;
      next_value = 0;

      if (find_new_partial_solution) {
         // check the upper bound on each solution in partial_solutions and see which is the highest so far
         for (int i = 0; i < size; i++) {
            if (partial_solutions[i].getUpperBound() > next_value) {
               next_index = i;
               next_value = partial_solutions[i].getUpperBound();
            }
         }
         find_new_partial_solution = false;
      }

      // next_index should be set already if find_new_solutions is false
      next_solution = partial_solutions[next_index];

      //cout << "Id: " << next_solution.getId() << "; Best partial solution layer: " << next_solution.getLayer() 
      //<< " Index: " << next_index << " UB: " << next_value << endl;
      
      // check to see if we have a complete solution 
      if (next_solution.getLayer() == k.getNumObjects() - 1) {
         //cout << "got highest layer" << endl;
         if (next_solution.getSelectedItemsValue() > best_solution.getSelectedItemsValue()) {
            best_solution = next_solution;
            new_best_solution = true;
            //cout << "got new best" << endl;
         }
         partial_solutions.erase(partial_solutions.begin() + next_index);

         // if we've gone as deep as we can, then we search for a new best partial solution
         find_new_partial_solution = true;
      }
      // if we're not at the final layer of branching, we remove the node we just explored and branch its children
      else {
         partial_solutions.erase(partial_solutions.begin() + next_index);
         //cout << "erasing and spawning children from soln  " << next_solution.getId() << endl; 
         BranchAndBoundNode child1, child2;
         int curr_layer = next_solution.getLayer() + 1;

         vector<int> temp = next_solution.getSelectedItems();
         
         // If this partial solution was already in the list and wasn't pruned,
         // then it implies that it had a higher upper bound than our best solution's value
         // so it doesn't need to be checked.
         child1 = BranchAndBoundNode(temp, curr_layer, k);
         if (child1.getUpperBound() > best_solution.getSelectedItemsValue()) {
            partial_solutions.push_back(BranchAndBoundNode(temp, curr_layer, k));
            child_1_index = partial_solutions.size() - 1;
         }

         temp.push_back(curr_layer);
         k.unSelectAll();
         k.selectList(temp);
         if (k.getCost() <= k.getCostLimit()) {
            child2 = BranchAndBoundNode(temp, curr_layer, k);

            // we can prune new solutions as they arrive by comparing them to the value
            // of our previous best
            if (child2.getUpperBound() > best_solution.getSelectedItemsValue()) {
               partial_solutions.push_back(child2);
               child_2_index = partial_solutions.size() - 1;
            } 
         } 
         // then we decide which one will be our next solution to be explored - just take the one with a higher upper bound
         // might run into problems if they're at the last layer? 
         if (child_2_index > 0 && child_1_index > 0) {
            next_index = partial_solutions[child_2_index].getUpperBound() > partial_solutions[child_1_index].getUpperBound() ?
                         child_2_index :
                         child_1_index;
         } else if (child_2_index > 0) {
            next_index = child_2_index;
         } else if (child_1_index > 0) {
            next_index = child_1_index;
         } else {
            next_index = 0;
            find_new_partial_solution = true;
         }
      }

      // we only need to search through the old solutions if we get a new best solution
      // otherwise, we only need to prune new solutions as they come in.
      if (new_best_solution) {
         // remove partial solutions whose upper bounds are less than or equal to our current best solution's value
         size = partial_solutions.size();
         for (int i = 0; i < size; i++) {
            if (partial_solutions[i].getUpperBound() <= best_solution.getSelectedItemsValue()) {
               partial_solutions.erase(partial_solutions.begin() + i);
               i--;
               size--;
            }
         }
         new_best_solution = false;
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
