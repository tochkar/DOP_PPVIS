// STL_Graph.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "graph.hpp"

#include <iostream>
#include <vector>
#include <string>

#include <map>

/*
* graph
* комментарий к работе:
* граф представлен в виде тройки: две вершины и дуга между ними.
* значниея(названия) вершин - константы.
* изменять можно только значение дуги. доступ через оператор ()
* данные храняться в виде бинарного дерева. в идеале дерева двоичного поиска... но тут как повезёт...
* функция удаления не сделана. слишком мало времени. а к работе нужно подключать итераторы
* 
* hipergraph
* наследник от map, у которого типом значения является граф. самый очевидный способ представить эти данные. и минимум затрат по времени
* это всё, что нужно знать...
*/

int main()
{
    std::vector<std::string> vec;
    vec.push_back("hello");
    auto sIt = vec.begin();

    graph<std::string, int> first_graph;

    first_graph.push("aaa", "bbb", 500);
    first_graph.push("Grodno", "Minsk", 360);
    first_graph.push("Minsk","Grodno", 360);
    first_graph.push("h", "r", 1000);
    first_graph.push("Mogilov", "Minsk", 200);
    
    for (auto it = first_graph.begin(); it != first_graph.end(); ++it)
    {
        std::cout << it->_From << "\t" << it->_To << "\t" << it->_Value << std::endl;
    }
    first_graph("Minsk", "Grodno") = 1000;

    for (auto val : first_graph)
    {
        std::cout << val._From << "\t" << val._To << "\t" << val._Value << std::endl;
    }

    hipergraph<int, std::string, int> hgraph;
    hgraph[1].push("Minsk", "Moskow", 100);
    hgraph[2].push("A", "B", 200);
    for (auto graph : hgraph)
        for (auto val : graph.second)
            std::cout << val._From << "\t" << val._To << "\t" << val._Value << std::endl;
    return 0;
}