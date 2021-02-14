#include "utils.h"
#include "DxLib.h"
#include "Sequence\Title.h"
#include "Sequence\Option.h"
#include "FileNames.h"
#include "define.h"
#include "Configuration.h"
#include "CommonFunc.h"
#include "Sequence\Parent.h"
#include <boost\lexical_cast.hpp>
#include "CommonString.h"
#include "aes_encryption.h"

/**
 *  オプション画面
 */

namespace Sequence{

	/** 設定値表示位置関連 */
	const int Option::DisplayBarDrawPosX  = 50;
	const int Option::DisplayBarDrawPosY  = 50;
	const int Option::DisplayBarIntervalX = 0;
	const int Option::DisplayBarIntervalY = 100;
	
	const int Option::SelectModePosX = 150;
	const int Option::SelectModePosY = 420;
	const int Option::SelectModeIntervalX = 240;
	const int Option::SelectModeIntervalY = 0;

	/** グリッド表示関連 */
	const int Option::GridDrawPosX = 400;
	const int Option::GridDrawPosY = 100;
	const int Option::DrawMapChipNum = 3;
	const int Option::GridLineColor = GetColor( 0, 0, 0 );

	/** ±アイコン */
	const int Option::PlusMinusIconSizeX = 40;
	const int Option::PlusMinusIconSizeY = 40;
	const int Option::PlusMinusIconNum  = 2;

	/** フォント関連 */
	const int Option::FontSize = 16;
	const int Option::FontColor = GetColor(UCHAR_MAX,UCHAR_MAX,UCHAR_MAX);

	Option::Option() : commonFunc( 0 ){

		commonFunc = CommonFunc::getInstance();

		/** フォントハンドル作成 */
		mFontHandle = CreateFontToHandle( NULL, FontSize, -1,DX_FONTTYPE_NORMAL );
		/** 背景読み込み */
		mBackGroundHandle = LoadGraph( FileNames::Image::OptionBack.c_str() );
		/** 表示バー読み込み */
		mDisplayBarHandle = LoadGraph( FileNames::Image::CharaBuildStatusBar.c_str() );
		/** メータ読み込み */
		mMeterHandle = LoadGraph( FileNames::Image::OptionMeter.c_str() );
		/** サンプル用マップチップ読み込み */
		mMapChipSampleHandle = LoadGraph( FileNames::Image::OptionMapChip.c_str() );

		/** ±アイコン読み込み(育成と同じ処理なので共通関数化するかも) */
		int buf[256];
		ASSERT( (  LoadDivGraph( FileNames::Image::CharaBuildPlusMinusIcon.c_str(), 
			PlusMinusIconNum , 2 , 1 , PlusMinusIconSizeX, PlusMinusIconSizeY, buf ) + 1 ) );
		mMinusIconHandle = buf[0];
		mPlusIconHandle = buf[1];

		/** 読み込みチェック */
		ASSERT( mMeterHandle         + 1 );
		ASSERT( mBackGroundHandle    + 1 );
		ASSERT( mDisplayBarHandle    + 1 );
		ASSERT( mPlusIconHandle      + 1 );
		ASSERT( mMinusIconHandle     + 1 );
		ASSERT( mMapChipSampleHandle + 1 );

		/** 現在の設定値受け取り */
		mSetValues = Configuration::getInstance()->getValue().at( ConfigData::SetValueNode );
		/** 各値の最大値受け取り */
		mMaxValues = Configuration::getInstance()->getValue().at( ConfigData::MaxValueNode );

		/** 選択モード取得 */
		mModeName.push_back( CommonStr::Common::DecisionStr );
		mModeName.push_back( CommonStr::Common::CancelStr   );

		/** グリッド濃度を設定中の濃度にセット */
		float ratio = static_cast< float >( mSetValues.at( ConfigData::GridLevelStr ) );
		ratio /= static_cast< float >( mMaxValues.at( ConfigData::GridLevelStr ) );
		mGridDipth = static_cast< int >( UCHAR_MAX * ratio );

		/** 描画位置計算 */
		culcDrawPos();

		/** 設定値テーブルセット */
		for ( unsigned int i = 0; i < ConfigData::VariableConfigVector.size(); i++ ) {
			mValueTable[ ConfigData::VariableConfigVector.at(i) ] = static_cast<int>( i );
		}

	}

