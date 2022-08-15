#pragma once

#include<optional>
#include<string>
#include<fstream>
#include<sstream>
#include<filesystem>	// C++17以上の機能
#include"SafetyDataType.h"

// ファイルに関する便利系関数ライブラリ
namespace FileUtility {

	// ビット演算：https://qiita.com/qiita_kuru/items/3a6ab432ffb6ae506758#n%E3%83%93%E3%83%83%E3%83%88%E7%9B%AE%E3%82%92%E5%8F%8D%E8%BB%A2%E3%81%99%E3%82%8B

	// IOマニピュレーター
	//using _OpenMode = std::_Iosb<int_t>::_Openmode;	// ファイルの展開方法			// https://cpprefjp.github.io/reference/ios/ios_base/type-openmode.html
	//using _Format = std::_Iosb<int_t>::_Fmtflags;		// ファイルのフォーマット情報	// 
	//using _Seek = std::_Iosb<int_t>::_Seekdir;			// ファイル内のシーク座標
	using _OpenMode = std::ios_base::openmode;	// ファイルの展開方法			
	using _Format = std::ios_base::fmtflags;		// ファイルのフォーマット情報	
	using _Seek = std::ios_base::seekdir;			// ファイル内のシーク座標
	
	// ビットマスク
	constexpr _Format _MaskBit = std::ios::_Fmtmask;
	constexpr _Format _ZeroBit = std::ios::_Fmtzero;

	namespace Utility {

#if _MSVC_LANG > 201402L

		// ファイルの行数を取得
		inline ulonglong_t FileColumn( std::filesystem::path path ) {
			std::fstream fs( path, std::ios::in );
			std::string lineStr;
			ulonglong_t i = 0;
			while( std::getline( fs, lineStr ) ) i++;

			return i;
		}

		// 拡張子を含むファイル名を取得
		inline std::string FileNameExtraction( std::filesystem::path path ) {
			return path.filename().u8string();
		}

		// ファイル名を取得
		inline std::string FileName( std::filesystem::path path ) {
			return path.stem().u8string();
		}

		// ファイル拡張子を取得
		inline std::string FileExtension( std::filesystem::path path ) {
			return path.extension().u8string();
		}

		// ファイル/フォルダが存在するか確認
		inline bool Exist( std::filesystem::path path ) {
			return std::filesystem::exists( path );
		}

		// ファイルのデータ量を測定
		inline std::size_t FileSize( std::filesystem::path path ) {
			return std::filesystem::file_size( path );
		}

		// ファイルのデータ量を測定
		inline std::size_t FileSize( std::fstream& fs ) {
			auto curPos = fs.tellg();		// ファイルの現在の読み込み位置を保存
			fs.seekg( 0, std::ios::end );	// ファイル読み込み位置を終端まで移動
			auto size = fs.tellg();			// 現在の読み込み位置を取得（サイズ）
			fs.seekg( 0, curPos );			// ファイルの読み込み位置を元の場所に移動
			return size;
		}

		// フォルダの作成
		inline void CreateFolder( std::string path ) {
			std::filesystem::create_directory( path );
		}

#else

		// ファイル名を取得する
		inline std::string FileName( std::string path ) {
			if( path.find_last_of( "/" ) == std::string::npos ) {
				size_t index = path.find_last_of( "." );
				return path.substr( 0, index - 1 );
			}
			else {
				size_t sindex = path.find_last_of( "/" );
				size_t eindex = path.find_last_of( "." );
				return path.substr( sindex + 1, eindex - 1 );
			}
		}

		// ファイル拡張子を取得
		inline std::string FileExtension( std::string path ) {
			size_t index = path.find_last_of( "." );
			return path.substr( index + 1, path.size() - index );
		}

		// ファイルが存在するか確認
		inline bool FildExist( std::string path ) {
			std::fstream fs( path );
			if( fs ) {
				fs.close();
				return true;
			}
			else {
				fs.close();
				false;
			}
		}

