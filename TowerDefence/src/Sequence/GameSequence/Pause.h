#pragma once

#include "Sequence\GameSequence\Child.h"
#include <map>
#include <string>

/**
 * ゲーム本編クラス
 */

class FadeInFadeOut;

namespace Sequence{
	
	class CommonFunc;

	namespace GameSequence{
		class Parent;

		class Pause : public Child{
		public:
			Pause( Parent* );
			~Pause();
			Base* update( Parent* );
		private:
			void draw( Parent* , bool selectionDraw ) const;
			void mInputPrompt( Base** nextMode, Parent** parent );
			
			FadeInFadeOut* bright;

			bool isSelectGameEnd;
			bool isGameEnd;

			int mBGMHandle;
			int mDecisionSEHandle;
			int mConfirmBackHandle;

			static const int ContinueButtonPosX;
			static const int ContinueButtonPosY;

			static const int PlayStopButtonPosX;
			static const int PlayStopButtonPosY;

			static const int ConfirmBackPosX;
			static const int ConfirmBackPosY;

			static const int YesButtonPosX;
			static const int YesButtonPosY;
			
			static const int NoButtonPosX;
			static const int NoButtonPosY;

			/** ウィンドウサイズ */
			int windowSizeX;
			int windowSizeY;
		};
	}
}