	Option::~Option(){
		ASSERT( DeleteGraph( mBackGroundHandle    ) + 1 );
		ASSERT( DeleteGraph( mMeterHandle         ) + 1 );
		ASSERT( DeleteGraph( mDisplayBarHandle    ) + 1 );
		ASSERT( DeleteGraph( mPlusIconHandle      ) + 1 );
		ASSERT( DeleteGraph( mMinusIconHandle     ) + 1 );
		ASSERT( DeleteGraph( mMapChipSampleHandle ) + 1 );
	}

	/** 描画位置計算 */
	void Option::culcDrawPos(){
		/** 画像サイズ取得 */
		int barSizeX, barSizeY;
		GetGraphSize( mDisplayBarHandle, &barSizeX, &barSizeY );

		for ( unsigned int i = 0; i < ConfigData::VariableConfigVector.size(); i++ ) {
			pos drawPos;
			
			/** 設定値名表示部分 */
			drawPos.x = DisplayBarDrawPosX + i * DisplayBarIntervalX;
			drawPos.y = DisplayBarDrawPosX + i * DisplayBarIntervalY;
			mValueTypePos[ ConfigData::VariableConfigVector.at(i) ] = drawPos;

			/** メータ表示部分 */
			drawPos.y += barSizeY;
			mMeterStartPos[ ConfigData::VariableConfigVector.at(i) ] = drawPos;

			/** 設定値表示部分 */
			drawPos.y -= barSizeY;
			drawPos.x += barSizeX;
			mValueRatioPos[ ConfigData::VariableConfigVector.at(i) ] = drawPos;
			
			/** -アイコン表示部分 */
			drawPos.x += barSizeX;
			mMinusIconPos[ ConfigData::VariableConfigVector.at(i) ] = drawPos;

			/** +アイコン表示部分 */
			drawPos.x += PlusMinusIconSizeX;
			mPlusIconPos[ ConfigData::VariableConfigVector.at(i) ] = drawPos;
		}
	}

	/** 描画 */
	void Option::draw(){
		DrawGraph( 0, 0, mBackGroundHandle, FALSE );

		for ( unsigned int i = 0; i < ConfigData::VariableConfigVector.size(); i++ ) {
			string ValueType = ConfigData::VariableConfigVector.at(i);
			/** 値名表示 */
			commonFunc->drawStringOnImg( 
				mValueTypePos.at(ValueType).x,
				mValueTypePos.at(ValueType).y,
				mDisplayBarHandle, 
				FontColor ,mFontHandle, ValueType );

			/** 割合表示 */
			int ratio = ( 100 / mMaxValues.at(ValueType) ) * mSetValues.at(ValueType);
			string ratioStr = boost::lexical_cast<string>( ratio ) + "%";
			commonFunc->drawStringOnImg(
				mValueRatioPos.at(ValueType).x,
				mValueRatioPos.at(ValueType).y,
				mDisplayBarHandle,
				FontColor, mFontHandle, ratioStr );

			/** ±アイコン表示 */
			drawPlusMinusIcon( ValueType );

			/** メータ表示 */
			drawMeter( ValueType );

			/** グリッド濃度サンプル表示 */
			drawGridSample();

			/** モード選択フレーム表示 */
			CommonFunc::getInstance()->drawMultiSelectFrame(
				SelectModePosX, SelectModePosY,
				SelectModeIntervalX, SelectModeIntervalY,
				mModeName,true );

			/** マウスポインタ表示 */
			commonFunc->drawMousePointer();

		}
	}

