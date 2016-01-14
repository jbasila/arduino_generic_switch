
#ifndef _TRIGGER_LOGIC_H
#define _TRIGGER_LOGIC_H

#define DEBOUNCE_DELAY 50

class TriggerLogic : public BasicLogic {
  public:
    TriggerLogic(const Configuration::SupportedDevices _deviceId);
    ~TriggerLogic();

  private:
    virtual bool initializeInstnace(const byte _param1, const byte _param2, const Configuration::PrivateData& _privateData);
    virtual void getPrivateData(Configuration::PrivateData& _privateData);
    virtual LogicAction action(const byte _action, const byte _param, String& _sResponse);
    virtual LogicAction tick(String& _sResponse);

    bool m_active;
    bool m_edgeEvent; // true - rising edge, false - falling edge.
    byte m_triggerPin;
    bool m_lastKnownState;
    bool m_lastCycleState;
    bool m_debounce;
    unsigned long m_lastDebounceTime;
};

#endif//_TRIGGER_LOGIC_H

