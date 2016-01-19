
#include <SoftwareSerial.h>

#include "pgmString.h"
#include "BluetoothConnector.h"
#include "configuration.h"
#include "ILogic.h"
#include "deviceContainer.h"
#include "cliCommands.h"
#include "debugInfo.h"

#define BLUETOOTH_RX 2
#define BLUETOOTH_TX 3
#define BLUETOOTH_POWER_PIN 6

#define RESET_CONFIGURATION_PIN 7

Configuration g_cConfiguration(RESET_CONFIGURATION_PIN);
DeviceContainer g_cDeviceContainer;
BluetoothConnector* g_pBluetoothConnector = NULL;

Stream* g_pStream = NULL;

bool initializeStream() {
  Serial.begin(9600);
  g_pBluetoothConnector = new BluetoothConnector(BLUETOOTH_RX, BLUETOOTH_TX, BLUETOOTH_POWER_PIN);

  bool _bRequireConfigurationUpdate = false;
  Configuration::BluetoothConfiguration _bluetoothConfiguration = g_cConfiguration.getBluetoothConfiguration();
  if (!_bluetoothConfiguration.m_baudRate) {
    _bRequireConfigurationUpdate = true;
    sprintf(_bluetoothConfiguration.m_deviceName, "UnnamedDevice");
    sprintf(_bluetoothConfiguration.m_pin, "0000");
  }

  if (!g_pBluetoothConnector->checkBaudRate(_bluetoothConfiguration.m_baudRate)) {
    if (!g_pBluetoothConnector->detectBaudRate(_bluetoothConfiguration.m_baudRate))
      return false;

    _bRequireConfigurationUpdate = true;
  }

  Serial.println(_bRequireConfigurationUpdate ? "Config Update" : "restore");
  Serial.print("Setting parameters: ");
  Serial.print(_bluetoothConfiguration.m_baudRate);
  Serial.print(" - ");
  Serial.print(_bluetoothConfiguration.m_pin);
  Serial.print(" - ");
  Serial.println(_bluetoothConfiguration.m_deviceName);
  g_pBluetoothConnector->setPin(_bluetoothConfiguration.m_pin);
  g_pBluetoothConnector->setDeviceName(_bluetoothConfiguration.m_deviceName);

  if (_bRequireConfigurationUpdate)
    g_cConfiguration.setBluetoothConfiguration(_bluetoothConfiguration);

  g_pStream = g_pBluetoothConnector->getStream();
  //    g_pStream = &Serial;
}

void setup() {
  bool _restore = g_cConfiguration.restoreConfiguration();
  initializeStream();
  bool _store = g_cConfiguration.storeConfiguration();

  //  dumpConfigurationAndEeprom(*g_pStream, g_cConfiguration, _restore, _store);

  byte _index;
  for (_index = 0; _index < g_cConfiguration.maxNumberOfDevices(); ++_index) {
    Configuration::SupportedDevices _deviceId;
    byte _param1, _param2;
    Configuration::PrivateData _privateData;
    if ( g_cConfiguration.getDeviceParams(_index, _deviceId, _param1, _param2, _privateData) )
      g_cDeviceContainer.setLogic(_index, _deviceId, _param1, _param2, _privateData);
  }

  cliCommands_init(*g_pStream, g_cDeviceContainer, g_cConfiguration);

  g_pStream->print("\n\nREADY\n");
  Serial.println("\n\nREADY\n");
}

void loop() {
  // Run the logic
  byte _index;
  for (_index = 0; _index < NUMBER_OF_DEVICES; ++_index) {
    ILogic* _pILogic = NULL;

    _pILogic = g_cDeviceContainer.getLogic(_index);
    if (_pILogic) {
      String _sResponse;
      switch (_pILogic->tick(_sResponse)) {
        case ILogic::E_ACTION_RESPONSE:
          g_pStream->print(getPgmString(STR_EVENT));
          g_pStream->print((char)('0' + _index));
          g_pStream->print(getPgmString(STR_SEMICOLON_SPACE));

          _sResponse += '\n';
          g_pStream->print(_sResponse);
          break;
      }
    }
  }

  cliCommands_readSerial();
}

