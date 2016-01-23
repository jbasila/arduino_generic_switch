
#include <Arduino.h>
#include <SoftwareSerial.h>

#include "SerialCommand.h"
#include "pgmString.h"
#include "configuration.h"
#include "ILogic.h"
#include "deviceContainer.h"
#include "BluetoothConnector.h"

static Stream* g_pStreamToUse = NULL;
static SerialCommand* g_pSerialCommand = NULL;
static DeviceContainer* g_pDeviceContainer = NULL;
static Configuration* g_pConfiguration = NULL;
static BluetoothConnector* g_pBluetoothConnector = NULL;

static void commandHelp() {
  char* _pToken = g_pSerialCommand->next();
  if (!_pToken) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_HELP));
    g_pStreamToUse->println();
    return;
  }
  String _sCommand = _pToken;

  _pToken = g_pSerialCommand->next();
  if (_pToken) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_HELP_USAGE));
    g_pStreamToUse->println();
    return;
  }

  if (_sCommand == getPgmString(STR_CMD_SET)) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_SET_USAGE));
    g_pStreamToUse->println(getPgmString(STR_COMMAND_HELP_SET));
    g_pStreamToUse->println();
  } else if (_sCommand == getPgmString(STR_CMD_ACTION)) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_ACTION_USAGE));
    g_pStreamToUse->println(getPgmString(STR_COMMAND_HELP_ACTION));
    g_pStreamToUse->println();
  } else {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_HELP_USAGE));
    g_pStreamToUse->println();
  }
}

static void commandList() {
  char _atmpBuffer[64] = { 0 };
  g_pStreamToUse->println(getPgmString(STR_COMMAND_LIST_FORMAT_HEADER));
  g_pStreamToUse->println(getPgmString(STR_COMMAND_LIST_FORMAT_LINE));

  ILogic* _pIlogic = NULL;
  String _sFormatString = getPgmString(STR_COMMAND_LIST_FORMAT_STRING);
  byte _iIndex = 0;
  for (_iIndex = 0; _iIndex < NUMBER_OF_DEVICES; ++_iIndex) {
    String _sDeviceId;
    byte _param1;
    byte _param2;
    Configuration::PrivateData _privateData;

    _pIlogic = g_pDeviceContainer->getLogic(_iIndex);
    if (_pIlogic) {
      Configuration::SupportedDevices _deviceId;
      _pIlogic->getParams(_deviceId, _param1, _param2, _privateData);
      _sDeviceId = Configuration::convert(_deviceId);
    } else {
      _sDeviceId = "-";
      _param1 = 0;
      _param2 = 0;
      memset(&_privateData, 0, sizeof(_privateData));
    }
    sprintf(_atmpBuffer, _sFormatString.c_str(),
            _iIndex,
            _sDeviceId.c_str(),
            _param1,
            _param2,
            _privateData.m_privateData[0],
            _privateData.m_privateData[1],
            _privateData.m_privateData[2],
            _privateData.m_privateData[3]);
    g_pStreamToUse->println(_atmpBuffer);
  }
  g_pStreamToUse->println();
}

static void commandSet() {
  char* _pToken = g_pSerialCommand->next();
  if (!_pToken) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_SET_USAGE));
    return;
  }

  String _sTmpString = _pToken;
  if (!isDigit(_sTmpString[0])) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_SET_USAGE));
    return;
  }

  byte _index = _sTmpString.toInt();
  _pToken = g_pSerialCommand->next();
  if (!_pToken) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_SET_USAGE));
    return;
  }

  _sTmpString = _pToken;
  Configuration::SupportedDevices _eDeviceId = Configuration::convert(_sTmpString);
  if (_eDeviceId == Configuration::E_DEVICE_UNKNOWN) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_SET_USAGE));
    return;
  }

  _pToken = g_pSerialCommand->next();
  if (!_pToken) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_SET_USAGE));
    return;
  }

  _sTmpString = _pToken;
  if (!isDigit(_sTmpString[0])) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_SET_USAGE));
    return;
  }

  byte _param1 = _sTmpString.toInt();
  _pToken = g_pSerialCommand->next();
  if (!_pToken) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_SET_USAGE));
    return;
  }

  _sTmpString = _pToken;
  if (!isDigit(_sTmpString[0])) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_SET_USAGE));
    return;
  }

  byte _param2 = _sTmpString.toInt();
  _pToken = g_pSerialCommand->next();

  if (_pToken) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_SET_USAGE));
    return;
  }

  Configuration::PrivateData _privateData;
  memset(&_privateData, 0, sizeof(_privateData));
  g_pStreamToUse->print(getPgmString(STR_COMMAND_SET));
  if (g_pDeviceContainer->setLogic(_index, _eDeviceId, _param1, _param2, _privateData)) {
    // Added a new logic, update the configuration
    ILogic* _pLogic = g_pDeviceContainer->getLogic(_index);
    if (_pLogic) {
      _pLogic->getParams(_eDeviceId, _param1, _param2, _privateData);
      g_pConfiguration->setDeviceParams(_index, _eDeviceId, _param1, _param2, _privateData);
      g_pStreamToUse->println(getPgmString(STR_SUCCESS));
    }
  } else g_pStreamToUse->println(getPgmString(STR_FAILED));
}

