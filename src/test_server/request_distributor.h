#ifndef __REQUEST_DISTRIBUTOR_H_
#define __REQUEST_DISTRIBUTOR_H_

#include <functional>
#include <vector>
#include <unistd.h>
#include <iostream>

#include "interface/xgt_trader_struct.h"
#include "logger/logger.h"

namespace XGT {

static constexpr int INT_SIZE = sizeof(int);
static constexpr int RequestMaxBufferSize = 4096;

void DistributeRequest(int, char*);
void DispatchRequest(int, int, char*);

} //namespace XGT

#endif
