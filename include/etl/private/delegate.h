///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 jwellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

/******************************************************************************

  Copyright (C) 2017 by Sergey A Kryukov: derived work
  http://www.SAKryukov.org
  http://www.codeproject.com/Members/SAKryukov

  Based on original work by Sergey Ryazanov:
  "The Impossibly Fast C++ Delegates", 18 Jul 2005
  https://www.codeproject.com/articles/11015/the-impossibly-fast-c-delegates

  MIT license:
  http://en.wikipedia.org/wiki/MIT_License

  Original publication: https://www.codeproject.com/Articles/1170503/The-Impossibly-Fast-Cplusplus-Delegates-Fixed

******************************************************************************/

#ifndef ETL_PRIVATE_DELEGATE_INCLUDED
#define ETL_PRIVATE_DELEGATE_INCLUDED

#include "delegate_base.h"

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
    template <typename T, TReturn(T::*Method)(TParams...)>
    static delegate create(T* instance)
    {
      return delegate(instance, method_stub<T, Method>);
    }

    //*************************************************************************
    template <typename T, TReturn(T::*Method)(TParams...) const>
    static delegate create(T const* instance)
    {
      return delegate(const_cast<T*>(instance), const_method_stub<T, Method>);
    }

    //*************************************************************************
    template <TReturn(*Method)(TParams...)>
    static delegate create()
    {
      return delegate(nullptr, function_stub<Method>);
    }

    //*************************************************************************
    template <typename TLambda>
    static delegate create(const TLambda & instance)
    {
      return delegate((void*)(&instance), lambda_stub<TLambda>);
    }

    //*************************************************************************
    template <typename T, T& Instance, TReturn(T::*Method)(TParams...)>
    static delegate create()
    {
      return delegate(method_instance_stub<T, Instance, Method>);
    }

    //*************************************************************************
    template <typename T, T const& Instance, TReturn(T::*Method)(TParams...) const>
    static delegate create()
    {
      return delegate(method_instance_stub<T, Instance, Method>);
    }

    //*************************************************************************
    TReturn operator()(TParams... arg) const
    {
      return (*invocation.stub)(invocation.object, arg...);
    }

  private:

    //*************************************************************************
    delegate(void* object, typename delegate_base<TReturn(TParams...)>::stub_type stub)
    {
      invocation.object = object;
      invocation.stub   = stub;
    }

    //*************************************************************************
    delegate(typename delegate_base<TReturn(TParams...)>::stub_type stub)
    {
      invocation.object = nullptr;
      invocation.stub   = stub;
    }

    //*************************************************************************
    void assign(void* object, typename delegate_base<TReturn(TParams...)>::stub_type stub)
    {
      this->invocation.object = object;
      this->invocation.stub   = stub;
    }

    //*************************************************************************
    template <typename T, TReturn(T::*Method)(TParams...)>
    static TReturn method_stub(void* this_ptr, TParams... params)
    {
      T* p = static_cast<T*>(this_ptr);
      return (p->*Method)(params...);
    }

    //*************************************************************************
    template <typename T, TReturn(T::*Method)(TParams...) const>
    static TReturn const_method_stub(void* this_ptr, TParams... params)
    {
      T* const p = static_cast<T*>(this_ptr);
      return (p->*Method)(params...);
    }

    //*************************************************************************
    template <typename T, T& Instance, TReturn(T::*Method)(TParams...)>
    static TReturn method_instance_stub(void*, TParams... params)
    {
      return (Instance.*Method)(params...);
    }

    //*************************************************************************
    template <typename T, const T& Instance, TReturn(T::*Method)(TParams...) const>
    static TReturn const_method_instance_stub(void*, TParams... params)
    {
      return (Instance.*Method)(params...);
    }

    //*************************************************************************
    template <TReturn(*Method)(TParams...)>
    static TReturn function_stub(void*, TParams... params)
    {
      return (Method)(params...);
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
    typename delegate_base<TReturn(TParams...)>::invocation_element invocation;
  };
}

#endif
