//
//  ActorEdge.cpp
//  cse100pa4
//
//  Created by Kaijie Cai on 11/22/17.
//  Copyright © 2017 kj. All rights reserved.
//

#include <stdio.h>
#include "ActorEdge.h"

ActorEdge::ActorEdge(string second, string m, int v){
    second_end = second;
    movie = m;
    weighted_value = v;
}

ActorEdge::~ActorEdge(){
}
