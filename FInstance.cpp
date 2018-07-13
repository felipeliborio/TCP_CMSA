#include "FInstance.h"

FInstance::FInstance()
{
}

FInstance::FInstance(TInstData const & InstData)
{
	int i;
	for (i = 1; i < InstData.size(); i++) {
		if (InstData[i].size() > 1) {
			this->InsertEdge(InstData[i]);
		}
		else {
			++i;
			break;
		}
	}
	for (; i < InstData.size(); i++) {
		for (auto Terminal : InstData[i]) {
			this->Terminals.push_back(Terminal);
		}
	}
}

FInstance::~FInstance()
{
	this->Graph.clear();
	this->Terminals.clear();
}

//TODO rewrite how it is checked using find()
//Insert an edge if it is not already in the graph or can be replaced, both ways
EOperationStatus FInstance::InsertEdge(std::deque<int> const & Edge,
	bool bCanCicle, bool bReplace)
{
	if (!bCanCicle && ClosesCicle(Edge)) {
		this->RemoveEdge(Edge);
		return EOperationStatus::Cicle;
	}
	if (!bReplace) {
		if (Graph[Edge[0]][Edge[1]].Weight != DEFAULT_VALUE &&
			Graph[Edge[1]][Edge[0]].Weight != DEFAULT_VALUE) {
			return EOperationStatus::Exists;
		}
	}
	FEdgeAttributes Att;
	Att.Weight = Edge[2];
	if (Edge.size() == 4) {
		Att.Age = Edge[3];
	}
	else {
		Att.Age = 0;
	}
	Graph[Edge[0]][Edge[1]] = Att;
	Graph[Edge[1]][Edge[0]] = Att;
	return EOperationStatus::Succeded;
}

EOperationStatus FInstance::InsertEdge(FEdge & Edge, bool bCanCicle, bool bReplace)
{
	return this->InsertEdge({ Edge.Vertex1, Edge.Vertex2, Edge.Attributes.Weight,
		Edge.Attributes.Age}, bCanCicle, bReplace);
}

//Remove an arc from the graph
void FInstance::RemoveArc(std::deque<int> const & Arc)
{
	this->Graph[Arc[0]].erase(Arc[1]);
	if (this->Graph[Arc[0]].size() == 0) {
		this->Graph[Arc[0]].clear();
		this->Graph.erase(Arc[0]);
	}
	return;
}

//Remove an edge from the graph, both ways
void FInstance::RemoveEdge(std::deque<int> const & Edge)
{//REVIEW aparently working, may be a problem in the future
	this->RemoveArc(Edge);
	this->RemoveArc({ Edge[1], Edge[0] });
	return;
}

void FInstance::RemoveEdge(FEdge & Edge)
{
	this->RemoveEdge({ Edge.Vertex1, Edge.Vertex2});
	return;
}

//(Re)Define the terminals
EOperationStatus FInstance::DefineTerminals(std::deque<int> const & Terminals, bool bCanReset)
{
	if (!bCanReset) {
		if (this->Terminals.size() > 0) {
			return EOperationStatus::Exists;
		}
	}
	this->Terminals = Terminals;
	return EOperationStatus::Succeded;
}

void FInstance::SetArcAge(std::deque<int> const & Arc, int Age)
{
	this->Graph[Arc[0]][Arc[1]].Age = Age;
	return;
}

void FInstance::SetEdgeAge(std::deque<int> const & Edge, int Age)
{
	this->SetArcAge(Edge, Age);
	this->SetArcAge({ Edge[1], Edge[0] }, Age);
	return;
}

void FInstance::IncrementArcAge(std::deque<int> const & Arc, int const & Incremetation)
{
	this->SetArcAge(Arc, this->Graph[Arc[0]][Arc[1]].Age + Incremetation);
	return;
}

void FInstance::IncrementEdgeAge(std::deque<int> const & Edge, int const & Incremetation)
{
	this->SetEdgeAge(Edge, this->Graph[Edge[0]][Edge[1]].Age + Incremetation);
	return;
}

std::deque<int> FInstance::GetTerminals() const
{
	return this->Terminals;
}

