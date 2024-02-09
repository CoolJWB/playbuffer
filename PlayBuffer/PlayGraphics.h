#ifndef PLAY_PLAYGRAPHICS_H
#define PLAY_PLAYGRAPHICS_H
//********************************************************************************************************************************
// File:		PlayGraphics.h
// Description:	Manages 2D graphics operations on a PixelData buffer 
// Platform:	Independent
// Notes:		Uses PNG format. The end of the filename indicates the number of frames e.g. "bat_4.png" or "tiles_10x10.png"
//********************************************************************************************************************************
namespace Play::Graphics
{
	enum BlendMode
	{
		BLEND_NORMAL = 0,
		BLEND_ADD,
		BLEND_MULTIPLY,
		BLEND_SUBTRACT
	};

	extern BlendMode blendMode;

	// Create/Destroy manager functions
	//********************************************************************************************************************************

	// Creates the PlayGraphics manager and generates sprites from all the PNGs in the directory indicated
	bool CreateManager( int bufferWidth, int bufferHeight, const char* path );
	// Destroys the PlayGraphics manager
	bool DestroyManager();

	// Basic drawing functions
	//********************************************************************************************************************************

	// Sets the colour of an individual pixel in the display buffer
	void DrawPixel( Point2f pos, Pixel pix );
	// Draws a line of pixels into the display buffer
	void DrawLine( Point2f startPos, Point2f endPos, Pixel pix );
	// Draws a rectangle into the display buffer
	void DrawRect( Point2f bottomLeft, Point2f topRight, Pixel pix, bool fill = false );
	// Draws a circle into the display buffer
	void DrawCircle( Point2f centrePos, int radius, Pixel pix );
	// Draws raw pixel data to the display buffer
	// > Pre-multiplies the alpha on the image data if this hasn't been done before
	void DrawPixelData( PixelData* pixelData, Point2f pos, float alpha = 1.0f );

	// Debug font functions
	//********************************************************************************************************************************

	// Draws a single character using the in-built debug font
	// > Returns the character width in pixels
	int DrawDebugCharacter( Point2f pos, char c, Pixel pix );
	// Draws text using the in-built debug font
	// > Returns the x position at the end of the text
	int DrawDebugString( Point2f pos, const std::string& s, Pixel pix, bool centred = true );

	// Sprite Loading functions
	//********************************************************************************************************************************

	// Loads a sprite sheet and creates a sprite from it (custom asset pipelines)
	// > All sprites are normally automatically created by the PlayGraphics constructor
	int LoadSpriteSheet( const std::string& path, const std::string& filename );
	// Adds a sprite sheet dynamically from memory (custom asset pipelines)
	// > All sprites are normally created by the PlayGraphics constructor
	int AddSprite( const std::string& name, PixelData& pixelData, int hCount = 1, int vCount = 1 );
	// Updates a sprite sheet dynamically from memory (custom asset pipelines)
	// > Left to caller to release old PixelData
	int UpdateSprite( const std::string& name, PixelData& pixelData, int hCount = 1, int vCount = 1 );
	// Regenerates the premultiplied alpha data.
	int UpdateSprite( const std::string& name );
	
	// Loads a background image which is assumed to be the same size as the display buffer
	// > Returns the index of the loaded background
	int LoadBackground( const char* fileAndPath );

	// Sprite Getters and Setters
	//********************************************************************************************************************************

	// Gets the sprite id of the first matching sprite whose filename contains the given text
	// > Returns -1 if not found
	int GetSpriteId( const char* spriteName );
	// Gets the root filename of a specific sprite
	const std::string& GetSpriteName( int spriteId );
	// Gets the size of the sprite with the given id
	Vector2f GetSpriteSize(int spriteId);
	// Gets the number of frames in the sprite with the given id
	int GetSpriteFrames( int spriteId );
	// Gets the origin of the sprite with the given id (offset from bottom left)
	Vector2f GetSpriteOrigin( int spriteId );
	// Sets the origin of the sprite with the given id (offset from bottom left)
	void SetSpriteOrigin( int spriteId, Vector2f newOrigin, bool relative = false );
	// Vertically flips the origin of the sprite with the given id (for legacy sprite origins)
	void FlipSpriteOriginVertically( int spriteId );
	// Vertically flips the origin of all sprites (for legacy sprite origins)
	void FlipAllSpriteOriginsVertically( void );
	// Centres the origin of the sprite with the given id
	void CentreSpriteOrigin( int spriteId );
	// Centres the origins of all the sprites
	void CentreAllSpriteOrigins();
	// Sets the origin of all sprites found matching the given name (offset from bottom left)
	void SetSpriteOrigins( const char* rootName, Vector2f newOrigin, bool relative = false );
	// Gets the number of sprites which have been loaded and created by PlayGraphics
	int GetTotalLoadedSprites();
	// Gets a (read only) pointer to a sprite's canvas buffer data
	const PixelData* GetSpritePixelData( int spriteId ); 

