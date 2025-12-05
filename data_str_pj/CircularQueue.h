#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H

#include "LogEntry.h"
class CircularQueue{
private:
      LogNode* buffer[1000];
      int head;
      int tail;
      int count;
      const int maxSize=1000;

public:
      CircularQueue();
      ~CircularQueue();
      
      void pushQ(LogNode* node); //新日志入队
      void clear();

      int getsize();
      LogNode* get(int index);
      //获取idx=i对应的元素

      void Recent(int n);
      bool Remove(LogNode *node);

};    
#endif