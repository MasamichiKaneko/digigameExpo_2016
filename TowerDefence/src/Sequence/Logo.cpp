#include "DxLib.h"
#include "Sequence\Logo.h"
#include "Sequence\Title.h"
#include "Sequence\Parent.h"
#include "Configuration.h"
#include "define.h"
#include "FileNames.h"
#include "utils.h"

namespace Sequence{

	Logo::Logo()
	: mImageHandle( 0 ), mTime( 0 ), bright( 0 ){
		/* ‰æ‘œ‚ð“Ç‚Ýž‚Þˆ— */
		mImageHandle = LoadGraph( FileNames::Image::Logo.c_str() );
		bright = new FadeInFadeOut(1.5, 1.0, 1.5, 1.0);

		/* ƒGƒ‰[ƒ`ƒFƒbƒN */
		ASSERT( mImageHandle + 1 );
		
	}

	Logo::~Logo(){
		SAFE_DELETE( bright );
		/* “Ç‚Ýž‚Ý‰æ‘œ‚Ì”jŠü */
		DeleteGraph( mImageHandle );
	}

	Base* Logo::update( Parent* parent ){
		Base* next = this;

		/* ƒƒS•\Ž¦ */
		if ( draw() ) {
			next = new Title( true );
		}
		return next;
	}

	bool Logo::draw(){
		bool displayCompleted = false;
		bright->UpdateParameter();

		/* ƒƒS•\Ž¦’†‚È‚ç•`‰æ */
		if ( bright->getMode() == FadeInFadeOut::END ) {
			displayCompleted = true;
		}

		SetDrawBright( bright->getParameter(), bright->getParameter(), bright->getParameter() );
		DrawGraph(0,0,mImageHandle,FALSE);

		return displayCompleted;
	}
}