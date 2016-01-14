
#ifndef _ON_OFF_SWITCH_LOGIC_H
#define _ON_OFF_SWITCH_LOGIC_H

class OnOffSwitchLogic : public BasicLogic {
  public:
    OnOffSwitchLogic(const Configuration::SupportedDevices _deviceId);
    ~OnOffSwitchLogic();

  private:
    virtual bool initializeInstnace(const byte _param1, const byte _param2, const Configuration::PrivateData& _privateData);
    virtual void getPrivateData(Configuration::PrivateData& _privateData);
    virtual LogicAction action(const byte _action, const byte _param, String& _sResponse);
    virtual LogicAction tick(String& _sResponse);

    byte m_outputPin;
};

#endif//_ON_OFF_SWITCH_LOGIC_H

