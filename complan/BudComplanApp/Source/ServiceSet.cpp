//! @file
//! @brief
#include "precomp.h"

#include <JsonCpp/json.h>
#include <XML/amgXmlElement.h>
#include <XML/amgXmlText.h>
#include <Debug/amgDebug.h>
#include <amgErrors.h>
#include <ckiBudService.h>
#include <ckiSerializers.h>
#include "ServiceSet.h"

using namespace NAmg;

void TServiceSetData::ToXml(const TServiceSetData& _value, NAmg::NXml::TElement& _xml)
{
   _xml.AddAttr("label", _value.Label);
   _xml.AddAttr("scheme-id", _value.SchemeId);
   auto& descriptionElement = *(new NAmg::NXml::TElement("Description"));
   descriptionElement.AddChild(*(new NAmg::NXml::TText(_value.Description)));
   _xml.AddChild(descriptionElement);

   for (auto& item : _value.ServiceIds)
   {
      auto& element = *(new NAmg::NXml::TElement("Service"));
      element.AddAttr("id", item);
      _xml.AddChild(element);
   }
}

void TServiceSetData::FromXml(const NAmg::NXml::TElement& _xml, TServiceSetData& _value)
{
   _value = {};

   auto* attr = _xml.FindAttr("label");
   if (nullptr != attr)
   {
      _value.Label = attr->AsTString();
   }

   attr = _xml.FindAttr("scheme-id");
   if (nullptr != attr)
   {
      _value.SchemeId = attr->AsTString();
   }

   if (1 == _xml.ElemCount("Description"))
   {
      _value.Description = _xml.ElChild(0, "Description").GetText();
   }

   auto& elServices = _xml.ElChilds("Service");
   for (TVectorIndex i = 0; i < elServices.Count(); ++i)
   {
      auto* attr = elServices[i].FindAttr("id");
      if (nullptr != attr)
      {
         _value.ServiceIds.push_back(attr->AsTString());
      }
   }
}

void TServiceSetData::ToJson(const TServiceSetData& _value, Json::Value& _json)
{
   _json["label"] = _value.Label;
   _json["description"] = _value.Description;
   _json["scheme_id"] = _value.SchemeId;
   if (!_value.ServiceIds.empty())
   {
      auto& element = _json["services"];
      for (auto& item : _value.ServiceIds)
      {
         element[item] = item;
      }
   }
}

void TServiceSetData::FromJson(const Json::Value& _json, TServiceSetData& _value)
{
   _value = {};   
   NAmg::NCakApi::JsonDeserializer(_json)
      .Extract("label", _value.Label)
      .Extract("description", _value.Description)
      .Extract("scheme_id", _value.SchemeId)
      .Handle("services", [&_value](const Json::Value& _services)
   {
      auto serviceMembers = _services.getMemberNames();
      for (auto& id : serviceMembers)
      {
         _value.ServiceIds.push_back(id);
      }
   });
}

void TServiceSetState::ToJson(const TServiceSetState& _value, Json::Value& _json)
{
   _json["current"] = _value.CurrentSetId;
   _json["awaited"] = _value.AwaitedSetId;
}

void TServiceSetState::FromJson(const Json::Value& _json, TServiceSetState& _value)
{
   //_value = {};   
   NAmg::NCakApi::JsonDeserializer(_json)
      .Extract("current", _value.CurrentSetId)
      .Extract("awaited", _value.AwaitedSetId);
}

void AmgCall TServiceSetState::AwaitedToCache(NAmg::NCakApi::TBud& _bud, const TAdapter& _current, const std::string& _toSetId)
{
   NAmg::NScl::NHttp::TQueueUpdate update;
   update.Method = "PUT";
   update.Path = _current.Path + "awaited";
   update.Body = &NAmg::NScl::NText::toBody(Json::Value(_toSetId));
   _bud.Update(update);
}

void AmgCall TServiceSetState::CurrentToCache(NAmg::NCakApi::TBud& _bud, const TAdapter& _current, const std::string& _toSetId)
{
   TServiceSetState state = _current.Section;
   state.AwaitedSetId = _toSetId;
   state.CurrentSetId = _toSetId;
   NAmg::NScl::NHttp::TQueueUpdate update;
   _current.Prepare(state, update);
   _bud.Update(update);
}