	// Sprite Drawing functions
	//********************************************************************************************************************************

	// Draw the sprite with transparency (slower than without transparency)
	void DrawTransparent(int spriteId, Point2f pos, int frameIndex, BlendColour globalMultiply = { 1.0f, 1.0f, 1.0f, 1.0f } ); 
	// Draw the sprite without rotation or transparency (fastest draw)
	inline void Draw( int spriteId, Point2f pos, int frameIndex ) { DrawTransparent( spriteId, pos, frameIndex ); } // DrawTransparent only ends up performing a global multiply if any of its values are < 1.0f
	// Draw the sprite rotated with transparency (slowest draw)
	void DrawRotated( int spriteId, Point2f pos, int frameIndex, float angle, float scale = 1.0f, BlendColour globalMultiply = { 1.0f, 1.0f, 1.0f, 1.0f } );
	// Draw the sprite using a matrix transformation and transparency (slowest draw)
	void DrawTransformed( int spriteId, const Matrix2D& transform, int frameIndex, BlendColour globalMultiply = { 1.0f, 1.0f, 1.0f, 1.0f } );
	// Draws a previously loaded background image
	void DrawBackground( int backgroundIndex = 0 );
	// Multiplies the sprite image buffer by the colour values
	// > Applies to all subseqent drawing calls for this sprite, but can be reset by calling agin with rgb set to white
	void ColourSprite( int spriteId, int r, int g, int b );

	// Draws a string using a sprite-based font exported from PlayFontTool
	int DrawString( int fontId, Point2f pos, std::string text );
	// Draws a centred string using a sprite-based font exported from PlayFontTool
	int DrawStringCentred( int fontId, Point2f pos, std::string text );
	// Draws an individual text character using a sprite-based font 
	int DrawChar( int fontId, Point2f pos, char c );
	// Draws a rotated text character using a sprite-based font 
	int DrawCharRotated( int fontId, Point2f pos, float angle, float scale, char c );
	// Gets the width of an individual text character from a sprite-based font
	int GetFontCharWidth( int fontId, char c );

	// A pixel-based sprite collision test (slooow!)
	int SpriteCollide( int spriteIdA, int frameIndexA, Matrix2D& transA, int spriteIdB, int frameIndexB, Matrix2D& transB );

	// Internal sprite structure for storing individual sprite data
	struct Sprite
	{
		int id{ -1 }; // Fast way of finding the right sprite
		std::string name; // Slow way of finding the right sprite
		int width{ -1 }, height{ -1 }; // The width and height of a single image in the sprite
		//int canvasWidth{ -1 }, canvasHeight{ -1 }; // The width and height of the entire sprite canvas
		int hCount{ -1 }, vCount{ -1 }, totalCount{ -1 };  // The number of sprite images in the canvas horizontally and vertically
		int originX{ 0 }, originY{ 0 }; // The origin and centre of rotation for the sprite (whole pixels only)
		PixelData canvasBuffer; // The sprite image data
		PixelData preMultAlpha; // The sprite data pre-multiplied with its own alpha
		Sprite() = default;
	};

	// Miscellaneous functions
	//********************************************************************************************************************************

	// Gets a pointer to the drawing buffer's pixel data
	PixelData* GetDrawingBuffer(void);
	// Resets the timing bar data and sets the current timing bar segment to a specific colour
	void TimingBarBegin( Pixel pix );
	// Sets the current timing bar segment to a specific colour
	// > Returns the number of timing segments
	int SetTimingBarColour( Pixel pix );
	// Draws the timing bar for the previous frame at the given position and size
	void DrawTimingBar( Point2f pos, Point2f size );
	// Gets the duration (in milliseconds) of a specific timing segment
	float GetTimingSegmentDuration( int id );
	// Clears the display buffer using the given pixel colour
	inline void ClearBuffer( Pixel colour ) { Render::ClearRenderTarget( colour ); }
	// Sets the render target for drawing operations
	inline PixelData* SetRenderTarget(PixelData* renderTarget) { return Render::SetRenderTarget(renderTarget); }
	// Set the blend mode for all subsequent drawing operations that support different blend modes
	inline void SetBlendMode(BlendMode bMode) { blendMode = bMode; }
};
#endif // PLAY_PLAYGRAPHICS_H