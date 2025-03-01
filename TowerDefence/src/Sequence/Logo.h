#pragma once

/**
* ロゴ表示
*/

#include "Sequence\Child.h"
#include <string>

class FadeInFadeOut;

namespace Sequence{

	class Logo : public Child{
	public:
		Logo();
		~Logo();
		Base* update( Parent* );

	private:
		/* ロゴ描画関数 */
		/* 表示が終わったらtrueを返す予定 */
		bool draw();

		/* 読み込み画像のハンドル */
		int mImageHandle;

		FadeInFadeOut* bright;

		int mTime;
	};

}