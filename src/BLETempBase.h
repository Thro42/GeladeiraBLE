#ifndef _BLE_TEMP
#define _BLE_TEMP
#include <Arduino.h>
#include <ArduinoBLE.h>
typedef enum _BLETemp_Type{
TempBase,
ibsTh1,
ibsTh2
} BLETemp_Type;

class BLETempBase
{
public:
  BLETempBase() {};
  BLETempBase( String name );
  BLETempBase( uint8_t address[6] );
  virtual BLETemp_Type sensorType(){ return TempBase;};
  void setup();
  void setupScan();
  void setupScan(String suuid, String cuuid);
  void loop();
  void setDebugMode(bool mode);
  String saddress();
  void setService(String uuid);
  void setCharacteristic(String uuid);
  boolean hasValues();
  float Temperature();
  float Humidity();
  int Battery();

protected:
    void _findDevice(void);
    void _DoConnect( void );
    void _saveBuffer(const unsigned char data[], int length);
    void _readValues(void);
    String  _name;
    uint8_t _address[6];
    String  _saddress;
    boolean _useAddress = false;
    boolean _doScan = false;
    boolean _doConnect = false;
    boolean _connected = false;
    BLEDevice _myDevice;
    boolean _debug = false;
    String _serviceUUID = "fff0";
    String _characteristicUUID = "fff2";
    int _valueLength;
    uint8_t _valueBuff[64];
};
#endif