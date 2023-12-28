#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <algorithm>
#include <string>

using namespace std;

struct Node {
    vector<int> heirs;
    bool busy = false;
    string work = "not job";
};

map<int, Node> graph;  

bool Cyclic_one(int v, bool visited[], bool *recStack) {
    if (!visited[v]) {
        visited[v] = true;
        recStack[v] = true;

        for (int i = 0; i < graph[v].heirs.size(); i++) {
            int n = graph[v].heirs[i];
            if (!visited[n] && Cyclic_one(n, visited, recStack)) {
                return true;
            } else if (recStack[n]) {
                return true;
            }
        }

    }
    recStack[v] = false;
    return false;
}

bool Cyclic() {
    bool *visited = new bool[graph.size()];
    bool *recStack = new bool[graph.size()];
    for (int i = 0; i < graph.size(); i++) {
        visited[i] = false;
        recStack[i] = false;
    }

    for (int i = 0; i < graph.size(); i++) {
        if (Cyclic_one(i, visited, recStack)) {
            return true;
        }
    }
    return false;
}

bool Connected(std::vector<int>& endnods) {
    int count_out;
    int id_node;
    bool found;
    for (auto & pair : graph) {
        id_node = pair.first; 
        count_out = pair.second.heirs.size();
        found = std::find(endnods.begin(), endnods.end(), id_node) != endnods.end();
        if (count_out > 1 && found == false)
            return false;
    }
    return true;
}


void all_work(std::vector<int>& startNodes){
    queue<int> queue;
    int id_work_node, id_work_node_next, size;
    Node *work_node;
    string work;
    Node *next_work_node;
    for (int i = 0; i < startNodes.size(); i++){
        queue.push(startNodes[i]);
    }
        for (int i = 0; i < graph.size() - 1 ; i++){
            id_work_node = queue.front();
            cout << id_work_node << endl;
            queue.pop();
            work_node = &(graph.find(id_work_node) -> second);
            work = work_node->work;
            cout << work << endl;
            work_node->busy = true;
            size = work_node->heirs.size();
            if(size != 0){
                id_work_node_next = work_node->heirs.front();
                next_work_node = &(graph.find(id_work_node_next) -> second);
                if(next_work_node->busy == false){
                    next_work_node->busy = true;
                    queue.push(id_work_node_next);
                }
            }
        }
}

int main() {
    ifstream configFile("dag.ini");
    string line;
    vector<int> startNodes;
    vector<int> endNodes;
    int id;
    Node *work_node_now;
    while (getline(configFile, line)) {
        if (line.find('=') != string::npos) {
            istringstream is(line);
            string key, value;
            if (getline(is, key, '=') && getline(is, value)) {
                if (key == "NODE") {
                    Node node;
                    id = stoi(value);
                    graph[stoi(value)] = node;
                } else if (key == "EDGE") {
                    int from, to;
                    istringstream(value) >> from >> to;
                    graph[from].heirs.push_back(to);
                } else if (key == "START_NODE") {
                    startNodes.push_back(stoi(value));
                } else if (key == "END_NODE") {
                    endNodes.push_back(stoi(value));
                } else if (key == "JOB") {
                    work_node_now = &(graph.find(id) -> second);
                    work_node_now->work = value;
                }
            }
        }
    }
    configFile.close();
    if (!Connected(endNodes)) {
        cout << "Граф не связан или пересвязан" << endl;return 1;}
    if (Cyclic()) {
        cout << "Граф содержит циклы" << endl;return 1;}
    if (startNodes.empty() || endNodes.empty()) {
        cout << "Отсутствуют стартовые или завершающие узлы" << endl;   return 1;}
    all_work(startNodes);
    return 0;
}
