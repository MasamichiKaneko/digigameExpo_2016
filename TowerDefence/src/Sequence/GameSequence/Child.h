#pragma once

#include "Sequence\Base.h"

namespace Sequence{
	namespace GameSequence{
		class Parent;

		class Child : public Base{
		public:
			virtual ~Child(){};
			Base* update( Base* );
			virtual Base* update( Parent* ) = 0;
		};
	}
}