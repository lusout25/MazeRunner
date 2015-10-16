#include "Texture.h"
#include <string>

namespace GameEngine3D {

	class ImageLoader
	{
	public:
		static Texture loadPNG(std::string filePath);
	};

}