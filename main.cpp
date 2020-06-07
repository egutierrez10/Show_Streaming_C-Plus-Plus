#include <iostream>
#include <fstream> 
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <cctype>
#include <regex>

#include "hash.h"
#include "hashmap.h"
using namespace std;

struct Show{
    int id, year;
    float imdb;
    string title,age,rotten;
    bool netflix, hulu, prime, disney; // 1 = yes, 0 = no as to what platforms it on
    Show(){
        id = 0, year = 0; 
        imdb = 0.0;
        title = "", age = "",rotten = "";
        netflix = false, hulu = false, prime = false, disney = false;
    }
};

bool isNumeric(string s){
    regex pattern("[[:d:]]+", regex::ECMAScript);

	smatch matchResults;  // provides more details on the match

	if (regex_match(s, matchResults, pattern))
		return true;
	else
		return false;
}

bool readFile(string fileName, hashmap<int, Show>& shows){
    ifstream infile(fileName);
    string line;
    int countShows = 0;
    if(!infile.good()){
        cout << "**Error: unable to open '" << fileName << "'..." << endl;
        return false;
    }

    string id, year;
    string imdb;
    string title, age, rotten;
    string netflix, hulu, prime, disney;
    string type;
    getline(infile, line);
    while(getline(infile, line)){
        stringstream s(line);
        getline(s, id, ',');
        getline(s, title, ',');
        getline(s, year, ',');
        getline(s, age, ',');
        getline(s, imdb, ',');
        getline(s, rotten, ',');
        getline(s, netflix, ',');
        getline(s, hulu, ',');
        getline(s, prime, ',');
        getline(s, disney, ',');
        getline(s, type, ',');
        
        Show show;
        show.id = stoi(id);
        show.title = title;
        show.year = stoi(year);
        show.age = age;
        show.imdb = stof(imdb);
        show.rotten = rotten;
        show.netflix = stoi(netflix);
        show.hulu = stoi(hulu);
        show.prime = stoi(prime);
        show.disney = stoi(disney);

        shows.insert(show.id, show, HashId);
        countShows++;
    }
    cout << "# of shows: " << countShows << endl;
    return true;
}

void displayCommands(){
    cout << "Available commands: " << endl;
    cout << "Enter 'help' to display all commands" << endl;
    cout << "Enter year to display all shows released (e.g. year 2019)" << endl;
    cout << "Enter to search for specific age group (e.g. 18)" << endl;
    cout << "Enter age to search age group greater than or equal to (e.g. >19)" << endl;
    cout << "Enter IMDB rating for really high rated shows (e.g. rating 8.0)" << endl;
    cout << "Enter Rotten Tomatoes rating to find really good shows (e.g. rotten 90)" << endl;
}

void searchYear(string command, hashmap<int, Show>& shows){
    stringstream ss(command);
    string com, year;
    getline(ss, com, ' ');
    getline(ss, year, ' ');
    int yr;
    if(year.length() <= 3 || year.length() > 4){
        cout << "Invalid year entered" << endl;
        return;
    }
    try{
        yr = stoi(year);
    }catch(exception e){
        cout << "Invalid year entered" << endl;
        return;
    }
    
    vector<Show> yearShows;
    unordered_set<int> keys;
    Show show;
    keys = shows.getKeys();
    for(auto itr = keys.begin(); itr != keys.end(); itr++){
        bool found = shows.search(*itr, show,HashId);
        if(!found)
            continue;
        if(show.year == yr)
            yearShows.push_back(show);
    }  

    cout << "Shows at your year request of: " << yr << endl;
    for(auto i = yearShows.begin(); i != yearShows.end();++i){
        cout << "Show Name: " << (*i).title << endl;
        string streaming = (*i).netflix == 1 ? "yes":"no";
        cout << "   Netflix: " << streaming << endl;
        streaming = (*i).hulu == 1 ? "yes":"no";
        cout << "   Hulu: " << streaming << endl;
        streaming = (*i).prime == 1 ? "yes":"no";
        cout << "   Amazon Prime: " << streaming << endl;
        streaming = (*i).disney == 1 ? "yes":"no";
        cout << "   Disney+: " << streaming << endl;
        cout << endl;
    }
}

