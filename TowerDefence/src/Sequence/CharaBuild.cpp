#include "DxLib.h"
#include "Sequence\CharaBuild.h"
#include "define.h"
#include "utils.h"
#include "Sequence\Parent.h"
#include "Sequence\CommonFunc.h"
#include "FileNames.h"
#include "Sequence\StageSelect.h"
#include "Game\CharaStatusReader.h"
#include "aes_encryption.h"
#include "CommonString.h"
#include "XMLNodeNameString.h"
#include "Configuration.h"
#include <algorithm>
#include <numeric>
#include <sstream>

#include "CommonConst.h"

using namespace std;

namespace Sequence{
	
	/** �o�[�\���� */
	const int CharaBuild::DrawBarNum = 4;
	
	/** �t�H���g�֘A */
	const int CharaBuild::FontSize = 26;
	const int CharaBuild::FontColor = GetColor(UCHAR_MAX,UCHAR_MAX,UCHAR_MAX);
	const char* CharaBuild::FontStyle = "HGS�s����";

	/** �L�����A�C�R���`��J�n�ʒu */
	const int CharaBuild::CharaIconDrawX = 95;
	const int CharaBuild::CharaIconDrawY = 75;

	/** �L�����A�C�R���`��Ԋu */
	const int CharaBuild::CharaIconIntervalX = 0;
	const int CharaBuild::CharaIconIntervalY = 60;

	/** �L���������G�`��ʒu */
	const int CharaBuild::CharaStandDrawX = 20;
	const int CharaBuild::CharaStandDrawY = 120;

	/** �I���t���[���`��ʒu */
	const int CharaBuild::SelectPosX = 290;
	const int CharaBuild::SelectPosY = 420;

	/** �X�e�[�^�X�\���t���[���`��ʒu */
	const int CharaBuild::CharaStatusBackPosX = 20;
	const int CharaBuild::CharaStatusBackPosY = 20;

	/** �X�e�[�^�X�o�[�`��J�n�ʒu */
	const int CharaBuild::StatusBarPosX = 150;
	const int CharaBuild::StatusBarPosY = 325;

	/** �v���}�C�A�C�R�� */
	const int CharaBuild::PlusMinusIconSizeX = 40;
	const int CharaBuild::PlusMinusIconSizeY = 40;
	const int CharaBuild::PlusMinusIconNum = 2;

	/** �`��Ԋu */
	const int CharaBuild::SelectIntervalX = 180;
	const int CharaBuild::SelectIntervalY = 0;

	const int CharaBuild::StatusBarIntervalX = 0;
	const int CharaBuild::StatusBarIntervalY = 0;

	/** ���x���\���ʒu */
	const int CharaBuild::LevelPosX = 250;
	const int CharaBuild::LevelPosY = 30;

	/** sp�\���ʒu */
	const int CharaBuild::SpValuePosX = 350;
	const int CharaBuild::SpValuePosY = 30;

	/** ���̃��x���܂ł�exp�\�� */
	const int CharaBuild::ExpValueForNextLvPosX = 450;
	const int CharaBuild::ExpValueForNextLvPosY = 30;

	/** �X�e�[�^�X�\���ʒu */
	const int CharaBuild::StatusDrawPosX = 210;
	const int CharaBuild::StatusDrawIntervalX = 130;


	int map_acc(int lhs, const std::pair<string, int> & rhs)
	{
		return lhs + rhs.second;
	}


