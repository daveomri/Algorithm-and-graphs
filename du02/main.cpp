#include <iostream>
using namespace std;

/**
 * This class represents an avl tree
 * this class can be construct with diferent nodes, given by template
 */
template <class T>
class AVLTree{
  public:
    T * root;
    T * nodeToRemove;

    AVLTree(){
      root = NULL;
      nodeToRemove = NULL;
    }
    ~AVLTree(){
      if (root!=NULL)
        deleteNode(root);
      root = NULL;
    }
    void deleteNode(T*node){
      if (node->left != NULL)
        deleteNode(node->left);
      if (node->right != NULL)
        deleteNode(node->right);
      delete node;
    }

    int getHeight(T*node){
        if(node == NULL)
          return 0;
        return node->height;
      }

    int getState(T*node){
      if (node == NULL)
        return 0;
      return getHeight(node->left) - getHeight(node->right);
    }

    //rotate left from top
    T* rotateLeft(T * top){
      T * x = top->right;
      T * topRight = x->left;
      //make turn
      top->right = topRight;
      if (topRight!=NULL)
        topRight->parent = top;

      x->left = top;
      if (top!=NULL)
        top->parent = x;
      // new heights
      int max = ( (getHeight(top->left)<getHeight(top->right))?(getHeight(top->right)):(getHeight(top->left)));
      top->height = 1 + max;
      max = ( (getHeight(x->left)<getHeight(x->right))?(getHeight(x->right)):(getHeight(x->left)));
      x->height = 1 + max;

      //return top node
      return x;
    }
    T* rotateRight(T * top){
      T * x = top->left;
      T * topLeft = x->right;
      //make turn

      top->left = topLeft;
      if (topLeft!=NULL)
        topLeft->parent = top;

      x->right = top;
      if (top!=NULL)
        top->parent = x;

      //set new height
      int max = ( (getHeight(top->left)<getHeight(top->right))?(getHeight(top->right)):(getHeight(top->left)));
      top->height = 1 + max;
      max = ( (getHeight(x->left)<getHeight(x->right))?(getHeight(x->right)):(getHeight(x->left)));
      x->height = 1 + max;

      //return top node
      return x;
    }

    T* insertNode(T*origin, T*node){
      T*tmp;
      if (origin == NULL)
        return node;
      if (origin->key == node->key){
        origin->insert(node);
      }

      else if (origin->lessThan(node->key)){
        tmp = insertNode(origin->right, node);
        origin->right = tmp;
        if (tmp!=NULL)
          tmp->parent = origin;
      }
      else{
        tmp = insertNode(origin->left, node);
        origin->left = tmp;
        if (tmp!=NULL)
          tmp->parent = origin;
      }

      //balancing of tree
      int state = getHeight(origin->left) - getHeight(origin->right);
      //left left balancing
      if (state>1 && getHeight(origin->left->left) >= getHeight(origin->left->right)){
          return rotateRight(origin);
      }
      //left right balancing
      if (state>1 && getHeight(origin->left->left) < getHeight(origin->left->right)){
        tmp = rotateLeft(origin->left);
        origin->left = tmp;
        if (tmp!=NULL)
          tmp->parent = origin;
        return rotateRight(origin);
      }
      //right right balancing
      if (state<-1 && getHeight(origin->right->right) >= getHeight(origin->right->left)){
        return rotateLeft(origin);
      }
      //right left balancing
      if (state<-1 && getHeight(origin->right->right) < getHeight(origin->right->left)){
        tmp =  rotateRight(origin->right);
        origin->right = tmp;
        if (tmp!=NULL)
          tmp->parent = origin;
        return rotateLeft(origin);
      }

      int max = ( (getHeight(origin->left)<getHeight(origin->right))?(getHeight(origin->right)):(getHeight(origin->left)));
      origin->height =  1 + max;
      return origin;
    }

    void insert(T* node){
      root = insertNode(root, node);
      if (root!=NULL)
        root->parent = NULL;
    }

    T * findMin(T*node){
      while (node->left != NULL){
        node = node->left;
      }
      return node;
    }

    T* findMinNode(){
      if (root == NULL)
        return root;
      return findMin(root);
    }

    T * findNode (unsigned int key){
      T*tmp = root;
      while (true){
        if (tmp == NULL)
          return NULL;
        if (tmp->getKey() == key)
          return tmp;
        if (tmp->left != NULL && tmp->right != NULL){
          if (tmp->getKey()>key)
            tmp = tmp->left;
          else
            tmp = tmp->right;
        }
        else{
          tmp = (tmp->left)?(tmp->left):(tmp->right);
          if (tmp == NULL || tmp->getKey()>key)
            return NULL;
        }
      }
    }

