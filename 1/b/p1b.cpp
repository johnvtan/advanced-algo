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

void incrementColors(Graph &g, int maxColor) {
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr) {
        g[*vItr].color++;
        if (g[*vItr].color > maxColor) {
            g[*vItr].color = 0;
        } else {
            break;
        }
    }
}

bool checkIfDone(Graph &g, int maxColor) {
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr) {
        if (g[*vItr].color != maxColor) {
            return false;
        }
    }
    return true;
}

/*
bool numConflicts(Graph &g) {
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

    // check each node's color in the graph and its neighbor's colorsclear
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr) {
        pair<Graph::adjacency_iterator, Graph::adjacency_iterator> adjItrRange = adjacent_vertices(*vItr, g);

        // loop through each neighbor for the current node and check the colors
        for (Graph::vertex_iterator adjItr= adjItrRange.first; adjItr != adjItrRange.second; ++adjItr) {
            // if the current node has the same color as a neighbor, we have an illegal coloring
            if (g[*vItr].color == g[*adjItr].color) {
                return false;
            }
        }
    }

    return true;
}
*/

void printColors(Graph &g) {
    cout << "---------------New Coloring---------------" << endl;
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr) {
        cout << "Node (" << *vItr << "): " << g[*vItr].color << endl;
    }
}

int exhaustiveColoring(Graph &g, int numColors, int t) {
    int leastConflicts = INT_MAX;
    int currNumConflicts = INT_MAX;
    int bestColors[num_vertices(g)] = {0};
    while (!checkIfDone(g, numColors - 1)) {
        // assign color to each node in graph
        // increment searchSpace
        // track the least number of conflicts
        incrementColors(g, numColors - 1);
        printColors(g);
        /*
        currNumConflicts = numConflicts(g);
        if (currNumConflicts < leastConflicts) {
            leastConflicts = currNumConflicts;
            int i = 0;
            pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
            for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr) {
                bestColors[i++] = g[*vItr].color;
            }
        } 
        */
    }
    return leastConflicts;
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
        cout << "Reading graph" << endl;
        Graph g;
        int numColors;
        int numConflicts = -1;
        fin >> numColors;
        initializeGraph(g,fin);
        
        cout << "Num nodes: " << num_vertices(g) << endl;
        cout << "Num edges: " << num_edges(g) << endl;
        cout << endl;
        
        // cout << g;
        
        numConflicts = exhaustiveColoring(g, numColors, 600);
        //printSolution(g, numConflicts);
        
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