static void commandClear() {
  String _sIndex;
  char* _pToken = g_pSerialCommand->next();
  if (!_pToken) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_CLEAR_USAGE));
    return;
  }

  _sIndex = _pToken;
  _pToken = g_pSerialCommand->next();
  if (_pToken) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_CLEAR_TOO_MANY_ARGUMENTS));
    return;
  }

  Configuration::PrivateData _privateData;
  memset(&_privateData, 0, sizeof(_privateData));
  if (_sIndex == "all") {
    int _index;
    for (_index = 0; _index < NUMBER_OF_DEVICES; ++_index) {
      g_pDeviceContainer->clearLogic(_index);
      g_pConfiguration->setDeviceParams(_index, Configuration::E_DEVICE_UNKNOWN, 0, 0, _privateData);
    }
    g_pStreamToUse->println(getPgmString(STR_COMMAND_CLEAR) + getPgmString(STR_ALL));
  } else if (isDigit(_sIndex[0])) {
    g_pDeviceContainer->clearLogic(_sIndex.toInt());
    g_pConfiguration->setDeviceParams(_sIndex.toInt(), Configuration::E_DEVICE_UNKNOWN, 0, 0, _privateData);
    g_pStreamToUse->print(getPgmString(STR_COMMAND_CLEAR));
    g_pStreamToUse->println(_sIndex.toInt());
  } else g_pStreamToUse->println(getPgmString(STR_COMMAND_CLEAR_NOT_NUMBER_OR_ALL));
}

static void commandSave() {
  char* _pToken = g_pSerialCommand->next();
  if (_pToken) {
    ILogic* _pILogic = NULL;
    int _index;
    for (_index = 0; _index < NUMBER_OF_DEVICES; ++_index) {
      ILogic* _pLogic = g_pDeviceContainer->getLogic(_index);
      if (_pLogic) {
        Configuration::SupportedDevices _deviceId;
        byte _param1;
        byte _param2;
        Configuration::PrivateData _privateData;
        _pLogic->getParams(_deviceId, _param1, _param2, _privateData);
        g_pConfiguration->setDeviceParams(_index, _deviceId, _param1, _param2, _privateData);
      }
    }
  }

  g_pStreamToUse->print(getPgmString(STR_COMMAND_SAVE));
  g_pStreamToUse->println(getPgmString(g_pConfiguration->storeConfiguration() ? STR_YES : STR_NO));
}

static void commandAction() {
  char* _pToken = g_pSerialCommand->next();
  if (!_pToken) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_ACTION_USAGE));
    return;
  }

  String _sTmpString = _pToken;
  byte _index = _sTmpString.toInt();
  _pToken = g_pSerialCommand->next();
  if (!_pToken) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_ACTION_USAGE));
    return;
  }

  _sTmpString = _pToken;
  byte _action = _sTmpString.toInt();
  _pToken = g_pSerialCommand->next();
  if (!_pToken) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_ACTION_USAGE));
    return;
  }

  _sTmpString = _pToken;
  byte _param = _sTmpString.toInt();

  // fetch the logic
  ILogic* _pLogic = g_pDeviceContainer->getLogic(_index);
  if (!_pLogic) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_ACTION_INVALID_LOGIC));
    return;
  }

  String _sResponse;
  switch (_pLogic->action(_action, _param, _sResponse)) {
    case ILogic::E_ACTION_RESPONSE:
      g_pStreamToUse->print(getPgmString(STR_ACTION));
      g_pStreamToUse->print((char)('0' + _index));
      g_pStreamToUse->print(getPgmString(STR_SEMICOLON_SPACE));
      g_pStreamToUse->println(_sResponse);
      break;

    case ILogic::E_ACTION_OK:
      break;

    default:
      break;
  }
}

