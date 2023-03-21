#ifndef __XGT_TRADER_STRUCT_H_
#define __XGT_TRADER_STRUCT_H_

#include <memory>

namespace XGT {

struct XGTLoginRequest {
  char account[32];
  char password[32];
};

struct XGTLogouRequest {
  char account[32];
};

struct XGTSubscribeRequest {
  
};

}//namespace XGT

#endif
