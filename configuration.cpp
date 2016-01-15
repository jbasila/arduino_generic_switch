
#include <Arduino.h>
#include "pgmString.h"
#include <EEPROM.h>

#include "configuration.h"
#include "debugInfo.h"

Configuration::Configuration(const int _iResetPin) :
  m_bRequireStoring(false),
  m_iResetPin(_iResetPin) {
  // Setup the reset configuration pin
  pinMode(m_iResetPin, INPUT_PULLUP);

  memset(&g_configuration, 0, sizeof(g_configuration));
  memcpy(&g_configuration.m_header.m_magic, HEADER_MAGIC, sizeof(HEADER_MAGIC) - 1);

  g_configuration.m_header.m_size = sizeof(g_configuration);
}

Configuration::~Configuration() {
}

Configuration::SupportedDevices Configuration::convert(const String& _sDevice) {
  if (_sDevice == getPgmString(STR_ON_OFF)) return E_DEVICE_ON_OFF;
  else if (_sDevice == getPgmString(STR_TRIGGER)) return E_DEVICE_TRIGGER;
  else if (_sDevice == getPgmString(STR_SENSOR)) return E_DEVICE_SENSOR;
  else return E_DEVICE_UNKNOWN;
}

String Configuration::convert(const SupportedDevices _eDevice) {
  switch (_eDevice) {
    case E_DEVICE_ON_OFF:
      return getPgmString(STR_ON_OFF);
      break;

    case E_DEVICE_TRIGGER:
      return getPgmString(STR_TRIGGER);
      break;

    case E_DEVICE_SENSOR:
      return getPgmString(STR_SENSOR);
      break;

    default:
      return getPgmString(STR_UNKNOWN);
      break;
  }

  return getPgmString(STR_UNKNOWN);
}

bool Configuration::restoreConfiguration() {
  bool _bReturnValue = false;

  // If the EEPROM has valid Magic and size, then there
  // is a valid configuration in the EEPROM, read it.
  // Otherwise, start from blank.

  // Check the reset PIN, if it's set LOW, then the configuration
  // will be erased
  if (digitalRead(m_iResetPin)) {
    // comparing EEPROM memory with the configuration header
    byte* _pConfiguration = (byte*)&g_configuration;
    unsigned int _uiIndex;
    for (_uiIndex = 0; _uiIndex < sizeof(g_configuration.m_header); ++_uiIndex) {
      if (_pConfiguration[_uiIndex] != EEPROM[_uiIndex]) {
        break;
      }
    }

    if (_uiIndex == sizeof(g_configuration.m_header))
    {
      for (_uiIndex = 0; _uiIndex < sizeof(g_configuration); ++_uiIndex)
        _pConfiguration[_uiIndex] = EEPROM[_uiIndex];

      _bReturnValue = true;
    } else
      m_bRequireStoring = true;
  } else
    m_bRequireStoring = true;

  return _bReturnValue;
}

bool Configuration::storeConfiguration() {
  bool _bReturnValue = false;

  if (m_bRequireStoring) {
    byte* _pConfiguration = (byte*)&g_configuration;
    unsigned int _uiIndex;
    for (_uiIndex = 0; _uiIndex < sizeof(g_configuration); ++_uiIndex)
      EEPROM[_uiIndex] = _pConfiguration[_uiIndex];

    m_bRequireStoring = false;
    _bReturnValue = true;
  }

  return _bReturnValue;
}

byte Configuration::maxNumberOfDevices() {
  return NUMBER_OF_DEVICES;
}

bool Configuration::getDeviceParams(const byte _deviceIndex,
                                    SupportedDevices& _deviceId,
                                    byte& _param1,
                                    byte& _param2,
                                    Configuration::PrivateData& _privateData) {
  bool _bReturnValue = false;

  if (_deviceIndex < maxNumberOfDevices()) {
    _deviceId = g_configuration.m_devices[_deviceIndex].m_deviceId;
    _param1 = g_configuration.m_devices[_deviceIndex].m_param1;
    _param2 = g_configuration.m_devices[_deviceIndex].m_param2;
    _privateData = g_configuration.m_devices[_deviceIndex].m_privateData;

    _bReturnValue = true;
  }

  return _bReturnValue;
}

bool Configuration::setDeviceParams(const byte _deviceIndex,
                                    const SupportedDevices _deviceId,
                                    const byte _param1,
                                    const byte _param2,
                                    const Configuration::PrivateData& _privateData) {
  bool _bReturnValue = false;

  if (_deviceIndex >= 0 && _deviceIndex < maxNumberOfDevices()) {
    g_configuration.m_devices[_deviceIndex].m_deviceId = _deviceId;
    g_configuration.m_devices[_deviceIndex].m_param1 = _param1;
    g_configuration.m_devices[_deviceIndex].m_param2 = _param2;
    g_configuration.m_devices[_deviceIndex].m_privateData = _privateData;

    m_bRequireStoring = true;
    _bReturnValue = true;
  }

  return _bReturnValue;
}

void Configuration::dumpConfiguration(String& _sDump) {
  dumpMemory(&g_configuration, sizeof(g_configuration), _sDump);
}

