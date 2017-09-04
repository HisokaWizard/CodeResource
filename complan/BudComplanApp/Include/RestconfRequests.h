#ifndef __BudComplan_RestconfRequests__
#define __BudComplan_RestconfRequests__
//! @file RestconfRequests.h

#include <amgDefs.h>
#include <Link/THttpUri.h>
#include <Debug/amgDebug.h>
#include <Restconf/sclRestconfConsts.h>
#include <HTTP/sclHttpClientService.h>

#include "EventMsg.h"

#ifdef _MSC_VER
#pragma pack(push,16)
#endif

namespace NUtils
{
   //------------------ stub for gcc, bug 41933 ---------------------//
   //something like std::apply in C++17
   template <size_t... Indices> struct sequence {};

   template <size_t SizeRemain, size_t... Tail> struct generator : generator<SizeRemain - 1, SizeRemain - 1, Tail...> {};
   template <size_t... Tail> struct generator<0, Tail...> { typedef sequence<Tail...> type; };

   template <class Func, class Tuple, size_t... Seq>
   auto apply_impl(Func&& _func, Tuple&& _tuple, sequence<Seq...>) -> decltype(_func(std::get<Seq>(std::forward<Tuple>(_tuple))...))
   {
      return _func(std::get<Seq>(std::forward<Tuple>(_tuple))...);
   }

   template <class Func, class... Args>
   auto apply(Func&& _func, std::tuple<Args...>&& _tuple) -> typename std::result_of<Func(Args...)>::type
   {
      return apply_impl(std::forward<Func>(_func), std::move(_tuple), typename generator<sizeof...(Args)>::type());
   }
   //------------------------------------------------------------------//
   template <class T>
   struct shared_ptr_ref
   {
      std::shared_ptr<T> ptr;
      T& ref;

      template <class U, class ...Args>
      friend shared_ptr_ref<U> make_shared_ptr_ref(Args&&... _args);

      static shared_ptr_ref<T> make(std::shared_ptr<T>&& _ptr)
      {
         if (!_ptr)
            throw std::runtime_error("bad pointer");

         return shared_ptr_ref<T>(std::move(_ptr));
      }

      static shared_ptr_ref<T> make(const std::shared_ptr<T>& _ptr)
      {
         return make(std::shared_ptr<T>(_ptr));
      }

      shared_ptr_ref(T& _pointee) : ptr(&_pointee), ref(_pointee) {}
      shared_ptr_ref(const std::shared_ptr<T>&) = delete;

   private:
      shared_ptr_ref(std::shared_ptr<T>&& _ptr) : ptr(std::move(_ptr)), ref(*ptr) {}
   };

   template <class U, class ...Args>
   shared_ptr_ref<U> make_shared_ptr_ref(Args&&... _args)
   {
      return shared_ptr_ref<U>(std::make_shared<U, Args...>(std::forward<Args>(_args)...));
   }

   template <class T>
   using shared_cptr_cref = shared_ptr_ref<const T>;

   template <class U, class ...Args>
   shared_cptr_cref<U> make_shared_cptr_cref(Args&&... _args)
   {
      return make_shared_ptr_ref<const U>(std::forward<Args>(_args)...);
   }
   //------------------------------------------------------------------//
}

struct TEndpointData;
struct TServiceState;
struct TServiceData;
struct TFrequencyRange;

namespace NRestconf
{
   template <class ... TArgs>
   void AmgCall AssignHandlers(NAmg::NScl::NHttp::TPromise _request, std::function<void(bool, const std::string&)>&& _onResult, TArgs&&... _args)
   {
      using namespace NAmg;
      using namespace NAmg::NScl;

      //gcc bug 41933 (cannot expand pack in lambda capture list)
      using Pack = std::tuple<typename std::decay<TArgs>::type...>;
      Pack gccStub(std::forward<TArgs>(_args)...);

      _request
         .Then([_onResult, gccStub](NHttp::TResponseData _responce) mutable
      {
         amgDebug.Show(NUtils::apply([](TArgs&&... _args) { return TRequestAcceptedMsg::TClass::Create(_args...); }, std::move<Pack&>(gccStub)));

         if (_onResult)
         {
            std::string body{};
            _responce.To(body);
            _onResult(true, body);
         }
      })
         .Catch([_onResult, gccStub](NHttp::TErrorData _error) mutable
      {
         if (nullptr != _error.Response())
         {
            std::string responce("");
            _error.Response()->To(responce);
            amgDebug.Show(NUtils::apply(
               [&responce](TArgs&&... _args) { return TRequestRejectedMsg::TClass::Create(_args..., responce); },
               std::move<Pack&>(gccStub)));
         }
         else if (nullptr != _error.Error())
            amgDebug.Show(NUtils::apply(
               [&_error](TArgs&&... _args) {return TRequestFailedToSendMsg::TClass::Create(_args..., "TransportError code: " + _error.Error()->Code()); },
               std::move<Pack&>(gccStub)));

         if (_onResult)
            _onResult(false, std::string());
      });
   }

