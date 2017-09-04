//! @file
//! @brief Реализация программного интерфейса модуля конечного приложения,
//!   основанного на "служебном" приложении.
#include "precomp.h"

#include <JsonCpp/json.h>
#include <amgFile.h>
#include <Debug/amgDebug.h>
#include <HTTP/sclHttpFolderControl.h>

#include "Application.h"

#ifdef _MSC_VER
#pragma pack(16)
#endif

using namespace NAmg;
using namespace NAmg::NScl;

AmgCreate TAppData::TAppData()
   : ClientService()
   , PresetManager()
   , BudParameters()
   , Bud("cp")
   , Hosts(Bud.Cache(),                   "/hosts")
   , DeviceData(Bud.Cache(),              "/devices/data")
   , DeviceState(Bud.Cache(),             "/devices/state")
   , DeviceTypes(Bud.Cache(),             "/devices/types")
   , ExtraDeviceTypes(Bud.Cache(),        "/bud/cp/types") 
   , MediaTypes(Bud.Cache(),              "/media")
   , ServiceSetDatas(Bud.Cache(),         "/bud/cp/sets/data")
   , ServiceSetState(Bud.Cache(),         "/bud/cp/sets/state")
   , ServiceDatas(Bud.Cache(),            "/bud/cp/services/data")
   , ServiceStates(Bud.Cache(),           "/bud/cp/services/state")
   , FrequencyRanges(Bud.Cache(),         "/bud/cp/ranges")
   , Subscriptions()//(Bud.Cache(),       "/bud/cp/subscriptions")
   , ForbiddenFrequencyRange(Bud.Cache(), "/bud/cp/forbiddenfreqranges")
   , ForbiddenFrequency(Bud.Cache(),      "/bud/cp/forbiddenfrequency")
   , Schemes(Bud.Cache(),                 "/bud/cp/schemes")
   , EmissionClasses(Bud.Cache(),         "/bud/cp/classes")
{
   BudParameters.Modules.Modules["complan"].Path = "app/bud/cp/complan.js";
}

AmgDestroy TAppData::~TAppData()
{
}

void AmgCall TAppData::LoadFromFile(TAppData& _app, const NAmg::TPath& _fileName)
{
   NSmart::TDiskFileReader::TREF reader(NSmart::TDiskFileReader::Create(_fileName));
   NAmg::NXml::TDocument doc;
   doc.LoadFile(reader);

   //Из кэша???
   //_app.Hosts
   //_app.MediaTypes
   //_app.ActiveServiceSet

   try
   {
      TEmissionClass::TAdapter::TArray tmpValue;
      TEmissionClass::TAdapter::FromXml(doc.RootElement().ElChild(0, "Classes"), "Class", tmpValue);
      NAmg::NScl::NHttp::TQueueUpdate update;
      _app.EmissionClasses.Prepare(tmpValue, update);
      _app.Bud.Update(update);
   }
   catch (const NAmg::TIError& _err)
   {
      amgDebug.Show("Cannot process frequency ranges in " + _fileName, _err().Message(), errl_Warning);
   }

   //сначала грузим диапазоны, пока нет остального
   try
   {
      TFrequencyRange::TAdapter::TArray tmpValue;
      TFrequencyRange::TAdapter::FromXml(doc.RootElement().ElChild(0, "Ranges"), "Range", tmpValue);
      NAmg::NScl::NHttp::TQueueUpdate update;
      _app.FrequencyRanges.Prepare(tmpValue, update);
      _app.Bud.Update(update);
   }
   catch(const NAmg::TIError& _err)
   {
      amgDebug.Show("Cannot process frequency ranges in " + _fileName, _err().Message(), errl_Warning);
   }

   //затем грузим сервисы (до наборов)
   try
   {
      TServiceData::TAdapter::TArray tmpValue;
      TServiceData::TAdapter::FromXml(doc.RootElement().ElChild(0, "Services"), "Service", tmpValue);
      NAmg::NScl::NHttp::TQueueUpdate update;
      _app.ServiceDatas.Prepare(tmpValue, update);
      _app.Bud.Update(update);
   }
   catch(const NAmg::TIError& _err)
   {
      amgDebug.Show("Cannot process services in " + _fileName, _err().Message(), errl_Warning);
   }

   //в конце, когда все загружено, грузим наборы
   try
   {
      TServiceSetData::TAdapter::TArray tmpValue;
      TServiceSetData::TAdapter::FromXml(doc.RootElement().ElChild(0, "Sets"), "Set", tmpValue);
      NAmg::NScl::NHttp::TQueueUpdate update;
      _app.ServiceSetDatas.Prepare(tmpValue, update);
      _app.Bud.Update(update);
   }
   catch(const NAmg::TIError& _err)
   {
      amgDebug.Show("Cannot process service sets in " + _fileName, _err().Message(), errl_Warning);
   }

}

void AmgCall TAppData::SaveToFile(const TAppData& /*_app*/, const NAmg::TPath& /*_fileName*/)
{
}

void AmgCall TAppData::LoadFromFrequencyFile(TAppData& _app, const NAmg::TPath& _fileName)
{
   NSmart::TDiskFileReader::TREF reader(NSmart::TDiskFileReader::Create(_fileName));
   NAmg::NXml::TDocument doc;
   doc.LoadFile(reader);
   //грузим запрещенные частоты
   try
   {
      TForbiddenFrequency::TAdapter::TArray tmpValue;
      TForbiddenFrequency::TAdapter::FromXml(doc.RootElement().ElChild(0, "FreqValues"), "FreqValue", tmpValue);
      NAmg::NScl::NHttp::TQueueUpdate update;
      _app.ForbiddenFrequency.Prepare(tmpValue, update);
      _app.Bud.Update(update);
   }
   catch (const NAmg::TIError& _err)
   {
      amgDebug.Show("Cannot process service sets in " + _fileName, _err().Message(), errl_Warning);
   }
   try
   {
      TForbiddenFrequencyRange::TAdapter::TArray tmpValue;
      TForbiddenFrequencyRange::TAdapter::FromXml(doc.RootElement().ElChild(0, "ForbiddenFreqRanges"), "ForbiddenFreqRange", tmpValue);
      NAmg::NScl::NHttp::TQueueUpdate update;
      _app.ForbiddenFrequencyRange.Prepare(tmpValue, update);
      _app.Bud.Update(update);
   }
   catch (const NAmg::TIError& _err)
   {
      amgDebug.Show("Cannot process service sets in " + _fileName, _err().Message(), errl_Warning);
   }
}

void AmgCall TAppData::SaveToFrequencyFile(const TAppData& ,  const NAmg::TPath& )
{
}

void AmgCall TAppData::LoadFromSchemeFile(TAppData& _app, const NAmg::TPath& _fileName)
{
   NSmart::TDiskFileReader::TREF reader(NSmart::TDiskFileReader::Create(_fileName));
   NAmg::NXml::TDocument doc;
   doc.LoadFile(reader);
   try
   {
      NCakApi::TScheme::TAdapter::TArray tmpValue;
      NCakApi::TScheme::TAdapter::FromXml(doc.RootElement(), "Scheme", tmpValue);
      NAmg::NScl::NHttp::TQueueUpdate update;
      _app.Schemes.Prepare(tmpValue, update);
      _app.Bud.Update(update);
   }
   catch (const NAmg::TIError& _err)
   {
      amgDebug.Show("Cannot process service sets in " + _fileName, _err().Message(), errl_Warning);
   }

}

void AmgCall TAppData::SaveToSchemeFile(TAppData&, const NAmg::TPath&)
{}

AmgCreate TApplication::TApplication()
   : NAmg::NApplication::TServiceApplication()
   , AppData(*new TAppData())
//   , m_manager(std::make_shared<NAmg::NCakApi::TManager>("budcomplan"))
  // , m_schemeManager(std::make_shared<NAmg::NCakApi::TSchemeManager>(m_manager))
{
   FConsole.AfterInput =
      NAmg::NApplication::TConsole::TAfterInput::TClosure(
      *this, &TApplication::Cb_AfterInput);
   //m_schemeManager->InitializeCacheAdapter();
}

AmgDestroy TApplication::~TApplication()
{
}

void AmgCall TApplication::NotifyCacosAboutSetIsSend(TAString Author, bool IsSend, TAString Description, TAString Bud, const TDateTime& RegTime)
{
   static TAString chTractSet("TractSet");
   static TAString chAuthor("Author");
   static TAString chLiveIndicator("LiveIndicator");
   static TAString chDescription("Description");
   static TAString chBudItem("From");
   static TAString chRegTime("RegTime");
   DCreateLogRecord(rec, chTractSet);
   DAddFieldToRecord(rec, TAString, chAuthor, Author);
   DAddFieldToRecord(rec, bool, chLiveIndicator, IsSend);
   DAddFieldToRecord(rec, TAString, chDescription, Description);
   DAddFieldToRecord(rec, TAString, chBudItem, Bud);
   DAddFieldToRecord(rec, TAString, chRegTime, RegTime);
   DAddRecordToLog(AppData.ref.Bud.Log(), rec);
}

