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

#ifndef ETL_PRIVATE_DELEGATE_BASE_CPP03_INCLUDED
#define ETL_PRIVATE_DELEGATE_BASE_CPP03_INCLUDED

#include "../nullptr.h"

namespace etl
{
  //***************************************************************************
	template<typename T>
	class delegate_base;

  //***************************************************************************
	template<typename TReturn, typename TParam>
	class delegate_base<TReturn(TParam)>
  {
	protected:

		typedef TReturn(*stub_type)(void* this_ptr, TParams);

    //*************************************************************************
		struct invocation_element
    {
      //***********************************************************************
			invocation_element(void* this_ptr, stub_type stub)
        : object(this_ptr)
        , stub(stub)
      {
      }

      //***********************************************************************
      void clone(invocation_element& target) const
      {
				target.stub   = stub;
				target.object = object;
			}

      //***********************************************************************
      bool operator ==(const invocation_element& another) const
      {
				return (another.stub == stub) && (another.object == object);
			}

      //***********************************************************************
      bool operator !=(const invocation_element& another) const
      {
				return (another.stub != stub) || (another.object != object);
			}

      //***********************************************************************
			void*     object = nullptr;
			stub_type stub   = nullptr;
		};
	};
}

#endif
