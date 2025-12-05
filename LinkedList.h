#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "LogEntry.h"
#include <iostream>

class LinkedList {
public:
      LogNode* head;
      LogNode* tail;
      int size;

      LinkedList();//创建
      ~LinkedList();//析构

      void push_back(std::string t, std::string l, std::string m, std::string msg);//尾插
      void push_front(std::string t, std::string l, std::string m, std::string msg); //头插
      bool deleteByIndex(int index); //删除指定行号
      void clear();//清空
      void Forward();//从头到尾
      void Backward();//从尾到头
      LogNode* getLatest();//获取最新结点，用于放入循环队列
      LogNode* getNode(int index); 
      LinkedList* clone(); //克隆链表
      };

#endif