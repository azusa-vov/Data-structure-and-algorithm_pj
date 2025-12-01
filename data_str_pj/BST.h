#ifndef BST_H
#define BST_H

#include <string>
#include <iostream>
struct BSTNode{
      std::string key;
      int value;
      BSTNode* left;
      BSTNode* right;   

      BSTNode(std::string k):key(k),value(1),left(nullptr),right(nullptr){}
};

struct StatPair{
      std::string module;
      int count;
};

class BST{
private:
      BSTNode* root;

      int SearchAid(BSTNode* node, const std::string& key);
      void InorderAid(BSTNode* node);
      void ClearAid(BSTNode* node);
      BSTNode* CloneAid(BSTNode* node);
public:
      BST();
      ~BST();
      void Insert(const std::string& module);
      int Search(const std::string& module);
      void Inorder();
      void Clear();
      void DeleteNode(const std::string& key);
      void printStats();
      BST* Clone();
      int countNodes(BSTNode* node);
      void toArrayAid(BSTNode* node,StatPair* arr,int& index);
};

#endif