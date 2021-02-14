#pragma once

/**
* �L�����N�^�X�e�[�^�X�ǂݍ��݃N���X
* �o���l�e�[�u����ǂݍ���łȂ���΃X�e�[�^�X�͓ǂ߂Ȃ�
*/

#include <vector>
#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/exception/all.hpp>

namespace Game{
	class CharaStatusReader{
	public:
		
		CharaStatusReader();
		CharaStatusReader( const char* CharaDataFileName );

		~CharaStatusReader();

		/** �L�����f�[�^xml�Ǎ��� */
		bool readCharaData( const char* CharaDataFileName);
		bool readCharaData( int CharaDataFileHandle );

		/** �o���l�e�[�u���ǂݍ��݊֐� */
		bool readExpTable( const char* ExpTableDataFileName );
		//bool readExpTable( int CharaDataFileHandle );

		void setLevel( int level );

		/** �L�����f�[�^xml�������� */
		void writeCharaData( const char* o_fileName );

		/** ����U��sp�l�v�Z */
		int culcAssignedSpValue(int id);

		/** �X�e�[�^�X�l */
		typedef struct{
			std::string name;  //���O
			int lvMax;         //Max���x��
			int lv;            //���x��
			int id;            //�ŗLid
			int totalSp;       //sp����
			std::map< std::string, int > initValue;         //�����l
			std::map< std::string, int > riseValueEachOneSP;//1SP���Ƃ̏㏸�l
			std::map< std::string, int > assignSPValue;     //����U��SP�l
			std::map< std::string, int > SPMaxValue;        //�e�X�e��SP�̍ő�l
			std::map< std::string, int > TotalValue;        //�e�X�e�̌��݂̍��v�l
			/** id�����p */
			bool operator==( const int aId ) const{
				return (id == aId);
			}
		}Status;

		typedef struct{
			int totalExp;         //���̃��x���܂łɕK�v�ȗ݌v�o���l
			int needExpForNextLv; //���̃��x���܂łɕK�v�Ȍo���l
		}expTableValue;

		/** getter */
		std::vector< Status >* getStatusValue() { return &mStatusValue; }
	//	std::map< int, expTableValue >* getExpTable() { return &mExpTable; }
		int getMaxLevel() const{ return maxLevel; }
		int getSpValueEachOneLevel() const { return SPValueEachOneLv; }

		/** SP����U��֐� */
		bool setAssignSPValue( int id , std::map<std::string, int> assignStatus );

	private:
		/** �X�e�[�^�X�l */
		std::vector< Status > mStatusValue;

		/** �������݃X�e�[�^�X�ǉ��֐� */
		void addWriteStatus( std::string targetNode, std::map<std::string, int>& i_status,  boost::property_tree::ptree& pt );

		/** �o���l�e�[�u�� */
		/** first : ���x��
		 * second: �o���l���
		 */
	//	std::map<int, expTableValue> mExpTable;

		/** �ő僌�x�� */
		int maxLevel;

		/** 1lv����SP�l */
		int SPValueEachOneLv;

	};
}