// Code to read graph instances from a file.  Uses the Boost Graph Library (BGL).

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <climits>

#include <boost/graph/adjacency_list.hpp>

#include "Genetic.h"

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

int const NONE = -1;  // Used to represent a node that does not exist

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

static Graph globalG;

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

void setNodeWeights(Graph &g, int w)
// Set all node weights to w.
{
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
    
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr)
    {
        g[*vItr].weight = w;
    }
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

// helper function to print out graph coloring
void printColors(Graph &g) {
    cout << "---------------New Coloring---------------" << endl;
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr) {
        cout << "Node (" << *vItr << "): " << g[*vItr].color << endl;
    }
}

void printSolution(Graph &g, int numConflicts)
{
    cout << "Total Conflicts: " << numConflicts << endl;

    for (int counter = 0; counter < num_vertices(g); counter++)
    {
        cout << counter << ": " << g[counter].color << endl;

    }
}

int getNumNodes(Graph &g) {
    int rv = 0;
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
    
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr)
    {
        ++rv;
    }
    return rv;
}

// fitness function that genetic algo class points to 
int fitness(vector<int> p) {
    int pIdx = 0;
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(globalG);
    
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr)
    {
        // set each color for each node in the graph to the corresponding element in vec p
        globalG[*vItr].color = p[pIdx++];
    }
    return numConflicts(globalG);
}

// runs genetic algo to find coloring with least conflicts
int geneticColoring(int numColors, int t) {
    clock_t startTime = clock();
    double totalTime = 0.0;
    
    // we set max population size to the number of nodes in the graph
    int maxPopulationSize = getNumNodes(globalG);
    int minInitialPopulationSize = maxPopulationSize / 2;
    
    // each individual should be the size of the number of nodes in the graph
    int individualSize = getNumNodes(globalG);
    double survivalRate = 0.25;
    bool minimize = true;

    Genetic g(numColors, maxPopulationSize, minInitialPopulationSize, individualSize, survivalRate, minimize, fitness);
    int lowestConflicts = INT_MAX;
    vector<int> bestSolution, thisSolution;
    int genCount = 0;
    while (totalTime <= t) {
        thisSolution = g.nextGeneration();
        //cout << "Gen " << genCount++ << ": " << fitness(thisSolution) << endl;
        // check to make sure the next generation solution is better
        if (fitness(thisSolution) < lowestConflicts) {
            //cout << "Better solution!" << endl;
            bestSolution = thisSolution;
            lowestConflicts = fitness(thisSolution);
        }
        totalTime = (clock() - startTime) / (double) CLOCKS_PER_SEC;
    }

    // call fitness() instead of returning lowestConflicts because
    // we need fitness to select the solution in the global graph
    return fitness(bestSolution);
}


int main(int argc, char **argv)
{
    char x;
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
        int numColors;
        int numConflicts = -1;
        fin >> numColors;
        initializeGraph(globalG,fin);
        
        numConflicts = geneticColoring(numColors, 300);
        printSolution(globalG, numConflicts);

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