	CharaBuild::CharaBuild( /*const char* mCharaDataFileName*/ )
		: commonFunc( 0 ), mReadStatus( 0 ), mWriteFlag( false )
	{
		mReadStatus = new Game::CharaStatusReader();
		
		/** ��ʂ���̃|�C���^�Q�� */
		commonFunc = CommonFunc::getInstance();

		/** �t�H���g�n���h���쐬 */
		mFontHandle = CreateFontToHandle( FontStyle, FontSize, -1, DX_FONTTYPE_ANTIALIASING );

		/** �摜�n���h���Ǎ��� */
		mBackGroundHandle  = LoadGraph( FileNames::Image::CharaBuildBack.c_str() );
		mCursorHandle      = LoadGraph( FileNames::Image::CharaBuildCursor.c_str() );
		mStatusBackHandle  = LoadGraph( FileNames::Image::CharaBuildStatusBack.c_str() );
		mStatusBarHandle   = LoadGraph( FileNames::Image::CharaBuildStatusBar.c_str() );
		mMeterHandle       = LoadGraph( FileNames::Image::CharaBuildMeter.c_str() );
		mWritingBackHandle = LoadGraph( FileNames::Image::WritingBack.c_str() );

		ASSERT( mBackGroundHandle  + 1 );
		ASSERT( mCursorHandle      + 1 );
		ASSERT( mStatusBackHandle  + 1 );
		ASSERT( mStatusBarHandle   + 1 );
		ASSERT( mMeterHandle       + 1 );
		ASSERT( mWritingBackHandle + 1 );

		/** �v���}�C�A�C�R���ǂݍ��� */
		int buf[256];
		ASSERT( (  LoadDivGraph( FileNames::Image::CharaBuildPlusMinusIcon.c_str(), 
			PlusMinusIconNum , 2 , 1 , PlusMinusIconSizeX, PlusMinusIconSizeY, buf ) + 1 ) );
		mMinusIconHandle = buf[0];
		mPlusIconHandle = buf[1];

		/** �o���l�e�[�u���ǂݍ��� */
		//ASSERT( mReadStatus->readExpTable( FileNames::Text::ExpTableDataName.c_str() ) );

		/** �L�����X�e�[�^�X�ǂݍ��� */
		/** ������ */
		decrypt( FileNames::CharaStatus.c_str(), PassWord::EncryptPass );
		/** �ǂݍ��� */
		if( !mReadStatus->readCharaData( FileNames::CharaStatus.c_str() ) ){
			/** ���s�Ȃ�e���v���ǂݍ��� */
			int statusHandle = FileRead_open( FileNames::Text::CharaDataTemplateName.c_str() );
		/*	vector<string> readData;
			FileRead_getAll( &readData, statusHandle );
			FileWrite_vectorAll(readData,FileNames::CharaStatus.c_str());*/
			ASSERT( mReadStatus->readCharaData( statusHandle ) );
			FileRead_close( statusHandle );
		} else {
			/** �Í��� */
			encrypt( FileNames::CharaStatus.c_str(), PassWord::EncryptPass );
		}
		/** �ύX�l������ */
		vector<Game::CharaStatusReader::Status>::iterator statusIt = mReadStatus->getStatusValue()->begin();
		for ( ; statusIt != mReadStatus->getStatusValue()->end() ; statusIt++ ) {
			mAssignValues[ statusIt->id ] = statusIt->assignSPValue;
			beforeLevel = statusIt->lv;
			afterLevel  = beforeLevel;
			maxLevel    = statusIt->lvMax;
		}

		/** �\�����ݒ� */
		mDisplayName[ CharaDataXml::PowerStr ] = CommonStr::Status::PowerStr;
		mDisplayName[ CharaDataXml::SpeedStr ] = CommonStr::Status::SpeedStr;
		mDisplayName[ CharaDataXml::RangeStr ] = CommonStr::Status::RangeStr;

		/** �L�����摜�Ǎ��� */
		unsigned int charaNum = FileNames::Image::StandImagesVector.size();
		for ( unsigned int i = 0; i < charaNum; i++ ) {
			/** �����G */
			int standHandle = LoadGraph( FileNames::Image::StandImagesVector.at( i ).c_str() );
			/** �A�C�R�� */
			int dotHandle[1];
			/** �h�b�g�G�̐擪�����ǂݍ��� */
			int dotHandleStatus = LoadDivGraph(
				FileNames::Image::DotImagesVector.at(i).c_str(),
				1, 1, 1,
				CommonConst::CharaDotImgSizeX,
				CommonConst::CharaDotImgSizeY,
				dotHandle
				);
			
			ASSERT( standHandle + 1 );
			ASSERT( dotHandleStatus + 1 );

			mStandImgHandles.push_back( standHandle );
			mCharaIconHandles.push_back( dotHandle[ 0 ] );
		}
		/** �`��ʒu�v�Z */
		culcDrawPos();

		mModeName.push_back( CommonStr::Common::DecisionStr );
		mModeName.push_back( CommonStr::Common::CancelStr   );

	}

	CharaBuild::~CharaBuild() {
		ASSERT(DeleteGraph(mBackGroundHandle)  + 1);
		ASSERT(DeleteGraph(mCursorHandle)      + 1);
		ASSERT(DeleteGraph(mStatusBackHandle)  + 1);
		ASSERT(DeleteGraph(mMeterHandle)       + 1);
		ASSERT(DeleteGraph(mPlusIconHandle)    + 1);
		ASSERT(DeleteGraph(mMinusIconHandle)   + 1);
		ASSERT(DeleteGraph(mWritingBackHandle) + 1);
		ASSERT(DeleteFontToHandle(mFontHandle) + 1);

		for ( unsigned int i = 0; i < mStandImgHandles.size(); i++ ) {
			ASSERT(DeleteGraph(mStandImgHandles.at(i))     + 1);
		}
		for ( unsigned int i = 0; i < mCharaIconHandles.size(); i++ ) {
			ASSERT(DeleteGraph(mCharaIconHandles.at(i))     + 1);
		}
		SAFE_DELETE( mReadStatus );
	}

