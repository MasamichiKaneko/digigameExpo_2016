#pragma once

#include "Sequence\GameSequence\Child.h"
#include <vector>
#include <string>

/**
* �Q�[���J�n�O����
* ���[�f�B���O�Ƃ��͂����ł��
*/
class DivStatus;
class FileLoader;

namespace Sequence{
	namespace GameSequence{
		class Parent;
		class Ready : public Child{
		public:
			Ready(Parent* ,
				std::vector< std::string >* ImgFileList,
				std::vector< DivStatus >* DivImgFileList,
				std::vector< std::string >* TxtFileList,
				std::vector< std::string >* SoundFileList );
			~Ready();
			Base* update( Parent* );
		private:
			int mLoadBackImgHandle;

		};
	}
}