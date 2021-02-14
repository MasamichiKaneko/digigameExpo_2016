#pragma once

/**
* ƒƒS•\¦
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
		/* ƒƒS•`‰æŠÖ” */
		/* •\¦‚ªI‚í‚Á‚½‚çtrue‚ğ•Ô‚·—\’è */
		bool draw();

		/* “Ç‚İ‚İ‰æ‘œ‚Ìƒnƒ“ƒhƒ‹ */
		int mImageHandle;

		FadeInFadeOut* bright;

		int mTime;
	};

}