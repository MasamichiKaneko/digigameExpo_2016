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
			/* �v���C���[�h */
			enum Mode{
				MODE_PLAY,

				MODE_NONE,
			};
			Parent( GrandParent::Mode,int StageNum );
			~Parent();
			Base* update( GrandParent* );

			Mode mode() const;
			/** ���[�h�J�n�֐� */
			void startLoading(
				std::vector< std::string >* ImageFileNameList,
				std::vector< DivStatus >* DivImageFileNameList,
				std::vector< std::string >* TextFileNameList,
				std::vector< std::string >* SoundFileNameList );

			/** ������ԃZ�b�g */
			void setInitGameState();

			int getStageNum() const{ return mStageNumber; }
			Game::State* state(){ return mState; };
			static FileLoader* loader(){ return mLoader; };
		private:
			/** �����ǂݍ��݃t�@�C���̃Z�b�g */
			void setInitReadFile();
			void setInitImage();
			void setInitDivImage();
			void setInitText();
			void setInitSound();

			/** �e�����ǂݍ��݃t�@�C�� */
			std::vector< std::string >* mInitImageFileList;
			std::vector< DivStatus >* mInitDivImageFileList;
			std::vector< std::string >* mInitTextFileList;
			std::vector< std::string >* mInitSoundFileList;

			GameSequence::Child* mChild;
			/* �I�����ꂽ�X�e�[�W */
			int mStageNumber;
			/* �Q�[���̏�� */
			Game::State* mState;
			/* Ready�N���X�œǂݍ��񂾏��ێ��p */
			static FileLoader* mLoader;
		};
	}
}