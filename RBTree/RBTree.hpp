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

/* 红黑树 */
template <class Key, class Compare = std::less<Key> >
class RBTree{
private:
    enum Color{Red=0,Black=1}; /*颜色定义*/
public:
    /*红黑树节点*/
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
    /*插入方法，主要处理红红冲突，当出现红红冲突时
     * 1。如果叔叔节点存在且为红，直接改变颜色，叔叔和父亲为黑，爷爷为红，自己为红。
     * 否则旋转。
     * 1。红红在左左 右旋
     * 2。红红在左右 左旋 右旋
     * 3。红红在右右 左旋
     * 4。红红在右左 右旋 左旋*/
    void insert(Key k);

    /*依照层次打印 挺好看的*/
    void printRBTree();
    /*删除节点
     * 删除阶段
     * 1。当删除的节点拥有两个子节点时需要找到替代节点（前驱或者后继）转换为2或者3
     * 2。当删除节点有子节点，用子节点代替，删除当前节点并调节
     * 2。1 删除节点时红色时不用管
     * 2。2 当删除节点是黑色时，后面节点必然是红色，由红黑树的删除必然是删除2-3-4树的叶子节点可知，直接将替代节点改为红色
     * 3。当删除节点是没有子节点时，
     * 3。1 如果是红色则，直接删除
     * 3。2 如果是黑色需要兄弟节点帮忙
     *    首先查找兄弟节点，如果兄弟节点是红色，则该节点不是兄弟节点(左右节点必然存在，且为黑色)，是2-3-4树的上一层节点。
     *    需要旋转一次，如果当前节点在左则左旋，在右则右旋，并改变颜色，则该兄弟节点会旋转为爷爷节点，并且真正的兄弟节点得到
     *    这时需要判断兄弟能否帮忙，如果兄弟有红色的子节点则可以帮忙，只有黑色，或者没有则不能
     *    3。2。1  如果可以帮忙，则旋转（node在左 左旋，反之亦然）即可达到左右平衡
     *    3。2。2  如果不可以帮忙，则需要设置兄弟节点为红色，使以父节点为根的子树达到平衡，设置需要调节的节点为父节点，
     *    并再一次向兄弟借。
     *    */
    void deleteKey(Key k);
    /*查找*/
    RBTreeNode *find(Key k);
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
typename RBTree<Key,Compare>::RBTreeNode *
 RBTree<Key,Compare>::find(Key k) {
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
    return node;
}

template <class Key, class Compare>
void RBTree<Key,Compare>::deleteKey(Key k){
    RBTreeNode *node = find(k);
    if(node == nullptr) return;
    if(node->left && node->right){
        auto next = findNext(node);
        node->k = next->k;
        node = next;
    }
    RBTreeNode * replaceNode = node->left?node->left:node->right; // 查找替代者
    if(replaceNode){// 如果替代者存在，则替代
        replaceNode->parent = node->parent;
        if(node->parent == nullptr){
            root = replaceNode;
        }else if(node == node->parent->left){
            node->parent->left = replaceNode;
        }else{
            node->parent->right = replaceNode;
        }
        node->parent = node->left = node->right = nullptr;
        if(node->color == Black) adjustAfterDelete(replaceNode); // replaceNode 一定是红色
        delete node;
    }else if (node->parent == nullptr){ // 如果该节点没有代替节点，且该节点是根节点
        root = nullptr;
        delete node;
    }else{
        // 叶子节点，且不为根节点
        if(node->color == Black) adjustAfterDelete(node);
        if(node->parent){
            if(node == node->parent->left)node->parent->left = nullptr;
            else if(node == node->parent->right) node->parent->right = nullptr;
        }
        node->parent = nullptr;
        delete node;
    }

}

/*删除后调整*/
template <class Key, class Compare>
void RBTree<Key,Compare>::adjustAfterDelete(RBTreeNode *node){

    while(node != root && node->color == Black){
        // 只能找兄弟
        if(node == node->parent->left){
            RBTreeNode *b = node->parent->right;
            if(b->color == Red){
                b->color = Black;
                node->parent->color = Red;
                leftRotate(node->parent);
                b = node->parent->right;
            }
            if((b->left && b->left->color == Red)||(b->right && b->right->color == Red)){
                // 当兄弟的孩子里面有兄弟能借
                if(b->right == nullptr || b->right->color == Black){
                    b->color = Red;
                    b->left->color = Black;
                    rightRotate(b);
                    b = node->parent->right;
                }
                b->color = node->parent->color;
                node->parent->color = Black;
                b->right->color = Black;
                leftRotate(node->parent);
                node = root;
            }else{
                b->color = Red;
                node = node->parent;
            }

        }else{
            RBTreeNode *b = node->parent->left;
            if(b->color == Red){
                b->color = Black;
                node->parent->color = Red;
                rightRotate(node->parent);
                b = node->parent->left;
            }
            if((b->right && b->right->color == Red)||(b->left && b->left->color == Red)){
                // 当兄弟的孩子里面有兄弟能借
                if(b->left == nullptr || b->left->color == Black){
                    b->color = Red;
                    b->right->color = Black;
                    leftRotate(b);
                    b = node->parent->left;
                }
                b->color = node->parent->color;
                node->parent->color = Black;
                b->left->color = Black;
                rightRotate(node->parent);
                node = root;
            }else{
                b->color = Red;
                node = node->parent;
            }
        }
    }
    //  自己能搞定， 即本身是红色或者有红色能够代替。
    node->color = Black;


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

int pow(int n, int p){
    int re = 1;
    while(p > 0){
        re *= n;
        --p;
    }
    return re;
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
    int i = 0;
    while (depth) {
        std::string sep;
        std::string str((pow(2,depth -1) - 1) * 2 ,' ');

        std::cout<<str;
        int size = (int)que.size();
        str = std::string((pow(2,depth) - 1) * 2, ' ');
        while(size){
            RBTreeNode *node = que.front();
            que.pop();
            if(node == nullptr){

                std::cout<<""<<"--"<<str;
                que.push(nullptr);
                que.push(nullptr);
            }else{
                if(node->color == Red){
//                    std::cout<<_RED<<node->k<<" "<<_RESET;
                    printf("\033[31m%02d%s\033[0m",node->k,str.data());

                }else{
//                    std::cout<<node->k<<" ";
                    printf("%02d%s",node->k,str.data());
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
