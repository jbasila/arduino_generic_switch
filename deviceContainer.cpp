
#include <Arduino.h>

#include "configuration.h"
#include "ILogic.h"
#include "BasicLogic.h"
#include "OnOffSwitchLogic.h"
#include "TriggerLogic.h"
#include "SensorLogic.h"
#include "deviceContainer.h"

DeviceContainer::DeviceContainer() {
  memset(m_aLogicArray, 0, sizeof(m_aLogicArray));
}

DeviceContainer::~DeviceContainer() {}

bool DeviceContainer::setLogic(const byte _index,
                               const Configuration::SupportedDevices _deviceId,
                               const byte _param1,
                               const byte _param2,
                               const Configuration::PrivateData& _privateData) {
  bool _bReturnValue = false;

  if (_index >= 0 && _index < NUMBER_OF_DEVICES) {
    ILogic* _pNewLogic = NULL;
    switch (_deviceId) {
      case Configuration::E_DEVICE_ON_OFF:
        _pNewLogic = new OnOffSwitchLogic(_deviceId);
        break;

      case Configuration::E_DEVICE_TRIGGER:
        _pNewLogic = new TriggerLogic(_deviceId);
        break;

      case Configuration::E_DEVICE_SENSOR:
        _pNewLogic = new SensorLogic(_deviceId);
        break;

      default:
        break;
    }

    if (_pNewLogic) {
      if (_pNewLogic->init(_param1, _param2, _privateData)) {
        if (m_aLogicArray[_index]) {
          delete m_aLogicArray[_index];
          m_aLogicArray[_index] = NULL;
        }

        m_aLogicArray[_index] = _pNewLogic;
        _bReturnValue = true;
      }
    }
  }

  return _bReturnValue;
}

void DeviceContainer::clearLogic(const byte _index) {
  if (_index >= 0 && _index < NUMBER_OF_DEVICES) {
    if (m_aLogicArray[_index])
      delete m_aLogicArray[_index];
    m_aLogicArray[_index] = NULL;
  }
}

ILogic* DeviceContainer::getLogic(const byte _index) {
  ILogic* _pReturnValue = NULL;

  if (_index >= 0 && _index < NUMBER_OF_DEVICES)
    _pReturnValue = m_aLogicArray[_index];

  return _pReturnValue;
}