	Base* CharaBuild::update( Parent* parent ){
		Base* next = this;

		/** TODO:�\�Ȃ烊�t�@�N�^�����O */
		if ( !mWriteFlag ) {
			/** ���͎�t */
			mInputPrompt( &next, &parent);
		} else {
			next = new StageSelect;
			WaitTimer( 1000 );
		}

		/** �`�� */
		draw();

		return next;
	}

	/** �`��ʒu�v�Z */
	void CharaBuild::culcDrawPos() {
		/** �L�����A�C�R���ʒu */
		for ( unsigned int i = 0; i < mCharaIconHandles.size(); i++ ) {
			pos charaIconPos;
			charaIconPos.x = CharaIconDrawX + i * CharaIconIntervalX;
			charaIconPos.y = CharaIconDrawY + i * CharaIconIntervalY;
			mCharaIconPos.push_back(charaIconPos);
		}
		/** �X�e�[�^�X�o�[����уv���}�C�A�C�R���\���ʒu�擾 */
		/** �摜�T�C�Y�擾 */
		int barSizeX, barSizeY;
		int meterSizeX, meterSizeY;

		/** �摜�T�C�Y�擾 */
		ASSERT( GetGraphSize( mStatusBarHandle, &barSizeX, &barSizeY ) + 1 );
		ASSERT( GetGraphSize( mMeterHandle, &meterSizeX, &meterSizeY ) + 1 );

		/** �e�X�e�[�^�X�\���ʒu */
		vector<pos> statusBarPos;
		pos buf;
		buf.y = StatusBarPosY;
		buf.x = StatusBarPosX;

		buf.y += barSizeY;
		mMeterDrawPos = buf;
		buf.y -= barSizeY;
		for (int j = 0; j < DrawBarNum ; j++) {
			buf.x = StatusBarPosX + barSizeX * j;
			statusBarPos.push_back( buf );
		}
		mStatusBarPos = statusBarPos;

		/** �}�C�i�X�A�C�R���ʒu */
		buf.x = StatusBarPosX + barSizeX * DrawBarNum;
		buf.y = StatusBarPosY + StatusBarIntervalY;
		mMinusIconPos = buf;

		/** �v���X�A�C�R���ʒu */
		buf.x += PlusMinusIconSizeX;
		mPlusIconPos = buf;

	}

	void CharaBuild::draw() const{
		/** �w�i�`�� */
		DrawGraph(0,0,mBackGroundHandle,FALSE);

		/** �X�e�[�^�X�����`�� */
		mDrawCharaStatus();

		/** �L�����A�C�R���`�� */
		mDrawCharaIcon();

		/** �I���t���[���`�� */
		commonFunc->drawMultiSelectFrame(
			SelectPosX,
			SelectPosY,
			SelectIntervalX,
			SelectIntervalY,
			mModeName,
			true);

		/** �}�E�X�|�C���^�`�� */
		commonFunc->drawMousePointer();
		
		if ( mWriteFlag ) {
			DrawGraph( 0, 0, mWritingBackHandle , TRUE );
		}
	}

	void CharaBuild::mDrawCharaIcon() const {
		for ( unsigned int i = 0; i < mCharaIconHandles.size(); i++ ) {
			DrawGraph( mCharaIconPos.at(i).x, mCharaIconPos.at(i).y, mCharaIconHandles.at(i), TRUE );
		}
	}

	void CharaBuild::mDrawCharaStatus() const{
		
		DrawGraph( CharaStatusBackPosX, CharaStatusBackPosY, mStatusBackHandle,FALSE );

		const vector<string> statusStr = CharaDataXml::VariableStatusStrVector;
		for ( unsigned int i = 0; i < mCharaIconHandles.size(); i++ ) {
			int posY = mCharaIconPos.at(i).y + 10;
			for ( unsigned int j = 0; j < statusStr.size(); j++ ) {
				string typeName = statusStr.at( j );
				int value = mReadStatus->getStatusValue()->at(i).initValue.at(typeName);
				value += mReadStatus->getStatusValue()->at(i).riseValueEachOneSP.at(typeName) * afterLevel;
				int posX = StatusDrawPosX + j * StatusDrawIntervalX;
				string valueStr = getDisplayStatusValue( typeName, value );
				DrawStringToHandle(posX , posY ,valueStr.c_str(), FontColor , mFontHandle );
			}
		}

		for ( unsigned int i = 0; i < mStatusBarPos.size(); i++ ) {
			int posX = mStatusBarPos.at(i).x;
			int posY = mStatusBarPos.at(i).y;
			stringstream drawStr;
			switch( i ) {
			case 0:
				drawStr << CommonStr::Status::LvStr;
				break;
			case 1:
				drawStr << beforeLevel;
				break;
			case 2:
				if ( beforeLevel != afterLevel ) {
					drawStr << "��";
				} else {
					drawStr << "";
				}
				break;
			case 3:
				if ( beforeLevel != afterLevel ) {
					drawStr << afterLevel;
				} else {
					drawStr << "";
				}
				break;
			default:
				break;
			}
			CommonFunc::drawStringOnImg( posX, posY, mStatusBarHandle, FontColor ,mFontHandle , drawStr.str() );
			/** ���[�^�\�� */
			int meterSizeX, meterSizeY;
			ASSERT( GetGraphSize( mMeterHandle, &meterSizeX, &meterSizeY ) + 1 );
			for ( int i = 0; i < maxLevel ; i++) {
				if ( afterLevel <= i ) {
					SetDrawBright( UCHAR_MAX/2, UCHAR_MAX/2, UCHAR_MAX/2);
				}
				DrawGraph( mMeterDrawPos.x + meterSizeX * i , mMeterDrawPos.y , mMeterHandle, TRUE );
			}
			SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX);
		}
		
