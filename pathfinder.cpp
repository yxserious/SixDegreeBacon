//
//  pathfinder.cpp
//  cse100pa4
//
//  Created by Kaijie Cai on 11/22/17.
//  Copyright © 2017 kj. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <stack>
#include <limits.h>
#include <utility>
#include "ActorGraph.h"





using namespace std;

struct LessThanByDist
{
    bool operator()(const pair<int, ActorNode*>*  lhs ,
                    const pair<int, ActorNode*>*  rhs) const
    {
        return lhs->first > rhs->first;
        
    }
};


ActorNode* BFS(string end, string start, ActorGraph& actorGraph){
    cout<<"computing path for ("<<end<<") -> ("<<start<<")"<<endl;
    
    auto actorlist = actorGraph.actorList;
    ActorNode* startActor = actorGraph.actorList[start];
    //mark all nodes as not visited
    for(auto cur = actorlist.begin(); cur != actorlist.end(); cur++){
        (*cur).second->clearVisited();
        (*cur).second->edge = "";
        (*cur).second->prev = nullptr;
        
    }
    
    
    
    
    queue<ActorNode*> queue;
    queue.push(startActor);
    while( !queue.empty()){
        ActorNode* curActor = queue.front();
        curActor->markVisited();
        vector<ActorEdge*>* edges = curActor->edges;
        for( auto curEdge = edges->begin(); curEdge != edges->end(); curEdge++){
            ActorEdge* edge = *curEdge;
            string neighbor_str = edge->second_end;
            ActorNode* neighbor = actorlist[neighbor_str];
            if( neighbor->isVisited() || neighbor->prev != nullptr)
                continue;
            //set prev and edge that connect two nodes
            neighbor->prev = curActor;
            neighbor->edge = edge->movie;
            if( neighbor->getName() == end)
                return neighbor;
            //push
            queue.push(neighbor);
        }
        queue.pop();
    }
    
    return nullptr;
}

ActorNode* dijkstra(string end, string start, ActorGraph& actorGraph){
    cout<<"computing path for ("<<end<<") -> ("<<start<<")"<<endl;
    auto actorlist = actorGraph.actorList;
    
    ActorNode* startActor = actorGraph.actorList[start];
    //mark all nodes as not visited
    
    //clear(explored);
    
    for(auto cur = actorlist.begin(); cur != actorlist.end(); cur++){
        (*cur).second->clearVisited();
        (*cur).second->edge = "";
        (*cur).second->prev = nullptr;
        //max int in c++
        (*cur).second->dist = INT_MAX;
    }
    

    
    //Basically the same with the BST one, but need to consider distance
    priority_queue<pair<int, ActorNode*>*, vector<pair<int, ActorNode*>*>,
                        LessThanByDist> queue;
    //enqueue the first actor with distant
    
    /* set pair*/
    startActor->dist = 0;
    auto newPair = new pair<int, ActorNode*>(startActor->dist, startActor);
    //explored->push_back(startActor);
    
    queue.push(newPair);
    while( !queue.empty()){
        auto curActor = queue.top()->second;
        queue.pop();
        if( curActor->getName() == end)
            return curActor;
        
        curActor->markVisited();
        vector<ActorEdge*>* edges = curActor->edges;
        //always pop the first one
        for( int i = 0; i < edges->size(); i++){
            ActorEdge* edge = edges->operator[](i);
            
            string neighbor_str = edge->second_end;
            ActorNode* neighbor = actorlist[neighbor_str];
            if(neighbor->isVisited())
                continue;
            
            int weight = edge->weighted_value;
            //set prev and edge that connect two nodes
            //input distant value
            int total = curActor->dist + weight;
            
            //That's why set dist initial infinity
            //just for compare
            if(total < neighbor->dist )
            {
                neighbor->prev = curActor;
                neighbor->dist = total;
                neighbor->edge = edge->movie;
                
                auto newPair = new pair<int, ActorNode*>(neighbor->dist, neighbor);
                queue.push(newPair);
                
               // explored->push_back(neighbor);
            }

            
        }
    }

    return nullptr;
}


void printPath(ActorNode* target, ofstream& out){
    auto cur = target;
    while(cur != nullptr){
        out<<"("<<cur->getName()<<")";
        if(cur->edge != ""){
            out<<"--["<<cur->edge<<"]-->";
        }
        cur= cur->prev;
    }
    out<<endl;
    
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

int main(int argc, const char * argv[]){
    
    if( argc != 5){
        cout<<"incorrect number of args, 4 needed"<<endl;
        return 0;
    }
    
    string weighted_edg_flag_str = argv[2];
    string pair_actors_str = argv[3];
    string output_str = argv[4];
    
    
    //check u/w flag
    if(weighted_edg_flag_str!= "u" && weighted_edg_flag_str!= "w"){
        cout<<"flag must be 'u' or 'w'"<<endl;
        return 0;
    }
    
    //set use_weighted_edges
    bool use_weighted_edges = true;
    if( weighted_edg_flag_str == "u")
        use_weighted_edges = false;
    
    //open pair_actors file
    ifstream pair_actors(pair_actors_str);
    if(! pair_actors){
        cout<<"cannot open pair_actors file"<<endl;
        return 0;
    }
    
    
    ActorGraph actorGraph;

    actorGraph.loadFromFile(argv[1], use_weighted_edges);
    actorGraph.connectingActorNodes();
    
    cout<<"# of actors: "<<actorGraph.actorList.size()<<endl;
    cout<<"# of movies: "<<actorGraph.movieList.size()<<endl;
    cout<<"# of edges: "<<actorGraph.edgeList.size()<<endl;
    cout<<"done"<<endl;
    
    ofstream outfile(output_str);
    
    
    auto pairs = loadingFile(pair_actors);
    
    if(use_weighted_edges){
        outfile<<"(actor)--[movie#@year]-->(actor)--..."<<endl;
        for(int i = 0; i < pairs.size(); i++){
            auto node = dijkstra(pairs[i][0], pairs[i][1],actorGraph);
            printPath(node, outfile);
            
        }
    }
    else{
        outfile<<"(actor)--[movie#@year]-->(actor)--..."<<endl;
        for(int i = 0; i < pairs.size(); i++){
            auto node = BFS(pairs[i][0], pairs[i][1], actorGraph);
           
            printPath(node, outfile);
        }
    }
    
    outfile.close();
    pair_actors.close();
    
    
    return 0;
}
