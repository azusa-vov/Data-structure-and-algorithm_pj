#include "CircularQueue.h"
#include <iostream>
CircularQueue::CircularQueue(){
      head=0;
      tail=0;
      count=0;
      for (int i =0;i<maxSize;i++){
            buffer[i]=nullptr;
      }//新建队列
}
CircularQueue::~CircularQueue(){
}
//析构函数
void CircularQueue::pushQ(LogNode* node){
      buffer[tail]=node;
      tail=(tail+1)%maxSize;//体现循环性
      if(count<maxSize){
            count++;
      }else{
            head=(head+1)%maxSize;//这里需要后插一个新元素，相应需要丢弃旧指针
      }
}
//新日志入队
void CircularQueue::clear(){
      head=0;
      tail=0;
      count=0;
}
//清空队列
int CircularQueue::getsize(){
      return count;
}
//获取当前队列大小
LogNode* CircularQueue::get(int index){
      if(index<0 || index>=count){
            return nullptr;
      }//0是最旧，count-1是最新
      return buffer[(head+index)%maxSize];
}
//按照逻辑索引获取结点指针

void CircularQueue::Recent(int n){
      if(count==0) return;
      int printCount=(n>count)?count:n;
      int startidx=count-printCount;
      //这里是说从最新的结点开始打印，向前数printCount个结点，count-1中的"-1"
      //和实际结点索引位置+1抵消

      for(int i=0;i<printCount;i++){
            int idx=startidx+i; //当前的逻辑索引
            int realidx=(head+idx)%maxSize;  //物理上的索引位置
            LogNode* node=buffer[realidx];
            if(node){
                  std::cout<<"["<<node->time<<"]"
                           <<node->level<<" "
                           <<node->module<<" "
                           <<node->message<<std::endl;
            }
      }
}
//输出最近n条日志
bool CircularQueue::Remove(LogNode* target){
      if(count==0)     return false;
      int flag=-1;
      for(int i=0;i<count;i++){
            int realidx=(head+i)%maxSize;
            if(buffer[realidx]==target){
                  flag=i;
                  break;
            }
      }//找到了需要删除的结点
      if(flag==-1){
            return false;
      }
      for(int i=flag;i<count-1;i++){
            int curr_idx=(head+i)%maxSize;
            int next_idx=(head+1+i)%maxSize;
            buffer[curr_idx]=buffer[next_idx];
      }
      tail=(tail-1+maxSize)%maxSize;
      count--;

      return true;
}
//从队列中移除指定结点指针