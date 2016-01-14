
#include <Arduino.h>

#include "configuration.h"
#include "ILogic.h"
#include "BasicLogic.h"

#define NUMBER_OF_INPUT_PINS 6
#define NUMBER_OF_OUTPUT_PINS 6

const byte BasicLogic::m_inputPinMapping[NUMBER_OF_INPUT_PINS] = { A0, A1, A2, A3, A4, A5 };
const byte BasicLogic::m_outputPinMapping[NUMBER_OF_OUTPUT_PINS] = { 8, 9, 10, 11, 12, 13 };

BasicLogic::BasicLogic(const Configuration::SupportedDevices _deviceId) : ILogic(_deviceId) {}
BasicLogic::~BasicLogic() {}

byte BasicLogic::getInputPin(const byte _iPinId) {
  byte _returnValue = 0;

  if (_iPinId >= 0 && _iPinId < NUMBER_OF_INPUT_PINS)
    _returnValue = m_inputPinMapping[_iPinId];

  return _returnValue;
}

byte BasicLogic::getOutputPin(const byte _iPinId) {
byte _returnValue = 0;

  if (_iPinId >= 0 && _iPinId < NUMBER_OF_OUTPUT_PINS)
    _returnValue = m_outputPinMapping[_iPinId];

  return _returnValue;
}

bool BasicLogic::init(const byte _param1, const byte _param2, const Configuration::PrivateData& _privateData) {
  m_initialParam1 = _param1;
  m_initialParam2 = _param2;

  return initializeInstnace(_param1, _param2, _privateData);
}

void BasicLogic::getParams(Configuration::SupportedDevices& _deviceId,
                           byte& _param1,
                           byte& _param2,
                           Configuration::PrivateData& _privateData) {
  _deviceId = m_deviceId;
  _param1 = m_initialParam1;
  _param2 = m_initialParam2;

  memset(&_privateData, 0, sizeof(_privateData));
  getPrivateData(_privateData);
}

