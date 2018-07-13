#pragma once

#include "MPrim.h"
#include <deque>
#include <time.h>
#include <set>
#include <map>

//REVIEW The AvailableEdges size seems off by a huge amount. Check why.

using EdgeMap = std::map<int, std::deque<FEdge>>;//weight to edge deque map

void AddAvailableEdges(std::deque<FEdge>& NewEdges, EdgeMap& EdgeMap);
FEdge GetAlphaEdge(float const & Alpha, EdgeMap& AvailableEdges);
void InsertEdge(FEdge& NewEdge, EdgeMap& EdgeMap);
void RemoveAvailableEdge(FEdge& Edge, EdgeMap& EdgeMap);

template <typename T>
void DeleteDequeItem(T const & Item, std::deque<T>& Deque);

void VisitVertex(int& Vertex, std::deque<int>& UnreachedTerminals, FInstance& Graph, 
	EdgeMap& AvailabeEdges, std::map<int, bool>& VisitedVertices);

FInstance MPrim(FInstance & Graph, float const & Alpha)
{
	srand((int) time(NULL));
	FInstance Output;
	std::deque<int> UnreachedTerminals(Graph.GetTerminals());
	Output.DefineTerminals(UnreachedTerminals);
	std::map<int, bool> VisitedVertices;
	EdgeMap AvailabeEdges;
	int Origin = UnreachedTerminals[rand() % (int) UnreachedTerminals.size()];
	VisitVertex(Origin, UnreachedTerminals, Graph, AvailabeEdges, VisitedVertices);
	while (UnreachedTerminals.size() > 0) {
		for (auto& Vertex : *Output.GetGraphPointer()) {
			if (!VisitedVertices[Vertex.first]) {
				int V = Vertex.first;
				VisitVertex(V, UnreachedTerminals, Graph, AvailabeEdges, VisitedVertices);
			}
		}
		if (UnreachedTerminals.size() > 0) {
			if (AvailabeEdges.size() > 0) {
				EOperationStatus Inserted = EOperationStatus::Failed;
				do {
					FEdge AlphaEdge = GetAlphaEdge(Alpha, AvailabeEdges);
					Inserted = Output.InsertEdge(AlphaEdge, false, false);
					RemoveAvailableEdge(AlphaEdge, AvailabeEdges);
				} while (Inserted != EOperationStatus::Succeded && AvailabeEdges.size() > 0);
			}
			else {
				return FInstance();
			}
		}
	}
	return Output;
}

void VisitVertex(int& Vertex, std::deque<int>& UnreachedTerminals, FInstance& Graph,
	EdgeMap& AvailabeEdges, std::map<int, bool>& VisitedVertices)
{
	DeleteDequeItem(Vertex, UnreachedTerminals);
	std::deque<FEdge> NewEdges = Graph.GetEdgesConnectedToVertex(Vertex);
	AddAvailableEdges(NewEdges, AvailabeEdges);
	for (auto& Edge : NewEdges) {
		Graph.RemoveEdge(Edge);
	}
	VisitedVertices[Vertex] = true;
}

template <typename T>
void DeleteDequeItem(T const & Item, std::deque<T>& Deque)
{
	for (auto It = Deque.begin(); It != Deque.end(); ++It) {
		if ((*It) == Item) {
			Deque.erase(It);
			break;
		}
	}
}

void AddAvailableEdges(std::deque<FEdge> & NewEdges, EdgeMap & EdgeMap)
{
	for (auto& Edge : NewEdges) {
		InsertEdge(Edge, EdgeMap);
	}
	return;
}

void InsertEdge(FEdge & NewEdge, EdgeMap & EdgeMap)
{
	FEdge BEdge;
	BEdge.Vertex1 = NewEdge.Vertex2;
	BEdge.Vertex2 = NewEdge.Vertex1;
	BEdge.Attributes = NewEdge.Attributes;
	for (FEdge& Edge : EdgeMap[NewEdge.Attributes.Weight]) {
		if (NewEdge.Vertex1 == Edge.Vertex1 &&
			NewEdge.Vertex2 == Edge.Vertex2) {
			return;
		}
	}
	EdgeMap[NewEdge.Attributes.Weight].push_back(NewEdge);
	EdgeMap[NewEdge.Attributes.Weight].push_back(BEdge);
	return;
}

void DeleteArc(FEdge const & Edge, EdgeMap & EdgeMap)
{
	std::deque<FEdge>& Subset = EdgeMap[Edge.Attributes.Weight];
	std::deque<FEdge>::iterator It;
	for (It = Subset.begin(); It != Subset.end(); ++It) {
		if (Edge.Vertex1 == It->Vertex1 &&
			Edge.Vertex2 == It->Vertex2) {
			Subset.erase(It);
			break;
		}
	}
	if (EdgeMap[Edge.Attributes.Weight].size() == 0) {
		EdgeMap.erase(EdgeMap.find(Edge.Attributes.Weight));
	}
	return;
}

void RemoveAvailableEdge(FEdge & Edge, EdgeMap & EdgeMap)
{
	DeleteArc(Edge, EdgeMap);
	FEdge BEdge;
	BEdge.Vertex1 = Edge.Vertex2;
	BEdge.Vertex2 = Edge.Vertex1;
	BEdge.Attributes = Edge.Attributes;
	DeleteArc(BEdge, EdgeMap);
	return;
}

FEdge GetAlphaEdge(float const & Alpha, EdgeMap & AvailableEdges)
{
	auto It = AvailableEdges.begin();
	int Key = (int) (It->first + Alpha * ((--AvailableEdges.end())->first - It->first));
	std::deque<int> AvailableKeys;
	for (; It != AvailableEdges.upper_bound(Key); ++It) {
		AvailableKeys.push_back(It->first);
	}
	Key = AvailableKeys[rand() % AvailableKeys.size()];
	std::deque<FEdge>& ChosenSubset = AvailableEdges[Key];
	Key = rand() % ChosenSubset.size();
	return ChosenSubset[Key];
}
