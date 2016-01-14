
#include <Arduino.h>
#include "configuration.h"
#include "ILogic.h"
#include "BasicLogic.h"
#include "SensorLogic.h"
#include "pgmString.h"

SensorLogic::SensorLogic(const Configuration::SupportedDevices _deviceId) :
  BasicLogic(_deviceId),
  m_analogInputPin(0),
  m_lowThreshold(0),
  m_highThreshold(0),
  m_thresholdStatus(E_THRESHOLD_OFF) {}

SensorLogic::~SensorLogic() {}

bool SensorLogic::initializeInstnace(const byte _param1, const byte _param2, const Configuration::PrivateData& _privateData) {
  bool _bReturnValue = false;

  m_analogInputPin = getInputPin(_param1);
  if (m_analogInputPin) {
    pinMode(m_analogInputPin, INPUT);
    m_lowThreshold = _privateData.m_privateData[0] * 256;
    m_lowThreshold += _privateData.m_privateData[1];

    m_highThreshold = _privateData.m_privateData[2] * 256;
    m_highThreshold += _privateData.m_privateData[3];

    _bReturnValue = true;
  }

  return _bReturnValue;
}

void SensorLogic::getPrivateData(Configuration::PrivateData& _privateData) {
  _privateData.m_privateData[0] = m_lowThreshold / 256;
  _privateData.m_privateData[1] = m_lowThreshold % 256;

  _privateData.m_privateData[2]= m_highThreshold / 256;
  _privateData.m_privateData[3]= m_highThreshold % 256;
}

ILogic::LogicAction SensorLogic::action(const byte _action, const byte _param, String& _sResponse) {
  LogicAction _laReturnValue = E_ACTION_RESPONSE;

  switch (_action) {
    case 0: {
        // Read sensor value
        _sResponse += getPgmString(STR_SENSOR__STATE);
        char _tmpBuffer[16];
        sprintf(_tmpBuffer, "%d", analogRead(m_analogInputPin));
        _sResponse += _tmpBuffer;
        break;
      }

    case 1: {
        // Display thresholds
        _sResponse += getPgmString(STR_SENSOR__STATE);
        char _tmpBuffer[32];
        sprintf(_tmpBuffer, "L(%d) H(%d) S(%d)", m_lowThreshold, m_highThreshold, m_thresholdStatus);
        _sResponse += _tmpBuffer;
        break;
      }

    case 2: {
        // Set active threshold
        _sResponse += getPgmString(STR_SENSOR__SET_ACTIVE_THRESHOLD);
        if (_param) {
          // Activate threshold notification
          // set the correct pending threshold notification
          m_thresholdStatus = calculateCurrentStatus();

          _sResponse += getPgmString(STR_ON);
        } else {
          // Disable threshold notification
          m_thresholdStatus = E_THRESHOLD_OFF;
          _sResponse += getPgmString(STR_OFF);
        }
        break;
      }

    case 10 ... 13: {
        // Set lower threshold
        m_lowThreshold = (_action % 10) * 256 + _param;
        _sResponse += getPgmString(STR_SENSOR__SET_THRESHOLD);
        char _tmpBuffer[16];
        sprintf(_tmpBuffer, "L(%d)", m_lowThreshold);
        _sResponse += _tmpBuffer;
        break;
      }

    case 20 ... 23: {
        // Set lower threshold
        m_highThreshold = (_action % 10) * 256 + _param;
        _sResponse += getPgmString(STR_SENSOR__SET_THRESHOLD);
        char _tmpBuffer[16];
        sprintf(_tmpBuffer, "H(%d)", m_highThreshold);
        _sResponse += _tmpBuffer;
        break;
      }

    default:
      // Read switch state
      _sResponse += getPgmString(STR_INVALID_ACTION);
  }

  return _laReturnValue;
}

ILogic::LogicAction SensorLogic::tick(String& _sResponse) {
  LogicAction _eReturnValue = E_ACTION_OK;

  switch (m_thresholdStatus) {
    case E_THRESHOLD_OFF: {
        break;
      }

    case E_THRESHOLD_LOW: {
        if (analogRead(m_analogInputPin) <= m_lowThreshold) {
          _sResponse += getPgmString(STR_LOW);
          m_thresholdStatus = E_THRESHOLD_HIGH;
          _eReturnValue = E_ACTION_RESPONSE;
        }
        break;
      }

    case E_THRESHOLD_HIGH: {
        if (analogRead(m_analogInputPin) >= m_highThreshold) {
          _sResponse += getPgmString(STR_HIGH);
          m_thresholdStatus = E_THRESHOLD_LOW;
          _eReturnValue = E_ACTION_RESPONSE;
        }
        break;
      }
  }

  return _eReturnValue;
}

SensorLogic::E_THRESHOLD SensorLogic::calculateCurrentStatus() {
  E_THRESHOLD _thresholdStatus = E_THRESHOLD_OFF;

  if (analogRead(m_analogInputPin) < ((m_highThreshold + m_lowThreshold) / 2))
    _thresholdStatus = E_THRESHOLD_HIGH;
  else
    _thresholdStatus = E_THRESHOLD_LOW;

  return _thresholdStatus;
}

