#ifndef __BudComplan_Application__
#define __BudComplan_Application__
//! @file Application.h
//! @brief ќбъ€вление программного интерфейса модул€ конечного приложени€,
//!   основанного на "служебном" приложении.
//
#include <amgDefs.h>
#include <amgServiceApplication.h>
#include <HTTP/sclHttpClientService.h>
#include <ckiBudService.h>
#include <ckiHosts.h>
#include <ckiDevices.h>
#include <ckiMediaType.h>
#include <ckiSchemeManager.h>
#include <ckiSchemes.h>
#include <XML/amgXmlDocument.h>
#include "Service.h"
#include "ServiceSet.h"
#include "RestconfRequests.h"
#include "Frequency.h"
#include "Types.h"
//
//
#ifdef _MSC_VER
#pragma pack(push,16)
#endif

struct TAppData
{
   NAmg::NScl::NHttp::TClientService ClientService;
   NAmg::NCakApi::TPresetManager PresetManager;
   NAmg::NCakApi::TBud::TParameters BudParameters;
   NAmg::NCakApi::TBud Bud;
   NAmg::NCakApi::THost::TAdapter Hosts;
   NAmg::NCakApi::TDeviceData::TAdapter DeviceData;
   NAmg::NCakApi::TDeviceState::TAdapter DeviceState;
   NAmg::NCakApi::TDeviceType::TAdapter DeviceTypes;
   TExtraDeviceType::TAdapter ExtraDeviceTypes;
   NAmg::NCakApi::TMediaType::TAdapter MediaTypes;
   TServiceSetData::TAdapter ServiceSetDatas;
   TServiceSetState::TAdapter ServiceSetState;
   TServiceData::TAdapter ServiceDatas;
   TServiceState::TAdapter ServiceStates;
   TFrequencyRange::TAdapter FrequencyRanges;
   TEndpointsSubscribedOn Subscriptions;
   TForbiddenFrequencyRange::TAdapter ForbiddenFrequencyRange;
   TForbiddenFrequency::TAdapter ForbiddenFrequency;
   NAmg::NCakApi::TScheme::TAdapter Schemes;
   TEmissionClass::TAdapter EmissionClasses;
   AmgCreate TAppData();
   AmgDestroy ~TAppData();

   static void AmgCall LoadFromFile(TAppData& _app, const NAmg::TPath& _fileName);
   static void AmgCall SaveToFile(const TAppData& _app, const NAmg::TPath& _fileName);

   static void AmgCall LoadFromFrequencyFile(TAppData& _app, const NAmg::TPath& _fileName);
   static void AmgCall SaveToFrequencyFile(const TAppData& _app, const NAmg::TPath& _fileName);

   static void AmgCall LoadFromSchemeFile(TAppData& _app, const NAmg::TPath& _fileName);
   static void AmgCall SaveToSchemeFile(TAppData& _app, const NAmg::TPath& _fileName);
};

struct TIdsCapture
{
   AmgCreate TIdsCapture(const std::string& _sId) : ServiceId(_sId) {}
   AmgCreate TIdsCapture(const std::string& _sId, const std::string& _mId, const std::string& _dId = "")
      : ServiceId(_sId), ModeId(_mId), DeviceId(_dId) {}

   std::string ServiceId = "";
   std::string ModeId = "";
   std::string DeviceId = "";
};

class TApplication : public NAmg::NApplication::TServiceApplication
{
public:
   AmgCreate TApplication();
   virtual AmgDestroy ~TApplication();

   NUtils::shared_ptr_ref<TAppData> AppData;

   using TOnResult = std::function<void(bool)>;
   void AmgCall NotifyCacosAboutSetIsSend(NAmg::TAString Author, bool IsSend, NAmg::TAString Description, NAmg::TAString Bud, const NAmg::TDateTime& RegTime);

protected:
   virtual bool AmgCall Initialize() override;
   virtual void AmgCall DoLoadConfiguration(void) override;
   virtual void AmgCall DoSaveConfiguration(void) override;

