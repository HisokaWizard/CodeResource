//! @file
//! @brief Реализация класса NAmg::TAlarmSource.
#include "precomp.h"
//
#include "EventSource.h"
//
#ifdef _MSC_VER
   #pragma pack(16)
#endif

//
namespace NAmg
{
   DAmgSourceData(Cacos);
   DAmgSourceData(Agent);
   DAmgSourceData(User);

   namespace 
   {
      class TErrorsInitialization
      {
      public:
         AmgCreate TErrorsInitialization(void);
         AmgDestroy ~TErrorsInitialization(void);
      };

      AmgCreate TErrorsInitialization::TErrorsInitialization(void)
      {
         errs_Cacos();
         errs_Agent();
         errs_User();
      }

      AmgDestroy TErrorsInitialization::~TErrorsInitialization(void)
      {
         TerminateProcess();
      }

      static TErrorsInitialization staticErrorsInitialization;      
   }
} // namespace NAmg
