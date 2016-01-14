
#include <Arduino.h>
#include "configuration.h"
#include "ILogic.h"
#include "BasicLogic.h"
#include "TriggerLogic.h"

#include "pgmString.h"

TriggerLogic::TriggerLogic(const Configuration::SupportedDevices _deviceId) :
  BasicLogic(_deviceId), m_active(false), m_edgeEvent(true), m_debounce(false) {
}

TriggerLogic::~TriggerLogic() {}

bool TriggerLogic::initializeInstnace(const byte _param1, const byte _param2, const Configuration::PrivateData& _privateData) {
  bool _bReturnValue = false;

  m_triggerPin = getInputPin(_param1);
  if (m_triggerPin) {
    m_edgeEvent = _param2;
    pinMode(m_triggerPin, INPUT_PULLUP);
    m_lastKnownState = m_lastCycleState = digitalRead(m_triggerPin);
    m_active = _privateData.m_privateData[0];
    m_edgeEvent = _privateData.m_privateData[1];
    m_debounce = _privateData.m_privateData[2];

    _bReturnValue = true;
  }

  return _bReturnValue;
}

void TriggerLogic::getPrivateData(Configuration::PrivateData& _privateData) {
  _privateData.m_privateData[0] = m_active;
  _privateData.m_privateData[1] = m_edgeEvent;
  _privateData.m_privateData[2] = m_debounce;
}

ILogic::LogicAction TriggerLogic::action(const byte _action, const byte _param, String& _sResponse) {
  LogicAction _laReturnValue = E_ACTION_RESPONSE;

  switch (_action) {
    case 0:
      // Read switch state
      _sResponse += getPgmString(STR_TRIGGER__ACTIVE_STATE);
      _sResponse += getPgmString(m_active ? STR_YES : STR_NO);
      break;

    case 1:
      // Set edge event
      m_active = _param ? true : false;
      _sResponse += getPgmString(STR_TRIGGER__SET_ACTIVE);
      _sResponse += getPgmString(m_active ? STR_YES : STR_NO);
      break;

    case 2:
      m_edgeEvent = _param ? true : false;
      _sResponse += getPgmString(STR_TRIGGER__SET_EDGE);
      _sResponse += getPgmString(m_edgeEvent ? STR_TRIGGER__RISING : STR_TRIGGER__FALLING);
      break;

    case 3:
      m_debounce = _param ? true : false;
      _sResponse += getPgmString(STR_TRIGGER__SET_DEBOUNCE);
      _sResponse += getPgmString(m_debounce ? STR_ON : STR_OFF);
      break;

    default:
      // Read switch state
      _sResponse += getPgmString(STR_INVALID_ACTION);
  }

  return _laReturnValue;
}

ILogic::LogicAction TriggerLogic::tick(String& _sResponse) {
  LogicAction _eReturnValue = E_ACTION_OK;

  if (m_active) {
    bool _readState = digitalRead(m_triggerPin);
    if (m_debounce) {
      if (_readState != m_lastCycleState) {
        // Need to debounce
        m_lastDebounceTime = millis();
      }

      m_lastCycleState = _readState;
      if ((millis() - m_lastDebounceTime) < DEBOUNCE_DELAY) {
        // Still debouncing
        return _eReturnValue;
      }
    }

    if (m_lastKnownState != _readState) {
      switch (m_edgeEvent) {
        case true:
          if (_readState) {
            // Rising edge event - notify
            _sResponse += getPgmString(STR_TRIGGER_EVENT);
            _eReturnValue = E_ACTION_RESPONSE;
          }
          break;

        case false:
          if (m_lastKnownState) {
            // Falling edge event - notify
            _sResponse += getPgmString(STR_TRIGGER_EVENT);
            _eReturnValue = E_ACTION_RESPONSE;
          }
          break;
      }

      m_lastKnownState = _readState;
    }
  }

  return _eReturnValue;
}

