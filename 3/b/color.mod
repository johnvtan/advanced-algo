param numNodes;
param numColors;
set edgeList dimen 2;
param edge {i in 0..numNodes-1, j in 0..numNodes-1} := 
    (if (i, j) in edgeList or (j, i) in edgeList then 1 else 0);

var colors {i in 0..numNodes-1, j in 0..numColors-1} binary;

minimize objective: 
    sum {i in 0..numNodes-1, j in 0..numNodes-1, c in 0..numColors-1} 
	edge[i, j] * (colors[i, c] + colors[j, c]);

subject to c1 {i in 0..numNodes-1}:
    sum {j in 0..numColors-1} colors[i,j] = 1; 
