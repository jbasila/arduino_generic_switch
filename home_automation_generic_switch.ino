                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
#include "pgmString.h"
#include <SoftwareSerial.h>

#define BLUETOOTH_RX 2
#define BLUETOOTH_TX 3
SoftwareSerial g_bluetoothSerial(BLUETOOTH_RX, BLUETOOTH_TX);

#include "dataDumping.h"
#include "configuration.h"

#include "ILogic.h"
#include "deviceContainer.h"

#include "SerialCommand.h"

#define RESET_CONFIGURATION_PIN 7
Configuration g_cConfiguration(RESET_CONFIGURATION_PIN);
DeviceContainer g_cDeviceContainer;
//SerialCommand g_cSerialCommand;
SerialCommand g_cSerialCommand(g_bluetoothSerial);

void printToSerial(const String _sString) {
  //  Serial.print(_sString);
  g_bluetoothSerial.print(_sString);
}

void printToSerial(const char _char) {
  String _sString;
  _sString += _char;
  printToSerial(_sString);
}

void printToSerial(const char* _buffer) {
  String _sString = _buffer;
  printToSerial(_sString);
}

void commandHelp() {
  printToSerial(getPgmString(STR_COMMAND_HELP));
}

void printToSerial(const int _int) {
  char _tmpBuffer[16] = { 0 };
  sprintf(_tmpBuffer, "%d", _int);
  printToSerial(_tmpBuffer);
}

void commandList() {
  char _atmpBuffer[64] = { 0 };
  printToSerial(getPgmString(STR_COMMAND_LIST_FORMAT_HEADER));
  printToSerial(getPgmString(STR_COMMAND_LIST_FORMAT_LINE));

  ILogic* _pIlogic = NULL;
  String _sFormatString = getPgmString(STR_COMMAND_LIST_FORMAT_STRING);
  byte _iIndex = 0;
  for (_iIndex = 0; _iIndex < NUMBER_OF_DEVICES; ++_iIndex) {
    String _sDeviceId;
    byte _param1;
    byte _param2;
    Configuration::PrivateData _privateData;

    _pIlogic = g_cDeviceContainer.getLogic(_iIndex);
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
    printToSerial(_atmpBuffer);
  }
  printToSerial('\n');
}

void commandSet() {
  char* _pToken = g_cSerialCommand.next();
  if (!_pToken) {
    printToSerial(getPgmString(STR_COMMAND_SET_USAGE));
    return;
  }

  String _sTmpString = _pToken;
  if (!isDigit(_sTmpString[0])) {
    printToSerial(getPgmString(STR_COMMAND_SET_USAGE));
    return;
  }

  byte _index = _sTmpString.toInt();
  _pToken = g_cSerialCommand.next();
  if (!_pToken) {
    printToSerial(getPgmString(STR_COMMAND_SET_USAGE));
    return;
  }

  _sTmpString = _pToken;
  Configuration::SupportedDevices _eDeviceId = Configuration::convert(_sTmpString);
  if (_eDeviceId == Configuration::E_DEVICE_UNKNOWN) {
    printToSerial(getPgmString(STR_COMMAND_SET_USAGE));
    return;
  }

  _pToken = g_cSerialCommand.next();
  if (!_pToken) {
    printToSerial(getPgmString(STR_COMMAND_SET_USAGE));
    return;
  }

  _sTmpString = _pToken;
  if (!isDigit(_sTmpString[0])) {
    printToSerial(getPgmString(STR_COMMAND_SET_USAGE));
    return;
  }

  byte _param1 = _sTmpString.toInt();
  _pToken = g_cSerialCommand.next();
  if (!_pToken) {
    printToSerial(getPgmString(STR_COMMAND_SET_USAGE));
    return;
  }

  _sTmpString = _pToken;
  if (!isDigit(_sTmpString[0])) {
    printToSerial(getPgmString(STR_COMMAND_SET_USAGE));
    return;
  }

  byte _param2 = _sTmpString.toInt();
  _pToken = g_cSerialCommand.next();

  if (_pToken) {
    printToSerial(getPgmString(STR_COMMAND_SET_USAGE));
    return;
  }

  Configuration::PrivateData _privateData;
  memset(&_privateData, 0, sizeof(_privateData));
  printToSerial(getPgmString(STR_COMMAND_SET));
  if (g_cDeviceContainer.setLogic(_index, _eDeviceId, _param1, _param2, _privateData)) {
    // Added a new logic, update the configuration
    ILogic* _pLogic = g_cDeviceContainer.getLogic(_index);
    if (_pLogic) {
      _pLogic->getParams(_eDeviceId, _param1, _param2, _privateData);
      g_cConfiguration.setDeviceParams(_index, _eDeviceId, _param1, _param2, _privateData);
      printToSerial(getPgmString(STR_SUCCESS));
    }
  } else printToSerial(getPgmString(STR_FAILED));
  printToSerial('\n');
}

