
#include <avr/pgmspace.h>
#include <Arduino.h>

#include "pgmString.h"

#ifdef PGM_STRING
#undef PGM_STRING
#endif

#define PGM_STRING(_stringId, _stringText) const char pgmString_##_stringId[] PROGMEM = _stringText;
#include "pgmStringStash.h"

#ifdef PGM_STRING
#undef PGM_STRING
#endif

#define PGM_STRING(_stringId, _stringText) pgmString_##_stringId,
const char* const pgmStringTable[] PROGMEM = {
#include "pgmStringStash.h"
NULL };

#ifdef PGM_STRING
#undef PGM_STRING
#endif

String getPgmString(STR_PGM _stringId) {
  String _sReturnValue = "";
  char* _pgmBuffer = (char*)pgm_read_word(&(pgmStringTable[_stringId]));
  char _char;
  int _index = 0;
  while (_char = pgm_read_byte_near(_pgmBuffer + _index++)) {
    _sReturnValue += _char;
  }

  return _sReturnValue;
}

