#pragma once
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include "Graph.h"

class WeightedGraph : public Graph {
private:
	std::unordered_map<std::string, std::unordered_map<std::string, double>*> *adjacencyList;

	void AddArc(const std::string& firstVertex, const std::string& secondVertex, double weightForWeightedGraph = 1) override {
		if ((*adjacencyList).count(firstVertex) == 0)
			throw "Vertex " + firstVertex + " doesn't exist";
		else if ((*adjacencyList).count(secondVertex) == 0)
			throw "Vertex " + secondVertex + " doesn't exist";
		else if ((*adjacencyList)[firstVertex]->count(secondVertex) != 0)
			throw "Edge (" + firstVertex + ", " + secondVertex + ") already exists";
		(*adjacencyList)[firstVertex]->insert({ secondVertex, weightForWeightedGraph });
	}
public:
	WeightedGraph(bool direction) {
		isDirected = direction;
		adjacencyList = new std::unordered_map<std::string, std::unordered_map<std::string, double>*>();
	}
	WeightedGraph(std::ifstream& file) {
		char direction;
		file >> direction;// >> vertices;
		switch (direction) {
		case 'U':
			isDirected = false;
			break;
		default:
			isDirected = true;
		}
		std::string firstStr, secondStr;
		double weight;
		adjacencyList = new std::unordered_map<std::string, std::unordered_map<std::string, double>*>();
		do {
			file >> firstStr;
			AddVertex(firstStr);
		} while (file.get() != '\n');
		for (file >> firstStr; !file.eof(); file >> firstStr) {
			while (file.get() != '\n') {
				file >> secondStr >> weight;
				AddArc(firstStr, secondStr, weight);
			}
		}
	}
	WeightedGraph(WeightedGraph& other) {
		isDirected = other.isDirected;
		adjacencyList = new std::unordered_map<std::string, std::unordered_map<std::string, double>*>();
		for (auto list : *other.adjacencyList)
			AddVertex(list.first);
		for (auto list : *other.adjacencyList)
			for (auto item : *list.second)
				AddEdge(list.first, item.first, item.second);
	}
	~WeightedGraph() {
		for (auto& item : *adjacencyList)
			delete item.second;
		delete adjacencyList;
	}
	int Size() const {
		return adjacencyList ? adjacencyList->size() : 0;
	}
	Graph* Clone() override {
		return new WeightedGraph(*this);
	}
	void AddVertex(const std::string& vertex) override {
		if ((*adjacencyList).count(vertex) != 0)
			throw "Vertex " + vertex + " already exists";
		(*adjacencyList).insert({ vertex, new std::unordered_map<std::string, double>() });
	}
	void AddEdge(const std::string& firstVertex, const std::string& secondVertex, double weightForWeightedGraph) override {
		AddArc(firstVertex, secondVertex, weightForWeightedGraph);
		if (!isDirected)
			AddArc(secondVertex, firstVertex, weightForWeightedGraph);
	}
	void RemoveVertex(const std::string& vertex) override {
		if ((*adjacencyList).count(vertex) == 0)
			throw "Vertex " + vertex + " doesn't exist";
		delete (*adjacencyList)[vertex];
		(*adjacencyList).erase(vertex);
		for (auto item : *adjacencyList)
			item.second->erase(vertex);
	}
	void RemoveEdge(const std::string& firstVertex, const std::string& secondVertex) override {
		if ((*adjacencyList).count(firstVertex) == 0)
			throw "Vertex " + firstVertex + " doesn't exist";
		else if ((*adjacencyList).count(secondVertex) == 0)
			throw "Vertex " + secondVertex + " doesn't exist";
		else if ((*adjacencyList)[firstVertex]->count(secondVertex) == 0)
			throw "Edge (" + firstVertex + ", " + secondVertex + ") doesn't exists";
		(*adjacencyList)[firstVertex]->erase(secondVertex);
		if (!isDirected)
			(*adjacencyList)[secondVertex]->erase(firstVertex);
	}
	void Output(std::ostream& stream) override {
		if (isDirected)
			stream << "W D\n";
		else stream << "W U\n";
		for (auto list : *adjacencyList)
			stream << ' ' << list.first;
		stream << '\n';
		for (auto list : *adjacencyList) {
			stream << ' ' << list.first;
			for (auto item : *list.second)
				stream << ' ' << item.first << ' ' << item.second;
			stream << '\n';
		}
	}
	int GetOutdeg(const std::string& vertex) const override {
		if ((*adjacencyList).count(vertex) == 0)
			throw "Vertex " + vertex + " doesn't exist";
		return (*adjacencyList)[vertex]->size();
	}
	std::vector<std::string> GetLoopVertices() const override {
		std::vector<std::string> result;
		for (auto list : *adjacencyList)
			if (list.second->count(list.first) != 0)
				result.push_back(list.first);
		return result;
	}
};

// ---------------------- Невзвешенный граф -----------------------------------------

class UnweightedGraph : public Graph {
private:
	std::unordered_map<std::string, std::unordered_set<std::string>*>* adjacencyList;

