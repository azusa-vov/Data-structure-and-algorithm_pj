#include "kMP.h"

int* KMP::computeNext(const std::string& pattern){
      int m=pattern.length();
      int* next=new int[m];
      //对于pattern的第i个位置，next[i]记录以i结尾的前缀中，
      //最长的具有“前缀等于后缀”的长度
      next[0]=0;
      int len=0;//当前最长相等前后缀
      int i=1;

      while (i<m){
            if(pattern[i]==pattern[len]){
                  len++;
                  next[i]=len;
                  i++;
            }else{//字符无法扩展前后缀
                  if(len!=0){
                        len=next[len-1];//去找len前一位所标记的前缀，再尝试匹配
                  }else{
                        next[i]=0;//一般如果发现前缀到开头都没有匹配的，就直接next标0
                        i++;//i恢复到用于比较的位数
                  }
            }
      }
      return next;
}
//用于构建前缀函数
bool KMP::match(const std::string& text,const std::string& pattern){
      if(pattern.empty()) return true;
      if(text.empty()) return false;

      int n=text.length();
      int m=pattern.length();

      int* next=computeNext(pattern);

      int i=0; //text_idx
      int j=0;  //pattern_idx
      bool found=false;

      while (i<n){
            if(pattern[j]==text[i]){
                  i++;
                  j++;
            }//对于相等的就继续比

            if(j==m){
                  found=true;
                  break;//项目中SEARCH输出匹配的日志只要找到一次就可以返回
            }else if(i<n && pattern[j]!=text[i]){
                  if(j!=0){
                        j=next[j-1];//用j的前缀看看有没有希望
                  }else{
                        i++;//不行就继续比
                  }
            }
      }
      delete[] next;
      return found;
}
//用于KMP匹配