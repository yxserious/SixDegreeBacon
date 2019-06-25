//
//  ActorEdge.h
//  cse100pa4
//
//  Created by Kaijie Cai on 11/22/17.
//  Copyright © 2017 kj. All rights reserved.
//

#ifndef ActorEdge_h
#define ActorEdge_h
#include <iostream>
#include <vector>

using namespace std;
class ActorEdge{
    
public:
    //string first_end;
    string second_end;
    string movie;
    int weighted_value;
    
    ActorEdge( string second, string m, int v);
    
    ~ActorEdge();

};


#endif /* ActorEdge_h */
