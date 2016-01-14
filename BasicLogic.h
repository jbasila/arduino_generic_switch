
#ifndef _BASIC_LOGIC_H
#define _BASIC_LOGIC_H

#define MAX_NUMBER_OF_IO_PINS 6

class BasicLogic : public ILogic {
  public:
    BasicLogic(const Configuration::SupportedDevices _deviceId);
    ~BasicLogic();

    virtual LogicAction action(const byte _action, const byte _param, String& _sResponse) = 0;
    virtual LogicAction tick(String& _sResponse) = 0;

  protected:
    virtual bool initializeInstnace(const byte _param1, const byte _param2, const Configuration::PrivateData& _privateData) = 0;
    virtual void getPrivateData(Configuration::PrivateData& _privateData) = 0;
    
    byte getInputPin(const byte _iPinId);
    byte getOutputPin(const byte _iPinId);

    static const byte m_inputPinMapping[];
    static const byte m_outputPinMapping[];

  private:
    virtual bool init(const byte _param1, const byte _param2, const Configuration::PrivateData& _privateData);
    virtual void getParams(Configuration::SupportedDevices& _deviceId,
                           byte& _param1,
                           byte& _param2,
                           Configuration::PrivateData& _privateData);

    byte m_initialParam1;
    byte m_initialParam2;
};

#endif//_BASIC_LOGIC_H

