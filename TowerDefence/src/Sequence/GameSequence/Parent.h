#pragma once

#include "Sequence\Child.h"
#include <vector>
#include <string>

namespace Game{
	class State;
}

class FileLoader;
class DivStatus;
namespace Sequence{
	class Parent;
	namespace GameSequence{
		class Child;
		class Parent : public Sequence::Child{
		public:
			typedef Sequence::Parent GrandParent;
			/* プレイモード */
			enum Mode{
				MODE_PLAY,

				MODE_NONE,
			};
			Parent( GrandParent::Mode,int StageNum );
			~Parent();
			Base* update( GrandParent* );

			Mode mode() const;
			/** ロード開始関数 */
			void startLoading(
				std::vector< std::string >* ImageFileNameList,
				std::vector< DivStatus >* DivImageFileNameList,
				std::vector< std::string >* TextFileNameList,
				std::vector< std::string >* SoundFileNameList );

			/** 初期状態セット */
			void setInitGameState();

			int getStageNum() const{ return mStageNumber; }
			Game::State* state(){ return mState; };
			static FileLoader* loader(){ return mLoader; };
		private:
			/** 初期読み込みファイルのセット */
			void setInitReadFile();
			void setInitImage();
			void setInitDivImage();
			void setInitText();
			void setInitSound();

			/** 各初期読み込みファイル */
			std::vector< std::string >* mInitImageFileList;
			std::vector< DivStatus >* mInitDivImageFileList;
			std::vector< std::string >* mInitTextFileList;
			std::vector< std::string >* mInitSoundFileList;

			GameSequence::Child* mChild;
			/* 選択されたステージ */
			int mStageNumber;
			/* ゲームの状態 */
			Game::State* mState;
			/* Readyクラスで読み込んだ情報保持用 */
			static FileLoader* mLoader;
		};
	}
}