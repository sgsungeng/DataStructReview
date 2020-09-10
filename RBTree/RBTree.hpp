//
//  RBTree.h
//  RBTree
//
//  Created by apple008 on 2020/9/10.
//  Copyright © 2020 sungeng. All rights reserved.
//

#ifndef RBTree_hpp
#define RBTree_hpp

#include <iostream>
#include <iostream>
#include <cstdlib>
#include <queue> // 打印调试时使用
#include <string> // 打印二叉树时使用


template <class Key, class Compare = std::less<Key> >
class RBTree{
private:
    enum Color{Red=0,Black=1};
public:
    class RBTreeNode{
    public:
        RBTreeNode *left;
        RBTreeNode *right;
        RBTreeNode *parent;
        Key k;
        Color color;
        explicit RBTreeNode(Key k):k(k),left(nullptr),right(nullptr),parent(nullptr),color(Red){};
        virtual ~RBTreeNode(){
            if(left) delete left;
            if(right) delete right;
            if(parent) delete parent;
        }
    };
    void insert(Key k);

    /// 依照层次打印
    void printRBTree();
    /// 删除节点
    void deleteKey(Key k);
private:
    RBTreeNode *root = nullptr;
    Compare cmp = Compare();

    /*右旋转*/
    void rightRotate(RBTreeNode *node);

    /*左旋转*/
    void leftRotate(RBTreeNode *node);

    /**
     插入后调整
     @param node 插入的节点
     */
    void adjustAfterInsert(RBTreeNode *node);
    /// 获取深度
    int  getDepth(RBTreeNode *node);

    /*查找前驱*/
    RBTreeNode * findPre(RBTreeNode *node);

    /*查找后继*/
    RBTreeNode * findNext(RBTreeNode *node);

    /*删除后调整*/
    void adjustAfterDelete(RBTreeNode *node);
};


template <class Key, class Compare>
void RBTree<Key,Compare>::deleteKey(Key k){
    RBTreeNode *node = root;
    while(node){
        if(node->k == k){
            break;
        }else if(k < node->k){
            node = node ->left;
        }else{
            node = node->right;
        }
    }
    if(node == nullptr) return;
    if(node->left && node->right){
        auto next = findNext(node);
        node->k = next->k;
        node = next;
    }
    if(node->color == Red){
        if(node->left){
            node->left->parent = node->parent;
            if(node->parent) {
                if(node == node->parent->left){
                    node->parent->left = node->left;
                }else {
                    node->parent->right = node->left;
                }
                node->parent = nullptr;
            }
            node->left = nullptr;
            delete node;
        }else if(node->right) {
            node->right->parent = node->parent;
            if(node->parent) {
                if(node == node->parent->left){
                    node->parent->left = node->right;
                }else {
                    node->parent->right = node->right;
                }
                node->parent = nullptr;
            }
            node->right = nullptr;
            delete node;
        }else{
            if(node == node->parent->left){
                node->parent->left = nullptr;
            }else{
                node->parent->left = nullptr;
            }
            node->parent = nullptr;
        }
    }else{
        adjustAfterDelete(node);
    }
}

/*删除后调整*/
template <class Key, class Compare>
void RBTree<Key,Compare>::adjustAfterDelete(RBTreeNode *node){
    if (node->parent){
        
    }



}

template <class Key, class Compare>
void RBTree<Key,Compare>::insert(Key k){
    if(root == NULL){
        root = new RBTreeNode(k);
        root->color = Black;
        return;
    }
    RBTreeNode *t = root;
    while (t) { // 查找插入位置的父节点位置
        if(Compare()(k,t->k)){
            if(t->left) t = t->left;
            else break;
        }else{
            if(t->right)t = t->right;
            else break;
        }
    }
    auto newNode = new RBTreeNode(k);
    if(t->k > k) t->left = newNode;
    else t->right = newNode;
    newNode->parent = t;
    adjustAfterInsert(newNode);
}


template <class Key,class Compare>
void
RBTree<Key,Compare>::rightRotate(RBTreeNode *node){
    RBTreeNode *parent = node->parent;
    RBTreeNode *left = node->left;
    if(left == nullptr) return;
    node->left = left->right;
    if(left->right)left->right->parent = node;
    left->right = node;
    node->parent = left;
    if(parent){
        left->parent = parent;
        if(node == parent->left)
            parent->left = left;
        else
            parent->right = left;
    }else{
        root = left;
        root->parent = nullptr;
    }
}

