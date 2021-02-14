#include "Sequence\GameSequence\Parent.h"
#include "Sequence\GameSequence\GameOver.h"
#include "Game\State.h"
#include "CommonConst.h"
#include "FileNames.h"
#include "Game\State.h"
#include "DxLib.h"
#include "define.h"
#include "Score.h"
#include "utils.h"
#include "Sequence\StageSelect.h"
#include "Game\CharaStatusReader.h"
#include "Sequence\CommonFunc.h"
#include "aes_encryption.h"
#include "Configuration.h"
#include "StageConfig.h"
#include <sstream>


namespace Sequence{
	namespace GameSequence{

		const float GameOver::mFadeInTime = 1.5;
		const int GameOver::mLogoPosX = 205;
		const int GameOver::mLogoPosY = 100;
		
		const int GameOver::mScoreBoardX = 150;
		const int GameOver::mScoreBoardY = 210;

		const int GameOver::mScorePosX = mScoreBoardX + 16;
		const int GameOver::mScorePosY = mScoreBoardY + 35;

		const int GameOver::mAssetBonusPosX = mScorePosX;
		const int GameOver::mAssetBonusPosY = mScoreBoardY +78;
		
		const int GameOver::mLevelRatioPosX = mScorePosX;
		const int GameOver::mLevelRatioPosY = mScoreBoardY + 125;

		const int GameOver::mTotalScorePosX = mScorePosX;
		const int GameOver::mTotalScorePosY = mScoreBoardY + 172;

		const int GameOver::mMessagePosX = 200;
		const int GameOver::mMessagePosY = 420;

		const int GameOver::mAssetBonusRatio = 100;

		const int GameOver::colorBlack = GetColor( 0, 0, 0 );
		const int GameOver::colorWhite = GetColor( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );
		const int GameOver::FontSize = 18;
		const char* GameOver::FontStyle = "HGS行書体";

		GameOver::GameOver( Parent* parent )
		: mIsWin( false ), mScoreRatio( 0.0 ), mEarningsValue( 0 ), mResultLogoHandle( 0 ), mDecisionSEHandle( 0 ),
		mScoreBoardHandle( 0 ), mEndMessageImgHandle( 0 ), mRoopCount( 0 ), isMoveNextScene( false ) {
			bright = new FadeInFadeOut( 1.0 , 0.0, 0.0, 0.0 );
			
			/** 勝敗判定 */
			mIsWin = ( parent->state()->getInitAssetsValue() <= parent->state()->getAssetsValue() );
			
			/** 各ハンドル取得 */
			mScoreBoardHandle    = parent->loader()->getImageFileHandle()->at( FileNames::Image::ScoreBoardImage );
			mEndMessageImgHandle = parent->loader()->getImageFileHandle()->at( FileNames::Image::EndMessageImage );
			mDecisionSEHandle    = parent->loader()->getSoundFileHandle()->at( FileNames::Sound::DecisionSEName  );

			/** フォントハンドル作成 */
			mFontHandle = CreateFontToHandle( FontStyle, FontSize, -1,  DX_FONTTYPE_ANTIALIASING );
			ASSERT( mFontHandle + 1 );

			Result result;

			/** ウィンドウサイズ取得 */
			ASSERT( GetWindowSize( &windowSizeX, &windowSizeY ) + 1);

			/** 勝利時はスコア上書き */
			if ( mIsWin ) {
				Score* score = Score::getInstance();
				Game::State* state = parent->state();
				
				mEarningsValue = ( state->getAssetsValue() - state->getInitAssetsValue() );
				mTotalScore = ( mEarningsValue * mAssetBonusRatio ) + state->getScoreValue();
				int stdLevel = ( StageConfig::getInstance()->getConfigValue()->at( state->getStageNum() ).stdLevelValue );
				int levelValue = ( state->getReadFriendData()->getStatusValue()->at(0).lv );
				int maxLevelValue =( state->getReadFriendData()->getStatusValue()->at(0).lvMax );
				mScoreRatio = static_cast< double >( maxLevelValue + ( stdLevel - levelValue ) );
				mScoreRatio /= static_cast<double>( maxLevelValue );
				mTotalScore = static_cast< int >( mScoreRatio * mTotalScore );
				score->insertScore( parent->getStageNum(), mTotalScore );
				if ( score->getUnlockStageNum() < CommonConst::TotalStageNum 
					&& ( state->getStageNum() + 1 ) == score->getUnlockStageNum() ) {
					score->setUnlockStageNum( score->getUnlockStageNum() + 1 );
				}
				
				score->outputScoreFile( FileNames::ScoreDataName.c_str() );
				encrypt( FileNames::ScoreDataName.c_str(), PassWord::EncryptPass );
				result = Win;
			} else {
				result = Lose;
			}
			/** ハンドル受け取り */
			mResultLogoHandle = parent->loader()->getDivImageFileHanlde()->at( FileNames::Image::ResultLogoImage.c_str() ).at( result );

		}

