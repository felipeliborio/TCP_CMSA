#pragma once
#include "CMSA.h"
#include <time.h>
#include <deque>

#define OUT

void Adapt(FInstance& ReducedInstance, FInstance& BestSoFar, int const & AgeMax);
void SimplifyInstance(FInstance& Instance);
FInstance ApplyExactSolver(FInstance & Instance);

FInstance CMSA(FInstance & Instance, int PrimExecutions, int AgeMax, float const Alpha)
{
	SimplifyInstance(Instance);
	FInstance BestSoFar, ReducedInstance, TempInstance;
	BestSoFar = Instance;
	ReducedInstance.DefineTerminals(Instance.GetTerminals());
	double EndTime = (double)time(NULL) + Instance.GetVertexCount();
	int UnenhancendIt = 0;
	int const MaxUnenhanced = (int) (pow((double) Instance.GetTerminals().size(), (1.0/3))) + 10;
	while (time(NULL) < EndTime && UnenhancendIt < MaxUnenhanced) {
		for (int Iteration = 0; Iteration < PrimExecutions; ++Iteration) {
			TempInstance = Instance;
			FInstance PrimSolution = MPrim(OUT TempInstance, Alpha);
			ReducedInstance.Merge(OUT PrimSolution);
		}
		SimplifyInstance(ReducedInstance);
		TempInstance = ReducedInstance;
		FInstance SubInstOptimalSolution = ApplyExactSolver(OUT TempInstance);
		if (BestSoFar.GetLength() > SubInstOptimalSolution.GetLength()) {
			BestSoFar = SubInstOptimalSolution;
			UnenhancendIt = 0;
		}
		else {
			++UnenhancendIt;
		}
		Adapt(ReducedInstance, SubInstOptimalSolution, AgeMax);
	}
	return BestSoFar;
}

FInstance ApplyExactSolver(FInstance & Instance) {
	//TODO use DW algorithm or CPLEX
	FInstance Output = MPrim(OUT Instance, 0);
	SimplifyInstance(Output);
	return Output;
}

void Adapt(FInstance & ReducedInstance, FInstance & BestSoFar, int const & AgeMax)
{
	Graph_t* RI_Graph= ReducedInstance.GetGraphPointer();
	Graph_t* BSF_Graph = BestSoFar.GetGraphPointer();
	for (auto& Vertex : (*RI_Graph)) {
		for (auto& DVertex : (*RI_Graph)[Vertex.first]) {//Dividing not to create new vertices
			if ((*BSF_Graph).find(Vertex.first) == (*BSF_Graph).end()) {
				++(*RI_Graph)[Vertex.first][DVertex.first].Age;
			}
			else if((*BSF_Graph)[Vertex.first].find(DVertex.first) == 
				(*BSF_Graph)[Vertex.first].end()){
				++(*RI_Graph)[Vertex.first][DVertex.first].Age;
			}
			else {
				(*RI_Graph)[Vertex.first][DVertex.first].Age = 0;
			}
		}
	}
	return;
}

template <typename T>
bool IsInDeque(T Item, std::deque<T>& Deque)
{//REVIEW consider sorting terminals to use binary search
	for (auto& Element : Deque) {
		if (Element == Item) {
			return true;
		}
	}
	return false;
}

void SimplifyInstance(FInstance & Instance)
{
	Graph_t* Graph_ptr = Instance.GetGraphPointer();
	std::deque<int> Terminals = Instance.GetTerminals();
	bool bDone = false;
	while (!bDone) {
		bDone = true;
		for (auto& Vertex1 : *Graph_ptr) {
			if ((*Graph_ptr)[Vertex1.first].size() == 1 && !IsInDeque(Vertex1.first, Terminals)) {
				Instance.RemoveEdge({ Vertex1.first, Vertex1.second.begin()->first });
				bDone = false;
				break;
			}
		}
	}
}
