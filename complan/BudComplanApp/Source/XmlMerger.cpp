//! @file
//! @brief Реализация программного класса NAmg::SimpleXmlMerger.
#include "precomp.h"
//
#include "XmlMerger.h"
#include <XML/amgXmlHandle.h>
#include <XML/amgXmlPrinter.h>
#include <XML/amgXmlDocument.h>
#include <vector>
//
#ifdef _MSC_VER
   #pragma pack(16)
#endif
//
namespace NAmg
{

   AmgCreate SimpleXmlMerger::SimpleXmlMerger()
      : m_target( NULL )
   {}

   AmgDestroy SimpleXmlMerger::~SimpleXmlMerger()
   {
      if(m_target) delete m_target;
   }

   void AmgCall SimpleXmlMerger::SetTarget(
      const std::string& from, NXml::TEncoding encoding)
   {
      if(m_target) delete m_target;
      m_target = new NXml::TDocument("target");
      m_target->Parse(from.c_str(), NULL, encoding);
   }

   void AmgCall SimpleXmlMerger::Merge(const NXml::TNode& with)
   {
      if(m_target)
      {
         if(m_target->Type() == with.Type())
         {
            throw "";//слияние документов пока не реализовано
         }
         else if(with.Type() == NXml::TNode::ELEMENT)
         {
            NXml::THandle hndl = m_target->FirstElChild(with.Value());
            if(!hndl.IsEmpty())
            {
               MergeElements(hndl.AsElement(), with.AsElement());
            }
         }
         else
         {
            //просто запихиваем в конец документа
            NXml::TNode& clone = *(with.Clone());
            m_target->AddChild(clone);
         }
      }
   }

   NXml::TNode* AmgCall SimpleXmlMerger::GetResult()
   {
      NXml::TNode& ret_val = m_target->Childs().Extract(0);
      return &ret_val;
   }

   NXml::TDocument* AmgCall SimpleXmlMerger::GetFullResult()
   {
      NXml::TDocument* ret_val = m_target;
      m_target = NULL;
      return ret_val;
   }

   void AmgCall SimpleXmlMerger::GetStrResult(std::string& result)const
   {
      if(m_target)
      {
         result.clear();
         NXml::TPrinter printer;
         printer.SetStreamPrinting();
         m_target->Accept( &printer );
         result = printer.CStr();
      }
   }

void AmgCall SimpleXmlMerger::MergeElements(
   NXml::TElement& target, const NXml::TElement& with)
{
   assert(target.Value()==with.Value());
   if( target.HasText() && target.ElChilds().Count()==0
       && with.HasText() && with.ElChilds().Count()==0 )
   {
      target.SetText(with.GetText());
   }
   else
   {
      NXml::TElemSet& targetChilds = target.ElChilds();
      const NXml::TNodeSet& withChilds = with.Childs();
      std::vector<char> merged(withChilds.Count(), 0);

      //Убираем из target элементы, которые встречаются больше раза в with
      //или в самом target.
      int current = 0;
      while(current < targetChilds.Count())
      {
         std::string cur_name(targetChilds[current].Value().c_str());
         if( target.ElChilds(cur_name).Count() > 1 ||
             with.ElChilds(cur_name).Count() > 1 )
            target.ElChilds(cur_name).Clear();
         else
            current++;
      }

      //Делаем Мердж() для элементов, которые встречаются по одному разу.
      for(TVectorIndex i=0, n = targetChilds.Count(); i<n; i++)
      {
         NXml::THandle hndl = with.FirstElChild(targetChilds[i].Value());
         if(!hndl.IsEmpty())
         {
            merged[withChilds.IndexOf(hndl.AsElement())] = 1;
            MergeElements(targetChilds[i], hndl.AsElement());
         }
      }
      //Добавляем в конец target узлы из with, которых не оказалось в target
      for(TVectorIndex i=0, n=withChilds.Count(); i<n; i++)
      {
         if(merged[i] == 0)
         {
            NXml::TNode& clone = *(withChilds[i].Clone());
            target.AddChild(clone);
         }
      }
   }
}

} //namespace NAmg