		// ファイルのデータ量を測定
		inline std::size_t FileSize( std::fstream& fs ) {
			auto curPos = fs.tellg();		// ファイルの現在の読み込み位置を保存
			fs.seekg( 0, std::ios::end );	// ファイル読み込み位置を終端まで移動
			auto size = fs.tellg();			// 現在の読み込み位置を取得（サイズ）
			fs.seekg( 0, curPos );			// ファイルの読み込み位置を元の場所に移動
			return size;
		}

				// フォルダの作成
		inline void CreateFolder( std::string path ) {
			CreateDirectoryA( path.c_str(), NULL );
		}

#endif


	}

	// バイナリファイル用
	namespace Binary {



		// ファイルをロードしfstreamを返す
		inline std::optional<std::fstream> LoadFile( std::filesystem::path path, _OpenMode openMode = std::ios::in, _Format format = std::ios::hex  ) {
			if( FileUtility::Utility::Exist( path ) ) {
				// ファイルが存在する場合
				std::fstream fs( path, std::ios::in | std::ios::binary | openMode | format );
				return fs;
			}
			else {
				// ファイルが存在しない場合無効値を返す
				return std::nullopt;
			}
		}


		inline const std::vector<char>& Read( std::fstream& fs ) {
			if( fs ) {
				auto size = FileUtility::Utility::FileSize( fs );

				std::vector<char> byteData(size);
				fs.read( byteData.data(), size );
				return byteData;
			}
		}

	}

	// テキストファイル用
	namespace Text {

		// ファイルを作成
		inline std::optional<std::fstream> CreateTextFile( std::filesystem::path path ) {
			std::ofstream ofs( path );
			ofs.close();
			std::fstream fs( path, std::ios::out );
			return fs;
		}

		// ファイルをロードしfstreamを返す
		inline std::optional<std::fstream> LoadFile( std::filesystem::path path, _OpenMode openMode = std::ios::in, _Format format = _ZeroBit ) {
			if( FileUtility::Utility::Exist( path ) ) {
				// ファイルが存在する場合
				// 5ビット目が1かどうかを判定（5ビット目はstd::ios::binary : 0b100000：[32]）
				if( (( openMode >> 5 ) & 1) == 1 ) {
					// std::ios::binaryが使用されていた場合、その部分のビットのみ反転させる
					uint_t _openMode = openMode;	// フラグを反転するためにenumから数値型へ変更
					_openMode ^= ( 1 << 5 );		// 5ビット目を反転
					std::fstream fs( path, std::ios::in | _openMode | format );
					return fs;
				}
				else {
					std::fstream fs( path.u8string(), std::ios::in | openMode | format );
#if _DEBUG
					try {
						if( fs ) {
							return fs;
						}
						else if( fs.is_open() ) {
							throw std::runtime_error( "is opened" );
						}
						else {
							throw std::runtime_error( "file open failed" );
						}
					}
					catch( std::exception& except ) {
						std::cout << except.what() << std::endl;
						return fs;
					}
#endif
					return fs;
				}
			}
			else {
				// ファイルが存在しない場合無効値を返す
				return std::nullopt;
			}
		}

		// ファイルを読み込み用でロードする
		inline std::optional<std::fstream> LoadReadFile( std::filesystem::path path, _OpenMode openMode = std::ios::in, _Format format = _ZeroBit ) {
			return Text::LoadFile( path, std::ios::in | openMode, format );
		}

		// ファイルを書き込み用でロードする
		inline std::optional<std::fstream> LoadWriteFile( std::filesystem::path path, _OpenMode openMode = std::ios::out, _Format format = _ZeroBit ) {
			return Text::LoadFile( path, std::ios::out | openMode, format );
		}

		//===================================================
		// ファイルの読み込み
		//===================================================

		// ファイルの内容を読み込む
		inline std::optional<std::vector<char>> Read( std::fstream& fs ) {
			if( fs ) {
				auto size = FileUtility::Utility::FileSize( fs );

				std::vector<char> byteData( size );
				fs.read( byteData.data(), size );
				return byteData;
			}
			else {
				return std::nullopt;
			}
		}

