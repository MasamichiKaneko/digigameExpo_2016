#include "define.h"
#include "Sequence/Child.h"
#include "Sequence/Parent.h"

namespace Sequence{



Base* Child::update( Base* p ){
	Parent* parent = dynamic_cast< Parent* >( p );
	ASSERT( parent );
	return update( parent );
}

} //namespace Sequence
