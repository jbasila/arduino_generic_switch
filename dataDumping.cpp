
#include <Arduino.h>
#include <EEPROM.h>

void dumpEepromMemory(unsigned int _uiStartAddress, unsigned int _uiLength, String& _sDump) {
  for (unsigned int _uiAddress = _uiStartAddress; _uiAddress < (_uiStartAddress + _uiLength); ++_uiAddress) {
    if (! ((_uiAddress) % 8) ) _sDump += "\n";
    byte _byte = EEPROM.read(_uiAddress);
    char _tmpBuffer[16];
    sprintf(_tmpBuffer, "%02X ", _byte);
    _sDump += _tmpBuffer;
  }
}

void dumpMemory(void* _pStartAddress, unsigned int _uiLength, String& _sDump) {
  for (byte* _pAddress = (byte*)_pStartAddress; _pAddress < ((byte*)_pStartAddress + _uiLength); ++_pAddress) {
    if (! (((unsigned int)_pAddress - (unsigned int)_pStartAddress) % 8) ) _sDump += "\n";
    char _tmpBuffer[16];
    byte _byte = *_pAddress;
    sprintf(_tmpBuffer, "%02X ", _byte);
    _sDump += _tmpBuffer;
  }
}


