#include "node.h"
using namespace std;

node::node(int X, int Y, vector<bool> path, int way){
    x=X;
    y=Y;
    ways = way;
    paths = path;
}