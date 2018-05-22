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

void greedyKnapsack(knapsack &k) {
	int nextIndex = -1;
	while (!k.allItemsOff()) {
		nextIndex = k.getMaxRatioIndex();
		if (k.getCost() + k.getCost(nextIndex) > k.getCostLimit()) {
			k.setRatioOff(nextIndex);
			//cout << "Can't put " << nextIndex << " in knapsack" << endl;
		} else {
			k.select(nextIndex);
			//cout << "Selected: " << nextIndex << endl;
		}
	}
	k.printSolution();
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
      greedyKnapsack(k);
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
