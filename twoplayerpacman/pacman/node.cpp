#include "node.h"
using namespace std;

node::node(int X, int Y, string path, int way){
    x=X;
    y=Y;
    ways = way;
    paths = path;
}
