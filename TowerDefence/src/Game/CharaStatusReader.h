#pragma once

/**
* キャラクタステータス読み込みクラス
* 経験値テーブルを読み込んでなければステータスは読めない
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

		/** キャラデータxml読込み */
		bool readCharaData( const char* CharaDataFileName);
		bool readCharaData( int CharaDataFileHandle );

		/** 経験値テーブル読み込み関数 */
		bool readExpTable( const char* ExpTableDataFileName );
		//bool readExpTable( int CharaDataFileHandle );

		void setLevel( int level );

		/** キャラデータxml書き込み */
		void writeCharaData( const char* o_fileName );

		/** 割り振りsp値計算 */
		int culcAssignedSpValue(int id);

		/** ステータス値 */
		typedef struct{
			std::string name;  //名前
			int lvMax;         //Maxレベル
			int lv;            //レベル
			int id;            //固有id
			int totalSp;       //sp総数
			std::map< std::string, int > initValue;         //初期値
			std::map< std::string, int > riseValueEachOneSP;//1SPごとの上昇値
			std::map< std::string, int > assignSPValue;     //割り振りSP値
			std::map< std::string, int > SPMaxValue;        //各ステのSPの最大値
			std::map< std::string, int > TotalValue;        //各ステの現在の合計値
			/** id検索用 */
			bool operator==( const int aId ) const{
				return (id == aId);
			}
		}Status;

		typedef struct{
			int totalExp;         //そのレベルまでに必要な累計経験値
			int needExpForNextLv; //次のレベルまでに必要な経験値
		}expTableValue;

		/** getter */
		std::vector< Status >* getStatusValue() { return &mStatusValue; }
	//	std::map< int, expTableValue >* getExpTable() { return &mExpTable; }
		int getMaxLevel() const{ return maxLevel; }
		int getSpValueEachOneLevel() const { return SPValueEachOneLv; }

		/** SP割り振り関数 */
		bool setAssignSPValue( int id , std::map<std::string, int> assignStatus );

	private:
		/** ステータス値 */
		std::vector< Status > mStatusValue;

		/** 書き込みステータス追加関数 */
		void addWriteStatus( std::string targetNode, std::map<std::string, int>& i_status,  boost::property_tree::ptree& pt );

		/** 経験値テーブル */
		/** first : レベル
		 * second: 経験値情報
		 */
	//	std::map<int, expTableValue> mExpTable;

		/** 最大レベル */
		int maxLevel;

		/** 1lv毎のSP値 */
		int SPValueEachOneLv;

	};
}