bool AmgCall TApplication::Initialize()
{
   //задаем параметры бада по-умолчанию, чтобы в случае, если отсутствует конфиг и его
   //создавали в TApplication::DoLoadConfiguration, туда пошли дефолтные значения
   AppData.ref.BudParameters.Settings.PullAddress.Host() = NAmg::LocalAddress();
   AppData.ref.BudParameters.Settings.PullAddress.Port(4900);
   AppData.ref.BudParameters.Settings.InterfacePort.SetPort(8889);

   if (!NApplication::TApplication::Initialize())
      return(false);

   auto comPlansPath = FindConfig("ComPlans.xml");
   auto frequencyPath = FindConfig("Frequency.xml");
   auto schemesPath = FindConfig("Schemes.xml");
   auto extTypesPath = FindConfig("ExtraDeviceTypes.xml");
   auto projectWebPath = RootPath() + "Web";
   auto commonWebPath = RootPath() + "../../Web"; //путь к папке какоса

   std::weak_ptr<TAppData> weakApp = AppData.ptr;
   auto& bud = AppData.ref.Bud;

   bud.OnCreateCache([this, weakApp, comPlansPath, frequencyPath, schemesPath, extTypesPath]()
   {
      amgDebug.Show("Creating cache", errl_Inform);
      std::shared_ptr<TAppData> app = weakApp.lock();
      if (app)
         TApplication::CreateCache(NUtils::shared_ptr_ref<TAppData>::make(std::move(app)), comPlansPath, frequencyPath, schemesPath, extTypesPath);
   });

   bud.OnActive([] ()
   {
      amgDebug.Show("Bud now active", errl_Inform);
   });

   bud.OnPassive([] ()
   {
      amgDebug.Show("Bud now passive", errl_Inform);
   });

   bud.OnUpdate([] (const NHttp::TQueueUpdate& _update)
   {
      amgDebug.Show("Bud update recieved", _update.Method + TAString::Blank() + _update.Path, errl_Inform);
   });

   AppData.ref.Hosts.BeforeUpdate([weakApp](const std::string& _id, const NCakApi::THost& _valueBefore, const NCakApi::THost& _valueAfter)
   {
      std::shared_ptr<TAppData> app = weakApp.lock();
      if (!app) return;

      bool becameOnline = !NCakApi::THost::IsOnline(_valueBefore.State) && NCakApi::THost::IsOnline(_valueAfter.State);
      bool becameOffline = NCakApi::THost::IsOnline(_valueBefore.State) && !NCakApi::THost::IsOnline(_valueAfter.State);
      if (!becameOnline && !becameOffline)
         return;

      for (auto& service : app->ServiceStates.Array)
      {
         auto it = service.second.Endpoints.find(_id);
         if (service.second.Endpoints.end() == it)
            continue;

         if (becameOnline)
         {
            TEndpointState::RequestToAttach(app->Bud, app->ServiceStates.Path, service.first, it->first);
         }

         if (becameOffline)
         {
            TEndpointState::RequestToDettach(app->Bud, app->ServiceStates.Path, service.first, it->first);
         }
      }
   });

   AppData.ref.ServiceStates.BeforeUpdate([weakApp] (const std::string& _id, const TServiceState& /*_valueBefore*/, const TServiceState& _valueAfter)
   {
      std::shared_ptr<TAppData> app = weakApp.lock();
      if (!app) return;

      auto& setState = app->ServiceSetState;
      //идет ли переключение набора?
      if (setState.Section.CurrentSetId != setState.Section.AwaitedSetId)
      {
         //если да, то проверяем, есть ли активные сервисы
         bool hasActive = false;
         for (auto& stateIt : app->ServiceStates.Array)
         {
            const TServiceState* state;
            if (_id == stateIt.first)
               state = &_valueAfter;
            else
               state = &stateIt.second;
            if (!state->CurrentModeId.empty() || !state->AwaitedModeId.empty())
            {
               hasActive = true;
               break;
            }
         }
         if (!hasActive)
         {
            //сервисы все отключены, можно переключать активный сервис
            TServiceSetState::CurrentToCache(app->Bud, setState, setState.Section.AwaitedSetId);
         }
      }
   });

   AppData.ref.ServiceSetState.BeforeUpdate([weakApp](const TServiceSetState& _setState)
   {
      if (_setState.AwaitedSetId != _setState.CurrentSetId)
         return;

      std::shared_ptr<TAppData> app = weakApp.lock();
      if (!app) return;

      auto appData = NUtils::shared_ptr_ref<TAppData>::make(std::move(app));

      auto& inSet = appData.ref.ServiceSetDatas[_setState.CurrentSetId].ServiceIds;

      for (auto& item : appData.ref.ServiceDatas.Array)
      {
         if (inSet.end() == std::find(inSet.begin(), inSet.end(), item.first))
         {
            NAmg::NScl::NHttp::TQueueUpdate update;
            appData.ref.ServiceStates.PrepareDelete(item.first, update);
            appData.ref.Bud.Update(update);
         }
         else
         {
            TServiceState::InitInCache(appData.ref.Bud, appData.ref.ServiceStates, item.first);

            const auto& endpoints = item.second.Endpoints;
            for (auto& ep : endpoints)
            {
               DoAttachEndpoint(
                  appData,
                  NUtils::make_shared_cptr_cref<TIdsCapture>(item.first, "", ep.first),
                  [](bool){});
            }
         }
      }
   });

   bud.OnInitialized([projectWebPath, commonWebPath, weakApp] (NHttp::TRouteService& _http)
   {
      std::shared_ptr<TAppData> app = weakApp.lock();
      if (app)
         TApplication::InitializeHttp(NUtils::shared_ptr_ref<TAppData>::make(std::move(app)), projectWebPath, commonWebPath, _http);
   });

   bud.SetParameters(AppData.ref.BudParameters);

   return(true);
}

void AmgCall TApplication::DoLoadConfiguration(void)
{
   NAmg::NApplication::TServiceApplication::DoLoadConfiguration();

   auto configurationPath = FindConfig("Configuration.xml");
   try
   {
      NXml::TDocument doc;
      doc.LoadFile(configurationPath);
      NCakApi::TBud::TSettings::FromXml(doc.RootElement(), AppData.ref.BudParameters.Settings);

      //если файлы для кэша заданы относительные, то смотрим их относительно папки с программой,
      //а не из "текущей" папки, откуда запускали
      auto& localCache = AppData.ref.BudParameters.Settings.LocalFileCache;
      if (!localCache.FileToReadPath.IsEmpty() && !localCache.FileToReadPath.IsAbsolute())
         localCache.FileToReadPath = RootPath() + localCache.FileToReadPath;
      if (!localCache.FileToWritePath.IsEmpty() && !localCache.FileToWritePath.IsAbsolute())
         localCache.FileToWritePath = RootPath() + localCache.FileToWritePath;
   }
   catch (const NCakApi::TXmlParsingException& _error)
   {
      amgDebug.Show("Failed to load " + configurationPath + "\nReason: " + _error.what(), errl_Warning);
   }
   catch (const TFileError::TError& _error)
   {
      amgDebug.Show(_error);
   }
   catch (...)
   {
      amgDebug.Show("Error loading configuration!", errl_Warning);
   }

   auto presetDataPath = FindConfig("PresetData.xml");
   try
   {
      NSmart::TDiskFileReader::TREF reader(NSmart::TDiskFileReader::Create(presetDataPath));
      AppData.ref.PresetManager.LoadFromFile(reader);
   }
   catch (NCakApi::TXmlParsingException& e)
   {
      amgDebug.Show("Failed to load " + presetDataPath + "\nReason: " + e.what(), errl_Warning);
   }
   catch (const TFileError::TError& _error)
   {
      amgDebug.Show(_error);
   }
   catch (...)
   {
      amgDebug.Show("Error loading managed data config!", errl_Warning);
   }
}

void AmgCall TApplication::DoSaveConfiguration(void)
{
   NAmg::NApplication::TServiceApplication::DoSaveConfiguration();
   //
   auto configurationPath = FindConfig("Configuration.xml");
   try
   {
      NXml::TDocument doc;
      std::unique_ptr<NXml::TElement> root{new NXml::TElement("Configuration")};
      NCakApi::TBud::TSettings::ToXml(AppData.ref.BudParameters.Settings, *root);
      doc.AddChild(*root.release());
      doc.SaveFile(configurationPath);
   }
   catch (const NCakApi::TXmlParsingException& _error)
   {
      amgDebug.Show("Failed to save " + configurationPath + "\nReason: " + _error.what(), errl_Warning);
   }
   catch (const TFileError::TError& _error)
   {
      amgDebug.Show(_error);
   }
   catch (...)
   {
      amgDebug.Show("Error saving configuration!", errl_Warning);
   }
}

