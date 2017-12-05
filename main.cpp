#include <iostream>
#include "o2tree/o2tree.hpp"

yazik::o2tree::t<int> tree{1024};

int main() {
    tree.put(0,0);
    std::cout<<"Hello, World!"<<std::endl;
    return 0;
}