		GameOver::~GameOver(){
			ASSERT(DeleteFontToHandle(mFontHandle)  + 1);
			SAFE_DELETE( bright );
		}


		/** 描画 */
		void GameOver::draw( Parent* parent ) const{
			parent->state()->draw( false );

			/** 暗転 */
			SetDrawBlendMode( DX_BLENDMODE_ALPHA, UCHAR_MAX / 2 );
			DrawBox( 0, 0, windowSizeX, windowSizeY, colorBlack, TRUE );
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND,  0 );
			

			/** 結果表示 */
			DrawGraph( mLogoPosX, mLogoPosY ,mResultLogoHandle ,TRUE );
			/** 勝利時はスコアボード表示 */
			if ( mIsWin ) {
				stringstream scoreStr;
				stringstream assetBonusStr;
				stringstream levelRatioStr;
				stringstream totalScoreStr;

				scoreStr << parent->state()->getScoreValue();
				assetBonusStr << scoreStr.str() << " + " << ( mEarningsValue * mAssetBonusRatio );
				levelRatioStr << ( parent->state()->getScoreValue() + ( mEarningsValue * mAssetBonusRatio ) )
					<< " × " << fixed << setprecision(1) << mScoreRatio;
				totalScoreStr << mTotalScore;

				DrawGraph( mScoreBoardX, mScoreBoardY ,mScoreBoardHandle ,TRUE );
				DrawStringToHandle(mScorePosX , mScorePosY , scoreStr.str().c_str() , colorWhite , mFontHandle );
				DrawStringToHandle(mAssetBonusPosX , mAssetBonusPosY , assetBonusStr.str().c_str() , colorWhite , mFontHandle );
				DrawStringToHandle(mLevelRatioPosX , mLevelRatioPosY , levelRatioStr.str().c_str() , colorWhite , mFontHandle );
				DrawStringToHandle(mTotalScorePosX , mTotalScorePosY , totalScoreStr.str().c_str() , colorWhite , mFontHandle );
			}

			if ( mRoopCount == Configuration::getInstance()->getFrameRate() ) {
				DrawGraph( mMessagePosX, mMessagePosY, mEndMessageImgHandle, TRUE );
			}

			/** マウスポインタ */
			CommonFunc::getInstance()->drawMousePointer();

			/** 終了ならに徐々に暗転 */
			if ( isMoveNextScene ) {
				SetDrawBlendMode( DX_BLENDMODE_ALPHA, bright->getParameter() );
				DrawBox( 0, 0, windowSizeX, windowSizeY, colorBlack,TRUE );
				SetDrawBlendMode( DX_BLENDMODE_NOBLEND,  0 );
			}
		}

		Base* GameOver::update( Parent* parent ){
			Base* next = this;
			draw( parent );
			if ( !isMoveNextScene ) {
				if ( mRoopCount < Configuration::getInstance()->getFrameRate() ) {
					mRoopCount++;
				}

				/** 左クリック */
				if ( inputMouseTriggered( MOUSE_INPUT_LEFT ) 
					&& mRoopCount == Configuration::getInstance()->getFrameRate() ) {
						isMoveNextScene = true;
						ASSERT( PlaySoundMem( mDecisionSEHandle, DX_PLAYTYPE_BACK ) + 1 );
				}
			} else {
				bright->UpdateParameter();
				switch( bright->getMode() ) {
				case FadeInFadeOut::FADE_IN:
					/** なにもしない */
					break;
				case FadeInFadeOut::WAIT_IN:
				case FadeInFadeOut::FADE_OUT:
				case FadeInFadeOut::WAIT_OUT:
				default:
					next = new StageSelect;
					break;
				}
			}
			return next;
		}

	}
}