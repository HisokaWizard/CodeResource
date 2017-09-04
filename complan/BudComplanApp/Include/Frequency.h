#ifndef __BudComplan_Frequency_H__
#define __BudComplan_Frequency_H__
//
//! @file
//! @brief Программные интерфейсы модулей базового приложения
//!   и менеджера службы присутствия.
//
#include <HTTP/sclHttpQueue.h>
#include <HTTP/sclHttpServerService.h>
#include <HTTP/sclHttpFolderControl.h>
#include <Threads/TLockableObject.h>
#include <amgSmartObject.h>
#include <TListOfPointers.h>
#include <XML/amgXmlDocument.h>
#include <Text/sclTextBody.h>
#include <ckiCache.h>

//
#ifdef __BORLANDC__
#pragma option push -w -O2 -vi- -b -6 -k -a16 -pc -ff
#endif
//
#ifdef _MSC_VER
#pragma pack(push,16)
#endif
//
namespace NAmg {
   namespace NXml { class TElement; }
   namespace NCakApi { class TBud; }
}
namespace Json { class Value; }
//
struct TEmissionClass
{
   TEmissionClass()
   :Label(),
   Description()
   {}

   std::string Label;
   std::string Description;
   static void ToJson(const TEmissionClass&, Json::Value&);
   static void FromJson(const Json::Value&, TEmissionClass&);

   using TAdapter = NAmg::NCakApi::arrayCacheAdapter<TEmissionClass, TEmissionClass::FromJson>;

   static void ToXml(const TEmissionClass&, NAmg::NXml::TElement&);
   static void FromXml(const NAmg::NXml::TElement&, TEmissionClass&);
};
//
struct TFrequencyRange
{
   AmgCreate TFrequencyRange()
   :Label(),
   Description(),
   Suffix(){}

   std::string Label;
   std::string Description;
   std::string Suffix;

   static void ToJson(const TFrequencyRange&, Json::Value&);
   static void FromJson(const Json::Value&, TFrequencyRange&);

   using TAdapter = NAmg::NCakApi::arrayCacheAdapter<TFrequencyRange, TFrequencyRange::FromJson>;

   static void ToXml(const TFrequencyRange&, NAmg::NXml::TElement&);
   static void FromXml(const NAmg::NXml::TElement&, TFrequencyRange&);
};
//
struct TForbiddenFrequency
{
   std::string FreqValue;
   std::string Description;
   //
   static void ToJson(const TForbiddenFrequency&, Json::Value&);
   static void FromJson(const Json::Value&, TForbiddenFrequency&);
   //
   using TAdapter = NAmg::NCakApi::arrayCacheAdapter<TForbiddenFrequency, TForbiddenFrequency::FromJson>;
   //
   static void ToXml(const TForbiddenFrequency&, NAmg::NXml::TElement&);
   static void FromXml(const NAmg::NXml::TElement&, TForbiddenFrequency&);
};
//
struct TForbiddenFrequencyRange
{
   std::string FreqValueMin;
   std::string FreqValueMax;
   std::string Description;
   //
   static void ToJson(const TForbiddenFrequencyRange&, Json::Value&);
   static void FromJson(const Json::Value&, TForbiddenFrequencyRange&);
   //
   using TAdapter = NAmg::NCakApi::arrayCacheAdapter<TForbiddenFrequencyRange, TForbiddenFrequencyRange::FromJson>;
   //
   static void ToXml(const TForbiddenFrequencyRange&, NAmg::NXml::TElement&);
   static void FromXml(const NAmg::NXml::TElement&, TForbiddenFrequencyRange&);
};
//
namespace NAmg
{
   namespace NScl
   {
      namespace NText
      {
         template<> TBody::TREF AmgCall toBody<TForbiddenFrequency>(const TForbiddenFrequency& _inValue);
         template<> void AmgCall fromBody<TForbiddenFrequency>(const TBody& _body, TForbiddenFrequency& _outValue);
      }
      namespace NText
      {
         template<> TBody::TREF AmgCall toBody<TForbiddenFrequencyRange>(const TForbiddenFrequencyRange& _inValue);
         template<> void AmgCall fromBody<TForbiddenFrequencyRange>(const TBody& _body, TForbiddenFrequencyRange& _outValue);
      }
   }
}
//
#ifdef _MSC_VER
#pragma pack(pop)
#endif
//
#ifdef __BORLANDC__
#pragma option pop
#endif
//
#endif 