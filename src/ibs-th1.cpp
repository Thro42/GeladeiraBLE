#include "ibs-th1.h"

ibs_th1::ibs_th1( String name ) {
    _name = name;
    _useAddress = false;
}

ibs_th1::ibs_th1( uint8_t address[6] ) {
    for(uint i =0 ; i < 6 ; i++) {
        _address[i] = address[i];
    }
    _useAddress = true;
}

