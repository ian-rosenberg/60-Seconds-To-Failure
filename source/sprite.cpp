#include "sprite.h"

static Uint32* SDL_GetPixel32(Uint32* pixels, int x, int y, int w)
{
	return &pixels[y * w + x];
}

static int SDL_CalculatePitch(Uint32 format, int width)
{
	int pitch;

	if (SDL_ISPIXELFORMAT_FOURCC(format) || SDL_BITSPERPIXEL(format) >= 8) {
		pitch = (width * SDL_BYTESPERPIXEL(format));
	}
	else {
		pitch = ((width * SDL_BITSPERPIXEL(format)) + 7) / 8;
	}
	pitch = (pitch + 3) & ~3;   /* 4-byte aligning for speed */
	return pitch;
}

Sprite::Sprite()
{
	texture = nullptr;
	frame = 0;
	yOffset = 0;
	frameWidth = 0;
	frameHeight = 0;
	flip = { 0,0 };
	scale = { 0,0 };
	scaleCenter = { 0,0 };
	rotation = { 0,0,0 };
	color = { 0,0,0,255 };
	graphics.reset();
	filepath = "";
	srcRect = { 0,0,0,0 };
	surf = nullptr;
}

Sprite::Sprite(const Sprite &oldSprite)
{
	filepath = std::string(oldSprite.filepath);
	frame = oldSprite.frame;
	yOffset = oldSprite.yOffset;
	frameWidth = oldSprite.frameWidth;
	frameHeight = oldSprite.frameHeight;
	flip = oldSprite.flip;
	scale = oldSprite.scale;
	scaleCenter = oldSprite.scaleCenter;
	rotation = oldSprite.rotation;
	color = oldSprite.color;
	graphics = oldSprite.graphics;
	srcRect = oldSprite.srcRect;
	texture = oldSprite.texture;
	if (oldSprite.surf)
		surf = oldSprite.surf;
	else
		surf = nullptr;
}

Sprite::Sprite(std::string fp, Vector2 drawPosition, Vector2 scle, Vector2 scleCen, Vector3 rot, Vector2 flp, Vector4 colorShift, int frm, int off, int width, int height, std::shared_ptr<Graphics> ren)
{
	filepath = fp;
	frame = frm;
	yOffset = off;
	frameWidth = width;
	frameHeight = height;
	flip = flp;
	scale = scle;
	scaleCenter = scleCen;
	rotation = rot;
	color = colorShift;
	graphics = ren;
	srcRect = { 0, frameHeight * off, frameWidth, frameHeight};
	surf = nullptr;
	LoadPNGImage(filepath);
}

Sprite::Sprite(std::shared_ptr<SDL_Texture> tex, SDL_Rect* sourceRect, Vector2 drawPosition, Vector3 rotation, Vector2 flip, Vector4 colorShift, std::shared_ptr<Graphics> ren, std::string fp)
{
	filepath = fp;
	texture = tex;
	srcRect = *sourceRect;
	frameWidth = sourceRect->w;
	frameHeight = sourceRect->h;
	flip = { 0,0 };
	scale = { 1.f,1.f };
	scaleCenter = {frameWidth/2.f, frameHeight/2.f};
	rotation = { 0.f, 0.f, 0.f };
	color = colorShift;
	graphics = ren;
}

Sprite::Sprite(std::string fp, int width, int height, const std::shared_ptr<Graphics>& graphics)
{
	filepath = fp;
	frame = 1.0f;
	yOffset = 0;
	frameWidth = width;
	frameHeight = height;
	flip = {};
	scale = {};
	scaleCenter = {};
	rotation = {};
	color = {};
	this->graphics = graphics;
	srcRect = { 0, 0, width, height };
	LoadPNGImage(filepath);
}

std::shared_ptr<SDL_Texture> Sprite::CreateRenderTexture(int width, int height, const std::shared_ptr<Graphics>& graphics, Uint32 fmt)
{
	SDL_Rect srcRect = { 0,0,width,height };

	std::shared_ptr<SDL_Texture> texture = std::shared_ptr<SDL_Texture>(SDL_CreateTexture(graphics->GetRenderer(), fmt, SDL_TEXTUREACCESS_TARGET, width, height),
		[](SDL_Texture* ptr) { SDL_DestroyTexture(ptr); });

	SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_BLEND);

	if (!texture.get())
	{
		printf_s("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());

		return NULL;
	}

	return texture;
}

