# The Max Flow Problem

## Introduction
This repo contains the code for computing the maximum flow of a flow network using the Edmonds-Karp implementation of Ford-Fulkerson. It also has an application of the max flow problem called "The Baseball Elimination Problem".

I have made videos on these topics on my YouTube channel!
* Ford-Fulkerson and Edmonds-Karp: Finding the Maximum Flow
https://www.youtube.com/watch?v=0wMs9SCHL10
* Baseball Elimination Problem - How to Prove a Team is Eliminated with Max Flow
https://www.youtube.com/watch?v=vYFHNGGOlKE

## Running the Code
```
g++ edmonds-karp.cpp baseball-elimination.cpp max-flow-test.cpp -Wall -Werror -fsanitize=address -o max-flow-test
./max-flow-test
```

## Flow Network Properties
Flow networks consist of a source node where flow is emanating from, and a sink node where flow will reach. 

Nodes are connected with weighted directed edges. The edge weights specify the capacity of that edge, or the maximum flow value which can be sent through that edge.

Two key properties to consider:
* Capacity Limit: For all edges in the graph, the flow through an edge cannot exceed its capacity.
* Flow Conservation: For all nodes other than the source and sink, the flow entering the node must equal the flow leaving the node.

The total flow is equal to the sum of the flow values on edges leaving the source, which is equivalent to the sum of the flow values on edges entering the sink.

With these properties in mind, we ask ourselves - what is the maximum flow that can reach the sink?

## Ford-Fulkerson
The Ford-Fulkerson method of finding the maximum flow follows these steps:
1. While there are still paths from s to t, find a path from s to t.
2. Find the bottleneck capacity of this path, fb.
3. Push fb units of flow through the path. If the edge in the path is a forward edge, add fb units of flow to it. If the edge in the path is a backward edge, deduct fb units of flow from it.
4. Rebuild the residual graph. For each edge in the path, insert a back edge whose capacity is equal to the flow value of that edge.
5. When there are no more paths remaining, return the final flow as the maximum flow.

## Edmonds-Karp
Edmonds-Karp is an implementation of the Ford-Fulkerson method for finding the max flow. It uses a breadth-first search to find the augmenting paths from s to t. Its time complexity is O(m^2 n).

## Baseball Elimination Problem
The Baseball Elimination Problem is one application of finding the max flow in a flow network. Given a leaderboard of teams with the number of wins and remaining games between the teams, it aims to determine if a particular team still has a chance of winning the season.

This can be formulated as a max flow problem. Suppose you are checking whether team k is eliminated.

1. Connect the source to game vertices representing the games of other teams (i, j). The capacity of these edges is the number of remaining games between team i and j.
2. Connect these game vertices to the two team vertices of the teams in that game, i and j. Set the capacity of these edges to be infinity (don't restrict the flow here).
3. Connect the team vertices to the sink. Set the capacity to be the maximum number of wins this team (i) can get to not eliminate the team you are checking (k): wins of k + remaining games of k - wins of i.

Team k is eliminated if and only if the max flow is less than the number of games remaining between the other teams (i.e. the sum of the capacity of edges from the source node to the game nodes).
