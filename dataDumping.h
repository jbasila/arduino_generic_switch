
#ifndef _DATA_DUMPING_H
#define _DATA_DUMPING_H

void dumpEepromMemory(unsigned int _uiStartAddress, unsigned int _uiLength, String& _sDump);
void dumpMemory(void* _pStartAddress, unsigned int _uiLength, String& _sDump);

#endif//_DATA_DUMPING_H

