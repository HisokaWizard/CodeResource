#ifndef  __BudMobile_Application_H__
#define __BudMobile_Application_H__
//
#include <amgDefs.h>
//
#include <string>
//
#include <amgServiceApplication.h>
#include <HTTP/sclHttpClientService.h>
#include <ckiBudService.h>
#include <ckiHosts.h>
#include <ckiModules.h>
#include <ckiMediaType.h>
#include <ckiPresetType.h>
#include <ckiUsers.h>
#include <ckiSchemes.h>
#include <ckiLocation.h>
#include <ckiDevices.h>
#include <ckiManagerService.h>
//
#include "EndPoint.h"
#include "Group.h"
//
#ifdef _MSC_VER
   #pragma pack(push,16)
#endif
//
namespace NAmg
{
   struct TBudSettings : public NAmg::NCakApi::TBud::TSettings
   {
      unsigned int SmrDeviceInterval = 20; // в секундах
      unsigned int MultigraphInterval = 300; // в секундах
      TAString TestBudMobilePath;
      static void ToXml(const TBudSettings&, NAmg::NXml::TElement&);
      static void FromXml(const NAmg::NXml::TElement&, TBudSettings&);
   };
   //
   struct TBudParameters
   {
      TBudSettings Settings;
   };
   //
   struct TAppData
   {
      TBudParameters BudParameters;
      //NAmg::NCakApi::TBud::TParameters BudParameters;
      NAmg::NCakApi::TBud Bud;

      NAmg::NCakApi::THost::TAdapter Hosts;
      NAmg::NCakApi::TMediaType::TAdapter MediaTypes;
      NCakApi::NNms::TUser::TAdapter User;

      NScl::NHttp::TClientService HttpClient;
      TSmrSource::TAdapter SmrSources;
      NCakApi::NNms::TLocation::TAdapter Location;
      NAmg::NCakApi::TScheme::TAdapter Scheme;
      //TScheme::TAdapter Scheme;
      TBioGroup::TAdapter BioGroup;
      TEndPointData::TAdapter EndPointData;
      TEndPointState::TAdapter EndPointState;
      //
      NAmg::TPath mobileSchemesConfigPath;
      //
      std::map<std::string, TDateTime> EndPointAvailability;
      std::map<std::string, TDateTime> MultigraphAvailability;
      unsigned int Interval;
      //
      AmgCreate TAppData();
      AmgDestroy ~TAppData();

      //static void AmgCall LoadFromSchemeFile(TAppData& _app, const NAmg::TPath& _fileName);
      //static void AmgCall SaveToSchemeFile(TAppData& _app, const NAmg::TPath& _fileName);
      static void AmgCall LoadSchemesFromFile(TAppData& _app, const NAmg::TPath& _fileName);
      static void AmgCall SaveSchemesToFile(const TAppData& _app, const NAmg::TPath& _fileName);

      static void AmgCall LoadFromFile(TAppData&, const NAmg::TPath& _fileName);
      static void AmgCall SaveToFile(const TAppData&, const NAmg::TPath& _fileName);
   };
   //
   class TApp : public NAmg::NApplication::TServiceApplication
   {
   public:
      AmgCreate TApp();
      virtual AmgDestroy ~TApp(void);
      //
      TApp(const TApp&) = delete;
      TApp& operator=(const TApp&) = delete;
      //
      std::shared_ptr<TAppData> AppDataPTR;
      //
      using TOnResult = std::function<void(bool)>;
      void AmgCall NotifyCacosAboutAlive(TAString Author, bool Alive, TAString Description, TAString Bud, const TDateTime& RegTime);
      void AmgCall NotifyCacosAboutLocation(TAString Author, TAString real_location, TAString last_location, TAString Description, TAString Bud, const TDateTime& RegTime);
      void AmgCall NotifyCacosAboutSmrStatus(TAString Status, TAString Description, TAString Bud);
      bool AmgCall NeedAttention(std::shared_ptr<TAppData>&, const std::string&, TEndPointState&);
      //
   protected:
      virtual bool AmgCall Initialize() override;
      virtual void AmgCall DoLoadConfiguration(void) override;
      virtual void AmgCall DoSaveConfiguration(void) override;

      static void AmgCall InitializeHttp(std::shared_ptr<TAppData>&, const NAmg::TPath& _ownWebPath, const NAmg::TPath& _commonWebPath, NAmg::NScl::NHttp::TRouteService&);
      static void AmgCall CreateCache(std::shared_ptr<TAppData>&, const NAmg::TPath& _filePath);
      static void AmgCall ConnectCacheAdapters(std::shared_ptr<TAppData>&);
      //
   private:
      //! @brief ќбработчик нажатий "гор€чих" клавиш.
      void AmgCallBack Cb_AfterInput(char ch);
   };
   //
   DeclareApplication(NAmg::TApp);
}

//
#ifdef _MSC_VER
   #pragma pack(pop)
#endif
//
#endif // __Application_H__
