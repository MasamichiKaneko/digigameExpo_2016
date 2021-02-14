#include "Charactor\Enemy\EnemyBase.h"
#include "Charactor\Friend\FriendBase.h"
#include "Game\StageMap.h"
#include "CommonConst.h"
#include "Configuration.h"
#include <cstdlib>

using namespace std;
using namespace Game;

namespace Charactor {

	namespace Enemy {

		/** �L�����^�C�vgetter */
		string EnemyBase::getCharactorType() { return CharactorType; }

		/** �����̉摜�؂�ւ��Ԋu */
		const double EnemyBase::WalkSwitchingSecond = 0.2;

		EnemyBase::EnemyBase(
			int aId,                 //id
			int aLevel,              //���x��
			int aHitPoint,           //HP
			int aSpeed,              //���x
			int aScore,              //���������Z�X�R�A
			int aAssets,             //���������Z����
			int aDamage,             //���B�����Q�l
			int aRangeRadius,        //�����蔻�蔼�a
			int aAdventStartFrame,   //�o���J�n�t���[��
			std::vector< int > aRoot,//�o�H���
			StageMap* aStage,        //�}�b�v���
			double aResistStun,       //�X�^���ϐ�
			double aResistSlow        //�X���E�ϐ�
			) :
			mLevel( aLevel ),
			mHitPoint( aHitPoint ),
			mMaxHitPoint( aHitPoint ),
			mSpeed( aSpeed ),
			mScore( aScore ),
			mAssets( aAssets ),
			mAssetDamage( aDamage ),
			mAdventStartFrame( aAdventStartFrame ),
			mRoot( aRoot ),
			mResistStun( aResistStun ),
			mResistSlow( aResistSlow ),
			mStun( 0 ),
			mSlip( 0 ),
			mSlow( 0 ),
			mMoveCount( 0 ),
			mPriority( false ),
			mAppearance( false ),
			mSurvive( true ),
			mSlipCount( 0 ){
				/** id�Z�b�g */
				mId = aId;
				/** ���[�V�����Z�b�g */
				mMotionId = 0;
				/** �����蔻��Z�b�g */
				mRange = aRangeRadius;
				/** �������[�V�����Ԋu�Z�b�g */
				mWalkMotion = static_cast< int >( Configuration::getInstance()->getFrameRate() * WalkSwitchingSecond );
				/** �������W�Z�b�g */
				int posX, posY;
				int chipSize = aStage->getChipSize();
				posX = aStage->getRoot()->at( mRoot.at( 0 ) ).x * chipSize;
				posY = aStage->getRoot()->at( mRoot.at( 0 ) ).y * chipSize;
				mPoint = Point( posX, posY );
				/** �}�b�v��񂩂�S�[���܂ł̋����v�Z */
				mDistanceToGoal = culcDistanceToGoal( aStage );
		}

		int EnemyBase::culcDistanceToGoal( StageMap* stage ) {
			int totalDistance = 0;

			for ( unsigned int i = 1; i < mRoot.size(); i++ ) {
				/** x����y�������ꂼ��̋������v�Z(���}�X�P��) */
				int distanceX = stage->getRoot()->at( mRoot.at( i-1 ) ).x;
				int distanceY = stage->getRoot()->at( mRoot.at( i-1 ) ).y;
				distanceX -=stage->getRoot()->at( mRoot.at( i ) ).x;
				distanceY -=stage->getRoot()->at( mRoot.at( i ) ).y;

				/** �΂ߕ����͕s���Ƃ��� */
				if ( distanceX != 0 && distanceY != 0 ) {
					totalDistance = -1;
					goto FUNC_END;
				}
				totalDistance += abs( distanceX ) + abs( distanceY );
			}
			/** �h�b�g�P�ʂɂȂ�悤�Ƀ`�b�v�T�C�Y����Z���� */
			totalDistance *= stage->getChipSize();
FUNC_END:
			return totalDistance;
		}

		

		/** �ړ����� */
		void EnemyBase::move( StageMap* stage, int movingDistance ){
			
			/** �ŏI�n�_�܂œ��B���Ă��Ȃ���Ώ������s�� */
			if ( mRoot.size() > 1 && movingDistance > 0 ) {
				/** �ړ������v�Z�p */
				int distance = movingDistance;
				int remainDistance = 0;

				int chipSize = stage->getChipSize();
				/** �O�̓��B�_���擾 */
				vector<int>::iterator prev = mRoot.begin();
				int prevPosX = stage->getRoot()->at( (*prev) ).x * chipSize;
				int prevPosY = stage->getRoot()->at( (*prev) ).y * chipSize;
				/** ���̓��B�_���擾 */
				vector<int>::iterator next = mRoot.begin();
				next++;
				int nextPosX = stage->getRoot()->at( (*next) ).x * chipSize;
				int nextPosY = stage->getRoot()->at( (*next) ).y * chipSize;

				/** �������[�V�����v�Z */
				mMoveCount++;
				int addWalkMotionValue = ( mMoveCount / ( mWalkMotion * ( ( mSlow % 2 ) + 1 ) ) );
				addWalkMotionValue = addWalkMotionValue % CommonConst::EnemyDotImgDivNumX;

				/** x�����ړ��v���X�̃p�^�[�� */
				if ( prevPosX < nextPosX ) {
					/** �`��Id�v�Z */
					mMotionId = Right * CommonConst::EnemyDotImgDivNumY + addWalkMotionValue;
					/** �ړ����� */
					mPoint.moveX( movingDistance );
					/** �͂ݏo�����������͎����W���B���̏��� */
					if ( nextPosX <= mPoint.getX() ) {
						/** �O�̓��B�_���폜 */
						mRoot.erase( prev );
						/** �c��̈ړ��������擾���A�͂ݏo���C�����s�� */
						remainDistance = mPoint.getX() - nextPosX; 
						mPoint.setX( nextPosX );
						/** �c��̋������ēx���� */
						move( stage, remainDistance );
					}
				/** x�����ړ��}�C�i�X�̃p�^�[�� */
				} else if( prevPosX > nextPosX ) {
					mMotionId = Left * CommonConst::EnemyDotImgDivNumY + addWalkMotionValue;
					mPoint.moveX( ( -1 ) * movingDistance );
					if ( mPoint.getX() <= nextPosX ) {
						mRoot.erase( prev );
						remainDistance = nextPosX - mPoint.getX();
						mPoint.setX( nextPosX );
						move( stage, remainDistance );
					}
				/** y�����ړ��v���X�̃p�^�[�� */
				} else if( prevPosY < nextPosY ) {
					mMotionId = Front * CommonConst::EnemyDotImgDivNumY + addWalkMotionValue;
					mPoint.moveY( movingDistance );
					if ( nextPosY <= mPoint.getY() ) {
						mRoot.erase( prev );
						remainDistance = mPoint.getY() - nextPosY;
						mPoint.setY( nextPosY );
						move( stage, remainDistance );
					}
				/** y�����ړ��}�C�i�X�̃p�^�[�� */
				} else if( prevPosY > nextPosY ) {
					mMotionId = Back * CommonConst::EnemyDotImgDivNumY + addWalkMotionValue;
					mPoint.moveY( ( -1 ) * movingDistance );
					if ( mPoint.getY() <= nextPosY ) {
						mRoot.erase( prev );
						remainDistance = nextPosY - mPoint.getY();
						mPoint.setY( nextPosY );
						move( stage, remainDistance );
					}
				}
				/** �S�[���܂ł̋������X�V */
				mDistanceToGoal -= ( distance - remainDistance );
			}
			return;
		}

	}
}