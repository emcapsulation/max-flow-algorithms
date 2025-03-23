#include <iostream>
#include <vector>
#include <unordered_map>
#include <limits.h>
#include <queue>
#include <unordered_set>


int BFS(std::unordered_map<std::string, std::unordered_set<std::string>> &nodes,
	std::unordered_map<std::string, int[2]> &edges,
	std::unordered_map<std::string, int[2]> &backEdges) {

	std::cout << "\nPERFORMING BFS\n";

	// Keep track of parents
	std::unordered_map<std::string, std::string> parents;

	// Keep track of visited nodes
	std::unordered_set<std::string> visited;

	std::queue<std::string> q;
	q.push("source");
	visited.insert("source");

	bool completed = false;
	while (!q.empty()) {
		std::string cur = q.front();
		q.pop();

		if (cur == "sink") {
			std::cout << "Sink found (stopping)\n"; 
			completed = true;
			break;
		}

		for (auto &p : nodes[cur]) {
			std::string neighbour = p;

			if (visited.find(neighbour) == visited.end()) {						
				std::string edgeKey = cur+'-'+neighbour;

				if ((edges.find(edgeKey) != edges.end() && 
					edges[edgeKey][0] < edges[edgeKey][1]) || 
					(backEdges.find(edgeKey) != backEdges.end() && 
					backEdges[edgeKey][0] < backEdges[edgeKey][1])) {

					q.push(neighbour);
					parents[neighbour] = cur;	
					visited.insert(neighbour);
				}	
			}
		}
	}

	// Return 0 flow if no path was found
	if (!completed) {
		std::cout << "No more paths found.\n";
		return 0;
	}

	// Find the bottleneck
	std::cout << "\nFINDING BOTTLENECK\n";
	int bottleneck = INT_MAX;
	std::string cur = "sink";
	while (cur != "source") {
		std::string edgeKey = parents[cur]+'-'+cur;

		if (edges.find(edgeKey) != edges.end() && 
			edges[edgeKey][0] < edges[edgeKey][1]) {

			std::cout << edgeKey << ": " << edges[edgeKey][1] - edges[edgeKey][0] << '\n';
			bottleneck = std::min(bottleneck, edges[edgeKey][1] - edges[edgeKey][0]);
		
		} else if (backEdges.find(edgeKey) != backEdges.end() && 
			backEdges[edgeKey][0] < backEdges[edgeKey][1]) {

			std::cout << "Back edge: " << edgeKey << ": " << backEdges[edgeKey][1] - backEdges[edgeKey][0] << '\n';
			bottleneck = std::min(bottleneck, backEdges[edgeKey][1] - backEdges[edgeKey][0]);
		}
		cur = parents[cur];
	}
	std::cout << "BOTTLENECK: " << bottleneck << '\n';

	// Correct the flows
	std::cout << "\nCORRECTING FLOWS\n";
	cur = "sink";
	while (cur != "source") {
		std::string edgeKey = parents[cur]+'-'+cur;
		std::string backEdgeKey = cur+'-'+parents[cur];

		if (edges.find(edgeKey) != edges.end() && 
			edges[edgeKey][0] < edges[edgeKey][1]) {

			edges[edgeKey][0] += bottleneck;
			std::cout << "Edge: " << edgeKey << " Flow: " << edges[edgeKey][0] << " Capacity: " << edges[edgeKey][1] << '\n';

			// Back edge has capacity updated to be flow of forward edge
			backEdges[backEdgeKey][0] = 0;
			backEdges[backEdgeKey][1] = edges[edgeKey][0];
			std::cout << "Back edge: " << backEdgeKey << " Flow: " << backEdges[backEdgeKey][0] << " Capacity: " << backEdges[backEdgeKey][1] << '\n';

		} else if (backEdges.find(edgeKey) != backEdges.end() && 
			backEdges[edgeKey][0] < backEdges[edgeKey][1]) {

			// Deduct from the corresponding forward edge
			edges[backEdgeKey][0] -= bottleneck;
			std::cout << "Edge: " << backEdgeKey << " Flow: " << edges[backEdgeKey][0] << " Capacity: " << edges[backEdgeKey][1] << '\n';

			// Back edge has capacity updated to be flow of forward edge
			backEdges[edgeKey][0] = 0;
			backEdges[edgeKey][1] = edges[backEdgeKey][0];
			std::cout << "Back edge: " << edgeKey << " Flow: " << backEdges[edgeKey][0] << " Capacity: " << backEdges[edgeKey][1] << '\n';
		}		

		// Insert the parent as a neighbour
		nodes[cur].insert(parents[cur]);

		cur = parents[cur];
	}

	return bottleneck;
}


// Returns (int): Returns the max flow
//
// nodes: Map each node key (string) to a vector of neighbours
//        Each pair element of the neighbour vector contains:
//        - Key of the neighbour node (string)
//        - Capacity of the edge (int)
int edmondsKarp(std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> nodeInput) {
	// Map edge n1-n2 to [current flow, capacity]
	std::unordered_map<std::string, int[2]> edges;
	std::unordered_map<std::string, int[2]> backEdges;

	// Map nodes to a set of their neighbours
	std::unordered_map<std::string, std::unordered_set<std::string>> nodes;

	// Create edges
	std::cout << "CREATING EDGES\n";
	for (auto &p : nodeInput) {
		std::string nodeKey = p.first;
		std::vector<std::pair<std::string, int>> neighbours = p.second;

		for (size_t i = 0; i < neighbours.size(); i++) {
			// Insert the neighbour
			nodes[nodeKey].insert(neighbours[i].first);

			// Add the edge
			std::string edgeKey = p.first + '-' + neighbours[i].first;
			edges[edgeKey][0] = 0;
			edges[edgeKey][1] = neighbours[i].second;
			std::cout << edgeKey << ' ' << neighbours[i].second << '\n';
		}
	}


	int maxFlow = 0;

	int newFlow = BFS(nodes, edges, backEdges);
	while (newFlow > 0) {
		std::cout << "\nNEW FLOW: " << newFlow << '\n';
		maxFlow += newFlow;
		newFlow = BFS(nodes, edges, backEdges);
	}

	std::cout << "\nMAX FLOW: " << maxFlow << "\n\n";


	for (auto &p : edges) {
		std::cout << "Edge: " << p.first << " Flow: " << p.second[0] << " Capacity: " << p.second[1] << '\n';
	}


	return maxFlow;
}