static void commandFreeMem() {
  void* _pMem = NULL;
  int _size = 1024 * 2;
  for (_size = 1024 * 2; _size > 0; --_size) {
    _pMem = malloc(_size);
    if (_pMem) {
      free(_pMem);
      g_pStreamToUse->print(getPgmString(STR_COMMAND_FREE));
      g_pStreamToUse->println(_size);
      break;
    }
  }
}

static void commandBt() {
  char* _pToken = NULL;
  if (!(_pToken = g_pSerialCommand->next())) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_BT_USAGE));
    return;
  }
  String _sBaud = _pToken;

  if (!(_pToken = g_pSerialCommand->next())) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_BT_USAGE));
    return;
  }
  String _sName = _pToken;

  if (!(_pToken = g_pSerialCommand->next())) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_BT_USAGE));
    return;
  }
  String _sPin = _pToken;

  long _baudRate = _sBaud.toInt();
  if (!g_pBluetoothConnector->isValidBaudRate(_baudRate)) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_BT_ERROR_INVALID_BAUD_RATE) + _sBaud);
    return;
  }

  if (_sPin.length() != 4) {
    g_pStreamToUse->println(getPgmString(STR_COMMAND_BT_ERROR_INVALID_PIN_CODE) + _sPin);
    return;
  }

  int _iIndex = 0;
  for (_iIndex = 0; _iIndex < 4; ++_iIndex) {
    if (!isDigit(_sPin[_iIndex])) {
      g_pStreamToUse->println(getPgmString(STR_COMMAND_BT_ERROR_INVALID_PIN_CODE) + _sPin);
      return;
    }
  }

  if (!g_pBluetoothConnector->setPin(_sPin))
    Serial.println(getPgmString(STR_DBG_BT_FAILED_SET_PIN));
  delay(1000);
  if (!g_pBluetoothConnector->setDeviceName(_sName))
    Serial.println(getPgmString(STR_DBG_BT_FAILED_SET_DEVICE_NAME));
  delay(1000);
  if (!g_pBluetoothConnector->setBaudRate(_baudRate))
    Serial.println(getPgmString(STR_DBG_BT_FAILED_SET_BAUD_RATE));
  delay(1000);
}

static void commandUnknown() {
  g_pStreamToUse->println(getPgmString(STR_COMMAND_UNKNOWN));
}

bool cliCommands_init(Stream& _stream,
                      DeviceContainer& _deviceContainer,
                      Configuration& _configuration,
                      BluetoothConnector& _pBluetoothConnector) {
  g_pStreamToUse = &_stream;
  g_pDeviceContainer = &_deviceContainer;
  g_pConfiguration = &_configuration;
  g_pBluetoothConnector = &_pBluetoothConnector;

  g_pSerialCommand = new SerialCommand(*g_pStreamToUse);
  g_pSerialCommand->setTerm('\n');
  g_pSerialCommand->addCommand(getPgmString(STR_CMD_HELP), commandHelp);
  g_pSerialCommand->addCommand(getPgmString(STR_CMD_LIST), commandList);
  g_pSerialCommand->addCommand(getPgmString(STR_CMD_SET), commandSet);
  g_pSerialCommand->addCommand(getPgmString(STR_CMD_CLEAR), commandClear);
  g_pSerialCommand->addCommand(getPgmString(STR_CMD_SAVE), commandSave);

  g_pSerialCommand->addCommand(getPgmString(STR_CMD_ACTION), commandAction);

  g_pSerialCommand->addCommand(getPgmString(STR_CMD_BT), commandBt);

  g_pSerialCommand->addCommand(getPgmString(STR_CMD_FREE), commandFreeMem);
  g_pSerialCommand->addDefaultHandler(commandUnknown);
}

void cliCommands_readSerial() {
  if (g_pSerialCommand)
    g_pSerialCommand->readSerial();
}

