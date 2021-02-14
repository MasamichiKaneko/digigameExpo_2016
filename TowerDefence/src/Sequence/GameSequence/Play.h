#pragma once

#include "Sequence\GameSequence\Child.h"
#include <map>
#include <string>

/**
 * �Q�[���{�҃N���X
 */

namespace Sequence{
	
	class CommonFunc;

	namespace GameSequence{
		class Parent;

		class Play : public Child{
		public:
			Play( Parent* );
			~Play();
			Base* update( Parent* );
		private:
			/** ���艹�n���h�� */
			int mDecisionSEHandle;
			void draw( Parent* , bool selectionDraw ) const;
			void mInputPrompt( Base** nextMode, Parent** parent );
			static const int PauseButtonPosX;
			static const int PauseButtonPosY;
		};
	}
}