		// ファイルの内容を文字列として読み込む
		inline std::optional<std::vector<std::string>> ReadString( std::fstream& fs ) {
			if( fs ) {
				//// ファイルの行数を取得
				//auto columns = FileUtility::Utility::FileColumn( fs );
				//
				//// 行数分のバッファを確保
				//std::vector<std::string> lineBuffers( columns );
				//
				//// 行数分のデータを読み込み
				//for( ulonglong_t i = 0; i < columns; i++ ) std::getline( fs, lineBuffers[i] );
				//
				//// 全行分のデータを返す
				//return lineBuffers;
				std::vector<std::string> lineBuffers;
				std::string tmp;
				while( std::getline( fs, tmp ) ) lineBuffers.push_back( tmp );
				return lineBuffers;
			}
			else {
				return std::nullopt;
			}
		}

		// ファイルをロードして、ファイルの内容を読み込む
		inline std::optional<std::vector<char>> LoadAndRead( std::filesystem::path path, _OpenMode openMode = std::ios::in, _Format format = _ZeroBit ) {
			auto fs = Text::LoadReadFile( path, openMode, format );
			return ( fs.has_value() ) ? Read( fs.value() ) : std::nullopt;
		}

		// ファイルをロードして、ファイルの内容を文字列として読み込む
		inline std::optional<std::vector<std::string>> LoadAndReadString( std::filesystem::path path, _OpenMode openMode = std::ios::in, _Format format = _ZeroBit ) {
			auto fs = Text::LoadReadFile( path, openMode, format );
			return ( fs.has_value() ) ? ReadString( fs.value() ) : std::nullopt;
		}

		//===================================================
		// ファイルの書き込み
		//===================================================

		// ファイルに内容を書き込む
		inline void Write( const std::vector<char>& byteData, std::fstream& fs ) {
			if( fs ) {
				fs.write( byteData.data(), byteData.size() );
			}
		}

		// ファイルに内容を書き込む
		inline void Write( const std::vector<std::string>& lineDatas, std::fstream& fs ) {
			if( fs ) {
				auto size = lineDatas.size();
				for( std::size_t i = 0; i < size; i++ ) fs << lineDatas[i];
			}
		}

		// ファイルをロードして、ファイルに内容を書き込む
		inline void LoadAndWrite( const std::vector<char>& byteData, std::filesystem::path path, _OpenMode openMode = std::ios::out, _Format format = _ZeroBit ) {
			auto fs = Text::LoadWriteFile( path, openMode, format );

			if( fs.has_value() && fs.value() ) {
				Text::Write( byteData, fs.value() );
			}
			else {
				fs = Text::CreateTextFile( path );
				Text::Write( byteData, fs.value() );
			}
		}

		// ファイルをロードして、ファイルに内容を書き込む
		inline void LoadAndWrite( const std::vector<std::string>& lineDatas, std::filesystem::path path, _OpenMode openMode = std::ios::out, _Format format = _ZeroBit ) {
			auto fs = Text::LoadWriteFile( path, openMode, format );

			if( fs.has_value() && fs.value()) {
				Text::Write( lineDatas, fs.value() );
			}
			else {
				fs = Text::CreateTextFile( path );
				Text::Write( lineDatas, fs.value() );
			}
		}

		// ファイルをロードして、ファイルに内容を書き込む
		inline void LoadAndWrite( std::initializer_list<std::string> lineDatas, std::filesystem::path path, _OpenMode openMode = std::ios::out, _Format format = _ZeroBit ) {
			auto fs = Text::LoadWriteFile( path, openMode, format );

			if( fs.has_value() && fs.value() ) {
				Text::Write( lineDatas, fs.value() );
			}
			else {
				fs = Text::CreateTextFile( path );
				Text::Write( lineDatas, fs.value() );
			}
		}

	}

	// CSVファイル
	namespace CSV {

	}

}