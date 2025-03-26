#include <iostream>
#include <vector>
#include <unordered_map>
#include <limits.h>
#include "max-flow-functions.h"


/**
 * Given a list of teams and their current wins, and their remaining games against other teams,
 * computes whether they have a chance of winning (most wins) after all remaining games are played out.
 * 
 * @param teams The list of team names.
 * @param wins The number of wins of each team so far in the season.
 * @param games games[i][j] gives the number of remaining games i and j have to play against each other.
 * @param ourTeam The name of the team we are checking to see if it is eliminated.
 * 
 * @return True if team ourTeam could win the season. False if they are eliminated.
 */
bool baseballElimination(std::vector<std::string> teams,
	std::vector<int> wins,
	std::vector<std::vector<int>> games,
	std::string ourTeam) {

	int n = teams.size();

	// Get the index of the team we are checking in the teams list
	int ourTeamIndex = 0;
	for (int i = 0; i < n; i++) {
		if (teams[i] == ourTeam) {
			ourTeamIndex = i;
		}
	}

	// Get the number of remaining games for the team we are checking
	int ourTeamRemaining = 0;
	for (int i = 0; i < n; i++) {
		ourTeamRemaining += games[ourTeamIndex][i];
	}

	// See if the team is trivially eliminated
	// This occurs when our team's wins plus their remaining games is less than some other teams wins so far
	// Meaning our team can never catch them
	for (int i = 0; i < n; i++) {
		if (i != ourTeamIndex && wins[ourTeamIndex] + ourTeamRemaining < wins[i]) {
			std::cout << "Team is trivially eliminated\n";
			return false;
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
			if (teams[i] != ourTeam && teams[j] != ourTeam) {
				// Create game vertex
				std::string key = teams[i] + '|' + teams[j];
				nodes[key];

				// Capacity from source to game node is the number of games left between i and j
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

		// Capacity from team node i to sink
		// wins[ourTeam] - remaining[ourTeam] + wins[i]
		if (teams[i] != ourTeam) {
			nodes[teams[i]].push_back({"sink", wins[ourTeamIndex]+ourTeamRemaining-wins[i]});
		}
	}


	// Calculate the max flow
	int maxFlow = edmondsKarp(nodes);

	// Get the number of games remaining amongst other teams
	int numGamesRemaining = 0;
	for (size_t i = 0; i < nodes["source"].size(); i++) {
		numGamesRemaining += nodes["source"][i].second;
	}
	std::cout << "Number of games remaining between other teams: " << numGamesRemaining << '\n';

	if (maxFlow < numGamesRemaining) {
		std::cout << "Team cannot win\n";
		return false;
	}

	std::cout << "Team can win\n";
	return true;
}