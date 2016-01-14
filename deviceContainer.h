
#ifndef _DEVICE_CONTAINER_H
#define _DEVICE_CONTAINER_H

class DeviceContainer {
  public:
    DeviceContainer();
    virtual ~DeviceContainer();

    bool setLogic(const byte _index,
                  const Configuration::SupportedDevices _deviceId,
                  const byte _param1,
                  const byte _param2,
                  const Configuration::PrivateData& _privateData);
    void clearLogic(const byte _index);
    ILogic* getLogic(const byte _index);

  private:
    ILogic* m_aLogicArray[NUMBER_OF_DEVICES];
};

#endif//_DEVICE_CONTAINER_H

