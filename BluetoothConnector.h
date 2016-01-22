
#ifndef _BLUETOOTHCONNECTOR_H
#define _BLUETOOTHCONNECTOR_H

class BluetoothConnector {
  public:
    BluetoothConnector(int _rx, int _tx, int _powerPin);
    ~BluetoothConnector();

    bool checkBaudRate(const long _baudRate);
    bool detectBaudRate(long& _detectedBaudRate);
    bool setDeviceName(const String& _deviceName);
    bool isValidBaudRate(const long _baudRate);
    bool setBaudRate(const long _baudRate);
    bool setPin(const String& _pinCode);

    void powerCycle();

    Stream* getStream();

  private:
    bool checkValidResponse();
    void flushInputBuffer();
    int blockingRead(unsigned long _timeout);

    int m_powerPin;

    byte convertFromBaudRate(const long _baudRate);
    long convertToBaudRate(const byte _baudRateCode);

    SoftwareSerial m_bluetoothSerial;
};

#endif//_BLUETOOTHCONNECTOR_H

