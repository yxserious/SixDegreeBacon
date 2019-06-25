//
//  actorconnections.cpp
//  cse100pa4
//
//  Created by Kaijie Cai on 12/1/17.
//  Copyright © 2017 kj. All rights reserved.
//


#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <unordered_set>
#include "ActorGraph.h"
using namespace std;

void reUnion(string actor, ActorNode* target,
             unordered_map<string, ActorNode*>* actorList){
    auto curSet = actorList->operator[](actor);
    while(curSet->prev != nullptr){
        auto prev = curSet;
        curSet = curSet->prev;
        prev->prev = target;
        
    }
}

string find(string actor, unordered_map<string, ActorNode*>* actorList){
    auto curSet = actorList->operator[](actor);
    while(curSet->prev != nullptr){
        curSet = curSet->prev;
      
    }
    reUnion(actor, curSet, actorList);
    
    return curSet->getName();
}

void unionNodes(string actor1, string actor2,
                unordered_map<string, ActorNode*>* actorList){
    string first_set = find(actor1, actorList);
    string second_set = find(actor2, actorList);
    if(first_set == second_set)
        return;
    actorList->operator[](first_set)->prev = actorList->operator[](second_set);

}


vector<vector<string>> loadingFile(ifstream& infile){
    bool have_header = false;
    vector<vector<string>> pairs;
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
        
        if (record.size() != 2) {
            // we should have exactly 3 columns
            continue;
        }
        
        pairs.push_back(record);
    }
    
    return pairs;
    
}

void print(string actor1, string actor2, int year, ofstream& out){
    out<<actor1<<"\t"<<actor2<<"\t"<<year<<endl;
    
}

void searching(string actor1, string actor2, ActorGraph& actorGraph,
               ofstream& out){
    auto actors = actorGraph.actorList;
    auto moviesList = actorGraph.sorted_moviesList;
    int year_prev = 1;
    int year_cur = 1;
    if(moviesList.size()!= 0){
        year_prev = year_cur = (*moviesList.begin())->year;
    }
    for(int k = 0; k < moviesList.size(); k++){
        auto movie = moviesList[k];
        year_cur = movie->year;
        if(year_prev!= year_cur){
            
            if(find(actor1, &actors)== find(actor2, &actors)){
                print(actor1, actor2, year_prev,out);
                
                return;
            }
            year_prev = year_cur;
        }
        auto actorsInMovie = movie->actors;
        for(int i = 1; i < actorsInMovie->size(); i++){
            unionNodes(actorsInMovie->operator[]((0)),
                       actorsInMovie->operator[](i),
                       &actors);
        }

        
    }
    print(actor1, actor2,9999,out);
}
bool BFS(string start, string end, ActorGraph& actorGraph){
    queue<ActorNode*> queue;
    auto actorlist = actorGraph.actorList;
    //mark all nodes as not visited
    for(auto cur = actorlist.begin(); cur != actorlist.end(); cur++){
        if((*cur).second->isVisited() && cur->second->edges->size()!= 0){
            queue.push(cur->second);
        }
    }
    while( !queue.empty()){
        ActorNode* curActor = queue.front();
        vector<ActorEdge*>* edges = curActor->edges;
        for( auto curEdge = edges->begin(); curEdge != edges->end(); curEdge++){
            ActorEdge* edge = *curEdge;
            string neighbor_str = edge->second_end;
            ActorNode* neighbor = actorlist[neighbor_str];
            if( neighbor->getName() == end)
                return true;
            if( neighbor->isVisited())
                continue;
            neighbor->markVisited();
            
            //push
            queue.push(neighbor);
        }
        delete edges;
        curActor->edges = new vector<ActorEdge*>;
        queue.pop();
    }
    
    return false;
}


