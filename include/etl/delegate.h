/*

  Copyright (C) 2017 by Sergey A Kryukov: derived work
  http://www.SAKryukov.org
  http://www.codeproject.com/Members/SAKryukov

  Based on original work by Sergey Ryazanov:
  "The Impossibly Fast C++ Delegates", 18 Jul 2005
  https://www.codeproject.com/articles/11015/the-impossibly-fast-c-delegates

  MIT license:
  http://en.wikipedia.org/wiki/MIT_License

  Original publication: https://www.codeproject.com/Articles/1170503/The-Impossibly-Fast-Cplusplus-Delegates-Fixed

*/

#ifndef ETL_DELEGATE_INCLUDED
#define ETL_DELEGATE_INCLUDED

#include "platform.h"

#if ETL_CPP11_SUPPORTED

#include "private/delegate_base.h"

namespace etl
{
  template <typename T> class delegate;
  template <typename T> class multicast_delegate;

  template<typename TReturn, typename ...TParams>
  class delegate<TReturn(TParams...)> final : private delegate_base<TReturn(TParams...)>
  {
  public:

    //*************************************************************************
    delegate() = default;

    //*************************************************************************
    bool is_null() const
    {
      return invocation.stub == nullptr;
    }

    //*************************************************************************
    bool operator ==(void* ptr) const
    {
      return (ptr == nullptr) && this->is_null();
    }

    //*************************************************************************
    bool operator !=(void* ptr) const
    {
      return (ptr != nullptr) || (!this->is_null());
    }

    //*************************************************************************
    delegate(const delegate& other)
    {
      other.invocation.clone(invocation);
    }

    //*************************************************************************
    template <typename TLambda>
    delegate(const TLambda& lambda)
    {
      assign((void*)(&lambda), lambda_stub<TLambda>);
    }

    //*************************************************************************
    delegate& operator =(const delegate& other)
    {
      other.invocation.clone(invocation);
      return *this;
    }

    //*************************************************************************
    template <typename TLambda> // Template instantiation is not needed, will be deduced (inferred):
    delegate& operator =(const TLambda& instance)
    {
      assign((void*)(&instance), lambda_stub<TLambda>);
      return *this;
    }

    //*************************************************************************
    bool operator == (const delegate& other) const
    {
      return invocation == other.invocation;
    }

    //*************************************************************************
    bool operator != (const delegate& other) const
    {
      return invocation != other.invocation;
    }

    //*************************************************************************
    bool operator ==(const multicast_delegate<TReturn(TParams...)>& other) const
    {
      return other == (*this);
    }

    //*************************************************************************
    bool operator !=(const multicast_delegate<TReturn(TParams...)>& other) const
    {
      return other != (*this);
    }

    //*************************************************************************
    template <typename T, TReturn(T::*TMethod)(TParams...)>
    static delegate create(T* instance)
    {
      return delegate(instance, method_stub<T, TMethod>);
    }

    //*************************************************************************
    template <typename T, TReturn(T::*TMethod)(TParams...) const>
    static delegate create(T const* instance)
    {
      return delegate(const_cast<T*>(instance), const_method_stub<T, TMethod>);
    }

    //*************************************************************************
    template <TReturn(*TMethod)(TParams...)>
    static delegate create()
    {
      return delegate(nullptr, function_stub<TMethod>);
    }

    //*************************************************************************
    template <typename TLambda>
    static delegate create(const TLambda & instance)
    {
      return delegate((void*)(&instance), lambda_stub<TLambda>);
    }

    //*************************************************************************
    TReturn operator()(TParams... arg) const
    {
      return (*invocation.stub)(invocation.object, arg...);
    }

  private:

    //*************************************************************************
    delegate(void* anObject, typename delegate_base<TReturn(TParams...)>::stub_type aStub)
    {
      invocation.object = anObject;
      invocation.stub = aStub;
    }

    //*************************************************************************
    void assign(void* anObject, typename delegate_base<TReturn(TParams...)>::stub_type aStub)
    {
      this->invocation.object = anObject;
      this->invocation.stub = aStub;
    }

    //*************************************************************************
    template <typename T, TReturn(T::*TMethod)(TParams...)>
    static TReturn method_stub(void* this_ptr, TParams... params)
    {
      T* p = static_cast<T*>(this_ptr);
      return (p->*TMethod)(params...);
    }

    //*************************************************************************
    template <typename T, TReturn(T::*TMethod)(TParams...) const>
    static TReturn const_method_stub(void* this_ptr, TParams... params)
    {
      T* const p = static_cast<T*>(this_ptr);
      return (p->*TMethod)(params...);
    }

    //*************************************************************************
    template <TReturn(*TMethod)(TParams...)>
    static TReturn function_stub(void* this_ptr, TParams... params)
    {
      return (TMethod)(params...);
    }

    //*************************************************************************
    template <typename TLambda>
    static TReturn lambda_stub(void* this_ptr, TParams... arg)
    {
      TLambda* p = static_cast<TLambda*>(this_ptr);
      return (p->operator())(arg...);
    }

    //*************************************************************************
    friend class multicast_delegate<TReturn(TParams...)>;

    //*************************************************************************
    typename delegate_base<TReturn(TParams...)>::InvocationElement invocation;
  };
}

#else
  #error NOT SUPPORTED FOR C++03 OR BELOW
#endif

#endif
