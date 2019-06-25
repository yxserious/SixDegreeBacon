//
//  Movie.cpp
//  cse100pa4
//
//  Created by Kaijie Cai on 11/16/17.
//  Copyright © 2017 kj. All rights reserved.
//

#include "Movie.h"

Movie::Movie(string n, int y){
    name = n;
    year = y;
    actors = new vector<string>;
    
}
string Movie::getKey() const{
    return name+"#@"+to_string(year);
}


bool Movie::operator==(const Movie & m) const
{
    if (getKey() == m.getKey())
        return true;
    else
        return false;
}

Movie::~Movie(){
    delete actors;
}


