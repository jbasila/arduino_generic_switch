
#include <SoftwareSerial.h>

#include "pgmString.h"
#include "configuration.h"
#include "ILogic.h"
#include "deviceContainer.h"
#include "cliCommands.h"
#include "debugInfo.h"

#define BLUETOOTH_RX 2
#define BLUETOOTH_TX 3

#define RESET_CONFIGURATION_PIN 7

Configuration g_cConfiguration(RESET_CONFIGURATION_PIN);
DeviceContainer g_cDeviceContainer;

Stream* g_pStream = NULL;

void initializeStream() {
  Serial.begin(9600);
  SoftwareSerial* _bluetoothSerial = new SoftwareSerial(BLUETOOTH_RX, BLUETOOTH_TX);
  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);

  _bluetoothSerial->begin(57600);

  g_pStream = _bluetoothSerial;
//  g_pStream = &Serial;
}

void setup() {
  initializeStream();

  bool _restore = g_cConfiguration.restoreConfiguration();
  bool _store = g_cConfiguration.storeConfiguration();

  dumpConfigurationAndEeprom(*g_pStream, g_cConfiguration, _restore, _store);

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

