#include "Game\CharaStatusReader.h"
#include "utils.h"
#include <sstream>
#include "XMLNodeNameString.h"
#include "DxLib.h"
#include "FileNames.h"
#include "define.h"
#include <sstream>
#include <algorithm>
#include "CommonString.h"
#include <sys\stat.h>
#include <numeric>

using namespace std;
using namespace boost::property_tree::xml_parser;
using namespace boost::property_tree;
using namespace CharaDataXml;

namespace Game{

	int map_acc(int lhs, const std::pair<string, int> & rhs)
	{
		return lhs + rhs.second;
	}

	CharaStatusReader::CharaStatusReader() : maxLevel( 0 ){
	}

	CharaStatusReader::CharaStatusReader( const char* CharaDataFileName ) : maxLevel( 0 ){
		readCharaData( CharaDataFileName );
	}

	CharaStatusReader::~CharaStatusReader(){
	}

	int CharaStatusReader::culcAssignedSpValue( int id ){
		int value = -1;
		vector<Status>::iterator it = find( mStatusValue.begin(), mStatusValue.end(), id );
		map< string, int > assignValue;

		if ( it == mStatusValue.end()){
			goto FUNC_END;
		}
		assignValue = it->assignSPValue;

		value = std::accumulate( assignValue.begin(), assignValue.end(), 0, map_acc );
FUNC_END:
		return value;
	}

	/** �L�����f�[�^xml�Ǎ��� */
	bool CharaStatusReader::readCharaData( const char* CharaDataFileName){
		
		bool result = false;
		int fileHandle;

		/** �t�@�C�������݂��Ȃ���ΏI�� */
		struct stat ifile_stat;
		if ( stat( CharaDataFileName, &ifile_stat ) != 0 ) {
			goto FUNC_END;
		}

		/** �t�@�C���ǂݍ��� */
		fileHandle = FileRead_open( CharaDataFileName );
		result = readCharaData( fileHandle );
		FileRead_close( fileHandle );
FUNC_END:

		return result;
	}

	bool CharaStatusReader::readCharaData( int CharaDataFileHandle ) {

		mStatusValue.clear();
		bool result = true;
		ptree pt_status;
		/** �ǂݍ��݃o�b�t�@ */
		boost::optional<string> nameBuf;
		boost::optional<int> idBuf;
		boost::optional<int> lvBuf;
		boost::optional<int> lvMaxBuf;

		/** �L�����f�[�^xml�ǂݍ��� */
		if ( !readXml( CharaDataFileHandle, pt_status ) ){
			result = false;
			goto FUNC_END;
		}

		/** �L�����f�[�^�Ǎ��� */
		BOOST_FOREACH( ptree::value_type& child , pt_status.get_child(RootNode) ) {
			ptree& charaInfo = child.second;
			Status statusBuf;

			/** ���O�擾 */
			nameBuf = charaInfo.get_optional<string>( CharaNameAttr );
			/** id�擾 */
			idBuf = charaInfo.get_optional<int>( CharaIdAttr );
			/** ���x���ǂݍ��� */
			lvBuf = charaInfo.get_optional<int>( CharaLvValueAttr );
			/** Max���x���ǂݍ��� */
			lvMaxBuf = charaInfo.get_optional<int>( CharaLvMaxValueAttr );
			/** 1�ł��ǂݍ��߂Ȃ���Ύ��s */
			if ( !( nameBuf && idBuf && lvMaxBuf && lvBuf ) ) {
				result = false;
				goto FUNC_END;
			}
			statusBuf.name    = *nameBuf;
			statusBuf.id      = *idBuf;
			statusBuf.lv      = *lvBuf;
			statusBuf.lvMax   = *lvMaxBuf;
			statusBuf.totalSp = statusBuf.lv * SPValueEachOneLv;

			/** �����l�ǂݍ��� */
			if ( !readAttr( InitNode, statusBuf.initValue, charaInfo, CharaDataXml::StatusStrVector )) {
				result = false;
				goto FUNC_END;
			}
			/** �㏸�l�ǂݍ��� */
			if ( !readAttr( SpRisingValueNode, statusBuf.riseValueEachOneSP , charaInfo, CharaDataXml::StatusStrVector ) ) {
				result = false;
				goto FUNC_END;
			}
			/** �琬�f�[�^�Ǎ��� */
			if ( !readAttr( SPNode, statusBuf.assignSPValue, charaInfo, CharaDataXml::StatusStrVector ) ) {
				result = false;
				goto FUNC_END;
			}
			/** �e�X�eSP�ő�l�ǂݍ��� */
			if ( !readAttr( SPMaxNode, statusBuf.SPMaxValue, charaInfo ,CharaDataXml::StatusStrVector ) ) {
				result = false;
				goto FUNC_END;
			}

			/** �e�X�e���v�l�v�Z */
			map< string, int > totalBuf;
			for ( unsigned int i = 0; i < CharaDataXml::StatusStrVector.size(); i++ ) {
				string valueType = CharaDataXml::StatusStrVector.at(i);
				/** �����l�擾 */
				int value = statusBuf.initValue.at(valueType);
				/** ���Z����ǉ� */
				value += statusBuf.assignSPValue.at(valueType) * statusBuf.riseValueEachOneSP.at(valueType);
				totalBuf[valueType] = value;
			}
			statusBuf.TotalValue = totalBuf;

			mStatusValue.push_back( statusBuf );
		}
FUNC_END:
		/** ���s�Ȃ�ǂݍ��񂾕������� */
		if ( !result ){
			mStatusValue.clear();
		}

		return result;
	}

