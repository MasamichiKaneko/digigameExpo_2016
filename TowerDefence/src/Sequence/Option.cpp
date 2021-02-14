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
 *  �I�v�V�������
 */

namespace Sequence{

	/** �ݒ�l�\���ʒu�֘A */
	const int Option::DisplayBarDrawPosX  = 50;
	const int Option::DisplayBarDrawPosY  = 50;
	const int Option::DisplayBarIntervalX = 0;
	const int Option::DisplayBarIntervalY = 100;
	
	const int Option::SelectModePosX = 150;
	const int Option::SelectModePosY = 420;
	const int Option::SelectModeIntervalX = 240;
	const int Option::SelectModeIntervalY = 0;

	/** �O���b�h�\���֘A */
	const int Option::GridDrawPosX = 400;
	const int Option::GridDrawPosY = 100;
	const int Option::DrawMapChipNum = 3;
	const int Option::GridLineColor = GetColor( 0, 0, 0 );

	/** �}�A�C�R�� */
	const int Option::PlusMinusIconSizeX = 40;
	const int Option::PlusMinusIconSizeY = 40;
	const int Option::PlusMinusIconNum  = 2;

	/** �t�H���g�֘A */
	const int Option::FontSize = 16;
	const int Option::FontColor = GetColor(UCHAR_MAX,UCHAR_MAX,UCHAR_MAX);