	/** メータ表示 */
	void Option::drawMeter( string aValueType ) {

		/** メータサイズ取得 */
		int sizeX, sizeY;
		GetGraphSize( mMeterHandle, &sizeX, &sizeY );

		/** 現在の設定値まで描画 */
		for ( int i = 0; i < mSetValues.at( aValueType ); i++ ) {
			DrawGraph( 
				mMeterStartPos.at(aValueType).x + i * sizeX ,
				mMeterStartPos.at(aValueType).y, mMeterHandle, TRUE );
		}

		/** 設定値が最大でなければ暗転させて残りを描画 */
		if ( mSetValues.at( aValueType ) < mMaxValues.at( aValueType ) ) {
			SetDrawBright( UCHAR_MAX/2, UCHAR_MAX/2, UCHAR_MAX/2);
			for ( int i = mSetValues.at( aValueType ); i < mMaxValues.at( aValueType ); i++ ) {
				DrawGraph( 
				mMeterStartPos.at(aValueType).x + i * sizeX ,
				mMeterStartPos.at(aValueType).y, mMeterHandle, TRUE );
			}
			SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX);
		}

	}

	/** ±アイコン表示 */
	void Option::drawPlusMinusIcon( string aValueType ){

		/** マウス位置チェック */
		bool PlusBrightFlag = 
			commonFunc->isMousePointOnImage(
			mPlusIconPos.at(aValueType).x,
			mPlusIconPos.at(aValueType).y,
			mPlusIconHandle );
		bool MinusBrightFlag = 
			commonFunc->isMousePointOnImage(
			mMinusIconPos.at(aValueType).x,
			mMinusIconPos.at(aValueType).y,
			mMinusIconHandle );
		
		/** 最大(最小)であるかチェック */
		PlusBrightFlag  &= ( mSetValues.at( aValueType ) < mMaxValues.at( aValueType ) );
		MinusBrightFlag &= ( 0 < mSetValues.at( aValueType ) );

		/** +アイコン */
		if ( PlusBrightFlag ) {
			SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );
		} else {
			SetDrawBright( UCHAR_MAX/2, UCHAR_MAX/2, UCHAR_MAX/2 );
		}
		DrawGraph( mPlusIconPos.at( aValueType ).x, mPlusIconPos.at( aValueType ).y, mPlusIconHandle ,TRUE );

		/** -アイコン */
		if ( MinusBrightFlag ) {
			SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );
		} else {
			SetDrawBright( UCHAR_MAX/2, UCHAR_MAX/2, UCHAR_MAX/2 );
		}
		DrawGraph( mMinusIconPos.at( aValueType ).x, mMinusIconPos.at( aValueType ).y, mMinusIconHandle ,TRUE );
		
		SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );

	}

	/** グリッド線濃度サンプル描画 */
	void Option::drawGridSample(){
		int sizeX,sizeY;
		ASSERT( GetGraphSize( mMapChipSampleHandle, &sizeX, &sizeY ) + 1 );

		/** マップチップ敷き詰め */
		for ( int x = 0; x < DrawMapChipNum; x++ ) {
			for ( int y = 0; y < DrawMapChipNum; y++ ) {
				int posX = GridDrawPosX + ( x * sizeX );
				int posY = GridDrawPosY + ( y * sizeY );
				DrawGraph( posX, posY, mMapChipSampleHandle, TRUE);
			}
		}

		/** グリッド線描画 */
		ASSERT( SetDrawBlendMode( DX_BLENDMODE_ALPHA  ,mGridDipth ) + 1 );
		drawGrid( 
			GridDrawPosX, GridDrawPosY,
			DrawMapChipNum + 1,DrawMapChipNum + 1,
			sizeX, sizeY, GridLineColor );
		ASSERT( SetDrawBlendMode( DX_BLENDMODE_NOBLEND ,UCHAR_MAX ) + 1 );
	}

	/** 入力部分 */
	void Option::mInputPrompt( Base** nextMode, Parent** parent ){
		
		if ( inputMouseTriggered( MOUSE_INPUT_LEFT ) ) {
			switch( commonFunc->getSelectFrame() ) {
				/** 決定のみ行う処理 */
			case DECIDE:
				/** 保持している状態を保存 */
				for ( unsigned int i = 0; i < ConfigData::VariableConfigVector.size(); i++ ) {
					string ValueType = ConfigData::VariableConfigVector.at( i );
					ASSERT( Configuration::getInstance()->setVariableValue( ValueType, mSetValues.at( ValueType ) ) );
				}
				Configuration::getInstance()->writeVariableConfig( FileNames::Text::VariableConfig.c_str() );
				encrypt( FileNames::Text::VariableConfig.c_str(), PassWord::EncryptPass );
				
				/** 決定・キャンセルどちらでも行う処理 */
			case CANCEL:
				/** 流れているBGM設定を保存してある状態へ */
				{
					int BGMVolume = UCHAR_MAX;
					float BGMRatio = static_cast<float>( Configuration::getInstance()->getValue().at( ConfigData::SetValueNode ).at( ConfigData::BGMLevelStr ) );
					BGMRatio /= static_cast<float>( Configuration::getInstance()->getValue().at( ConfigData::MaxValueNode ).at( ConfigData::BGMLevelStr ) );
					BGMVolume = static_cast< int >( static_cast<float>( BGMVolume ) * BGMRatio );
					ASSERT( ChangeVolumeSoundMem( BGMVolume ,(*parent)->getBGMHandle() ) + 1 );

					int SEVolume = UCHAR_MAX;
					float SERatio = static_cast<float>( Configuration::getInstance()->getValue().at( ConfigData::SetValueNode ).at( ConfigData::SELevelStr ) );
					SERatio /= static_cast<float>( Configuration::getInstance()->getValue().at( ConfigData::MaxValueNode ).at( ConfigData::SELevelStr ) );
					SEVolume = static_cast< int >( static_cast<float>( SEVolume ) * SERatio );
					ASSERT( ChangeVolumeSoundMem( SEVolume ,(*parent)->getDecisionHandle() ) + 1 );
					
				}
				/** タイトルへ */
				ASSERT( PlaySoundMem( (*parent)->getDecisionHandle() , DX_PLAYTYPE_BACK ) + 1 );
				*nextMode = new Title(false);
				break;
				/** それ以外の判定 */
			default:
				/** ±アイコン判定 */
				for ( unsigned int i = 0; i < ConfigData::VariableConfigVector.size(); i++ ) {
					string ValueType = ConfigData::VariableConfigVector.at( i );
					/** +アイコン判定 */
					if ( commonFunc->isMousePointOnImage( mPlusIconPos.at( ValueType ).x,mPlusIconPos.at( ValueType ).y ,mPlusIconHandle) ) {
						/** 最大値が設定されていなければ加算 */
						if ( mSetValues.at( ValueType ) < mMaxValues.at( ValueType ) ) {
							mSetValues[ValueType]++;
							mSetConfigValue( ValueType, parent );
						}
						break;
					}
					/** -アイコン判定 */
					if ( commonFunc->isMousePointOnImage( mMinusIconPos.at( ValueType ).x,mMinusIconPos.at( ValueType ).y ,mMinusIconHandle) ) {
						/** 最小値が設定されていなければ減算 */
						if ( 0 < mSetValues.at( ValueType ) ) {
							mSetValues[ValueType]--;
							mSetConfigValue( ValueType, parent );
						}
						break;
					}
				}

				break;
			}
		}
	}

	/** 設定を現在の設定値セット */
	void Option::mSetConfigValue( string aValueType, Parent** parent ){
		
		switch ( mValueTable.at( aValueType ) ) {
		case BGM:
			/** BGM音量変更 */
			{
				int BGMVolume = UCHAR_MAX;
				float BGMRatio = static_cast<float>( mSetValues.at( aValueType ) );
				BGMRatio /= static_cast<float>( mMaxValues.at( aValueType ) );
				BGMVolume = static_cast< int >( static_cast<float>( BGMVolume ) * BGMRatio );
				ASSERT( ChangeVolumeSoundMem( BGMVolume ,(*parent)->getBGMHandle() ) + 1 );
			}
			break;
		case SE:
			/** 設定した値で音鳴らす */
			{
				int SEVolume = UCHAR_MAX;
				float SERatio = static_cast<float>( mSetValues.at( aValueType ) );
				SERatio /= static_cast<float>( mMaxValues.at( aValueType ) );
				SEVolume = static_cast< int >( static_cast<float>( SEVolume ) * SERatio );
				ASSERT( ChangeVolumeSoundMem( SEVolume ,(*parent)->getDecisionHandle() ) + 1 );
				ASSERT( PlaySoundMem( (*parent)->getDecisionHandle() , DX_PLAYTYPE_BACK ) + 1 );
			}
			break;
		case Grid:
			/** Grid濃度変更 */
			{
				float ratio = static_cast< float >( mSetValues.at( ConfigData::GridLevelStr ) );
				ratio /= static_cast< float >( mMaxValues.at( ConfigData::GridLevelStr ) );
				mGridDipth = static_cast< int >( UCHAR_MAX * ratio );
			}
			break;
		default:
			/** 何もしない */
			break;
		}
	}

	Base* Option::update( Parent* parent ){
			Base* next = this;
			
			mInputPrompt( &next, &parent );

			draw();

			return next;
	}

}