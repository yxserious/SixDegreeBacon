/*
 * ActorGraph.cpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <queue>
#include <algorithm>
#include <time.h>
#include "ActorGraph.h"

using namespace std;

ActorGraph::ActorGraph(void) {}


bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
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
        //make movie name be title+year
        string movie_name = movie_title +"#@"+ to_string(movie_year);
        
        cur = actor_name;
        //if this is a new actor, then create a ActorNode for it and
        //add it to the actorlist
        if( cur != pre){
            ActorNode* newActor = new ActorNode(actor_name);
            actorList[actor_name] = newActor;
            pre = cur;
        }
        //if this is a new movie, then create a new movie object and then
        //add it to movielist
        if( movieList.find(movie_name) == movieList.end()){
            Movie* newMovie = new Movie(movie_title, movie_year);
            movieList[movie_name] = newMovie;
        }
        
        
        //build acotr-movie relationship but add movie to actor and actor to
        actorList[actor_name]->movies->push_back(movieList[movie_name]);
       
        movieList[movie_name]->actors->push_back(actor_name);

    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();
    
    

    return true;
}

bool movieCmptr(Movie* lhs ,Movie*  rhs)
{
    return lhs->year < rhs->year;
    
}


void ActorGraph::sortingMoviesList(){
    for(auto cur = movieList.begin(); cur != movieList.end(); cur++){
        sorted_moviesList.push_back(cur->second);
    }
    sort(sorted_moviesList.begin(), sorted_moviesList.end(), movieCmptr);
}

void ActorGraph::connectingActorNodes(){
    
    //iteract through all actors, and connect two actors if they have play
    //in same movie
    for(auto cur = actorList.begin(); cur != actorList.end(); cur++){
        //a list of movies cur has played
        vector<Movie*>* movies = cur->second->movies;
        
        //loop through the movies
        for(auto curMovie = movies->begin();
            curMovie!= movies->end(); curMovie++){
            //get the actors in that movie
            vector<string>* actors = (*curMovie)->actors;
            //loop through actors
            for(auto curActor_str = actors->begin();
                curActor_str != actors->end(); curActor_str++){
                //skip if encount cur itself
                if((*cur).first == *curActor_str)
                    continue;
                //clacule edge's weight
                int w_value = 1+ (2015 - (*curMovie)->year);
                
                //make edge
                string movie_name = (*curMovie)->getKey();
                auto edge =new ActorEdge(*curActor_str, movie_name, w_value);
                
                (*cur).second->edges->push_back(edge);
                edgeList.push_back(edge);
            }
            
        }
        
    }
    
}
