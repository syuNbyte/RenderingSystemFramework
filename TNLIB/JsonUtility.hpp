#pragma once
// Jsonファイルを簡単に扱うためのライブラリ


#include<string>
#include<iostream>
#include<fstream>
#include"SafetyDataType.h"

// rapidjsonライブラリのインクルード
#include<rapidjson/document.h>
#include<rapidjson/writer.h>
#include<rapidjson/stringbuffer.h>
#include<rapidjson/istreamwrapper.h>
#include<rapidjson/ostreamwrapper.h>
#include<rapidjson/filereadstream.h>
#include<rapidjson/filewritestream.h>

// nlohmann-jsonライブラリのインクルード
#include"json.hpp"

namespace JsonUtility {

	// このライブラリで使用されている外部ライブラリ：RapidJson
	// 公式サイト			：http://rapidjson.org/index.html
	// 公式チュートリアル：http://rapidjson.org/md_doc_tutorial.html
	// 配布元GitHub		：https://github.com/Tencent/rapidjson/

	// RapidJsonに関する参考サイト	：https://pystyle.info/cpp-rapidjson-basic-usage/
	// Jsonファイルに関する参考サイト：https://www.tohoho-web.com/ex/json.html

	// エイリアス宣言
	using Document = rapidjson::Document;
	using ValueType = rapidjson::Type;



	// Jsonファイルのデータを取得
	// path : Jsonファイルへの相対パス
	inline void Deserialize( std::string path, JsonUtility::Document& document ) {

		// ファイルストリームの取得
		std::ifstream ifs( path.c_str() );
		rapidjson::IStreamWrapper isw( ifs );

		// パース
		document.ParseStream( isw );

	}


	// Jsonファイルのデータを取得
	// path : Jsonファイルへの相対パス
	inline void _Deserialize( std::string path, JsonUtility::Document& document ) {

		FILE* fp = fopen( path.c_str(), "r" ); // non-Windows use "r"

		char readBuffer[65536];
		rapidjson::FileReadStream is( fp, readBuffer, sizeof( readBuffer ) );

		// パース
		document.ParseStream( is );
		fclose( fp );

	}

	// Jsonファイルへの書き込み
	// document : 書き込むデータを保持するJsonUtility::Document型変数
	// path : 書き込み先ファイルへのパス
	inline void Serialize( std::string path, const JsonUtility::Document& document) {

		std::ofstream ofs( path.c_str() );
		rapidjson::OStreamWrapper osw( ofs );

		rapidjson::Writer<rapidjson::OStreamWrapper> writer( osw );
		document.Accept( writer );
	}

	// Jsonファイルへの書き込み
	// document : 書き込むデータを保持するJsonUtility::Document型変数
	// path : 書き込み先ファイルへのパス
	inline void _Serialize( std::string path, const JsonUtility::Document& document ) {

		FILE* fp = fopen( path.c_str(), "w" ); // non-Windows use "w"

		char writeBuffer[65536];
		rapidjson::FileWriteStream os( fp, writeBuffer, sizeof( writeBuffer ) );

		rapidjson::Writer<rapidjson::FileWriteStream> writer( os );
		document.Accept( writer );

		fclose( fp );

	}





	// Jsonクラス
	class Json {
	private:
		std::string path;
	public:
		JsonUtility::Document document;
	public:
		explicit Json( std::string path ) : path( path ) {}
	public:
		void Serialize() {
			JsonUtility::Serialize( path, document );
		}
		void Deserialize() {
			JsonUtility::Deserialize( path, document );
		}
	};

}


namespace _JsonUtility {

	using Json = nlohmann::json;

	// このインターフェイスクラスを通常のクラスに継承させる
	struct JsonInterface {
		virtual void Serialize( Json obj ) = 0;
		virtual Json Deserialize() = 0;
	};



}