   static void AmgCall InitializeHttp(const NUtils::shared_ptr_ref<TAppData>&, const NAmg::TPath& _ownWebPath, const NAmg::TPath& _commonWebPath, NAmg::NScl::NHttp::TRouteService&);
   void AmgCall CreateCache(const NUtils::shared_ptr_ref<TAppData>&, 
                            const NAmg::TPath& _complansPath, 
                            const NAmg::TPath& _frequencyPath, 
                            const NAmg::TPath& _schemesPath,
                            const NAmg::TPath& _extDeviceTypesPath);

   //! @brief
   //!   ‘актическа€ смена текущего набора сервисов
   //! @details
   //!   xxx
   static bool AmgCall DoSwitchSet(const NUtils::shared_ptr_ref<TAppData>&, const std::string& _awaitedSetId);

   static void AmgCall DoBuildService(const NUtils::shared_ptr_ref<TAppData>&, const std::string& _serviceId, const std::string& _modeId, TOnResult&&);

   static void AmgCall DoDestroyService(const NUtils::shared_ptr_ref<TAppData>&, const std::string& _serviceId, TOnResult&&);

   static void AmgCall DoSwitchServiceMode(const NUtils::shared_ptr_ref<TAppData>&, const std::string& _serviceId, const std::string& _modeId, TOnResult&&);

   static void AmgCall DoApplySet(const NUtils::shared_ptr_ref<TAppData>&, NAmg::NXml::TDocument& _xml, TOnResult&&);

   static void AmgCall DoAttachEndpoint(
      const NUtils::shared_ptr_ref<TAppData>& _app,
      const NUtils::shared_cptr_cref<TIdsCapture>& _ids,
      TOnResult&&);

   static void AmgCall DoDetachEndpoint(
      const NUtils::shared_ptr_ref<TAppData>& _app,
      const NUtils::shared_cptr_cref<TIdsCapture>& _ids,
      TOnResult&&);

   static void AmgCall DoChangeEndpointsMode(const NUtils::shared_ptr_ref<TAppData>&, const NUtils::shared_cptr_cref<TIdsCapture>& _ids);

private:
   static void AmgCall DoSwitchLocking(const NUtils::shared_ptr_ref<TAppData>&,
                                       const NUtils::shared_cptr_cref<TIdsCapture>&,
                                       bool _doLock);

   static void AmgCall DoBuildMode(const NUtils::shared_ptr_ref<TAppData>&,
                                   const NUtils::shared_cptr_cref<TIdsCapture>&,
                                   const TModeData& _modeData,
                                   TOnResult&& _onResult);

   static void AmgCall DoDestroyMode(const NUtils::shared_ptr_ref<TAppData>&,
                                     const NUtils::shared_cptr_cref<TIdsCapture>&,
                                     const TModeData& _modeData,
                                     TOnResult&& _onResult);

   static void AmgCall DoApplyPreset(const NUtils::shared_ptr_ref<TAppData>&,
                                     const TModeData::TPresetData& _preset,
                                     std::function<void(TModeState::TPresetState)> _onResult);

   static void AmgCall DoSwitchEndpointsConnection(
      bool _doConnect,
      const NUtils::shared_ptr_ref<TAppData>& _app,
      const std::string& _serviceId,
      const std::string& _initiatorId);

   static std::string AmgCall DefaultMode(const NUtils::shared_ptr_ref<TAppData>&, const std::string& _serviceId);

   //! @brief ќбработчик нажатий "гор€чих" клавиш.
   void AmgCallBack Cb_AfterInput(char ch);

   DisableObjectCopy(TApplication);
};

extern ::TApplication& AmgCall Application();

#ifdef _MSC_VER
#pragma pack(pop)
#endif

#endif // __BudComplan_Application__
