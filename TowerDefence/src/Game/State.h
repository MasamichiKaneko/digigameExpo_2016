#pragma once

/**
*  ゲームの状態クラス
*/

#include <vector>
#include <map>
#include <string>
#include "Game\Point.h"

class FileLoader;


namespace Charactor{
	class CharactorBase;
	namespace Friend{
		class FriendBase;
	}
	namespace Enemy{
		class EnemyBase;
	}
}

namespace Effect{
	class Explosion;
	class EffectBase;
}

namespace Game{
	
	class CharaStatusReader;
	class EnemyStatusReader;
	class StageMap;

	class State{
	public:
		State( FileLoader* loader , int stageNum );
		~State();

		void update();  //ゲーム状態更新
		
		/** 選択自キャラタイプ */
		enum FriendType{
			Plane,
			Miko,
			Majokko,
			Yojo,

			Sale,
			Awake,
			Non
		};

		/** 停止・再生ボタンタイプ定義 */
		enum PlayStopButtonType{
			Play,
			Stop
		};

		/** エフェクトタイプ */
		enum EffectType{
			Shot,
			Explosion
		};

		bool getGameOver() const{ return mGameOver; }
		int getAssetsValue() const{ return mAssets; }
		int getInitAssetsValue() const{ return mInitAssets; }
		int getScoreValue() const{ return mScore; }
		int getStageNum() const{ return mStageNum; }
		Game::CharaStatusReader* getReadFriendData() const { return mFriendData; }
		StageMap* getMapState() const{ return mStageMap; }
		FriendType getSelectionType() const { return mSelection; }
		std::vector<int> getSelectIconHandles() const { return mCharaSelectIconHandles; }

		/** 描画関数 */
		void draw( bool selectionDraw ) const;

		/** 停止・再生ボタン描画 */
		void drawPlayStopButton( PlayStopButtonType type ) const;

		/**
		* キャラ売却関数
		* return true :売却完了
		*        false:売却不可
		*/
		bool mSaleFriend( int posX, int posY );

		/**
		* キャラ覚醒関数
		* return true :売却完了
		*        false:売却不可
		*/
		bool mAwakeFriend( int posX, int posY );

		/** 入力受付 */
		/** posX,posY: マウス座標 */
		void mInputPrompt( int posX, int posY );

		/** マウス座標が停止・再生ボタン上に存在するかを返す */
		/** true:存在する false:存在しない */
		bool mIsMousuPointOnPlayStopButton() const;

		/** 選択キャラタイプセット */
		void mSetCharaType( int aCharaTypeId );

		/** 選択ずらし関数 */
		void mShiftSelection();

	private:

		/** チップ種別 */
		enum ChipType{
			PlaneChip = 0,
			NotBePlaced,   //設置不可チップ
			GoalChip,      //目標チップ
			/** 以下道チップ */
			RoadChip_0,
			RoadChip_1,
			RoadChip_2,
			RoadChip_3,
			RoadChip_4,
			RoadChip_5,
			RoadChip_6,
			RoadChip_7,
			RoadChip_8,
			RoadChip_9,
			RoadChip_10
		};

		/** 選択中キャラタイプ */
		FriendType mSelection;

		/** マップ描画 */
		void drawMap( bool selectionDraw )const;
		/** 自キャラ攻撃範囲描画 */
		void drawFriendsRange()const;
		/** アイコン描画 */
		void drawCharaIcon( bool selectionDraw )const;
		/** 状況描画 */
		void drawState()const;
		/** エフェクト描画 */
		void drawEffect( std::vector<Effect::EffectBase*> effect ) const;
		

		/** 座標がマップ上であるかどうかの判定 */
		bool isPointOnMap( int posX, int posY ) const;

		/** 
		* キャラ追加関数 
		* return true :配置完了
		*        false:配置不可
		*/
		Charactor::Friend::FriendBase* getAddFriend( int posX, int posY ) const;

		/** 描画対象のキャラ取得関数 */
		std::vector<Charactor::CharactorBase*> getDrawTargets() const;

		/** キャラ描画 */
		void drawCharactors( bool selectionDraw )const;

