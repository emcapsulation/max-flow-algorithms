#include <iostream>
#include <vector>
#include <unordered_map>
#include "max-flow-functions.h"


int main() {
	// Edmonds-Karp
	std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> nodes = {
		{"source", {{"A", 1}, {"B", 2}}},
		{"A", {{"B", 3}, {"sink", 1}}},
		{"B", {{"sink", 2}}},
		{"sink", {}}
	};
	int ans = edmondsKarp(nodes);
	std::cout << ans << std::endl;
	std::cout << "\n\n\n\n\n";

	nodes = {
		{"source", {{"1", 12}, {"2", 5}, {"3", 4}}},
		{"1", {{"sink", 9}}},
		{"2", {{"1", 6}, {"4", 3}}},
		{"3", {{"4", 7}}},
		{"4", {{"sink", 10}}},
		{"sink", {}}
	};
	ans = edmondsKarp(nodes);
	std::cout << ans << std::endl;
	std::cout << "\n\n\n\n\n";

	nodes = {
		{"source", {{"1", 4}, {"2", 4}}},
		{"1", {{"3", 6}, {"6", 5}}},
		{"2", {{"4", 7}}},
		{"3", {{"5", 3}}},
		{"4", {{"6", 3}}},
		{"5", {{"sink", 8}}},
		{"6", {{"sink", 4}}},
		{"sink", {}}
	};
	ans = edmondsKarp(nodes);
	std::cout << ans << std::endl;
	std::cout << "\n\n\n\n\n";
	

	// Baseball Elimination
	bool ans2 = baseballElimination(
		{"circle", "square", "triangle", "diamond"}, 
		{6, 4, 4, 2},
		{{0, 3, 1, 0}, {3, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 1, 0}},
		"diamond"
	);

	ans2 = baseballElimination(
		{"square", "pentagon", "triangle", "diamond"}, 
		{22, 20, 19, 19},
		{{0, 3, 2, 1}, {3, 0, 4, 0}, {2, 4, 0, 3}, {1, 0, 3, 0}},
		"diamond"
	);

	ans2 = baseballElimination(
		{"square", "pentagon", "triangle", "diamond", "hexagon"}, 
		{7, 5, 3, 2, 2},
		{{0, 2, 1, 1, 1}, {2, 0, 2, 1, 1}, {1, 2, 0, 1, 2}, {1, 1, 1, 0, 2}, {1, 1, 2, 2, 0}},
		"diamond"
	);
	std::cout << ans2 << std::endl;

	return 0;
}