		bool plusIconBright = CommonFunc::isMousePointOnImage( mPlusIconPos.x, mMinusIconPos.y, mMinusIconHandle );
		plusIconBright &= ( afterLevel < maxLevel );

		bool minusIconBright = CommonFunc::isMousePointOnImage( mMinusIconPos.x, mPlusIconPos.y, mPlusIconHandle );
		minusIconBright &= ( 0 < afterLevel );
		if ( minusIconBright ) {
			SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX);
		} else {
			SetDrawBright( UCHAR_MAX/2, UCHAR_MAX/2, UCHAR_MAX/2);
		}
		DrawGraph( mMinusIconPos.x, mMinusIconPos.y, mMinusIconHandle, TRUE );

		if ( plusIconBright ){
			SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX);
		} else {
			SetDrawBright( UCHAR_MAX/2, UCHAR_MAX/2, UCHAR_MAX/2);
		}
		DrawGraph( mPlusIconPos.x, mPlusIconPos.y, mPlusIconHandle, TRUE );

		SetDrawBright( UCHAR_MAX, UCHAR_MAX, UCHAR_MAX);
	}

	void CharaBuild::mInputPrompt( Base** nextMode, Parent** parent ){
		if ( inputMouseTriggered( MOUSE_INPUT_LEFT ) ) {
			switch ( commonFunc->getSelectFrame() ){
			case DECIDE:
				{
					/** �X�e�[�^�X�������� */
					for ( map<int, map<string, int> >::iterator statusIt = mAssignValues.begin()
						; statusIt != mAssignValues.end() ; statusIt++ ) {

							//mReadStatus->setAssignSPValue( statusIt->first, mAssignValues.at( statusIt->first ) );
							mReadStatus->setLevel( afterLevel );
					}
					mReadStatus->writeCharaData( FileNames::CharaStatus.c_str() );
					encrypt( FileNames::CharaStatus.c_str(), PassWord::EncryptPass );
					mWriteFlag = true;
				}
				ASSERT( PlaySoundMem( (*parent)->getDecisionHandle() , DX_PLAYTYPE_BACK ) + 1 );
				break;
			case CANCEL:
				ASSERT( PlaySoundMem( (*parent)->getDecisionHandle() , DX_PLAYTYPE_BACK ) + 1 );
				*nextMode = new StageSelect;
				break;
			default:
				/** �v���}�C�A�C�R������ */
				if ( CommonFunc::isMousePointOnImage( mPlusIconPos.x, mPlusIconPos.y, mPlusIconHandle ) ) {
					if ( afterLevel < maxLevel ) {
						afterLevel++;
					}
				} else if ( CommonFunc::isMousePointOnImage( mMinusIconPos.x, mMinusIconPos.y, mMinusIconHandle ) ){
					if ( 0 < afterLevel ) {
						afterLevel--;
					}
				}
				break;
			}

		}
	}

	string CharaBuild::getDisplayStatusValue( string typeName, int value ) const {
		stringstream retStr;
		if ( typeName == CharaDataXml::PowerStr ) {
			retStr << "  " <<value;
		} else if ( typeName == CharaDataXml::SpeedStr ) {
			double speedValue = static_cast<double>( value );
			speedValue /= static_cast<double>( Configuration::getInstance()->getFrameRate() );
			retStr << fixed << setprecision(2) << speedValue << CommonStr::Common::SecondStr;
		} else if ( typeName == CharaDataXml::RangeStr ) {
			double rangeValue = static_cast<double>( value );
			rangeValue /= static_cast<double>( CommonConst::RangeBasicUnit );
			retStr << fixed << setprecision(2) << rangeValue << CommonStr::Common::TroutStr;
		}
		return retStr.str();
	}

}