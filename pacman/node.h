#include<string>
#include<vector>
using namespace std;

#ifndef nod_H
#define nod_H

class node{
    public:
        int x;  //xcoordinate
        int y;  //ycoordinate
        int ways;
        vector<bool> paths;
        node(int X, int Y, vector<bool> path, int way);
};

#endif