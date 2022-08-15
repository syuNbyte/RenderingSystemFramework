// Assimp/material.h�ɒ�`����Ă��邱��

// STL
#include<string>

// TNLIB
#include"SafetyDataType.h"

class DXAssimpDataType {
public:

	// �V�F�[�f�B���O����@
	enum ShadingMode {
		Flat,			// �t���b�g
		Gouraud,		// �O�[���[
		Phong,			// �t�H��
		Blinn,			// �u�����t�H��
		Toon,			// �g�D�[��
		OrenNayar,		// �I�[�����l�C���[
		Minnaert,		// �����o�[�g�V�F�[�f�B���O���g�p���������ňÂ����l�������V�F�[�f�B���O
		CookTorrance,	// �N�b�N�g�����X
		NoShading,		// �V�F�[�f�B���O���Ȃ�
		Fresnel			// �t���l��
	};

	// �}�e���A���̃^�C�v
	enum MaterialType {
		Diffuse,
		Specular,
		Ambient,
		Emissive
	};

	// �e�N�X�`���Ɋւ����`
	class Texture {
	public:
		// �e�N�X�`���̃^�C�v
		// �Q�ƌ��FaiTextureType
		enum Type {
			None,			// �e�N�X�`���͑��݂��Ȃ�
			Diffuse,		// �f�B�t���[�Y
			Specular,		// �X�y�L����
			Ambient,		// �A���r�G���g
			Emissive,		// �G�~�b�V�u
			Height,			// �����}�b�v
			Normal,			// �@���}�b�v
			Shininess,		// �P�x�}�b�v
			Opacity,		// �����x�}�b�v
			Displacement,	// ���ʃ}�b�v
			Light,			// ���C�g�}�b�v
			Reflection,		// ���˃}�b�v
			Unknown			// ���ʂł��Ȃ��e�N�X�`���E�e�N�X�`���^�C�v�̐�
		};

		// �e�N�X�`���̎g�p���@
		// �Q�ƌ��FaiTextureFlags
		enum Flag {
			Invert,			// �e�N�X�`���̐��l��1����������l���g�p����
			UseAlpha,		// �A���t�@�l���g�p����
			IgnoreAlpha,	// �A���t�@�l���g�p���Ȃ�
		};

		// �`�掞�̃u�����f�B���O���@
		// T1�����̐F	�iSource)
		// T2�����g�̐F	�iDestination�j
		// �Q�ƌ��FaiTextureOp
		enum Operation {
			Mul,			// ��Z�����u�����h						// T = T1 * T2
			Add,			// ���Z�����u�����h						// T = T1 + T2
			Sub,			// ���Z�����u�����h						// T = T1 - T2
			Div,			// ���Z�����u�����h						// T = T1 / T2
			SmoothAdd,		// ���Z�����u�����h - ��Z�����u�����h	// T = (T1 + T2) - (T1 * T2)
			SignedAdd		// �悭�킩���							// T = T1 + (T2 - 0.5)
		};

		// �}�b�s���O����UV���W�̈�����
		// [0.0�`1.0]�͈̔͊O��UV���W���ǂ̂悤�Ɉ��������`���܂��B
		// �Q�ƌ��FaiTextureMapMode
		enum MapMode {
			Wrap,			// �e�N�X�`�����Wu|v�́Au%1|v%1�ɕϊ�����܂��B
			Clamp,			// [0.0�`1.0]�ȊO�̃e�N�X�`�����W�͂̊O���̃e�N�X�`�����W�́A�ł��߂��L���Ȓl�ɃN�����v����܂��B
			Decal,			// ����s�N�Z���̃e�N�X�`�����W��[0...1]�̊O���ɂ���ꍇ�A���̃s�N�Z���ɂ̓e�N�X�`���͓K�p����܂���B
			Mirror			// �e�N�X�`�����W u|v �́A(u-(u%1.0))%2.0 �� 0.0 �Ȃ�� u%1.0|v%1.0 �ƂȂ�A����ȊO�Ȃ� 1.0-(u%1.0)|1.0-(v%1.0) �ƂȂ�܂��B
		};

		// �e�N�X�`���}�b�s���O���s���Ă��Ȃ��ꍇ��
		// UV���W�𐶐�����֐��iaiProcess_GenUVCoords�֐��j�Ŏg�p����}�b�s���O���@
		// �Q�ƌ��FaiTextureMapping
		enum Mapping {
			Uv,				// 
			Sphere,			// ��
			Cylinder,		// �~��
			Box,			// ������
			Plane,			// ����
			Other			// ����`
		};
	};

public:
	static std::string GetTextureTypeName(int_t typeEnum);
};

#define DXAssimpMaxTextureType = DXAssimpDataType::TextureType::Unknown;