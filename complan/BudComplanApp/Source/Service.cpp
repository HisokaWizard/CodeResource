//! @file
//! @brief
#include "precomp.h"

#include <JsonCpp/json.h>
#include <XML/amgXmlElement.h>
#include <Debug/amgDebug.h>
#include <amgErrors.h>
#include <ckiBudService.h>
#include <ckiSerializers.h>
#include "Service.h"

using namespace NAmg;

void TEquipmentType::ToJson(const TEquipmentType& _value, Json::Value& _json)
{
   _json["device-type-id"] = _value.DeviceTypeId;
   _json["role-id"] = _value.RoleId;
   _json["can-transmit"] = _value.CanTransmit;
   _json["can-receive"] = _value.CanReceive;
   if (!_value.FrequencyRanges.empty())
   {
      auto& ranges(_json["ranges"]);
      for (auto& id : _value.FrequencyRanges)
      {
         ranges.append(id);
      }
   }
}

void TEquipmentType::FromJson(const Json::Value& _json, TEquipmentType& _value)
{
   _value = {};
   _value.DeviceTypeId = _json["device-type-id"].asString();
   _value.RoleId = _json["role-id"].asString();
   _value.CanTransmit = _json["can-transmit"].asBool();
   _value.CanReceive = _json["can-receive"].asBool();
   if (_json.isMember("ranges") && _json["ranges"].isArray())
   {
      for (auto& id : _json["ranges"])
      {
         _value.FrequencyRanges.push_back(id.asString());
      }
   }
}

void TEquipmentType::ToXml(const TEquipmentType& _value, NAmg::NXml::TElement& _xml)
{
   _xml.AddAttr("device-type-id", _value.DeviceTypeId);
   _xml.AddAttr("role-id", _value.RoleId);
   _xml.AddAttr("can-transmit", TAString::BoolToStr(_value.CanTransmit));
   _xml.AddAttr("can-receive", TAString::BoolToStr(_value.CanReceive));
   if (!_value.FrequencyRanges.empty())
   {
      auto ranges = std::unique_ptr<NXml::TElement>(new NXml::TElement(NAmg::TAString("Ranges")));
      for (auto& id : _value.FrequencyRanges)
      {
         auto element = std::unique_ptr<NXml::TElement>(new NXml::TElement(NAmg::TAString("Range")));
         element->AddAttr("id", id);
         ranges->AddChild(*element.release());
      }
      _xml.AddChild(*ranges.release());
   }
}

void TEquipmentType::FromXml(const NAmg::NXml::TElement& _xml, TEquipmentType& _value)
{
   _value = {};

   const NXml::TAttribute* attr;
   if (nullptr != (attr = _xml.FindAttr("device-type-id")))
   {
      _value.DeviceTypeId = attr->AsTString();
   }
   if (nullptr != (attr = _xml.FindAttr("role-id")))
   {
      _value.RoleId = attr->AsTString();
   }
   if (nullptr != (attr = _xml.FindAttr("can-transmit")))
   {
      _value.CanTransmit = attr->AsTString().ToBool();
   }
   if (nullptr != (attr = _xml.FindAttr("can-receive")))
   {
      _value.CanReceive = attr->AsTString().ToBool();
   }

   auto ranges = _xml.FirstElChild("Ranges");
   if (!ranges.IsEmpty())
   {
      for (TVectorIndex i = 0; i < ranges.AsElement().ElemCount("Range"); i++)
      {
         auto& element(ranges.AsElement().ElChild(i, "Range"));
         if (nullptr == (attr = element.FindAttr("id")))
         {
            continue;
         }
         _value.FrequencyRanges.push_back(attr->AsTString());
      }
   }
}
namespace
{
   TServiceMediaEnum ServiceMediaEnum;
}
void TServiceData::ToJson(const TServiceData& _value, Json::Value& _json)
{
   _json["label"] = _value.Label;
   _json["description"] = _value.Description;
   _json["switch_id"] = _value.SwitchId;
   _json["range_id"] = _value.FrequencyRangeId;
   _json["crypted"] = _value.Crypted;
   _json["media_type_id"] = ServiceMediaEnum.ToString(_value.MediaTypeId);
   _json["graph_id"] = _value.GraphId;
   _json["encrypted_id"] = _value.EncryptedId.Fast();
   _json["in"]["radio"]["id"] = _value.In.Radio.Id;
   _json["in"]["radio"]["port_id"] = _value.In.Radio.PortId;
   _json["in"]["antenna_id"] = _value.In.AntennaId;
   _json["out"]["radio"]["id"] = _value.Out.Radio.Id;
   _json["out"]["radio"]["port_id"] = _value.Out.Radio.PortId;
   _json["out"]["antenna_id"] = _value.Out.AntennaId;

   if (!_value.TargetUri.IsEmpty())
      _json["target-uri"] = _value.TargetUri.ToString();

   _json["can-send"] = _value.CanSend;
   _json["can-recv"] = _value.CanRecv;

   if (!_value.Modes.empty())
   {
      auto& jsonNode = _json["modes"];
      for (auto& mode : _value.Modes)
      {
         TModeData::ToJson(mode.second, jsonNode[mode.first]);
      }
   }

   if (!_value.Endpoints.empty())
   {
      auto& jsonNode = _json["endpoints"];
      for (auto& endpoint : _value.Endpoints)
      {
         TEndpointData::ToJson(endpoint.second, jsonNode[endpoint.first]);
      }
   }
}

