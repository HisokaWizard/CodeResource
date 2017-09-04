#ifndef __BudComplan_Endpoint_H__
#define __BudComplan_Endpoint_H__
//! @file
//! @brief

#include <amgDefs.h>
#include <ckiCache.h>

#ifdef _MSC_VER
   #pragma pack(push,16)
#endif

namespace NAmg { 
   namespace NXml { class TElement; }
   namespace NCakApi { class TBud; }
}
namespace Json { class Value; }

//! @brief
//!   Пользователь сервиса
//! @details
//!
struct TEndpointData
{
   bool AutoConnect = false;
   bool CanBuild = false;

   static void ToXml(const TEndpointData&, NAmg::NXml::TElement&);
   static void FromXml(const NAmg::NXml::TElement&, TEndpointData&);
   static void ToJson(const TEndpointData&, Json::Value&);
   static void FromJson(const Json::Value&, TEndpointData&);
};

//! @brief
//!   
//! @details
//!
struct TEndpointState
{
   enum TValue
   {
      Detached,
      Attaching,
      Attached,
      Rejected,
      Detaching
   };
   
   //aggregate initialization doesn't work with default non-static member initializers until C++14
   TValue Value;// = Detached;
   bool IsConnected;// = false;

   static std::string AmgCall ValueToStr(TValue);
   static TValue AmgCall StrToValue(const std::string&);

   static void FromJson(const Json::Value&, TEndpointState&);
   static void ToJson(const TEndpointState&, Json::Value&);

   static void AmgCall RequestToUpdate(NAmg::NCakApi::TBud&, const NAmg::TPath&, const std::string& _serviceId, const std::string& _endpointId, bool _isConnected);
   static void AmgCall RequestToAttach(NAmg::NCakApi::TBud&, const NAmg::TPath&, const std::string& _serviceId, const std::string& _endpointId);
   static void AmgCall RequestToDettach(NAmg::NCakApi::TBud&, const NAmg::TPath&, const std::string& _serviceId, const std::string& _endpointId);
   static void AmgCall ToCache(NAmg::NCakApi::TBud&, const NAmg::TPath&, const std::string& _serviceId, const std::string& _endpointId, const TEndpointState& _state);
   static void AmgCall ToCache(NAmg::NCakApi::TBud&, const NAmg::TPath&, const std::string& _serviceId, const std::string& _endpointId, const TEndpointState::TValue&);
   static void AmgCall ToCache(NAmg::NCakApi::TBud&, const NAmg::TPath&, const std::string& _serviceId, const std::string& _endpointId, bool _isConnected);
};

using TEndpointsSubscribedOn = std::set<std::string>;

namespace NAmg
{
namespace NScl 
{
namespace NText
{
template<> TBody::TREF AmgCall toBody<TEndpointState>(const TEndpointState& _inValue);
template<> void AmgCall fromBody<TEndpointState>(const TBody& _body, TEndpointState& _outValue);
} 
}
}

#ifdef _MSC_VER
   #pragma pack(pop)
#endif
//
#endif
