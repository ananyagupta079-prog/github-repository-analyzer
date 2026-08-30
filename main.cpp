#include<iostream>
#include<vector>
#include"json.hpp"
#include<string>
#include<algorithm>
using namespace std;
struct repository{
    string name;
    string language;
    bool readme;
    vector<string>topics;
    int stars;
    int score;
    string structure;
    vector<string>folders;
    vector<string>files;
};
int main(){
    string repoURL;
    cout<<"enter GitHub Repository URL";
    cin>>repoURL;
    cout<<"you entered"<<repoURL<<endl;
    size_t pos=repoURL.find("github.com/");
    string path=repoURL.substr(pos+11);
    size_t slash=path.find("/");
    string username=path.substr(0,slash);
    string repositoryName=path.substr(slash+1);
    if(!repositoryName.empty() && repositoryName.back()=='/'){
        repositoryName.pop_back();
    }
    cout<<"username="<<username<<endl;
    cout<<"repositoryName="<<repositoryName<<endl;
    string apiURL="https://api.github.com/repos/"+username+"/"+repositoryName+"/contents";
    //system(("curl.exe"+apiURL+"> contents.json").c_str());
    cout<<"API URL:"<<apiURL<<endl;
    string command="\"C:\\Windows\\System32\\curl.exe\""+apiURL+"> contents.json";
    cout<<command<<endl;
    //system(("cmd/c"+command).c_str());
    string target;
    int n;
    cout<<"enter number of repositories";
    cin>>n;
    vector<repository>repo(n);
    cout<<"enter target role";
    cin>>target;
    if(target!="cpp" &&target!="web" && target!="data"){
        cout<<"role not supported"<<endl;
        return 0;
    }
    for(int i=0;i<repo.size();i++){
        cout<<"enter name";
        cin>>repo[i].name;
        cout<<"enter language";
        cin>>repo[i].language;
        cout<<"enter readme(1 for 'yes' 0 for 'no')";
        cin>>repo[i].readme;
        cout<<"enter stars";
        cin>>repo[i].stars;
        cout<<"enter topics count";
        int topicscount;
        cin>>topicscount;
        for(int j=0;j<topicscount;j++){
            string topic;
            cin>>topic;
            repo[i].topics.push_back(topic);
        }
        repo[i].score=0;
        if(target=="cpp" && repo[i].language=="c++"){
            repo[i].score+=5;
        }
        if(target=="web" && repo[i].language=="javascript"){
            repo[i].score+=5;
        }
        if(target=="data" && repo[i].language=="python"){
            repo[i].score+=5;
        }
        if(repo[i].readme==true){
            repo[i].score+=3;
        }
        if(repo[i].stars>=10){
            repo[i].score+=2;
        }
        for (string topic:repo[i].topics){
            if(target=="cpp" && (topic=="cpp" || topic=="dsa"|| topic==" algorithm")){
                repo[i].score+=2;
                break;
            }
            if(target=="web" && (topic=="html" || topic=="css" || topic=="javascript")){
                repo[i].score+=2;
                break;
            }
            if(target=="data" && (topic=="python"||topic=="pandas"||topic=="sql")){
                repo[i].score+=2;
                break;
            }
            int foldercount=repo[i].folders.size();
            int filecount=repo[i].files.size();
            string structure;
            if(filecount==0){
                structure="Empty";
            }
            else if(filecount<=5){
                structure="Small";
            }
            else if(filecount<=20){
                structure="Medium";
            }
            else{
                structure="Large";
            }
            for(string folder:repo[i].folders){
                if(folder=="src"){
                    repo[i].score+=2;
                }
                if(folder=="include"){
                    repo[i].score+=1;
                }
                if(folder=="tests"){
                    repo[i].score+=2;
                }
                if(folder=="docs"){
                    repo[i].score+=1;
                }
            }
        }
    }
    sort(repo.begin(),repo.end(),[](repository a,repository b){
                return a.score>b.score;
            });
            cout<<"repository ranking";
            for(int i=0;i<repo.size();i++){
                cout<<"rank"<<i+1<<":"<<repo[i].name<<"| score:"<<repo[i].score<<" | Files: "<<repo[i].files.size()
                <<" | Folders: "<<repo[i].folders.size()<<" | Structure: "<<repo[i].structure<<endl;
            }

}
