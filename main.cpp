#include<iostream>
#include<vector>
#include"json.hpp"
#include<string>
#include<algorithm>
#include<fstream>
#include<cctype>

using namespace std;
using json = nlohmann::json;

struct repository{
    int commits = 0;
    int branches = 0;
    int contributions = 0;

    string name;
    string language;

    bool readme = false;
    vector<string> topics;

    int stars = 0;
    int score = 0;

    string structure;
    vector<string> folders;
    vector<string> files;

    bool gitignore = false;

    int sourceFiles = 0;
    int forks = 0;
    int pullRequests = 0;
    vector<string>reasons;
};


json readJSON(string filename){
    ifstream file(filename);

    if(!file){
        cout << "Could not open " << filename << endl;
        exit(1);
    }

    json data;
    file >> data;

    return data;
}
void analyzeContents(string url, repository &repo, int &fileCounter,string path)
{
    string filename =
        "contents_" + to_string(fileCounter++) + ".json";

    string command =
        "curl.exe -L -s -H \"User-Agent: GitHubAnalyzer\" \"" +
        url + "\" -o " + filename;

    int result = system(command.c_str());

    if(result != 0)
    {
        cout << "Failed to fetch repository contents." << endl;
        return;
    }

    json data = readJSON(filename);
    if(data.is_object() && data.contains("message")){
        cout<<"github API Error"<<data["message"]<<endl;
        return;
    }
    if(!data.is_array()){
        cout<<"Unexpected repository contents response"<<endl;
        return;
    }
    for(auto item : data)
    {
        string name = item["name"];
        string type = item["type"];

        if(type == "file")
        {
            repo.files.push_back(path+name);
        }

        else if(type == "dir")
        {
            repo.folders.push_back(path+name);
            string newPath=path+name+"/";

            analyzeContents(item["url"], repo, fileCounter,newPath);
        }
    }
}