    T * removeNode( T* origin, T* node){
      T*myTmp;
      if (origin == NULL)
        return origin;
      if (origin->key < node->key){
        myTmp = removeNode(origin->right, node);
        origin->right = myTmp;
        if (myTmp!=NULL)
          myTmp->parent = origin;
      }


      else if (node->key < origin->key){
        myTmp = removeNode(origin->left, node);
        origin->left = myTmp;
        if (myTmp!=NULL)
          myTmp->parent = origin;
      }


      else{
        if ( (origin->left == NULL) || (origin->right == NULL) ){
          T*tmp = origin->left ? origin->left: origin->right;
          if (tmp==NULL){
            tmp = origin;
            origin = NULL;
          }
          else{
            origin->swap(tmp);
            T*tmpNode = tmp;
            tmp = origin;
            origin = tmpNode;

          }
          nodeToRemove = tmp;


        }
        else{
          T*tmp = findMin(origin->right);
          origin->change(tmp);
          T*tmpNode=tmp;
          tmp = origin;
          origin=tmpNode;

          myTmp = removeNode(origin->right, tmp);
          origin->right = myTmp;
          if (myTmp!=NULL)
            myTmp->parent = origin;
        }
      }
      if (origin==NULL)
        return NULL;

      origin->height = 1 + (getHeight(origin->left)<getHeight(origin->right))?(getHeight(origin->right)):(getHeight(origin->left));

      int state = getHeight(origin->left) - getHeight(origin->right);

      if (state>1 && getHeight(origin->left->left) >= getHeight(origin->left->right))
        return rotateRight(origin);

      if (state>1 && getHeight(origin->left->left) < getHeight(origin->left->right)){
        origin->left = rotateLeft(origin->left);
        return rotateRight(origin);
      }

      if (state<-1 && getHeight(origin->right->right) >= getHeight(origin->right->left))
        return rotateLeft(origin);

      if (state<-1 && getHeight(origin->right->right) < getHeight(origin->right->left)){
        origin->right = rotateRight(origin->right);
        return rotateLeft(origin);
      }

      int max = ( (getHeight(origin->left)<getHeight(origin->right))?(getHeight(origin->right)):(getHeight(origin->left)));
      origin->height =  1 + max;

      return origin;
    }

    T* remove(T*node){
      root = removeNode(root, node);
      if (root != NULL)
        root->parent = NULL;
      T* tmp = nodeToRemove;
      if (root == NULL)
        tmp = node;
      nodeToRemove = NULL;
      return tmp;
    }

    bool isEmpty(){
      return root == NULL;
    }
};

  //avl trees are made from this
  struct Item{
    Item * next = NULL;
    Item * prev = NULL;
    unsigned int id = 0;
  };

  class ItemNode{
    public:
    ItemNode *left, *right, *parent;
    unsigned int key;
    int height = 1;
    Item *start = NULL, *end = NULL;

      bool lessThan(unsigned int otherKey){
        return key < otherKey;
      }
      unsigned int getKey(){
        return key;
      }
      void insert(ItemNode*tmp){
      }

      Item * minId(){
        Item * tmp = start;
        Item * smallest = start;
        while(tmp!=NULL){
          if (tmp->id<smallest->id)
            smallest = tmp;
          tmp = tmp->next;
        }
        return smallest;
      }

      void change(ItemNode * tmp){
        ItemNode * parentS = parent, *leftS = left, *rightS = right;
        if (parentS!=NULL){
          if (parentS->left == this)
            parentS->left = tmp;
          else
            parentS->right = tmp;
        }

        if (leftS != NULL)
          leftS->parent = tmp;

        if (rightS != NULL)
          rightS->parent = tmp;

        if (tmp->left!=NULL)
          tmp->left->parent = this;
        if (tmp->right!=NULL)
          tmp->right->parent = this;

        parent = tmp;
        left = tmp->left;
        right = tmp->right;

        tmp->parent = parentS;
        if (leftS == tmp)
          tmp->left = this;
        else
          tmp->left = leftS;

        if (rightS == tmp)
          tmp->right = this;
        else
          tmp->right = rightS;

        key = tmp->key;
      }
      void swap(ItemNode * tmp){
        if (left == tmp){

          tmp->parent = parent;
          if (parent != NULL)
          if (parent!=NULL){
            if (parent->right == this)
              parent->right = tmp;
            else
              parent->left = tmp;
          }
        }
        else{
            if (left!=NULL){
              left->parent = tmp;
              tmp->left = left;
            }
            left = NULL;
        }

        if (right == tmp){

          tmp->parent = parent;
          if (parent!=NULL){
            if (parent->right == this)
              parent->right = tmp;
            else
              parent->left = tmp;
          }
        }
        else{
            if (right!=NULL){
              right->parent = tmp;
              tmp->right = right;
            }
            right = NULL;
        }
      }
      ~ItemNode(){
        Item * tmp = start;
        while (tmp != NULL){
          tmp = start->next;
          delete start;
          start = tmp;
        }
      }
  };

