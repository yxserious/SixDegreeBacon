//
//  Actor.cpp
//  cse100pa4
//
//  Created by Kaijie Cai on 11/16/17.
//  Copyright © 2017 kj. All rights reserved.
//

#include "ActorNode.h"
#include <limits.h>

ActorNode::ActorNode(string n){
    name = n;
    visited = false;
    movies = new vector<Movie*>;
    edges = new vector<ActorEdge*>;
    
    prev = nullptr;
    edge = "";
    dist = INT_MAX;
    
    
    
    
    
    
    edges1 = new unordered_set<string>;
    end = n;
}

string ActorNode::getName() const{
    return name;
}

void ActorNode::clearVisited(){
    visited = false;
}


void ActorNode::markVisited(){
    visited = true;
}

bool ActorNode::isVisited(){
    return visited;
}

bool ActorNode::operator==(const ActorNode& a) const
{
    if (getName() == a.getName())
        return true;
    else
        return false;
}

ActorNode::~ActorNode(){
    delete movies;
}