void parseURL(string repoURL, string &username, string &repositoryName){

    size_t pos = repoURL.find("github.com/");

    if(pos == string::npos){
        cout << "Invalid GitHub URL" << endl;
        exit(1);
    }

    string path = repoURL.substr(pos + 11);

    size_t slash = path.find("/");

    if(slash == string::npos){
        cout << "Invalid GitHub repository URL" << endl;
        exit(1);
    }

    username = path.substr(0, slash);
    repositoryName = path.substr(slash + 1);

    if(!repositoryName.empty() && repositoryName.back() == '/'){
        repositoryName.pop_back();
    }
}
int getTotalCount(string apiURL,int repoIndex)
{
    int total = 0;
    int page = 1;

    while(true)
    {
        string filename =
            "page_" + to_string(repoIndex) + "_"+ to_string(page)+ ".json";

        string url =
            apiURL +
            "?page=" + to_string(page) +
            "&per_page=100";

        string command =
            "curl.exe -L -s -H \"User-Agent: GitHubAnalyzer\" \"" +
            url + "\" -o " + filename;

        int result = system(command.c_str());

        if(result != 0)
            break;

        json data = readJSON(filename);
        if(data.is_object() && data.contains("message")){
            cout<<"GitHub API Error:"<<data["message"];
            break;
        }

        if(!data.is_array() || data.empty())
            break;

        total += data.size();

        if(data.size()<100)
            break;
        page++;
        
    }

    return total;
}
int getContributions(string apiURL, string username, int repoIndex)
{
    int page = 1;

    while(true)
    {
        string filename =
            "contributors_" + to_string(repoIndex) +
            "_" + to_string(page) + ".json";

        string url =
            apiURL +
            "?page=" + to_string(page) +
            "&per_page=100";

        string command =
            "curl.exe -L -s -H \"User-Agent: GitHubAnalyzer\" \"" +
            url + "\" -o " + filename;

        int result = system(command.c_str());

        if(result != 0)
            break;

        json data = readJSON(filename);

        if(data.is_object() && data.contains("message"))
        {
            cout << "GitHub API Error: "
                 << data["message"] << endl;
            break;
        }

        if(!data.is_array() || data.empty())
            break;

        for(auto &contributor : data)
        {
            if(contributor["login"] == username)
            {
                return contributor["contributions"];
            }
        }

        if(data.size() < 100)
            break;

        page++;
    }

    return 0;
}
string toLowerCase(string s){
    for(char &c:s){
        c=static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return s;
}
int main(){

    string target;
    int n;

    cout << "Enter number of repositories: ";
    cin >> n;

    vector<repository> repo(n);
    int fileCounter=0;
    string repoURL;

    for(int i = 0; i < n; i++){

        cout << "\nEnter GitHub Repository URL: ";
        cin >> repoURL;

        cout << "You entered: " << repoURL << endl;

        string username;
        string repositoryName;

        parseURL(repoURL, username, repositoryName);

        cout << "Username: " << username << endl;
        cout << "Repository Name: " << repositoryName << endl;

        string apiURL =
            "https://api.github.com/repos/" +
            username + "/" + repositoryName + "/contents";

        string repoapi =
            "https://api.github.com/repos/" +
            username + "/" + repositoryName;

        string commitsapi =
            "https://api.github.com/repos/" +
            username + "/" + repositoryName + "/commits";

        string branchesapi =
            "https://api.github.com/repos/" +
            username + "/" + repositoryName + "/branches";

        string pullrequestapi =
            "https://api.github.com/repos/" +
            username + "/" + repositoryName + "/pulls";

        string contributionsapi =
            "https://api.github.com/repos/" +
            username + "/" + repositoryName + "/contributors";


        cout << "API URL: " << apiURL << endl;
        cout << "Repository URL: " << repoapi << endl;
        cout << "Commits URL: " << commitsapi << endl;
        cout << "Branches URL: " << branchesapi << endl;
        cout << "Pull Requests URL: " << pullrequestapi << endl;


        
        string repocommand =
            "curl.exe -L -s -H \"User-Agent: GitHubAnalyzer\" \"" +
            repoapi + "\" -o repo.json";

        
        


        // Fetch contributions
        

        cout << "Contributions information fetched successfully." << endl;


        // Fetch pull requests
        

        cout << "Pull Requests information fetched successfully." << endl;


        // Fetch branches
        

        cout << "Branches information fetched successfully." << endl;


        // Fetch commits
       

        cout << "Commits information fetched successfully." << endl;


        // Fetch repository information
        int repoResult =
            system(repocommand.c_str());

        if(repoResult != 0){
            cout << "Failed to fetch repository information." << endl;
            return 0;
        }

        cout << "Repository information fetched successfully." << endl;


        

        // Read JSON files

        cout << "\nReading repo.json..." << endl;

        json repodata = readJSON("repo.json");
        if(!repodata.is_object()|| repodata.contains("message")||!repodata.contains("name")){
            cout<<"invalid repository response from Github"<<endl;
            continue;
        }

        cout << "repo.json loaded successfully." << endl;


        // Store repository information
        repo[i].name = repodata["name"];
        repo[i].language = repodata.value("language",string(""));
        if(repodata["language"].is_null()){
            repo[i].language="";
        }
        repo[i].stars = repodata.value("stargazers_count",0);
        repo[i].forks = repodata.value("forks_count",0);


        // Store topics
        for(auto topic : repodata["topics"]){
            repo[i].topics.push_back(topic);
        }


        cout << "\nRepository Name: "
             << repo[i].name << endl;

        cout << "Language: "
             << repo[i].language << endl;

        cout << "Stars: "
             << repo[i].stars << endl;

        cout << "Topics: ";

        for(auto topic : repo[i].topics){
            cout << topic << " ";
        }

        cout << endl;


        //Analyze repository contents
        analyzeContents(apiURL,repo[i],fileCounter,"");


        
        
        // Detect README and .gitignore
        for (string file : repo[i].files) {
            size_t pos = file.find_last_of("/\\");
            string filename = (pos == string::npos)
                              ? file
                              : file.substr(pos + 1);

            filename = toLowerCase(filename);

            if (filename == "readme" ||
                filename.rfind("readme.", 0) == 0) {
                repo[i].readme = true;
            }

            if (filename == ".gitignore") {
                repo[i].gitignore = true;
            }
        }


        // Count source files
        repo[i].sourceFiles = 0;

        for(string file : repo[i].files){

            size_t pos = file.find_last_of(".");

            if(pos != string::npos){

                string extension = toLowerCase(file.substr(pos));

                if(extension == ".cpp" ||
                   extension == ".c" ||
                   extension == ".py" ||
                   extension == ".js" ||
                   extension == ".java"){

                    repo[i].sourceFiles++;
                }
            }
        }


        // Read commits
        

        repo[i].commits = getTotalCount(commitsapi,i);

        cout << "Number of commits: "
             << repo[i].commits << endl;


        // Read branches
        

        repo[i].branches = getTotalCount(branchesapi,i);

        cout << "Number of branches: "
             << repo[i].branches << endl;


        // Read pull requests
        

        repo[i].pullRequests =
            getTotalCount(pullrequestapi,i);

        cout << "Number of pull requests: "
             << repo[i].pullRequests << endl;


        // Read contributions
        
        repo[i].contributions = getContributions(contributionsapi,username,i);

        
        

        cout << "Number of contributions: "
             << repo[i].contributions << endl;
    }


    // Target role
    cout << "\nEnter target role (cpp/web/data): ";
    cin >> target;

    if(target != "cpp" &&
       target != "web" &&
       target != "data"){

        cout << "Role not supported." << endl;
        return 0;
    }


    // Calculate scores
    for(int i = 0; i < repo.size(); i++){

        repo[i].score = 0;


        // Language score
        if(target == "cpp" &&
           repo[i].language == "C++"){

            repo[i].score += 5;
            repo[i].reasons.push_back("C++ language +5");
        }

        if(target == "web" &&
           repo[i].language == "JavaScript"){

            repo[i].score += 5;
            repo[i].reasons.push_back("Javascript language +5");
        }

        if(target == "data" &&
           repo[i].language == "Python"){

            repo[i].score += 5;
            repo[i].reasons.push_back("Python language +5");
        }


        // README
        if(repo[i].readme){

            repo[i].score += 3;
            repo[i].reasons.push_back("README present +3");
        }


        // Gitignore
        if(repo[i].gitignore){

            repo[i].score += 1;
            repo[i].reasons.push_back("Gitignore present +1");
        }


        // Source files
        if(repo[i].sourceFiles > 0){

            repo[i].score += 2;
            repo[i].reasons.push_back("Source files present +2");
        }


        // Stars
        if(repo[i].stars >= 10){

            repo[i].score += 2;
            repo[i].reasons.push_back("10+ stars +2");
        }


        // Commits
        if(repo[i].commits >= 100){

            repo[i].score += 5;
            repo[i].reasons.push_back("100+ commits +5");
        }
        else if(repo[i].commits >= 50){

            repo[i].score += 3;
            repo[i].reasons.push_back("50+ commits +3");
        }
        else if(repo[i].commits >= 10){

            repo[i].score += 2;
            repo[i].reasons.push_back("10+ commits +2");
        }
        


        // Branches
        if(repo[i].branches >= 2){

            repo[i].score += 1;
            repo[i].reasons.push_back("2+ branches +1");
        }


        // Pull requests
        if(repo[i].pullRequests >= 1){

            repo[i].score += 2;
            repo[i].reasons.push_back("Pull requests present +2");
        }


        // Contributions
        if(repo[i].contributions >= 100){

            repo[i].score += 5;
            repo[i].reasons.push_back("100+ contributions +5");
        }
        else if(repo[i].contributions >= 50){

            repo[i].score += 4;
            repo[i].reasons.push_back("50+ contributions +4");
        }
        else if(repo[i].contributions >= 10){

            repo[i].score += 3;
            repo[i].reasons.push_back("10+ contributions +3");
        }
        else if(repo[i].contributions >= 5){

            repo[i].score += 1;
            repo[i].reasons.push_back("5+ contributions +1");
        }


        // Topic matching
        for(string topic : repo[i].topics){
            for(char &c:topic){
                c=tolower(c);
            }
            if(target == "cpp" &&
              (topic == "cpp" ||
               topic == "dsa" ||
               topic == "algorithm"||topic=="recursion"||topic=="stl"||topic=="data-structures"
            ||topic=="competitive-programming"||topic=="linked-list"||
        topic=="stack"||topic=="queue"||topic=="arrays")){

                repo[i].score += 2;
                repo[i].reasons.push_back("Relevant C++/DSA topic +2");
                break;
            }
            


            if(target == "web" &&
              (topic == "html" ||
               topic == "css" ||
               topic == "javascript")){

                repo[i].score += 2;
                repo[i].reasons.push_back("Relevant web topic +2");
                break;
            }


            if(target == "data" &&
              (topic == "python" ||
               topic == "pandas" ||
               topic == "sql")){

                repo[i].score += 2;
                repo[i].reasons.push_back("Relevant data topic +2");
                break;
            }
        }


        // Repository structure
        int filecount = repo[i].files.size();

        if(filecount == 0){

            repo[i].structure = "Empty";
        }

        else if(filecount <= 5){

            repo[i].structure = "Small";
        }

        else if(filecount <= 20){

            repo[i].structure = "Medium";
        }

        else{

            repo[i].structure = "Large";
        }


        // Folder scoring
        for(string folder : repo[i].folders){
            if(folder == "src"){
                repo[i].score += 2;
                repo[i].reasons.push_back("src folder +2");
            }

            if(folder == "include"){
                repo[i].score += 1;
                repo[i].reasons.push_back("include folder +1");
            }

            if(folder == "tests"){
                repo[i].score += 2;
                repo[i].reasons.push_back("tests folder +2");
            }

            if(folder == "docs"){
                repo[i].score += 1;
                repo[i].reasons.push_back("docs folder +1");
            }
        }
    }


    // Sort repositories by score
    sort(repo.begin(), repo.end(),
        [](const repository &a, const repository &b){

            if(a.score!=b.score){
                return a.score>b.score;
            }
            return a.name<b.name;
        });


    // Display ranking
    cout << "\n\n========== REPOSITORY RANKING ==========\n";

    for(int i = 0; i < repo.size(); i++){

        cout << "\nRank " << i + 1
             << ": " << repo[i].name

             << " | Score: "
             << repo[i].score

             << " | Files: "
             << repo[i].files.size()

             << " | Folders: "
             << repo[i].folders.size()

             << " | Structure: "
             << repo[i].structure

             << " | README: "
             << (repo[i].readme ? "Yes" : "No")

             << " | Gitignore: "
             << (repo[i].gitignore ? "Yes" : "No")

             << " | Source files: "
             << repo[i].sourceFiles

             << " | Forks: "
             << repo[i].forks
             << " | Commits:"<<repo[i].commits<<
             "| Branches:"<<repo[i].branches
             <<" | Pull Requests:"<<repo[i].pullRequests
             <<" | Contributions:"<<repo[i].contributions<<endl<<
            "Why this repository got this score:"<<endl;
            for(string reason :repo[i].reasons){
                cout<<" - "<<reason<<endl;
            }
    }return 0;
}