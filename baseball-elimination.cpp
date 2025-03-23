#include <iostream>
#include <vector>
#include <unordered_map>
#include <limits.h>
#include "max-flow-functions.h"


bool baseballElimination(
	std::vector<std::string> teams,
	std::vector<int> wins,
	std::vector<std::vector<int>> games,
	std::string check
	)
{
	int n = teams.size();

	// Get the index of the team we are checking in the teams list
	int checkIndex = 0;
	for (int i = 0; i < n; i++) {
		if (teams[i] == check) {
			checkIndex = i;
		}
	}

	// Get the number of remaining games for the team we are checking
	int checkRemaining = 0;
	for (int i = 0; i < n; i++) {
		checkRemaining += games[checkIndex][i];
	}

	// See if the team is trivially eliminated
	for (int i = 0; i < n; i++) {
		if (i != checkIndex) {
			if (wins[checkIndex] + checkRemaining < wins[i]) {
				std::cout << check << " is trivially eliminated.\n\n\n";
				return false;
			}
		}
	}


	// Map a node to its list of neighbours
	// Each element in the list has:
	// 0: The neighbour key
	// 1: The edge weight to that neighbour
	std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> nodes;

	// Insert source and sink
	nodes["source"];
	nodes["sink"];

	for (int i = 0; i < n; i++) {
		for (int j = i+1; j < n; j++) {
			if (teams[i] != check && teams[j] != check) {
				// Create game vertex
				std::string key = teams[i] + '|' + teams[j];
				nodes[key];

				// Capacity from source to game node is the # of
				// games remaining between i and j
				nodes["source"].push_back({key, games[i][j]});

				// Create team vertices
				if (nodes.find(teams[i]) == nodes.end()) {
					nodes[teams[i]];
				}
				nodes[key].push_back({teams[i], INT_MAX});

				if (nodes.find(teams[j]) == nodes.end()) {
					nodes[teams[j]];
				}
				nodes[key].push_back({teams[j], INT_MAX});
			}
		}

		// Capacity from team node to sink
		// wins[check] - remaining[check] + wins[i]
		if (teams[i] != check) {
			nodes[teams[i]].push_back({"sink", wins[checkIndex]+checkRemaining-wins[i]});
		}
	}


	// Calculate the max flow
	int maxFlow = edmondsKarp(nodes);
	std::cout << "\nMax flow: " << maxFlow << '\n';

	// Get the number of games remaining amongst other teams
	int numGamesRemaining = 0;
	for (size_t i = 0; i < nodes["source"].size(); i++) {
		numGamesRemaining += nodes["source"][i].second;
	}
	std::cout << "Num games remaining: " << numGamesRemaining << '\n';

	if (maxFlow < numGamesRemaining) {
		std::cout << "Team cannot win.\n\n\n";
		return false;
	}

	std::cout << "Team can win.\n\n\n";
	return true;
}