#pragma once

#include "TCP_LNS.h"
#include <iostream>
#include <deque>
#include <time.h>
#include <set>
#include <map>
#include "MPrim.h"
#include "CMSA.h"

void ExpandNeighborhood(FInstance & Neighborhood, FInstance & Instance, std::deque<int> SolutionVertices, float const & Destruction);

FInstance LNS(FInstance & Instance, FInstance & Solution) {
	FInstance BestSolution = Solution;
	FInstance NewSolution = Solution;
	std::deque<int> SolutionVertices = Solution.GetVertices();
	FInstance Neighborhood = FInstance();
	for (int i = 0; i < Instance.GetTerminalsPointer()->size() + 2000; i++) {
		ExpandNeighborhood(Neighborhood, Instance, SolutionVertices, (float)(((rand() % 16) + 3) / 20.0));
		NewSolution.Merge(Neighborhood);
		NewSolution = MPrim(NewSolution, (float) (((rand() % 24)) / 30.0));
		if (NewSolution.GetLength() < BestSolution.GetLength()) {
			BestSolution = NewSolution;
		}
		else {
			NewSolution = BestSolution;
		}
		Neighborhood = FInstance();
	}
	return BestSolution;
}

void ExpandNeighborhood (FInstance & Neighborhood, FInstance & Instance, std::deque<int> SolutionVertices, float const & Destruction) {
	while (Neighborhood.GetGraphPointer()->size() < SolutionVertices.size() * Destruction) {
		int Chosen = SolutionVertices[rand() % SolutionVertices.size()];
		std::deque<FEdge> EdgesToInsert = Instance.GetEdgesConnectedToVertex(Chosen);
		for (auto Edge : EdgesToInsert) {
			Neighborhood.InsertEdge(Edge);
		}
	}
}
