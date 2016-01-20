
#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

#define HEADER_MAGIC "V001"
#define NUMBER_OF_DEVICES 6
#define PRIVATE_DATA_SIZE 4

#define BT_DEVICE_NAME_MAX_LEN 16

class Configuration {
  public:
    enum SupportedDevices {
      E_DEVICE_UNKNOWN,
      E_DEVICE_ON_OFF,
      E_DEVICE_TRIGGER,
      E_DEVICE_SENSOR,
      E_DEVICE_SIZE
    };

    class PrivateData {
      public:
        byte m_privateData[PRIVATE_DATA_SIZE];
    };

    class BluetoothConfiguration {
      public:
        long m_baudRate;
    };

    Configuration(const int _iResetPin);
    ~Configuration();

    static SupportedDevices convert(const String& _sDevice);
    static String convert(const SupportedDevices _eDevice);

    bool restoreConfiguration();
    bool storeConfiguration();

    BluetoothConfiguration getBluetoothConfiguration();
    void setBluetoothConfiguration(const BluetoothConfiguration& _bluetoothConfiguration);

    byte maxNumberOfDevices();

    bool getDeviceParams(const byte _deviceIndex,
                         SupportedDevices& _deviceId,
                         byte& _param1,
                         byte& _param2,
                         Configuration::PrivateData& _privateData);

    bool setDeviceParams(const byte _deviceIndex,
                         const SupportedDevices _deviceId,
                         const byte _param1,
                         const byte _param2,
                         const Configuration::PrivateData& _privateData);

    void dumpConfiguration(String& _sDump);

  private:
    bool m_bRequireStoring;
    int m_iResetPin;

    struct {
      struct {
        byte m_magic[sizeof(HEADER_MAGIC) - 1];
        byte m_size;
      } m_header;
      BluetoothConfiguration m_bluetoothConfiguration;
      struct {
        SupportedDevices m_deviceId;
        byte m_param1;
        byte m_param2;
        PrivateData m_privateData;
      } m_devices[NUMBER_OF_DEVICES];
    } g_configuration;
};

#endif //_CONFIGURATION_H

