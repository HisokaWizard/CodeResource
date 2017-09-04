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
#include "Types.h"

using namespace NAmg;

namespace
{

std::map<std::string, TRole>& AmgCall Roles()
{
   static std::map<std::string, TRole> roles = 
      {
         {"endpoint", TRole::Endpoint},
         {"encryptor", TRole::Encryptor},
         {"antenna", TRole::Antenna},
         {"radio", TRole::Radio},
      };
   return roles;
}

std::string AmgCall RoleToString(TRole _role)
{
   for (auto& item : Roles())
   {
      if (item.second == _role)
         return(item.first);
   }
   return {};
}
//
relevantObject<TRole> AmgCall RoleFromString(const std::string& _str)
{
   auto it = Roles().find(_str);
   if (Roles().end() == it)
      return {};
   return(it->second);
}

}

void TExtraDeviceType::ToJson(const TExtraDeviceType& _value, Json::Value& _json)
{
   _json["label"] = _value.Label;
   _json["description"] = _value.Description;
   _json["role"] = RoleToString(_value.Role);
   _json["can-send"] = _value.CanSend;
   _json["can-recieve"] = _value.CanReceive;
   _json["component-id"] = _value.ComponentId;

   if (!_value.RangeIds.empty())
   {
      auto& element = _json["ranges"];
      for (auto& item : _value.RangeIds)
      {
         element[item] = item;
      }
   }
   if (!_value.MediaTypes.empty())
   {
      auto& element = _json["media-types-id"];
      for (auto& item : _value.MediaTypes)
      {
         element[item] = item;
      }
   }
}

void TExtraDeviceType::FromJson(const Json::Value& _json, TExtraDeviceType& _value)
{
   _value = {};
   if (_json.isMember("label"))
      _value.Label = _json["label"].asString();
   if (_json.isMember("description"))
      _value.Description = _json["description"].asString();
   if (_json.isMember("role"))
   {
       auto roleREL = RoleFromString(_json["role"].asString());
       if (!roleREL.IsNULL())
         _value.Role = roleREL();
   }
   if (_json.isMember("can-send"))
      _value.CanSend = _json["can-send"].asBool();
   if (_json.isMember("can-recieve"))
      _value.CanReceive = _json["can-recieve"].asBool();
   if (_json.isMember("component-id"))
      _value.ComponentId = _json["component-id"].asString();

   if (_json.isMember("ranges"))
   {
      auto& element = _json["ranges"];
      auto items = element.getMemberNames();
      for (auto& id : items)
      {
         _value.RangeIds.push_back(id);
      }
   }
   if (_json.isMember("media-types"))
   {
      auto& element = _json["media-types"];
      auto items = element.getMemberNames();
      for (auto& id : items)
      {
         _value.MediaTypes.push_back(id);
      }
   }
}

void TExtraDeviceType::ToXml(const TExtraDeviceType& _value, NXml::TElement& _xml)
{
   _xml.AddAttr("label", _value.Label);

   std::unique_ptr<NXml::TElement> descriptionElement(new NXml::TElement("Description"));
   descriptionElement->AddChild(*(new NXml::TText(_value.Description)));
   _xml.AddChild(*descriptionElement.release());

   _xml.AddAttr("role", RoleToString(_value.Role));
   _xml.AddAttr("can-send", TAString::BoolToStr(_value.CanSend));
   _xml.AddAttr("can-recieve", TAString::BoolToStr(_value.CanReceive));
   _xml.AddAttr("component-id", _value.ComponentId);

   if (!_value.RangeIds.empty())
   {
      std::unique_ptr<NXml::TElement> rangesElement(new NXml::TElement("Ranges"));
      for (auto& item : _value.RangeIds)
      {
         std::unique_ptr<NXml::TElement> rangeElement(new NXml::TElement("Range"));
         rangeElement->AddAttr("id", item);
         rangesElement->AddChild(*rangeElement.release());
      }
      _xml.AddChild(*rangesElement.release());
   }
   if (!_value.MediaTypes.empty())
   {
      std::unique_ptr<NXml::TElement> mediaTypesElement(new NXml::TElement("MediaTypes"));
      for (auto& item : _value.MediaTypes)
      {
         std::unique_ptr<NXml::TElement> mediaTypeElement(new NXml::TElement("MediaType"));
         mediaTypeElement->AddAttr("id", item);
         mediaTypesElement->AddChild(*mediaTypeElement.release());
      }
      _xml.AddChild(*mediaTypesElement.release());
   }
}

void TExtraDeviceType::FromXml(const NAmg::NXml::TElement& _xml, TExtraDeviceType& _value)
{
   _value = {};

   const NXml::TAttribute* attr;
   if (nullptr != (attr = _xml.FindAttr("label")))
      _value.Label = attr->AsTString();

   if (1 == _xml.ElemCount("Description"))
   {
      _value.Description = _xml.ElChild(0, "Description").GetText();
   }

   if (nullptr != (attr = _xml.FindAttr("role")))
   {
      auto roleREL = RoleFromString(attr->AsTString());
      if (!roleREL.IsNULL())
         _value.Role = roleREL();
   }
   if (nullptr != (attr = _xml.FindAttr("can-send")))
      _value.CanSend = attr->AsTString().ToBool();
   if (nullptr != (attr = _xml.FindAttr("can-recieve")))
      _value.CanReceive = attr->AsTString().ToBool();
   if (nullptr != (attr = _xml.FindAttr("component-id")))
      _value.ComponentId = attr->AsTString();

   if (1 == _xml.ElemCount("Ranges"))
   {
      auto& elements = _xml.ElChild(0, "Ranges").ElChilds("Range");
      for (TVectorIndex i = 0; i < elements.Count(); ++i)
      {
         if (nullptr != (attr = elements[i].FindAttr("id")))
            _value.RangeIds.push_back(attr->AsTString());
      }
   }
   if (1 == _xml.ElemCount("MediaTypes"))
   {
      auto& elements = _xml.ElChild(0,"MediaTypes").ElChilds("MediaType");
      for (TVectorIndex i = 0; i < elements.Count(); ++i)
      {
         attr = elements[i].FindAttr("id");
         if (nullptr != attr)
            _value.MediaTypes.push_back(attr->AsTString());
         _value.MediaTypes.push_back(elements[i].GetText());
      }
   }
}