std::vector<std::vector<SDL_Color>> Sprite::GetPixelData(const char* filepath, SDL_Rect* r, const std::shared_ptr<Graphics>& graphics)
{
	std::vector<std::vector<SDL_Color>> pixels;
	SDL_Surface* surf = IMG_Load(filepath);
	int x, y;
	SDL_PixelFormat* fmt;
	std::vector<std::vector<SDL_Color>> pixelResults;
	SDL_Color pixel;
	std::vector<SDL_Color> resultLine;
	int bpp;

	SDL_LockSurface(surf);

	fmt = surf->format;
	bpp = surf->format->BytesPerPixel;
	//cols then rows
	for (y = 0; y < surf->h; y++)
	{
		resultLine.clear();
		for (x = 0; x < surf->w; x++)
		{
			Uint32 p = GetPixel(surf, x, y);

			SDL_GetRGBA(p,
				fmt,
				&pixel.r,
				&pixel.g,
				&pixel.b,
				&pixel.a);
			resultLine.push_back(pixel);
		}
		pixelResults.push_back(resultLine);
	}

	SDL_UnlockSurface(surf);

	return pixelResults;

	return pixels;
}

Sprite::~Sprite()
{
	texture.reset();
	surf.reset();

	graphics.reset();
}

Sprite::Sprite(std::string fp, int imgWidth, int imgHeight, int width, int height, int yOff, int xOff, std::shared_ptr<Graphics> ren)
{
	frame = 1.0f;
	filepath = fp;
	yOffset = yOff;
	xOffset = xOff;
	frameWidth = width;
	frameHeight = height;
	flip = {};
	scale = {};
	scaleCenter = {};
	rotation = {};
	color = {SDL_MAX_UINT8,SDL_MAX_UINT8,SDL_MAX_UINT8,SDL_MAX_UINT8 };
	graphics = ren;
	srcRect = { xOffset * frameWidth, yOffset * frameHeight, frameWidth, frameHeight };
	LoadPNGImage(filepath);
}

Uint8 Sprite::LoadPNGImage(std::string filepath)
{
	surf = std::shared_ptr<SDL_Surface>(IMG_Load(filepath.c_str()), [](SDL_Surface* ptr) { SDL_FreeSurface(ptr); });

	texture = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(graphics->GetRenderer(), surf.get()), 
		[](SDL_Texture* ptr) { SDL_DestroyTexture(ptr); });

	if (!texture) {
		std::cout << "Image " << filepath << " could not be loaded!" << std::endl;
		printf("IMG_Load: %s\n", IMG_GetError());
		return 0;
	}

	std::cout << "Image " << filepath << " was loaded!" << std::endl;

	return 1;
}

std::shared_ptr<SDL_Surface> Sprite::LoadSurface(std::string filepath)
{
	return std::shared_ptr<SDL_Surface>(IMG_Load(filepath.c_str()));
}

std::shared_ptr<SDL_Texture> Sprite::GetTexture()
{
	return texture;
}

void Sprite::RotateTextureZ(float theta)
{
	SDL_Renderer* ren = graphics->GetRenderer();
	SDL_Point ctr = SDL_Point(frameWidth / 2.f, frameHeight / 2.f);

	SDL_SetRenderTarget(ren, texture.get());
	SDL_RenderCopyEx(ren, texture.get(), &srcRect, nullptr, theta, &ctr, SDL_FLIP_NONE);
	SDL_SetRenderTarget(ren, nullptr);
}

Sprite* Sprite::MakeFlippedTexture(SDL_RendererFlip flip)
{
	Sprite* flippedCopy = new Sprite(*this);
	SDL_Renderer* ren = graphics->GetRenderer();
	Uint32 fmt;
	SDL_Point ctr = SDL_Point(frameWidth / 2.f, frameHeight / 2.f);

	SDL_QueryTexture(texture.get(), &fmt, nullptr, nullptr, nullptr);

	flippedCopy->texture = std::shared_ptr<SDL_Texture>(SDL_CreateTexture(ren, fmt, SDL_TEXTUREACCESS_TARGET, frameWidth, frameHeight), 
		[](SDL_Texture* ptr) { SDL_DestroyTexture(ptr); });

	SDL_SetRenderTarget(ren, flippedCopy->texture.get());
	SDL_RenderCopyEx(ren, flippedCopy->texture.get(), &srcRect, nullptr, 0.f, &ctr, flip);
	SDL_SetRenderTarget(ren, nullptr);

	return flippedCopy;
}

bool Sprite::CheckIfViableTexture(SDL_Rect sR)
{
	Uint32  pix;


	SDL_LockSurface(surf.get());
	
	for (int y = sR.y, x = sR.x; y < sR.h+sR.y && x < sR.w+sR.x; x++, y++) {
		SDL_Color p(0);

		pix = GetPixel(surf.get(), x, y);

		SDL_GetRGBA(pix, 
			surf.get()->format,
			&p.r,
			&p.g,
			&p.b,
			&p.a);

		if (p.a > 0) {
			SDL_UnlockSurface(surf.get());

			return true;
		}
	}

	for (int y = sR.y, x = sR.x + sR.w - 1; y < sR.h+sR.y && x >= sR.x; x--, y++) {	
		SDL_Color p(0);

		pix = GetPixel(surf.get(), x, y);

		SDL_GetRGBA(pix,
			surf.get()->format,
			&p.r,
			&p.g,
			&p.b,
			&p.a); 


		if (p.a > 0) {
			SDL_UnlockSurface(surf.get());
		
			return true;
		}
	}

	SDL_UnlockSurface(surf.get());

	return false;
}

