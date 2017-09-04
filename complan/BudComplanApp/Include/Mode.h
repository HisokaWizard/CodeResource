#ifndef __BudComplan_Mode_H__
#define __BudComplan_Mode_H__
//! @file
//! @brief

#include <amgDefs.h>
#include <vector>
#include <Link/TPath.h>

#include <ckiCache.h>
#include <ckiPresetType.h>

#ifdef _MSC_VER
   #pragma pack(push,16)
#endif

namespace NAmg
{
   namespace NXml { class TElement; } 
   namespace NCakApi { class TBud; } 
}
namespace Json { class Value; }

//! @brief
//!   ����� ���������������� �������
//! @details
//!   � �������� ���������������� ���������� ������� ����������� ���������
//!   ������ (�������� ������������) ��������������� ���� �������������.
struct TModeData
{
   std::string Label;
   std::string Description;
   NAmg::relevantObject<uint16_t> Channel;
   NAmg::relevantObject<uint16_t> RxFrequency;
   NAmg::relevantObject<uint16_t> TxFrequency;
   NAmg::relevantObject<std::string> RxClassId;
   NAmg::relevantObject<std::string> TxClassId;
   NAmg::relevantObject<uint16_t> TxPower;
   struct TPresetData
   {
      std::string HostId;
      NAmg::NCakApi::TSpecificPreset Preset;
   };
   using TPresets = std::vector<TPresetData>;
   //! @breif
   //!   �������������, ������������ ��� ������������ ������ �������, � �������� � ��������������� ������������
   //! @details
   //!   ��������������� ������������ ����� �� ������� � ������� � �������� � �������� ����� ����
   //!   ������������, ��� �������� �� ������ �������������.
   TPresets BuildPresets;

   //! @breif
   //!   �������������, ������������ ��� ��������������� (������������) ������ �������, � �������� � ��������������� ������������
   //! @details
   //!   ��������������� ������������ ����� �� ������� � ������� � �������� � �������� ����� ����
   //!   ������������, ��� �������� �� ������ �������������.
   TPresets DestroyPresets;

   static void ToXml(const TModeData&, NAmg::NXml::TElement&);
   static void FromXml(const NAmg::NXml::TElement&, TModeData&);
   static void ToJson(const TModeData&, Json::Value&);
   static void FromJson(const Json::Value&, TModeData&);
};

//! @brief
//!   
//! @details
//!
struct TModeState
{
   enum TPresetState
   {
      Pending,
      Accepted,
      Rejected
   };

   using TPresets = std::vector<TPresetState>;

   TPresets BuildPresets;
   TPresets DestroyPresets;

   //! ����� ��������, ���� ��� ������������� � ��������� TModeState::Pending
   bool AmgCall Completed(void) const;
   bool AmgCall Successfull(void) const;

   static std::string AmgCall PresetStateToStr(TPresetState);
   static TPresetState AmgCall StrToPresetState(const std::string&);

   static void ToJson(const TModeState&, Json::Value&);
   static void FromJson(const Json::Value&, TModeState&);

   static void AmgCall ToCache(NAmg::NCakApi::TBud&, const NAmg::TPath&, const std::string& _serviceId, const TModeState&);
};

#ifdef _MSC_VER
   #pragma pack(pop)
#endif
//
#endif