void TServiceData::FromJson(const Json::Value& _json, TServiceData& _value)
{
   _value = {};
   NAmg::NCakApi::JsonDeserializer(_json)
      .Extract("label", _value.Label)
      .Extract("description", _value.Description)
      .Extract("switch_id", _value.SwitchId)
      .Extract("range_id", _value.FrequencyRangeId)
      .Extract("crypted", _value.Crypted)
      .Handle("media_type_id", [&_value](const Json::Value& _media_type_id)
   {
      _value.MediaTypeId = ServiceMediaEnum.FromString(_media_type_id.asString());
   })
      .Extract("graph_id", _value.GraphId)
      .Extract("target-uri", _value.TargetUri)
      .Extract("can-send", _value.CanSend)
      .Extract("can-recv", _value.CanRecv)
      .Extract("encrypted_id", _value.EncryptedId)
      .Handle("in", [&_value](const Json::Value& _in)
   {
      _value.In.Radio.Id = _in["radio"]["id"].asString();
      _value.In.Radio.PortId = _in["radio"]["port_id"].asString();
      _value.In.AntennaId = _in["antenna_id"].asString();
   })
      .Handle("out", [&_value](const Json::Value& _out)
   {
      _value.Out.Radio.Id = _out["radio"]["id"].asString();
      _value.Out.Radio.Id = _out["radio"]["port_id"].asString();
      _value.Out.AntennaId = _out["antenna_id"].asString();
   })
      .Handle("modes", [&_value](const Json::Value& _modes)
   {
      auto ids = _modes.getMemberNames();
      for (auto& id : ids)
      {
         TModeData mode;
         TModeData::FromJson(_modes[id], mode);
         _value.Modes.insert({id, mode});
      }
   })
      .Handle("endpoints", [&_value](const Json::Value& _endpoints)
   {
      auto ids = _endpoints.getMemberNames();
      for (auto& id : ids)
      {
         TEndpointData endpoint;
         TEndpointData::FromJson(_endpoints[id], endpoint);
         _value.Endpoints.insert({id, endpoint});
      }
   });
}

