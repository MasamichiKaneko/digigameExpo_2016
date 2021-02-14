#pragma once

/** 初期配置画面クラス */
#include "Sequence\GameSequence\Child.h"
#include <map>
#include <string>


namespace Game{
	class State;
}

namespace Sequence{
	
	class CommonFunc;

	namespace GameSequence{
		class Parent;

		class InitPlace : public Child{
		public:
			InitPlace( Parent* );
			~InitPlace();
			Base* update( Parent* );
		private:

			/** 決定音ハンドル */
			int mDecisionSEHandle;
			void draw( Game::State* state, bool selectionDraw ) const;
			void mInputPrompt( Base** nextMode, Parent** parent );
			static const int StartButtonPosX;
			static const int StartButtonPosY;
		};
	}
}