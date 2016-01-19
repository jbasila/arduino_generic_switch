
#ifndef _BLUETOOTHCONNECTOR_H
#define _BLUETOOTHCONNECTOR_H

class BluetoothConnector {
  public:
    BluetoothConnector(int _rx, int _tx, int _powerPin);
    ~BluetoothConnector();

    bool detectBaudRate();
    bool setDeviceName(const String& _deviceName);
    bool setBaudRate(const byte _baudRate);
    bool setPin(const String& _pinCode);

    Stream* getStream();

  private:
    bool checkBaudRate(long _baudRate);
    bool checkValidResponse();
    int m_powerPin;
    
    SoftwareSerial m_bluetoothSerial;
};

#endif//_BLUETOOTHCONNECTOR_H

