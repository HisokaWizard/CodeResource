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
//!   ���������� �������� ����� ���������
//! @details
//!   ������������� � ������ ������������ �������������� 
//!   � ������ ����� ��������� ������ (NCakApi::TDeviceType::TAdapter).
struct TExtraDeviceType
{
   std::string Label;
   std::string Description;
   TRole Role;
   bool CanSend = true;
   bool CanReceive = true;

   //! @brief ������������� ���������� ������ ����������
   //! @details
   //!   ��� �������� ����������� �������������.
   //!   ���� �� ����� (������), �� ���������� � ������� �� �����������.
   std::string ComponentId; 

   //! @brief ������ ��������������� �������������� ���������� (TFrequencyRange)
   std::list<std::string> RangeIds;
   //! @brief ������ �������������� ����� ����� ������� 
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
