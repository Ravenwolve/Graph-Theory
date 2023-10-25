#pragma once
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include "Graph.h"

template Graph<true, true>;
template Graph<false, false>;
template Graph<true, false>;
template Graph<false, true>;
template void GraphIO::Input<true, true>(Graph<true, true>&, std::ifstream&);
template void GraphIO::Input<false, false>(Graph<false, false>&, std::ifstream&);
template void GraphIO::Input<true, false>(Graph<true, false>&, std::ifstream&);
template void GraphIO::Input<false, true>(Graph<false, true>&, std::ifstream&);
template void GraphIO::Output<true, true>(const Graph<true, true>&, std::ostream&);
template void GraphIO::Output<false, false>(const Graph<false, false>&, std::ostream&);
template void GraphIO::Output<false, true>(const Graph<false, true>&, std::ostream&);
template void GraphIO::Output<true, false>(const Graph<true, false>&, std::ostream&);

template <bool Directing, bool Weighting>
inline size_t Graph<Directing, Weighting>::Size() const noexcept {
	return adjacencyList ? adjacencyList->size() : 0;
}

template <bool Directing, bool Weighting>
inline Graph<Directing, Weighting>::Graph() noexcept {
	adjacencyList = new std::unordered_map<std::string, WeightingType<Weighting>*>();
	visited = new std::unordered_map<std::string, bool>();
}

template <bool Directing, bool Weighting>
inline Graph<Directing, Weighting>::Graph(const Graph<false, Weighting>& other) noexcept {
	adjacencyList = new std::unordered_map<std::string, WeightingType<Weighting>*>(*(other.adjacencyList));
	for (auto list : *other.adjacencyList)
		(*adjacencyList)[list.first] = new WeightingType<Weighting>(*list.second);
	visited = new std::unordered_map<std::string, bool>(*other.visited);
}

template <bool Directing, bool Weighting>
inline Graph<Directing, Weighting>::Graph(const Graph<true, Weighting>& other) noexcept {
	if constexpr (Directing) {
		adjacencyList = new std::unordered_map<std::string, WeightingType<Weighting>*>(*(other.adjacencyList));
		for (auto list : *other.adjacencyList)
			(*adjacencyList)[list.first] = new WeightingType<Weighting>(*list.second);
		visited = new std::unordered_map<std::string, bool>(*other.visited);
	}
	else {
		adjacencyList = new std::unordered_map<std::string, WeightingType<Weighting>*>();
		visited = new std::unordered_map<std::string, bool>();
		for (auto item : *other.adjacencyList)
			AddVertex(item.first);
		for (auto list : *other.adjacencyList)
			for (auto item : *list.second) {
				if constexpr (Weighting) {
					try { AddArc(list.first, item.first, item.second); }
					catch (...) {}
				}
				else {
					try { AddArc(list.first, item); }
					catch (...) {}
				}
			}
	}
}

template <bool Directing, bool Weighting>
inline Graph<Directing, Weighting>::~Graph() noexcept {
	for (auto list : *adjacencyList)
		delete (*adjacencyList)[list.first];
	delete adjacencyList;
	delete visited;
}

template <bool Directing, bool Weighting>
inline void Graph<Directing, Weighting>::AddVertex(const std::string& vertex) {
	if (adjacencyList->contains(vertex))
		throw "Vertex " + vertex + " already exists";
	adjacencyList->insert({ vertex, new WeightingType<Weighting>() });
	visited->insert({ vertex, false });
}

template <bool Directing, bool Weighting>
inline void Graph<Directing, Weighting>::AddA(const std::string& firstVertex, const std::string& secondVertex, double weightForWeightedGraph) {
	if (!adjacencyList->contains(firstVertex))
		throw "Vertex " + firstVertex + " doesn't exist";
	else if (!adjacencyList->contains(secondVertex))
		throw "Vertex " + secondVertex + " doesn't exist";
	else if ((*adjacencyList)[firstVertex]->contains(secondVertex))
		throw (Directing ? "Arc (" + firstVertex + ", " + secondVertex + ")" : "Edge {" + firstVertex + ", " + secondVertex + "}") + " already exists";
	if constexpr (Weighting)
		(*adjacencyList)[firstVertex]->insert({ secondVertex, weightForWeightedGraph });
	else (*adjacencyList)[firstVertex]->insert(secondVertex);
}

