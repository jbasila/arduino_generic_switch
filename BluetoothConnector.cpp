
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
  flushInputBuffer();
  m_bluetoothSerial.print(getPgmString(STR_ATNAME) + _deviceName);
  return checkValidResponse();
}

bool BluetoothConnector::setBaudRate(const long _baudRate) {
  powerCycle();
  m_bluetoothSerial.print(getPgmString(STR_ATBAUD));
  m_bluetoothSerial.print((char)(convertFromBaudRate(_baudRate) + '0'));

  delay(1000);
  m_bluetoothSerial.end();
  m_bluetoothSerial.begin(_baudRate);
  return true;
}

bool BluetoothConnector::setPin(const String& _pinCode) {
  powerCycle();
  m_bluetoothSerial.print(getPgmString(STR_ATPIN) + _pinCode);
  return checkValidResponse();
}

void BluetoothConnector::powerCycle() {
  digitalWrite(m_powerPin, HIGH);
  delay(250);
  digitalWrite(m_powerPin, LOW);
  // Give the device some time to reset
  delay(1000);
}

Stream* BluetoothConnector::getStream() {
  return &m_bluetoothSerial;
}

bool BluetoothConnector::checkBaudRate(long _baudRate) {
  if (!_baudRate)
    return false;

  m_bluetoothSerial.end();
  powerCycle();
  m_bluetoothSerial.begin(_baudRate);
  m_bluetoothSerial.print("AT");
  
  return checkValidResponse();
}

bool BluetoothConnector::checkValidResponse() {
  bool _bReturnValue = false;

  if (blockingRead(1000) == 'O') {
    if (blockingRead(1000) == 'K') {
      flushInputBuffer();

      _bReturnValue = true;
    }
  }

  return _bReturnValue;
}

void BluetoothConnector::flushInputBuffer() {
  while (blockingRead(200) != -1);
}

int BluetoothConnector::blockingRead(unsigned long _timeout) {
  int _iReturnValue = -1;

  unsigned long _timeToStop = millis() + _timeout;
  while ((_iReturnValue = m_bluetoothSerial.read()) == -1) {
    if (millis() >= _timeToStop)
      break;
  }

  return _iReturnValue;
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

