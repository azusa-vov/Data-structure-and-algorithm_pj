#include "BST.h"
BST::BST(){
      root=nullptr;
}
BST::~BST(){
      Clear();
}
//构造析构函数
void BST::Insert(const std::string& module){
      if(root == nullptr){
            root = new BSTNode(module);
            return;
      }
      BSTNode* pointer = root;
      while(pointer != nullptr){
            if(module == pointer->key){
                  pointer->value++;
                  return;
            }
            else if(module < pointer->key){
                  if(pointer->left == nullptr){
                        pointer->left = new BSTNode(module);
                        return;
                  }
                  else {
                        pointer = pointer->left;
                  }
            }
            else{
                  if(pointer->right == nullptr){
                        pointer->right = new BSTNode(module);
                        return;
                  }
                  else{
                        pointer = pointer->right;
                  }
            }
      }
}
//插入模块名，若已存在则计数加一
int BST::SearchAid(BSTNode* node,const std::string& key){
      if(node==nullptr){
            return 0;
      }
      if(key==node->key){
            return node->value;
      }else if(key<node->key){
            return SearchAid(node->left,key);
      }else{
            return SearchAid(node->right,key);
      }
}
//辅助搜索函数
int BST::Search(const std::string& module){
      return SearchAid(root,module);
}
//搜索模块名，返回错误计数，未找到返回0
void BST::InorderAid(BSTNode* node){
      if(node==nullptr) return;
      InorderAid(node->left);
      InorderAid(node->right);
}
//中序遍历辅助函数
void BST::Inorder(){
      InorderAid(root);
}
//中序遍历
void BST::DeleteNode(const std::string& key){
      BSTNode* parent=nullptr;
      BSTNode* pointer=root;

      while(pointer!=nullptr && pointer->key!=key){
            parent=pointer;
            if(key<pointer->key){
                  pointer=pointer->left;
      }else{
            pointer=pointer->right;
      }
}
      if(pointer==nullptr) return;

      if(pointer->value>1){
            pointer->value--;
            return;
      }
      BSTNode* temppointer;
      if(pointer->left==nullptr){//如果待删除的节点没有左子树
            temppointer=pointer->right;
      }
      else{
            temppointer=pointer->left;
            BSTNode* tempparent=nullptr;
            //寻找左子树中的最大节点
            while(temppointer->right!=nullptr){
                  tempparent=temppointer;
                  temppointer=temppointer->right;
            }
      if(tempparent==nullptr){
            //左孩子本身就是最大，相当于没有右子树
      }else{
            tempparent->right=temppointer->left;
            temppointer->left=pointer->left;
            //这里的逻辑在于用temppointer（左子树的最大结点）的左子树去替代temppointer原来的位置
            //用temppointer去替代pointer的位置
      }
      temppointer->right=pointer->right;
}

if(parent==nullptr){
      root=temppointer;

}
else if (parent->left==pointer){
      parent->left=temppointer;
}
else{
      parent->right=temppointer;
}

      delete pointer;
}

// 删除全局Search函数，避免未定义标识符错误

void BST::ClearAid(BSTNode* node){
      if(node==nullptr) return;
      ClearAid(node->left);
      ClearAid(node->right);
      delete node;
}

void BST::Clear(){
      ClearAid(root);
      root = nullptr;
}
//清空BST
BSTNode* BST::CloneAid(BSTNode* node){
      if(node==nullptr) return nullptr;
      BSTNode* newnode=new BSTNode(node->key);
      newnode->value=node->value;
      newnode->left=CloneAid(node->left);
      newnode->right=CloneAid(node->right);     
      return newnode;
}

BST* BST::Clone(){
      BST* newBST=new BST();
      newBST->root=CloneAid(this->root);
      return newBST;
}
//复制BST，返回一个新的BST指针

int BST::countNodes(BSTNode* node){
      if(node == nullptr) return 0;
      return 1+countNodes(node->left)+countNodes(node->right);

}
//计算节点总数

void BST::toArrayAid(BSTNode* node,StatPair* arr,int& index){
      if(!node) return;
      toArrayAid(node->left,arr,index);
      arr[index].module=node->key;
      arr[index].count=node->value;
      index++;
      toArrayAid(node->right,arr,index);
}
//把BST内容存入数组，inorder方式
void BST::printStats(){
      int total=countNodes(root);
      if(total==0) return;

      StatPair* arr=new StatPair[total];
      int idx=0;
      toArrayAid(root,arr,idx);
      for (int i=0;i< total-1;i++){
            for(int j=0;j < total-i-1;j++){
                  bool swap=false;
                  if(arr[j].count<arr[j+1].count){
                        swap=true;//如果count少就往后放
                  }else if(arr[j].count==arr[j+1].count && arr[j].module>arr[j+1].module){
                        swap=true;//如果count数目一致，就把字典序小的往前放
                  }
                  if(swap){
                        StatPair temp=arr[j];
                        arr[j]=arr[j+1];
                        arr[j+1]=temp;
                  }
            }
      }
      for(int i=0;i<total;i++){
            std::cout<<arr[i].module<<": "<<arr[i].count<<" errors"<<std::endl;
      }
      delete []arr;
      
}
//打印错误统计信息