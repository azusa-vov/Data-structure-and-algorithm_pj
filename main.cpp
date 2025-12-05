#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "KMP.h"
#include "LinkedList.h"
#include "BST.h"
#include "UndoStack.h"
#include "CircularQueue.h"

LinkedList* g_logList=new LinkedList();
CircularQueue* g_logQueue=new CircularQueue();
BST* g_errorStats=new BST();
UndoStack g_undoStack;

int myFindChar(const std::string& str,char c,int start=0){
      if(start<0) start=0;
      for(std::size_t i=static_cast<std::size_t>(start); i<str.length(); ++i){
            if(str[i]==c){
                  return static_cast<int>(i);
            }
      }
      return -1;
} 
//辅助函数：在字符串str中从start位置开始查找字符c，返回索引，未找到返回-1

void parseAndAddLog(const std::string& line){
      //Part.1:[YYYY-MM-DD HH:MM:SS] LEVEL Module Message
      if(line.empty()) return;

      //找time tag结束位置的"]"
      int endBracket=myFindChar(line,']');
      if(endBracket==-1) return; //格式错误，直接返回

      //时间提取
      std::string time_tag=line.substr(1,endBracket-1);

      //提取level
      int str_lev=endBracket+2;
      //以下一个' '为界
      int end_lev=myFindChar(line,' ',str_lev);
      if(end_lev==-1) return; //格式有误
      std::string level=line.substr(str_lev,end_lev-str_lev);//因为最后一个是空格，可以不要
      
      //提取module
      int str_mod=end_lev+1;
      int end_mod=myFindChar(line,' ',str_mod); 
      if(end_mod==-1) return; //格式有误
      std::string module=line.substr(str_mod,end_mod-str_mod);

      //提取message
      std::string message=line.substr(end_mod+1);//后续为message部分，不用控制长度

      //Part.2 :存入数据结构
      g_logList->push_back(time_tag,level,module,message);
      g_logQueue->pushQ(g_logList->getLatest());
      if(level=="ERROR"){
            g_errorStats->Insert(module);
      }
}

//Part.3:处理命令

void cmdLoad(const std::string& filename){
      //读取日志文件，构建链表、循环队列、BST
      g_logList->clear();
      g_logQueue->clear();
      g_errorStats->Clear();

      std::ifstream file(filename.c_str());
      if(!file.is_open()){
            std::cout<<"Failed to open file: "<<filename<<std::endl;
            return;
      }

      std::string line;
      while(std::getline(file,line)){
            parseAndAddLog(line);
      }
      file.close();
      std::cout<<"Loaded "<<g_logList->size<<" entries"<<std::endl;
      //这里在数量的前后需要加空格

}

void cmdFilter(std::string strTime,std::string endTime){
      //从链表中找到一段特定时间内的所有日志并输出：YYYY-MM-DD_HH:MM:SS YYYY-MM-DD_HH:MM:SS
      for (size_t i=0;i<strTime.length();i++){
            if(strTime[i]=='_') strTime[i]=' ';
      }
      for (size_t i=0;i<endTime.length();i++){
            if(endTime[i]=='_') endTime[i]=' ';
      }
      
      LogNode* curr=g_logList->head;
      while(curr){
            if(curr->time>=strTime && curr->time<=endTime){
                  std::cout<<"["<<curr->time<<"] "
                           <<curr->level<<" "
                           <<curr->module<<" "
                           <<curr->message<<std::endl;
            }
            curr = curr->next;
      }
}

void cmdSearch(const std::string& key){
      //用KMP查找message中的关键词,输出为Found X match(es),然后换行输出一条完整的日志
      int count=0;
      LogNode* temp=g_logList->head;
      while(temp){
            if(KMP::match(temp->message,key)){
                  count++;
            }
            temp = temp->next;
      }
      std::cout<<"Found "<<count<<" match(es)"<<std::endl;
      if(count>0){
            LogNode* curr=g_logList->head;
            while(curr){
                  if(KMP::match(curr->message,key)){
                        std::cout<<"["<<curr->time<<"] "
                                 <<curr->level<<" "
                                 <<curr->module<<" "
                                 <<curr->message<<std::endl;
                  }
                  curr = curr->next;
            }
      }
}

void cmdDelete(int index){
      LogNode* target=g_logList->getNode(index);
      if(!target) return ;
      
      g_undoStack.push(g_logList,g_errorStats);//在删除之前将当前状态作为快照压入栈（5次）
      g_logQueue->Remove(target); //从循环队列中移除对应结点指针
      if(target->level=="ERROR"){ //更新BST中ERROR的统计信息
            g_errorStats->DeleteNode(target->module);

      }
      if(g_logList->deleteByIndex(index)){//删除指定行号的日志
            std::cout<<"Deleted entry "<<index<<std::endl;
      }
}

void cmdUndo(){
      //撤销上一次的DELETE；恢复链表，队列，BST；5次；Undo successful/No more undo
      if(g_undoStack.pop(g_logList,g_errorStats,g_logQueue)){
            std::cout<<"Undo successful"<<std::endl;
      }else{
            std::cout<<"No more undo"<<std::endl;
      }

}

void cmdRecent(int n){
      //输出最近n条
      g_logQueue->Recent(n);
}

void cmdStats(){
      //按照模块输出error数量
      g_errorStats->printStats();
}


int main(){
      std::string cmd;
      while(std::cin>>cmd){
            if((cmd=="LOAD")){
                  std::string filename;
                  std::cin>>filename;
                  cmdLoad(filename);
      }     else if(cmd=="FILTER"){
            std::string startTime,endTime;
            std::cin>>startTime>>endTime;
            cmdFilter(startTime,endTime);
      }     else if(cmd=="STATS"){
            cmdStats();
      }     else if(cmd=="SEARCH"){
            std::string key;
            std::cin>>key;
            cmdSearch(key);
      }     else if(cmd=="DELETE"){
            int index;
            std::cin>>index;
            cmdDelete(index);
      }     else if(cmd=="UNDO"){
            cmdUndo();
      }     else if(cmd=="RECENT"){
            int n;
            std::cin>>n;
            cmdRecent(n);
      }     else if (cmd=="EXIT"){
            break;
      }
}
      delete g_errorStats;
      delete g_logList;
      delete g_logQueue;
      return 0;



}
      