void AmgCall TApplication::InitializeHttp(const NUtils::shared_ptr_ref<TAppData>& _app, const NAmg::TPath& _ownWebPath, const NAmg::TPath& _commonWebPath, NAmg::NScl::NHttp::TRouteService& _http)
{
   //отдаем свой раздел
   _http["/app/bud/cp/**"].Ctrl<NHttp::TFolderControl>().Base(_ownWebPath);
   //отдаем свой app.js
   _http["/app/app.js"].Ctrl<NHttp::TFolderControl>().Base(_ownWebPath);
   //отдаем свой index.html
   _http["/"].Rewrite("/app/index.html").Ctrl<NHttp::TFolderControl>().Base(_ownWebPath);
   _http["/index.html"].Rewrite("/app/index.html").Ctrl<NHttp::TFolderControl>().Base(_ownWebPath);

   //остальное берем из какоса
   _http["/img/**"].Ctrl<NHttp::TFolderControl>().Base(_commonWebPath);
   _http["/lib/**"].Ctrl<NHttp::TFolderControl>().Base(_commonWebPath);
   _http["/app/**"].Ctrl<NHttp::TFolderControl>().Base(_commonWebPath);

   std::weak_ptr<TAppData> weakApp = _app.ptr;
   _http["/bud/cp/sets/state/awaited"].Ctrl()
      .Method("PUT", [weakApp] (const NHttp::TRequestData& _request)
      {
         //просят переключить набор трактов
         
         std::shared_ptr<TAppData> app = weakApp.lock();
         if (!app)
            return;
 
         Json::Value json;
         _request.To(json);
         //Запрос на переключение набора трактов
         DCreateLogRecord(rec, L"");
         DAddFieldToRecord(rec, TAString, NLotData::AuthorStr(), L"Author");
         DAddFieldToRecord(rec, TAString, NLotData::FromSetIdStr(), L"");
         DAddFieldToRecord(rec, TAString, NLotData::ToSetIdStr(), json["id"].asString());
         DAddFieldToRecord(rec, TAString, NLotData::DescriptionStr(), StringRes.Short("SwitchSetOfTracts"));
         DAddFieldToRecord(rec, TAString, NLotData::RegTimeStr(), TDateTime());
         DAddRecordToLog(app->Bud.Log(), rec);

         if (DoSwitchSet(NUtils::shared_ptr_ref<TAppData>::make(app), json["id"].asString()))
         {
            //for (int i = 0; i < 100; i++) {
            //   Application().NotifyCacosAboutSetIsSend("Kosturev", true, "Set params is send", "BudComplan", TDateTime());
            //}
            NHttp::TRouteService::Send(_request, 202, "Accepted");
            //Завершение переключения набора трактов
            DCreateLogRecord(rec, L"");
            DAddFieldToRecord(rec, TAString, NLotData::AuthorStr(), L"Author");
            DAddFieldToRecord(rec, TAString, NLotData::FromSetIdStr(), L"");
            DAddFieldToRecord(rec, TAString, NLotData::ToSetIdStr(), json["id"].asString());
            DAddFieldToRecord(rec, TAString, NLotData::DescriptionStr(), StringRes.Short("SwitchSetOfTractsCompleted") + TAString::Blank() + StringRes.Short("successful"));
            DAddFieldToRecord(rec, bool, NLotData::ResultStr(), true);
            DAddFieldToRecord(rec, TAString, NLotData::RegTimeStr(), TDateTime());
            DAddRecordToLog(app->Bud.Log(), rec);
         }
         else
         {
            NHttp::TRouteService::Send(_request, 400, "Bad Request");
            //Завершение переключения набора трактов
            DCreateLogRecord(rec, L"");
            DAddFieldToRecord(rec, TAString, NLotData::AuthorStr(), L"Author");
            DAddFieldToRecord(rec, TAString, NLotData::FromSetIdStr(), L"");
            DAddFieldToRecord(rec, TAString, NLotData::ToSetIdStr(), json["id"].asString());
            DAddFieldToRecord(rec, TAString, NLotData::DescriptionStr(), StringRes.Short("SwitchSetOfTractsCompleted") + TAString::Blank() + StringRes.Short("unsuccessful"));
            DAddFieldToRecord(rec, bool, NLotData::ResultStr(), false);
            DAddFieldToRecord(rec, TAString, NLotData::RegTimeStr(), TDateTime());
            DAddRecordToLog(app->Bud.Log(), rec);
         }
      });

   _http["/bud/cp/services/state/:serviceId"].Ctrl()
      .Method("POST", [weakApp] (const NHttp::TRequestData& _request)
      {
         //просят сформировать тракт
      
         std::shared_ptr<TAppData> app = weakApp.lock();
         if (!app)
            return;
         
         std::shared_ptr<NHttp::TRequestData> req{new NHttp::TRequestData{_request}};
         std::weak_ptr<NHttp::TRequestData> weakReq{req};
         NHttp::TRouteService::OnError(_request,
            [req] () mutable
            {
               req.reset();
            });

         auto appData = NUtils::shared_ptr_ref<TAppData>::make(app);

         std::string modeId{};
         auto& serviceId = _request.Vars.at("serviceId");
         //Запрос на формирование тракта
         DCreateLogRecord(rec, L"");
         DAddFieldToRecord(rec, TAString, NLotData::AuthorStr(), L"Author");
         DAddFieldToRecord(rec, TAString, NLotData::ServiceIdStr(), serviceId);
         DAddFieldToRecord(rec, TAString, NLotData::DescriptionStr(), StringRes.Short("CreateTract"));
         DAddFieldToRecord(rec, TAString, NLotData::RegTimeStr(), TDateTime());
         DAddRecordToLog(app->Bud.Log(), rec);
         Json::Value json;
         _request.To(json);

         auto members = json.getMemberNames();
         if (members.end() != std::find(members.begin(), members.end(), "id"))
            modeId = json["id"].asString();

         if (modeId.empty())
            modeId = DefaultMode(appData, serviceId);

         auto action = DoBuildService;

         auto itState = appData.ref.ServiceStates.Array.find(serviceId);
         if (appData.ref.ServiceStates.Array.end() != itState && !itState->second.CurrentModeId.empty())
         {
            if (modeId == itState->second.CurrentModeId)
               NHttp::TRouteService::Send(*req, 200, "OK");
            else
               action = DoSwitchServiceMode;
         }

         action(appData, serviceId, modeId,
            [weakReq, weakApp, serviceId] (bool success)
            {
               std::shared_ptr<NHttp::TRequestData> req = weakReq.lock();
               std::shared_ptr<TAppData> app = weakApp.lock();
               if (!req)
                  return;
               if (success)
               {
                  NHttp::TRouteService::Send(*req, 200, "OK");
                  //Завершение формирования тракта
                  if (app)
                  {
                     DCreateLogRecord(rec, L"");
                     DAddFieldToRecord(rec, TAString, NLotData::AuthorStr(), L"Author");
                     DAddFieldToRecord(rec, TAString, NLotData::ServiceIdStr(), serviceId);
                     DAddFieldToRecord(rec, TAString, NLotData::DescriptionStr(), StringRes.Short("CreateTractCompleted") + TAString::Blank() + StringRes.Short("successful"));
                     DAddFieldToRecord(rec, bool, NLotData::ResultStr(), true);
                     DAddFieldToRecord(rec, TAString, NLotData::RegTimeStr(), TDateTime());
                     DAddRecordToLog(app->Bud.Log(), rec);
                  }
               }
               else
               {
                  NHttp::TRouteService::Send(*req, 400, "Bad Request");
                  //Завершение формирования тракта
                  if (app)
                  {
                     DCreateLogRecord(rec, L"");
                     DAddFieldToRecord(rec, TAString, NLotData::AuthorStr(), L"Author");
                     DAddFieldToRecord(rec, TAString, NLotData::ServiceIdStr(), serviceId);
                     DAddFieldToRecord(rec, TAString, NLotData::DescriptionStr(), StringRes.Short("CreateTractCompleted") + TAString::Blank() + StringRes.Short("unsuccessful"));
                     DAddFieldToRecord(rec, bool, NLotData::ResultStr(), false);
                     DAddFieldToRecord(rec, TAString, NLotData::RegTimeStr(), TDateTime());
                     DAddRecordToLog(app->Bud.Log(), rec);
                  }
               }
            });
      })
      .Method("DELETE", [weakApp] (const NHttp::TRequestData& _request)
      {
         //просят расформировать тракт
         
         std::shared_ptr<TAppData> app = weakApp.lock();
         if (!app)
            return;
         auto& serviceId = _request.Vars.at("serviceId");
         //Запрос на расформирование тракта
         DCreateLogRecord(rec, L"");
         DAddFieldToRecord(rec, TAString, NLotData::AuthorStr(), L"Author");
         DAddFieldToRecord(rec, TAString, NLotData::ServiceIdStr(), serviceId);
         DAddFieldToRecord(rec, TAString, NLotData::DescriptionStr(), StringRes.Short("DeleteTract"));
         DAddFieldToRecord(rec, TAString, NLotData::RegTimeStr(), TDateTime());
         DAddRecordToLog(app->Bud.Log(), rec);
         std::shared_ptr<NHttp::TRequestData> req{new NHttp::TRequestData{_request}};
         std::weak_ptr<NHttp::TRequestData> weakReq{req};
         NHttp::TRouteService::OnError(_request,
            [req] () mutable
            {
               req.reset();
            });

         DoDestroyService(NUtils::shared_ptr_ref<TAppData>::make(app), _request.Vars.at("serviceId"),
            [weakReq, weakApp, serviceId] (bool success)
            {
               std::shared_ptr<NHttp::TRequestData> req = weakReq.lock();
               std::shared_ptr<TAppData> app = weakApp.lock();
               if (!req)
                  return;
               if (success)
               {
                  NHttp::TRouteService::Send(*req, 200, "OK");
                  //Завершение расформирования тракта
                  if (app)
                  {
                     DCreateLogRecord(rec, L"");
                     DAddFieldToRecord(rec, TAString, NLotData::AuthorStr(), L"Author");
                     DAddFieldToRecord(rec, TAString, NLotData::ServiceIdStr(), serviceId);
                     DAddFieldToRecord(rec, TAString, NLotData::DescriptionStr(), StringRes.Short("DeleteTractCompleted") + TAString::Blank() + StringRes.Short("successful")); 
                     DAddFieldToRecord(rec, bool, NLotData::ResultStr(), true);
                     DAddFieldToRecord(rec, TAString, NLotData::RegTimeStr(), TDateTime());
                     DAddRecordToLog(app->Bud.Log(), rec);
                  }
               }
               else
               {
                  NHttp::TRouteService::Send(*req, 400, "Bad Request");
                  //Завершение расформирования тракта
                  if (app)
                  {
                     DCreateLogRecord(rec, L"");
                     DAddFieldToRecord(rec, TAString, NLotData::AuthorStr(), L"Author");
                     DAddFieldToRecord(rec, TAString, NLotData::ServiceIdStr(), serviceId);
                     DAddFieldToRecord(rec, TAString, NLotData::DescriptionStr(), StringRes.Short("DeleteTractCompleted") + TAString::Blank() + StringRes.Short("unsuccessful"));
                     DAddFieldToRecord(rec, bool, NLotData::ResultStr(), false);
                     DAddFieldToRecord(rec, TAString, NLotData::RegTimeStr(), TDateTime());
                     DAddRecordToLog(app->Bud.Log(), rec);
                  }
               }
            });
      });

   _http["/bud/cp/services/state/:serviceId/mode/:modeId"].Ctrl()
      .Method("POST", [weakApp] (const NHttp::TRequestData& _request)
      {
         //просят изменить режим
         std::shared_ptr<TAppData> app = weakApp.lock();
         if (!app) return;
         auto& serviceId = _request.Vars.at("serviceId");
         auto& toModeId = _request.Vars.at("modeId");
         DCreateLogRecord(rec, L"");
         DAddFieldToRecord(rec, TAString, NLotData::AuthorStr(), L"Author");
         DAddFieldToRecord(rec, TAString, NLotData::ServiceIdStr(), serviceId);
         DAddFieldToRecord(rec, TAString, NLotData::FromModeIdStr(), L"");
         DAddFieldToRecord(rec, TAString, NLotData::ToModeIdStr(), toModeId);
         DAddFieldToRecord(rec, TAString, NLotData::DescriptionStr(), StringRes.Short("SwitchTractMode"));
         DAddFieldToRecord(rec, TAString, NLotData::RegTimeStr(), TDateTime());
         DAddRecordToLog(app->Bud.Log(), rec);
         std::shared_ptr<NHttp::TRequestData> req{new NHttp::TRequestData{_request}};
         std::weak_ptr<NHttp::TRequestData> weakReq{req};
         NHttp::TRouteService::OnError(_request,
            [req] () mutable
            {
               req.reset();
            });

         DoSwitchServiceMode(NUtils::shared_ptr_ref<TAppData>::make(app), _request.Vars.at("serviceId"), _request.Vars.at("modeId"),
            [weakReq, weakApp, serviceId, toModeId] (bool success)
            {
               std::shared_ptr<NHttp::TRequestData> req = weakReq.lock();
               std::shared_ptr<TAppData> app = weakApp.lock();
               if (!req)
                  return;
               if (success)
               {
                  NHttp::TRouteService::Send(*req, 200, "OK");
                  if (app)
                  {
                     DCreateLogRecord(rec, L"");
                     DAddFieldToRecord(rec, TAString, NLotData::AuthorStr(), L"Author");
                     DAddFieldToRecord(rec, TAString, NLotData::ServiceIdStr(), serviceId);
                     DAddFieldToRecord(rec, TAString, NLotData::FromModeIdStr(), L"");
                     DAddFieldToRecord(rec, TAString, NLotData::ToModeIdStr(), toModeId);
                     DAddFieldToRecord(rec, TAString, NLotData::DescriptionStr(), StringRes.Short("SwitchTractModeCompleted") + TAString::Blank() + StringRes.Short("successful"));
                     DAddFieldToRecord(rec, bool, NLotData::ResultStr(), true);
                     DAddFieldToRecord(rec, TAString, NLotData::RegTimeStr(), TDateTime());
                     DAddRecordToLog(app->Bud.Log(), rec);
                  }
               }
               else
               {
                  NHttp::TRouteService::Send(*req, 400, "Bad Request");
                  if (app)
                  {
                     DCreateLogRecord(rec, L"");
                     DAddFieldToRecord(rec, TAString, NLotData::AuthorStr(), L"Author");
                     DAddFieldToRecord(rec, TAString, NLotData::ServiceIdStr(), serviceId);
                     DAddFieldToRecord(rec, TAString, NLotData::FromModeIdStr(), L"");
                     DAddFieldToRecord(rec, TAString, NLotData::ToModeIdStr(), toModeId);
                     DAddFieldToRecord(rec, TAString, NLotData::DescriptionStr(), StringRes.Short("SwitchTractModeCompleted") + TAString::Blank() + StringRes.Short("unsuccessful"));
                     DAddFieldToRecord(rec, bool, NLotData::ResultStr(), false);
                     DAddFieldToRecord(rec, TAString, NLotData::RegTimeStr(), TDateTime());
                     DAddRecordToLog(app->Bud.Log(), rec);
                  }
               }
            });
      });

   _http["/bud/cp/services/state/:serviceId/endpoints"].Ctrl()
      .Method("PUT", [weakApp] (const NHttp::TRequestData& _request)
      {
         //просят подсоединить или отсоединить оконечные устройства
      //Подключение терминала к тракту
         //Отключение терминала от тракта
         std::shared_ptr<NHttp::TRequestData> req{new NHttp::TRequestData{_request}};
         std::weak_ptr<NHttp::TRequestData> weakReq{req};
         NHttp::TRouteService::OnError(_request,
            [req] () mutable
            {
               req.reset();
            });

         NHttp::TRouteService::Send(*req, 200, "OK");

         std::shared_ptr<TAppData> app = weakApp.lock();
         if (!app)
            return;

         Json::Value json;
         _request.To(json);

         std::string fromEndpoint{};

         auto members = json.getMemberNames();
         if (members.end() != std::find(members.begin(), members.end(), "source"))
            fromEndpoint = json["source"].asString();

         DoSwitchEndpointsConnection(
            json["connected"].asBool(),
            NUtils::shared_ptr_ref<TAppData>::make(std::move(app)),
            _request.Vars.at("serviceId"),
            fromEndpoint);
   });

   _http["/bud/cp/services/state/:serviceId/endpoints/:endpointId"].Ctrl()
      .Method("POST", [weakApp](const NHttp::TRequestData& _request)
      {
         //просят подключить оконечное устройство
         std::shared_ptr<TAppData> app = weakApp.lock();
         if (!app)
            return;
         auto& serviceId = _request.Vars.at("serviceId");
         auto& endpointId = _request.Vars.at("endpointId");
         DCreateLogRecord(rec, L"");
         DAddFieldToRecord(rec, TAString, NLotData::AuthorStr(), L"Author");
         DAddFieldToRecord(rec, TAString, NLotData::ServiceIdStr(), serviceId);
         DAddFieldToRecord(rec, TAString, NLotData::EndPointIdStr(), endpointId);
         DAddFieldToRecord(rec, TAString, NLotData::DescriptionStr(), StringRes.Short("AddTerminal"));
         DAddFieldToRecord(rec, TAString, NLotData::RegTimeStr(), TDateTime());
         DAddRecordToLog(app->Bud.Log(), rec);
         std::shared_ptr<NHttp::TRequestData> req{ new NHttp::TRequestData{ _request } };
         std::weak_ptr<NHttp::TRequestData> weakReq{ req };
         NHttp::TRouteService::OnError(_request,
            [req]() mutable
         {
            req.reset();
         });

         DoAttachEndpoint(
            NUtils::shared_ptr_ref<TAppData>::make(app),
            NUtils::make_shared_cptr_cref<TIdsCapture>(_request.Vars.at("serviceId"), "", _request.Vars.at("endpointId")),
            [weakReq, weakApp, serviceId, endpointId](bool success)
         {
            std::shared_ptr<NHttp::TRequestData> req = weakReq.lock();
            std::shared_ptr<TAppData> app = weakApp.lock();
            if (!req)
               return;
            if (success)
            {
               NHttp::TRouteService::Send(*req, 200, "OK");
               if (app)
               {
                  DCreateLogRecord(rec, L"");
                  DAddFieldToRecord(rec, TAString, NLotData::AuthorStr(), L"Author");
                  DAddFieldToRecord(rec, TAString, NLotData::ServiceIdStr(), serviceId);
                  DAddFieldToRecord(rec, TAString, NLotData::EndPointIdStr(), endpointId);
                  DAddFieldToRecord(rec, TAString, NLotData::DescriptionStr(), StringRes.Short("AddTerminalCompleted") + TAString::Blank() + StringRes.Short("successful"));
                  DAddFieldToRecord(rec, bool, NLotData::ResultStr(), true);
                  DAddFieldToRecord(rec, TAString, NLotData::RegTimeStr(), TDateTime());
                  DAddRecordToLog(app->Bud.Log(), rec);
               }
            }
            else
            {
               NHttp::TRouteService::Send(*req, 400, "Bad Request");
               if (app)
               {
                  DCreateLogRecord(rec, L"");
                  DAddFieldToRecord(rec, TAString, NLotData::AuthorStr(), L"Author");
                  DAddFieldToRecord(rec, TAString, NLotData::ServiceIdStr(), serviceId);
                  DAddFieldToRecord(rec, TAString, NLotData::EndPointIdStr(), endpointId);
                  DAddFieldToRecord(rec, TAString, NLotData::DescriptionStr(), StringRes.Short("AddTerminalCompleted") + TAString::Blank() + StringRes.Short("unsuccessful"));
                  DAddFieldToRecord(rec, bool, NLotData::ResultStr(), false);
                  DAddFieldToRecord(rec, TAString, NLotData::RegTimeStr(), TDateTime());
                  DAddRecordToLog(app->Bud.Log(), rec);
               }
            }
         });
      })
      .Method("DELETE", [weakApp] (const NHttp::TRequestData& _request)
      {
         //просят отключить оконечное устройство
         std::shared_ptr<TAppData> app = weakApp.lock();
         if (!app) return;
         auto& serviceId = _request.Vars.at("serviceId");
         auto& endpointId = _request.Vars.at("endpointId");
         DCreateLogRecord(rec, L"");
         DAddFieldToRecord(rec, TAString, NLotData::AuthorStr(), L"Author");
         DAddFieldToRecord(rec, TAString, NLotData::ServiceIdStr(), serviceId);
         DAddFieldToRecord(rec, TAString, NLotData::EndPointIdStr(), endpointId);
         DAddFieldToRecord(rec, TAString, NLotData::DescriptionStr(), StringRes.Short("DeleteTerminal"));
         DAddFieldToRecord(rec, TAString, NLotData::RegTimeStr(), TDateTime());
         DAddRecordToLog(app->Bud.Log(), rec);
         std::shared_ptr<NHttp::TRequestData> req{new NHttp::TRequestData{_request}};
         std::weak_ptr<NHttp::TRequestData> weakReq{req};
         NHttp::TRouteService::OnError(_request,
            [req] () mutable
            {
               req.reset();
            });

         DoDetachEndpoint(
            NUtils::shared_ptr_ref<TAppData>::make(app),
            NUtils::make_shared_cptr_cref<TIdsCapture>(_request.Vars.at("serviceId"), "", _request.Vars.at("endpointId")),
            [weakReq, weakApp, serviceId, endpointId] (bool success)
            {
               std::shared_ptr<NHttp::TRequestData> req = weakReq.lock();
               std::shared_ptr<TAppData> app = weakApp.lock();
               if (!req)
                  return;
               if (success)
               {
                  NHttp::TRouteService::Send(*req, 200, "OK");
                  if (app)
                  {
                     DCreateLogRecord(rec, L"");
                     DAddFieldToRecord(rec, TAString, NLotData::AuthorStr(), L"Author");
                     DAddFieldToRecord(rec, TAString, NLotData::ServiceIdStr(), serviceId);
                     DAddFieldToRecord(rec, TAString, NLotData::EndPointIdStr(), endpointId);
                     DAddFieldToRecord(rec, TAString, NLotData::DescriptionStr(), StringRes.Short("DeleteTerminalCompleted") + TAString::Blank() + StringRes.Short("successful"));
                     DAddFieldToRecord(rec, bool, NLotData::ResultStr(), true);
                     DAddFieldToRecord(rec, TAString, NLotData::RegTimeStr(), TDateTime());
                     DAddRecordToLog(app->Bud.Log(), rec);
                  }
               }
               else
               {
                  NHttp::TRouteService::Send(*req, 400, "Bad Request");
                  if (app)
                  {
                     DCreateLogRecord(rec, L"");
                     DAddFieldToRecord(rec, TAString, NLotData::AuthorStr(), L"Author");
                     DAddFieldToRecord(rec, TAString, NLotData::ServiceIdStr(), serviceId);
                     DAddFieldToRecord(rec, TAString, NLotData::EndPointIdStr(), endpointId);
                     DAddFieldToRecord(rec, TAString, NLotData::DescriptionStr(), StringRes.Short("DeleteTerminalCompleted") + TAString::Blank() + StringRes.Short("unsuccessful"));
                     DAddFieldToRecord(rec, bool, NLotData::ResultStr(), false);
                     DAddFieldToRecord(rec, TAString, NLotData::RegTimeStr(), TDateTime());
                     DAddRecordToLog(app->Bud.Log(), rec);
                  }
               }
            });
      });
      auto onApplyServiceState = [weakApp](const NHttp::TRequestData& _request) {       
         std::shared_ptr<TAppData> app = weakApp.lock();
         if (!app)
            return;
         auto& serviceId = _request.Vars.at("serviceId");     
         NAmg::NScl::NHttp::TQueueUpdate update;
         update.Method = "PUT";
         update.Path = app->ServiceStates.Path + serviceId;
         Json::Value json;
         try
         {
            _request.To(json);
            update.Body = &NAmg::NScl::NText::toBody(json);
            app->Bud.Update(update);
         }
         catch(...)
         { 
            NHttp::TRouteService::Send(_request, 400, "Bad Request");
            return;
         }
         NHttp::TRouteService::Send(_request, 200, "OK");
      };      
      //изменение настроек приема
      _http["/bud/cp/services/state/:serviceId/in"].Ctrl().Method("PUT", onApplyServiceState);
      //изменение настроек передачи
      _http["/bud/cp/services/state/:serviceId/out"].Ctrl().Method("PUT", onApplyServiceState);      
      //смена шифратора
      _http["/bud/cp/services/state/:serviceId/encrypted_id"].Ctrl().Method("PUT", onApplyServiceState);

      _http["/bud/cp/armks"].Ctrl().Method("PUT", [weakApp](const NHttp::TRequestData& _request){
         //установка набора с АРМ КС
         std::shared_ptr<TAppData> app = weakApp.lock();
         if (!app) return;
         NXml::TDocument xml;
         _request.To(xml);
         std::shared_ptr<NHttp::TRequestData> req{ new NHttp::TRequestData{ _request } };
         std::weak_ptr<NHttp::TRequestData> weakReq{ req };
         NHttp::TRouteService::OnError(_request,
            [req]() mutable
         {
            req.reset();
         });
         DoApplySet(
            NUtils::shared_ptr_ref<TAppData>::make(std::move(app)),
            xml,
            [weakReq](bool success)
         {
            std::shared_ptr<NHttp::TRequestData> req = weakReq.lock();
            if (!req)
               return;
            if (success)
               NHttp::TRouteService::Send(*req, 200, "OK");
            else
               NHttp::TRouteService::Send(*req, 400, "Bad Request");
         });
      });
     
}