void TServiceData::ToXml(const TServiceData& _value, NAmg::NXml::TElement& _xml)
{
   _xml.AddAttr("label", _value.Label);
   _xml.AddAttr("range-id", _value.FrequencyRangeId);
   _xml.AddAttr("media-type-id", ServiceMediaEnum.ToString(_value.MediaTypeId));
   _xml.AddAttr("graph-id", _value.GraphId);

   if (!_value.TargetUri.IsEmpty())
      _xml.AddAttr("target-uri", _value.TargetUri.ToString());

   static auto AddChild = [](NAmg::NXml::TElement& _xml, std::string&& _name, std::string _value)
   {
      using namespace NAmg::NXml;

      std::unique_ptr<TElement> element(new TElement(std::move(_name)));
      element->AddChild(*new TText(std::move(_value)));
      _xml.AddChild(*element.release());
   };

   AddChild(_xml, "Description", _value.Description);
   AddChild(_xml, "Crypted", TAString::BoolToStr(_value.Crypted));
   AddChild(_xml, "Can-Send", TAString::BoolToStr(_value.CanSend));
   AddChild(_xml, "Can-Recv", TAString::BoolToStr(_value.CanRecv));
   std::unique_ptr<NAmg::NXml::TElement> encryptedXml(
      new NAmg::NXml::TElement("Encrypted"));
   encryptedXml->AddAttr("id", _value.EncryptedId.Fast());
   _xml.AddChild(*encryptedXml.release());

   std::unique_ptr<NAmg::NXml::TElement> switchXml(
      new NAmg::NXml::TElement("Switch"));
   switchXml->AddAttr("id", _value.SwitchId);
   _xml.AddChild(*switchXml.release());

   std::unique_ptr<NAmg::NXml::TElement> inXml(
      new NAmg::NXml::TElement("In"));
   std::unique_ptr<NAmg::NXml::TElement> radioXml(
      new NAmg::NXml::TElement("Radio"));
   radioXml->AddAttr("id", _value.In.Radio.Id);
   radioXml->AddAttr("port-id", _value.In.Radio.PortId);
   inXml->AddChild(*radioXml.release());
   std::unique_ptr<NAmg::NXml::TElement> antennaXml(
      new NAmg::NXml::TElement("Antenna"));
   antennaXml->AddAttr("id", _value.In.AntennaId);
   inXml->AddChild(*antennaXml.release());
   _xml.AddChild(*inXml.release());

   std::unique_ptr<NAmg::NXml::TElement> outXml(
      new NAmg::NXml::TElement("Out"));
   std::unique_ptr<NAmg::NXml::TElement> radioXml2(new NAmg::NXml::TElement("Radio"));
   radioXml2->AddAttr("id", _value.Out.Radio.Id);
   radioXml2->AddAttr("port-id", _value.Out.Radio.PortId);
   outXml->AddChild(*radioXml2.release());
   std::unique_ptr<NAmg::NXml::TElement>  antennaXml2(new NAmg::NXml::TElement("Antenna"));
   antennaXml2->AddAttr("id", _value.In.AntennaId);
   outXml->AddChild(*antennaXml2.release());
   _xml.AddChild(*outXml.release());
   
   if (!_value.Modes.empty())
   {
      auto& modesElement = *(new NXml::TElement("Modes"));
      for (auto& mode : _value.Modes)
      {
         auto& element = *(new NXml::TElement("Mode"));
         element.AddAttr("id", mode.first);
         TModeData::ToXml(mode.second, element);
         modesElement.AddChild(element);
      }
      _xml.AddChild(modesElement);
   }

   if (!_value.Endpoints.empty())
   {
      auto& endpointsElement = *(new NXml::TElement("Endpoints"));
      for (auto& endpoint : _value.Endpoints)
      {
         auto& element = *(new NXml::TElement("Endpoint"));
         element.AddAttr("id", endpoint.first);
         TEndpointData::ToXml(endpoint.second, element);
         endpointsElement.AddChild(element);
      }
      _xml.AddChild(endpointsElement);
   }
}

void TServiceData::FromXml(const NAmg::NXml::TElement& _xml, TServiceData& _value)
{
   _value = {};

   auto* attr = _xml.FindAttr("label");
   if (nullptr != attr)
      _value.Label = attr->AsTString();

   attr = _xml.FindAttr("range-id");
   if (nullptr != attr)
      _value.FrequencyRangeId = attr->AsTString();

   attr = _xml.FindAttr("media-type-id");
   if (nullptr != attr)
      _value.MediaTypeId = ServiceMediaEnum.FromString(attr->AsTString());
   else
      _value.MediaTypeId = TServiceMediaEnum::TType::Audio;

   attr = _xml.FindAttr("graph-id");
   if (nullptr != attr)
      _value.GraphId = attr->AsTString();

   attr = _xml.FindAttr("target-uri");
   if (nullptr != attr)
      _value.TargetUri = attr->Value();

   if (1 <= _xml.ElemCount("Description"))
      _value.Description = _xml.ElChild(0, "Description").GetText();

   if (1 <= _xml.ElemCount("Crypted"))
      _value.Crypted = _xml.ElChild(0, "Crypted").GetText().ToBool();

   _value.CanSend = 1 <= _xml.ElemCount("Can-Send") ?
      _xml.ElChild(0, "Can-Send").GetText().ToBool() : true;

   _value.CanRecv = 1 <= _xml.ElemCount("Can-Recv") ?
      _xml.ElChild(0, "Can-Recv").GetText().ToBool() : true;
   if (1 <= _xml.ElemCount("Encrypted"))
   {
      _value.EncryptedId = _xml.ElChild(0, "Encrypted").FindAttr("id")->AsTString();
   }
   if (1 <= _xml.ElemCount("Switch"))
   {
      _value.SwitchId = _xml.ElChild(0, "Switch").FindAttr("id")->AsTString();
   }
   if (1 <= _xml.ElemCount("In"))
   {
      _value.In.Radio.Id = _xml.ElChild(0, "In").ElChild(0, "Radio").
         FindAttr("id")->AsTString();
      _value.In.Radio.PortId = _xml.ElChild(0, "In").ElChild(0, "Radio").
         FindAttr("port-id")->AsTString();
      _value.In.AntennaId = _xml.ElChild(0, "In").ElChild(0, "Antenna").
         FindAttr("id")->AsTString();
   }
   if (1 <= _xml.ElemCount("Out"))
   {
      _value.Out.Radio.Id = _xml.ElChild(0, "Out").ElChild(0, "Radio").
         FindAttr("id")->AsTString();
      _value.Out.Radio.PortId = _xml.ElChild(0, "Out").ElChild(0, "Radio").
         FindAttr("port-id")->AsTString();
      _value.Out.AntennaId = _xml.ElChild(0, "Out").ElChild(0, "Antenna").
         FindAttr("id")->AsTString();
   }
   if (1 <= _xml.ElemCount("Modes"))
   {
      auto& element = _xml.ElChild(0, "Modes");
      for (TVectorIndex i = 0, c = element.ElemCount("Mode"); i < c; ++i)
      {
         auto& item = element.ElChild(i);
         auto* attr = item.FindAttr("id");
         if (nullptr != attr)
         {
            TModeData mode;
            TModeData::FromXml(item, mode);
            _value.Modes.insert({attr->AsTString(), mode});
         }
      }
   }

   if (1 <= _xml.ElemCount("Endpoints"))
   {
      auto& element = _xml.ElChild(0, "Endpoints");
      for (TVectorIndex i = 0, c = element.ElemCount("Endpoint"); i < c; ++i)
      {
         auto& item = element.ElChild(i);
         auto* attr = item.FindAttr("id");
         if (nullptr != attr)
         {
            TEndpointData endpoint;
            TEndpointData::FromXml(item, endpoint);
            _value.Endpoints.insert({attr->AsTString(), endpoint});
         }
      }
   }
}

