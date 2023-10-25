#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

template <bool Directing, bool Weighting>
class Graph;

template <bool W>
struct ConditionalGraphType {
	using ListType = std::unordered_set<std::string>;
};

template <>
struct ConditionalGraphType<true> {
	using ListType = std::unordered_map<std::string, double>;
};

template <bool W>
using WeightingType = typename ConditionalGraphType<W>::ListType;

namespace GraphIO {
	template <bool Directing, bool Weighting>
	inline void Input(Graph<Directing, Weighting>&, std::ifstream&);

	template <bool Directing, bool Weighting>
	inline void Output(const Graph<Directing, Weighting>&, std::ostream&);
}

template <bool Directing, bool Weighting = 0>
class Graph {
	friend class Graph<false, Weighting>;
	friend class Graph<true, Weighting>;
	friend void GraphIO::Output(const Graph&, std::ostream&);
protected:
	std::unordered_map<std::string, bool> *visited;
	std::unordered_map<std::string, WeightingType<Weighting>*> *adjacencyList;
	inline void AddA(const std::string&, const std::string&, double = 1);
	inline void DFS(const std::string&) noexcept;
	inline void BFS(const std::string&) noexcept;
	inline Graph GetResponsed() const noexcept;
public:
	inline Graph() noexcept;
	inline Graph(const Graph<false, Weighting>&) noexcept;
	inline Graph(const Graph<true, Weighting>&) noexcept;
	inline ~Graph() noexcept;
	// Возвращает количество вершин в графе.
	inline size_t Size() const noexcept;
	// Добавляет вершину с заданным именем
	inline void AddVertex(const std::string&);
	// Для орграфа: добавляет дугу, ведущую из первой заданной вершины во вторую;
	// Для неориентированного графа: добавляет ребро, соединяющее заданную пару вершин.
	inline void AddArc(const std::string&, const std::string&, double = 1);
	// Удаляет вершину с заданным именем.
	inline void RemoveVertex(const std::string&);
	// Для орграфа: удаляет дугу, ведущую из первой заданной вершины во вторую;
	// Для неориентированного графа: удаляет ребро, соединяющее заданную пару вершин.
	inline void RemoveArc(const std::string&, const std::string&);
	// Для орграфа: возвращает полустепень исхода для заданной вершины;
	// Для неориентированного графа: возвращает степень вершины (эквивалентно GetIndeg).
	inline size_t GetOutdeg(const std::string&) const;
	// Для орграфа: возвращает полустепень захода для заданной вершины;
	// Для неориентированного графа: возвращает степень вершины (эквивалентно GetOutdeg).
	inline size_t GetIndeg(const std::string&) const;
	// Возвращает множество петель в графе.
	inline std::unordered_set<std::string> GetLoopSet() const noexcept;
	// Возвращает копию графа без висячих вершин.
	inline Graph GetNonHanging() const noexcept;
	// Для орграфа:
	// - возвращает 1, если граф сильно связный;
	// - возвращает -1, если граф слабо связный.
	// Для неориентированного графа:
	// - возвращает 1, если граф связный.
	// Для графа любого случая:
	// - возвращает 0, если граф несвязный.
	inline int IsConnected() noexcept;
	// Возвращает словарь с вершинами, имеющими путь в заданную вершину, где значением для каждой вершины является количество дуг до заданной вершины.
	inline std::unordered_map<std::string, int> GetWaysTo(const std::string&) const;
};
