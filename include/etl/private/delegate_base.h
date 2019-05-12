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

#ifndef ETL_DELEGATE_BASE_INCLUDED
#define ETL_DELEGATE_BASE_INCLUDED

#include "../nullptr.h"

namespace etl
{
  //***************************************************************************
	template<typename T>
	class delegate_base;

  //***************************************************************************
	template<typename TReturn, typename ...TParams>
	class delegate_base<TReturn(TParams...)>
  {
	protected:

		using stub_type = TReturn(*)(void* this_ptr, TParams...);

    //*************************************************************************
		struct invocation_element
    {
			invocation_element() = default;

			invocation_element(void* this_ptr, stub_type aStub) : object(this_ptr), stub(aStub)
      {
      }

      void clone(invocation_element& target) const
      {
				target.stub = stub;
				target.object = object;
			}

			bool operator ==(const invocation_element& another) const
      {
				return another.stub == stub && another.object == object;
			}

			bool operator !=(const invocation_element& another) const
      {
				return another.stub != stub || another.object != object;
			}

			void*     object = nullptr;
			stub_type stub   = nullptr;
		};
	};
}

#endif
