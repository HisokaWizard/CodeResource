#ifndef  Cacos_XmlMerger_H
#define Cacos_XmlMerger_H
//! @file
//! @brief ���������� ������ NAmg::SimpleXmlMerger.
#include "amgDefs.h"
#include <XML/amgXmlElement.h>
//
#ifdef _MSC_VER
   #pragma pack(push,16)
#endif
//
//

namespace NAmg
{
namespace NXml{ class TDocument; }
class SimpleXmlMerger
{
public:
   AmgCreate SimpleXmlMerger();
   virtual AmgDestroy ~SimpleXmlMerger();
   void AmgCall SetTarget(const std::string& from,
      NXml::TEncoding encoding = NXml::ENCODING_UNKNOWN);
   //! @brief ����������� xml �� m_target.
   void AmgCall Merge(const NXml::TNode& with);
   //! @brief ���������� ������ ���� ����������.
   //! @detatils ��������, � ���������� � ��� ��������� xml:
   //! <a><c/></a>text<b /> - � ���� ������ �� ������� <a><c/></a>.
   NXml::TNode* AmgCall GetResult();
   //! @brief ���������� ������� �������� �� ����� ��� ������ ����������.
   //! @detatils ��������, � ���� � ���������� ������� �������� xml
   //! <a><c/></a>text<b />, �� ������� �������� �� ����� ������ ����������.
   NXml::TDocument* AmgCall GetFullResult();
   //! @brief ����������� ��������� ������ � ������ � �������� �� � result.
   void AmgCall GetStrResult(std::string& result)const;
private:
   void AmgCall MergeElements(NXml::TElement& target, const NXml::TElement& with);
   NXml::TDocument* m_target;
};

}// namespace NAmg

#ifdef _MSC_VER
   #pragma pack(pop)
#endif

#endif // Cacos_XmlMerger_H
