#pragma once

/**
* 敵キャラ基底クラス
*/

#include <vector>
#include "Game\Point.h"
#include "Charactor\CharactorBase.h"

namespace Game{
	class StageMap;
}

namespace Charactor{

	namespace Friend{
		class FriendBase;
	}

	namespace Enemy{

		static const std::string CharactorType = "Enemy";

		class EnemyBase : public CharactorBase{
		public:
			EnemyBase(
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
				Game::StageMap* aStage,        //マップ情報
				double aResistStun,       //スタン耐性
				double aResistSlow        //スロウ耐性
				);
			virtual ~EnemyBase(){};

			/* 更新関数 */
			/**                     ↓敵キャラ配列                         ↓自キャラ配列                                ↓マップ配列  */
			virtual void update( std::vector< EnemyBase* >* aEnemys, std::vector< Friend::FriendBase* >* aFriends, Game::StageMap* aStageMap ) = 0;

			/* ステータスgetter定義 */
			int getScore() const { return mScore; }
			int getAssets() const { return mAssets; }

			int getHitPoint() const{ return mHitPoint; }
			int getMaxHitPoint() const{ return mMaxHitPoint; }
			int getSpeed() const{ return mSpeed; }
			int getLevel() const{ return mSlow; }
			int getAssetDamage() const { return mAssetDamage; }

			int getStun() const{ return mStun; }
			int getSlow() const{ return mSlow; }
			int getSlip() const{ return mSlip; }
			
			bool getAppearance() const{ return mAppearance; }
			bool getSurvive() const{ return mSurvive; }

			bool getPriority() const{ return mPriority; }
			int getDistanceToGoal() const { return mDistanceToGoal; }

			/** 表示ゲージにHPを返す */
			int getGauge() const{ return mHitPoint; }
			int getGaugeMax() const{ return mMaxHitPoint; }

			/** キャラタイプgetter */
			std::string getCharactorType();

			/* ルート取得 */
			const std::vector< int >* getRoot() const{ return &mRoot; }

			/** ダメージ付与関数 */
			void addDamage( int aDamageValue ){
				mHitPoint -= aDamageValue;
			}

			/** 生存フラグセット関数 */
			void setSurvive( bool aSurvive ) {
				mSurvive = aSurvive;
			}

			/* スタン値付与関数 */
			void setStun( int aStunValue ){
	//			if ( mStun == 0 ) {
					mStun = static_cast< int >( aStunValue * mResistStun );
	//			}
			}
			/* スロウ値付与関数 */
			void setSlow( int aSlowValue ){
	//			if ( mSlow == 0 ) {
					mSlow = static_cast< int >( aSlowValue * mResistSlow );
	//			}
			}
			/* スリップ値付与関数 */
			void setSlip( int aSlip ){
				mSlip = aSlip;
			}

			std::vector< Effect::EffectBase* > getEffect()const{ return mEffect; }

		protected:

			/** 向きId */
			enum DirectionId {
				Front = 0,
				Left,
				Right,
				Back
			};

			/** 移動処理  ↓マップ情報  ↓移動距離(ドット単位) */
			void move( Game::StageMap* stage, int movingDistance );

			/** ゴールまでの距離計算 */
			/** 計算不可は-1が返される */
			int culcDistanceToGoal( Game::StageMap* stage );

			/* 共通ステータス */
			/* 撃破時の増加スコア */
			int mScore;
			/* 撃破時の増加資産   */
			int mAssets;

			/** スリップ状態用のカウント */
			int mSlipCount;
			/** 動いた回数(モーション計算用) */
			int mMoveCount;
			/** 歩くモーションの間隔 */
			int mWalkMotion;
			/* 耐久 */
			int mMaxHitPoint;
			int mHitPoint;
			/* 移動速度 */
			int mSpeed;
			/* レベル */
			int mLevel;

			/* 到達時の損害 */
			int mAssetDamage;

			/** 出現開始フレーム */
			int mAdventStartFrame;

			/** 出現中フラグ */
			bool mAppearance;

			/* スタン値 */
			int mStun;
			/* スリップ値 */
			int mSlip;
			/* スロウ値 */
			int mSlow;
			
			/** 歩きの画像切り替え間隔 */
			static const double WalkSwitchingSecond;

			/* 生存フラグ */
			bool mSurvive;

			/** 優先フラグ */
			/* trueは優先的に攻撃される予定 */
			bool mPriority;

			/* 防衛目標までの距離 */
			/* ターゲットフラグがなきゃ防衛目標に一番近いやつが狙われる */
			int mDistanceToGoal;
			
			/** エフェクト */
			std::vector< Effect::EffectBase* > mEffect;

			/* 移動経路 */
			/* マップ上の曲がり角や分かれ道にあらかじめナンバーを振っておいて */
			/* それをたどる処理を想定している */
			std::vector< int > mRoot;

			/* 耐性値 */
			/* 受ける状態異常値にこれを掛ける */
			/* 0.0なら効かない、1.0より大きいなら効きやすい感じで */
			double mResistStun;
			double mResistSlow;
		};
	}
}