std::deque<int> FInstance::GetVertices() const
{
	std::deque<int> Output;
	for (auto Vertex : this->Graph) {
		Output.push_back(Vertex.first);
	}
	return Output;
}

//Return a deque with the edges connected to a vertex
std::deque<FEdge> FInstance::GetEdgesConnectedToVertex(int const & Vertex)
{//TODO this doesn't work that well
	std::deque<FEdge> Output;
	FEdge Edge;
	for (auto CVertex : this->Graph[Vertex]) {
		Edge.Vertex1 = Vertex;
		Edge.Vertex2 = CVertex.first;
		Edge.Attributes = CVertex.second;
		Output.push_back(Edge);
	}
	return Output;
}

//Returns instace data, same manner as input data but ordered
TInstData FInstance::GetInstanceData()
{
	TInstData Output;
	int VertexCount = (int) this->Graph.size();
	int EdgeCount = 0;
	Output.push_back({ VertexCount, EdgeCount });
	std::map<int, std::map<int, bool>> Inserted;
	for (auto Vertex : this->Graph) {
		EdgeCount += (int) this->Graph[Vertex.first].size();
		for (auto CVertex : this->Graph[Vertex.first]) {
			if (!Inserted[CVertex.first][Vertex.first]) {
				Output.push_back({ Vertex.first, CVertex.first, CVertex.second.Weight });
				Inserted[Vertex.first][CVertex.first] = true;
			}
		}
	}
	EdgeCount /= 2;
	Output[0][1] = EdgeCount;
	Output.push_back({ (int) this->Terminals.size() });
	Output.push_back(this->Terminals);
	return Output;
}

int FInstance::GetVertexCount() const
{
	return (int) this->Graph.size();
}

Graph_t * FInstance::GetGraphPointer()
{
	return &Graph;
}

//Add another instance to the calling one, keeping old existing edges or updating them
void FInstance::Merge(FInstance & Instance, bool Update)
{
	Graph_t* ExternGraph = Instance.GetGraphPointer();
	for (auto Vertex : *ExternGraph) {
		for (auto DVertex : (*ExternGraph)[Vertex.first]) {
			if (Update && this->Graph[Vertex.first][DVertex.first].Weight != DEFAULT_VALUE) {
				(*ExternGraph)[Vertex.first][DVertex.first].Age = this->Graph[Vertex.first][DVertex.first].Age;
				(*ExternGraph)[Vertex.first][DVertex.first].Weight = this->Graph[Vertex.first][DVertex.first].Weight;
			}
			this->Graph[Vertex.first][DVertex.first] = DVertex.second;
		}
	}
	return;
}

int FInstance::GetLength()
{
	int Output = 0;
	for (auto Vertex : this->Graph) {
		for (auto DVertex : this->Graph[Vertex.first]) {
			Output += DVertex.second.Weight;
		}
	}
	return Output / 2;
}

//Checks if an edge closes a cicle, assuming the graph is connected
bool FInstance::ClosesCicle(std::deque<int> const & Edge)
{//HACK this doesn't check for actual cicles
	if (this->Graph[Edge[0]].size() > 0 && this->Graph[Edge[1]].size() > 0) {
		return true;
	}
	return false;
}

int FInstance::GetMaxRouters()
{
	return this->MaxRouters;
}

void FInstance::SetMaxRouters(int MaxRouters)
{
	this->MaxRouters = MaxRouters;
}


int FInstance::GetMaxLinks()
{
	return this->MaxLinks;
}

void FInstance::SetMaxLinks(int MaxLinks)
{
	this->MaxLinks = MaxLinks;
}

int FInstance::GetArcCount()
{
	return (int) this->Graph.size();
}

std::deque<std::deque<int>> FInstance::GetAdjacencyMatrix()
{

	int Size = this->Graph.rbegin()->first;
	std::deque<int> Line(Size + 1, -1);
	std::deque<std::deque<int>> GraphM(Size + 1, Line);

	for (auto Vertex : this->Graph) {
		for (auto CVertex : Vertex.second) {
			GraphM[Vertex.first][CVertex.first] = CVertex.second.Weight;
			GraphM[CVertex.first][Vertex.first] = CVertex.second.Weight;
		}
	}

	return GraphM;
}
