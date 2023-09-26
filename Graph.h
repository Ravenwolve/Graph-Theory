#pragma once
#include <string>
#include <iostream>

class Graph {
protected:
	bool isDirected;
	virtual void AddArc(const std::string&, const std::string&, double = 1) = 0;
public:
	virtual int Size() const = 0;
	virtual void AddVertex(const std::string&) = 0;
	virtual void AddEdge(const std::string&, const std::string&, double = 1) = 0;
	virtual void RemoveVertex(const std::string&) = 0;
	virtual void RemoveEdge(const std::string&, const std::string&) = 0;
	virtual void Output(std::ostream&) = 0;
	virtual int GetOutdeg(const std::string&) const = 0;
	virtual std::vector<std::string> GetLoopVertices() const = 0;
	virtual Graph* Clone() = 0;
	static Graph* Create(bool, bool);
	static Graph* Create(std::string);
	static Graph* Create(Graph&);
};