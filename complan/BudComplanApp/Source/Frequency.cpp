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
#include <ckiSerializers.h>
//
#include "Frequency.h"
//
#ifdef __BORLANDC__
#pragma option -w -O2 -vi- -b -6 -k -a16 -pc -ff
#endif
//
using namespace NAmg;
//
void TEmissionClass::ToJson(const TEmissionClass& _value, Json::Value& _json)
{
   _json["label"] = _value.Label;
   _json["description"] = _value.Description;
}
//
void TEmissionClass::FromJson(const Json::Value& _json, TEmissionClass& _value)
{
   _value = {};
   NAmg::NCakApi::JsonDeserializer(_json)
      .Extract("label", _value.Label)
      .Extract("description", _value.Description);
}
//
void TEmissionClass::ToXml(const TEmissionClass& _value, NAmg::NXml::TElement& _xml)
{
   _xml.AddAttr("label", _value.Label);

   auto& descriptionElement = *(new NAmg::NXml::TElement("Description"));
   descriptionElement.AddChild(*(new NAmg::NXml::TText(_value.Description)));
   _xml.AddChild(descriptionElement);

}
//
void TEmissionClass::FromXml(const NAmg::NXml::TElement& _xml, TEmissionClass& _value)
{
   _value = {};

   auto* attr = _xml.FindAttr("label");
   if (nullptr != attr)
   {
      _value.Label = attr->AsTString();
   }

   if (1 == _xml.ElemCount("Description"))
   {
      _value.Description = _xml.ElChild(0, "Description").GetText();
   }
}
//
void TFrequencyRange::ToJson(const TFrequencyRange& _value, Json::Value& _json)
{
   _json["label"] = _value.Label;
   _json["description"] = _value.Description;
   _json["suffix"] = _value.Suffix;
}
//
void TFrequencyRange::FromJson(const Json::Value& _json, TFrequencyRange& _value)
{
   _value = {};
   NAmg::NCakApi::JsonDeserializer(_json)
      .Extract("label", _value.Label)
      .Extract("description", _value.Description)
      .Extract("suffix", _value.Suffix);
}
//
void TFrequencyRange::ToXml(const TFrequencyRange& _value, NAmg::NXml::TElement& _xml)
{
   _xml.AddAttr("label", _value.Label);

   auto& descriptionElement = *(new NAmg::NXml::TElement("Description"));
   descriptionElement.AddChild(*(new NAmg::NXml::TText(_value.Description)));
   _xml.AddChild(descriptionElement);

   _xml.AddAttr("suffix", _value.Suffix);
}
//
void TFrequencyRange::FromXml(const NAmg::NXml::TElement& _xml, TFrequencyRange& _value)
{
   _value = {};

   auto* attr = _xml.FindAttr("label");
   if (nullptr != attr)
   {
      _value.Label = attr->AsTString();
   }

   if (1 == _xml.ElemCount("Description"))
   {
      _value.Description = _xml.ElChild(0, "Description").GetText();
   }

   attr = _xml.FindAttr("suffix");
   if (nullptr != attr)
   {
      _value.Suffix = attr->AsTString();
   }
}
//
void TForbiddenFrequency::ToJson(const TForbiddenFrequency& _value, Json::Value& _json)
{
   _json["freqvalue"] = _value.FreqValue;
   _json["description"] = _value.Description;
}
//
void TForbiddenFrequency::FromJson(const Json::Value& _json, TForbiddenFrequency& _value)
{
   auto members = _json.getMemberNames();
   if (members.end() != std::find(members.begin(), members.end(), "freqvalue"))
   {
      _value.FreqValue = _json["freqvalue"].asString();
   }
   if (members.end() != std::find(members.begin(), members.end(), "description"))
   {
      _value.Description = _json["description"].asString();
   }
}
//
void TForbiddenFrequency::ToXml(const TForbiddenFrequency& _value, NAmg::NXml::TElement& _xml)
{
   _xml.AddAttr("freqvalue", _value.FreqValue);

   auto& descriptionElement = *(new NAmg::NXml::TElement("Description"));
   descriptionElement.AddChild(*(new NAmg::NXml::TText(_value.Description)));
   _xml.AddChild(descriptionElement);
}
//
void TForbiddenFrequency::FromXml(const NAmg::NXml::TElement& _xml, TForbiddenFrequency& _value)
{
   auto* attrFreqValue = _xml.FindAttr("freqvalue");
   if (nullptr != attrFreqValue)
   {
      _value.FreqValue = attrFreqValue->AsTString();
   }
   if (1 == _xml.ElemCount("Description"))
   {
      _value.Description = _xml.ElChild(0, "Description").GetText();
   }
}
//////////////////////////////////////////////////////////////////////////
void TForbiddenFrequencyRange::ToJson(const TForbiddenFrequencyRange& _value, Json::Value& _json)
{
   _json["freqvalmin"] = _value.FreqValueMin;
   _json["freqvalmax"] = _value.FreqValueMax;
   _json["description"] = _value.Description;

}
//
void TForbiddenFrequencyRange::FromJson(const Json::Value& _json, TForbiddenFrequencyRange& _value)
{
   auto members = _json.getMemberNames();
   if (members.end() != std::find(members.begin(), members.end(), "freqvalmin"))
   {
      _value.FreqValueMin = _json["freqvalmin"].asString();
   }
   if (members.end() != std::find(members.begin(), members.end(), "freqvalmax"))
   {
      _value.FreqValueMax = _json["freqvalmax"].asString();
   }
   if (members.end() != std::find(members.begin(), members.end(), "description"))
   {
      _value.Description = _json["description"].asString();
   }
}
//
void TForbiddenFrequencyRange::ToXml(const TForbiddenFrequencyRange& _value, NAmg::NXml::TElement& _xml)
{
   _xml.AddAttr("freqvalmin", _value.FreqValueMin);
   _xml.AddAttr("freqvalmax", _value.FreqValueMax);

   auto& descriptionElement = *(new NAmg::NXml::TElement("Description"));
   descriptionElement.AddChild(*(new NAmg::NXml::TText(_value.Description)));
   _xml.AddChild(descriptionElement);
}
//
void TForbiddenFrequencyRange::FromXml(const NAmg::NXml::TElement& _xml, TForbiddenFrequencyRange& _value)
{
   auto* attrFreqValMin = _xml.FindAttr("freqvalmin");
   auto* attrFreqValMax = _xml.FindAttr("freqvalmax");
   if (nullptr != attrFreqValMin)
   {
      _value.FreqValueMin = attrFreqValMin->AsTString();
   }
   if (nullptr != attrFreqValMax)
   {
      _value.FreqValueMax = attrFreqValMax->AsTString();
   }
   if (1 == _xml.ElemCount("Description"))
   {
      _value.Description = _xml.ElChild(0, "Description").GetText();
   }
}
//
namespace NAmg
{
   namespace NScl
   {
      namespace NText
      {
         template<> TBody::TREF AmgCall toBody<TForbiddenFrequency>(const TForbiddenFrequency& _inValue)
         {
            Json::Value json;
            TForbiddenFrequency::ToJson(_inValue, json);
            return(toBody(json));
         }
         template<> void AmgCall fromBody<TForbiddenFrequency>(const TBody& _body, TForbiddenFrequency& _outValue)
         {
            Json::Value json;
            fromBody(_body, json);
            TForbiddenFrequency::FromJson(json, _outValue);
         }
         template<> TBody::TREF AmgCall toBody<TForbiddenFrequencyRange>(const TForbiddenFrequencyRange& _inValue)
         {
            Json::Value json;
            TForbiddenFrequencyRange::ToJson(_inValue, json);
            return(toBody(json));
         }
         template<> void AmgCall fromBody<TForbiddenFrequencyRange>(const TBody& _body, TForbiddenFrequencyRange& _outValue)
         {
            Json::Value json;
            fromBody(_body, json);
            TForbiddenFrequencyRange::FromJson(json, _outValue);
         }
      }
   }
}