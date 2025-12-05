#ifndef KMP_H
#define KMP_H
 
#include <string>
class KMP{
public:
      static int* computeNext(const std::string& pattern);
      static bool match(const std::string& next,const std::string& pattern);

};     

#endif