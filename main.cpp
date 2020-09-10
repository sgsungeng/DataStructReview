//
//  main.cpp
//  RBTree
//
//  Created by apple008 on 2020/9/10.
//  Copyright © 2020 sungeng. All rights reserved.
//

#include <iostream>
#include <ctime>
#include "RBTree.hpp"


int main(int argc, const char * argv[]) {
    // insert code here...
    RBTree<int> rb;
    int i = 20;
    while (i) {
        int k = rand() % 50;
        std::cout<<"=============="<<k<<std::endl;
        rb.insert(k);
        rb.printRBTree();
        --i;
    }


    std::cout << "Hello, World!\n";
    return 0;
}
