#pragma once

namespace Sequence{

	class Base{
	public:
		virtual ~Base(){} //�Ȃɂ����Ȃ��̂ł���ł���
		virtual Base* update( Base* ) = 0;
	};

} //namespace Sequence