template <class Key,class Compare>
void
RBTree<Key,Compare>::leftRotate(RBTreeNode *node){
    RBTreeNode *parent = node->parent;
    RBTreeNode *right = node->right;
    if(right == nullptr) return;
    node->right = right->left;
    if(right->left)right->left->parent = node;
    right->left = node;
    node->parent = right;
    if(parent){
        right->parent = parent;
        if(node == parent->left)
            parent->left = right;
        else
            parent->right = right;
    }else{
        root = right;
        root->parent = nullptr;
    }
}

template <class Key,class Compare>
void
RBTree<Key,Compare>::adjustAfterInsert(RBTreeNode *node){
    while (node != root && node->parent->color == Red) {
        // 当该节点不为根节点，则必有父亲节点
        // 当父节点为红时需要调整
        if(node->parent == node->parent->parent->left){
            // 当该父亲节点为爷爷节点的左孩子时

            // 当存在叔叔节点时
            if(node->parent->parent->right && node->parent->parent->right->color == Red){
                    node->parent->parent->right->color = Black;
                    node->parent->color = Black;
                    node->parent->parent->color = Red;
                    // 调整完成后需要判断爷爷节点和爷爷的爸爸节点是否冲突。
                    node = node->parent->parent;
            }else{//  不存在叔叔节点时需要旋转
                if(node == node->parent->right){
                    leftRotate(node->parent);
                    node = node->left;
                }
                node->parent->color = Black;
                node->parent->parent->color = Red;
                rightRotate(node->parent->parent);
            }
        }else{
            // 当父亲节点为爷爷节点的右孩子时

            // 当存在叔叔节点时,且叔叔是红色，直接更换颜色
            if(node->parent->parent->left && node->parent->parent->left->color == Red){

                node->parent->parent->left->color = Black;
                node->parent->color = Black;
                node->parent->parent->color = Red;
                // 调整完成后需要判断爷爷节点和爷爷的爸爸节点是否冲突。
                node = node->parent->parent;


            }else{//  不存在叔叔节点需要旋转
                if(node == node->parent->left){
                    rightRotate(node->parent);
                    node = node->right;
                }
                node->parent->color = Black;
                node->parent->parent->color = Red;
                leftRotate(node->parent->parent);
            }
        }

    }

    root->color = Black;
}



template <class Key,class Compare>
void RBTree<Key,Compare>::printRBTree(){

#define _RESET   "\033[0m "
#define _BLACK   "\033[30m"      /* Black */
#define _RED     "\033[31m "      /* Red */

    RBTreeNode* node = root;
    if(node == nullptr) return;
    int depth = getDepth(node);
    std::queue<RBTreeNode *>que;
    que.push(node);
    while (depth) {
        int size = (int)que.size();
        while(size){
            RBTreeNode *node = que.front();
            que.pop();
            if(node == nullptr){

                std::cout<<"-"<<" ";
                que.push(nullptr);
                que.push(nullptr);
            }else{
                if(node->color == Red){
                    std::cout<<_RED<<node->k<<" "<<_RESET;
//                    printf("\033[31m %d ",node->k);
                }else{
                    std::cout<<node->k<<" ";
                }
                que.push(node->left);
                que.push(node->right);
            }
            --size;
        }
        --depth;
        std::cout<<std::endl;
    }
}

template <class Key,class Compare>
int
RBTree<Key,Compare>::getDepth(RBTreeNode *node){
    if(node == nullptr)return 0;
    return std::max(getDepth(node->left),getDepth(node->right)) + 1;
}


/*查找前驱*/
template <class Key,class Compare>
typename RBTree<Key,Compare>::RBTreeNode *
RBTree<Key,Compare>::findPre(RBTreeNode *node){
    if(node == nullptr)return nullptr;
    if(node->left != nullptr){
        node = node->left;
        while (node->right)node = node->right;
    }else{
        while (node && node == node->parent->left)node = node->parent;
    }
    return node;
}

/*查找后继*/
template <class Key,class Compare>
typename RBTree<Key,Compare>::RBTreeNode *
RBTree<Key,Compare>::findNext(RBTreeNode *node){
    if(node == nullptr)return nullptr;
    if(node->right != nullptr){
        node = node->right;
        while (node->left)node = node->left;
    }else{
        while (node && node == node->parent->right)node = node->parent;
    }
    return node;
}
#endif /* RBTree_hpp */