void TServiceState::ToJson(const TServiceState& _value, Json::Value& _json)
{
   auto& out = _json["out"];
   out["antenna_id"] = _value.Out.AntennaId;
   auto& outRadio = out["radio"];
   outRadio["id"] = _value.Out.Id;
   outRadio["port_id"] = _value.Out.PortId;
   auto& in = _json["in"];
   in["antenna_id"] = _value.In.AntennaId;
   auto& inRadio = in["radio"];
   inRadio["id"] = _value.In.Id;
   inRadio["port_id"] = _value.In.PortId;
   _json["encrypted_id"] = _value.EncryptedId;
   auto& mode = _json["mode"];
   mode["current"] = _value.CurrentModeId;
   mode["awaited"] = _value.AwaitedModeId;
   TModeState::ToJson(_value.ModeState, mode["state"]);
   auto& endpoints = _json["endpoints"];
   for (auto& item : _value.Endpoints)
   {
      TEndpointState::ToJson(item.second, endpoints[item.first]);
   }
   auto& locked = _json["locked-modes"];
   Json::UInt index = 0u;
   for (auto& item : _value.LockedModes)
   {
      locked[index++] = item;
   }
}

void TServiceState::FromJson(const Json::Value& _json, TServiceState& _value)
{
   //_value = {};
   NAmg::NCakApi::JsonDeserializer(_json)
      .Extract("encrypted_id", _value.EncryptedId)
      .Handle("in", [&_value](const Json::Value& _in) 
   {
      NAmg::NCakApi::JsonDeserializer(_in)
         .Extract("antenna_id", _value.In.AntennaId)
         .Handle("radio",[&_value](const Json::Value& _radio)
      {
         NAmg::NCakApi::JsonDeserializer(_radio)
            .Extract("id", _value.In.Id)
            .Extract("port_id", _value.In.PortId);
      });
   })
      .Handle("out",[&_value](const Json::Value& _out)
   {
      NAmg::NCakApi::JsonDeserializer(_out)
         .Extract("antenna_id", _value.In.AntennaId)
         .Handle("radio", [&_value](const Json::Value& _radio)
      {
         NAmg::NCakApi::JsonDeserializer(_radio)
            .Extract("id", _value.In.Id)
            .Extract("port_id", _value.In.PortId);
      });
   })
      .Handle("mode", [&_value](const Json::Value& _mode)
   {
      NAmg::NCakApi::JsonDeserializer(_mode)
         .Extract("current", _value.CurrentModeId)
         .Extract("awaited", _value.AwaitedModeId)
         .Handle("state", [&_value](const Json::Value& _state)
      {
         TModeState::FromJson(_state, _value.ModeState);
      });
   })
      .Handle("endpoints", [&_value](const Json::Value& _endpoints)
   {
      auto endpointNames = _endpoints.getMemberNames();
      for (auto& id : endpointNames)
      {
         TEndpointState::FromJson(_endpoints[id], _value.Endpoints[id]);
      }
   })
   .Handle("locked-modes", [&_value](const Json::Value& _ids)
   {
      _value.LockedModes.clear();

      for (auto& id : _ids.getMemberNames())
         _value.LockedModes.insert(id);
   })
   .Handle("lock", [&_value](const Json::Value& _ids)
   {
      for (auto& id : _ids.getMemberNames())
         _value.LockedModes.insert(id);
   })
   .Handle("unlock", [&_value](const Json::Value& _ids)
   {
      for (auto& id : _ids.getMemberNames())
         _value.LockedModes.erase(id);
   });
}

