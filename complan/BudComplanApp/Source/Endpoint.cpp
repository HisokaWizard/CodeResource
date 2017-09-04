//! @file
//! @brief
#include "precomp.h"

#include <JsonCpp/json.h>
#include <XML/amgXmlElement.h>
#include <Debug/amgDebug.h>
#include <amgErrors.h>
#include <ckiBudService.h>
#include "Endpoint.h"

using namespace NAmg;

namespace
{
const std::map<std::string, TEndpointState::TValue>& AmgCall EndpointStateValues(void)
{
   static std::map<std::string, TEndpointState::TValue> value = 
   {
      {"attaching", TEndpointState::Attaching},
      {"attached",  TEndpointState::Attached},
      {"rejected",  TEndpointState::Rejected},
      {"detaching", TEndpointState::Detaching},
      {"detached",  TEndpointState::Detached}
   };
   return(value);
}
}

void TEndpointData::ToXml(const TEndpointData& _value, NAmg::NXml::TElement& _xml)
{
   _xml.AddAttr("auto-connect", TAString::BoolToStr(_value.AutoConnect));
   _xml.AddAttr("can-build", TAString::BoolToStr(_value.CanBuild));
}

void TEndpointData::FromXml(const NAmg::NXml::TElement& _xml, TEndpointData& _value)
{
   _value = {};

   auto* attr = _xml.FindAttr("auto-connect");
   if (nullptr != attr)
      _value.AutoConnect = attr->Value().ToBool();

   attr = _xml.FindAttr("can-build");
   if (nullptr != attr)
      _value.CanBuild = attr->Value().ToBool();
}

void TEndpointData::ToJson(const TEndpointData& _value, Json::Value& _json)
{
   _json["auto-connect"] = _value.AutoConnect;
   _json["can-build"] = _value.CanBuild;
}

void TEndpointData::FromJson(const Json::Value& _json, TEndpointData& _value)
{
   _value = {};

   auto members = _json.getMemberNames();

   if (members.end() != std::find(members.begin(), members.end(), "auto-connect"))
      _value.AutoConnect = _json["auto-connect"].asBool();

   if (members.end() != std::find(members.begin(), members.end(), "can-build"))
      _value.CanBuild = _json["can-build"].asBool();
}

std::string AmgCall TEndpointState::ValueToStr(TEndpointState::TValue _value)
{
   for (const auto& item : EndpointStateValues())
   {
      if (item.second == _value)
      {
         return(item.first);
      }
   }
   return(EndpointStateValues().begin()->first);
}

TEndpointState::TValue AmgCall TEndpointState::StrToValue(const std::string& _str)
{
   const auto& values = EndpointStateValues();
   auto it = values.find(_str);
   if (values.end() != it)
   {
      return(it->second);
   }
   return(EndpointStateValues().begin()->second);
}

void TEndpointState::ToJson(const TEndpointState& _value, Json::Value& _json)
{
   _json["state"] = TEndpointState::ValueToStr(_value.Value);
   _json["connected"] = _value.IsConnected;
}

void TEndpointState::FromJson(const Json::Value& _json, TEndpointState& _value)
{
   //_value = { Detached, false };
   auto members = _json.getMemberNames();
   if (members.end() != std::find(members.begin(), members.end(), "state"))
   {
      _value.Value = TEndpointState::StrToValue(_json["state"].asString());
   }
   if (members.end() != std::find(members.begin(), members.end(), "connected"))
   {
      _value.IsConnected = _json["connected"].asBool();
   }
}

void AmgCall TEndpointState::RequestToUpdate(NAmg::NCakApi::TBud& _bud, const NAmg::TPath& _root, const std::string& _serviceId, const std::string& _endpointId, bool _isConnected)
{
   NAmg::NScl::NHttp::TQueueRequest request;
   request.Method = "PUT";
   request.Path = _root;
   request.Path += _serviceId;
   request.Path += "endpoints";

   Json::Value json;
   json["connected"] = _isConnected;
   json["source"] = _endpointId;
   request.Body = &NAmg::NScl::NText::toBody(json);

   _bud.Request(request,
      [](const NAmg::NScl::NHttp::TQueueResponse&)
   {
      //onResponse
   },
      []()
   {
      //onError
   });
}

