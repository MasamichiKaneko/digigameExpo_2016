#pragma once


class Score;
namespace Sequence{
	class Child;

	class Parent{
	public:

		/* �Q�[�����[�h */
		/* �I�v�V�����Ƃ��X�R�A�\���Ƃ� */
		/* �Q�[�����e�ɂ���Ă�1P�Ƃ�2P�Ƃ� */
		enum Mode{
			MODE_PLAY,    //�Q�[���{��

			MODE_NONE,    //�����Ȃ�(�J�ڃG���[���o�p)
		};
		void update();
		Mode mode() const;
		void setMode( Mode );

		static void create();
		static void destroy();
		static Parent* instance();
		Score* getScore(){ return mScore; }
		int getBGMHandle(){ return mBGMHandle; }
		int getDecisionHandle(){ return mDecisionSEHandle; }

		/** �Q�[���I���t���O */
		/** true�ɕύX���ꂽ��v���O��������� */
		static bool isGameEsc;

	private:
		Parent();
		~Parent();

		Mode mMode;

		int mBGMHandle;
		int mDecisionSEHandle;

		Child* mChild;
		Score* mScore;
		static Parent* mInstance;
	};

} //namespace Sequence