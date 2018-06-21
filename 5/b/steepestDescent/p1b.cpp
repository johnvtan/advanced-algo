// Code to read graph instances from a file.  Uses the Boost Graph Library (BGL).

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <climits>

#include <boost/graph/adjacency_list.hpp>

#define LargeValue 99999999

using namespace std;
using namespace boost;

class nullPtrError : public baseException
{
    public:
        nullPtrError(const string& msg = ""):
            baseException(msg)
        {}
};

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
  if ( !v.empty() ) {
    out << '[';
    std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
    out << "\b\b]";
  }
  return out;
}

struct VertexProperties;
struct EdgeProperties;

typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties> Graph;

struct VertexProperties
{
    pair<int,int> cell; // maze cell (x,y) value
    Graph::vertex_descriptor pred;
    bool visited;
    bool marked;
    int weight;
    int color;

    VertexProperties() : color(0) {}
};

// Create a struct to hold properties for each edge
struct EdgeProperties
{
    int weight;
    bool visited;
    bool marked;
};

void initializeGraph(Graph &g, ifstream &fin)
// Initialize g using data from fin.
{
    int n, e;
    int j,k;
    
    fin >> n >> e;
    Graph::vertex_descriptor v;
    
    // Add nodes.
    for (int i = 0; i < n; i++)
        v = add_vertex(g);
    
    for (int i = 0; i < e; i++)
    {
        fin >> j >> k;
        add_edge(j,k,g);  // Assumes vertex list is type vecS
    }
}

void setRandomColors(Graph &g, int numColors) {
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr) {
        // set a random color on the new node
        g[*vItr].color = rand() % numColors;
    }
}

// return a vector containing each node's color
vector<int> getGraphColors(const Graph &g) {
    vector<int> colors;

    for (unsigned int i = 0; i < num_vertices(g); i++) {
        colors.push_back(g[i].color);
    }

    return colors;
}

// update each node's color based on the vector passed in
void setGraphColors(Graph &g, const vector<int> colors) {
    for (unsigned int i = 0; i < num_vertices(g); i++) {
        g[i].color = colors[i];
    }
}

// switch one node in the graph to a random different color, once for each node
vector< vector<int> > getNeighbors(vector<int> solution, int numColors) {
    vector< vector<int> > neighbors;

    // if we only have 1 color available, there are no neighbors
    if (numColors <= 1) {
        neighbors.push_back(solution);
        return neighbors;
    }

    // loop through each node in the list and randomize its color, then add that new neighbor
    for (unsigned int i = 0; i < solution.size(); i++) {
        int currentColor = solution[i];
        int newColor = solution[i];

        // keep choosing a random number until we get one that's different than the current color
        while (newColor == currentColor) {
            newColor = rand() % numColors;
        }

        // loop through the neighbor and add the new node color
        vector<int> newNeighbor;
        for (unsigned int j = 0; j < solution.size(); j++) {
            if (i == j) {
                newNeighbor.push_back(newColor);
            }
            else {
                newNeighbor.push_back(solution[j]);
            }
        }
        neighbors.push_back(newNeighbor);
    }

    return neighbors;
}

int numConflicts(Graph &g) {
    pair<Graph::edge_iterator, Graph::edge_iterator> eItrRange = edges(g);
    Graph::vertex_descriptor v1, v2;
    int count = 0;
    // Loop over all edges in the graph
    for (Graph::edge_iterator eItr= eItrRange.first; eItr != eItrRange.second; ++eItr) {
        
        // count the number of conflicts by checking the source and target nodes
        // of each edge in the graph and seeing if their colors match
        v1 = source(*eItr, g);
        v2 = target(*eItr, g);
        if (g[v1].color == g[v2].color) {
            count++;
        }
    }
    return count;
}

void printSolution(Graph &g, int numConflicts)
{
    cout << "Total Conflicts: " << numConflicts << endl;

    for (unsigned int counter = 0; counter < num_vertices(g); counter++)
    {
        cout << counter << ": " << g[counter].color << endl;

    }
}

int steepestDescent(Graph &g, int numColors, int t) {
   time_t seed = time(0);
   srand(seed);

    setRandomColors(g, numColors);

    vector<int> bestSolution = getGraphColors(g);
    int bestNumConflicts = numConflicts(g);

    vector< vector<int> > neighbors;

    bool bestSolutionChanged = true;

    clock_t start_time;
    start_time = clock();
    double total_time = 0;

   // keep improving our solution until we reach a local maximum
    while (bestSolutionChanged) {
        bestSolutionChanged = false;
        neighbors = getNeighbors(bestSolution, numColors);

        // loop through each neighbor and count the conflicts associated with it
        for (unsigned int i = 0; i < neighbors.size(); i++) {
            // try coloring the graph
            setGraphColors(g, neighbors[i]);

            // get the number of conflicts associated with the neighbor
            vector<int> currentSolution = getGraphColors(g);
            int currentNumConflicts = numConflicts(g);

            // compare our current conflicts to the best so far
            if (currentNumConflicts < bestNumConflicts) {
                bestSolution = currentSolution;
                bestNumConflicts = currentNumConflicts;
                bestSolutionChanged = true;
            }
        }

        // checking if we go over time limit
        total_time = (clock() - start_time) / (double) CLOCKS_PER_SEC;
        if (total_time >= t) {
            break;
        }
    }

    // make sure we update the graph to reflect our final solution
    setGraphColors(g, bestSolution);

    printSolution(g, bestNumConflicts);
    return bestNumConflicts;
}

int main(int argc, char **argv)
{
    ifstream fin;
    string fileName;
    
    // Read the name of the graph from the keyboard or
    // hard code it here for testing.
    if (2 != argc) {
      cout << "Error: need to include path to input as argument.\n"
           << "\tp1b.out <input-file>\n";
      exit(1);
    }
    
    //   cout << "Enter filename" << endl;
    //   cin >> fileName;
    
    fin.open(argv[1]);
    if (!fin)
    {
        cerr << "Cannot open " << fileName << endl;
        exit(1);
    }
    
    try
    {
        //cout << "Reading graph" << endl;
        Graph g;
        int numColors;
        int numConflicts = -1;
        fin >> numColors;
        initializeGraph(g, fin);
        
        numConflicts = steepestDescent(g, numColors, 300);
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