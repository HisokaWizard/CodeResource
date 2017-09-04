//! @file
//! @brief Реализация программного интерфейса модуля конечного приложения,
//!   основанного на "служебном" приложении.
#include "precomp.h"

#include <sstream>
#include <Models/MvtExternalComModel.h>
#include <Yang/sclYangXmlParser.h>
#include <Yang/sclYangVisitor.h>
#include "RestconfRequests.h"
#include "Service.h"

#ifdef _MSC_VER
#pragma pack(16)
#endif

namespace NRestconf
{
   using namespace NAmg;
   using namespace NAmg::NScl;
   using namespace NScl::NYang;

   void AmgCall SendGetRequest(NHttp::TClientService& _client, const THttpUri& _uri, const std::string& _hostId,
      std::function<void(const std::string&)>&& _onResult)
   {
      NText::builder<NHttp::TRequestHandler::THeader> builder;
      builder
         .Header({ THttpMethod::StrGetName(), _uri })
         .Field("Accept", TMediaType::MakeContentType(TMediaType::TId::YANG_DATA, TFormat::TId::XML));

      AssignHandlers(_client.Request(builder), [_onResult](bool _result, const std::string& _body) { if (_result && _onResult) _onResult(_body); }, _hostId, _uri.Path());
   }

   void AmgCall SendDeleteService(
      NAmg::NScl::NHttp::TClientService& _client,
      const std::string& _hostId, const NAmg::TSocketAddress& _hostAddr, const std::string& _serviceId,
      std::function<void(bool)>&& _onResult)
   {
      THttpUri uri;
      uri.SetAddress(_hostAddr);
      uri.SetPath(RootResourcePath() + ("service=" + _serviceId));

      SendDeleteRequest(_client, uri, std::move(_onResult), _hostId, uri.Path());
   }

   NAmg::NMvt::TService& AmgCall MakeEndpointService(
      const std::string& _epId,
      const std::string& _serviceId,
      const TServiceData& _serviceData,
      const std::map<std::string, TServiceState>& _serviceStates,
      const std::map<std::string, TFrequencyRange>& _ranges);

   void AmgCall SendAddService(
      NAmg::NScl::NHttp::TClientService& _client,
      const std::string& _hostId, const NAmg::TSocketAddress& _hostAddr,
      const std::string& _serviceId, const TServiceData& _serviceData,
      const std::map<std::string, TServiceState>& _serviceStates,
      const std::map<std::string, TFrequencyRange>& _ranges,
      std::function<void(bool)>&& _onResult)
   {
      THttpUri uri;
      uri.SetAddress(_hostAddr);
      uri.SetPath(RootResourcePath());
      std::unique_ptr<NAmg::NMvt::TService> service;
      try
      {
         service.reset(&MakeEndpointService(_hostId, _serviceId, _serviceData, _serviceStates, _ranges));
      }
      catch (...)
      {
         amgDebug.Show("MakeEndpointService : Error!", errl_Warning);
      }
      std::string body{};
      TRestconfVisitor visitor(body, TFormat::TId::XML);
      service->Get(visitor);

      SendPostRequest(_client, uri, body, [_onResult](bool _result) { if (_onResult) _onResult(_result); }, _hostId, uri.Path());
   }

   void AmgCall SendSubscribeRequest(
      NHttp::TClientService& _client,
      const std::string& _hostId, const NAmg::TSocketAddress& _hostAddr, const std::string& _filterExpr,
      std::function<void(const std::string&)>&& _onResult)
   {
      THttpUri uri;
      uri.SetAddress(_hostAddr);
      
      static TPath path = TRestconfPath::StreamsRes() + "/stream/NETCONF/events";
      uri.SetPath(path);

      if (!_filterExpr.empty())
         uri.Headers()["filter"] = _filterExpr;

      NText::builder<NHttp::TRequestHandler::THeader> builder;
      builder
         .Header({ THttpMethod::StrGetName(), uri })
         .Field("Accept", TMediaType::MakeContentType(TMediaType::TId::EVENT_STREAM, TFormat::TId::XML));

      AssignHandlers(_client.Request(builder, false),
         [_onResult](bool _result, const std::string& _body)
      {
         if (_result && _onResult)
         {
            auto beErased = _body.find("data:");

            std::istringstream in(_body.substr(beErased + std::string("data:").length()));
            NXml::TDocument xmlDoc;
            in >> xmlDoc;

            const auto& res_path = xmlDoc.RootElement().ElChild(0, "netconf-config-change").ElChild(0, "edit").ElChild(0, "target").GetText();
            _onResult(res_path);
         }
      }, _hostId, uri.Path());
   }

   void AmgCall SendClearServices(
      NAmg::NScl::NHttp::TClientService& _client,
      const std::string& _hostId, const std::string& _hostAddr,
      std::function<void(const std::string&, const NAmg::TSocketAddress&, const std::string&)>&& _deleter)
   {
      auto address = NAmg::TSocketAddress(_hostAddr);

      THttpUri uri;
      uri.SetPath(RootResourcePath());
      uri.SetAddress(address);
      uri.Headers()["depth"] = "2";

      NRestconf::SendGetRequest(_client, uri, _hostId, [_hostId, address, _deleter](const std::string& _body)
      {
         NAmg::NMvt::TExternalComModel model;

         TXmlParser parser(_body);
         model.Update(parser);

         auto count = model.Services.ChildCount();
         for (auto i = count; i > 0; --i)
            _deleter(_hostId, address, model.Services.Child(i - 1).As<NAmg::NMvt::TService>().Id.GetValue());
      });
   }

