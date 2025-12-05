#include "UndoStack.h"
#include <iostream>
// Add the header that defines the full type of listSnap

UndoStack::UndoStack(){
      top_idx=-1;
      for(int i=0;i<5;i++){
            stack[i]=nullptr;
      }
}
//构造函数，初始化栈
UndoStack::~UndoStack(){
      while(top_idx>=0){
            delete stack[top_idx]->bstSnap;
            delete stack[top_idx]->listSnap;
            delete stack[top_idx];
            top_idx--;
            
      }
}
//析构函数，释放所有快照内存
bool UndoStack::isEmpty(){
      return top_idx==-1;
}     
//判断栈空
bool UndoStack::isFull(){
      return top_idx==4;
}
//判断栈满
void UndoStack::push(LinkedList* currList,BST* currBST){
      //保存最近5次意味着满栈需要把最底下的挤掉后再入栈
      if(isFull()){
            delete stack[0]->bstSnap;
            delete stack[0]->listSnap;
            delete stack[0];
            for(int i=1;i<5;i++){
                  stack[i-1]=stack[i];
            }
            stack[4]=nullptr;
            top_idx--;
      }
      Snapshot* newSnap=new Snapshot();
      //深拷贝
      newSnap->listSnap=currList->clone();
      newSnap->bstSnap=currBST->Clone();

      //入栈
      stack[++top_idx]=newSnap;
      /*详询了AI后理解到，快照只要保存链表和BST的深拷贝，因为队列里存的是链表结点的指针，
      一旦链表被替换后，这些快照中的指针可能悬空，因此队列需要在pop时重建*/
      
}


bool UndoStack::pop(LinkedList*& currList,BST*& currBST,CircularQueue*& currQueue){
      if(isEmpty()){
            return false;
      }
      Snapshot* topSnap=stack[top_idx--];
      
      //释放旧的内存
      delete currBST;
      delete currList;
      currQueue->clear();//这里队列应该清空，因为队列里存的是旧链表的指针
      // list和BST直接把快照的指针转让给当前系统
      currList=topSnap->listSnap;
      currBST=topSnap->bstSnap;
      /*但队列需要重建，因为其内部指针必须指向currList里的结点
      因此希望遍历currList中的最后1000个结点推入queue*/

      int count=currList->size;
      int start=0;
      if(count>1000) start=count-1000;

      LogNode* curr=currList->head;
      int idx=0;
      while(curr!=nullptr){
            if(idx>=start){
                  currQueue->pushQ(curr);
            }
            curr=curr->next;
            idx++;
      }

      delete topSnap;
      return true;
}
//弹出栈顶快照，恢复链表和BST，重建队列