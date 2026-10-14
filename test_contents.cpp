#include <cstdlib>
#include<iostream>
#include<fstream>
#include "json.hpp"
using namespace std;
using json=nlohmann::json;
int main() {
    system("curl.exe https://api.github.com/repos/ananyagupta079-prog/github-repository-analyzer/contents > contents.json");
    ifstream file("contents.json");
    json contents;
    file>>contents;
    for(auto item:contents){
        if(item["type"]=="dir"){
            cout<<"Folder"<<item["name"]<<endl;
        }
        else{
            cout<<"File"<<item["name"]<<endl;
        }
    }
     return 0;
}