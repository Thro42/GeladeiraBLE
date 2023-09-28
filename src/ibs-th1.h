
#ifndef _IBS_TH1
#define _IBS_TH1
#include "BLETempBase.h"

class ibs_th1: public BLETempBase
{
public:
  ibs_th1( String name );
  ibs_th1( uint8_t address[6] );
  virtual BLETemp_Type sensorType(){ return ibsTh1;};
};
#endif