template <bool Directing, bool Weighting>
inline void Graph<Directing, Weighting>::AddArc(const std::string& firstVertex, const std::string& secondVertex, double weightForWeightedGraph) {
	AddA(firstVertex, secondVertex, weightForWeightedGraph);
	if constexpr (!Directing)
		AddA(secondVertex, firstVertex, weightForWeightedGraph);
}

template <bool Directing, bool Weighting>
inline void Graph<Directing, Weighting>::RemoveArc(const std::string& firstVertex, const std::string& secondVertex) {
	if (!adjacencyList->contains(firstVertex))
		throw "Vertex " + firstVertex + " doesn't exist";
	else if (!adjacencyList->contains(secondVertex))
		throw "Vertex " + secondVertex + " doesn't exist";
	else if (!(*adjacencyList)[firstVertex]->contains(secondVertex))
		throw (Directing ? "Arc (" + firstVertex + ", " + secondVertex + ")" : "Edge {" + firstVertex + ", " + secondVertex + "}") + " doesn't exists";
	(*adjacencyList)[firstVertex]->erase(secondVertex);
	if constexpr (!Directing)
		(*adjacencyList)[secondVertex]->erase(firstVertex);
}

template <bool Directing, bool Weighting>
inline void Graph<Directing, Weighting>::RemoveVertex(const std::string& vertex) {
	if (!adjacencyList->contains(vertex))
		throw "Vertex " + vertex + " doesn't exist";
	for (auto iter = adjacencyList->begin(); iter != adjacencyList->end(); ++iter)
		if (iter->second->contains(vertex))
			RemoveArc(iter->first, vertex);
	delete (*adjacencyList)[vertex];
	adjacencyList->erase(vertex);
	visited->erase(vertex);
}

template <bool Directing, bool Weighting>
inline size_t Graph<Directing, Weighting>::GetOutdeg(const std::string& vertex) const {
	if (!adjacencyList->contains(vertex))
		throw "Vertex " + vertex + " doesn't exist";
	return (*adjacencyList)[vertex]->size();
}

template <bool Directing, bool Weighting>
inline size_t Graph<Directing, Weighting>::GetIndeg(const std::string& vertex) const {
	if (!adjacencyList->contains(vertex))
		throw "Vertex " + vertex + " doesn't exist";
	for (auto list : *adjacencyList)
		if (list.second->contains(vertex))
			return list.second->size();
	return 0;
}

template <bool Directing, bool Weighting>
inline std::unordered_set<std::string> Graph<Directing, Weighting>::GetLoopSet() const noexcept {
	std::unordered_set<std::string> result;
	for (auto list : *adjacencyList)
		if (list.second->contains(list.first))
			result.insert(list.first);
	return result;
}

template <bool Directing, bool Weighting>
inline Graph<Directing, Weighting> Graph<Directing, Weighting>::GetNonHanging() const noexcept {
	Graph graph = *this;
	if constexpr (Directing) {
		for (auto list : *adjacencyList)
			if (GetOutdeg(list.first) == 1 && GetIndeg(list.first) == 1)
				graph.RemoveVertex(list.first);
	}
	else for (auto list : *graph.adjacencyList)
		if (GetOutdeg(list.first) == 1)
			graph.RemoveVertex(list.first);
	return graph;
}

template <bool Directing, bool Weighting>
inline void Graph<Directing, Weighting>::DFS(const std::string& vertex) noexcept {
	std::stack<std::string>* stack = new std::stack<std::string>();
	std::string curVertex;
	(*visited)[vertex] = true;
	stack->push(vertex);
	while (!stack->empty()) {
		curVertex = stack->top();
		stack->pop();
		for (auto item : *(*adjacencyList)[curVertex]) {
			if constexpr (Weighting) {
				if (!(*visited)[item.first]) {
					stack->push(item.first);
					(*visited)[item.first] = true;
				}
			}
			else if (!(*visited)[item]) {
				stack->push(item);
				(*visited)[item] = true;
			}
		}
	}
	delete stack;
}

template <bool Directing, bool Weighting>
inline void Graph<Directing, Weighting>::BFS(const std::string& vertex) noexcept {
	std::queue<std::string>* queue = new std::queue<std::string>();
	std::string curVertex;
	queue->push(vertex);
	(*visited)[vertex] = true;
	while (!queue->empty()) {
		curVertex = queue->front();
		queue->pop();
		for (auto item : *(*adjacencyList)[curVertex]) {
			if constexpr (Weighting) {
				if (!(*visited)[item.first]) {
					queue->push(item.first);
					(*visited)[item.first] = true;
				}
			}
			else if (!(*visited)[item]) {
				queue->push(item);
				(*visited)[item] = true;
			}
		}
	}
	delete queue;
}