	void AddArc(const std::string& firstVertex, const std::string& secondVertex, double weightForWeightedGraph = 1) override {
		if ((*adjacencyList).count(firstVertex) == 0)
			throw "Vertex " + firstVertex + " doesn't exist";
		else if ((*adjacencyList).count(secondVertex) == 0)
			throw "Vertex " + secondVertex + " doesn't exist";
		else if ((*adjacencyList)[firstVertex]->count(secondVertex) != 0)
			throw "Edge (" + firstVertex + ", " + secondVertex + ") already exists";
		else if (firstVertex == secondVertex && !isDirected)
			throw "Loops are only possible on a directed graph";
		(*adjacencyList)[firstVertex]->insert(secondVertex);
	}
public:
	UnweightedGraph(bool direction = 0) {
		isDirected = direction;
		adjacencyList = new std::unordered_map<std::string, std::unordered_set<std::string>*>();
	}
	UnweightedGraph(std::ifstream& file) {
		char direction;
		file >> direction;
		switch (direction) {
		case 'U':
			isDirected = false;
			break;
		default:
			isDirected = true;
		}
		std::string firstStr, secondStr;
		adjacencyList = new std::unordered_map<std::string, std::unordered_set<std::string>*>();
		do {
			file >> firstStr;
			AddVertex(firstStr);
		} while (file.get() != '\n');
		for (file >> firstStr; !file.eof(); file >> firstStr) {
			while (file.get() != '\n') {
				file >> secondStr;
				AddArc(firstStr, secondStr);
			}
		}
	}
	UnweightedGraph(UnweightedGraph& other) {
		isDirected = other.isDirected;
		adjacencyList = new std::unordered_map<std::string, std::unordered_set<std::string>*>();
		for (auto list : *other.adjacencyList)
			AddVertex(list.first);
		for (auto list : *other.adjacencyList)
			for (auto item : *list.second)
				AddArc(list.first, item);
	}
	~UnweightedGraph() {
		for (auto& item : *adjacencyList)
			delete item.second;
		delete adjacencyList;
	}
	int Size() const {
		return adjacencyList ? adjacencyList->size() : 0;
	}
	Graph* Clone() override {
		return new UnweightedGraph(*this);
	}
	void AddVertex(const std::string& vertex) override {
		if ((*adjacencyList).count(vertex) != 0)
			throw "Vertex " + vertex + " already exists";
		(*adjacencyList).insert({ vertex, new std::unordered_set<std::string>() });
	}
	void AddEdge(const std::string& firstVertex, const std::string& secondVertex, double weightForWeightedGraph = 1) override {
		AddArc(firstVertex, secondVertex, weightForWeightedGraph);
		if (!isDirected)
			AddArc(secondVertex, firstVertex, weightForWeightedGraph);
	}
	void RemoveVertex(const std::string& vertex) override {
		if ((*adjacencyList).count(vertex) == 0)
			throw "Vertex " + vertex + " doesn't exist";
		delete (*adjacencyList)[vertex];
		(*adjacencyList).erase(vertex);
		for (auto item : *adjacencyList)
			item.second->erase(vertex);
	}
	void RemoveEdge(const std::string& firstVertex, const std::string& secondVertex) override {
		if ((*adjacencyList).count(firstVertex) == 0)
			throw "Vertex " + firstVertex + " doesn't exist";
		else if ((*adjacencyList).count(secondVertex) == 0)
			throw "Vertex " + secondVertex + " doesn't exist";
		else if ((*adjacencyList)[firstVertex]->count(secondVertex) == 0)
			throw "Edge (" + firstVertex + ", " + secondVertex + ") doesn't exists";
		(*adjacencyList)[firstVertex]->erase(secondVertex);
		if (!isDirected)
			(*adjacencyList)[secondVertex]->erase(firstVertex);
	}
	void Output(std::ostream& stream) override {
		if (isDirected)
			stream << "U D\n";
		else stream << "U U\n";
		for (auto list : *adjacencyList)
			stream << ' ' << list.first;
		stream << '\n';
		for (auto list : *adjacencyList) {
			stream << ' ' << list.first;
			for (auto item : *list.second)
				stream << ' ' << item;
			stream << '\n';
		}
	}
	int GetOutdeg(const std::string& vertex) const override {
		if ((*adjacencyList).count(vertex) == 0)
			throw "Vertex " + vertex + " doesn't exist";
		return (*adjacencyList)[vertex]->size();
	}
	std::vector<std::string> GetLoopVertices() const override {
		std::vector<std::string> result;
		for (auto list : *adjacencyList)
			if (list.second->count(list.first) != 0)
				result.push_back(list.first);
		return result;
	}
};

Graph* Graph::Create(bool direction, bool weighting) {
	if (weighting)
		return new WeightedGraph(direction);
	return new UnweightedGraph(direction);
}

Graph* Graph::Create(std::string path)
{
	std::ifstream file(path);
	char weighting;
	file >> weighting;
	switch (weighting) {
	case 'W':
		return new WeightedGraph(file);
	default:
		return new UnweightedGraph(file);
	}
	file.close();
}

Graph* Graph::Create(Graph& other) {
	return other.Clone();
}