namespace
{
   void AmgCall RequestTo(NAmg::NCakApi::TBud& _bud, const NAmg::TPath& _root, const std::string& _serviceId, const std::string& _endpointId, bool _doAttach)
   {
      NAmg::NScl::NHttp::TQueueRequest request;
      request.Method = _doAttach ? "POST" : "DELETE";
      request.Path = _root;
      request.Path += _serviceId;
      request.Path += "endpoints";
      request.Path += _endpointId;

      _bud.Request(request,
         [](const NAmg::NScl::NHttp::TQueueResponse&)
      {
         //onResponse
      },
         []()
      {
         //onError
      });
   }
}

void AmgCall TEndpointState::RequestToAttach(NAmg::NCakApi::TBud& _bud, const NAmg::TPath& _root, const std::string& _serviceId, const std::string& _endpointId)
{
   RequestTo(_bud, _root, _serviceId, _endpointId, true);
}

void AmgCall TEndpointState::RequestToDettach(NAmg::NCakApi::TBud& _bud, const NAmg::TPath& _root, const std::string& _serviceId, const std::string& _endpointId)
{
   RequestTo(_bud, _root, _serviceId, _endpointId, false);
}

namespace
{
   template <class T>
   void AmgCall DoUpdate(NAmg::NCakApi::TBud& _bud, const TPath& _root, const std::string& _serviceId, const std::string& _endpointId, const T& _newValue)
   {
      NAmg::NScl::NHttp::TQueueUpdate update;
      update.Method = "PUT";
      update.Path = _root;
      update.Path += _serviceId;
      update.Path += "endpoints";
      update.Path += _endpointId;

      Json::Value json;
      TEndpointState::ToJson(_newValue, json);
      update.Body = &NAmg::NScl::NText::toBody(json);

      _bud.Update(update);
   }
}

void AmgCall TEndpointState::ToCache(NAmg::NCakApi::TBud& _bud, const TPath& _root, const std::string& _serviceId, const std::string& _endpointId, const TEndpointState& _state)
{
   DoUpdate(_bud, _root, _serviceId, _endpointId, _state);
}

void AmgCall TEndpointState::ToCache(NAmg::NCakApi::TBud& _bud, const TPath& _root, const std::string& _serviceId, const std::string& _endpointId, const TEndpointState::TValue& _value)
{
   TEndpointState state { _value, false };
   DoUpdate(_bud, _root, _serviceId, _endpointId, state);
}

void AmgCall TEndpointState::ToCache(NAmg::NCakApi::TBud& _bud, const TPath& _root, const std::string& _serviceId, const std::string& _endpointId, bool _isConnected)
{
   NAmg::NScl::NHttp::TQueueUpdate update;
   update.Method = "PUT";
   update.Path = _root;
   update.Path += _serviceId;
   update.Path += "endpoints";
   update.Path += _endpointId;
   update.Path += "connected";

   update.Body = &NAmg::NScl::NText::toBody(Json::Value(_isConnected));
   _bud.Update(update);
}

namespace NAmg
{
namespace NScl 
{
namespace NText
{

template<> TBody::TREF AmgCall toBody<TEndpointState>(const TEndpointState& _inValue)
{
   Json::Value json;
   TEndpointState::ToJson(_inValue, json);
   return(toBody(json));
}

template<> void AmgCall fromBody<TEndpointState>(const TBody& _body, TEndpointState& _outValue)
{
   Json::Value json;
   fromBody(_body, json);
   TEndpointState::FromJson(json, _outValue);
}

} 
}
}
