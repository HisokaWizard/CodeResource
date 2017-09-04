#ifndef  Cacos_XmlMerger_H
#define Cacos_XmlMerger_H
//! @file
//! @brief Объявление класса NAmg::SimpleXmlMerger.
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
   //! @brief Накладывает xml на m_target.
   void AmgCall Merge(const NXml::TNode& with);
   //! @brief Возвращает первый узел результата.
   //! @detatils Например, в результате у нас получился xml:
   //! <a><c/></a>text<b /> - в этом случае мы получим <a><c/></a>.
   NXml::TNode* AmgCall GetResult();
   //! @brief Возвращает элемент документ со всеми все узлами результата.
   //! @detatils Например, в если в результате слияния получили xml
   //! <a><c/></a>text<b />, то получим документ со всеми узлами результата.
   NXml::TDocument* AmgCall GetFullResult();
   //! @brief Преобразует результат слиния в строку и помещает ее в result.
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