   namespace
   {
	   std::string AmgCall ConnectionState(bool _doConnect)
	   {
		   TBoolLeaf curentStateLeaf("is-connected", TBool(_doConnect));
		   TBoolLeaf awaitedStateLeaf("do-connected", TBool(_doConnect));

		   std::string body{};

		   TRestconfVisitor visitor(body, TFormat::TId::XML);
		   curentStateLeaf.Get(visitor);
		   awaitedStateLeaf.Get(visitor);

		   return body;
	   }
   }

   std::string AmgCall BuildConnectionState(bool _doConnect)
   {
	   return "<service>" + ConnectionState(_doConnect) + "</service>";
   }

   std::string AmgCall BuildServiceState(const std::string& _currentId, bool _doConnect)
   {
      assert(!(_currentId.empty() && _doConnect));

      NAmg::NMvt::TIdValue currentIdLeaf("current-mode-id");
      currentIdLeaf.SetValue(_currentId);

      NAmg::NMvt::TIdValue awaitedIdLeaf("awaited-mode-id");
      awaitedIdLeaf.SetValue(_currentId);

      std::string body{};

      TRestconfVisitor visitor(body, TFormat::TId::XML);
      currentIdLeaf.Get(visitor);
      awaitedIdLeaf.Get(visitor);

      return "<service>" + body + ConnectionState(_doConnect) + "</service>";
   }

   TAwaited AmgCall ParseAwaitedState(const std::string& _body)
   {
      NAmg::NMvt::TService service;

      TXmlParser parser(_body);
      service.Update(parser);

      bool modeChanged = service.AwaitedMode.GetValue() != service.CurrentMode.GetValue();
      bool stateChanged = service.DoConnected.GetValue() != service.IsConnected.GetValue();

      if (service.AwaitedMode.GetValue().empty())
         return{ service.Id.GetValue(), { modeChanged, std::string()}, { stateChanged, false } };

      return { service.Id.GetValue(),
         { modeChanged, service.AwaitedMode.GetValue()},
         { stateChanged, service.IsConnected.GetValue().Get()} };
   }
   namespace
   {
      TServiceMediaEnum ServiceMediaEnum;
   }
   NAmg::NMvt::TService& AmgCall MakeEndpointService(
      const std::string& _epId,
      const std::string& _serviceId,
      const TServiceData& _serviceData,
      const std::map<std::string, TServiceState>& _serviceStates,
      const std::map<std::string, TFrequencyRange>& _ranges)
   {
      std::unique_ptr<NAmg::NMvt::TService> service(new NAmg::NMvt::TService);
      std::unique_ptr<NAmg::NMvt::TMode> mode(nullptr);

      service->Id.SetValue(_serviceId);
      service->Label.SetValue(_serviceData.Label);
      service->Description.SetValue(_serviceData.Description);

      service->TargetUri.SetValue(_serviceData.TargetUri.ToString());

      service->FrequencyRange.SetValue(_ranges.at(_serviceData.FrequencyRangeId).Label);
      service->Encryption.SetValue(_serviceData.Crypted ? TOnOffCommand::ON : TOnOffCommand::OFF);

      service->MediaType.SetValueFromStr(ServiceMediaEnum.ToString(_serviceData.MediaTypeId));
      service->CanSend.SetValue(TBool(_serviceData.CanSend));
      service->CanRecv.SetValue(TBool(_serviceData.CanRecv));

      for (auto& item : _serviceData.Modes)
      {
         mode.reset(new NAmg::NMvt::TMode);

         mode->Id.SetValue(item.first);
         mode->Label.SetValue(item.second.Label);
         mode->Description.SetValue(item.second.Description);

         if (!item.second.Channel.IsNULL() && !item.second.RxFrequency.IsNULL())
         {
            mode->RxChannel.Number.SetValue(item.second.Channel);
            mode->RxChannel.Frequency.SetValue(TAString::IntToStr(item.second.RxFrequency));
         }

         if (!item.second.Channel.IsNULL() && !item.second.TxFrequency.IsNULL())
         {
            mode->TxChannel.Number.SetValue(item.second.Channel);
            mode->TxChannel.Frequency.SetValue(TAString::IntToStr(item.second.TxFrequency));
         }

         service->Modes.AddChild(*mode.release());
      }

      auto state = _serviceStates.find(_serviceId);
      if (_serviceStates.end() != state)
      {
         service->CurrentMode.SetValue(state->second.CurrentModeId);

         auto epData = _serviceData.Endpoints.find(_epId);
         if (_serviceData.Endpoints.end() != epData)
         {
            auto epState = state->second.Endpoints.find(_epId);
            if (state->second.Endpoints.end() != epState)
            {
               service->IsConnected.SetValue(TBool(epData->second.AutoConnect));
            }
         }
      }

      return *service.release();
   }

   const TPath& AmgCall RootResourcePath()
   {
      static const TPath path(NAmg::NScl::NYang::TRestconfPath::DatastoreRes() + "/mvt-config/ext-com");
      return path;
   }
} // namespace NRestconf