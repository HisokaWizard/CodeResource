//! @file
//! @brief
#include "precomp.h"

#include <JsonCpp/json.h>
#include <XML/amgXmlElement.h>
#include <Debug/amgDebug.h>
#include <amgErrors.h>
#include <ckiBudService.h>
#include <ckiSerializers.h>
#include "Mode.h"

using namespace NAmg;

namespace
{
const std::map<std::string, TModeState::TPresetState>& AmgCall PresetStateValues(void)
{
   static std::map<std::string, TModeState::TPresetState> value = 
   {
      {"pending",  TModeState::Pending},
      {"accepted", TModeState::Accepted},
      {"rejected", TModeState::Rejected}
   };
   return(value);
}
}

void TModeData::ToXml(const TModeData& _value, NAmg::NXml::TElement& _xml)
{
   _xml.AddAttr("label", _value.Label);

   auto& descriptionElement = *(new NAmg::NXml::TElement("Description"));
   descriptionElement.AddChild(*(new NAmg::NXml::TText(_value.Description)));
   _xml.AddChild(descriptionElement);

   if (!_value.Channel.IsNULL())
      _xml.AddAttr("channel", TAString::IntToStr(_value.Channel()));
   if (!_value.RxFrequency.IsNULL())
      _xml.AddAttr("rx-frequency", TAString::IntToStr(_value.RxFrequency()));
   if (!_value.TxFrequency.IsNULL())
      _xml.AddAttr("tx-frequency", TAString::IntToStr(_value.TxFrequency()));
   if (!_value.RxClassId.IsNULL())
      _xml.AddAttr("rx-class-id", _value.RxClassId());
   if (!_value.TxClassId.IsNULL())
      _xml.AddAttr("tx-class-id", _value.TxClassId());
   if (!_value.TxClassId.IsNULL())
      _xml.AddAttr("tx-power", TAString::IntToStr(_value.TxPower()));

   auto PresetsToXML = [&_xml](const decltype(_value.BuildPresets)& presets, const std::string& tag)
   {
      if (!presets.empty())
      {
         auto presetsElement = std::unique_ptr<NXml::TElement>(new NXml::TElement(tag));
         std::unique_ptr<NXml::TElement> itemElement;
         for (auto& preset : presets)
         {
            itemElement.reset(new NXml::TElement("Preset"));
            itemElement->AddAttr("Host", preset.HostId);
            NAmg::NCakApi::TSpecificPreset::ToXml(preset.Preset, *itemElement);
            presetsElement->AddChild(*itemElement.release());
         }
         _xml.AddChild(*presetsElement.release());
      }
   };

   PresetsToXML(_value.BuildPresets, "Build");
   PresetsToXML(_value.DestroyPresets, "Destroy");
}

void TModeData::FromXml(const NAmg::NXml::TElement& _xml, TModeData& _value)
{
   _value = {};

   auto* attr = _xml.FindAttr("label");
   if (nullptr != attr)
   {
      _value.Label = attr->AsTString();
   }

   if (1 <= _xml.ElemCount("Description"))
   {
      _value.Description = _xml.ElChild(0, "Description").GetText();
   }

   attr = _xml.FindAttr("channel");
   if (nullptr != attr)
   {
      _value.Channel = attr->AsTString().ToUInt16();
   }

   attr = _xml.FindAttr("rx-frequency");
   if (nullptr != attr)
   {
      _value.RxFrequency = attr->AsTString().ToUInt16();
   }

   attr = _xml.FindAttr("tx-frequency");
   if (nullptr != attr)
   {
      _value.TxFrequency = attr->AsTString().ToUInt16();
   }

   attr = _xml.FindAttr("rx-class-id");
   if (nullptr != attr)
   {
      _value.RxClassId = attr->AsTString();
   }

   attr = _xml.FindAttr("tx-class-id");
   if (nullptr != attr)
   {
      _value.TxClassId = attr->AsTString();
   }
   
   attr = _xml.FindAttr("tx-power");
   if (nullptr != attr)
   {
      _value.TxPower = attr->AsTString().ToUInt16();
   }
   auto PresetsFromXML = [&_xml](decltype(_value.BuildPresets)& presets, const std::string& tag)
   {
      if (1 == _xml.ElemCount(tag))
      {
         auto& element = _xml.ElChild(0, tag);
         for (TVectorIndex i = 0, size = element.ElemCount("Preset"); i < size; ++i)
         {
            auto& child = element.ElChild(i);

            NAmg::NCakApi::TSpecificPreset preset;
            NAmg::NCakApi::TSpecificPreset::FromXml(child, preset);

            presets.push_back({ child.Attribute("Host").Value(), std::move(preset) });
         }
      }
   };

   PresetsFromXML(_value.BuildPresets, "Build");
   PresetsFromXML(_value.DestroyPresets, "Destroy");
}

