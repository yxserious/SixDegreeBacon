//
//  Movie.h
//  cse100pa4
//
//  Created by Kaijie Cai on 11/16/17.
//  Copyright © 2017 kj. All rights reserved.
//

#ifndef Movie_h
#define Movie_h
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Movie{
    public:
        string name;
        int year;
        vector<string>* actors;
    
        Movie(){};
        Movie(string n, int y);
        string getKey() const;

    
        bool operator==(const Movie & m) const;
        ~Movie();
};



#endif /* Movie_h */
