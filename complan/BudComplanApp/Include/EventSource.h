#ifndef  Cacos_AlarmSource_H
#define Cacos_AlarmSource_H
//! @file
//! @brief ќбъ€вление класса NAmg::TAlarmSource.
#include "amgDefs.h"
#include "amgSources.h"

#ifdef __BORLANDC__
   #pragma option push -w -O2 -vi- -b -6 -k -pc -ff
#endif

#pragma pack(push,16)

namespace NAmg
{
   DAmgSource(Cacos);
   DAmgSource(Agent);
   DAmgSource(User);
}// namespace NAmg

#ifdef _MSC_VER
   #pragma pack(pop)
#endif

#endif // Cacos_AlarmSource_H