SDL_Color Sprite::translate_color(Uint32 int_color)
//Change from an "int color" to an SDL_Color
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN        
	SDL_Color color = { (int_color & 0x00ff0000) / 0x10000,(int_color & 0x0000ff00) / 0x100,(int_color & 0x000000ff),0 };
#else        
	SDL_Color color = { (int_color & 0x000000ff),(int_color & 0x0000ff00) / 0x100,(int_color & 0x00ff0000) / 0x10000,0 };
#endif    
	return color;
}

//from SDL docs
Uint32 Sprite::GetPixel(SDL_Surface* surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		return *p;
		break;

	case 2:
		return *(Uint16*)p;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;

	case 4:
		return *(Uint32*)p;
		break;

	default:
		return 0;       /* shouldn't happen, but avoids warnings */
	}
}

void Sprite::SetSourceRectLocation(Vector2 pos)
{
	srcRect.x = pos.x;
	srcRect.y = pos.y;
}

void Sprite::Draw(Sprite* sprite,
	Vector2 drawPosition,
	Vector2* scale,
	Vector2* scaleCenter,
	Vector3* rotation,
	Vector2 flip,
	Vector4* colorShift,
	int frame,
	int offset,
	int frameWidth,
	int frameHeight)
{
	SDL_Rect cell, target;
	SDL_RendererFlip flipFlags = SDL_FLIP_NONE;
	Vector3 r = {};
	SDL_Point c;
	Vector2 scaleFactor = { 1, 1 };
	Vector2 scaleOffset = { 0, 0 };

	if (!sprite)
	{
		return;
	}

	if (scale != nullptr)
	{
		vector2_copy(scaleFactor, (*scale));
	}

	if (scaleCenter != nullptr)
	{
		vector2_copy(scaleOffset, (*scaleCenter));
	}
	else
	{
		vector2_copy(scaleOffset, (vector2(0.5f, 0.5f)));
	}
	if (rotation != nullptr)
	{
		vector2_copy(c, (*rotation));
		c.x *= scaleFactor.x;
		c.y *= scaleFactor.y;
	}
	else
	{
		vector2_copy(r, vector2(0, 1));
	}

	if (flip.x || flip.y)
	{
		if (flip.x)flipFlags = (SDL_RendererFlip)(flipFlags | SDL_FLIP_HORIZONTAL);
		if (flip.y)flipFlags = (SDL_RendererFlip)(flipFlags | SDL_FLIP_VERTICAL);
	}
	else
	{
		flipFlags = SDL_FLIP_NONE;
	}

	if (colorShift)
	{
		SDL_SetTextureColorMod(
			sprite->GetTexture().get(),
			colorShift->x,
			colorShift->y,
			colorShift->z);
		SDL_SetTextureAlphaMod(
			sprite->GetTexture().get(),
			colorShift->w);
	}

	gf2d_rect_set(
		cell,
		0,
		(frameHeight * (int)frame),
		frameWidth,
		frameHeight);
	gf2d_rect_set(
		target,
		drawPosition.x - (scaleFactor.x * scaleOffset.x),
		drawPosition.y - (scaleFactor.y * scaleOffset.y),
		frameWidth * scaleFactor.x,
		frameHeight * scaleFactor.y);

	SDL_RenderCopyEx(graphics->GetRenderer(),
		sprite->GetTexture().get(),
		&cell,
		&target,
		rotation->z,
		nullptr,
		flipFlags);
	if (colorShift)
	{
		SDL_SetTextureColorMod(
			sprite->GetTexture().get(),
			255,
			255,
			255);
		SDL_SetTextureAlphaMod(
			sprite->GetTexture().get(),
			255);
	}
}

void Sprite::Draw(Vector2 drawPosition, SDL_Rect srcRect, Vector2* scale, Vector2* scaleCenter, Vector4* colorShift, SDL_RendererFlip flipFlags)
{
	SDL_Rect targetRect = {
		drawPosition.x,
		drawPosition.y,
		srcRect.w,
		srcRect.h
	};

	SDL_RenderCopyEx(graphics->GetRenderer(),
		texture.get(),
		&srcRect,
		&targetRect,
		0.f,
		nullptr,
		flipFlags);
}

void Sprite::DrawSpriteImage(Sprite* image, Vector2 position, int width, int height)
{
	Vector2 scale = { 1,1 };
	Vector2 scaleCenter = { width / 2.0f, height / 2.0f };
	Vector3 rotation = { 0,0,0 };
	Vector4 colorShift = Vector4(SDL_MAX_UINT8, SDL_MAX_UINT8, SDL_MAX_UINT8, SDL_MAX_UINT8);
	Draw(image,
		position,
		&scale,
		&scaleCenter,
		&rotation,
		{ 0,0 },
		&colorShift,
		0,
		0,
		width,
		height);
}