class CChain {
  public:
    CChain(int id){
      idChain = id;
      indexInHeap = 0;
      numOfItems = 0;
      minRevenue = 0;
      tree = new AVLTree<ItemNode>();
    }

    ~CChain(){
      if (tree!=NULL)
        delete tree;
      tree = NULL;
    }

    //insert a new item
    void insertItem(unsigned int id, unsigned int revenue){
      Item * item = new Item();
      item->id = id;

      ItemNode * present = tree->findNode(revenue);

      if (minRevenue > revenue || tree->isEmpty())
        minRevenue = revenue;

      if (present == NULL){
        present = new ItemNode();
        present->key = revenue;
        present->start = present->end = item;
        item->prev = item->next = NULL;
        tree->insert(present);
      }
      else{
        present->end->next = item;
        item->next = NULL;
        item->prev = present->end;
        present->end = item;
      }
    }

    //delete item from chain
    unsigned int removeItem(){
      ItemNode * present = tree->findNode(minRevenue);

      Item * item = present->minId();


      unsigned int id = item->id;

      if (item->prev == NULL)
        present->start = item->next;
      if (item->next!=NULL)
        item->next->prev = item->prev;

      if (item->prev!=NULL)
        item->prev->next = item->next;


      if (present->start == NULL){

        present->end = NULL;
        present = tree->remove(present);

        if (tree->isEmpty()){
          minRevenue = 0;
        }
        else{
          minRevenue=tree->findMinNode()->key;
        }
        delete present;
      }
      delete item;

      return id;
    }

    bool isEmpty(){
      return tree->isEmpty();
    }

    int getId(){
      return idChain;
    }

    int getIndexInHeap(){
      return indexInHeap;
    }

    int getMinRevenue(){
      return minRevenue;
    }

    void setIndexInHeap(int i){
      indexInHeap = i;
    }

  private:
    unsigned int idChain;
    int indexInHeap;
    unsigned int numOfItems;
    unsigned int minRevenue;
    AVLTree<ItemNode> * tree;

};

class BinaryHeap {
  public:
    CChain ** binHeap;
    int itemsNum;
    int heapSize;

    BinaryHeap (int size) {
      binHeap = new CChain*[size]();
      itemsNum = 0;
      heapSize = size;
    }

    ~BinaryHeap(){
      delete[] binHeap;
    }

    int parent(int i){
      return (i-1)/2;
    }

    unsigned int minChain(){
      return binHeap[0]->getId();
    }

    void insertItem(CChain * item){
      CChain * tmp = NULL;
      if (itemsNum == heapSize)
        return;
      itemsNum++;
      int i = itemsNum-1;
      binHeap[i] = item;
      binHeap[i]->setIndexInHeap(i);

      while (i != 0 && (binHeap[parent(i)]->isEmpty() || binHeap[parent(i)]->getMinRevenue() > binHeap[i]->getMinRevenue()
            || (binHeap[parent(i)]->getMinRevenue() == binHeap[i]->getMinRevenue() && binHeap[parent(i)]->getId() > binHeap[i]->getId())
            )){
        tmp = binHeap[i];

        binHeap[i] = binHeap[parent(i)];
        binHeap[parent(i)] = tmp;
        binHeap[parent(i)]->setIndexInHeap(parent(i));
        binHeap[i]->setIndexInHeap(i);

        i = parent(i);
      }
    }

    int left(int i){
      return 2*i+1;
    }

    int right(int i){
      return 2*i+2;
    }

