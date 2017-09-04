#ifndef __BudComplan_Service_H__
#define __BudComplan_Service_H__
//! @file
//! @brief

#include <amgDefs.h>

#include <forward_list>
#include <unordered_set>

#include <Link/TSipUri.h>
#include <ckiCache.h>
#include "Endpoint.h"
#include "Mode.h"
#include "Frequency.h"

#ifdef _MSC_VER
   #pragma pack(push,16)
#endif

namespace NAmg { namespace NCakApi { class TBud; } }

struct TEquipmentType
{
   std::string DeviceTypeId;
   std::string RoleId;
   bool CanTransmit = true;
   bool CanReceive = true;
   std::vector<std::string> FrequencyRanges; //!< Список идентификаторов TFrequencyRange

   static void ToJson(const TEquipmentType&, Json::Value&);
   static void FromJson(const Json::Value&, TEquipmentType&);

   using TAdapter = NAmg::NCakApi::arrayCacheAdapter<TEquipmentType, TEquipmentType::FromJson>;

   static void ToXml(const TEquipmentType&, NAmg::NXml::TElement&);
   static void FromXml(const NAmg::NXml::TElement&, TEquipmentType&);
};

struct TServiceMediaEnum {
   enum TType { Audio, Telecode, Text, Control };
   const std::map<std::string, TType> Types =
   {
      { "audio", TType::Audio },
      { "telecode", TType::Telecode },
      { "text", TType::Text },
      { "control", TType::Control },
   };
   std::string ToString(TType _type)
   {
      for (auto& item : Types)
      {
         if (item.second == _type)
            return item.first;
      }
      return{};
   }
   TType FromString(const std::string& _str)
   {
      auto it = Types.find(_str);
      if (Types.end() == it)
         return{};
      return (it->second);
   }
};
//! @brief
//!   Конфигурация управляемого сервиса (тракта)
//! @details
//
struct TServiceData
{
   std::string Label;
   //! Текстовое описание сервиса
   std::string Description;
   std::string FrequencyRangeId;
   bool Crypted = false;
   //std::string CryptoId; ???
   TServiceMediaEnum::TType MediaTypeId;
   std::string GraphId;
   NAmg::TSipUri TargetUri;
   NAmg::relevantObject<std::string> EncryptedId;
   std::string SwitchId;
   struct TRadio
   {
      std::string Id, PortId;
   };
   struct TRadioStream
   {
      TRadio Radio;
      std::string AntennaId;
   };
   TRadioStream In, Out;
   bool CanSend;
   bool CanRecv;

   std::map<std::string, TModeData> Modes;
   std::map<std::string, TEndpointData> Endpoints;

   //! Сохранение конфигурации сервиса в JSON
   static void ToJson(const TServiceData&, Json::Value&);
   //! Чтение конфигурации сервиса из JSON
   static void FromJson(const Json::Value&, TServiceData&);

   using TAdapter = NAmg::NCakApi::arrayCacheAdapter<TServiceData, TServiceData::FromJson>;

   //! Сохранение конфигурации сервиса в XML
   static void ToXml(const TServiceData&, NAmg::NXml::TElement&);
   //! Чтение конфигурации сервиса из XML
   static void FromXml(const NAmg::NXml::TElement&, TServiceData&);
};

//! Составное описание текущего действительного статуса сервиса
struct TServiceState
{
   std::string CurrentModeId;
   std::string AwaitedModeId;

   TModeState ModeState;

   std::map<std::string, TEndpointState> Endpoints;
   std::unordered_set<std::string> LockedModes;
   std::string EncryptedId;

   struct Radio 
   {
      std::string Id;      
      std::string PortId;
      std::string AntennaId;
   };
   Radio In;
   Radio Out;
   
   static void ToJson(const TServiceState&, Json::Value&);
   static void FromJson(const Json::Value&, TServiceState&);

   using TAdapter = NAmg::NCakApi::arrayCacheAdapter<TServiceState, TServiceState::FromJson>;

   static void AmgCall RequestToBuild(NAmg::NCakApi::TBud&, TAdapter&, const std::string& _serviceId, const std::string& _modeId);
   static void AmgCall RequestToDestroy(NAmg::NCakApi::TBud&, TAdapter&, const std::string& _serviceId);

   static void AmgCall AwaitedToCache(NAmg::NCakApi::TBud&, TAdapter&, const std::string& _serviceId, const std::string& _awaitedModeId);
   static void AmgCall CurrentToCache(NAmg::NCakApi::TBud&, TAdapter&, const std::string& _serviceId, const std::string& _currentModeId);

   static void AmgCall InitInCache(NAmg::NCakApi::TBud& _bud, TServiceState::TAdapter& _adapter, const std::string& _serviceId);

   static void AmgCall LockingToCache(NAmg::NCakApi::TBud&, TAdapter&, const std::string& _serviceId, std::forward_list<std::string> _ids, bool _doLock);
};

namespace NAmg
{
namespace NScl
{
namespace NText
{
template<> TBody::TREF AmgCall toBody<TServiceData>(const TServiceData& _inValue);
template<> void AmgCall fromBody<TServiceData>(const TBody& _body, TServiceData& _outValue);
}
}
}

#ifdef _MSC_VER
   #pragma pack(pop)
#endif
//
#endif
