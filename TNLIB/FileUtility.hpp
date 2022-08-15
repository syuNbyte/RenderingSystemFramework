#pragma once

#include<optional>
#include<string>
#include<fstream>
#include<sstream>
#include<filesystem>	// C++17�ȏ�̋@�\
#include"SafetyDataType.h"

// �t�@�C���Ɋւ���֗��n�֐����C�u����
namespace FileUtility {

	// �r�b�g���Z�Fhttps://qiita.com/qiita_kuru/items/3a6ab432ffb6ae506758#n%E3%83%93%E3%83%83%E3%83%88%E7%9B%AE%E3%82%92%E5%8F%8D%E8%BB%A2%E3%81%99%E3%82%8B

	// IO�}�j�s�����[�^�[
	//using _OpenMode = std::_Iosb<int_t>::_Openmode;	// �t�@�C���̓W�J���@			// https://cpprefjp.github.io/reference/ios/ios_base/type-openmode.html
	//using _Format = std::_Iosb<int_t>::_Fmtflags;		// �t�@�C���̃t�H�[�}�b�g���	// 
	//using _Seek = std::_Iosb<int_t>::_Seekdir;			// �t�@�C�����̃V�[�N���W
	using _OpenMode = std::ios_base::openmode;	// �t�@�C���̓W�J���@			
	using _Format = std::ios_base::fmtflags;		// �t�@�C���̃t�H�[�}�b�g���	
	using _Seek = std::ios_base::seekdir;			// �t�@�C�����̃V�[�N���W
	
	// �r�b�g�}�X�N
	constexpr _Format _MaskBit = std::ios::_Fmtmask;
	constexpr _Format _ZeroBit = std::ios::_Fmtzero;

	namespace Utility {

#if _MSVC_LANG > 201402L

		// �t�@�C���̍s�����擾
		inline ulonglong_t FileColumn( std::filesystem::path path ) {
			std::fstream fs( path, std::ios::in );
			std::string lineStr;
			ulonglong_t i = 0;
			while( std::getline( fs, lineStr ) ) i++;

			return i;
		}

		// �g���q���܂ރt�@�C�������擾
		inline std::string FileNameExtraction( std::filesystem::path path ) {
			return path.filename().u8string();
		}

		// �t�@�C�������擾
		inline std::string FileName( std::filesystem::path path ) {
			return path.stem().u8string();
		}

		// �t�@�C���g���q���擾
		inline std::string FileExtension( std::filesystem::path path ) {
			return path.extension().u8string();
		}

		// �t�@�C��/�t�H���_�����݂��邩�m�F
		inline bool Exist( std::filesystem::path path ) {
			return std::filesystem::exists( path );
		}

		// �t�@�C���̃f�[�^�ʂ𑪒�
		inline std::size_t FileSize( std::filesystem::path path ) {
			return std::filesystem::file_size( path );
		}

		// �t�@�C���̃f�[�^�ʂ𑪒�
		inline std::size_t FileSize( std::fstream& fs ) {
			auto curPos = fs.tellg();		// �t�@�C���̌��݂̓ǂݍ��݈ʒu��ۑ�
			fs.seekg( 0, std::ios::end );	// �t�@�C���ǂݍ��݈ʒu���I�[�܂ňړ�
			auto size = fs.tellg();			// ���݂̓ǂݍ��݈ʒu���擾�i�T�C�Y�j
			fs.seekg( 0, curPos );			// �t�@�C���̓ǂݍ��݈ʒu�����̏ꏊ�Ɉړ�
			return size;
		}

		// �t�H���_�̍쐬
		inline void CreateFolder( std::string path ) {
			std::filesystem::create_directory( path );
		}

#else

		// �t�@�C�������擾����
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

		// �t�@�C���g���q���擾
		inline std::string FileExtension( std::string path ) {
			size_t index = path.find_last_of( "." );
			return path.substr( index + 1, path.size() - index );
		}

		// �t�@�C�������݂��邩�m�F
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

		// �t�@�C���̃f�[�^�ʂ𑪒�
		inline std::size_t FileSize( std::fstream& fs ) {
			auto curPos = fs.tellg();		// �t�@�C���̌��݂̓ǂݍ��݈ʒu��ۑ�
			fs.seekg( 0, std::ios::end );	// �t�@�C���ǂݍ��݈ʒu���I�[�܂ňړ�
			auto size = fs.tellg();			// ���݂̓ǂݍ��݈ʒu���擾�i�T�C�Y�j
			fs.seekg( 0, curPos );			// �t�@�C���̓ǂݍ��݈ʒu�����̏ꏊ�Ɉړ�
			return size;
		}

				// �t�H���_�̍쐬
		inline void CreateFolder( std::string path ) {
			CreateDirectoryA( path.c_str(), NULL );
		}

#endif


	}

	// �o�C�i���t�@�C���p
	namespace Binary {



