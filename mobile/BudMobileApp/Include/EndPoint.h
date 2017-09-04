#ifndef __BudMobile_EndPoint_H__
#define __BudMobile_EndPoint_H__
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
struct TEndPointData
{
   std::string MacAddress;
   std::string UserId;
   std::string Label;
   std::string Description;
   //
   static void ToJson(const TEndPointData&, Json::Value&);
   static void FromJson(const Json::Value&, TEndPointData&);
   //
   using TAdapter = NAmg::NCakApi::arrayCacheAdapter<TEndPointData, TEndPointData::FromJson>;
   //
   static void ToXml(const TEndPointData&, NAmg::NXml::TElement&);
   static void FromXml(const NAmg::NXml::TElement&, TEndPointData&);
};
//
struct TEndPointState
{
   struct TTime
   {
      std::string Hr;
      std::string Min;
      std::string Sec;
   };
   // SMR
   std::string UserId; // может меняться взависимости от того кто взял данный тмр
   std::string LocationId;
   std::string Speed_High;
   std::string Speed_Low;
   std::string Acceleration;
   TTime Time;
   std::string Status;
   // Multigraph
   std::string Account;
   unsigned int Blood;
   unsigned int Bp;
   unsigned int Temperature;
   // overall
   unsigned int Pulse;
   //
   // flag to indication
   bool NeedAttention;
   bool SMREnabled;
   bool SMRDeviceEnabled;
   bool MultigraphEnabled;
   //
   bool AmgCall operator==(const TEndPointState& smr) const;
   bool AmgCall operator!=(const TEndPointState& smr) const;
   //
   static void ToJson(const TEndPointState&, Json::Value&);
   static void FromJson(const Json::Value&, TEndPointState&);
   //
   using TAdapter = NAmg::NCakApi::arrayCacheAdapter<TEndPointState, TEndPointState::FromJson>;
   //
   static void AmgCall FromSMR(const NAmg::NXml::TElement&, TEndPointState::TAdapter::TArray&);
   static std::string AmgCall FromMultiGraph(const TRequestData& _data, TEndPointState::TAdapter::TArray&, TEndPointData::TAdapter::TArray&);
   static void AmgCall StartInit(TEndPointState::TAdapter::TArray&, TEndPointData::TAdapter::TArray&);
};
//
struct TDataSource
{
   TAString Type;
};
//
struct TSmrSource : public TDataSource
{
   THttpUri Uri;
   unsigned int Interval = 2000;
   Word WebPort = 9000;
   //
   static void ToJson(const TSmrSource&, Json::Value&);
   static void FromJson(const Json::Value&, TSmrSource&);
   //
   using TAdapter = NAmg::NCakApi::arrayCacheAdapter<TSmrSource, TSmrSource::FromJson>;
   //
   static void ToXml(const TSmrSource&, NAmg::NXml::TElement&);
   static void FromXml(const NAmg::NXml::TElement&, TSmrSource&);
   //
   struct TTranslate
   {
      std::string DataId;
      std::string EndPointId;
      bool Ignore_Pulse = false;
   };
   std::vector<TTranslate> Translations;
};
//
namespace NScl
{
   namespace NText
   {
      template<> TBody::TREF AmgCall toBody<TEndPointData>(const TEndPointData& _inValue);
      template<> void AmgCall fromBody<TEndPointData>(const TBody& _body, TEndPointData& _outValue);

      template<> TBody::TREF AmgCall toBody<TSmrSource>(const TSmrSource& _inValue);
      template<> void AmgCall fromBody<TSmrSource>(const TBody& _body, TSmrSource& _outValue);
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
