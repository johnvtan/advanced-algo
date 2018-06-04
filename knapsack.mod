param n;
param costBound;
param value {i in 0..n-1} >= 0;
param cost {i in 0..n-1} >= 0;
var x {i in 0..n-1} binary;
maximize objective: sum {i in 0..n-1} x[i] * value[i];
subject to: sum {i in 0..n-1} x[i] * cost[i] <= costBound;
