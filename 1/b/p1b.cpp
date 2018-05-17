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

void incIntArray(unsigned int *array, int size) {
    if (NULL == array || 0 == size) {
        throw nullPtrError("Passed in null ptr or size 0 to incIntArray\n");
    }
    bool carry = false;
    array[0]++;
    for (int i = 0; i < size; i++) {
        if (carry == true) {
            array[i]++;
        }
        if (array[i] == 0) {
            carry = true;
        } else {
            break;
        }
    }
}

#define UINT_BITS (8 * sizeof(unsigned int))

bool checkIfDone(unsigned int *array, int size, int numBits) {
    unsigned int mask = UINT_MAX;
    for (int i = 0; i < size; i++) {
        if (numBits < UINT_BITS) {
            mask = (1 << numBits) - 1;
        }
        if (array[i] & mask) {
            return false;
        }
        numBits -= UINT_BITS;
    }
    return true;
}

int colorGraph(Graph &g, unsigned int *colorArray, int size, int colorBits, int maxColor) {
    int currColor = -1;
    int mask = (1 << colorBits) - 1;
    int leftover = 0;
    int i = 0;          // index into colorArray
    int nBits = 0;      // bit index into current int
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr) {
        currColor = (colorArray[i] >> nBits) & mask;
        leftover = UINT_BITS - nBits;
        if (leftover < colorBits) {
            // crossing integer boundary
            nBits = colorBits - leftover;
            currColor |= (colorArray[++i] << leftover) & mask;
        } else {
            nBits += colorBits;
        }
        if (currColor > maxColor) {
            return -1;
        }
        g[*vItr].color = currColor;
        cout << "Node (" << *vItr << ") color = " << currColor << endl;
    }
    return 0;
}

bool isValidColoring(Graph &g) {
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

    // check each node's color in the graph and its neighbor's colors
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr) {
        pair<Graph::adjaceny_iterator, Graph::adjaceny_iterator> adjItrRange = adjacent_vertices(g[*vItr], g);

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

int exhaustiveColoring(Graph &g, int numColors, int t) {
    int colorBits = ceil(log2(1.0*numColors));
    cout << "Color Bits: " << colorBits << endl;

    // this should be the highest value that we
    int totalBits = num_vertices(g) * colorBits;
    cout << "Total bits for graph: " << totalBits << endl;
    
    int numInts = totalBits % UINT_BITS ? totalBits / UINT_BITS + 1 : totalBits / UINT_BITS;
    cout << "Num ints needed: " << numInts << endl;

    int searchSpaceSize = numInts * sizeof(unsigned int);
    unsigned int *searchSpace = (unsigned int*)calloc(1, searchSpaceSize);
    unsigned int *bestSolution = (unsigned int*)calloc(1, searchSpaceSize);
    if (NULL == searchSpace || NULL == bestSolution) {
        cout << "Malloc failed!\n";
        exit(1);
    }  

    incIntArray(searchSpace, searchSpaceSize);
    while (!checkIfDone(searchSpace, searchSpaceSize, totalBits)) {
        // assign color to each node in graph
        // increment searchSpace
        // track the least number of conflicts
        cout << "Next coloring" << endl;
        if (0 == colorGraph(g, searchSpace, searchSpaceSize, colorBits, numColors-1)) {
            // check if legal color and update best solution
            cout << "colored" << endl;
        }
        incIntArray(searchSpace, searchSpaceSize);
    }
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