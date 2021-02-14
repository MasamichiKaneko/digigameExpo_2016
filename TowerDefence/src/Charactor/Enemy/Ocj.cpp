#include "Charactor\Enemy\Ocj.h"
#include "Charactor\Friend\FriendBase.h"
#include "Configuration.h"

using namespace std;
using namespace Game;

namespace Charactor{
	namespace Enemy{

		/** Base�ƕύX�Ȃ� */
		Ocj::Ocj(
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
		EnemyBase(
			aId,
			aLevel,
			aHitPoint,
			aSpeed,
			aScore,
			aAssets,
			aDamage,
			aRangeRadius,
			aAdventStartFrame,
			aRoot,
			aStage,
			aResistStun,
			aResistSlow ){}

		Ocj::~Ocj(){}

		void Ocj::update(
			vector< EnemyBase* >* aEnemys,
			vector< Friend::FriendBase* >* aFriends,
			StageMap* aStageMap ){

				bool moveFlag = true;

				/** �������� */
				if ( !mSurvive ) {
					return;
				}

				/** �o���t���O���� */
				if ( !mAppearance ) {
					if ( mAdventStartFrame != 0 ) {
						mAdventStartFrame--;
						return;
					} else {
						mAppearance = true;
					}
				}

				/** �X���b�v���Ȃ��_���[�W */
				if ( mSlip != 0 ) {
					mSlipCount++;
					if ( mSlipCount % ( Configuration::getInstance()->getFrameRate() / 10 ) == 0 ) {
						EnemyBase::addDamage( 1 );
						mSlip--;
					}
				}

				/** �X�^�����Ȃ�s���ł��Ȃ� */
				if ( mStun != 0 ) {
					mStun--;
					moveFlag = false;
				}

				/** �X���E���Ȃ�ړ����x���� */
				if ( mSlow != 0 ) {
					mSlow--;
					if ( mSlow % 2 == 1 ) {
						moveFlag = false;
					}
				}

				if ( moveFlag ){
					/** �ړ����� */
					EnemyBase::move( aStageMap, mSpeed );
				}
		}

		void Ocj::draw() const{
		}

	}
}