void AmgCall TApplication::CreateCache(const NUtils::shared_ptr_ref<TAppData>& _app,
                                       const NAmg::TPath& _complansPath,
                                       const NAmg::TPath& _pathtoFreq,
                                       const NAmg::TPath& _schemesPath,
                                       const NAmg::TPath& _extDeviceTypesPath)
{
   try
   {
      TAppData::LoadFromFile(_app.ref, _complansPath);
      TAppData::LoadFromFrequencyFile(_app.ref, _pathtoFreq);
      TAppData::LoadFromSchemeFile(_app.ref, _schemesPath);
      TExtraDeviceType::TAdapter::Initialize(_extDeviceTypesPath, "ExtraDeviceTypes", "DeviceType", _app.ref.ExtraDeviceTypes, _app.ref.Bud);
//      m_schemeManager->LoadFromXml(NSmart::TDiskFileReader::Create(_schemesPath));
   }
   catch (const NXml::TErrors::TError& err)
   {
      amgDebug.Show(err().Message() + " in " + _complansPath, errl_Warning);
   }
   catch (const std::logic_error& err)
   {
      amgDebug.Show(TAString(err.what()) + " in " + _complansPath, errl_Warning);
   }
   catch (const TFileError::TError& _error)
   {
      amgDebug.Show(_error);
   }
   catch (...)
   {
      amgDebug.Show("Error loading config!", errl_Critical);
   }
}