void searchingBFS(string actor1, string actor2, ActorGraph& actorGraph,
                  ofstream& out){
    auto movies = actorGraph.sorted_moviesList;
    int year_prev = 1;
    int year_cur = 1;
    if(movies.size()!= 0){
        year_cur = year_prev = (*movies.begin())->year;
    }
    actorGraph.actorList[actor1]->markVisited();
    for(auto curMovie = movies.begin(); curMovie!= movies.end(); curMovie++){
        //actorslist in movie
        year_cur = (*curMovie)->year;
        if((year_cur != year_prev)){
            
            bool found = BFS(actor1, actor2, actorGraph);
            if(found)
            {
                print(actor1,actor2,year_prev,out);
                return ;
            }
        }
        
        year_prev = year_cur;
        
        vector<string>* actors = (*curMovie)->actors;
        
        bool visitedNode_existed = false;
        for( int i = 0; i < actors->size(); i++){
            ActorNode* node =actorGraph.actorList[actors->operator[](i)];
            if( node->isVisited()){
                visitedNode_existed = true;
                break;
            }
        }
        
        if(visitedNode_existed){
            for( int i = 0; i < actors->size(); i++){
                ActorNode* node =actorGraph.actorList[actors->operator[](i)];
                node->markVisited();
                if(node->getName() == actor2){
                    print(actor1,actor2,year_cur,out);
                    return ;
                }
                continue;
            }
        }
        ActorNode* cur = actorGraph.actorList[actors->operator[](0)];
        for(int j = 1; j < actors->size(); j++){
            ActorNode* next = actorGraph.actorList[actors->operator[](j)];
            
            auto edge1 = new ActorEdge(actors->operator[](j),
                                       "", 0);
            auto edge2 = new ActorEdge(actors->operator[](0),
                                       "", 0);
            
            cur->edges->push_back(edge1);
            next->edges->push_back(edge2);
        }
    }
    print(actor1,actor2,9999,out);
    
    
}

int main(int argc, const char * argv[]){
    
    if( argc != 5){
        cout<<"incorrect number of args, 4 needed"<<endl;
        return 0;
    }

    string pair_actors_str =argv[2];
    string output_str = argv[3];
    string isUfind = argv[4];
    
    //open pair_actors file
    ifstream pair_actors(pair_actors_str);
    if(! pair_actors){
        cout<<"cannot open pair_actors file"<<endl;
        return 0;
    }
    
    if( isUfind !="ufind" && isUfind != "bfs"){
        cout<<"please select ufind or bfs"<<endl;
        return 0;
    }
    
    
    ActorGraph actorGraph;
    
    actorGraph.loadFromFile(argv[1], true);
    actorGraph.sortingMoviesList();
    auto pairs = loadingFile(pair_actors);
    ofstream outfile(output_str);
    
    
    if(isUfind == "ufind"){
       
        auto actorsList = actorGraph.actorList;
        outfile<<"Actor1\tAcotr2\tYear"<<endl;
        for(int n = 0; n < pairs.size(); n++){
            string actor1 = pairs[n][0];
            string actor2 = pairs[n][1];
            searching(actor1, actor2, actorGraph,outfile);
            
            
            for(auto cur = actorsList.begin(); cur != actorsList.end(); cur++){
                cur->second->prev = nullptr;
            }

        }
    }
    else{
        auto actorsList = actorGraph.actorList;
        outfile<<"Actor1\tAcotr2\tYear"<<endl;
        for(int i = 0; i< pairs.size();i++){
            searchingBFS(pairs[i][0], pairs[i][1], actorGraph, outfile);

            for(auto cur = actorsList.begin(); cur != actorsList.end(); cur++){
                
                delete cur->second->edges;
                cur->second->edges = new vector<ActorEdge*>;
                (*cur).second->clearVisited();
                cur->second->end = cur->second->getName();
            }
            //all the actors, similar to actorgraph.cpp
            
            //for(auto cur = actorList.begin(); cur != actorList.end(); cur++){
            
            //movie is the one sorted by time order
            //looping through all the movies, stop if find
            
        }
    }
    

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

    
    outfile.close();
    pair_actors.close();
    
    
    return 0;
}
