#ifndef Cacos_AlarmMsg_H
#define  Cacos_AlarmMsg_H

#ifdef _MSC_VER
#pragma pack(push,16)
#endif

#include <Restconf/sclEventLogMsg.h>
#include "amgDefs.h"


namespace NAmg
{
   namespace NCakApi { struct TSpecificPreset; }

   enum class TResultCode
   {
      Accepted = 0x08,
      Rejected = 0x09,
      FailedToSend = 0x0A
   };

   //! @brief Сообщение об успешном применении предустановки или изменении конфигурации.
   class TrequestAcceptedMsg : public NScl::NYang::TEventLogMsg::TClass
   {
   public:
      DAmgError(TrequestAcceptedMsg,  NScl::NYang::TEventLogMsg);
      static TError AmgCall Create(const NCakApi::TSpecificPreset& _preset);
      static TError AmgCall Create(const TAString& _hostId, const TPath& _node);

   protected:
      AmgCreate TrequestAcceptedMsg(const NScl::NYang::TeventLogMsg::TBuilder& _build);
   };
   //! @brief Определяет интерфейс для работы с типами сообщений.
   typedef TrequestAcceptedMsg::TError TRequestAcceptedMsg;

   //! @brief Сообщение об отклонении предустановки агентом.
   class TrequestRejectedMsg : public NScl::NYang::TEventLogMsg::TClass
   {
   public:
      DAmgError(TrequestRejectedMsg,  NScl::NYang::TEventLogMsg);
      static TError AmgCall Create(const NCakApi::TSpecificPreset& _preset,
                                   const TAString& _agentResponce);
      static TError AmgCall Create(const TAString& _hostId, const TPath& _node,
                                   const TAString& _agentResponce);

   protected:
      AmgCreate TrequestRejectedMsg(const NScl::NYang::TeventLogMsg::TBuilder& _build);
   };
   //! @brief Определяет интерфейс для работы с типами сообщений.
   typedef TrequestRejectedMsg::TError TRequestRejectedMsg;

   //! @brief Сообщение об ошибке соединения при попытке применить предустановку.
   class TrequestFailedToSendMsg : public NScl::NYang::TEventLogMsg::TClass
   {
   public:
      DAmgError(TrequestFailedToSendMsg,  NScl::NYang::TEventLogMsg);
      static TError AmgCall Create(const NCakApi::TSpecificPreset& _preset,
                                   const TAString& _errorMsg);
      static TError AmgCall Create(const TAString& _hostId, const TPath& _node,
                                   const TAString& _errorMsg);

   protected:
      AmgCreate TrequestFailedToSendMsg(const NScl::NYang::TeventLogMsg::TBuilder& _build);
   };
   //! @brief Определяет интерфейс для работы с типами сообщений.
   typedef TrequestFailedToSendMsg::TError TRequestFailedToSendMsg;
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif

#endif //Cacos_AlarmMsg_H
