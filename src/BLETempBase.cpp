#include "BLETempBase.h"

/*---------------------------------------*/
  BLETempBase::BLETempBase( String name ) {
    _name = name;
    _useAddress = false;
  }

/*---------------------------------------*/
  BLETempBase::BLETempBase( uint8_t address[6] ) {

    for(uint i =0 ; i < 6 ; i++) {
        _address[i] = address[i];
    }
    _useAddress = true;
  }

/*---------------------------------------*/
  void BLETempBase::setup() {
    if (_debug == true)
      Serial.println("setup");
    _doConnect = false;
    _connected = false;
    _doScan = false;    
    if (!BLE.begin()) {
      Serial.println("starting Bluetooth® Low Energy module failed!");
      while (1);
    }
  }

/*---------------------------------------*/
  void BLETempBase::setupScan() {
    if (_debug == true)
      Serial.println("setupScan");
    setup();
    BLE.scan();
    _doScan = true;
  }

/*---------------------------------------*/
  void BLETempBase::setupScan(String suuid, String cuuid){
    if (_debug == true)
      Serial.println("setupScan");
    _serviceUUID = suuid;
    _characteristicUUID = cuuid;
    setupScan();
  }

  void BLETempBase::loop() {
    if ( _doScan == true) {
      _findDevice();
    } else {
      if (_connected != true)
      {
        _DoConnect();
      } else {
        _readValues();
      }    
    }

  }
/*---------------------------------------*/
  String BLETempBase::saddress() {
    char saddress[18];
    String result;
    sprintf(saddress, "%02x:%02x:%02x:%02x:%02x:%02x", _address[0], _address[1], _address[2], _address[3], _address[4], _address[5]);
    result = saddress;
    return result;
  }

/*---------------------------------------*/
  void BLETempBase::_findDevice(void) {
    if (_debug == true)
      Serial.println("findDevice");
    BLEDevice peripheral = BLE.available();
    if (peripheral) {
      if (_debug == true)
      {
        Serial.print("Found ");
        Serial.print(peripheral.address());
        Serial.print(" '");
        Serial.print(peripheral.localName());
        Serial.print("' ");
        Serial.print(peripheral.advertisedServiceUuid());
        Serial.println();
      }
      if (_useAddress == false)
      {
        if (_debug == true) {
          Serial.print("check Name:");
          Serial.println(_name);
        }
        if (peripheral.localName() == _name) {
          _myDevice = peripheral;
          BLE.stopScan();
          _doScan = false;
        }
      } else {
        if (_debug == true) {
          Serial.print("check Adress:");
          Serial.println(saddress());
        }
        if (peripheral.address() == saddress() )
        {
          _myDevice = peripheral;
          BLE.stopScan();
          _doScan = false;
        }        
      }
    }
  }
/*---------------------------------------*/
  void BLETempBase::_DoConnect( void ) {
    if (_debug == true)
      Serial.println("DoConnect");
    if (_myDevice.connect()) {
      if (_debug == true) {
        Serial.println("Connected");
      }
      _connected = true;
    } else {
      Serial.println("Failed to connect!");
    }
  }

/*---------------------------------------*/
/* Read the values vom the Characteristic*/
  void BLETempBase::_readValues(void) {
    if (_debug == true)
      Serial.println("_readValues");
    if (_myDevice.discoverAttributes()) {
      if (_debug == true) {
        Serial.println("Attributes discovered");
      }
    } else {
      Serial.println("Attribute discovery failed!");
      _valueLength = 0;
      _connected = _myDevice.connected();
    }
    BLEService service = _myDevice.service(_serviceUUID.c_str());
    BLECharacteristic characteristic = service.characteristic( _characteristicUUID.c_str() );
  // check if the characteristic is readable
    if (characteristic.canRead()) {
      // read the characteristic value
      characteristic.read();
      if (characteristic.valueLength() > 0) {
        _saveBuffer(characteristic.value(), characteristic.valueLength());
      }
    }
  }

/*---------------------------------------*/
void BLETempBase::_saveBuffer(const unsigned char data[], int length){
  _valueLength = length;
  for (int i = 0; i < length; i++) {
   _valueBuff[i] = data[i];
  }
}

/*---------------------------------------*/
float BLETempBase::Temperature() {
  float temp = 0;
  if (_valueLength > 0)
  {
    temp = ((_valueBuff[1]<<8) | _valueBuff[0])/100;
  }
  return temp;  
}

/*---------------------------------------*/
float BLETempBase::Humidity() {
  float hum = 0;
  if (_valueLength > 0)
  {
    hum = ((_valueBuff[3]<<8) | _valueBuff[2])/100;
  }
  return hum;
}

/*---------------------------------------*/
int BLETempBase::Battery() {
  int bat = 100;
  return bat;
}

/*---------------------------------------*/
void BLETempBase::setService(String uuid) {
    _serviceUUID = uuid;
}

/*---------------------------------------*/
void BLETempBase::setCharacteristic(String uuid) {
    _characteristicUUID = uuid;
}

/*---------------------------------------*/
boolean BLETempBase::hasValues() {
  boolean has;
  has = (_valueLength > 0);
  return has;
}

/*---------------------------------------*/
void BLETempBase::setDebugMode(bool mode) {
  _debug = mode;
}