void commandClear() {
  String _sIndex;
  char* _pToken = g_cSerialCommand.next();
  if (!_pToken) {
    printToSerial(getPgmString(STR_COMMAND_CLEAR_USAGE));
    return;
  }

  _sIndex = _pToken;
  _pToken = g_cSerialCommand.next();
  if (_pToken) {
    printToSerial(getPgmString(STR_COMMAND_CLEAR_TOO_MANY_ARGUMENTS));
    return;
  }

  Configuration::PrivateData _privateData;
  memset(&_privateData, 0, sizeof(_privateData));
  if (_sIndex = "all") {
    int _index;
    for (_index = 0; _index < NUMBER_OF_DEVICES; ++_index) {
      g_cDeviceContainer.clearLogic(_index);
      g_cConfiguration.setDeviceParams(_index, Configuration::E_DEVICE_UNKNOWN, 0, 0, _privateData);
    }
  } else if (isDigit(_sIndex[0])) {
    g_cDeviceContainer.clearLogic(_sIndex.toInt());
    g_cConfiguration.setDeviceParams(_sIndex.toInt(), Configuration::E_DEVICE_UNKNOWN, 0, 0, _privateData);
  } else printToSerial(getPgmString(STR_COMMAND_CLEAR_NOT_NUMBER_OR_ALL));
}

void commandSave() {
  char* _pToken = g_cSerialCommand.next();
  if (_pToken) {
    ILogic* _pILogic = NULL;
    int _index;
    for (_index = 0; _index < NUMBER_OF_DEVICES; ++_index) {
      ILogic* _pLogic = g_cDeviceContainer.getLogic(_index);
      if (_pLogic) {
        Configuration::SupportedDevices _deviceId;
        byte _param1;
        byte _param2;
        Configuration::PrivateData _privateData;
        _pLogic->getParams(_deviceId, _param1, _param2, _privateData);
        g_cConfiguration.setDeviceParams(_index, _deviceId, _param1, _param2, _privateData);
      }
    }
  }

  printToSerial(getPgmString(STR_COMMAND_SAVE));
  printToSerial(getPgmString(g_cConfiguration.storeConfiguration() ? STR_YES : STR_NO));
  printToSerial('\n');
}