		// �t�@�C�������[�h��fstream��Ԃ�
		inline std::optional<std::fstream> LoadFile( std::filesystem::path path, _OpenMode openMode = std::ios::in, _Format format = std::ios::hex  ) {
			if( FileUtility::Utility::Exist( path ) ) {
				// �t�@�C�������݂���ꍇ
				std::fstream fs( path, std::ios::in | std::ios::binary | openMode | format );
				return fs;
			}
			else {
				// �t�@�C�������݂��Ȃ��ꍇ�����l��Ԃ�
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

	// �e�L�X�g�t�@�C���p
	namespace Text {

		// �t�@�C�����쐬
		inline std::optional<std::fstream> CreateTextFile( std::filesystem::path path ) {
			std::ofstream ofs( path );
			ofs.close();
			std::fstream fs( path, std::ios::out );
			return fs;
		}

		// �t�@�C�������[�h��fstream��Ԃ�
		inline std::optional<std::fstream> LoadFile( std::filesystem::path path, _OpenMode openMode = std::ios::in, _Format format = _ZeroBit ) {
			if( FileUtility::Utility::Exist( path ) ) {
				// �t�@�C�������݂���ꍇ
				// 5�r�b�g�ڂ�1���ǂ����𔻒�i5�r�b�g�ڂ�std::ios::binary : 0b100000�F[32]�j
				if( (( openMode >> 5 ) & 1) == 1 ) {
					// std::ios::binary���g�p����Ă����ꍇ�A���̕����̃r�b�g�̂ݔ��]������
					uint_t _openMode = openMode;	// �t���O�𔽓]���邽�߂�enum���琔�l�^�֕ύX
					_openMode ^= ( 1 << 5 );		// 5�r�b�g�ڂ𔽓]
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
				// �t�@�C�������݂��Ȃ��ꍇ�����l��Ԃ�
				return std::nullopt;
			}
		}

		// �t�@�C����ǂݍ��ݗp�Ń��[�h����
		inline std::optional<std::fstream> LoadReadFile( std::filesystem::path path, _OpenMode openMode = std::ios::in, _Format format = _ZeroBit ) {
			return Text::LoadFile( path, std::ios::in | openMode, format );
		}

		// �t�@�C�����������ݗp�Ń��[�h����
		inline std::optional<std::fstream> LoadWriteFile( std::filesystem::path path, _OpenMode openMode = std::ios::out, _Format format = _ZeroBit ) {
			return Text::LoadFile( path, std::ios::out | openMode, format );
		}

		//===================================================
		// �t�@�C���̓ǂݍ���
		//===================================================

		// �t�@�C���̓��e��ǂݍ���
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

		// �t�@�C���̓��e�𕶎���Ƃ��ēǂݍ���
		inline std::optional<std::vector<std::string>> ReadString( std::fstream& fs ) {
			if( fs ) {
				//// �t�@�C���̍s�����擾
				//auto columns = FileUtility::Utility::FileColumn( fs );
				//
				//// �s�����̃o�b�t�@���m��
				//std::vector<std::string> lineBuffers( columns );
				//
				//// �s�����̃f�[�^��ǂݍ���
				//for( ulonglong_t i = 0; i < columns; i++ ) std::getline( fs, lineBuffers[i] );
				//
				//// �S�s���̃f�[�^��Ԃ�
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

		// �t�@�C�������[�h���āA�t�@�C���̓��e��ǂݍ���
		inline std::optional<std::vector<char>> LoadAndRead( std::filesystem::path path, _OpenMode openMode = std::ios::in, _Format format = _ZeroBit ) {
			auto fs = Text::LoadReadFile( path, openMode, format );
			return ( fs.has_value() ) ? Read( fs.value() ) : std::nullopt;
		}

		// �t�@�C�������[�h���āA�t�@�C���̓��e�𕶎���Ƃ��ēǂݍ���
		inline std::optional<std::vector<std::string>> LoadAndReadString( std::filesystem::path path, _OpenMode openMode = std::ios::in, _Format format = _ZeroBit ) {
			auto fs = Text::LoadReadFile( path, openMode, format );
			return ( fs.has_value() ) ? ReadString( fs.value() ) : std::nullopt;
		}

		//===================================================
		// �t�@�C���̏�������
		//===================================================

		// �t�@�C���ɓ��e����������
		inline void Write( const std::vector<char>& byteData, std::fstream& fs ) {
			if( fs ) {
				fs.write( byteData.data(), byteData.size() );
			}
		}

		// �t�@�C���ɓ��e����������
		inline void Write( const std::vector<std::string>& lineDatas, std::fstream& fs ) {
			if( fs ) {
				auto size = lineDatas.size();
				for( std::size_t i = 0; i < size; i++ ) fs << lineDatas[i];
			}
		}

		// �t�@�C�������[�h���āA�t�@�C���ɓ��e����������
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

		// �t�@�C�������[�h���āA�t�@�C���ɓ��e����������
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

		// �t�@�C�������[�h���āA�t�@�C���ɓ��e����������
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

	// CSV�t�@�C��
	namespace CSV {

	}

}