template <bool Directing, bool Weighting>
inline Graph<Directing, Weighting> Graph<Directing, Weighting>::GetResponsed() const noexcept {
	if constexpr (Directing) {
		Graph responsedGraph;
		for (auto item : *adjacencyList)
			responsedGraph.AddVertex(item.first);
		for (auto list : *adjacencyList)
			for (auto item : *(list.second)) {
				if constexpr (Weighting) {
					try { responsedGraph.AddA(item.first, list.first, item.second); }
					catch (...) {}
				}
				else {
					try { responsedGraph.AddA(item, list.first); }
					catch (...) {}
				}
			}
		return responsedGraph;
	}
	else {
		Graph copy = *this;
		return copy;
	}
}

template <bool Directing, bool Weighting>
inline int Graph<Directing, Weighting>::IsConnected() noexcept {
	if constexpr (!Directing) {
		int result = 1;
		DFS((*(adjacencyList->begin())).first);
		for (auto iter = visited->begin(); iter != visited->end(); ++iter) {
			if (!iter->second)
				result = 0;
			else iter->second = false;
		}
		return result;
	}
	else {
		bool strongConnect;
		for (auto list : *adjacencyList) {
			strongConnect = true;
			DFS(list.first);
			for (auto iter = visited->begin(); iter != visited->end(); ++iter) {
				if (!iter->second)
					strongConnect = false;
				else iter->second = false;
			}
			if (strongConnect) // если это сильно-связный орграф
				return 1;
		}
		return -Graph<0, Weighting>(*this).IsConnected(); // проверка на слабую связность (0 или -1)
	}
}

template <bool Directing, bool Weighting>
inline std::unordered_map<std::string, int> Graph<Directing, Weighting>::GetWaysTo(const std::string& vertex) const {
	std::unordered_map<std::string, int> result;
	Graph responsedGraph = GetResponsed();
	std::queue<std::pair<std::string, int>> queue;
	int iter = 0;
	std::pair<std::string, int> curVertex;
	queue.push({ vertex, iter });
	(*responsedGraph.visited)[vertex] = true;
	while (!queue.empty()) {
		curVertex = queue.front();
		queue.pop();
		iter = curVertex.second + 1;
		for (auto item : *(*(responsedGraph.adjacencyList))[curVertex.first]) {
			if constexpr (Weighting) {
				if (!(*(responsedGraph.visited))[item.first]) {
					queue.push({ item.first, iter });
					(*(responsedGraph.visited))[item.first] = true;
					result.insert({ item.first, iter });
				}
			}
			else if (!(*(responsedGraph.visited))[item]) {
				queue.push({ item, iter });
				(*(responsedGraph.visited))[item] = true;
				result.insert({ item, iter });
			}
		}
	}
	return result;
}

template <bool Directing, bool Weighting>
inline void GraphIO::Input(Graph<Directing, Weighting>& graph, std::ifstream& input) {
	bool d, w;
	input >> d >> w;
	if (d != Directing || w != Weighting)
		throw "Input graph doesn't match the declared type";
	std::string firstStr, secondStr;
	input.get();
	while (input.get() != '\n') {
		input >> firstStr;
		graph.AddVertex(firstStr);
	}
	double weight = 1;
	for (input >> firstStr; !input.eof(); input >> firstStr)
		while (input.get() != '\n') {
			input >> secondStr;
			if constexpr (Weighting)
				input >> weight;
			try {
				graph.AddArc(firstStr, secondStr, weight);
			}
			catch (...) {}
		}
}

template <bool Directing, bool Weighting>
inline void GraphIO::Output(const Graph<Directing, Weighting>& graph, std::ostream& output) {
	output << Directing << ' ' << Weighting << std::endl;
	for (auto list : *graph.adjacencyList)
		output << ' ' << list.first;
	output << '\n';
	for (auto list : *graph.adjacencyList) {
		output << ' ' << list.first;
		for (auto item : *list.second) {
			if constexpr (Weighting)
				output << ' ' << item.first << ' ' << item.second;
			else output << ' ' << item;
		}
		output << '\n';
	}
}
