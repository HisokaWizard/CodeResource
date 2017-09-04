#ifndef __BudMobile_Group_H__
#define __BudMobile_Group_H__
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
namespace NAmg
{
//
using namespace NScl::NHttp;
//
struct TBioGroup
{
   std::string Label;
   std::string Description;
   unsigned int MinPulse;
   unsigned int MaxPulse;
   unsigned int MinTemperature;
   unsigned int MaxTemperature;
   unsigned int MinBp;
   unsigned int MaxBp;
   unsigned int MinBlood;
   unsigned int MaxBlood;

   static void ToJson(const TBioGroup&, Json::Value&);
   static void FromJson(const Json::Value&, TBioGroup&);
   //
   using TAdapter = NAmg::NCakApi::arrayCacheAdapter<TBioGroup, TBioGroup::FromJson>;
   //
   static void ToXml(const TBioGroup&, NAmg::NXml::TElement&);
   static void FromXml(const NAmg::NXml::TElement&, TBioGroup&);
};
//
namespace NScl
{
   namespace NText
   {
      template<> TBody::TREF AmgCall toBody<TBioGroup>(const TBioGroup& _inValue);
      template<> void AmgCall fromBody<TBioGroup>(const TBody& _body, TBioGroup& _outValue);
   }
}
//
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