bool AmgCall TApplication::DoSwitchSet(const NUtils::shared_ptr_ref<TAppData>& _app, const std::string& _awaitedSetId)
{
   auto& setState = _app.ref.ServiceSetState;
   //если ничего не надо менять, то выходим
   if (setState.Section.AwaitedSetId == _awaitedSetId)
      return(false);

   bool ready = true;
   if (!_app.ref.ServiceStates.Array.empty())
   {
      for (auto& item : _app.ref.ServiceStates.Array)
      {
         for (auto& epItem : item.second.Endpoints)
         {
            if (TEndpointState::Attached == epItem.second.Value ||
               TEndpointState::Attaching == epItem.second.Value)
            {
               auto ids = NUtils::make_shared_cptr_cref<TIdsCapture>(item.first, "", epItem.first);
               DoDetachEndpoint(_app, ids, [](bool) {});
            }
         }

         if (item.second.CurrentModeId.empty())
            continue;

         ready = false;

         NHttp::TQueueRequest request;
         request.Method = "DELETE";
         request.Path = _app.ref.ServiceStates.Path + item.first;
         _app.ref.Bud.Request(request,
            [] (const NHttp::TQueueResponse&)
            {
               //onResponse
            },
            [] ()
            {
               //onError
            });
      }
      //есть еще незавершенные сервисы, отмечаем в кэше, что пошел процесс
      TServiceSetState::AwaitedToCache(_app.ref.Bud, setState, _awaitedSetId);
      //действительное переключение будет в обработчике ServiceStates.BeforeDelete
   }

   if (ready)
   {
      //сервисы все отключены, можно переключать активный сервис
      TServiceSetState::CurrentToCache(_app.ref.Bud, setState, _awaitedSetId);
   }

   return(true);
}

