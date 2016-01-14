
#include <Arduino.h>
#include "pgmString.h"

#include "configuration.h"
#include "ILogic.h"
#include "BasicLogic.h"
#include "OnOffSwitchLogic.h"

OnOffSwitchLogic::OnOffSwitchLogic(const Configuration::SupportedDevices _deviceId) : BasicLogic(_deviceId) {}

OnOffSwitchLogic::~OnOffSwitchLogic() {}

bool OnOffSwitchLogic::initializeInstnace(const byte _param1, const byte _param2, const Configuration::PrivateData& _privateData) {
  bool _bReturnValue = false;

  m_outputPin = getOutputPin(_param1);
  if (m_outputPin) {
    pinMode(m_outputPin, OUTPUT);
    _bReturnValue = true;
  }

  return _bReturnValue;
}

void OnOffSwitchLogic::getPrivateData(Configuration::PrivateData& _privateData) {
}

ILogic::LogicAction OnOffSwitchLogic::action(const byte _action, const byte _param, String& _sResponse) {
  LogicAction _laReturnValue = E_ACTION_RESPONSE;

  if (_action) {
    // Set switch
    _sResponse += getPgmString(STR_ON_OFF__OK_SWITCH);
    if (_param) {
      digitalWrite(m_outputPin, HIGH);
      _sResponse += getPgmString(STR_ON);
    } else {
      digitalWrite(m_outputPin, LOW);
      _sResponse += getPgmString(STR_OFF);
    }
  } else {
    // Read switch state
    _sResponse += getPgmString(STR_ON_OFF__STATE);
    _sResponse += getPgmString(digitalRead(m_outputPin) ? STR_ON : STR_OFF);
  }

  return _laReturnValue;
}

ILogic::LogicAction OnOffSwitchLogic::tick(String& _sResponse) {
  return E_ACTION_OK;
}

