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
#include <vector>


int main(int argc, const char * argv[]) {
    // insert code here...
    RBTree<int> rb;
    int i = 20;
    srand(time(nullptr));
    while (i) {
        int k = rand() % 100;
        std::cout<<"插入=============="<<k<<std::endl;
        rb.insert(k);
        rb.printRBTree();
        --i;
    }
//    int de = 0;
//    std::vector<int> d = {27,12,9,3,7,8};
//    for(int de : d){
//        rb.deleteKey(de);
//        std::cout<<"删除=============="<<de<<std::endl;
//        rb.printRBTree();
//    }
//    while (std::cin>>de){
//        rb.deleteKey(de);
//        std::cout<<"删除=============="<<de<<std::endl;
//        rb.printRBTree();
//    }
    while (true){
        int de = rand() % 100;
        rb.deleteKey(de);
        std::cout<<"删除=============="<<de<<std::endl;
        rb.printRBTree();
    }
    std::cout << "Hello, World!\n";
    return 0;
}