void AmgCall TApplication::DoBuildService(const NUtils::shared_ptr_ref<TAppData>& _app, const std::string& _serviceId, const std::string& _modeId, TOnResult&& _onResult)
{
   auto itState = _app.ref.ServiceStates.Array.find(_serviceId);
   if (_app.ref.ServiceStates.Array.end() != itState && _modeId == itState->second.CurrentModeId)
   {
	   _onResult(true);
	   return;
   }

   auto itSrv = _app.ref.ServiceDatas.Array.find(_serviceId);
   if (_app.ref.ServiceDatas.Array.end() == itSrv) //нет службы с таким идентификатором
   {
      _onResult(false);
      return;
   }

   auto itMode = itSrv->second.Modes.find(_modeId);
   if (itSrv->second.Modes.end() == itMode)
   {
      _onResult(false);
      return;
   }

   std::weak_ptr<TAppData> weakApp = _app.ptr;

   auto ids = NUtils::make_shared_cptr_cref<TIdsCapture>(_serviceId, _modeId);

   //рассылаем предустановки
   DoBuildMode(_app, ids, itMode->second, [weakApp, ids, _onResult] (bool _success)
      {
         _onResult(_success);

         std::shared_ptr<TAppData> app = weakApp.lock();
         if (!app) return;

         auto appData = NUtils::shared_ptr_ref<TAppData>::make(std::move(app));

         if (_success)
         {
            DoSwitchLocking(appData, ids, true);
            TServiceState::CurrentToCache(appData.ref.Bud, appData.ref.ServiceStates, ids.ref.ServiceId, ids.ref.ModeId);
            DoChangeEndpointsMode(appData, ids);
         }
      });
}

void AmgCall TApplication::DoDestroyService(const NUtils::shared_ptr_ref<TAppData>& _app, const std::string& _serviceId, TOnResult&& _onResult)
{
   auto itSrv = _app.ref.ServiceDatas.Array.find(_serviceId);
   if (_app.ref.ServiceDatas.Array.end() == itSrv) //нет службы с таким идентификатором
   {
      _onResult(false);
      return;
   }

   auto itState = _app.ref.ServiceStates.Array.find(_serviceId);
   if (_app.ref.ServiceStates.Array.end() == itState) //нет состояния для службы с таким идентификатором
   {
      _onResult(false);
      return;
   }

   if (itState->second.CurrentModeId.empty())
   {
	   _onResult(true);
	   return;
   }

   auto itMode = itSrv->second.Modes.find(itState->second.CurrentModeId);
   if (itSrv->second.Modes.end() == itMode)
   {
      _onResult(false);
      return;
   }

   std::shared_ptr<TModeState> modeState{new TModeState()};

   std::weak_ptr<TAppData> weakApp = _app.ptr;
   auto ids = NUtils::make_shared_cptr_cref<TIdsCapture>(_serviceId);

   //рассылаем предустановки
   DoDestroyMode(_app, ids, itMode->second, [weakApp, ids, _onResult] (bool _success)
      {
         _onResult(_success);

         std::shared_ptr<TAppData> app = weakApp.lock();
         if (!app) return;

         auto appData = NUtils::shared_ptr_ref<TAppData>::make(std::move(app));

         if (_success)
         {
            DoSwitchLocking(appData, ids, false);
            TServiceState::CurrentToCache(appData.ref.Bud, appData.ref.ServiceStates, ids.ref.ServiceId, ids.ref.ModeId);
            DoChangeEndpointsMode(appData, ids);
         }
      });
}

void AmgCall TApplication::DoSwitchServiceMode(const NUtils::shared_ptr_ref<TAppData>& _app, const std::string& _serviceId, const std::string& _modeId, TOnResult&& _onResult)
{
   auto itSrv = _app.ref.ServiceDatas.Array.find(_serviceId);
   if (_app.ref.ServiceDatas.Array.end() == itSrv) //нет службы с таким идентификатором
   {
      _onResult(false);
      return;
   }

   auto itState = _app.ref.ServiceStates.Array.find(_serviceId);
   if (_app.ref.ServiceStates.Array.end() == itState)
   {
      //менять режим можно только для сформированного сервиса
      _onResult(false);
      return;
   }

   if (itState->second.CurrentModeId.empty() || (!itState->second.AwaitedModeId.empty() && (itState->second.AwaitedModeId != itState->second.CurrentModeId)))
   {
      //менять режим можно только для сформированного сервиса
      _onResult(false);
      return;
   }

   auto itToMode = itSrv->second.Modes.find(_modeId);
   if (itSrv->second.Modes.end() == itToMode)
   {
      //нет в конфигурации такого режима
      _onResult(false);
      return;
   }

   std::weak_ptr<TAppData> weakApp = _app.ptr;
   auto idsToBuild = NUtils::make_shared_cptr_cref<TIdsCapture>(_serviceId, _modeId);
   auto& toModeData = itToMode->second;

   auto itFromMode = itSrv->second.Modes.find(itState->second.CurrentModeId);
   auto onBuild = [weakApp, idsToBuild, _onResult](bool _success)
   {
      std::shared_ptr<TAppData> app = weakApp.lock();
      if (!app) return;

      auto appData = NUtils::shared_ptr_ref<TAppData>::make(std::move(app));

      _onResult(_success);      
      if (_success)
      {
         DoSwitchLocking(appData, idsToBuild, true);
         TServiceState::CurrentToCache(appData.ref.Bud, appData.ref.ServiceStates, idsToBuild.ref.ServiceId, idsToBuild.ref.ModeId);
         DoChangeEndpointsMode(appData, idsToBuild);
      }
   };

   if (itSrv->second.Modes.end() != itFromMode)
   {
      //есть сформированный режим, который надо сначала расформировать
      auto idsToDestroy = NUtils::make_shared_cptr_cref<TIdsCapture>(_serviceId);
      DoDestroyMode(_app, idsToDestroy, itFromMode->second,
         [weakApp, idsToDestroy, idsToBuild, toModeData, _onResult, onBuild] (bool _success)
         {
            if (_success)
            {
               std::shared_ptr<TAppData> app = weakApp.lock();
               if (!app) return;

               auto appData = NUtils::shared_ptr_ref<TAppData>::make(std::move(app));

               DoSwitchLocking(appData, idsToDestroy, false);
               DoBuildMode(appData, idsToBuild, toModeData, onBuild);
            }
            else
            {
               _onResult(false);
            }
         });
   }
   else
   {
      DoBuildMode(_app, idsToBuild, toModeData, onBuild);
   }
}

void TApplication::DoApplySet(const NUtils::shared_ptr_ref<TAppData>& _app, NXml::TDocument& _xml, TOnResult&& _onResult)
{
   try
   {
      TServiceData::TAdapter::TArray tmpValue;
      TServiceData::TAdapter::FromXml(_xml.RootElement().ElChild(0, "Services"), "Service", tmpValue);
      Json::Value json;
      TServiceData::TAdapter::ToJson(tmpValue, json);
      auto ids = json.getMemberNames();
      for (auto& id : ids)
      {
         NAmg::NScl::NHttp::TQueueUpdate update;
         update.Method = "PUT";
         update.Path = _app.ref.ServiceDatas.Path + id;
         update.Body = &NAmg::NScl::NText::toBody(json[id]);
         _app.ref.Bud.Update(update);
      }

   }
   catch (...)
   {
      _onResult(false);
      return;
   }
   try
   {
      TServiceSetData::TAdapter::TArray tmpValue;
      TServiceSetData::TAdapter::FromXml(_xml.RootElement().ElChild(0, "Sets"), "Set", tmpValue);
      Json::Value json;
      TServiceSetData::TAdapter::ToJson(tmpValue, json);
      auto ids = json.getMemberNames();
      for (auto& id : ids)
      {
         NAmg::NScl::NHttp::TQueueUpdate update;
         update.Method = "PUT";
         update.Path = _app.ref.ServiceSetDatas.Path + id;
         update.Body = &NAmg::NScl::NText::toBody(json[id]);
         _app.ref.Bud.Update(update);
      }
   }
   catch (...)
   {
      _onResult(false);
      return;
   }
   _onResult(true);
}

namespace
{
   std::pair<bool, TEndpointState> EndpointState(
      const NUtils::shared_ptr_ref<TAppData>& _app,
      const std::string& _endpointId,
      const std::string& _serviceId)
   {
      auto service = _app.ref.ServiceStates.Array.find(_serviceId);
      if (_app.ref.ServiceStates.Array.end() != service)
      {
         auto endpoint = service->second.Endpoints.find(_endpointId);
         if (service->second.Endpoints.end() != endpoint)
            return { true, endpoint->second };
      }

      return { false, { TEndpointState::Detached, false } };
   }
}

void AmgCall TApplication::DoSwitchEndpointsConnection(
   bool _doConnect,
   const NUtils::shared_ptr_ref<TAppData>& _app,
   const std::string& _serviceId,
   const std::string& _initiatorId)
{
   THttpUri uri;
   uri.SetPath(NRestconf::RootResourcePath() + ("service=" + _serviceId));

   std::weak_ptr<TAppData> weakApp = _app.ptr;

   const auto& endpoints = _app.ref.ServiceStates[_serviceId].Endpoints;
   for (auto& item : endpoints)
   {
      if (!item.second.Attached || item.second.IsConnected == _doConnect)
         continue;

      if (_initiatorId != item.first)
      {
         auto& sData = _app.ref.ServiceDatas[_serviceId];
         auto epData = sData.Endpoints.find(item.first);
         if (sData.Endpoints.end() == epData || !epData->second.AutoConnect)
            continue;
      }

      auto epIds = NUtils::make_shared_cptr_cref<TIdsCapture>(_serviceId, "", item.first);

      auto state = item.second;
      state.IsConnected = _doConnect;
      auto  device = _app.ref.DeviceData.Find(epIds.ref.DeviceId);
      std::string hostId = device->Components.begin()->second.HostId;
      uri.SetAddress(TNetworkAddress(_app.ref.Hosts[hostId].Address));
      NRestconf::SendPutRequest(_app.ref.ClientService, uri, NRestconf::BuildConnectionState(_doConnect),
         [weakApp, epIds, state](bool _success)
      {
         if (!_success) return;

         std::shared_ptr<TAppData> app = weakApp.lock();
         if (!app) return;

         TEndpointState::ToCache(app->Bud, app->ServiceStates.Path, epIds.ref.ServiceId, epIds.ref.DeviceId, state);
      },
         epIds.ref.DeviceId, uri.Path()[uri.Path().Count() - 1]);
   }
}

