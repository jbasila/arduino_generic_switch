
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

static Configuration g_cConfiguration(RESET_CONFIGURATION_PIN);
static DeviceContainer g_cDeviceContainer;
static BluetoothConnector* g_pBluetoothConnector = NULL;

Stream* g_pStream = NULL;

bool initializeStream(bool _resetToFactory) {
  Serial.begin(57600);
  g_pBluetoothConnector = new BluetoothConnector(BLUETOOTH_RX, BLUETOOTH_TX, BLUETOOTH_POWER_PIN);

  bool _bRequireConfigurationUpdate = _resetToFactory;
  Configuration::BluetoothConfiguration _bluetoothConfiguration = g_cConfiguration.getBluetoothConfiguration();

  if (!g_pBluetoothConnector->checkBaudRate(_bluetoothConfiguration.m_baudRate)) {
    if (!g_pBluetoothConnector->detectBaudRate(_bluetoothConfiguration.m_baudRate))
      return false;

    _bRequireConfigurationUpdate = true;
  }

  if (_resetToFactory) {
    if (!g_pBluetoothConnector->setDeviceName(getPgmString(STR_BT_DEFAULT_NAME)))
      Serial.println(getPgmString(STR_DBG_BT_FAILED_SET_DEVICE_NAME));
    delay(1000);
    if (!g_pBluetoothConnector->setPin(getPgmString(STR_BT_DEFAULT_PIN)))
      Serial.println(getPgmString(STR_DBG_BT_FAILED_SET_PIN));
  }

  Serial.println(_bRequireConfigurationUpdate ? getPgmString(STR_CFG_UPDATE) : getPgmString(STR_CFG_RESTORED));
  Serial.print(getPgmString(STR_CFG_BAUD_RATE));
  Serial.println(_bluetoothConfiguration.m_baudRate);

  if (_bRequireConfigurationUpdate)
    g_cConfiguration.setBluetoothConfiguration(_bluetoothConfiguration);

  g_pStream = g_pBluetoothConnector->getStream();
  //  g_pStream = &Serial;
}

void setup() {
  bool _restore = g_cConfiguration.restoreConfiguration();
  if (!initializeStream(!_restore)) {
    Serial.println(getPgmString(STR_FATAL_BT));
    return;
  }

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

  cliCommands_init(*g_pStream, g_cDeviceContainer, g_cConfiguration, *g_pBluetoothConnector);

  String _ready = getPgmString(STR_READY);
  g_pStream->println(_ready);
  Serial.println(_ready);
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

