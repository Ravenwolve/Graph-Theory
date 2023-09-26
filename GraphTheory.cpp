#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Graph.h"

int main() {
	setlocale(LC_ALL, "Russian");
	// UU - неор. невзв., UW - неор. взв., DU - ор., невзв, DW - ор., взв.
	Graph* gr = nullptr;
	std::string cmd, secondStr;
	double weight = 1;
	while (true) {
		if (gr == nullptr) {
			std::cout << "Create - создать граф.\n";
			std::getline(std::cin, cmd);
			if (cmd == "Create") {
				std::cout << "P - чтобы указать путь к файлу с графом;\n";
				std::cout << "E - чтобы создать пустой граф.\n";
				std::getline(std::cin, cmd);
				if (cmd == "P") {
					std::cout << "Введите путь к файлу:\n";
					std::getline(std::cin, cmd);
					try {
						gr = Graph::Create(cmd);
						std::cout << "Данные с файла успешно считаны.\n";
					}
					catch(std::string) {
						std::cout << "Путь к файлу указан некорректно или такого файла не существует.\n";
					}
				}
				else if (cmd == "E") {
					std::cout << "D - ориентированный граф, U - неориентированный граф.\n";
					std::getline(std::cin, cmd);
					std::string direction = cmd;
					std::cout << "U - невзвешенный граф, W - взвешенный граф.\n";
					std::getline(std::cin, cmd);
					if (direction == "D") {
						if (cmd == "W")
							gr = Graph::Create(true, true);
						else if (cmd == "U")
							gr = Graph::Create(true, false);
					}
					else if (direction == "U") {
						if (cmd == "W")
							gr = Graph::Create(false, true);
						else if (cmd == "U")
							gr = Graph::Create(false, false);
					}
				}
			}
		}
		else {
			std::cout << "Введите Help чтобы получить список команд.\n";
			std::getline(std::cin, cmd);
			if (cmd == "Add v") {
				std::cout << "Введите имя метки вершины, которую хотите добавить в граф: ";
				std::cin >> cmd;
				try {
					gr->AddVertex(cmd);
				}
				catch (const std::string& ex) {
					std::cout << ex << '\n';
				}
			}
			else if (cmd == "Remove v") {
				std::cout << "Введите имя метки вершины, которую хотите удалить из графа: ";
				std::cin >> cmd;
				try {
					gr->RemoveVertex(cmd);
				}
				catch (const std::string& ex) {
					std::cout << ex << '\n';
				}
			}
			else if (cmd == "Add e") {
				std::cout << "Введите имя первой метки вершины: ";
				std::cin >> cmd;
				std::cout << "Введите имя второй метки вершины: ";
				std::cin >> secondStr;
				if (std::string(typeid(*gr).name()) == "class WeightedGraph") {
					std::cout << "Введите вес дуги/ребра: ";
					std::cin >> weight;
				}
				try {
					gr->AddEdge(cmd, secondStr, weight);
				}
				catch (const std::string& ex) {
					std::cout << ex << '\n';
				}
			}
			else if (cmd == "Remove e") {
				std::cout << "Введите имя первой метки вершины: ";
				std::cin >> cmd;
				std::cout << "Введите имя второй метки вершины: ";
				std::cin >> secondStr;
				try {
					gr->RemoveEdge(cmd, secondStr);
				}
				catch (const std::string& ex) {
					std::cout << ex << '\n';
				}
			}
			else if (cmd == "Show") {
				std::stringstream s;
				gr->Output(s);
				std::getline(s, cmd);
				std::getline(s, cmd);
				while (!s.eof()) {
					std::getline(s, cmd);
					if (cmd.size() > 2 && cmd[2] == ' ')
						cmd[2] = '\t';
					else if (cmd.size() == 2)
						cmd.append("\t");
					std::cout << cmd << std::endl;
				}
			}
			else if (cmd == "Save") {
				std::cout << "Введите путь файла, в который сохранить данные:\n";
				std::getline(std::cin, cmd);
				try {
					std::ofstream fileOut(cmd);
					std::cout << "Изменения успешно сохранены по указанному адресу.\n";
				}
				catch (std::string) {
					std::cout << "Путь к файлу указан некорректно или такого файла не существует.\n";
				}
			}
			else if (cmd == "Delete") {
				delete gr;
				gr = nullptr;
			}
			else if (cmd == "Stop")
				break;
			else if (cmd == "Help") {
				std::cout << "Save - сохранить граф;\n";
				std::cout << "Add v - добавить вершину с меткой в граф;\n";
				std::cout << "Remove v - удалить вершину с меткой в графе;\n";
				std::cout << "Add e - добавить ребро/дугу в граф;\n";
				std::cout << "Remove e - удалить ребро/дугу в графе;\n";
				std::cout << "Show - вывести текущий список смежностей графа;\n";
				std::cout << "Delete - удалить текущий граф;\n";
				std::cout << "Stop - остановить работу программы.\n";
			}
		}
	}

	std::cout << "Полустепень исхода вершины A = " << gr->GetOutdeg("A") << std::endl;
	std::cout << "Список вершин с петлями:" << std::endl;
	std::vector<std::string> ans = gr->GetLoopVertices();
	for (auto item : ans)
		std::cout << item << ' ';
	std::cout << std::endl;
}