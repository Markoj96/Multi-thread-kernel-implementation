#include "IVTE.h"

#define PREPAREENTRY(numEntry, callOld)\
void interrupt inter##numEntry(...); \
IVTEntry newEntry##numEntry(numEntry, inter##numEntry);\
void interrupt inter##numEntry(...) {\
	newEntry##numEntry.signal();\
	if (callOld == 1)\
		newEntry##numEntry.callOldRoutine();\
}