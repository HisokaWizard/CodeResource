#include "precomp.h"
//
#include <JsonCpp/json.h>
#include <Models/EquipmentModule.h>
#include <amgFile.h>
#include <Debug/amgDebug.h>
#include <Link/THttpUri.h>
#include <HTTP/sclHttpFolderControl.h>
#include <Log/Log.h>
//
#include <iostream>
#include <random>
#include <ctime>
#include <sstream>
//
#include "Application.h"
//
#ifdef _MSC_VER
   #pragma pack(16)
#endif
//
namespace NAmg
{
   DefineApplication(NAmg::TApp);
   //
   using namespace NAmg::NScl;
   //using namespace NScl::NYang;
   void TBudSettings::ToXml(const TBudSettings& _settings, NAmg::NXml::TElement& _xml)
   {
      NAmg::NCakApi::TBud::TSettings::ToXml(_settings, _xml);
      auto time_interval = std::unique_ptr<NXml::TElement>(new NXml::TElement(NAmg::TAString("TimeInterval")));
      time_interval->AddAttr("smr-device-interval", TAString::IntToStr(_settings.SmrDeviceInterval));
      time_interval->AddAttr("multigraph-interval", TAString::IntToStr(_settings.MultigraphInterval));
      _xml.AddChild(*time_interval.release());
      //
      auto testpath = std::unique_ptr<NXml::TElement>(new NXml::TElement(NAmg::TAString("TestBudMobilePath")));
      testpath->AddAttr("test-path", _settings.TestBudMobilePath);
      _xml.AddChild(*testpath.release());
   }
   //
   void TBudSettings::FromXml(const NAmg::NXml::TElement& _xml, TBudSettings& _settings)
   {
      NAmg::NCakApi::TBud::TSettings::FromXml(_xml, _settings);
      if (0 != _xml.ElemCount("TimeInterval"))
      {
         auto& time_interval = _xml.ElChild(0, "TimeInterval");
         auto attrSmr = time_interval.FindAttr("smr-device-interval");
         auto attrMultigraph = time_interval.FindAttr("multigraph-interval");
         if ((nullptr != attrMultigraph) && (nullptr != attrMultigraph))
         {
            try
            {
               _settings.SmrDeviceInterval = attrSmr->AsInteger();
               _settings.MultigraphInterval = attrMultigraph->AsInteger();
            }
            catch (const TIError&)
            {
            }
         }
      }
      if (0 != _xml.ElemCount("TestBudMobilePath"))
      {
         auto& testpath = _xml.ElChild(0, "TestBudMobilePath");
         auto attrTest = testpath.FindAttr("test-path");
         if (nullptr != attrTest)
         {
            try
            {
               _settings.TestBudMobilePath = attrTest->AsTString();
            }
            catch (const TIError&)
            {
            }
         }
      }
   }
   //
   AmgCreate TAppData::TAppData()
      : BudParameters()
      , Bud("mobile")
      , Hosts(Bud.Cache(), "/hosts")
      , MediaTypes(Bud.Cache(), "/media")
      , User(Bud.Cache(), "/users")
      , HttpClient(Bud.Worker())
      , SmrSources(Bud.Cache(), "/bud/mobile/smr")
      , Location(Bud.Cache(), "/locations")
      , Scheme(Bud.Cache(), "/bud/mobile/scheme")
      , BioGroup(Bud.Cache(), "/bud/mobile/biogroup")
      , EndPointData(Bud.Cache(), "/bud/mobile/endpoints/data")
      , EndPointState(Bud.Cache(), "/bud/mobile/endpoints/state")
      , EndPointAvailability()
      , MultigraphAvailability()
      , Interval(5000)
   {
   }
   //
   AmgDestroy TAppData::~TAppData()
   {
   }
   //
   AmgCreate TApp::TApp(void)
      : NAmg::NApplication::TServiceApplication(),
      AppDataPTR(std::make_shared<TAppData>())
   {
      FConsole.AfterInput =
         NAmg::NApplication::TConsole::TAfterInput::TClosure(
            *this, &TApp::Cb_AfterInput);
   }
   //
   //void AmgCall TAppData::LoadFromSchemeFile(TAppData& _app, const NAmg::TPath& _fileName)
   //{
   //   NSmart::TDiskFileReader::TREF reader(NSmart::TDiskFileReader::Create(_fileName));
   //   NAmg::NXml::TDocument doc;
   //   doc.LoadFile(reader);
   //   try
   //   {
   //      NCakApi::TScheme::TAdapter::TArray tmpValue;
   //      NCakApi::TScheme::TAdapter::FromXml(doc.RootElement(), "Scheme", tmpValue);
   //      NAmg::NScl::NHttp::TQueueUpdate update;
   //      _app.Scheme.Prepare(tmpValue, update);
   //      _app.Bud.Update(update);
   //   }
   //   catch (const NAmg::TIError& _err)
   //   {
   //      amgDebug.Show("Cannot process scheme in " + _fileName, _err().Message(), errl_Warning);
   //   }
   //}
   ////
   //void AmgCall TAppData::SaveToSchemeFile(TAppData&, const NAmg::TPath&)
   //{}
   //
   void AmgCall TAppData::LoadSchemesFromFile(TAppData& _app, const NAmg::TPath& _fileName)
   {
      NSmart::TDiskFileReader::TREF reader(NSmart::TDiskFileReader::Create(_fileName));
      NAmg::NXml::TDocument doc;
      doc.LoadFile(reader);
      NAmg::NScl::NHttp::TQueueUpdate plansUpdate;
      NAmg::NScl::NHttp::TQueueUpdate usersUpdate;
      NAmg::NScl::NHttp::TQueueUpdate camerasUpdate;
      try
      {
         NAmg::NCakApi::TScheme::TAdapter::TArray tmpValue;
         for (TVectorIndex i = 0; doc.RootElement().ElemCount("Plan") > i; ++i)
         {
            NAmg::NCakApi::TScheme plan;
            const auto& xmlPlan = doc.RootElement().ElChild(i);
            auto* id = xmlPlan.FindAttr("id");
            NAmg::NCakApi::TScheme::FromXml(xmlPlan, plan);
            tmpValue.insert({ id->AsTString(), plan });
         }
         _app.Scheme.Prepare(tmpValue, plansUpdate);
      }
      catch (const NAmg::TIError& _err)
      {
         amgDebug.Show("Cannot process data in " + _fileName, _err().Message(), errl_Warning);
      }

      try
      {
         _app.Bud.Update(plansUpdate);
      }
      catch (const NAmg::TIError& _err)
      {
         amgDebug.Show("Cannot process data in " + _fileName, _err().Message(), errl_Warning);
      }
      catch (const std::exception& _err)
      {
         amgDebug.Show("Cannot process data in " + _fileName + " : " + _err.what(), errl_Warning);
      }
   }
   //
   void AmgCall TAppData::SaveSchemesToFile(const TAppData& _app, const NAmg::TPath& _fileName)
   {
      NSmart::TDiskFileWriter::TREF writer(NSmart::TDiskFileWriter::Create(_fileName));
      NXml::TDocument doc;
      doc.AddChild(*(new NXml::TDeclaration("1.0", "UTF-8", "")));

      std::unique_ptr<NXml::TElement> xmlObservers(new NXml::TElement("Observers"));
      for (const auto& item : _app.Scheme.Array)
      {
         std::unique_ptr<NXml::TElement> xmlObserver(new NXml::TElement("Observer"));
         NXml::TAttribute id("id", item.first);
         xmlObserver->AddAttr(id);
         NAmg::NCakApi::TScheme::ToXml(item.second, *xmlObserver);
         xmlObservers->AddChild(*xmlObserver.release());
      }

      std::unique_ptr<NXml::TElement> xmlVCSData(new NXml::TElement("VCSData"));
      xmlVCSData->AddChild(*xmlObservers.release());
      doc.AddChild(*xmlVCSData.release());
      doc.SaveFile(writer);
   }
   //
   void AmgCall TAppData::LoadFromFile(TAppData& _app, const NAmg::TPath& _fileName)
   {
      NSmart::TDiskFileReader::TREF reader(NSmart::TDiskFileReader::Create(_fileName));
      NAmg::NXml::TDocument doc;
      doc.LoadFile(reader);
      //
      try
      {
         TSmrSource::TAdapter::TArray tmpValue;
         TSmrSource::TAdapter::FromXml(doc.RootElement(), "SmrSource", tmpValue);
         NAmg::NScl::NHttp::TQueueUpdate update;
         _app.SmrSources.Prepare(tmpValue, update);
         _app.Bud.Update(update);
      }
      catch (const NAmg::TIError& _err)
      {
         amgDebug.Show("Cannot process smrsources in " + _fileName, _err().Message(), errl_Warning);
      }
      //
      try
      {
         NCakApi::NNms::TLocation::TAdapter::TArray tmpValue;
         NCakApi::NNms::TLocation::TAdapter::FromXml(doc.RootElement().ElChild(0, "Locations"), "Location", tmpValue);
         NAmg::NScl::NHttp::TQueueUpdate update;
         _app.Location.Prepare(tmpValue, update);
         _app.Bud.Update(update);
      }
      catch (const NAmg::TIError& _err)
      {
         amgDebug.Show("Cannot process location in " + _fileName, _err().Message(), errl_Warning);
      }
      //
      try
      {
         TBioGroup::TAdapter::TArray tmpValue;
         TBioGroup::TAdapter::FromXml(doc.RootElement().ElChild(0, "BioGroups"), "BioGroup", tmpValue);
         NAmg::NScl::NHttp::TQueueUpdate update;
         _app.BioGroup.Prepare(tmpValue, update);
         _app.Bud.Update(update);
      }
      catch (const NAmg::TIError& _err)
      {
         amgDebug.Show("Cannot process biogroup in " + _fileName, _err().Message(), errl_Warning);
      }
      try
      {
         TEndPointData::TAdapter::TArray tmpValue;
         TEndPointData::TAdapter::FromXml(doc.RootElement().ElChild(0, "EndPoints"), "EndPoint", tmpValue);
         NAmg::NScl::NHttp::TQueueUpdate update;
         _app.EndPointData.Prepare(tmpValue, update);
         _app.Bud.Update(update);
         //
         TEndPointState::TAdapter::TArray tmpStateValue;
         TEndPointState::StartInit(tmpStateValue, tmpValue);
         NAmg::NScl::NHttp::TQueueUpdate updatestate;
         _app.EndPointState.Prepare(tmpStateValue, updatestate);
         _app.Bud.Update(updatestate);
      }
      catch (const NAmg::TIError& _err)
      {
         amgDebug.Show("Cannot process endpoint in " + _fileName, _err().Message(), errl_Warning);
      }
   }
   //
   void AmgCall TAppData::SaveToFile(const TAppData& , const NAmg::TPath& _fileName)
   {
      NSmart::TDiskFileWriter::TREF writer(NSmart::TDiskFileWriter::Create(_fileName));
      NXml::TDocument doc;
      doc.AddChild(*(new NXml::TDeclaration("1.0", "UTF-8", "")));
   }
   //
   AmgDestroy TApp::~TApp(void)
   {
   }
   //
   void AmgCall TApp::DoLoadConfiguration(void)
   {
      NAmg::NApplication::TServiceApplication::DoLoadConfiguration();
      //
      auto configurationPath = FindConfig("Configuration.xml");
      try
      {
         NXml::TDocument doc;
         doc.LoadFile(configurationPath);
         //NCakApi::TBud::TSettings::FromXml(doc.RootElement(), AppDataPTR->BudParameters.Settings);
         TBudSettings::FromXml(doc.RootElement(), AppDataPTR->BudParameters.Settings);

         //если файлы для кэша заданы относительные, то смотрим их относительно папки с программой,
         //а не из "текущей" папки, откуда запускали
         auto& localCache = AppDataPTR->BudParameters.Settings.LocalFileCache;
         if (!localCache.FileToReadPath.IsEmpty() && !localCache.FileToReadPath.IsAbsolute())
            localCache.FileToReadPath = RootPath() + localCache.FileToReadPath;
         if (!localCache.FileToWritePath.IsEmpty() && !localCache.FileToWritePath.IsAbsolute())
            localCache.FileToWritePath = RootPath() + localCache.FileToWritePath;
      }
      catch (NCakApi::TXmlParsingException& e)
      {
         amgDebug.Show("Failed to load " + configurationPath + "\nReason: " + e.what(), errl_Warning);
      }
      catch (const TFileError::TError& _error)
      {
         amgDebug.Show(_error);
      }
      catch (...)
      {
         amgDebug.Show("Error loading configuration!", errl_Warning);
      }
   }
   //
   void AmgCall TApp::DoSaveConfiguration(void)
   {
      NAmg::NApplication::TServiceApplication::DoSaveConfiguration();
      //
      auto configurationPath = FindConfig("Configuration.xml");
      try
      {
         NXml::TDocument doc;
         std::unique_ptr<NXml::TElement> root{ new NXml::TElement("Configuration") };
         //NCakApi::TBud::TSettings::ToXml(AppDataPTR->BudParameters.Settings, *root);
         TBudSettings::ToXml(AppDataPTR->BudParameters.Settings, *root);
         doc.AddChild(*root.release());
         doc.SaveFile(configurationPath);
      }
      catch (const TFileError::TError& _error)
      {
         amgDebug.Show(_error);
      }
      catch (...)
      {
         amgDebug.Show("Error saving configuration!", errl_Warning);
      }
      TAppData::SaveSchemesToFile(*AppDataPTR, FindConfig("Schemes.xml"));
   }
   //
namespace
{
   void AmgCall BudMobileTimeout(std::shared_ptr<TAppData>& _app)
   {
      if (!_app)
         return;
      std::weak_ptr<TAppData> weakApp = _app;
      _app->Bud.Worker().Timeout(_app->Interval, [weakApp](const TDateTime&) mutable
      {
         std::shared_ptr<TAppData> app = weakApp.lock();
         if (!app)
            return;
         TEndPointState::TAdapter::TArray tmpEndPointArray = app->EndPointState.Array;
         try {
            for (auto& endpoint : app->EndPointData.Array)
            {
               auto EndPointIt = tmpEndPointArray.find(endpoint.first);
               auto time_now = TDateTime();
               auto time_endpoint = app->EndPointAvailability[EndPointIt->second.UserId];
               if ((time_now - time_endpoint) > app->BudParameters.Settings.SmrDeviceInterval)
               {
                  if (false == EndPointIt->second.SMRDeviceEnabled)
                     continue;
                  EndPointIt->second.SMRDeviceEnabled = false;
                  NAmg::NScl::NHttp::TQueueUpdate updatestate;
                  app->EndPointState.PrepareUpdate(EndPointIt->first, EndPointIt->second, updatestate);
                  app->Bud.Update(updatestate);
               }
            }
            for (auto& endpoint : app->EndPointData.Array)
            {
               auto EndPointIt = tmpEndPointArray.find(endpoint.first);
               auto time_now = TDateTime();
               auto time_multigraph = app->MultigraphAvailability[EndPointIt->second.UserId];
               if ((time_now - time_multigraph) > app->BudParameters.Settings.MultigraphInterval)
               {
                  if (false == EndPointIt->second.MultigraphEnabled)
                     continue;
                  EndPointIt->second.MultigraphEnabled = false;
                  NAmg::NScl::NHttp::TQueueUpdate updatestate;
                  app->EndPointState.PrepareUpdate(EndPointIt->first, EndPointIt->second, updatestate);
                  app->Bud.Update(updatestate);
               }
            }
         }
         catch (const TIError& _err)
         {
            amgDebug.Show(_err, "BudMobileTimeout");
         }
         BudMobileTimeout(app);
      });
   }
};
//
namespace
{
   void AmgCall DoPoll(std::shared_ptr<TAppData>& _app, const std::string& _id)
   {
      std::weak_ptr<TAppData> weakApp = _app;
      auto smrIt = _app->SmrSources.Array.find(_id); // находим СМР с таким id какой передали из кеша (id именно СМРа (smr1, smr2 и т д))
      if (_app->SmrSources.Array.end() == smrIt)     // если до конца списка такого СМРа нет тогда выходим
         return;
      _app->Bud.Worker().Timeout(smrIt->second.Interval, [weakApp, _id] (const TDateTime&) mutable
      {
         std::shared_ptr<TAppData> app = weakApp.lock(); // Проверяем существует ли еще объект которым владеет app
         if (!app)                                       // Если нет то выходим
            return;
         auto smrIt = app->SmrSources.Array.find(_id);
         if (app->SmrSources.Array.end() == smrIt)
            return;
         app->HttpClient.Get(smrIt->second.Uri)
            .Then([weakApp, _id](const TResponseData& _response)
            {
               std::shared_ptr<TAppData> app = weakApp.lock();
               if (!app)
                  return;
               //
               auto smrIt = app->SmrSources.Array.find(_id);
               if (app->SmrSources.Array.end() == smrIt)
                  return;
               //
               if (_response.Body.IsNULL())  // Проверяем наличие тела у _response, если нет то выходим
                  return;
               //
               NXml::TDocument doc;
               _response.To(doc);
               TEndPointState::TAdapter::TArray tmpEndPointArray;
               //
               TEndPointState::FromSMR(doc.RootElement(), tmpEndPointArray);
               // для обновления данных
               try
               {
                  for (auto& translate : smrIt->second.Translations)
                  {
                     auto smrEndPointIt = tmpEndPointArray.find(translate.DataId); // ищем dataid из пришедших в данном SMR данных
                     auto localEndPointIt = app->EndPointData.Array.find(translate.EndPointId); // ищем в существующих пользователях id пользователя соответсвующее id из smr-translate
                     if ((tmpEndPointArray.end() != smrEndPointIt) && (app->EndPointData.Array.end() != localEndPointIt))
                     {
                        auto DeviceInitIt = app->EndPointState.Array.find(translate.EndPointId);
                        if (app->EndPointState.Array.end() != DeviceInitIt)
                        {
                           smrEndPointIt->second.MultigraphEnabled = DeviceInitIt->second.MultigraphEnabled;   // Multigraph инициализируем из кэша
                           smrEndPointIt->second.SMRDeviceEnabled = DeviceInitIt->second.SMRDeviceEnabled;     // SMRDeviceEnabled инициализируем из кэша
                           smrEndPointIt->second.SMREnabled = DeviceInitIt->second.SMREnabled;                 // SMREnabled инициализируем из кэша
                           smrEndPointIt->second.NeedAttention = DeviceInitIt->second.NeedAttention;           // NeedAttention инициализируем из кэша
                        }
                        if (translate.Ignore_Pulse) // если данные приходят с multigraph тогда мы используем их
                        {
                           smrEndPointIt->second.Pulse = app->EndPointState.Array.at(translate.EndPointId).Pulse;
                           smrEndPointIt->second.Blood = app->EndPointState.Array.at(translate.EndPointId).Blood;
                           smrEndPointIt->second.Bp = app->EndPointState.Array.at(translate.EndPointId).Bp;
                           smrEndPointIt->second.Temperature = app->EndPointState.Array.at(translate.EndPointId).Temperature;
                        }
                        //
                        auto localUserIt = app->User.Array.find(localEndPointIt->second.UserId);
                        if (app->User.Array.end() != localUserIt)
                        {
                           smrEndPointIt->second.UserId = localUserIt->first; // т к state все перетирает, нужно восстановить
                           smrEndPointIt->second.SMREnabled = true;
                           //
                           if (Application().NeedAttention(app, localUserIt->second.BioGroup, smrEndPointIt->second)) // отмечаем пользователей у которых проблеммы с биометрией
                              smrEndPointIt->second.NeedAttention = true;
                           else
                              smrEndPointIt->second.NeedAttention = false;
                           //
                           auto EndPointStateIt = app->EndPointState.Array.find(translate.EndPointId); // ищем в UsersState id соответствующий существующему пользователю
                           if (app->EndPointState.Array.end() != EndPointStateIt)
                           {
                              if (EndPointStateIt->second == smrEndPointIt->second) // если данные userStateIt и smrUserIt одинаковы, значит изменений не было, update вызывать не нужно
                              {
                                 continue;
                              }
                           }
                           //
                           app->EndPointAvailability.insert({ localEndPointIt->second.UserId, TDateTime() });   // заполняем map для проверки шлет ли что-то конкретный тмр
                           smrEndPointIt->second.SMRDeviceEnabled = true;  //включаем устройства и смр только если добрались до обновления по времени.
                           app->EndPointAvailability[localEndPointIt->second.UserId] = TDateTime();
                           //
                           NAmg::NScl::NHttp::TQueueUpdate updatestate;
                           app->EndPointState.PrepareUpdate(translate.EndPointId, smrEndPointIt->second, updatestate);
                           app->Bud.Update(updatestate);
                           //
                           //if (app->EndPointState.Array.at(EndPointStateIt->first).LocationId != smrEndPointIt->second.LocationId)  // запись в журнал об изменении позиции
                           //{
                           //   Application().NotifyCacosAboutLocation(localUserIt->second.UserName
                           //      , smrEndPointIt->second.LocationId
                           //      , app->EndPointState.Array.at(EndPointStateIt->first).LocationId
                           //      , "Location is change"
                           //      , "BudMobile"
                           //      , TDateTime());
                           //}
                           //// если данны поменялись и при этом биометрия стала плохой, то пишем об этом в журнал
                           //if (smrEndPointIt->second.NeedAttention)
                           //   Application().NotifyCacosAboutAlive(localUserIt->second.UserName, false, "Biometrics outside the critical range", "BudMobile", TDateTime());
                           //else
                           //   Application().NotifyCacosAboutAlive(localUserIt->second.UserName, true, "Biometric parameters are normal", "BudMobile", TDateTime());
                           //// если smr вновь заработал
                           //Application().NotifyCacosAboutSmrStatus("Enable", _id, "BudMobile");
                        }
                     }
                  }
               }
               catch(const TIError& _err)
               {
                  amgDebug.Show(_err, "DoPoll broken in update state from SMR");
               }
            })
            .Catch([weakApp, _id](NHttp::TErrorData)
            {
               std::shared_ptr<TAppData> app = weakApp.lock();
               if (!app)
                  return;
               auto smrIt = app->SmrSources.Array.find(_id);
               if (app->SmrSources.Array.end() == smrIt)
                  return;
               try
               {
                  TEndPointState::TAdapter::TArray tmpEndPointArray = app->EndPointState.Array;
                  for (auto& translate : smrIt->second.Translations)
                  {
                     auto smrEndPointIt = tmpEndPointArray.find(translate.EndPointId);
                     if ((tmpEndPointArray.end() != smrEndPointIt))
                     {
                        if (!smrEndPointIt->second.SMREnabled)
                           continue;
                        smrEndPointIt->second.SMREnabled = false;
                        NAmg::NScl::NHttp::TQueueUpdate updatestate;
                        app->EndPointState.PrepareUpdate(translate.EndPointId, smrEndPointIt->second, updatestate);
                        app->Bud.Update(updatestate);
                        // если smr перестал работать
                        //Application().NotifyCacosAboutSmrStatus("Disable", _id, "BudMobile");// будет слаться постоянно пока отключен хотя бы один смр
                     }
                  }
               }
               catch (const TIError& _err)
               {
                  amgDebug.Show(_err, "DoPoll broken after SMR crash!");
               }
               amgDebug.Show("DoPoll was broken", errl_Inform);
            })
               .Finally([weakApp, _id]()
            {
               std::shared_ptr<TAppData> app = weakApp.lock();
               if (!app)
                  return;
               DoPoll(app, _id);
            });
      });
   };
};
//
namespace
{
   void AmgCall TestBudMobile(std::shared_ptr<TAppData>& _app)
   {
      std::weak_ptr<TAppData> weakApp = _app;
      _app->Bud.Worker().Timeout(10000, [weakApp](const TDateTime&) mutable
      {
         std::shared_ptr<TAppData> app = weakApp.lock();
         if (!app)
            return;
         if (TAString("//192.168.10.59/Public/1/SMR2.xml") == app->BudParameters.Settings.TestBudMobilePath)
         {
            NXml::TDocument doc;
            try
            {
               doc.LoadFile(app->BudParameters.Settings.TestBudMobilePath);
               NXml::TElement& _xml = doc.RootElement();
               int new_pulse = 0;
               int new_location = 0;
               for (TVectorIndex i = 0, q = _xml.ElemCount(); q > i; i++)
               {
                  auto& item = _xml.ElChild(i, "User");
                  auto* attrId = item.FindAttr("id");
                  if (nullptr != attrId)
                  {
                     if ("2" == attrId->AsTString())
                     {
                        auto* attrPulse = item.FindAttr("pulse");
                        auto* attrLocation = item.FindAttr("location");
                        if ((nullptr != attrPulse) && (nullptr != attrLocation))
                        {
                           item.DeleteAttr("pulse");
                           item.DeleteAttr("location");
                           std::mt19937 gen((unsigned int)time(0));
                           std::uniform_int_distribution<> pulse(40, 160);
                           std::uniform_int_distribution<> location(1, 8);
                           new_pulse = pulse(gen);
                           new_location = location(gen);
                           item.AddAttr("pulse", TAString(new_pulse));
                           item.AddAttr("location", TAString(new_location));
                        }
                     }
                  }
               }
               doc.SaveFile(app->BudParameters.Settings.TestBudMobilePath);
            }
            catch (const TIError& _err)
            {
               amgDebug.Show(_err, "TestBudMobile");
            }
         }
         TestBudMobile(app);
      });
   }
};
   //
   bool AmgCall TApp::Initialize(void)
   {
      //задаем параметры бада по-умолчанию, чтобы в случае, если отсутствует конфиг и его
      //создавали в TApplication::DoLoadConfiguration, туда пошли дефолтные значения

      AppDataPTR->BudParameters.Settings.PullAddress.Host() = NAmg::LocalAddress();
      AppDataPTR->BudParameters.Settings.PullAddress.Port(4900);
      AppDataPTR->BudParameters.Settings.InterfacePort.SetPort(9000);

      if (!NApplication::TApplication::Initialize())
         return(false);

      auto mobilePath = FindConfig("budMobileConf.xml");
      //auto schemePath = FindConfig("Schemes.xml");
      AppDataPTR->mobileSchemesConfigPath = FindConfig("Schemes.xml");
      auto projectWebPath = RootPath() + "Web";
      auto commonWebPath = RootPath() + "../../Web"; //путь к папке какоса

      std::weak_ptr<TAppData> weakApp = AppDataPTR;
      auto& bud = AppDataPTR->Bud;
      bud.OnCreateCache([weakApp, mobilePath, path=AppDataPTR->mobileSchemesConfigPath]()
      {
         amgDebug.Show("Creating cache", errl_Inform);
         std::shared_ptr<TAppData> app = weakApp.lock();
         if (app)
         {
            TApp::CreateCache(app, mobilePath);
         }
      });

      bud.OnActive([]()
      {
         amgDebug.Show("Bud now active", errl_Inform);
      });

      bud.OnPassive([]()
      {
         amgDebug.Show("Bud now passive", errl_Inform);
      });

      bud.OnUpdate([](const NScl::NHttp::TQueueUpdate& _update)
      {
         amgDebug.Show("Bud update received", _update.Method + TAString::Blank() + _update.Path, errl_Inform);
      });

      AppDataPTR->SmrSources.AfterAdd([weakApp](const std::string& _id, const TSmrSource&)
      {
         std::shared_ptr<TAppData> app = weakApp.lock();
         if (app)
         {
            app->EndPointAvailability.clear();
            BudMobileTimeout(app);
            DoPoll(app, _id);
            TestBudMobile(app);
         }
      });
      //
      bud.OnInitialized([projectWebPath, commonWebPath, weakApp](NHttp::TRouteService& _http)
      {
         std::shared_ptr<TAppData> app = weakApp.lock();
         ConnectCacheAdapters(app);
         if (app)
            TApp::InitializeHttp(app, projectWebPath, commonWebPath, _http);
      });

      NAmg::NCakApi::TBud::TParameters params;
      params.Modules.Modules["mobile"].Path = "app/bud/mobile/users/mobile.js";
      params.Settings = AppDataPTR->BudParameters.Settings;
      bud.SetParameters(params, RootPath());

      return(true);
   }
   //
   void AmgCall TApp::InitializeHttp(std::shared_ptr<TAppData>& _app, const NAmg::TPath& _ownWebPath, const NAmg::TPath& _commonWebPath, NAmg::NScl::NHttp::TRouteService& _http)
   {
      std::weak_ptr<TAppData> weakApp = _app;

      //отдаем свой раздел
      _http["/app/bud/mobile/**"].Ctrl<NHttp::TFolderControl>().Base(_ownWebPath);
      //отдаем свой app.js
      _http["/app/app.js"].Ctrl<NHttp::TFolderControl>().Base(_ownWebPath);
      //отдаем свой index.html
      _http["/"].Rewrite("/app/index.html").Ctrl<NHttp::TFolderControl>().Base(_ownWebPath);
      _http["/index.html"].Rewrite("/app/index.html").Ctrl<NHttp::TFolderControl>().Base(_ownWebPath);

      //остальное берем из какоса
      _http["/img/**"].Ctrl<NHttp::TFolderControl>().Base(_commonWebPath);
      _http["/lib/**"].Ctrl<NHttp::TFolderControl>().Base(_commonWebPath);
      _http["/app/**"].Ctrl<NHttp::TFolderControl>().Base(_commonWebPath);
      //
      _app->MultigraphAvailability.clear(); // чищу при перезапуске приложения
      _http["/multigraph"]
         .Method("GET", [weakApp](const TRequestData& _data)
      {
         std::shared_ptr<TAppData> app = weakApp.lock();
         if (!app)
            return;
         NCakApi::NNms::TUser::TAdapter::TArray tmpUser = app->User.Array;
         TEndPointData::TAdapter::TArray tmpEndPointData = app->EndPointData.Array;
         TEndPointState::TAdapter::TArray tmpEndPointState = app->EndPointState.Array;
         std::string multigraphAccount = TEndPointState::FromMultiGraph(_data, tmpEndPointState, tmpEndPointData);
         for (auto& endpointdata : tmpEndPointData)
         {
            auto UserIt = tmpUser.find(endpointdata.second.UserId);
            if (tmpUser.end() != UserIt)
            {
               if (endpointdata.second.UserId == _data.Headers["Account"])
               {
                  if (Application().NeedAttention(app, UserIt->second.BioGroup, tmpEndPointState.at(endpointdata.first)))
                  {
                     tmpEndPointState.at(endpointdata.first).NeedAttention = true;
                     //Application().NotifyCacosAboutAlive(UserIt->second.UserName, false, "Biometrics outside the critical range", "BudMobile", TDateTime());
                  }
                  else
                  {
                     tmpEndPointState.at(endpointdata.first).NeedAttention = false;
                     //Application().NotifyCacosAboutAlive(UserIt->second.UserName, true, "Biometric parameters are normal", "BudMobile", TDateTime());
                  }
               }
               tmpEndPointState.at(endpointdata.first).MultigraphEnabled = true;
            }
         }
         app->MultigraphAvailability.insert({ tmpEndPointState.at(multigraphAccount).UserId , TDateTime() });
         NAmg::NScl::NHttp::TQueueUpdate update;
         app->EndPointState.PrepareUpdate(multigraphAccount, tmpEndPointState.at(multigraphAccount), update);
         app->Bud.Update(update);
         TRouteService::Send(_data, 200, "OK");
      });
   }
   //
   void AmgCall TApp::CreateCache(std::shared_ptr<TAppData>& _app, const NAmg::TPath& _mobilePath)
   {
      try
      {
         //TAppData::LoadFromSchemeFile(*_app, _schemesPath);
         TAppData::LoadSchemesFromFile(*_app, _app->mobileSchemesConfigPath);
         TAppData::LoadFromFile(*_app, _mobilePath);
      }
      catch (const NXml::TErrors::TError& err)
      {
         amgDebug.Show(err().Message() + " in " + _mobilePath, errl_Warning);
      }
      catch (const std::logic_error& err)
      {
         amgDebug.Show(TAString(err.what()) + " in " + _mobilePath, errl_Warning);
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
   //
   void AmgCall TApp::ConnectCacheAdapters(std::shared_ptr<TAppData>& _app)
   {
      NAmg::NCakApi::TBud::AutoAdapterCache(_app, _app->mobileSchemesConfigPath, "SchemeList", "Scheme", _app->Scheme);
   }
   //
   void AmgCallBack TApp::Cb_AfterInput(char ch)
   {
      switch (ch)
      {
      case 's':
         Application().SaveConfiguration();
         break;
      case 'q':
         Terminate();
         break;
      case 't':
         NotifyCacosAboutAlive("TestPerson", true,"ALIVE", "BudMobile", TDateTime());
         break;
      default:
         break;
      }
      //
   }

   void AmgCall TApp::NotifyCacosAboutAlive(TAString Author, bool Alive, TAString Description, TAString Bud, const TDateTime& RegTime)
   {
      static TAString chLive("Live");
      static TAString chAuthor("Author");
      static TAString chLiveIndicator("LiveIndicator");
      static TAString chDescription("Description");
      static TAString chBudItem("From");
      static TAString chRegTime("RegTime");
      DCreateLogRecord(rec, chLive);
      DAddFieldToRecord(rec, TAString, chAuthor, Author);
      DAddFieldToRecord(rec, bool, chLiveIndicator, Alive);
      DAddFieldToRecord(rec, TAString, chDescription, Description);
      DAddFieldToRecord(rec, TAString, chBudItem, Bud);
      DAddFieldToRecord(rec, TAString, chRegTime, RegTime);
      DAddRecordToLog(AppDataPTR->Bud.Log(), rec);
      //AppDataPTR->Bud.Notify(*rec);
   }
   //
   void AmgCall TApp::NotifyCacosAboutLocation(TAString Author, TAString real_location, TAString last_location, TAString Description, TAString Bud, const TDateTime& RegTime)
   {
      static TAString chLocation("Location");
      static TAString chAuthor("Author");
      static TAString chRealLocation("RealLocation");
      static TAString chLastLocation("LastLocation");
      static TAString chDescription("Description");
      static TAString chBudItem("From");
      static TAString chRegTime("RegTime");
      DCreateLogRecord(rec, chLocation);
      DAddFieldToRecord(rec, TAString, chAuthor, Author);
      DAddFieldToRecord(rec, TAString, chRealLocation, real_location);
      DAddFieldToRecord(rec, TAString, chLastLocation, last_location);
      DAddFieldToRecord(rec, TAString, chDescription, Description);
      DAddFieldToRecord(rec, TAString, chBudItem, Bud);
      DAddFieldToRecord(rec, TAString, chRegTime, RegTime);
      DAddRecordToLog(AppDataPTR->Bud.Log(), rec);
      //AppDataPTR->Bud.Notify(rec);
   }
   //
   void AmgCall TApp::NotifyCacosAboutSmrStatus(TAString Status, TAString Description, TAString Bud)
   {
      static TAString chSMR("SMR Device");
      static TAString chStatus("Status");
      static TAString chDescription("Description");
      static TAString chBudItem("From");
      DCreateLogRecord(rec, chSMR);
      DAddFieldToRecord(rec, TAString, chStatus, Status);
      DAddFieldToRecord(rec, TAString, chDescription, Description);
      DAddFieldToRecord(rec, TAString, chBudItem, Bud);
      DAddRecordToLog(AppDataPTR->Bud.Log(), rec);
   }
   //
   bool AmgCall TApp::NeedAttention(std::shared_ptr<TAppData>& _app, const std::string& biogroup, TEndPointState& _state)
   {
      for (auto& group : _app->BioGroup.Array)
      {
         if (group.first == biogroup)
         {
            if (((group.second.MaxPulse < _state.Pulse) || (group.second.MinPulse > _state.Pulse)))
//              && ((group.second.MaxTemperature < _state.Temperature) || (group.second.MinTemperature > _state.Temperature))
//              && ((group.second.MaxBp < _state.Bp) || (group.second.MinBp > _state.Bp))
//              && ((group.second.MaxBlood < _state.Blood) || (group.second.MinBlood > _state.Blood)))
            {
               return true;
            }
            else
            {
               return false;
            }
         }
      }
      return false;
   }
};