void ageSearch(string command, hashmap<int, Show>& shows){
    int age = stoi(command);
    int curAge;
    vector<Show> ageShows;
    unordered_set<int> keys;
    Show show;
    keys = shows.getKeys();
    for(auto itr = keys.begin(); itr != keys.end(); itr++){
        bool found = shows.search(*itr, show,HashId);
        if(!found)
            continue;
        if(show.age == "-"){
            continue;
        }else if(show.age.length() == 3 && show.age != "all"){
            curAge = stoi(show.age.substr(0,2));
            if(age == curAge)
                ageShows.push_back(show);
        }else if(show.age.length() == 2){
             curAge = stoi(show.age.substr(0,1));
            if(age == curAge)
                ageShows.push_back(show);
        }else{ //all age group
            ageShows.push_back(show);
        }
    }   

    cout << "Shows at your age request of: " << age << endl;
    for(auto i = ageShows.begin(); i != ageShows.end();++i){
        cout << "Show Name: " << (*i).title << endl;
        string streaming = (*i).netflix == 1 ? "yes":"no";
        cout << "   Netflix: " << streaming << endl;
        streaming = (*i).hulu == 1 ? "yes":"no";
        cout << "   Hulu: " << streaming << endl;
        streaming = (*i).prime == 1 ? "yes":"no";
        cout << "   Amazon Prime: " << streaming << endl;
        streaming = (*i).disney == 1 ? "yes":"no";
        cout << "   Disney+: " << streaming << endl;
        cout << endl;
    }
}

void greaterAgeSearch(string command, hashmap<int, Show>& shows){
    command = command.erase(0,1);
    int age = stoi(command);
    int curAge;
    vector<Show> ageShows;
    unordered_set<int> keys;
    Show show;
    keys = shows.getKeys();
    for(auto itr = keys.begin(); itr != keys.end(); itr++){
        bool found = shows.search(*itr, show,HashId);
        if(!found)
            continue;
        if(show.age == "-"){
            continue;
        }else if(show.age.length() == 3 && show.age != "all"){
            curAge = stoi(show.age.substr(0,2));
            if(age <= curAge)
                ageShows.push_back(show);
        }else if(show.age.length() == 2){
             curAge = stoi(show.age.substr(0,1));
            if(age <= curAge)
                ageShows.push_back(show);
        }else{ //all age group
            ageShows.push_back(show);
        }
    }   

    cout << "Shows at your age request of: " << age << endl;
    for(auto i = ageShows.begin(); i != ageShows.end();++i){
        cout << "Show Name: " << (*i).title << endl;
        string streaming = (*i).netflix == 1 ? "yes":"no";
        cout << "   Netflix: " << streaming << endl;
        streaming = (*i).hulu == 1 ? "yes":"no";
        cout << "   Hulu: " << streaming << endl;
        streaming = (*i).prime == 1 ? "yes":"no";
        cout << "   Amazon Prime: " << streaming << endl;
        streaming = (*i).disney == 1 ? "yes":"no";
        cout << "   Disney+: " << streaming << endl;
        cout << endl;
    }
}

void searchRating(string command, hashmap<int, Show>& shows){
    stringstream ss(command);
    string com, rating;
    getline(ss, com, ' ');
    getline(ss, rating, ' ');
    float r;
    try{
        r = stof(rating);
    }catch(exception e){
        cout << "Invalid rating entered" << endl;
        return;
    }
    vector<Show> ratingShows;
    unordered_set<int> keys;
    Show show;
    keys = shows.getKeys();
    for(auto itr = keys.begin(); itr != keys.end(); itr++){
        bool found = shows.search(*itr, show,HashId);
        if(!found)
            continue;
        float curR = show.imdb;
        if(curR == r)
            ratingShows.push_back(show);
    }

    cout << "Shows at your rating request of: " << r << endl;
    for(auto i = ratingShows.begin(); i != ratingShows.end();++i){
        cout << "Show Name: " << (*i).title << endl;
        string streaming = (*i).netflix == 1 ? "yes":"no";
        cout << "   Netflix: " << streaming << endl;
        streaming = (*i).hulu == 1 ? "yes":"no";
        cout << "   Hulu: " << streaming << endl;
        streaming = (*i).prime == 1 ? "yes":"no";
        cout << "   Amazon Prime: " << streaming << endl;
        streaming = (*i).disney == 1 ? "yes":"no";
        cout << "   Disney+: " << streaming << endl;
        cout << endl;
    }
}