void AmgCall TApplication::DoChangeEndpointsMode(
   const NUtils::shared_ptr_ref<TAppData>& _app,
   const NUtils::shared_cptr_cref<TIdsCapture>& _ids)
{
   auto uri = NUtils::make_shared_ptr_ref<THttpUri>();
   uri.ref.SetPath(NRestconf::RootResourcePath() + ("service=" + _ids.ref.ServiceId));

   std::weak_ptr<TAppData> weakApp = _app.ptr;
   static auto SendCurrentState = [](
      const NUtils::shared_ptr_ref<TAppData>& _app,
      const NUtils::shared_cptr_cref<TIdsCapture>& _ids,
      THttpUri& _uri, bool _doConnected)
   {
      const NAmg::NCakApi::TDeviceData* device =
         _app.ref.DeviceData.Find(_ids.ref.DeviceId); 
      const TExtraDeviceType* type = _app.ref.ExtraDeviceTypes.Find(device->Type);   
      auto hostId = device->Components.find(type->ComponentId)->second.HostId;
      _uri.SetAddress(TNetworkAddress(_app.ref.Hosts[hostId].Address));

      NRestconf::SendPutRequest(
         _app.ref.ClientService
         , _uri
         , NRestconf::BuildServiceState(_ids.ref.ModeId, _doConnected)
         , [_app, _ids, _doConnected](bool _success)
            {
               if (_success)
                  TEndpointState::ToCache(
                     _app.ref.Bud
                     , _app.ref.ServiceStates.Path
                     , _ids.ref.ServiceId
                     , _ids.ref.DeviceId
                     , _doConnected);
            }
         , _ids.ref.DeviceId
         , _uri.Path()[_uri.Path().Count() - 1]);
   };

   const auto& epStates = _app.ref.ServiceStates[_ids.ref.ServiceId].Endpoints;
   const auto& epDatas = _app.ref.ServiceDatas[_ids.ref.ServiceId].Endpoints;
   for (auto& item : epStates)
   {
      auto data = epDatas.find(item.first);
      bool doConnect = !_ids.ref.ModeId.empty() && (epDatas.end() != data && data->second.AutoConnect);

      auto epIds = NUtils::make_shared_cptr_cref<TIdsCapture>(_ids.ref.ServiceId, _ids.ref.ModeId, item.first);
      if (TEndpointState::Attached != item.second.Value)
      {
		  if (_ids.ref.ModeId.empty())
			  return;

         DoAttachEndpoint(_app, epIds, [weakApp, epIds, uri, doConnect](bool succsess)
         {
            if (!succsess) return;

            std::shared_ptr<TAppData> app = weakApp.lock();
            if (!app) return;

            SendCurrentState(NUtils::shared_ptr_ref<TAppData>::make(std::move(app)), epIds, uri.ref, doConnect);
         });
      }
      else
      {
         SendCurrentState(_app, epIds, uri.ref, doConnect);
      }
   }
}

void AmgCall TApplication::DoAttachEndpoint(
   const NUtils::shared_ptr_ref<TAppData>& _app,
   const NUtils::shared_cptr_cref<TIdsCapture>& _ids,
   TOnResult&& _onResult)
{
   //Запрос на добавление терминала
   auto state = EndpointState(_app, _ids.ref.DeviceId, _ids.ref.ServiceId);
   if (state.first)
   {
      if (
         TEndpointState::Attached == state.second.Value ||
         TEndpointState::Attaching == state.second.Value)
      {
         _onResult(true);
         return;
      }

      if (TEndpointState::Detached != state.second.Value &&
		   TEndpointState::Rejected != state.second.Value)
      {
         _onResult(false);
         return;
      }
   }

   TEndpointState::ToCache(_app.ref.Bud, _app.ref.ServiceStates.Path, _ids.ref.ServiceId, _ids.ref.DeviceId, TEndpointState::Attaching);

   std::weak_ptr<TAppData> weakApp = _app.ptr;
   auto  device = _app.ref.DeviceData.Find(_ids.ref.DeviceId);
   if(nullptr == device)
   {
      amgDebug.Show("Undefined device id!", errl_Warning);
      _onResult(false);
      return;
   }
   const TExtraDeviceType* type = _app.ref.ExtraDeviceTypes.Find(device->Type);
   if(nullptr == type)
   {
      amgDebug.Show("Undefined extra device type id!", errl_Warning);
      _onResult(false);
      return;
   }
   auto component = device->Components.find(type->ComponentId);
   if (device->Components.end() == component)
   {
      amgDebug.Show("Undefined component id!", errl_Warning);
      _onResult(false);
      return;
   }
   auto hostId = component->second.HostId;   
   auto host = _app.ref.Hosts.Find(hostId);
   if(nullptr == host)
   {
      amgDebug.Show("Undefined host id!", errl_Warning);
      _onResult(false);
      return;
   }
   auto hostAddress = TNetworkAddress(host->Address);

   NRestconf::SendAddService(
      _app.ref.ClientService, _ids.ref.DeviceId, hostAddress,
      _ids.ref.ServiceId, _app.ref.ServiceDatas[_ids.ref.ServiceId],
      _app.ref.ServiceStates.Array,
      _app.ref.FrequencyRanges.Array,
      [weakApp, _ids, hostAddress, _onResult](bool _requestResult)
   {
      std::shared_ptr<TAppData> app = weakApp.lock();
      if (!app)
         return;

      auto appData = NUtils::shared_ptr_ref<TAppData>::make(std::move(app));

      auto value = _requestResult ? TEndpointState::Attached : TEndpointState::Rejected;
      TEndpointState::ToCache(appData.ref.Bud, appData.ref.ServiceStates.Path, _ids.ref.ServiceId, _ids.ref.DeviceId, value);

      if (_requestResult)
      {
         static const std::string filter("contains(/notification/netconf-config-change/edit/target/text(), 'service')");

         //пока нет рекурсивных лямбд
         static void(*onNotification)(const std::weak_ptr<TAppData>&, decltype(_ids), const TNetworkAddress&, const std::string&)  =
            [](const std::weak_ptr<TAppData>& weakApp, decltype(_ids) _ids, const TNetworkAddress& _address, const std::string& _target)
         {
            std::shared_ptr<TAppData> app = weakApp.lock();
            if (!app) return;

            auto subscription = app->Subscriptions.find(_ids.ref.DeviceId);
            if (app->Subscriptions.end() == subscription)
               return;

            auto& client = app->ClientService;

            NRestconf::SendSubscribeRequest(client, _ids.ref.DeviceId, _address, filter,
               [weakApp, _ids, _address](const std::string& _target)
            { onNotification(weakApp, _ids, _address, _target); });

            THttpUri uri;
            uri.SetPath(NYang::TRestconfPath::DatastoreRes() + "/" + _target);

            assert(uri.Path().Count() == NRestconf::RootResourcePath().Count() + 1);

            uri.SetAddress(_address);
            uri.Headers()["depth"] = "2";

            NRestconf::SendGetRequest(client, uri, _ids.ref.DeviceId, [weakApp, _ids](const std::string& _body)
            {
               std::shared_ptr<TAppData> app = weakApp.lock();
               if (!app) return;

               auto appData = NUtils::shared_ptr_ref<TAppData>::make(std::move(app));

               auto awaited = NRestconf::ParseAwaitedState(_body);
               if (awaited.IsConnected.first)
               {
                  TEndpointState::RequestToUpdate(appData.ref.Bud, appData.ref.ServiceStates.Path, awaited.ServiceId, _ids.ref.DeviceId, awaited.IsConnected.second);
               }

               if (!awaited.ModeId.first)
                  return;

               if (awaited.ModeId.second.empty())
                  TServiceState::RequestToDestroy(appData.ref.Bud, appData.ref.ServiceStates, awaited.ServiceId);
               else
                  TServiceState::RequestToBuild(appData.ref.Bud, appData.ref.ServiceStates, awaited.ServiceId, awaited.ModeId.second);
            });
         };

         auto inserted = appData.ref.Subscriptions.insert(_ids.ref.DeviceId);
         if (inserted.second)
         {
            NRestconf::SendSubscribeRequest(appData.ref.ClientService, _ids.ref.DeviceId, hostAddress, filter,
               [weakApp, _ids, hostAddress](const std::string& _target)
            { onNotification(weakApp, _ids, hostAddress, _target); });
         }
      }

      _onResult(_requestResult);
   });
}

