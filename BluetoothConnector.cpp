
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

bool BluetoothConnector::detectBaudRate(long& _detectedBaudRate) {
  bool _bReturnValue = false;

  // Sending an AT command should yield an OK response
  for (byte _baudRateIndex = 8; _baudRateIndex > 0; --_baudRateIndex) {

    long _serialBaudRate = convertToBaudRate(_baudRateIndex);
    Serial.print("Checking: ");
    Serial.println(_serialBaudRate);
    if (checkBaudRate(_serialBaudRate)) {
      // Baud rate detected!
      _detectedBaudRate = _serialBaudRate;
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

bool BluetoothConnector::setBaudRate(const long _baudRate) {
  while (m_bluetoothSerial.read() != -1);
  m_bluetoothSerial.print(getPgmString(STR_ATBAUD));
  m_bluetoothSerial.print((char)(convertFromBaudRate(_baudRate) + '0'));

  delay(1000);
  m_bluetoothSerial.end();
  m_bluetoothSerial.begin(_baudRate);
  return true;
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
  if (!_baudRate)
    return false;

  m_bluetoothSerial.end();
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
  if (m_bluetoothSerial.read() == 'O') {
    if (m_bluetoothSerial.read() == 'K') {
      do {
        delay(200);
      } while (m_bluetoothSerial.read() != -1);
      _bReturnValue = true;
    }
  }

  return _bReturnValue;
}

byte BluetoothConnector::convertFromBaudRate(const long _baudRate) {
  switch (_baudRate) {
    case 115200:
      return 8;

    case 57600:
      return 7;

    case 38400:
      return 6;

    case 19200:
      return 5;

    case 9600:
      return 4;

    case 4800:
      return 3;

    case 2400:
      return 2;

    case 1200:
      return 1;
  }

  return 0;
}

long BluetoothConnector::convertToBaudRate(const byte _baudRateCode) {
  switch (_baudRateCode) {
    case 8:
      return 115200;

    case 7:
      return 57600;

    case 6:
      return 38400;

    case 5:
      return 19200;

    case 4:
      return 9600;

    case 3:
      return 4800;

    case 2:
      return 2400;

    case 1:
      return 1200;
  }

  return 0;
}

