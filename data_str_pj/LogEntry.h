#ifndef LOGENTRY_H
#define LOGENTRY_H
#include <string>
//这个文件用来定义存储log的双向链表的结点
struct LogNode{
      int id; // stable line id assigned when loading
      std::string time;
      std::string level;
      std::string module;    
      std::string message;
      //以上包含结点的信息
      LogNode* next;
      LogNode* prev;
      //定义前驱和后继指针

      LogNode(int entryId,std::string t, std::string l, std::string m, std::string msg)
          : id(entryId), time(t), level(l), module(m), message(msg), next(nullptr), prev(nullptr) {}
      //例化
};
#endif
