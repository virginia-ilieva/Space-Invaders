#include "Texture.h"

//Initialize
Texture::Texture(void)
{
	m_Texture = NULL;
	m_Width = 0;
	m_Height = 0;
}

//Deallocate
Texture::~Texture(void)
{
	free();
}

bool Texture::loadFromFile( std::string path, SDL_Renderer* renderer )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
		return false;
	}
	//Color key image
	SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

	//Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
	if( newTexture == NULL )
	{
		printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		return false;
	}

	//Get image dimensions
	m_Width = loadedSurface->w;
	m_Height = loadedSurface->h;

	//Get rid of old loaded surface
	SDL_FreeSurface( loadedSurface );

	//Return success
	m_Texture = newTexture;
	return m_Texture != NULL;
}

bool Texture::loadFromRenderedText( std::string textureText, SDL_Color textColor, const char font[], int size, SDL_Renderer* renderer, TTF_Font *gFont )
{
    //Get rid of preexisting texture
    free();
	
	//Open the font
	gFont = TTF_OpenFont( font, size );
	if( gFont == NULL )
	{
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
		return false;
	} 

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
		return false;
	}

	//Create texture from surface pixels
	m_Texture = SDL_CreateTextureFromSurface( renderer, textSurface );
	if( m_Texture == NULL )
	{
		printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		return false;
	}	

	//Get image dimensions
	m_Width = textSurface->w;
	m_Height = textSurface->h;

	//Get rid of old surface
	SDL_FreeSurface( textSurface );
    
    //Return success
    return m_Texture != NULL;
}

//Free texture if it exists
void Texture::free()
{
	if( m_Texture != NULL )
	{
		SDL_DestroyTexture( m_Texture );
		m_Texture = NULL;
		m_Width = 0;
		m_Height = 0;
	}
}

void Texture::render( int x, int y, SDL_Renderer* renderer, SDL_Rect* clip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, m_Width, m_Height };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen	
	SDL_RenderCopy( renderer, m_Texture, clip, &renderQuad );
}

int Texture::getWidth()
{
	return m_Width;
}

int Texture::getHeight()
{
	return m_Height;
}