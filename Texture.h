
#ifdef _WIN32

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#elif __APPLE__

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>

#endif

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>

class Texture
{
	private:
		//The actual hardware texture
		SDL_Texture* m_Texture;

		//Image dimensions
		int m_Width;
		int m_Height;

	public:
		Texture(void);
		~Texture(void);

		//Loads image at specified path
		bool loadFromFile( std::string path, SDL_Renderer* renderer );

		 //Creates image from font string
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor, const char font[], int size, SDL_Renderer* renderer, TTF_Font *gFont );

		//Deallocates texture
		void free();

		//Renders texture at given point
		void render( int x, int y, SDL_Renderer* renderer , SDL_Rect* clip = NULL);

		//Gets image dimensions
		int getWidth();
		int getHeight();		
};

#endif
