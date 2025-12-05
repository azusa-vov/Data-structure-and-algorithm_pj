#include "LinkedList.h"
LinkedList::LinkedList() {
      head=nullptr;
      tail=nullptr;
      size=0;
}
//初始化双链表，头尾置零

LinkedList::~LinkedList() {
      clear();
}
//析构函数

void LinkedList::push_back(std::string t, std::string l, std::string m, std::string msg) {
      LogNode* newNode = new LogNode(t, l, m, msg);//新定义一个结点
      if (head==nullptr) {
            head=newNode;
            tail=newNode;//如果只有一个新插入的结点（原来是空），头尾都指向它
      } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;//否则就在尾部建立前后的连接
      }
      size++;
}
//尾插
void LinkedList::push_front(std::string t, std::string l, std::string m, std::string msg){
      LogNode* newNode = new LogNode(t, l, m, msg);//新定义一个结点
      if (head == nullptr) {
            head = newNode;
            tail = newNode; //如果只有一个新插入的结点（原来是空），头尾都指向它
      } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode; //否则就在头部建立前后的连接
      }
      size++;
}
//头插

LogNode* LinkedList::getLatest(){
      if (tail != nullptr) {
            return tail; //返回最新结点
      }
      return nullptr; //如果链表为空，返回nullptr
}

void LinkedList::clear() {
      LogNode* current = head;
      while (current != nullptr) {
            LogNode* nextNode = current->next;
            delete current; 
            current = nextNode; 
      }
      head = nullptr;
      tail = nullptr;
      size = 0; 
}
//从头结点开始，逐个删除
bool LinkedList::deleteByIndex(int index){
      if(index<1 || index>size){
            return false;
      }
      
      LogNode* curr=head;
      if(index<=size/2){
            //距离head近，可以从前往后遍历
            for(int i=1;i<index;i++){
                  curr=curr->next;
            }
      }else{
            curr=tail;
            //距离tail近，可以从后往前遍历
            for(int i=size;i>index;i--){
                  curr=curr->prev;
            }
      }
      if(head==tail){
            head=tail=nullptr;//只有一个需要删除的index，头尾全部置空
      }
      else if(curr==head){
            head=head->next;
            if(head) head->prev=nullptr;
      }
      else if(curr==tail){
            tail=tail->prev;
            if(tail) tail->next=nullptr;
      }
      else{
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
            //前一个结点连后一个，后一个结点连前一个
      }
      delete(curr);
      size--;
      return true;
}


void LinkedList::Forward(){
      LogNode* curr=head;
      while(curr!=nullptr){
            std::cout<<"["<<curr->time<<"]"
                          <<curr->level<<" "
                          <<curr->module<<" "
                          <<curr->message<<std::endl;
                          //按照用例的格式预留了空格
                  curr=curr->next;
      }
}

void LinkedList::Backward(){
      LogNode* curr=tail;
      while(curr!=nullptr){
            std::cout<<"["<<curr->time<<"]"
                          <<curr->level<<" "
                          <<curr->module<<" "
                          <<curr->message<<std::endl;
                          //按照用例的格式预留了空格
                  curr=curr->prev;
      }
}
//这两个函数是从前往后和从后往前遍历

LinkedList* LinkedList::clone(){
      LinkedList* newList=new LinkedList();
      LogNode* curr=head;
      while(curr!=nullptr){
            newList->push_back(curr->time,curr->level,curr->module,curr->message);
            curr=curr->next;
      }
      return newList;
}
//复制当前链表，返回一个新的链表指针

LogNode* LinkedList::getNode(int index){
      if(index<1 || index>size) return nullptr;
      LogNode* curr=head;
      for(int i=1;i<index;i++){
            curr=curr->next;
      }
      return curr;
}
//按照稳定ID查找结点