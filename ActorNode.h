//
//  Actor.h
//  cse100pa4
//
//  Created by Kaijie Cai on 11/16/17.
//  Copyright © 2017 kj. All rights reserved.
//

#ifndef ActorNode_h
#define ActorNode_h
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <unordered_set>
#include "ActorEdge.h"
#include "Movie.h"

using namespace std;

class ActorNode{
    private:
        bool visited;
        string name;
    
    
    
    public:
    
        unordered_set<string>* edges1;
        string end;
    
    
    
    
        int dist;

        ActorNode* prev;
        string edge;

        vector<Movie*>* movies;
    
        vector<ActorEdge*>* edges;

        ActorNode(string n);
    
        void clearVisited();
        void markVisited();
        bool isVisited();
    
        string getName() const;
    
        ~ActorNode();
        bool operator==(const ActorNode & a) const;
};


#endif /* Actor_h */
