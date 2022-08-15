#pragma once
// Json�t�@�C�����ȒP�Ɉ������߂̃��C�u����


#include<string>
#include<iostream>
#include<fstream>
#include"SafetyDataType.h"

// rapidjson���C�u�����̃C���N���[�h
#include<rapidjson/document.h>
#include<rapidjson/writer.h>
#include<rapidjson/stringbuffer.h>
#include<rapidjson/istreamwrapper.h>
#include<rapidjson/ostreamwrapper.h>
#include<rapidjson/filereadstream.h>
#include<rapidjson/filewritestream.h>

// nlohmann-json���C�u�����̃C���N���[�h
#include"json.hpp"

namespace JsonUtility {

	// ���̃��C�u�����Ŏg�p����Ă���O�����C�u�����FRapidJson
	// �����T�C�g			�Fhttp://rapidjson.org/index.html
	// �����`���[�g���A���Fhttp://rapidjson.org/md_doc_tutorial.html
	// �z�z��GitHub		�Fhttps://github.com/Tencent/rapidjson/

	// RapidJson�Ɋւ���Q�l�T�C�g	�Fhttps://pystyle.info/cpp-rapidjson-basic-usage/
	// Json�t�@�C���Ɋւ���Q�l�T�C�g�Fhttps://www.tohoho-web.com/ex/json.html

	// �G�C���A�X�錾
	using Document = rapidjson::Document;
	using ValueType = rapidjson::Type;



	// Json�t�@�C���̃f�[�^���擾
	// path : Json�t�@�C���ւ̑��΃p�X
	inline void Deserialize( std::string path, JsonUtility::Document& document ) {

		// �t�@�C���X�g���[���̎擾
		std::ifstream ifs( path.c_str() );
		rapidjson::IStreamWrapper isw( ifs );

		// �p�[�X
		document.ParseStream( isw );

	}


	// Json�t�@�C���̃f�[�^���擾
	// path : Json�t�@�C���ւ̑��΃p�X
	inline void _Deserialize( std::string path, JsonUtility::Document& document ) {

		FILE* fp = fopen( path.c_str(), "r" ); // non-Windows use "r"

		char readBuffer[65536];
		rapidjson::FileReadStream is( fp, readBuffer, sizeof( readBuffer ) );

		// �p�[�X
		document.ParseStream( is );
		fclose( fp );

	}

	// Json�t�@�C���ւ̏�������
	// document : �������ރf�[�^��ێ�����JsonUtility::Document�^�ϐ�
	// path : �������ݐ�t�@�C���ւ̃p�X
	inline void Serialize( std::string path, const JsonUtility::Document& document) {

		std::ofstream ofs( path.c_str() );
		rapidjson::OStreamWrapper osw( ofs );

		rapidjson::Writer<rapidjson::OStreamWrapper> writer( osw );
		document.Accept( writer );
	}

	// Json�t�@�C���ւ̏�������
	// document : �������ރf�[�^��ێ�����JsonUtility::Document�^�ϐ�
	// path : �������ݐ�t�@�C���ւ̃p�X
	inline void _Serialize( std::string path, const JsonUtility::Document& document ) {

		FILE* fp = fopen( path.c_str(), "w" ); // non-Windows use "w"

		char writeBuffer[65536];
		rapidjson::FileWriteStream os( fp, writeBuffer, sizeof( writeBuffer ) );

		rapidjson::Writer<rapidjson::FileWriteStream> writer( os );
		document.Accept( writer );

		fclose( fp );

	}





	// Json�N���X
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

	// ���̃C���^�[�t�F�C�X�N���X��ʏ�̃N���X�Ɍp��������
	struct JsonInterface {
		virtual void Serialize( Json obj ) = 0;
		virtual Json Deserialize() = 0;
	};



}