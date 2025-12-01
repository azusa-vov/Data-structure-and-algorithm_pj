#ifndef UNDOSTACK_H
#define UNDOSTACK_H

#include "LinkedList.h"
#include "BST.h"
#include "CircularQueue.h"

struct Snapshot{
      LinkedList* listSnap;
      BST* bstSnap;
      //不需要恢复队列，因为队列存的是list的指针，恢复list后队列需要重建
};

class UndoStack{
private:
      Snapshot* stack[5];
      int top_idx;
public:

      UndoStack();
      ~UndoStack();

      void push(LinkedList* currList,BST* currBST);
      bool pop(LinkedList*& currList,BST*& currBST,CircularQueue*& currQueue);

      bool isEmpty();
      bool isFull();
};

#endif