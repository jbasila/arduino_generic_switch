
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "pgmString.h"
#include "BluetoothConnector.h"

BluetoothConnector::BluetoothConnector(int _rx, int _tx, int _powerPin)
  :
  m_powerPin(_powerPin),
  m_bluetoothSerial(_rx, _tx) {
  pinMode(m_powerPin, OUTPUT);
}

BluetoothConnector::~BluetoothConnector() {}

bool BluetoothConnector::detectBaudRate() {
  bool _bReturnValue = false;

  // Sending an AT command should yield an OK response
  for (byte _baudRate = 7; _baudRate > 2; --_baudRate) {
    long _serialBaudRate = _baudRate == 7 ? 57600 : ((long)1200 * (1 << (_baudRate - 1)));
    Serial.print("Checking: ");
    Serial.println(_serialBaudRate);
    if (checkBaudRate(_serialBaudRate)) {
      // Baud rate detected!
      _bReturnValue = true;
      break;
    }
  }

  return _bReturnValue;
}

bool BluetoothConnector::setDeviceName(const String& _deviceName) {
  while (m_bluetoothSerial.read() != -1);
  m_bluetoothSerial.print(getPgmString(STR_ATNAME) + _deviceName);
  return checkValidResponse();
}

bool BluetoothConnector::setBaudRate(const byte _baudRate) {
  while (m_bluetoothSerial.read() != -1);
  m_bluetoothSerial.print(getPgmString(STR_ATBAUD));
  m_bluetoothSerial.print((char)(_baudRate + '0'));

  if (!checkValidResponse())
    return false;

  m_bluetoothSerial.begin(_baudRate);
}

bool BluetoothConnector::setPin(const String& _pinCode) {
  while (m_bluetoothSerial.read() != -1);
  m_bluetoothSerial.print(getPgmString(STR_ATPIN) + _pinCode);
  return checkValidResponse();
}

Stream* BluetoothConnector::getStream() {
  return &m_bluetoothSerial;
}

bool BluetoothConnector::checkBaudRate(long _baudRate) {
  digitalWrite(m_powerPin, HIGH);
  delay(250);
  digitalWrite(m_powerPin, LOW);
  delay(1000);
  m_bluetoothSerial.begin(_baudRate);
  while (m_bluetoothSerial.read() != -1);

  m_bluetoothSerial.print("AT");
  delay(1000);
  return checkValidResponse();
}

bool BluetoothConnector::checkValidResponse() {
  bool _bReturnValue = false;

  delay(2000);
  if (m_bluetoothSerial.read() == 'O')
  {
    if (m_bluetoothSerial.read() == 'K')
      _bReturnValue = true;
  }

  return _bReturnValue;
}

byte BluetoothConnector::convert(const long _baudRate) {

}

long BluetoothConnector::convert(const byte _baudRateCode) {

}