void searchRotten(string command, hashmap<int, Show>& shows){
    stringstream ss(command);
    string com, rating;
    getline(ss, com, ' ');
    getline(ss, rating, ' ');
    float r;
    try{
        r = stof(rating);
    }catch(exception e){
        cout << "Invalid rating entered" << endl;
        return;
    }
    vector<Show> rotShows;
    unordered_set<int> keys;
    Show show;
    keys = shows.getKeys();
    for(auto itr = keys.begin(); itr != keys.end(); itr++){
        bool found = shows.search(*itr, show,HashId);
        if(!found)
            continue;
        string rotten = show.rotten;
        if(rotten == "-"){
            continue;
        }
        float curR = stof(rotten.substr(0,2));
        if(curR == r)
            rotShows.push_back(show);
    }

    cout << "Shows at your rotten rating request of: " << r << endl;
    for(auto i = rotShows.begin(); i != rotShows.end();++i){
        cout << "Show Name: " << (*i).title << endl;
        string streaming = (*i).netflix == 1 ? "yes":"no";
        cout << "   Netflix: " << streaming << endl;
        streaming = (*i).hulu == 1 ? "yes":"no";
        cout << "   Hulu: " << streaming << endl;
        streaming = (*i).prime == 1 ? "yes":"no";
        cout << "   Amazon Prime: " << streaming << endl;
        streaming = (*i).disney == 1 ? "yes":"no";
        cout << "   Disney+: " << streaming << endl;
        cout << endl;
    }
}

void searchTitle(string command, hashmap<int, Show>& shows){
    vector<Show> titleShows;
    unordered_set<int> keys;
    Show show;
    keys = shows.getKeys();
    for(auto itr = keys.begin(); itr != keys.end(); itr++){
        bool found = shows.search(*itr, show,HashId);
        if(!found)
            continue;
        if(show.title.find(command) != string::npos)
            titleShows.push_back(show);
    }

    cout << "Shows at your title request of: " << command << endl;
    for(auto i = titleShows.begin(); i != titleShows.end();++i){
        cout << "Show Name: " << (*i).title << endl;
        string streaming = (*i).netflix == 1 ? "yes":"no";
        cout << "   Netflix: " << streaming << endl;
        streaming = (*i).hulu == 1 ? "yes":"no";
        cout << "   Hulu: " << streaming << endl;
        streaming = (*i).prime == 1 ? "yes":"no";
        cout << "   Amazon Prime: " << streaming << endl;
        streaming = (*i).disney == 1 ? "yes":"no";
        cout << "   Disney+: " << streaming << endl;
        cout << endl;
    }
}

int main(){
    const int N = 6000;
    hashmap<int, Show> shows(N);
    cout << "**Show Streaming Analysis Program**" << endl;
    string fileName = "Show_Streaming.csv";
    cout << "Reading in file " << fileName << "...." << endl;
    bool success = readFile(fileName, shows);
    if(!success){
        cout << "File error occurred. Exiting program..." << endl;
        return 0;
    }
    string command;
    cout << "What we in the mood for today?" << endl;
    cout << "Please enter a command, help, or #> ";
    getline(cin, command);
    while(command != "#"){
        if(command == "help"){
            displayCommands();
        }else if(command.substr(0,4) == "year"){
            searchYear(command, shows);
        }else if(isNumeric(command)){
            ageSearch(command, shows);
        }else if(command[0] == '>'){
            greaterAgeSearch(command, shows);
        }else if(command.substr(0,6) == "rating"){
            searchRating(command, shows);
        }else if(command.substr(0,6) == "rotten"){
            searchRotten(command, shows);
        }else{
            searchTitle(command, shows);
        }
        cout << endl;
        cout << "What we in the mood for today?" << endl;
        cout << "Please enter a command, help, or #> ";
        getline(cin, command);
    }
    return 0;
}