
void dumpEepromMemory(unsigned int _uiStartAddress, unsigned int _uiLength, String& _sDump);
void dumpMemory(void* _pStartAddress, unsigned int _uiLength, String& _sDump);

void dumpConfigurationAndEeprom(Stream& _stream,
                                Configuration& _configuration,
                                bool _restoreConfiguration,
                                bool _storeConfiguration);

