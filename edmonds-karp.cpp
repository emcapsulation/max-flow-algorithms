#include <iostream>
#include <vector>
#include <unordered_map>
#include <limits.h>
#include <queue>
#include <unordered_set>


/**
 * Backtracks along the path found by BFS and augments the flow.
 * 	If the edge in the path was a forward edge, it adds bottleneck units of flow.
 * 	If the edge in the path was a backward edge, it deducts bottleneck units of flow.
 * It then updates back edges by setting capacity equal to flow in the corresponding forward edge.
 * 
 * @param bottleneck The bottleneck capacity of the path, found by findBottleneck.
 * @param parents The map of each node in the path to its parent in the path.
 * @param nodes The map of nodes to the set of its neighbours
 * @param edges The map of edges (n1-n2) to {flow, capacity}
 * @param backEdges The map of back edges (n1-n2) to {flow, capacity}
 * 
 * @return Void
 */
void correctFlows(int bottleneck,
	std::unordered_map<std::string, std::string> &parents,
	std::unordered_map<std::string, std::unordered_set<std::string>> &nodes,
	std::unordered_map<std::string, int[2]> &edges,
	std::unordered_map<std::string, int[2]> &backEdges) {

	std::string cur = "sink";
	while (cur != "source") {

		std::string edgeKey = parents[cur]+'-'+cur;
		std::string backEdgeKey = cur+'-'+parents[cur];

		if (edges.find(edgeKey) != edges.end() && edges[edgeKey][0] < edges[edgeKey][1]) {

			// We took a forward edge, add bottleneck flow
			edges[edgeKey][0] += bottleneck;

			// Back edge has capacity updated to be flow of forward edge
			backEdges[backEdgeKey][0] = 0;
			backEdges[backEdgeKey][1] = edges[edgeKey][0];

		} else if (backEdges.find(edgeKey) != backEdges.end() && backEdges[edgeKey][0] < backEdges[edgeKey][1]) {

			// We took a backward edge, deduct from the corresponding forward edge
			edges[backEdgeKey][0] -= bottleneck;

			// Back edge has capacity updated to be flow of forward edge
			backEdges[edgeKey][0] = 0;
			backEdges[edgeKey][1] = edges[backEdgeKey][0];
		}		

		// Insert the parent as a neighbour of the current node since we added a back edge
		nodes[cur].insert(parents[cur]);

		cur = parents[cur];
	}
}


/**
 * Backtracks along the path found in BFS and computes the bottleneck capacity.
 * 
 * @param parents The map of each node in the path to its parent in the path.
 * @param edges The map of edges (n1-n2) to {flow, capacity}
 * @param backEdges The map of back edges (n1-n2) to {flow, capacity}
 * 
 * @return Bottleneck capacity of the path.
 */
int findBottleneck(std::unordered_map<std::string, std::string> &parents,
	std::unordered_map<std::string, int[2]> &edges,
	std::unordered_map<std::string, int[2]> &backEdges) {

	int bottleneck = INT_MAX;
	std::string cur = "sink";

	while (cur != "source") {
		std::string edgeKey = parents[cur]+'-'+cur;

		// Flow which can be pushed through an edge is capacity (1) - current flow (0)
		if (edges.find(edgeKey) != edges.end() && edges[edgeKey][0] < edges[edgeKey][1]) {
			bottleneck = std::min(bottleneck, edges[edgeKey][1] - edges[edgeKey][0]);
		
		} else if (backEdges.find(edgeKey) != backEdges.end() && backEdges[edgeKey][0] < backEdges[edgeKey][1]) {
			bottleneck = std::min(bottleneck, backEdges[edgeKey][1] - backEdges[edgeKey][0]);
		}
		cur = parents[cur];
	}

	return bottleneck;
}


/**
 * Breadth-first search of the graph from the source node. 
 * Terminates when it finds the sink (or search was exhausted).
 * 
 * @param nodes The map of nodes to the set of its neighbours
 * @param edges The map of edges (n1-n2) to {flow, capacity}
 * @param backEdges The map of back edges (n1-n2) to {flow, capacity}
 * 
 * @return Augmenting flow of the path found. 0 if no path was found to the sink.
 */
int BFS(std::unordered_map<std::string, std::unordered_set<std::string>> &nodes,
	std::unordered_map<std::string, int[2]> &edges,
	std::unordered_map<std::string, int[2]> &backEdges) {

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

		// Search complete
		if (cur == "sink") {
			completed = true;
			break;
		}

		// Loop through neighbours of this node
		for (auto &p : nodes[cur]) {
			std::string neighbour = p;

			if (visited.find(neighbour) == visited.end()) {						
				std::string edgeKey = cur+'-'+neighbour;

				if ((edges.find(edgeKey) != edges.end() && edges[edgeKey][0] < edges[edgeKey][1]) || 
					(backEdges.find(edgeKey) != backEdges.end() && backEdges[edgeKey][0] < backEdges[edgeKey][1])) {

					// Push neighbour to queue if there is an edge from this node to the neighbour
					// and it has room for flow to be pushed through it (current flow < capacity)
					q.push(neighbour);
					parents[neighbour] = cur;	
					visited.insert(neighbour);
				}	
			}
		}
	}

	// Return 0 flow if no path to the sink was found
	if (!completed) {
		return 0;
	}

	// Find the bottleneck
	int bottleneck = findBottleneck(parents, edges, backEdges);

	// Correct the flows
	correctFlows(bottleneck, parents, nodes, edges, backEdges);

	return bottleneck;
}


/**
 * The Edmonds-Karp implementation of Ford-Fulkerson for finding the maximum flow.
 * 
 * @param nodeInput Input graph as a map of nodes to a vector of neighbours in the format {name, capacity}
 * 
 * @return Max flow
 */
int edmondsKarp(std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> nodeInput) {
	// Map edge n1-n2 to [current flow, capacity]
	std::unordered_map<std::string, int[2]> edges;
	std::unordered_map<std::string, int[2]> backEdges;

	// Map nodes to a set of their neighbours
	std::unordered_map<std::string, std::unordered_set<std::string>> nodes;

	// Create edges
	for (auto &p : nodeInput) {
		std::string nodeKey = p.first;
		std::vector<std::pair<std::string, int>> neighbours = p.second;

		for (size_t i = 0; i < neighbours.size(); i++) {
			// Insert the neighbour
			nodes[nodeKey].insert(neighbours[i].first);

			// Add the edge, give it 0 initial flow
			std::string edgeKey = p.first + '-' + neighbours[i].first;
			edges[edgeKey][0] = 0;
			edges[edgeKey][1] = neighbours[i].second;
		}
	}


	int maxFlow = 0, newFlow = 0;
	while ((newFlow = BFS(nodes, edges, backEdges)) > 0) {
		maxFlow += newFlow;
	}

	for (auto &p : edges) {
		std::cout << "Edge: " << p.first << "  Flow: " << p.second[0] << "  Capacity: " << p.second[1] << '\n';
	}
	std::cout << "\nMax flow: " << maxFlow << '\n';


	return maxFlow;
}