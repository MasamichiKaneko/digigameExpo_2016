#pragma once

#include "Sequence\GameSequence\Child.h"
#include <map>
#include <string>

/**
 * ゲーム本編クラス
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
			/** 決定音ハンドル */
			int mDecisionSEHandle;
			void draw( Parent* , bool selectionDraw ) const;
			void mInputPrompt( Base** nextMode, Parent** parent );
			static const int PauseButtonPosX;
			static const int PauseButtonPosY;
		};
	}
}