   void AmgCall SendGetRequest(NAmg::NScl::NHttp::TClientService& _client, const NAmg::THttpUri& _uri, const std::string& _hostId,
      std::function<void(const std::string&)>&& _onResult);

   template <class ... TArgs>
   void AmgCall SendPutRequest(NAmg::NScl::NHttp::TClientService& _client, const NAmg::THttpUri& _uri, const std::string& _body,
      std::function<void(bool)>&& _onResult, TArgs&&... _args)
   {
      using namespace NAmg::NScl;
      using namespace NAmg::NScl::NYang;

      NText::builder<NHttp::TRequestHandler::THeader> builder;
      builder
         .Header({ THttpMethod::StrPutName(), _uri })
         .Field("Content-Type", TMediaType::MakeContentType(TMediaType::TId::YANG_DATA, TFormat::TId::XML))
         .Body(_body);

      AssignHandlers(_client.Request(builder), [_onResult](bool _result, const std::string&) { if (_onResult) _onResult(_result); }, std::forward<TArgs>(_args)...);
   }

   template <class ... TArgs>
   void AmgCall SendPostRequest(NAmg::NScl::NHttp::TClientService& _client, const NAmg::THttpUri& _uri, const std::string& _body,
      std::function<void(bool)>&& _onResult, TArgs&&... _args)
   {
      using namespace NAmg::NScl;
      using namespace NAmg::NScl::NYang;

      NText::builder<NHttp::TRequestHandler::THeader> builder;
      builder
         .Header({ THttpMethod::StrPostName(), _uri })
         .Field("Content-Type", TMediaType::MakeContentType(TMediaType::TId::YANG_DATA, TFormat::TId::XML))
         .Body(_body);

      AssignHandlers(_client.Request(builder), [_onResult](bool _result, const std::string&) { if (_onResult) _onResult(_result); }, std::forward<TArgs>(_args)...);
   }

   template <class ... TArgs>
   void AmgCall SendDeleteRequest(NAmg::NScl::NHttp::TClientService& _client, const NAmg::THttpUri& _uri,
      std::function<void(bool)>&& _onResult, TArgs&&... _args)
   {
      AssignHandlers(_client.Delete(_uri), [_onResult](bool _result, const std::string&) { if (_onResult) _onResult(_result); }, std::forward<TArgs>(_args)...);
   }

   void AmgCall SendDeleteService(
      NAmg::NScl::NHttp::TClientService& _client,
      const std::string& _hostId, const NAmg::TSocketAddress& _hostAddr, const std::string& _serviceId,
      std::function<void(bool)>&& _onResult);

   void AmgCall SendAddService(
      NAmg::NScl::NHttp::TClientService& _client,
      const std::string& _hostId, const NAmg::TSocketAddress& _hostAddr,
      const std::string& _serviceId, const TServiceData& _serviceData,
      const std::map<std::string, TServiceState>& _serviceStates,
      const std::map<std::string, TFrequencyRange>& _ranges,
      std::function<void(bool)>&& _onResult);

   void AmgCall SendSubscribeRequest(
      NAmg::NScl::NHttp::TClientService& _client,
      const std::string& _hostId, const NAmg::TSocketAddress& _hostAddr, const std::string& _filterExpr,
      std::function<void(const std::string&)>&& _onResult);

   void AmgCall SendClearServices(
      NAmg::NScl::NHttp::TClientService& _client,
      const std::string& _hostId, const std::string& _hostAddr,
      std::function<void(const std::string&, const NAmg::TSocketAddress&, const std::string&)>&& _deleter);

   std::string AmgCall BuildServiceState(const std::string& _currentId, bool _doConnect);
   std::string AmgCall BuildConnectionState(bool _doConnect);

   struct TAwaited
   {
      std::string ServiceId;
      std::pair<bool, std::string> ModeId;
      std::pair<bool, bool> IsConnected;
   };

   TAwaited AmgCall ParseAwaitedState(const std::string& _body);

   const NAmg::TPath& AmgCall RootResourcePath();
} // namespace NRestconf

#ifdef _MSC_VER
#pragma pack(pop)
#endif

#endif // __BudComplan_RestconfRequests__
