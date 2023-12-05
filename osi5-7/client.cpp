#include <iostream>
#include "topology.h"
#include <set>
#include <limits>
#include "myZmq.h"
#include <sys/time.h>
#include <string>
#include <unistd.h>
#include "tree.h"

std::vector<int> treee;

int main(){
    std::string command;
    TNode node(-1, -1, -1);
    Tree tree;
    std::string answer;
    while(std::cin >> command){
        if(command == "create"){
            int child;
            std::cin >> child;
            if(tree.exist(child)){
                std::cout << "Error: child already existed!\n";
            }
            else{
                treee.push_back(child);
                while(true){
                    int idParent = tree.findId();
                    if(idParent == node.id){
                        answer = node.createChild(child);
                        tree.addElem(child, idParent);
                        break;
                    }
                    else{
                        std::string message = "create " + std::to_string(child);
                        answer = node.sendStr(message, idParent);
                        if(answer == "Error: id is not found"){
                            tree.notAvailable(idParent);
                        }
                        else{
                            tree.addElem(child, idParent);
                            break;
                        }
                    }
                }
                std::cout << answer << std::endl;
            }
        } else if (command == "heartbit"){
            int time;
            std::cin >> time;
            for (int i = 0; i <= 2; ++i) {
                for (int childC = 0; childC < treee.size(); ++childC) {
                    if(!tree.exist(treee[childC])){
                        std::cout << "Error: child is not existed!\n";
                    }
                    else if(node.leftId == treee[childC] || node.rightId == treee[childC]){
                        answer = node.Ping(treee[childC]);
                        std::cout << "(" << treee[childC] << ") => " << answer << std::endl;
                    }
                    else{
                        std::string message = "ping " + std::to_string(treee[childC]);
                        answer = node.sendStr(message, treee[childC]);
                        if(answer == "Error: id is not found"){
                            answer = "OK: 0";
                        }
                        std::cout << "(" << treee[childC] << ") => " << answer << std::endl;
                    }
                }
                usleep(time);
            }
        } else if(command == "ping"){
            int child;
            std::cin >> child;
            if(!tree.exist(child)){
                std::cout << "Error: child is not existed!\n";
            }
            else if(node.leftId == child || node.rightId == child){
                answer = node.Ping(child);
                std::cout << answer << std::endl;
            }
            else{
                std::string message = "ping " + std::to_string(child);
                answer = node.sendStr(message, child);
                if(answer == "Error: id is not found"){
                    answer = "OK: 0";
                }
                std::cout << answer << std::endl;
            }
        } else if(command == "exec"){
            std::string str;
            int child;
            std::cin >> child;
            getline(std::cin, str);
            if(!tree.exist(child)){
                std::cout << "Error: child is not existed!\n";
            }else{
                std::string message = "exec " + str;
                answer = node.sendStr(message, child);
                std::cout << answer << std::endl;
            }
        }
        else if(command == "remove"){
            int child;
            std::cin >> child;
            std::string message = "remove";
            if(!tree.exist(child)){
                std::cout << "Error: child is not existed!\n";
            }else{
                answer = node.sendStr(message, child);
                treee.erase(std::remove(treee.begin(), treee.end(), child), treee.end());
                if(answer != "Error: id is not found"){
                    tree.Remove(child);
                    if(child == node.leftId){
                        unbind(node.left, node.leftPort);
                        node.leftId = -2;
                        answer = "OK";
                    }
                    else if(child == node.rightId){
                        node.rightId = -2;
                        unbind(node.right, node.rightPort);
                        answer = "OK";
                    }
                    else{
                        message = "clear " + std::to_string(child);
                        answer = node.sendStr(message, std::stoi(answer));
                    }
                    std::cout << answer << std::endl;
                }
            }
        } else if (command == "kill") {
            node.removeElem();
            tree.Remove(tree.findId());
            treee.clear();
            break;
        }
    }
    return 0;
}