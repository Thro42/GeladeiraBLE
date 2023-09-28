
#ifndef _IBS_TH2
#define _IBS_TH2
#include "BLETempBase.h"

class ibs_th2: public BLETempBase
{
public:
  virtual BLETemp_Type sensorType(){ return ibsTh2;};
};
#endif