    void bubbleDown(int i) 
    { 
      CChain * tmp = NULL;
      int leftChild = left(i); 
      int rightChild = right(i); 
      int lesser = i; 

      if (binHeap[i]->isEmpty()==false){
        if (leftChild < itemsNum && binHeap[leftChild]->isEmpty()==false && (binHeap[leftChild]->getMinRevenue() < binHeap[lesser]->getMinRevenue() ||
                                  (binHeap[leftChild]->getMinRevenue() == binHeap[lesser]->getMinRevenue() &&
                                  binHeap[leftChild]->getId() < binHeap[lesser]->getId())))
          lesser = leftChild; 

        if (rightChild < itemsNum && binHeap[rightChild]->isEmpty()==false && (binHeap[rightChild]->getMinRevenue() < binHeap[lesser]->getMinRevenue() ||
                                    (binHeap[rightChild]->getMinRevenue() == binHeap[lesser]->getMinRevenue() &&
                                    binHeap[rightChild]->getId() < binHeap[lesser]->getId())))
          lesser = rightChild; 
      }
      else{
        if (leftChild < itemsNum && binHeap[leftChild]->isEmpty()==false) 
          lesser = leftChild; 

        if (rightChild < itemsNum && binHeap[rightChild]->isEmpty()==false && ( binHeap[lesser]->isEmpty() == true || binHeap[rightChild]->getMinRevenue() < binHeap[lesser]->getMinRevenue() ||
                                    (binHeap[rightChild]->getMinRevenue() == binHeap[lesser]->getMinRevenue() &&
                                    binHeap[rightChild]->getId() < binHeap[lesser]->getId())))
          lesser = rightChild; 
      /*
       if (leftChild < itemsNum && binHeap[leftChild]->isEmpty == true && binHeap[leftChild]->getId() < binHeap[lesser]->getId())
          lesser = leftChild;
        if (rightChild < itemsNum && binHeap[rightChild]->isEmpty == true && binHeap[rightChild]->getId() < binHeap[lesser]->getId())
          lesser = rightChild;
      */
      }

      if (lesser != i) 
      { 
          tmp = binHeap[i];
          binHeap[i] = binHeap[lesser];
          binHeap[lesser] = tmp;

          binHeap[lesser]->setIndexInHeap(lesser);
          binHeap[i]->setIndexInHeap(i);
          bubbleDown(lesser); 
      } 
    } 

    void bubbleUp(int i) 
    { 
      CChain * tmp = NULL;
      int childParent = parent(i);

      if ( i!=childParent && (binHeap[childParent]->getMinRevenue()>binHeap[i]->getMinRevenue() ||
        ((binHeap[childParent]->getMinRevenue() == binHeap[i]->getMinRevenue() && binHeap[childParent]->getId()>binHeap[i]->getId()) ||
        binHeap[childParent]->isEmpty()))
        ){
      tmp = binHeap[i];
      binHeap[i] = binHeap[childParent];
      binHeap[childParent] = tmp;

      binHeap[childParent]->setIndexInHeap(childParent);
      binHeap[i]->setIndexInHeap(i);
      bubbleUp(childParent);
      }
    }

    int getItemsNum(){
      return itemsNum;
    }
};

class CHolding
{
   public:
  CHolding(){
    indexArr =   new  CChain*[10000]();
    minHeap = new BinaryHeap(10000);
  }

  ~CHolding(){
    for (int i = 0; i< 10000; i++){
      if (indexArr!=NULL)
        delete indexArr[i];
    }
    delete[] indexArr;
    delete minHeap;
  }
  
   void Add    (int chain, unsigned int id, unsigned int revenue){
     if (indexArr[chain-1] == NULL){
       CChain * newChain = new CChain(chain);
       newChain->insertItem(id, revenue);
       indexArr[chain-1] = newChain;
       minHeap->insertItem(newChain);
     }
     else{
       indexArr[chain-1]->insertItem(id, revenue);
       minHeap->bubbleUp(indexArr[chain-1]->getIndexInHeap());
     }
   }

   bool Remove (int chain, unsigned int & id){
     if (indexArr[chain-1]==NULL || indexArr[chain-1]->isEmpty())
      return false;
     unsigned int outputInt = indexArr[chain-1]->removeItem();
     minHeap->bubbleDown(indexArr[chain-1]->getIndexInHeap());
     id = outputInt;
     return true;
   }

   bool Remove (unsigned int & id){
     if (minHeap->getItemsNum() == 0)
      return false;
     return Remove(minHeap->minChain(), id);
   }

   void Merge  (int dstChain, int srcChain){
   }

   bool Merge  (void){
     return false;
   }

  private:
    CChain ** indexArr ;
    BinaryHeap * minHeap;
};

int main(){
  return 0;
}
