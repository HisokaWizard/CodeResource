#include "precomp.h"
//
//! @file
//! @brief Реализация программных интерфейсов модулей базового приложения
//!   и менеджера службы присутствия.
//
#ifdef __BORLANDC__
#pragma hdrstop
#endif
//
#include <JsonCpp/json.h>
//
#include "Group.h"
//
#ifdef __BORLANDC__
   #pragma option -w -O2 -vi- -b -6 -k -a16 -pc -ff
#endif
//
namespace NAmg
{
   //
   void TBioGroup::ToJson(const TBioGroup& _value, Json::Value& _json)
   {
      _json["label"] = _value.Label;
      _json["description"] = _value.Description;
      _json["minpulse"] = _value.MinPulse;
      _json["maxpulse"] = _value.MaxPulse;
      _json["mintemperature"] = _value.MinTemperature;
      _json["maxtemperature"] = _value.MaxTemperature;
      _json["minbp"] = _value.MinBp;
      _json["maxbp"] = _value.MaxBp;
      _json["minblood"] = _value.MinBlood;
      _json["maxblood"] = _value.MaxBlood;
   }
   //
   void TBioGroup::FromJson(const Json::Value& _json, TBioGroup& _value)
   {
      auto members = _json.getMemberNames();

      if (members.end() != std::find(members.begin(), members.end(), "label"))
      {
         _value.Label = _json["label"].asString();
      }
      if (members.end() != std::find(members.begin(), members.end(), "description"))
      {
         _value.Description = _json["description"].asString();
      }
      if (members.end() != std::find(members.begin(), members.end(), "minpulse"))
      {
         _value.MinPulse = _json["minpulse"].asInt();
      }
      if (members.end() != std::find(members.begin(), members.end(), "maxpulse"))
      {
         _value.MaxPulse = _json["maxpulse"].asInt();
      }
      if (members.end() != std::find(members.begin(), members.end(), "mintemperature"))
      {
         _value.MinTemperature = _json["mintemperature"].asInt();
      }
      if (members.end() != std::find(members.begin(), members.end(), "maxtemperature"))
      {
         _value.MaxTemperature = _json["maxtemperature"].asInt();
      }
      if (members.end() != std::find(members.begin(), members.end(), "minbp"))
      {
         _value.MinBp = _json["minbp"].asInt();
      }
      if (members.end() != std::find(members.begin(), members.end(), "maxbp"))
      {
         _value.MaxBp = _json["maxbp"].asInt();
      }
      if (members.end() != std::find(members.begin(), members.end(), "minblood"))
      {
         _value.MinBlood = _json["minblood"].asInt();
      }
      if (members.end() != std::find(members.begin(), members.end(), "maxblood"))
      {
         _value.MaxBlood = _json["maxblood"].asInt();
      }
   }
   //
   void TBioGroup::ToXml(const TBioGroup& _value, NAmg::NXml::TElement& _xml)
   {
      _xml.AddAttr("label", _value.Label);

      auto& descriptionElement = *(new NAmg::NXml::TElement("Description"));
      descriptionElement.AddChild(*(new NAmg::NXml::TText(_value.Description)));
      _xml.AddChild(descriptionElement);

      auto& MinPulseElement = *(new NAmg::NXml::TElement("MinPulse"));
      MinPulseElement.AddChild(*(new NAmg::NXml::TText(TAString(_value.MinPulse))));
      _xml.AddChild(MinPulseElement);
      auto& MaxPulseElement = *(new NAmg::NXml::TElement("MaxPulse"));
      MaxPulseElement.AddChild(*(new NAmg::NXml::TText(TAString(_value.MaxPulse))));
      _xml.AddChild(MaxPulseElement);

      auto& MinTemperatureElement = *(new NAmg::NXml::TElement("MinTemperature"));
      MinTemperatureElement.AddChild(*(new NAmg::NXml::TText(TAString(_value.MinTemperature))));
      _xml.AddChild(MinTemperatureElement);
      auto& MaxTemperatureElement = *(new NAmg::NXml::TElement("MaxTemperature"));
      MaxTemperatureElement.AddChild(*(new NAmg::NXml::TText(TAString(_value.MaxTemperature))));
      _xml.AddChild(MaxTemperatureElement);

      auto& MinBpElement = *(new NAmg::NXml::TElement("MinBp"));
      MinBpElement.AddChild(*(new NAmg::NXml::TText(TAString(_value.MinBp))));
      _xml.AddChild(MinBpElement);
      auto& MaxBpElement = *(new NAmg::NXml::TElement("MaxBp"));
      MaxBpElement.AddChild(*(new NAmg::NXml::TText(TAString(_value.MaxBp))));
      _xml.AddChild(MaxBpElement);

      auto& MinBloodElement = *(new NAmg::NXml::TElement("MinBlood"));
      MinBloodElement.AddChild(*(new NAmg::NXml::TText(TAString(_value.MinBlood))));
      _xml.AddChild(MinBloodElement);
      auto& MaxBloodElement = *(new NAmg::NXml::TElement("MaxBlood"));
      MaxBloodElement.AddChild(*(new NAmg::NXml::TText(TAString(_value.MaxBlood))));
      _xml.AddChild(MaxBloodElement);
   }
   //
   void TBioGroup::FromXml(const NAmg::NXml::TElement& _xml, TBioGroup& _value)
   {
      auto* attrLabel = _xml.FindAttr("label");
      if ((nullptr != attrLabel))
      {
         _value.Label = attrLabel->AsTString();
      }
      if (1 == _xml.ElemCount("Description"))
      {
         _value.Description = _xml.ElChild(0, "Description").GetText();
      }
      if (1 == _xml.ElemCount("MinPulse"))
      {
         _value.MinPulse = _xml.ElChild(0, "MinPulse").GetText().ToUInt();
      }
      if (1 == _xml.ElemCount("MaxPulse"))
      {
         _value.MaxPulse = _xml.ElChild(0, "MaxPulse").GetText().ToUInt();
      }
      if (1 == _xml.ElemCount("MinTemperature"))
      {
         _value.MinTemperature = _xml.ElChild(0, "MinTemperature").GetText().ToUInt();
      }
      if (1 == _xml.ElemCount("MaxTemperature"))
      {
         _value.MaxTemperature = _xml.ElChild(0, "MaxTemperature").GetText().ToUInt();
      }
      if (1 == _xml.ElemCount("MinBp"))
      {
         _value.MinBp = _xml.ElChild(0, "MinBp").GetText().ToUInt();
      }
      if (1 == _xml.ElemCount("MaxBp"))
      {
         _value.MaxBp = _xml.ElChild(0, "MaxBp").GetText().ToUInt();
      }
      if (1 == _xml.ElemCount("MinBlood"))
      {
         _value.MinBlood = _xml.ElChild(0, "MinBlood").GetText().ToUInt();
      }
      if (1 == _xml.ElemCount("MaxBlood"))
      {
         _value.MaxBlood = _xml.ElChild(0, "MaxBlood").GetText().ToUInt();
      }
   }
   //
   namespace NScl
   {
      namespace NText
      {
         template<> TBody::TREF AmgCall toBody<TBioGroup>(const TBioGroup& _inValue)
         {
            Json::Value json;
            TBioGroup::ToJson(_inValue, json);
            return(toBody(json));
         }
         template<> void AmgCall fromBody<TBioGroup>(const TBody& _body, TBioGroup& _outValue)
         {
            Json::Value json;
            fromBody(_body, json);
            TBioGroup::FromJson(json, _outValue);
         }
      }
   }
//
}