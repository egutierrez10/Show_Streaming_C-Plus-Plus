/*main.cpp*/

// Everardo Gutierrez
// Project Show Streaming LookUp
//  Program is to read in file containing shows streamed on multiple platforms. 
//  User can enter in multiple commands to get show titles based on criteria
//  entered and show what streaming platforms they are available on.
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

//
// Show Structure
//
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

/*
    Function: isNumeric 
    Use regex to determine if string passed as parameter is an integer
    Parameters:
        s - string 
    Returns:
        boolean true | false
*/
bool isNumeric(string s){
    regex pattern("[[:d:]]+", regex::ECMAScript);

	smatch matchResults;  // provides more details on the match

	if (regex_match(s, matchResults, pattern))
		return true;
	else
		return false;
}

/*
    Function: readFile 
    Read data from the file passed and create an instance of show structure type.
    Instance is then inserted into the hash table based on the id that gets hashed.
    Parameters:
        fileName - string
        shows - hashmap 
    Returns:
        boolean - true | false
*/
bool readFile(string fileName, hashmap<int, Show>& shows){
    ifstream infile(fileName);
    string line;
    int countShows = 0;
    if(!infile.good()){ // file could not be opened return false
        cout << "**Error: unable to open '" << fileName << "'..." << endl;
        return false;
    }

    string id, year;
    string imdb;
    string title, age, rotten;
    string netflix, hulu, prime, disney;
    string type;
    getline(infile, line); // skip first line (column names)
    while(getline(infile, line)){ // read each line from the line
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
        
        // create instance of show and set its values to the ones read
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

        // insert show into the hash table 
        shows.insert(show.id, show, HashId);
        countShows++; //update number of shows read
    }
    // Display the number of shows read from the file
    cout << "# of shows: " << countShows << endl;
    return true;
}

/*
    Function: displayCommands 
    Display commands available for user to enter
    Parameters:
        none
    Returns:
        void
*/
void displayCommands(){
    cout << "Available commands: " << endl;
    cout << "Enter 'help' to display all commands" << endl;
    cout << "Enter year to display all shows released (e.g. year 2019)" << endl;
    cout << "Enter to search for specific age group (e.g. 18)" << endl;
    cout << "Enter age to search age group greater than or equal to (e.g. >19)" << endl;
    cout << "Enter IMDB rating for really high rated shows (e.g. rating 8.0)" << endl;
    cout << "Enter Rotten Tomatoes rating to find really good shows (e.g. rotten 90)" << endl;
}

/*
    Function: searchYear
    Search for a particular year passed as parameter in the hash table
    Displays all the shows that match the year passed.
    Parameters:
        command - string
        shows - hashmap 
    Returns:
        none
*/
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
    
    vector<Show> yearShows; // vector to hold all movies that meet criteria passed
    unordered_set<int> keys; // holds all keys from the hash table
    Show show;
    keys = shows.getKeys(); // get all keys from the hash table
    // loop through each key
    for(auto itr = keys.begin(); itr != keys.end(); itr++){
        // search for the key in the hash table
        bool found = shows.search(*itr, show,HashId);
        if(!found) // key could not be found move on to the next key
            continue;
        if(show.year == yr) // key found determine if year meets criteria passed
            yearShows.push_back(show);
    }  

    // loop through the vector that holds all show struct instances 
    // displaying show title and what platforms they are streamed on
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

/*
    Function: ageSearch
    Search for a particular age rating passed as parameter in the hash table
    Displays all the shows that match the age rating passed.
    Parameters:
        command - string
        shows - hashmap 
    Returns:
        none
*/
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

/*
    Function: greaterAgeSearch
    Search for a all ages greater than or equal to the one passed as parameter 
    in the hash table. Displays all the shows that match this criteria.
    Parameters:
        command - string
        shows - hashmap 
    Returns:
        none
*/
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

/*
    Function: searchRating
    Search for a particular IMDb rating passed as parameter in the hash table
    Displays all the shows that match the IMDb rating passed.
    Parameters:
        command - string
        shows - hashmap 
    Returns:
        none
*/
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

/*
    Function: searchRotten
    Search for a particular Rotten Tomatoe rating passed as parameter in the hash table
    Displays all the shows that match the Rotten Tomatoe rating passed.
    Parameters:
        command - string
        shows - hashmap 
    Returns:
        none
*/
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

/*
    Function: searchTitle
    Search for a particular show title passed as parameter in the hash table
    Displays all the shows that match the show title passed.
    Parameters:
        command - string
        shows - hashmap 
    Returns:
        none
*/
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
    if(!success){ // determine if file can be read 
        cout << "File error occurred. Exiting program..." << endl;
        return 0;
    }

    string command;
    cout << "What we in the mood for today?" << endl;
    cout << "Please enter a command, help, or #> ";
    getline(cin, command); // read in users desired command

    while(command != "#"){ // determine if user has asked to quit searching
        if(command == "help"){ // display commands available
            displayCommands();
        }else if(command.substr(0,4) == "year"){ // user wants to search by year
            searchYear(command, shows);
        }else if(isNumeric(command)){ // user wants to search by age rating
            ageSearch(command, shows);
        }else if(command[0] == '>'){ // user wants to search by age rating greater than one entered
            greaterAgeSearch(command, shows);
        }else if(command.substr(0,6) == "rating"){ // user wants to search by IMDb rating
            searchRating(command, shows);
        }else if(command.substr(0,6) == "rotten"){ // user wants to search by rotten tomatoe rating
            searchRotten(command, shows);
        }else{ // default will look for show title 
            searchTitle(command, shows);
        }
        // request users input once again
        cout << endl;
        cout << "What we in the mood for today?" << endl;
        cout << "Please enter a command, help, or #> ";
        getline(cin, command);
    }
    return 0;
}