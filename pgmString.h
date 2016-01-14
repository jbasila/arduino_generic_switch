
#ifndef _PGM_STRING_H
#define _PGM_STRING_H

#ifdef PGM_STRING
#undef PGM_STRING
#endif

#define PGM_STRING(_stringId, _stringTest) _stringId,

enum STR_PGM {
  #include "pgmStringStash.h"
  STR_SIZE
};

#ifdef PGM_STRING
#undef PGM_STRING
#endif

String getPgmString(STR_PGM _stringId);

#endif/*_PGM_STRING_H*/

