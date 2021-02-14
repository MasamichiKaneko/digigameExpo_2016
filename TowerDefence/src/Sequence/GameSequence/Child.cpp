#include "define.h"
#include "Sequence\GameSequence\Parent.h"
#include "Sequence\GameSequence\Child.h"

namespace Sequence{
	namespace GameSequence{

		Base* Child::update( Base* p ) {
			Parent* parent = dynamic_cast< Parent* >( p );
			ASSERT( parent );
			return update( parent );
		}
	}
}