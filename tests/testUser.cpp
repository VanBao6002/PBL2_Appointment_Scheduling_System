#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <string>
using namespace std; 

int main (){
    std::cout << std::hash<std::string>{}("abc") << std::endl;
}