	Option::Option() : commonFunc( 0 ){

		commonFunc = CommonFunc::getInstance();

		/** �t�H���g�n���h���쐬 */
		mFontHandle = CreateFontToHandle( NULL, FontSize, -1,DX_FONTTYPE_NORMAL );
		/** �w�i�ǂݍ��� */
		mBackGroundHandle = LoadGraph( FileNames::Image::OptionBack.c_str() );
		/** �\���o�[�ǂݍ��� */
		mDisplayBarHandle = LoadGraph( FileNames::Image::CharaBuildStatusBar.c_str() );
		/** ���[�^�ǂݍ��� */
		mMeterHandle = LoadGraph( FileNames::Image::OptionMeter.c_str() );
		/** �T���v���p�}�b�v�`�b�v�ǂݍ��� */
		mMapChipSampleHandle = LoadGraph( FileNames::Image::OptionMapChip.c_str() );

		/** �}�A�C�R���ǂݍ���(�琬�Ɠ��������Ȃ̂ŋ��ʊ֐������邩��) */
		int buf[256];
		ASSERT( (  LoadDivGraph( FileNames::Image::CharaBuildPlusMinusIcon.c_str(), 
			PlusMinusIconNum , 2 , 1 , PlusMinusIconSizeX, PlusMinusIconSizeY, buf ) + 1 ) );
		mMinusIconHandle = buf[0];
		mPlusIconHandle = buf[1];

		/** �ǂݍ��݃`�F�b�N */
		ASSERT( mMeterHandle         + 1 );
		ASSERT( mBackGroundHandle    + 1 );
		ASSERT( mDisplayBarHandle    + 1 );
		ASSERT( mPlusIconHandle      + 1 );
		ASSERT( mMinusIconHandle     + 1 );
		ASSERT( mMapChipSampleHandle + 1 );

		/** ���݂̐ݒ�l�󂯎�� */
		mSetValues = Configuration::getInstance()->getValue().at( ConfigData::SetValueNode );
		/** �e�l�̍ő�l�󂯎�� */
		mMaxValues = Configuration::getInstance()->getValue().at( ConfigData::MaxValueNode );

		/** �I�����[�h�擾 */
		mModeName.push_back( CommonStr::Common::DecisionStr );
		mModeName.push_back( CommonStr::Common::CancelStr   );

		/** �O���b�h�Z�x��ݒ蒆�̔Z�x�ɃZ�b�g */
		float ratio = static_cast< float >( mSetValues.at( ConfigData::GridLevelStr ) );
		ratio /= static_cast< float >( mMaxValues.at( ConfigData::GridLevelStr ) );
		mGridDipth = static_cast< int >( UCHAR_MAX * ratio );

		/** �`��ʒu�v�Z */
		culcDrawPos();

		/** �ݒ�l�e�[�u���Z�b�g */
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

	/** �`��ʒu�v�Z */
	void Option::culcDrawPos(){
		/** �摜�T�C�Y�擾 */
		int barSizeX, barSizeY;
		GetGraphSize( mDisplayBarHandle, &barSizeX, &barSizeY );

		for ( unsigned int i = 0; i < ConfigData::VariableConfigVector.size(); i++ ) {
			pos drawPos;
			
			/** �ݒ�l���\������ */
			drawPos.x = DisplayBarDrawPosX + i * DisplayBarIntervalX;
			drawPos.y = DisplayBarDrawPosX + i * DisplayBarIntervalY;
			mValueTypePos[ ConfigData::VariableConfigVector.at(i) ] = drawPos;

			/** ���[�^�\������ */
			drawPos.y += barSizeY;
			mMeterStartPos[ ConfigData::VariableConfigVector.at(i) ] = drawPos;

			/** �ݒ�l�\������ */
			drawPos.y -= barSizeY;
			drawPos.x += barSizeX;
			mValueRatioPos[ ConfigData::VariableConfigVector.at(i) ] = drawPos;
			
			/** -�A�C�R���\������ */
			drawPos.x += barSizeX;
			mMinusIconPos[ ConfigData::VariableConfigVector.at(i) ] = drawPos;

			/** +�A�C�R���\������ */
			drawPos.x += PlusMinusIconSizeX;
			mPlusIconPos[ ConfigData::VariableConfigVector.at(i) ] = drawPos;
		}
	}

	/** �`�� */
	void Option::draw(){
		DrawGraph( 0, 0, mBackGroundHandle, FALSE );

		for ( unsigned int i = 0; i < ConfigData::VariableConfigVector.size(); i++ ) {
			string ValueType = ConfigData::VariableConfigVector.at(i);
			/** �l���\�� */
			commonFunc->drawStringOnImg( 
				mValueTypePos.at(ValueType).x,
				mValueTypePos.at(ValueType).y,
				mDisplayBarHandle, 
				FontColor ,mFontHandle, ValueType );

			/** �����\�� */
			int ratio = ( 100 / mMaxValues.at(ValueType) ) * mSetValues.at(ValueType);
			string ratioStr = boost::lexical_cast<string>( ratio ) + "%";
			commonFunc->drawStringOnImg(
				mValueRatioPos.at(ValueType).x,
				mValueRatioPos.at(ValueType).y,
				mDisplayBarHandle,
				FontColor, mFontHandle, ratioStr );

			/** �}�A�C�R���\�� */
			drawPlusMinusIcon( ValueType );

			/** ���[�^�\�� */
			drawMeter( ValueType );

			/** �O���b�h�Z�x�T���v���\�� */
			drawGridSample();

			/** ���[�h�I���t���[���\�� */
			CommonFunc::getInstance()->drawMultiSelectFrame(
				SelectModePosX, SelectModePosY,
				SelectModeIntervalX, SelectModeIntervalY,
				mModeName,true );

			/** �}�E�X�|�C���^�\�� */
			commonFunc->drawMousePointer();

		}
	}

	/** ���[�^�\�� */
	void Option::drawMeter( string aValueType ) {

		/** ���[�^�T�C�Y�擾 */
		int sizeX, sizeY;
		GetGraphSize( mMeterHandle, &sizeX, &sizeY );

		/** ���݂̐ݒ�l�܂ŕ`�� */
		for ( int i = 0; i < mSetValues.at( aValueType ); i++ ) {
			DrawGraph( 
				mMeterStartPos.at(aValueType).x + i * sizeX ,
				mMeterStartPos.at(aValueType).y, mMeterHandle, TRUE );
		}

		/** �ݒ�l���ő�łȂ���ΈÓ]�����Ďc���`�� */
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

	/** �}�A�C�R���\�� */
	void Option::drawPlusMinusIcon( string aValueType ){

		/** �}�E�X�ʒu�`�F�b�N */
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
		
		/** �ő�(�ŏ�)�ł��邩�`�F�b�N */
		PlusBrightFlag  &= ( mSetValues.at( aValueType ) < mMaxValues.at( aValueType ) );
		MinusBrightFlag &= ( 0 < mSetValues.at( aValueType ) );

		/** +�A�C�R�� */
		if ( PlusBrightFlag ) {
			SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );
		} else {
			SetDrawBright( UCHAR_MAX/2, UCHAR_MAX/2, UCHAR_MAX/2 );
		}
		DrawGraph( mPlusIconPos.at( aValueType ).x, mPlusIconPos.at( aValueType ).y, mPlusIconHandle ,TRUE );

		/** -�A�C�R�� */
		if ( MinusBrightFlag ) {
			SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );
		} else {
			SetDrawBright( UCHAR_MAX/2, UCHAR_MAX/2, UCHAR_MAX/2 );
		}
		DrawGraph( mMinusIconPos.at( aValueType ).x, mMinusIconPos.at( aValueType ).y, mMinusIconHandle ,TRUE );
		
		SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX );

	}

	/** �O���b�h���Z�x�T���v���`�� */
	void Option::drawGridSample(){
		int sizeX,sizeY;
		ASSERT( GetGraphSize( mMapChipSampleHandle, &sizeX, &sizeY ) + 1 );

		/** �}�b�v�`�b�v�~���l�� */
		for ( int x = 0; x < DrawMapChipNum; x++ ) {
			for ( int y = 0; y < DrawMapChipNum; y++ ) {
				int posX = GridDrawPosX + ( x * sizeX );
				int posY = GridDrawPosY + ( y * sizeY );
				DrawGraph( posX, posY, mMapChipSampleHandle, TRUE);
			}
		}

		/** �O���b�h���`�� */
		ASSERT( SetDrawBlendMode( DX_BLENDMODE_ALPHA  ,mGridDipth ) + 1 );
		drawGrid( 
			GridDrawPosX, GridDrawPosY,
			DrawMapChipNum + 1,DrawMapChipNum + 1,
			sizeX, sizeY, GridLineColor );
		ASSERT( SetDrawBlendMode( DX_BLENDMODE_NOBLEND ,UCHAR_MAX ) + 1 );
	}

	/** ���͕��� */
	void Option::mInputPrompt( Base** nextMode, Parent** parent ){
		
		if ( inputMouseTriggered( MOUSE_INPUT_LEFT ) ) {
			switch( commonFunc->getSelectFrame() ) {
				/** ����̂ݍs������ */
			case DECIDE:
				/** �ێ����Ă����Ԃ�ۑ� */
				for ( unsigned int i = 0; i < ConfigData::VariableConfigVector.size(); i++ ) {
					string ValueType = ConfigData::VariableConfigVector.at( i );
					ASSERT( Configuration::getInstance()->setVariableValue( ValueType, mSetValues.at( ValueType ) ) );
				}
				Configuration::getInstance()->writeVariableConfig( FileNames::Text::VariableConfig.c_str() );
				encrypt( FileNames::Text::VariableConfig.c_str(), PassWord::EncryptPass );
				
				/** ����E�L�����Z���ǂ���ł��s������ */
			case CANCEL:
				/** ����Ă���BGM�ݒ��ۑ����Ă����Ԃ� */
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
				/** �^�C�g���� */
				ASSERT( PlaySoundMem( (*parent)->getDecisionHandle() , DX_PLAYTYPE_BACK ) + 1 );
				*nextMode = new Title(false);
				break;
				/** ����ȊO�̔��� */
			default:
				/** �}�A�C�R������ */
				for ( unsigned int i = 0; i < ConfigData::VariableConfigVector.size(); i++ ) {
					string ValueType = ConfigData::VariableConfigVector.at( i );
					/** +�A�C�R������ */
					if ( commonFunc->isMousePointOnImage( mPlusIconPos.at( ValueType ).x,mPlusIconPos.at( ValueType ).y ,mPlusIconHandle) ) {
						/** �ő�l���ݒ肳��Ă��Ȃ���Ή��Z */
						if ( mSetValues.at( ValueType ) < mMaxValues.at( ValueType ) ) {
							mSetValues[ValueType]++;
							mSetConfigValue( ValueType, parent );
						}
						break;
					}
					/** -�A�C�R������ */
					if ( commonFunc->isMousePointOnImage( mMinusIconPos.at( ValueType ).x,mMinusIconPos.at( ValueType ).y ,mMinusIconHandle) ) {
						/** �ŏ��l���ݒ肳��Ă��Ȃ���Ό��Z */
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

	/** �ݒ�����݂̐ݒ�l�Z�b�g */
	void Option::mSetConfigValue( string aValueType, Parent** parent ){
		
		switch ( mValueTable.at( aValueType ) ) {
		case BGM:
			/** BGM���ʕύX */
			{
				int BGMVolume = UCHAR_MAX;
				float BGMRatio = static_cast<float>( mSetValues.at( aValueType ) );
				BGMRatio /= static_cast<float>( mMaxValues.at( aValueType ) );
				BGMVolume = static_cast< int >( static_cast<float>( BGMVolume ) * BGMRatio );
				ASSERT( ChangeVolumeSoundMem( BGMVolume ,(*parent)->getBGMHandle() ) + 1 );
			}
			break;
		case SE:
			/** �ݒ肵���l�ŉ��炷 */
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
			/** Grid�Z�x�ύX */
			{
				float ratio = static_cast< float >( mSetValues.at( ConfigData::GridLevelStr ) );
				ratio /= static_cast< float >( mMaxValues.at( ConfigData::GridLevelStr ) );
				mGridDipth = static_cast< int >( UCHAR_MAX * ratio );
			}
			break;
		default:
			/** �������Ȃ� */
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