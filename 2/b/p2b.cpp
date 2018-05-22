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
int const NO_COLOR = -1;

struct VertexProperties;
struct EdgeProperties;

typedef adjacency_list<vecS, vecS, undirectedS, VertexProperties, EdgeProperties> Graph;

struct VertexProperties
{
    pair<int,int> cell; // maze cell (x,y) value
    Graph::vertex_descriptor pred;
    bool visited;
    bool marked;
    int weight;
    int color;

    VertexProperties() : color(NO_COLOR) {}
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

    for (unsigned int counter = 0; counter < num_vertices(g); counter++)
    {
        cout << counter << ": " << g[counter].color << endl;

    }
}

int countUncoloredNodes(Graph &g) {
	int count = 0;

	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr) {
        if (g[*vItr].color == NO_COLOR) {
        	count++;
        }
    }

    return count;
}

bool allTrue(vector<bool> v) {
	for (unsigned int i = 0; i < v.size(); i++) {
		if (!v[i]) {
			return false;
		}
	}

	return true;
}

int findMaximumDegreeNode(Graph &g, int numColors) {
    int maxDegreeIndex = -1;
    int maxDegreeCount = -1;

    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr) {
        int currDegreeCount = 0;
        pair<Graph::adjacency_iterator, Graph::adjacency_iterator> adjItrRange = adjacent_vertices(*vItr, g);

        // this array keeps track of what neighboring colors have been occupied so far
        vector<bool> adjColors(numColors, false);


        for (Graph::adjacency_iterator adjItr = adjItrRange.first; adjItr != adjItrRange.second; adjItr++) {
            currDegreeCount++;

            // this keeps track that a neighbor has colored a specific color already
            if (g[*adjItr].color != NO_COLOR) {
            	adjColors[g[*adjItr].color] = true;
            }
        }

        // If the node has already been colored, or if the node cannot be colored because of its neighbors' colors, we pass
        if (g[*vItr].color != NO_COLOR || allTrue(adjColors)) {
        	continue;
        }

        if (currDegreeCount > maxDegreeCount) {
            maxDegreeIndex = *vItr;
            maxDegreeCount = currDegreeCount;
        }
    }

    return maxDegreeIndex;
}

int findNodeColor(Graph &g, int nodeIndex, int numColors) {
	// this array keeps track of what neighboring colors have been occupied so far
    vector<bool> adjColors(numColors, false);

    pair<Graph::adjacency_iterator, Graph::adjacency_iterator> adjItrRange = adjacent_vertices(nodeIndex, g);

    for (Graph::adjacency_iterator adjItr = adjItrRange.first; adjItr != adjItrRange.second; adjItr++) {
        // this keeps track that a neighbor has colored a specific color already
        if (g[*adjItr].color != NO_COLOR) {
        	adjColors[g[*adjItr].color] = true;
        }
    }

    vector<int> availableColors;

    for (int i = 0; i < adjColors.size(); i++) {
    	// keep track of the colors that haven't been used
    	if (!adjColors[i]) {
    		availableColors.push_back(i);
    	}
    }

    // i guess for now we can just return whatever color is first available
    return availableColors[0];
}

// takes the uncolored nodes with the highest degree and color them until no more nodes can be colored
int greedyColoring(Graph &g, int numColors, int t) {
    int leastConflicts = INT_MAX;

    while(findMaximumDegreeNode(g, numColors) != -1) {
	    int maxDegreeNodeIndex = findMaximumDegreeNode(g, numColors);
	    int maxDegreeNodeColor = findNodeColor(g, maxDegreeNodeIndex, numColors);
	    g[maxDegreeNodeIndex].color = maxDegreeNodeColor;
	}

	// for now, we don't color a node if it creates a conflict. the amount of uncolored nodes = numConflicts;
	leastConflicts = countUncoloredNodes(g);
	cout << "Number of conflicts: " << leastConflicts << endl;

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
        
        numConflicts = greedyColoring(g, numColors, 600);
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