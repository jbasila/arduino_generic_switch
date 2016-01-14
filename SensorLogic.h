
#ifndef _SENSOR_LOGIC_H
#define _SENSOR_LOGIC_H

class SensorLogic : public BasicLogic {
  public:
    SensorLogic(const Configuration::SupportedDevices _deviceId);
    ~SensorLogic();

  private:
    enum E_THRESHOLD {
      E_THRESHOLD_OFF,
      E_THRESHOLD_HIGH,
      E_THRESHOLD_LOW
    };
    
    virtual bool initializeInstnace(const byte _param1, const byte _param2, const Configuration::PrivateData& _privateData);
    virtual void getPrivateData(Configuration::PrivateData& _privateData);
    virtual LogicAction action(const byte _action, const byte _param, String& _sResponse);
    virtual LogicAction tick(String& _sResponse);

    E_THRESHOLD calculateCurrentStatus();

    byte m_analogInputPin;
    int m_lowThreshold;
    int m_highThreshold;
    E_THRESHOLD m_thresholdStatus;
};

#endif//_SENSOR_LOGIC_H

