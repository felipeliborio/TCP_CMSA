/*
Defines a class and types to represent instances and do the operations 
needed for them.
*/
#pragma once

#include <deque>
#include <map>

int constexpr DEFAULT_VALUE = -2147483647;

enum class EOperationStatus {
	Failed,
	Succeded,
	Cicle,
	Exists,
	Inexists,
};

struct FEdgeAttributes
{
	int Weight = DEFAULT_VALUE;
	int Age = 0;
};

struct FEdge {
	int Vertex1 = DEFAULT_VALUE;
	int Vertex2 = DEFAULT_VALUE;
	FEdgeAttributes Attributes;
};

using TInstData = std::deque<std::deque<int>>;
//using Edge = std::deque<int>;
using Graph_t = std::map<int, std::map<int, FEdgeAttributes>>;

class FInstance
{
public:
	FInstance();
	FInstance(TInstData const &InstData);
	~FInstance();

	EOperationStatus InsertEdge(std::deque<int> const & Edge, bool bCanCicle = true, bool bReplace = false);
	EOperationStatus InsertEdge(FEdge & Edge, bool bCanCicle = true, bool bReplace = false);
	EOperationStatus DefineTerminals(std::deque<int> const & Terminals, bool bCanReset = false);
	void RemoveEdge(std::deque<int> const & Edge);
	void RemoveEdge(FEdge & Edge);
	void SetEdgeAge(std::deque<int> const & Edge, int Age);
	void IncrementEdgeAge(std::deque<int> const & Edge, int const &Incremetation);
	std::deque<int> GetTerminals() const;
	std::deque<int> GetVertices() const;
	std::deque<FEdge> GetEdgesConectedToVertex(int const & Vertex);
	TInstData GetInstanceData();
	int GetVertexCount() const;
	Graph_t * GetGraphPointer();
	void Merge(FInstance & Instance, bool Update = false);
	int GetLength();
	int GetMaxRouters();
	void SetMaxRouters(int MaxRouters);
	int GetMaxLinks();
	void SetMaxLinks(int MaxLinks);
	int GetArcCount();
	std::deque<std::deque<int>> GetAdjacencyMatrix();

private:
	Graph_t Graph;
	std::deque<int> Terminals;
	int MaxRouters;
	int MaxLinks;
	bool ClosesCicle(std::deque<int> const & Edge);
	void RemoveArc(std::deque<int> const & Arc);
	void SetArcAge(std::deque<int> const & Arc, int Age);
	void IncrementArcAge(std::deque<int> const & Arc, int const & Incremetation);
};

