
#ifndef _ILOGIC_H
#define _ILOGIC_H

class ILogic {
  public:
    enum LogicAction {
      E_ACTION_UNKNOW,
      E_ACTION_RESPONSE,
      E_ACTION_OK,
      E_ACTION_ERROR,
      E_ACTION_SIZE
    };

    ILogic(const Configuration::SupportedDevices _deviceId) : m_deviceId(_deviceId) {}
    virtual ~ILogic() {}

    virtual bool init(const byte _param1, const byte _param2, const Configuration::PrivateData& _privateData) = 0;
    virtual void getParams(Configuration::SupportedDevices& _deviceId,
                           byte& _param1,
                           byte& _param2,
                           Configuration::PrivateData& _privateData) = 0;
    virtual LogicAction action(const byte _action, const byte _param, String& _sResponse) = 0;
    virtual LogicAction tick(String& _sResponse) = 0;

  protected:
    Configuration::SupportedDevices m_deviceId;
};

#endif//_ILOGIC_H

