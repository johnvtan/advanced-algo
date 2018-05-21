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

// function to generate another possible combination of colors in the graph
void incrementColors(Graph &g, int maxColor) {
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr) {
        
        // for each node in the graph, increment the color value
        g[*vItr].color++;

        // if the color is larger than max color, reset this node's color and increment
        // the next node's color
        if (g[*vItr].color > maxColor) {
            g[*vItr].color = 0;
        } else {
            // if the node's color didn't need to be reset, then we don't need to carry
            // over the increment and can stop the iteration here
            break;
        }
    }
}

// checks if we've tried every possible coloring for the graph
bool checkIfDone(Graph &g, int maxColor) {
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr) {
        
        // because we generate combinations by incrementing, we know that we are done
        // when every node's color is the maxColor value
        if (g[*vItr].color != maxColor) {
            return false;
        }
    }
    return true;
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

int exhaustiveColoring(Graph &g, int numColors, int t) {
    int leastConflicts = INT_MAX;
    int currNumConflicts = INT_MAX;
    int bestColors[num_vertices(g)] = {0};
    clock_t start_time;
    start_time = clock();
    double total_time = 0;

    // we break out of while loop if leastConflicts == 0 since we know
    // we can't do better than 0 conflicts
    while (!checkIfDone(g, numColors - 1) and leastConflicts > 0) {
        
        // we search over the entire space by using incrementColors
        incrementColors(g, numColors - 1);
        currNumConflicts = numConflicts(g);
        if (currNumConflicts < leastConflicts) {
            // if the current coloring has less conflicts than our previous best
            // then we set leastConflicts and store the colors of each node in bestColors
            leastConflicts = currNumConflicts;
            
            int i = 0;
            pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
            for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr) {
                bestColors[i++] = g[*vItr].color;
            }
        }

        // checking if we go over time limit
        total_time = (clock() - start_time) / (double) CLOCKS_PER_SEC;
        if (total_time >= t) {
            break;
        }
    }

    // print out best solution
    //printColors(g);
    for (int i = 0; i < num_vertices(g); i++) {
        g[i].color = bestColors[i];
    }
    printSolution(g, leastConflicts);
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
        //cout << "Reading graph" << endl;
        Graph g;
        int numColors;
        int numConflicts = -1;
        fin >> numColors;
        initializeGraph(g,fin);
        
        numConflicts = exhaustiveColoring(g, numColors, 600);
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