void AmgCall TApplication::DoDetachEndpoint(
   const NUtils::shared_ptr_ref<TAppData>& _app,
   const NUtils::shared_cptr_cref<TIdsCapture>& _ids,
   TOnResult&& _onResult)
{
   //Запрос на удаление терминала
   auto state = EndpointState(_app, _ids.ref.DeviceId, _ids.ref.ServiceId);
   if (state.first)
   {
      if (TEndpointState::Attaching == state.second.Value)
      {
         _onResult(false);
         return;
      }

      if (TEndpointState::Rejected == state.second.Value)
      {
         _onResult(true);
         TEndpointState::ToCache(_app.ref.Bud, _app.ref.ServiceStates.Path, _ids.ref.ServiceId, _ids.ref.DeviceId, TEndpointState::Detached);
         return;
      }
   }

   if (!state.first ||
      TEndpointState::Detaching == state.second.Value ||
      TEndpointState::Detached == state.second.Value )
   {
      _onResult(true);
      return;
   }

   TEndpointState::ToCache(_app.ref.Bud, _app.ref.ServiceStates.Path, _ids.ref.ServiceId, _ids.ref.DeviceId, TEndpointState::Detaching);

   _app.ref.Subscriptions.erase(_ids.ref.DeviceId);

   std::weak_ptr<TAppData> weakApp = _app.ptr;
   
   auto device = _app.ref.DeviceData.Find(_ids.ref.DeviceId);
   const TExtraDeviceType* type = _app.ref.ExtraDeviceTypes.Find(device->Type);   
   auto hostId = device->Components.find(type->ComponentId)->second.HostId;
   NRestconf::SendDeleteService(_app.ref.ClientService, _ids.ref.DeviceId, TSocketAddress(_app.ref.Hosts[hostId].Address)
      , _ids.ref.ServiceId
      , [weakApp, _ids, _onResult](bool _requestResult)
   {
      std::shared_ptr<TAppData> app = weakApp.lock();
      if (!app)
         return;

      TEndpointState state{ TEndpointState::Detached, false };
      TEndpointState::ToCache(app->Bud, app->ServiceStates.Path, _ids.ref.ServiceId, _ids.ref.DeviceId, state);

      _onResult(_requestResult);
   });
}

void AmgCall TApplication::DoBuildMode(
   const NUtils::shared_ptr_ref<TAppData>& _app,
   const NUtils::shared_cptr_cref<TIdsCapture>& _ids,
   const TModeData& _modeData,
   TOnResult&& _onResult)
{
   //Запрос на переключение режима тракта (только если уже был задан режим - не из пустого)
   TServiceState::AwaitedToCache(_app.ref.Bud, _app.ref.ServiceStates, _ids.ref.ServiceId, _ids.ref.ModeId);

   std::weak_ptr<TAppData> weakApp = _app.ptr;
   std::shared_ptr<TModeState> modeState{ new TModeState() };
   modeState->BuildPresets.resize(_modeData.BuildPresets.size());

   size_t index = 0;
   for (auto presetIt : _modeData.BuildPresets)
   {
      modeState->BuildPresets[index] = TModeState::Pending;

      try
      {
         DoApplyPreset(_app, presetIt, [weakApp, _ids, index, modeState, _onResult](TModeState::TPresetState _presetState)
         {
            std::shared_ptr<TAppData> app = weakApp.lock();
            if (!app)
               return;

            modeState->BuildPresets[index] = _presetState;

            auto appData = NUtils::shared_ptr_ref<TAppData>::make(std::move(app));
            TModeState::ToCache(appData.ref.Bud, appData.ref.ServiceStates.Path, _ids.ref.ServiceId, *modeState);

            if (modeState->Completed())
            {
               _onResult(modeState->Successfull());
            }
         });
      }
      catch (const std::exception& _err)
      {
         amgDebug.Show(_err.what(), "TApplication::DoBuildMode", errl_Critical);

         modeState->BuildPresets[index] = TModeState::Rejected;

         std::shared_ptr<TAppData> app = weakApp.lock();
         if (!app)
            return;
         TModeState::ToCache(app->Bud, app->ServiceStates.Path, _ids.ref.ServiceId, *modeState);

         if (modeState->Completed())
         {
            _onResult(modeState->Successfull());
            return;
         }
      }
      catch(const NAmg::TIError& _err)
      {
         amgDebug.Show(_err, "TApplication::DoBuildMode",errl_Critical);

         modeState->BuildPresets[index] = TModeState::Rejected;

         std::shared_ptr<TAppData> app = weakApp.lock();
         if (!app)
            return;
         TModeState::ToCache(app->Bud, app->ServiceStates.Path, _ids.ref.ServiceId, *modeState);

         if (modeState->Completed())
         {
            _onResult(modeState->Successfull());
            return;
         }
      }

      ++index;
   }

   //отправляем обновление о том, что начали строить режим
   TModeState::ToCache(_app.ref.Bud, _app.ref.ServiceStates.Path, _ids.ref.ServiceId, *modeState);

   if (_modeData.BuildPresets.empty())
   {
      _onResult(true);
   }
}

void AmgCall TApplication::DoDestroyMode(const NUtils::shared_ptr_ref<TAppData>& _app,
                                         const NUtils::shared_cptr_cref<TIdsCapture>& _ids,
                                         const TModeData& _modeData,
                                         TOnResult&& _onResult)
{
   assert(_ids.ref.ModeId.empty());

   TServiceState::AwaitedToCache(_app.ref.Bud, _app.ref.ServiceStates, _ids.ref.ServiceId, _ids.ref.ModeId);

   if (_modeData.DestroyPresets.empty())
   {
      _onResult(true);
      return;
   }

   std::weak_ptr<TAppData> weakApp = _app.ptr;
   std::shared_ptr<TModeState> modeState{new TModeState()};
   modeState->DestroyPresets.resize(_modeData.DestroyPresets.size());

   size_t index = 0;

   for (auto presetIt : _modeData.DestroyPresets)
   {
      modeState->DestroyPresets[index] = TModeState::Pending;
      DoApplyPreset(_app, presetIt, [weakApp, _ids, index, modeState, _onResult] (TModeState::TPresetState _presetState)
         {
            std::shared_ptr<TAppData> app = weakApp.lock();
            if (!app)
               return;

            auto appData = NUtils::shared_ptr_ref<TAppData>::make(std::move(app));

            modeState->DestroyPresets[index] = _presetState;

            TModeState::ToCache(appData.ref.Bud, appData.ref.ServiceStates.Path, _ids.ref.ServiceId, *modeState);

            if (modeState->Completed())
            {
               _onResult(modeState->Successfull());
            }
         });

      ++index;
   }
}

void AmgCall TApplication::DoApplyPreset(const NUtils::shared_ptr_ref<TAppData>& _app,
                                         const TModeData::TPresetData& _preset,
                                         std::function<void(TModeState::TPresetState)> _onResult)
{
   auto onResultAdapter = [_onResult](bool _bResult)
   {
      _onResult(_bResult ? TModeState::Accepted : TModeState::Rejected);
   };

   const NAmg::NCakApi::TPresetManager::TSenderFunction function =
      [_app, &onResultAdapter](const NCakApi::TSpecificPreset& _preset, const NAmg::THttpUri& _uri, const std::string& _body)
      {
         if (_body.empty())
         {
            //ЗАГЛУШКА: если тело предустановки пустое, то не отправляем запросов на агенты,
            //          а сразу считаем успешно примененным
            onResultAdapter(true);
            return;
         }
         NRestconf::SendPutRequest(_app.ref.ClientService, _uri, _body, onResultAdapter, _preset);
      };

   const auto& host = _app.ref.Hosts[_preset.HostId];
   NAmg::NCakApi::TPresetManager::THostParams params(_preset.HostId, host.Type, host.Address);
   _app.ref.PresetManager.ApplyPreset(function, _preset.Preset, params);
}

std::string AmgCall TApplication::DefaultMode(const NUtils::shared_ptr_ref<TAppData>& _app, const std::string& _serviceId)
{
   auto service = _app.ref.ServiceDatas.Array.find(_serviceId);
   if (_app.ref.ServiceDatas.Array.end() == service || service->second.Modes.empty())
      return std::string();

   return service->second.Modes.begin()->first;
}

void AmgCall TApplication::DoSwitchLocking(const NUtils::shared_ptr_ref<TAppData>& _app, const NUtils::shared_cptr_cref<TIdsCapture>& _ids, bool _doLock)
{
   auto& modeId = _doLock ? _ids.ref.ModeId : _app.ref.ServiceStates[_ids.ref.ServiceId].CurrentModeId;

   assert(!modeId.empty());

   auto& modeData = _app.ref.ServiceDatas[_ids.ref.ServiceId].Modes.at(modeId);

   auto& inSet = _app.ref.ServiceSetDatas[_app.ref.ServiceSetState.Section.CurrentSetId].ServiceIds;

   for (auto& target : modeData.BuildPresets)
   {
      auto targetPath = NAmg::NCakApi::TPresetManager::SubstituteUriParams(_app.ref.PresetManager, target.Preset).Path();
      for (auto& serviceData : _app.ref.ServiceDatas.Array)
      {
         if (_ids.ref.ServiceId == serviceData.first) continue;

         if (inSet.end() == std::find(inSet.begin(), inSet.end(), serviceData.first)) continue;

         std::forward_list<std::string> update;

         for (auto& mode : serviceData.second.Modes)
         {
            for (auto& build : mode.second.BuildPresets)
            {
               if (target.HostId != build.HostId) continue;

               auto path = NAmg::NCakApi::TPresetManager::SubstituteUriParams(_app.ref.PresetManager, build.Preset).Path();
               auto relation = targetPath.RelateTo(path);
               if (NAmg::TPath::NotRelated == relation || NAmg::TPath::HasCommon == relation) continue;

               update.push_front(mode.first);
               break;
            }
         }

         if (!update.empty())
            TServiceState::LockingToCache(_app.ref.Bud, _app.ref.ServiceStates, serviceData.first, update, _doLock);
      }
   }
}

void AmgCallBack TApplication::Cb_AfterInput(char ch)
{
   switch(ch)
   {
   case 's':
      Application().SaveConfiguration();
      break;
   case 'q':
      Terminate();
      break;
   default:
      break;
   }
}
