#ifndef __BudComplan_ServiceSet_H__
#define __BudComplan_ServiceSet_H__
//! @file
//! @brief

#include <amgDefs.h>
#include <list>
#include <ckiCache.h>

#ifdef _MSC_VER
   #pragma pack(push,16)
#endif

//! @brief
//!   Конфигурация набора сервисов (трактов)
//! @details
//
struct TServiceSetData
{
   std::string Label;
   std::string Description;
   std::string SchemeId;
   std::list<std::string> ServiceIds;

   static void ToJson(const TServiceSetData&, Json::Value&);
   static void FromJson(const Json::Value&, TServiceSetData&);

   using TAdapter = NAmg::NCakApi::arrayCacheAdapter<TServiceSetData, TServiceSetData::FromJson>;

   static void ToXml(const TServiceSetData&, NAmg::NXml::TElement&);
   static void FromXml(const NAmg::NXml::TElement&, TServiceSetData&);
};

//! @brief
//!   Текущий и запрашиваемый пользователем набор сервисов (состояние набора)
//! @details
//
struct TServiceSetState
{
   std::string CurrentSetId;
   std::string AwaitedSetId;

   static void ToJson(const TServiceSetState&, Json::Value&);
   static void FromJson(const Json::Value&, TServiceSetState&);

   using TAdapter = NAmg::NCakApi::sectionCacheAdapter<TServiceSetState, TServiceSetState::FromJson>;

   //! @brief 
   //!   Передача обновления о смене текущего набора сервисов
   //! @details
   //!   Производит отправку обновления с запрашиваемым пользователем набором сервисов (AwaitedSetId)
   //!   через очередь на сервер управления либо применяет обновление при его недоступности.
   static void AmgCall AwaitedToCache(NAmg::NCakApi::TBud& _bud, const TAdapter& _current, const std::string& _toSetId);

   static void AmgCall CurrentToCache(NAmg::NCakApi::TBud& _bud, const TAdapter& _current, const std::string& _toSetId);
};

#ifdef _MSC_VER
   #pragma pack(pop)
#endif
//
#endif