	/** �L�����f�[�^xml�������� */
	void CharaStatusReader::writeCharaData( const char* fileName ) {
		ptree pt;
		for ( unsigned int i = 0; i < mStatusValue.size(); i++ ) {
			ptree& child = pt.add( RootNode + "." + CharaNode , "" );
			child.put( CharaNameAttr, mStatusValue.at(i).name );
			child.put( CharaIdAttr  , mStatusValue.at(i).id );
			child.put( CharaLvMaxValueAttr , mStatusValue.at(i).lvMax );
			child.put( CharaLvValueAttr  , mStatusValue.at(i).lv );

			addWriteStatus( InitNode         , mStatusValue.at(i).initValue         , child );
			addWriteStatus( SpRisingValueNode, mStatusValue.at(i).riseValueEachOneSP, child );
			addWriteStatus( SPNode           , mStatusValue.at(i).assignSPValue     , child );
			addWriteStatus( SPMaxNode        , mStatusValue.at(i).SPMaxValue        , child );
		}
		write_xml(fileName, pt, std::locale());
	}

	/** �������݃X�e�[�^�X�ǉ��֐� */
	void CharaStatusReader::addWriteStatus( string targetNode, map<string, int>& i_status, ptree& pt ){
		for ( unsigned int i = 0; i < CharaDataXml::StatusStrVector.size(); i++ ) {
			/** �������ݒ� */
			string writeAttr = targetNode+ "." + XmlCommon::attrStr + CharaDataXml::StatusStrVector.at(i);
			pt.put( writeAttr, i_status[CharaDataXml::StatusStrVector.at(i)] );
		}
	}


	void CharaStatusReader::setLevel( int level ) {
		if ( level < 0 || mStatusValue.at(0).lvMax < level ) {
			return;
		}
		for ( unsigned int i = 0; i < mStatusValue.size(); i++ ) {
			mStatusValue.at( i ).lv = level;
			for ( unsigned int j = 0; j < VariableStatusStrVector.size(); j++ ) {
				string typeName = VariableStatusStrVector.at( j );
				mStatusValue.at(i).assignSPValue[typeName] = level;
			}
		}
	}

	/** SP����U��֐� */
	bool CharaStatusReader::setAssignSPValue( int id, map<string, int> assignStatus ){
		bool result = true;
		vector<Status>::iterator it = find( mStatusValue.begin(), mStatusValue.end(), id );

		if ( it == mStatusValue.end() ) {
			result = false;
			goto FUNC_END;
		}

		for ( unsigned int i = 0; i < CharaDataXml::StatusStrVector.size(); i++ ) {
			it->assignSPValue[CharaDataXml::StatusStrVector.at(i)] 
			= assignStatus[CharaDataXml::StatusStrVector.at(i)];
		}

FUNC_END:
		return result;
	}
}