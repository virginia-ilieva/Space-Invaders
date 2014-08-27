#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
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