void commandAction() {
  char* _pToken = g_cSerialCommand.next();
  if (!_pToken) {
    printToSerial(getPgmString(STR_COMMAND_ACTION_USAGE));
    return;
  }

  String _sTmpString = _pToken;
  byte _index = _sTmpString.toInt();
  _pToken = g_cSerialCommand.next();
  if (!_pToken) {
    printToSerial(getPgmString(STR_COMMAND_ACTION_USAGE));
    return;
  }

  _sTmpString = _pToken;
  byte _action = _sTmpString.toInt();
  _pToken = g_cSerialCommand.next();
  if (!_pToken) {
    printToSerial(getPgmString(STR_COMMAND_ACTION_USAGE));
    return;
  }

  _sTmpString = _pToken;
  byte _param = _sTmpString.toInt();

  // fetch the logic
  ILogic* _pLogic = g_cDeviceContainer.getLogic(_index);
  if (!_pLogic) {
    printToSerial(getPgmString(STR_COMMAND_ACTION_INVALID_LOGIC));
    return;
  }

  String _sResponse;
  switch (_pLogic->action(_action, _param, _sResponse)) {
    case ILogic::E_ACTION_RESPONSE:
      printToSerial(getPgmString(STR_ACTION));
      printToSerial((char)('0' + _index));
      printToSerial(getPgmString(STR_SEMICOLON_SPACE));
      _sResponse += '\n';
      printToSerial(_sResponse);
      break;

    case ILogic::E_ACTION_OK:
      break;

    default:
      break;
  }
}

void commandFreeMem() {
  void* _pMem = NULL;
  int _size = 1024 * 2;
  for (_size = 1024 * 2; _size > 0; --_size) {
    _pMem = malloc(_size);
    if (_pMem) {
      free(_pMem);
      printToSerial(getPgmString(STR_COMMAND_FREE));
      printToSerial(_size);
      printToSerial('\n');
      break;
    }
  }
}

void commandUnknown() {
  printToSerial(getPgmString(STR_COMMAND_UNKNOWN));
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  g_bluetoothSerial.begin(57600);

  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);

  String _sDump = getPgmString(STR_SETUP_DEBUG_RESTORING_CONFIGURATION);
  _sDump += g_cConfiguration.restoreConfiguration() ? getPgmString(STR_SUCCESS) : getPgmString(STR_FAILED);
  _sDump += getPgmString(STR_SETUP_DEBUG_DUMP_OF_CONFIGURATION);
  g_cConfiguration.dumpConfiguration(_sDump);
  _sDump += getPgmString(STR_SETUP_DEBUG_DUMP_OF_EEPROM);
  dumpEepromMemory(0, 56, _sDump);

  _sDump += getPgmString(STR_SETUP_DEBUG_STORING);
  _sDump += g_cConfiguration.storeConfiguration() ? getPgmString(STR_YES) : getPgmString(STR_NO);

  byte _index;
  for (_index = 0; _index < g_cConfiguration.maxNumberOfDevices(); ++_index) {
    Configuration::SupportedDevices _deviceId;
    byte _param1, _param2;
    Configuration::PrivateData _privateData;
    if ( g_cConfiguration.getDeviceParams(_index, _deviceId, _param1, _param2, _privateData) )
      g_cDeviceContainer.setLogic(_index, _deviceId, _param1, _param2, _privateData);
  }

  _sDump += "\n\nREADY\n";
  printToSerial(_sDump);

  g_cSerialCommand.setTerm('\n');
  g_cSerialCommand.addCommand("?", commandHelp);
  g_cSerialCommand.addCommand(getPgmString(STR_CMD_LIST).c_str(), commandList);
  g_cSerialCommand.addCommand(getPgmString(STR_CMD_SET).c_str(), commandSet);
  g_cSerialCommand.addCommand(getPgmString(STR_CMD_CLEAR).c_str(), commandClear);
  g_cSerialCommand.addCommand(getPgmString(STR_CMD_SAVE).c_str(), commandSave);

  g_cSerialCommand.addCommand(getPgmString(STR_CMD_ACTION).c_str(), commandAction);

  g_cSerialCommand.addCommand(getPgmString(STR_CMD_FREE).c_str(), commandFreeMem);
  g_cSerialCommand.addDefaultHandler(commandUnknown);
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
          printToSerial(getPgmString(STR_EVENT));
          printToSerial((char)('0' + _index));
          printToSerial(getPgmString(STR_SEMICOLON_SPACE));

          _sResponse += '\n';
          printToSerial(_sResponse);
          break;
      }
    }
  }

  g_cSerialCommand.readSerial();
}

