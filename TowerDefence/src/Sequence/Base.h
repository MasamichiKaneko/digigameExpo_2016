#pragma once

namespace Sequence{

	class Base{
	public:
		virtual ~Base(){} //なにもしないのでこれでいい
		virtual Base* update( Base* ) = 0;
	};

} //namespace Sequence
