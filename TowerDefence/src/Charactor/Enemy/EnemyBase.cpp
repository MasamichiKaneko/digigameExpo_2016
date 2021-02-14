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

		/** キャラタイプgetter */
		string EnemyBase::getCharactorType() { return CharactorType; }

		/** 歩きの画像切り替え間隔 */
		const double EnemyBase::WalkSwitchingSecond = 0.2;

		EnemyBase::EnemyBase(
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
				/** idセット */
				mId = aId;
				/** モーションセット */
				mMotionId = 0;
				/** 当たり判定セット */
				mRange = aRangeRadius;
				/** 歩くモーション間隔セット */
				mWalkMotion = static_cast< int >( Configuration::getInstance()->getFrameRate() * WalkSwitchingSecond );
				/** 初期座標セット */
				int posX, posY;
				int chipSize = aStage->getChipSize();
				posX = aStage->getRoot()->at( mRoot.at( 0 ) ).x * chipSize;
				posY = aStage->getRoot()->at( mRoot.at( 0 ) ).y * chipSize;
				mPoint = Point( posX, posY );
				/** マップ情報からゴールまでの距離計算 */
				mDistanceToGoal = culcDistanceToGoal( aStage );
		}

		int EnemyBase::culcDistanceToGoal( StageMap* stage ) {
			int totalDistance = 0;

			for ( unsigned int i = 1; i < mRoot.size(); i++ ) {
				/** x方向y方向それぞれの距離を計算(何マス単位) */
				int distanceX = stage->getRoot()->at( mRoot.at( i-1 ) ).x;
				int distanceY = stage->getRoot()->at( mRoot.at( i-1 ) ).y;
				distanceX -=stage->getRoot()->at( mRoot.at( i ) ).x;
				distanceY -=stage->getRoot()->at( mRoot.at( i ) ).y;

				/** 斜め方向は不正とする */
				if ( distanceX != 0 && distanceY != 0 ) {
					totalDistance = -1;
					goto FUNC_END;
				}
				totalDistance += abs( distanceX ) + abs( distanceY );
			}
			/** ドット単位になるようにチップサイズ分乗算する */
			totalDistance *= stage->getChipSize();
FUNC_END:
			return totalDistance;
		}

		

		/** 移動処理 */
		void EnemyBase::move( StageMap* stage, int movingDistance ){
			
			/** 最終地点まで到達していなければ処理を行う */
			if ( mRoot.size() > 1 && movingDistance > 0 ) {
				/** 移動距離計算用 */
				int distance = movingDistance;
				int remainDistance = 0;

				int chipSize = stage->getChipSize();
				/** 前の到達点を取得 */
				vector<int>::iterator prev = mRoot.begin();
				int prevPosX = stage->getRoot()->at( (*prev) ).x * chipSize;
				int prevPosY = stage->getRoot()->at( (*prev) ).y * chipSize;
				/** 次の到達点を取得 */
				vector<int>::iterator next = mRoot.begin();
				next++;
				int nextPosX = stage->getRoot()->at( (*next) ).x * chipSize;
				int nextPosY = stage->getRoot()->at( (*next) ).y * chipSize;

				/** 歩くモーション計算 */
				mMoveCount++;
				int addWalkMotionValue = ( mMoveCount / ( mWalkMotion * ( ( mSlow % 2 ) + 1 ) ) );
				addWalkMotionValue = addWalkMotionValue % CommonConst::EnemyDotImgDivNumX;

				/** x方向移動プラスのパターン */
				if ( prevPosX < nextPosX ) {
					/** 描画Id計算 */
					mMotionId = Right * CommonConst::EnemyDotImgDivNumY + addWalkMotionValue;
					/** 移動処理 */
					mPoint.moveX( movingDistance );
					/** はみ出し時もしくは次座標到達時の処理 */
					if ( nextPosX <= mPoint.getX() ) {
						/** 前の到達点を削除 */
						mRoot.erase( prev );
						/** 残りの移動距離を取得し、はみ出し修正を行う */
						remainDistance = mPoint.getX() - nextPosX; 
						mPoint.setX( nextPosX );
						/** 残りの距離分再度処理 */
						move( stage, remainDistance );
					}
				/** x方向移動マイナスのパターン */
				} else if( prevPosX > nextPosX ) {
					mMotionId = Left * CommonConst::EnemyDotImgDivNumY + addWalkMotionValue;
					mPoint.moveX( ( -1 ) * movingDistance );
					if ( mPoint.getX() <= nextPosX ) {
						mRoot.erase( prev );
						remainDistance = nextPosX - mPoint.getX();
						mPoint.setX( nextPosX );
						move( stage, remainDistance );
					}
				/** y方向移動プラスのパターン */
				} else if( prevPosY < nextPosY ) {
					mMotionId = Front * CommonConst::EnemyDotImgDivNumY + addWalkMotionValue;
					mPoint.moveY( movingDistance );
					if ( nextPosY <= mPoint.getY() ) {
						mRoot.erase( prev );
						remainDistance = mPoint.getY() - nextPosY;
						mPoint.setY( nextPosY );
						move( stage, remainDistance );
					}
				/** y方向移動マイナスのパターン */
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
				/** ゴールまでの距離を更新 */
				mDistanceToGoal -= ( distance - remainDistance );
			}
			return;
		}

	}
}