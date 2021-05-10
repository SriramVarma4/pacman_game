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
        string paths;
        node(int X, int Y, string path, int way);
};

#endif