		/** 敵キャラ削除 */
		/** やられた敵 */
		void deleteDeadEnemys();
		/** 到達した敵 */
		void deleteArrivalEnemys();
		
		/** 爆発エフェクト削除 */
		void deleteEnemyExplosions();

		/** グリッドカラー */
		static const int GridColor;
		/** グリッド濃度   */
		int mGridDipth;

		/** 画像ハンドル */
		/** マップチップハンドル */
		std::vector<int> mMapChip;
		/** 神社ハンドル */
		int mTempleHandle;
		/** 攻撃範囲イメージハンドル */
		int mRangeHandle;
		/** 状況表示バックハンドル */
		int mStateBackHandle;
		/** 選択中フレームハンドル */
		int mSelectionFrameHandle;
		/** 赤字黒字画像ハンドル */
		std::vector<int> mEconomicIconHandle;
		/** キャラアイコンハンドル */
		std::vector< int > mCharaSelectIconHandles;
		std::map< std::string, std::vector< int > > mCharaIconHandles;
		/** 敵キャラアイコンハンドル */
		std::map< std::string, std::vector< int > > mEnemyIconHandles;
		/** エフェクト画像ハンドル */
		std::map< std::string, std::vector< int > > mEffectImageHandles;

		/** フォントハンドル */
		int mFontHandle;

		/** 音声ハンドル */
		int mExplosionSEHandle;

		/** 自キャラ選択用アイコン描画位置 */
		std::vector< Point > mIconDrawPos;

		/** 読み込んだキャラデータ格納用 */
		std::map<int, std::map<std::string, int> > mStatusValues;

		/** ゲーム終了フラグ */
		bool mGameOver;
		/** スコア */
		int mScore;
		/** 総資産 */
		int mAssets;
		/** 初期資産 */
		int mInitAssets;
		/** ステージ数 */
		int mStageNum;
		/** 自キャラ */
		std::vector<Charactor::Friend::FriendBase*> mFriends;
		/** 敵キャラ */
		std::vector<Charactor::Enemy::EnemyBase*> mEnemys;
		/** 敵爆破エフェクト */
		std::vector<Effect::Explosion*> mEnemyExplosions;
		
		/** マップ  */
		StageMap* mStageMap;
		/** 自キャラデータ読み込み */
		CharaStatusReader* mFriendData;
		/** 敵キャラデータ読み込み */
		EnemyStatusReader* mEnemyData;

		/** 定数 */
		/** 停止・再生ボタン描画位置 */
		static const int PlayStopButtonPosX;
		static const int PlayStopButtonPosY;
		/** キャラアイコン描画位置 */
		static const int CharaIconDrawX;
		static const int CharaIconDrawY;
		/** キャラアイコン描画間隔 */
		static const int CharaIconDrawIntervalX;
		static const int CharaIconDrawIntervalY;
		/** 敵キャラHP表示縦幅 */
		static const int GaugeDrawHeight;
		/** 状況描画位置 */
		static const int AssetDrawPosX;
		static const int AssetDrawPosY;
		static const int ScoreDrawPosX;
		static const int ScoreDrawPosY;
		static const int EconomicDrawPosX;
		static const int EconomicDrawPosY;
		/** 表示ゲージカラー */
		static const int EnemyHPGaugeColor;
		static const int FriendSkillGaugeColor;
		static const int GaugeBackColor;
		static const int GaugeFrameColor;
		/** 文字の色 */
		static const int StringColor;
		/** 行ごとのキャラアイコン描画数 */
		static const int CharaIconDrawNumRow;
		
		/** 状況表示開始位置 */
		static const int StateDrawPosX;
		static const int StateDrawPosY;

		/** ステータス表示位置 */
		static const int StatusDrawPosX;
		static const int PowerValueDrawPosY;
		static const int SpeedValueDrawPosY;
		static const int RangeValueDrawPosY;
		static const int CostValueDrawPosY;

		/** キャラ描画位置補正 */
		static const int FriendDrawCorr;
		
		/** フォントサイズ */
		static const int FontSize;
		/** フォントスタイル */
		static const char* FontStyle;

		/** お賽銭額表示桁数 */
		static const int AssetNumOfDisits;

		/** スコア表示桁数 */
		static const int ScoreNumOfDisits;
	};
}