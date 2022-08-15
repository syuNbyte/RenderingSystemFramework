
// TNLIB
#include"Debug.hpp"

// MyFile
#include"DXAssimpDataType.h"

std::string DXAssimpDataType::GetTextureTypeName(int_t typeEnum) {
	if (typeEnum < 0 || typeEnum >= DXAssimpDataType::Texture::Unknown) {
		Debug::Log(
			"DXAssimpDataType::GetTextureTypeName Method",
			"DXAssimpDataType::Texture::Type out of range."
		);
		return "out of range";
	}
	else {
		switch (typeEnum) {
			case Texture::Type::None: return "TextureType[None]"; break;
			case Texture::Type::Diffuse: return "TextureType[Diffuse]"; break;
			case Texture::Type::Specular: return "TextureType[Specular]"; break;
			case Texture::Type::Ambient: return "TextureType[Ambient]"; break;
			case Texture::Type::Emissive: return "TextureType[Emissive]"; break;
			case Texture::Type::Height: return "TextureType[Height]"; break;
			case Texture::Type::Normal: return "TextureType[Normal]"; break;
			case Texture::Type::Shininess: return "TextureType[Shininess]"; break;
			case Texture::Type::Opacity: return "TextureType[Opacity]"; break;
			case Texture::Type::Displacement: return "TextureType[Displacement]"; break;
			case Texture::Type::Light: return "TextureType[Light]"; break;
			case Texture::Type::Reflection: return "TextureType[Reflection]"; break;
			case Texture::Type::Unknown: return "TextureType[Unknown]"; break;
			default:return "out of range"; break;
		}
	}
}