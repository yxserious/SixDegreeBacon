//
//  main.cpp
//  cse100pa4
//
//  Created by Kaijie Cai on 11/16/17.
//  Copyright © 2017 kj. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <queue>
#include <unordered_map>
#include <unordered_set>
using namespace std;
class ActorGraph {
protected:
    
    
    // Maybe add class data structure(s) here
    
    //inner class movie which is a pair with name of movie and year
public:
    
    
    //a map of actors use actor name as key and actor class as value
    unordered_map<string, vector<string>*> actor_in_movies;
    //a map of movies use movie as key and movie class as value
    unordered_map<string, vector<string>*> movies2;
    
    
    ActorGraph(void){};

    bool loadFromFile(const char* in_filename, bool use_weighted_edges) {
        // Initialize the file stream
        ifstream infile(in_filename);
        
        bool have_header = false;
        
        //pre actor we were loading
        string pre = "";
        //currrent actor we are loading
        string cur = "";
        
        // keep reading lines until the end of file is reached
        while (infile) {
            string s;
            
            // get the next line
            if (!getline( infile, s )) break;
            
            if (!have_header) {
                // skip the header
                have_header = true;
                continue;
            }
            
            istringstream ss( s );
            vector <string> record;
            
            
            
            while (ss) {
                string next;
                
                // get the next string before hitting a tab character and put it in 'next'
                if (!getline( ss, next, '\t' )) break;
                
                record.push_back( next );
            }
            
            if (record.size() != 3) {
                // we should have exactly 3 columns
                continue;
            }
            
            string actor_name(record[0]);
            string movie_title(record[1]);
            int movie_year = stoi(record[2]);
            string movie_name = movie_title + to_string(movie_year);
            
            cur = actor_name;
            if( cur != pre){
                //Actor* newActor = new Actor(actor_name);
                vector<string>* m = new vector<string>;
                actor_in_movies[cur] = m;
                pre = cur;
            }
            
            if( movies2.find(movie_name) == movies2.end()){
                //Movie* newMovie = new Movie(movie_title,movie_year);
                vector<string>* a = new vector<string>;
                movies2[movie_name] = a;
            }
            
            actor_in_movies[cur]->push_back(movie_name);
            movies2[movie_name]->push_back(cur);
        }
        //connectNeighbors1();
        
        if (!infile.eof()) {
            cerr << "Failed to read " << in_filename << "!\n";
            return false;
        }
        infile.close();
        
        return true;
    }
};

int countUniqueNumber(string name, ActorGraph& actorGraph){
    //Actor* actor = actorGraph.actors1[name];
    vector<string>* movies = actorGraph.actor_in_movies[name];
    int count  = 0;
    
    unordered_set<string> uniqueNeighbor;
    for( int i = 0; i < movies->size(); i++){
        string movie = movies->operator[](i);
        vector<string>* actors = actorGraph.movies2[movie];
        
        for( int j = 0; j < actors->size(); j++){
            if(uniqueNeighbor.find(actors->operator[](j)) == uniqueNeighbor.end()
               && actors->operator[](j) != name){
                uniqueNeighbor.insert(actors->operator[](j));
                count++;
            }
        }
        
    }
    
    return count;
}

int main1(int argc, const char * argv[]) {
    
    if( argc != 4){
        cout<<"incorrect number of commands entered"<<endl;
        return  0;
    }
    
    string source_neighbors = argv[2];
    string ofile_str = argv[3];
    
    ActorGraph actorGraph = ActorGraph();
    actorGraph.loadFromFile(argv[1], false);

    
    
    ifstream infile(source_neighbors);
    
    bool have_header = false;
    vector <string> record;
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
        
        // get the next line
        if (!getline( infile, s )) break;
        
        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }
        record.push_back(s);
    }
    
    
    ofstream outfile(ofile_str);
    outfile<<"Number of Neighbors"<<endl;
    
    for(int i = 0; i < record.size(); i++){
        string str = record[i];
        //Actor* a = new Actor(str);
       // auto f = actorGraph.actors.find(a);
        outfile<<countUniqueNumber(str, actorGraph)<<endl;
        //outfile<<(f->second)->neighbors->size()<<endl;
        
    }
    
    infile.close();
    outfile.close();
    
    
    
    
    
    
    return 0;
}