void TModeData::ToJson(const TModeData& _value, Json::Value& _json)
{
   auto PresetsToJson = [](Json::Value& _array, const TPresets& _presets)
   {
      for (auto& item : _presets)
      {
         auto& jsonItem = _array.append({});
         jsonItem["host"] = item.HostId;
         NAmg::NCakApi::TSpecificPreset::ToJson(item.Preset, jsonItem["preset"]);
      }
   };

   NCakApi::JsonSerializer(_json)
      .Add("label", _value.Label)
      .Add("description", _value.Description)
      .Add("channel", _value.Channel)
      .Add("rx_frequency", _value.RxFrequency)
      .Add("tx_frequency", _value.TxFrequency)
      .Add("rx_class_id", _value.RxClassId)
      .Add("tx_class_id", _value.TxClassId)
      .Add("tx_power", _value.TxPower)
      .Handle("build", [&_value, &PresetsToJson](Json::Value& _build)
      {
         PresetsToJson(_build, _value.BuildPresets); 
      })
      .Handle("destroy", [&_value, &PresetsToJson](Json::Value& _destroy)
      {
         PresetsToJson(_destroy, _value.DestroyPresets);
      });
}

void TModeData::FromJson(const Json::Value& _json, TModeData& _value)
{
   _value = {};

   auto PresetFromJson = [](const Json::Value& _item, TPresets& _presets)
   {      
      NAmg::NCakApi::TSpecificPreset preset;
      NAmg::NCakApi::TSpecificPreset::FromJson(_item["preset"], preset);
      _presets.push_back({ _item["host"].asString(), preset });
   };

   NAmg::NCakApi::JsonDeserializer(_json)
      .Extract("label", _value.Label)
      .Extract("description", _value.Description)
      .Extract("channel", _value.Channel)
      .Extract("rx_frequency", _value.RxFrequency)
      .Extract("tx_frequency", _value.TxFrequency)
      .Extract("rx_class_id", _value.RxClassId)
      .Extract("tx_class_id", _value.TxClassId)
      .Extract("tx_power", _value.TxPower)
      .HandleArray("build", [&_value, &PresetFromJson](const Json::Value& _item)
   {
      PresetFromJson(_item, _value.BuildPresets);
   })
      .HandleArray("destroy", [&_value, &PresetFromJson](const Json::Value& _item)
   {
      PresetFromJson(_item, _value.DestroyPresets);
   });
}

void TModeState::ToJson(const TModeState& _value, Json::Value& _json)
{
   auto PresetsToJson = [](Json::Value& _array, const TPresets& _presets)
   {      
      auto index = 0;
      for (auto& preset : _presets)
      {
         _array[index++] = TModeState::PresetStateToStr(preset);
      }
   };

   NCakApi::JsonSerializer(_json)
      .Handle("build", [&_value, &PresetsToJson](Json::Value& _build)
   {
      PresetsToJson(_build, _value.BuildPresets);
   })
      .Handle("destroy", [&_value, &PresetsToJson](Json::Value& _destroy)
   {
      PresetsToJson(_destroy, _value.DestroyPresets);
   });
}

void TModeState::FromJson(const Json::Value& _json, TModeState& _value)
{
   //_value = {};   
   NAmg::NCakApi::JsonDeserializer(_json)
      .HandleArray("build", [&_value](const Json::Value& _item)
   {
      _value.BuildPresets.push_back(TModeState::StrToPresetState(_item.asString()));
   })
      .HandleArray("destroy", [&_value](const Json::Value& _item)
   {
      _value.DestroyPresets.push_back(TModeState::StrToPresetState(_item.asString()));
   });
}

bool AmgCall TModeState::Completed(void) const
{
   for (auto& itBuild : BuildPresets)
   {
      if (TModeState::Pending == itBuild)
         return(false);
   }
   for (auto& itBuild : DestroyPresets)
   {
      if (TModeState::Pending == itBuild)
         return(false);
   }
   return(true);
}

bool AmgCall TModeState::Successfull(void) const
{
   for (auto& itBuild : BuildPresets)
      if (TModeState::Accepted != itBuild)
         return false;

   for (auto& itBuild : DestroyPresets)
      if (TModeState::Accepted != itBuild)
         return false;

   return true;
}

std::string AmgCall TModeState::PresetStateToStr(TModeState::TPresetState _value)
{
   for (const auto& item : PresetStateValues())
   {
      if (item.second == _value)
      {
         return(item.first);
      }
   }
   return(PresetStateValues().begin()->first);
}

TModeState::TPresetState AmgCall TModeState::StrToPresetState(const std::string& _str)
{
   const auto& values = PresetStateValues();
   auto it = values.find(_str);
   if (values.end() != it)
   {
      return(it->second);
   }
   return(PresetStateValues().begin()->second);
}

void AmgCall TModeState::ToCache(NAmg::NCakApi::TBud& _bud, const NAmg::TPath& _root, const std::string& _serviceId, const TModeState& _state)
{
   NAmg::NScl::NHttp::TQueueUpdate update;
   update.Method = "PUT";
   update.Path = _root + _serviceId + "mode/state";

   Json::Value json;
   TModeState::ToJson(_state, json);
   update.Body = &NAmg::NScl::NText::toBody(json);

   _bud.Update(update);
}
