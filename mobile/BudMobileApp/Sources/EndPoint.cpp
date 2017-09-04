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
#include "EndPoint.h"
//
#ifdef __BORLANDC__
   #pragma option -w -O2 -vi- -b -6 -k -a16 -pc -ff
#endif
//
namespace NAmg
{
   //
   void TEndPointData::ToJson(const TEndPointData& _value, Json::Value& _json)
   {
      _json["user_id"] = _value.UserId;
      _json["mac_address"] = _value.MacAddress;
      _json["label"] = _value.Label;
      _json["description"] = _value.Description;
   }
   //
   void TEndPointData::FromJson(const Json::Value& _json, TEndPointData& _value)
   {
      auto members = _json.getMemberNames();
      if (members.end() != std::find(members.begin(), members.end(), "user_id"))
      {
         _value.UserId = _json["user_id"].asString();
      }
      if (members.end() != std::find(members.begin(), members.end(), "mac_address"))
      {
         _value.MacAddress = _json["mac_address"].asString();
      }
      if (members.end() != std::find(members.begin(), members.end(), "label"))
      {
         _value.Label = _json["label"].asString();
      }
      if (members.end() != std::find(members.begin(), members.end(), "description"))
      {
         _value.Description = _json["description"].asString();
      }
   }
   //
   void TEndPointData::ToXml(const TEndPointData& _value, NAmg::NXml::TElement& _xml)
   {
      _xml.AddAttr("user_id", _value.UserId);
      _xml.AddAttr("mac_address", _value.MacAddress);
      _xml.AddAttr("label", _value.Label);
   }
   //
   void TEndPointData::FromXml(const NAmg::NXml::TElement& _xml, TEndPointData& _value)
   {
      auto* attrMacAddress = _xml.FindAttr("mac_address");
      auto* attrUserId = _xml.FindAttr("user_id");
      auto* attrLabel = _xml.FindAttr("label");
      if (nullptr != attrUserId)
      {
         _value.UserId = attrUserId->AsTString();
      }
      if (nullptr != attrMacAddress)
      {
         _value.MacAddress = attrMacAddress->AsTString();
      }
      if (nullptr != attrLabel)
      {
         _value.Label = attrLabel->AsTString();
      }
   }
   //
   bool AmgCall TEndPointState::operator==(const TEndPointState& _state) const
   {
      return(!operator!=(_state));
   }
   //
   bool AmgCall TEndPointState::operator!=(const TEndPointState& _state) const
   {
      if ((Acceleration == _state.Acceleration)
       && (Account == _state.Account)
       && (Blood == _state.Blood)
       && (Bp == _state.Bp)
       && (LocationId == _state.LocationId)
       && (UserId == _state.UserId)
       && (Pulse == _state.Pulse)
       && (Speed_High == _state.Speed_High)
       && (Speed_High == _state.Speed_High)
       && (Speed_Low == _state.Speed_Low)
       && (Temperature == _state.Temperature)
       && (Time.Hr == _state.Time.Hr)
       && (Time.Min == _state.Time.Min)
       && (Time.Sec == _state.Time.Sec)
       && (Status == _state.Status)
       && (SMREnabled == _state.SMREnabled)
       && (SMRDeviceEnabled == _state.SMRDeviceEnabled)
       && (MultigraphEnabled == _state.MultigraphEnabled)
       && (NeedAttention == _state.NeedAttention))
         return false;
      else
         return true;
   }
   //
   void TEndPointState::ToJson(const TEndPointState& _value, Json::Value& _json)
   {
      _json["speed_high"] = _value.Speed_High;
      _json["speed_low"] = _value.Speed_Low;
      _json["accel"] = _value.Acceleration;
      _json["location"] = _value.LocationId;
      _json["user_id"] = _value.UserId;
      _json["time_h"] = _value.Time.Hr;
      _json["time_m"] = _value.Time.Min;
      _json["time_s"] = _value.Time.Sec;
      _json["status"] = _value.Status;
      _json["account"] = _value.Account;
      _json["blood"] = _value.Blood;
      _json["bp"] = _value.Bp;
      _json["temperature"] = _value.Temperature;
      _json["pulse"] = _value.Pulse;
      _json["need_attention"] = _value.NeedAttention;
      _json["smrdevice_enabled"] = _value.SMRDeviceEnabled;
      _json["smr_enabled"] = _value.SMREnabled;
      _json["multigraph_enabled"] = _value.MultigraphEnabled;
   }
   //
   void TEndPointState::FromJson(const Json::Value& _json, TEndPointState& _value)
   {
      auto members = _json.getMemberNames();
      if (members.end() != std::find(members.begin(), members.end(), "user_id"))
      {
         _value.UserId = _json["user_id"].asString();
      }
      if (members.end() != std::find(members.begin(), members.end(), "location"))
      {
         _value.LocationId = _json["location"].asString();
      }
      if (members.end() != std::find(members.begin(), members.end(), "speed_high"))
      {
         _value.Speed_High = _json["speed_high"].asString();
      }
      if (members.end() != std::find(members.begin(), members.end(), "speed_low"))
      {
         _value.Speed_Low = _json["speed_low"].asString();
      }
      if (members.end() != std::find(members.begin(), members.end(), "accel"))
      {
         _value.Acceleration = _json["accel"].asString();
      }
      if (members.end() != std::find(members.begin(), members.end(), "time_h"))
      {
         _value.Time.Hr = _json["time_h"].asString();
      }
      if (members.end() != std::find(members.begin(), members.end(), "time_m"))
      {
         _value.Time.Min = _json["time_m"].asString();
      }
      if (members.end() != std::find(members.begin(), members.end(), "time_s"))
      {
         _value.Time.Sec = _json["time_s"].asString();
      }
      if (members.end() != std::find(members.begin(), members.end(), "status"))
      {
         _value.Status = _json["status"].asString();
      }
      if (members.end() != std::find(members.begin(), members.end(), "account"))
      {
         _value.Account = _json["account"].asString();
      }
      if (members.end() != std::find(members.begin(), members.end(), "blood"))
      {
         _value.Blood = _json["blood"].asInt();
      }
      if (members.end() != std::find(members.begin(), members.end(), "bp"))
      {
         _value.Bp = _json["bp"].asInt();
      }
      if (members.end() != std::find(members.begin(), members.end(), "temperature"))
      {
         _value.Temperature = _json["temperature"].asInt();
      }
      if (members.end() != std::find(members.begin(), members.end(), "pulse"))
      {
         _value.Pulse = _json["pulse"].asInt();
      }
      if (members.end() != std::find(members.begin(), members.end(), "need_attention"))
      {
         _value.NeedAttention = _json["need_attention"].asBool();
      }
      if (members.end() != std::find(members.begin(), members.end(), "smrdevice_enabled"))
      {
         _value.SMRDeviceEnabled = _json["smrdevice_enabled"].asBool();
      }
      if (members.end() != std::find(members.begin(), members.end(), "smr_enabled"))
      {
         _value.SMREnabled = _json["smr_enabled"].asBool();
      }
      if (members.end() != std::find(members.begin(), members.end(), "multigraph_enabled"))
      {
         _value.MultigraphEnabled = _json["multigraph_enabled"].asBool();
      }
   }
   //
   void AmgCall TEndPointState::FromSMR(const NAmg::NXml::TElement& _xml, TEndPointState::TAdapter::TArray& _value)
   {
      auto& attr = _xml.ElChilds("User");
      try
      {
         for (TVectorIndex i = 0, q = attr.Count(); q > i; ++i)
         {
            TEndPointState tmpState;
            tmpState.Temperature = 0;
            tmpState.Bp = 0;
            tmpState.Blood = 0;
            tmpState.MultigraphEnabled = false;
            tmpState.SMRDeviceEnabled = false;
            tmpState.SMREnabled = false;
            tmpState.NeedAttention = false;
            auto* attrLocation = attr.Item(i).FindAttr("location");
            tmpState.LocationId = attrLocation->AsTString();
            auto* attrPulse = attr.Item(i).FindAttr("pulse");
            tmpState.Pulse = attrPulse->AsTString().ToUInt();
            auto* attrSpeedH = attr.Item(i).FindAttr("speed_high");
            tmpState.Speed_High = attrSpeedH->AsTString();
            auto* attrSpeedL = attr.Item(i).FindAttr("speed_low");
            tmpState.Speed_Low = attrSpeedL->AsTString();
            auto* attrAccel = attr.Item(i).FindAttr("accel");
            tmpState.Acceleration = attrAccel->AsTString();
            auto* attrTimeH = attr.Item(i).FindAttr("time_h");
            tmpState.Time.Hr = attrTimeH->AsTString();
            auto* attrTimeM = attr.Item(i).FindAttr("time_m");
            tmpState.Time.Min = attrTimeM->AsTString();
            auto* attrTimeS = attr.Item(i).FindAttr("time_s");
            tmpState.Time.Sec = attrTimeS->AsTString();
            auto* attrStatus = attr.Item(i).FindAttr("status");
            tmpState.Status = attrStatus->AsTString();
            auto* attrId = attr.Item(i).FindAttr("id");
            _value[attrId->AsTString()] = tmpState;
         }
      }
      catch (const NXml::TErrors::TREF&)
      {
      }
   }
   //
   std::string AmgCall TEndPointState::FromMultiGraph(const TRequestData& _data, TEndPointState::TAdapter::TArray& _userstate, TEndPointData::TAdapter::TArray& _userdata)
   {
      for (auto& userdata : _userdata)
      {
         if (userdata.second.UserId == _data.Headers["Account"])
         {
            TEndPointState user;
            user.UserId = userdata.second.UserId;
            user.LocationId = _userstate[userdata.first].LocationId;
            user.Speed_High = _userstate[userdata.first].Speed_High;
            user.Speed_Low = _userstate[userdata.first].Speed_Low;
            user.Time = _userstate[userdata.first].Time;
            user.Acceleration = _userstate[userdata.first].Acceleration;
            user.Status = _userstate[userdata.first].Status;
            if (_data.Headers.KeyExists("Account"))
               user.Account = _data.Headers["Account"];
            if (_data.Headers.KeyExists("Pulse"))
            {
               TAString pulse = _data.Headers["Pulse"];
               long double _pulse = pulse.ToFloat();
               user.Pulse = (unsigned int)_pulse;
            }
            if (_data.Headers.KeyExists("Blood"))
            {
               TAString blood = _data.Headers["Blood"];
               long double _blood = blood.ToFloat();
               user.Blood = (unsigned int)_blood;
            }
            if (_data.Headers.KeyExists("Bp"))
            {
               TAString bp = _data.Headers["Bp"];
               long double _bp = bp.ToFloat();
               user.Bp = (unsigned int)_bp;
            }
            if (_data.Headers.KeyExists("Temperature"))
            {
               TAString temperature = _data.Headers["Temperature"];
               long double _temperature = temperature.ToFloat();
               user.Temperature = (unsigned int)_temperature;
            }
            _userstate[userdata.first] = user;
            return userdata.first;
         }
      }
      return "";
   }
   //
   void AmgCall TEndPointState::StartInit(TEndPointState::TAdapter::TArray& _endpointstate, TEndPointData::TAdapter::TArray& _endpointdata)
   {
      for (auto& endpointdata : _endpointdata)
      {
         TEndPointState endpoint;
         endpoint.UserId = endpointdata.second.UserId;
         endpoint.Acceleration = "-";
         endpoint.Account = "-";
         endpoint.Blood = 0;
         endpoint.Bp = 0;
         endpoint.SMRDeviceEnabled = true;
         endpoint.SMREnabled = true;
         endpoint.MultigraphEnabled = false;
         endpoint.NeedAttention = false;
         endpoint.Pulse = 0;
         endpoint.Speed_High = "-";
         endpoint.Speed_Low = "-";
         endpoint.Status = "-";
         endpoint.Temperature = 0;
         endpoint.Time.Hr = "00";
         endpoint.Time.Min = "00";
         endpoint.Time.Sec = "00";
         _endpointstate[endpointdata.first] = endpoint;
      }
   }
   //
   void TSmrSource::ToJson(const TSmrSource& _value, Json::Value& _json)
   {
      _json["type"] = _value.Type;
      _json["uri"] = _value.Uri.ToString();
      _json["interval"] = _value.Interval;
      _json["web_port"] = _value.WebPort;
      if (!_value.Translations.empty())
      {
         for (unsigned int i = 0, q = (unsigned int)_value.Translations.size(); q > i; ++i)
         {
            auto& translate = _json["Translate"];
            translate[i]["data_id"] = _value.Translations[i].DataId;
            translate[i]["endpoint_id"] = _value.Translations[i].EndPointId;
            translate[i]["ignore_pulse"] = _value.Translations[i].Ignore_Pulse;
         }
      }
   }
   //
   void TSmrSource::FromJson(const Json::Value& _json, TSmrSource& _value)
   {
      auto members = _json.getMemberNames();
      if (members.end() != std::find(members.begin(), members.end(), "type"))
      {
         _value.Type = _json["type"].asString();
      }
      if (members.end() != std::find(members.begin(), members.end(), "uri"))
      {
         THttpUri uri;
         uri.FromString(_json["uri"].asString());
         _value.Uri = uri;
      }
      if (members.end() != std::find(members.begin(), members.end(), "interval"))
      {
         _value.Interval = _json["interval"].asInt();
      }
      if (members.end() != std::find(members.begin(), members.end(), "web_port"))
      {
         _value.WebPort = (int16_t)_json["web_port"].asUInt();
      }
      if (members.end() != std::find(members.begin(), members.end(), "Translate"))
      {
         auto& jsonElement = _json["Translate"];
         for (unsigned int i = 0, q = (unsigned int)jsonElement.size(); q > i; ++i)
         {
            TTranslate translate;
            translate.DataId = jsonElement[i]["data_id"].asString();
            translate.EndPointId = jsonElement[i]["endpoint_id"].asString();
            translate.Ignore_Pulse = jsonElement[i]["ignore_pulse"].asBool();
            _value.Translations.push_back(translate);
         }
      }
   }
   //
   void TSmrSource::ToXml(const TSmrSource& _value, NAmg::NXml::TElement& _xml)
   {
      _xml.AddAttr("type", _value.Type);
      _xml.AddAttr("uri", _value.Uri.ToString());
      _xml.AddAttr("interval", TAString(_value.Interval));
      _xml.AddAttr("web_port", TAString(_value.WebPort));
      for (TVectorIndex i = 0, q = _value.Translations.size(); q > i; ++i)
      {
         auto& item = _xml.ElChild(i, "Translate");
         item.AddAttr("data_id", _value.Translations.at(i).DataId);
         item.AddAttr("endpoint_id", _value.Translations.at(i).EndPointId);
         item.AddAttr("ignore_pulse", TAString(_value.Translations.at(i).Ignore_Pulse));
      }
   }
   //
   void TSmrSource::FromXml(const NAmg::NXml::TElement& _xml, TSmrSource& _value)
   {
      auto* attrType = _xml.FindAttr("type");
      if (nullptr != attrType)
      {
         _value.Type = attrType->AsTString();
         auto* attrUri = _xml.FindAttr("uri");
         auto* attrInterval = _xml.FindAttr("interval");
         auto* attrWPort = _xml.FindAttr("web_port");
         if ((nullptr != attrUri) && (nullptr != attrInterval) && (nullptr != attrWPort))
         {
            _value.Uri = attrUri->AsTString();
            _value.Interval = attrInterval->AsInteger();
            _value.WebPort = (int16_t)attrWPort->AsInteger();
         }
      }
      if (0 < _xml.ElemCount("Translate"))
      {
         for (TVectorIndex i = 0, q = _xml.ElemCount("Translate"); q > i; i++)
         {
            auto& item = _xml.ElChild(i, "Translate");
            auto* attrDataId = item.FindAttr("data_id");
            auto* attrEndPointId = item.FindAttr("endpoint_id");
            auto* attrIgnorePulse = item.FindAttr("ignore_pulse");
            if ((nullptr != attrDataId) && (nullptr != attrEndPointId))
            {
               TTranslate translate;
               translate.DataId = attrDataId->AsTString();
               translate.EndPointId = attrEndPointId->AsTString();
               if (nullptr != attrIgnorePulse)
                  translate.Ignore_Pulse = attrIgnorePulse->AsTString().ToBool();
               _value.Translations.push_back(translate);
            }
         }
      }
   }
   //
   namespace NScl
   {
      namespace NText
      {
         template<> TBody::TREF AmgCall toBody<TEndPointData>(const TEndPointData& _inValue)
         {
            Json::Value json;
            TEndPointData::ToJson(_inValue, json);
            return(toBody(json));
         }
         template<> void AmgCall fromBody<TEndPointData>(const TBody& _body, TEndPointData& _outValue)
         {
            Json::Value json;
            fromBody(_body, json);
            TEndPointData::FromJson(json, _outValue);
         }
         template<> TBody::TREF AmgCall toBody<TSmrSource>(const TSmrSource& _inValue)
         {
            Json::Value json;
            TSmrSource::ToJson(_inValue, json);
            return(toBody(json));
         }
         template<> void AmgCall fromBody<TSmrSource>(const TBody& _body, TSmrSource& _outValue)
         {
            Json::Value json;
            fromBody(_body, json);
            TSmrSource::FromJson(json, _outValue);
         }
      }
   }
//
}