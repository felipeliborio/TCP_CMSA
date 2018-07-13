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

FInstance LNS(FInstance & Instance, FInstance & Solution, float const & Destruction) {
	FInstance BestSolution = Solution;
	FInstance NewSolution = Solution;
	std::deque<int> SolutionVertices = Solution.GetVertices();
	FInstance Neighborhood = FInstance();
	float DestructionDelta = 0;
	for (int i = 0; i < SolutionVertices.size(); i++) {
		if (DestructionDelta >= 1 - Destruction) {
			DestructionDelta = 0;
		}
		ExpandNeighborhood(Neighborhood, Instance, SolutionVertices, 0.4 + DestructionDelta);
		NewSolution.Merge(Neighborhood);
		NewSolution = MPrim(NewSolution, (float) (((rand() % 20) + 10) / 100));
		SimplifyInstance(NewSolution);
		if (NewSolution.GetLength() < BestSolution.GetLength()) {
			BestSolution = NewSolution;
		}
		else {
			NewSolution = BestSolution;
		}
		Neighborhood = FInstance();
		DestructionDelta += 0.1;
	}
	return BestSolution;
}

void ExpandNeighborhood (FInstance & Neighborhood, FInstance & Instance, std::deque<int> SolutionVertices, float const & Destruction) {
	while (Neighborhood.GetGraphPointer()->size() < SolutionVertices.size() * Destruction) {
		int Chosen = SolutionVertices[rand() % SolutionVertices.size()];
		std::deque<FEdge> EdgesToInsert = Instance.GetEdgesConectedToVertex(Chosen);
		for (auto Edge : EdgesToInsert) {
			Neighborhood.InsertEdge(Edge);
		}
	}
}
