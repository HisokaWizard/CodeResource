#ifndef __BudComplan_Types_H__
#define __BudComplan_Types_H__
//! @file
//! @brief

#include <amgDefs.h>
#include <ckiCache.h>

#ifdef _MSC_VER
   #pragma pack(push,16)
#endif

enum class TRole
{
   Endpoint,
   Encryptor,
   Antenna,
   Radio
};

//! @brief
//!   Расширение описания типов устройств
//! @details
//!   Идентификатор в списке соответствет идентификатору 
//!   в списке типов устройств какоса (NCakApi::TDeviceType::TAdapter).
struct TExtraDeviceType
{
   std::string Label;
   std::string Description;
   TRole Role;
   bool CanSend = true;
   bool CanReceive = true;

   //! @brief Идентификатор компонента внутри устройства
   //! @details
   //!   Для которого применяются предустановки.
   //!   Если не задан (пустой), то устройство в трактах не управляется.
   std::string ComponentId; 

   //! @brief Список идентификаторов поддерживаемых диапазонов (TFrequencyRange)
   std::list<std::string> RangeIds;
   //! @brief Список поддерживаемых медиа типов трактов 
   std::list<std::string> MediaTypes;

   static void ToJson(const TExtraDeviceType&, Json::Value&);
   static void FromJson(const Json::Value&, TExtraDeviceType&);

   using TAdapter = NAmg::NCakApi::arrayCacheAdapter<TExtraDeviceType, TExtraDeviceType::FromJson>;

   static void ToXml(const TExtraDeviceType&, NAmg::NXml::TElement&);
   static void FromXml(const NAmg::NXml::TElement&, TExtraDeviceType&);
};

#ifdef _MSC_VER
   #pragma pack(pop)
#endif
//
#endif