namespace
{
   void AmgCall ToCache(NAmg::NCakApi::TBud& _bud, TServiceState::TAdapter& _adapter,
      const std::string& _serviceId, const std::string& _modeId, std::string&& _target)
   {
      NAmg::NScl::NHttp::TQueueUpdate update;
      update.Method = "PUT";
      update.Path = _adapter.Path + _serviceId;
      update.Path += "mode";
      update.Path += std::move(_target);
      update.Body = &NAmg::NScl::NText::toBody(Json::Value(_modeId));
      _bud.Update(update);
   }
}

namespace
{
   struct Converter
   {
      Converter()
      {
         static TServiceState initValue;
         TServiceState::ToJson(initValue, Json);
      }

      static Json::Value Json;
   };

   Json::Value Converter::Json;
}

void AmgCall TServiceState::InitInCache(NAmg::NCakApi::TBud& _bud, TServiceState::TAdapter& _adapter,
   const std::string& _serviceId)
{
   static Converter converter;

   NAmg::NScl::NHttp::TQueueUpdate update;
   update.Method = "PUT";
   update.Path = _adapter.Path + _serviceId;

   update.Body = &NAmg::NScl::NText::toBody(converter.Json);
   _bud.Update(update);
}

void AmgCall TServiceState::CurrentToCache(NAmg::NCakApi::TBud& _bud, TAdapter& _adapter, const std::string& _serviceId, const std::string& _currentModeId)
{
   ToCache(_bud, _adapter, _serviceId, _currentModeId, "current");
}

void AmgCall TServiceState::AwaitedToCache(NAmg::NCakApi::TBud& _bud, TAdapter& _adapter, const std::string& _serviceId, const std::string& _awaitedModeId)
{
   ToCache(_bud, _adapter, _serviceId, _awaitedModeId, "awaited");
}

void AmgCall TServiceState::LockingToCache(NAmg::NCakApi::TBud& _bud, TAdapter& _adapter, const std::string& _serviceId, std::forward_list<std::string> _ids, bool _doLock)
{
   Json::Value array(Json::arrayValue);
   Json::UInt index = 0u;
   for (auto& id : _ids)
      array[index++] = id;

   Json::Value json;
   json[_doLock ? "lock" : "unlock"] = array;

   NAmg::NScl::NHttp::TQueueUpdate update;
   update.Method = "PUT";
   update.Path = _adapter.Path;
   update.Path += _serviceId;
   update.Path += "locked-modes";

   update.Body = &NAmg::NScl::NText::toBody(json);
   _bud.Update(update);
}

namespace
{
   void AmgCall RequestTo(NAmg::NCakApi::TBud& _bud, TServiceState::TAdapter& _adapter, const std::string& _serviceId, const std::string& _modeId)
   {
      NAmg::NScl::NHttp::TQueueRequest request;
      request.Method = _modeId.empty() ? "DELETE" : "POST";
      request.Path = _adapter.Path + _serviceId;

      if (!_modeId.empty())
      {
         Json::Value json;
         json["id"] = _modeId;
         request.Body = &NAmg::NScl::NText::toBody(json);
      }

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

void AmgCall TServiceState::RequestToBuild(NAmg::NCakApi::TBud& _bud, TAdapter& _adapter, const std::string& _serviceId, const std::string& _modeId)
{
   RequestTo(_bud, _adapter, _serviceId, _modeId);
}

void AmgCall TServiceState::RequestToDestroy(NAmg::NCakApi::TBud& _bud, TAdapter& _adapter, const std::string& _serviceId)
{
   RequestTo(_bud, _adapter, _serviceId, "");
}

namespace NAmg
{
namespace NScl
{
namespace NText
{

template<> TBody::TREF AmgCall toBody<TServiceData>(const TServiceData& _inValue)
{
   Json::Value json;
   TServiceData::ToJson(_inValue, json);
   return(toBody(json));
}

template<> void AmgCall fromBody<TServiceData>(const TBody& _body, TServiceData& _outValue)
{
   Json::Value json;
   fromBody(_body, json);
   TServiceData::FromJson(json, _outValue);
}

}
}
}
