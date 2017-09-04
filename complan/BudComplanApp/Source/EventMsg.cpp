//! @file
//! @brief Реализация программных классов отвечающих за типы сообщений.
#include "precomp.h"
//
#include "EventMsg.h"
#include "EventSource.h"
#include <ckiPresetType.h>
#include <Link/THttpUri.h>
#include <TStringRes.h>
//
#ifdef _MSC_VER
#pragma pack(16)
#endif
//
namespace NAmg
{
   TRequestAcceptedMsg AmgCall TrequestAcceptedMsg::Create(const NCakApi::TSpecificPreset& _preset)
   {
      NScl::NYang::TeventLogMsg::TBuilder builder(errs_Cacos);
      TAString descr = StringRes.Short("CacosPresetApplied");
      descr.Replace("{{preset-id}}", _preset.Id)
           .Replace("{{preset-type}}", _preset.Type);
      builder.description = descr;
      if(_preset.Parameters.end() != _preset.Parameters.find("host-id"))
      {
         builder.hostId = _preset.Parameters.at("host-id");
      }
      return *new TrequestAcceptedMsg(builder);
   }

   TRequestAcceptedMsg AmgCall TrequestAcceptedMsg::Create(const TAString& _hostId, const TPath& _node)
   {
      NScl::NYang::TeventLogMsg::TBuilder builder(errs_Cacos);
      builder.hostId = _hostId;
      builder.description = _node;

      return *new TrequestAcceptedMsg(builder);
   }

   AmgCreate TrequestAcceptedMsg::TrequestAcceptedMsg(const NScl::NYang::TeventLogMsg::TBuilder& _build)
      : NScl::NYang::TEventLogMsg::TClass(_build)
   {
      FCode = static_cast<TErrorCode>(TResultCode::Accepted);
   }

   TRequestRejectedMsg AmgCall TrequestRejectedMsg::Create(const NCakApi::TSpecificPreset& _preset,
                                                         const TAString& _agentResponce)
   {
      NScl::NYang::TeventLogMsg::TBuilder builder(errs_Cacos);
      TAString descr = StringRes.Short("CacosPresetRejected");
      descr.Replace("{{preset-id}}", _preset.Id)
           .Replace("{{preset-type}}", _preset.Type)
           .Replace("{{agent-responce}}", _agentResponce);
      builder.description = descr;
      if(_preset.Parameters.end() != _preset.Parameters.find("host-id"))
      {
         builder.hostId = _preset.Parameters.at("host-id");
      }
      return *new TrequestRejectedMsg(builder);
   }

   TRequestRejectedMsg AmgCall TrequestRejectedMsg::Create(const TAString& _hostId, const TPath& _node,
      const TAString& _agentResponce)
   {
      NScl::NYang::TeventLogMsg::TBuilder builder(errs_Cacos);
      builder.hostId = _hostId;
      builder.description = _agentResponce + BlankString() + _node;

      return *new TrequestRejectedMsg(builder);
   }

   AmgCreate TrequestRejectedMsg::TrequestRejectedMsg(const NScl::NYang::TeventLogMsg::TBuilder& _build)
      : NScl::NYang::TEventLogMsg::TClass(_build) 
   {
      FCode = static_cast<TErrorCode>(TResultCode::Rejected);
   }

   TRequestFailedToSendMsg AmgCall TrequestFailedToSendMsg::Create(const NCakApi::TSpecificPreset& _preset,
                                                                 const TAString& _errorMsg)
   {
      NScl::NYang::TeventLogMsg::TBuilder builder(errs_Cacos);
      TAString descr = StringRes.Short("CacosPresetFailedToSend");
      descr.Replace("{{preset-id}}", _preset.Id)
           .Replace("{{preset-type}}", _preset.Type)
           .Replace("{{error-msg}}", _errorMsg);
      builder.description = descr;
      if(_preset.Parameters.end() != _preset.Parameters.find("host-id"))
      {
         builder.hostId = _preset.Parameters.at("host-id");
      }
      return *new TrequestFailedToSendMsg(builder);
   }

   TRequestFailedToSendMsg AmgCall TrequestFailedToSendMsg::Create(const TAString& _hostId, const TPath& _node,
      const TAString& _errorMsg)
   {
      NScl::NYang::TeventLogMsg::TBuilder builder(errs_Cacos);
      builder.hostId = _hostId;
      builder.description = _errorMsg + BlankString() + _node;

      return *new TrequestFailedToSendMsg(builder);
   }

   AmgCreate TrequestFailedToSendMsg::TrequestFailedToSendMsg(const NScl::NYang::TeventLogMsg::TBuilder& _build)
      : NScl::NYang::TEventLogMsg::TClass(_build)
   {
      FCode = static_cast<TErrorCode>(TResultCode::FailedToSend);
   }
}
