#include "Charactor\Enemy\Ocj.h"
#include "Charactor\Friend\FriendBase.h"
#include "Configuration.h"

using namespace std;
using namespace Game;

namespace Charactor{
	namespace Enemy{

		/** Baseと変更なし */
		Ocj::Ocj(
			int aId,                 //id
			int aLevel,              //レベル
			int aHitPoint,           //HP
			int aSpeed,              //速度
			int aScore,              //討伐時加算スコア
			int aAssets,             //討伐時加算資金
			int aDamage,             //到達時損害値
			int aRangeRadius,        //当たり判定半径
			int aAdventStartFrame,   //出現開始フレーム
			std::vector< int > aRoot,//経路情報
			StageMap* aStage,        //マップ情報
			double aResistStun,       //スタン耐性
			double aResistSlow        //スロウ耐性
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

				/** 生存判定 */
				if ( !mSurvive ) {
					return;
				}

				/** 出現フラグ判定 */
				if ( !mAppearance ) {
					if ( mAdventStartFrame != 0 ) {
						mAdventStartFrame--;
						return;
					} else {
						mAppearance = true;
					}
				}

				/** スリップ中なら被ダメージ */
				if ( mSlip != 0 ) {
					mSlipCount++;
					if ( mSlipCount % ( Configuration::getInstance()->getFrameRate() / 10 ) == 0 ) {
						EnemyBase::addDamage( 1 );
						mSlip--;
					}
				}

				/** スタン中なら行動できない */
				if ( mStun != 0 ) {
					mStun--;
					moveFlag = false;
				}

				/** スロウ中なら移動速度半減 */
				if ( mSlow != 0 ) {
					mSlow--;
					if ( mSlow % 2 == 1 ) {
						moveFlag = false;
					}
				}

				if ( moveFlag ){
					/** 移動処理 */
					EnemyBase::move( aStageMap, mSpeed );
				}
		}

		void Ocj::draw() const{
		}

	}
}