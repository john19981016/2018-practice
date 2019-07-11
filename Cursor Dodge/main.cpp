#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std;

//Decide screen size
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int maxHP = 5;
const int maxSkill = 5;
int countbullets=0;
int countheals=0;
int wontgethurttime=0;
int hp = maxHP;
int stage = 0;
bool protecting=false;
int startprotecting=0;
int skill=maxSkill;
enum stage
{
    MENU = 0,
    DOWNWARD = 1,
};

enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_MOUSE_INSIDE = 4,
    BUTTON_SPRITE_MOUSE_GONE = 5,
    BUTTON_SPRITE_TOTAL = 6
};
//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

void shoot(int startx,int starty,int endx,int endy,int number,int vx,int vy,int timetoshoot);

//Class

int min2(int a, int b);

class object
{
    public:
        object();
        ~object();
        bool loadFromFile(std::string path);
        void render(int x,int y);
        void free();
        void moving(int t);
        int getWidth();
        int getHeight();
        void changev(int x,int y);
        void changepos(int x,int y);
        void reset();
        int getmCurrentSprite();
        int getposx();
        int getposy();
        //Handles mouse event
        void handleEvent( SDL_Event* e );
    protected:
        //The actual hardware texture
        SDL_Texture* mTexture;
        //Image dimensions
        int t0;
        int vx;
        int vy;
        int shiftx;
        int shifty;
        int mWidth;
        int mHeight;
        LButtonSprite mCurrentSprite;
};

class bulletobject : public object
{
public:
    bulletobject();
    void render(int x,int y);
    void handleEvent( SDL_Event* e );
    int exploding;
    int startexploding;
};
class healobject : public object
{
public:
    void render(int x,int y);
    void handleEvent( SDL_Event* e );
};
class explosionobject : public object
{
public:
    explosionobject();
    void render(int x,int y,int startexploding);
    bool loadFromFile(std::string path);

};
class shieldobject : public object
{
public:
    void render(int x,int y);

};
//Texture wrapper class
class LTexture
{
    public:
        //Initializes variables
        LTexture();

        //Deallocates memory
        ~LTexture();

        //Loads image at specified path
        bool loadFromFile( std::string path );

        #ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

        //Deallocates texture
        void free();

        //Set color modulation
        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        //Set blending
        void setBlendMode( SDL_BlendMode blending );

        //Set alpha modulation
        void setAlpha( Uint8 alpha );

        //Renders texture at given point
        void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

        //Gets image dimensions
        int getWidth();
        int getHeight();

    private:
        //The actual hardware texture
        SDL_Texture* mTexture;

        //Image dimensions
        int mWidth;
        int mHeight;
};

//The application time based timer
class LTimer
{
    public:
		//Initializes variables
		LTimer();

		//The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();

		//Gets the timer's time
		Uint32 getTicks();

		//Checks the status of the timer
		bool isStarted();
		bool isPaused();

    private:
		//The clock time when the timer started
		Uint32 mStartTicks;

		//The ticks stored when the timer was paused
		Uint32 mPausedTicks;

		//The timer status
		bool mPaused;
		bool mStarted;
};
//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font *gFont = NULL;

//Rendered texture
LTexture gTextTexture;

//The application timer
LTimer timer;

shieldobject shield;
bulletobject bullet;
bulletobject bulletpos;//record the pos and velocity for each bullet
healobject heal;
healobject healpos;
explosionobject explosion;
vector<bulletobject> bullets;
vector<healobject> heals;
//bullet bbullet;
//Scene textures
LTexture gTimeTextTexture;
LTexture gPausePromptTexture;
LTexture gStartPromptTexture;
const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect bulletflash[ WALKING_ANIMATION_FRAMES ];
const int EXPLODE_ANIMATION_FRAMES = 25;
SDL_Rect explorsionflash[ EXPLODE_ANIMATION_FRAMES ];

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
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
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xFF, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}


	//Return success
	return mTexture != NULL;
}
#endif


void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}


LTimer::LTimer()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

void LTimer::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
    }
}

void LTimer::unpause()
{
    //If the timer is running and paused
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;
    }
}

Uint32 LTimer::getTicks()
{
	//The actual timer time
	Uint32 time = 0;

    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

bool LTimer::isStarted()
{
	//Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused()
{
	//Timer is running and paused
    return mPaused && mStarted;
}


SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

object::object()
{
    //Initialize
    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
    t0 = 0;
    vx = 0;
    vy = 0;
    shiftx = 0;
    shifty = 0;
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

bulletobject::bulletobject()
{
    //Initialize
    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
    t0 = 0;
    vx = 0;
    vy = 0;
    shiftx = 0;
    shifty = 0;
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
    exploding = false;
    startexploding = 0;
}

explosionobject::explosionobject()
{
    //Initialize
    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
    t0 = 0;
    vx = 0;
    vy = 0;
    shiftx = 0;
    shifty = 0;
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

object::~object()
{
    free();
}

bool object::loadFromFile(std::string path)
{
    free();
    SDL_Texture* newTexture=NULL;

    SDL_Surface* loadedSurface=IMG_Load(path.c_str());
    if(loadedSurface==NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xFF, 0xFF, 0xFF ) );
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }//TTF_Quit();

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

bool explosionobject::loadFromFile(std::string path)
{
    free();
    SDL_Texture* newTexture=NULL;

    SDL_Surface* loadedSurface=IMG_Load(path.c_str());
    if(loadedSurface==NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0, 0 ) );
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }//TTF_Quit();

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

void object::free()
{
    //Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        vx = 0;
        vy = 0;
        shiftx = 0;
        shifty = 0;
        mWidth = 0;
        mHeight = 0;
    }
}

void object::render( int x, int y )
{
    //SDL_Rect* currentClip = &bulletflash[ timer.getTicks()%1000 / 250 ];
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x+shiftx, y+shifty, mWidth, mHeight };
    /*if( currentClip != NULL )
    {
        renderQuad.w = currentClip->w;
        renderQuad.h = currentClip->h;
        mWidth = currentClip->w;
        mHeight = currentClip->h;
    }*/
    SDL_RenderCopy( gRenderer, mTexture, NULL, &renderQuad );

}

void shieldobject::render( int x, int y )
{
    //SDL_Rect* currentClip = &bulletflash[ timer.getTicks()%1000 / 250 ];
    //Set rendering space and render to screen
    int a;
    int b;
    SDL_GetMouseState( &a, &b );
    mWidth=200;
    mHeight=200;
    SDL_Rect renderQuad = { a-0.5*mWidth+x+shiftx,b-0.5*mHeight+y+shifty, mWidth, mHeight };
    /*if( currentClip != NULL )
    {
        renderQuad.w = currentClip->w;
        renderQuad.h = currentClip->h;
        mWidth = currentClip->w;
        mHeight = currentClip->h;
    }*/
    SDL_RenderCopy( gRenderer, mTexture, NULL, &renderQuad );

}


void healobject::render( int x, int y )
{
    //SDL_Rect* currentClip = &bulletflash[ timer.getTicks()%1000 / 250 ];
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x+shiftx, y+shifty, mWidth, mHeight };
    /*if( currentClip != NULL )
    {
        renderQuad.w = currentClip->w;
        renderQuad.h = currentClip->h;
        mWidth = currentClip->w;
        mHeight = currentClip->h;
    }*/
    SDL_RenderCopy( gRenderer, mTexture, NULL, &renderQuad );

}

void bulletobject::render( int x, int y )
{
    SDL_Rect* currentClip = &bulletflash[ timer.getTicks()%1000 / 250 ];
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x+shiftx, y+shifty, mWidth, mHeight };
    if( currentClip != NULL )
    {
        renderQuad.w = currentClip->w;
        renderQuad.h = currentClip->h;
        mWidth = currentClip->w;
        mHeight = currentClip->h;
    }
    SDL_RenderCopy( gRenderer, mTexture, currentClip, &renderQuad );
}

void explosionobject::render( int x, int y,int startexploding)
{
    if((timer.getTicks()-startexploding)<1000)
    {
        SDL_Rect* currentClip = &explorsionflash[ (timer.getTicks()-startexploding)%1000 / 40 ];
                //Set rendering space and render to screen
        SDL_Rect renderQuad = { x+shiftx, y+shifty, mWidth, mHeight };
        if( currentClip != NULL )
        {
            renderQuad.w = currentClip->w;
            renderQuad.h = currentClip->h;
            mWidth = currentClip->w;
            mHeight = currentClip->h;
        }
    SDL_RenderCopy( gRenderer, mTexture, currentClip, &renderQuad );
    }
}


void object::changev( int x, int y )
{
    vx = x;
    vy = y;
}

void object::changepos( int x, int y )
{
    shiftx = x;
    shifty = y;
}

void object::reset()
{
    vx = 0;
    vy = 0;
    t0 = 0;
    shiftx = 0;
    shifty = 0;
}

void object::moving( int t )
{
    t0++;
    if( t0 >= t )
    {
        shiftx += vx;
        shifty += vy;
        t0 = 0;
    }

}

void object::handleEvent( SDL_Event* e)
{
    //If mouse event happened
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );
        //Check if mouse is in button
        bool inside = true;
        if(float((x-(shiftx+0.5*bullet.getWidth()))*(x-(shiftx+0.5*bullet.getWidth()))+(y-(shifty+0.5*bullet.getHeight()))*(y-(shifty+0.5*bullet.getHeight())))>=180.0)
        {
            inside = false;
        }
        //Mouse is outside button
        if( !inside )
        {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        //Mouse is inside button
        else
        {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_INSIDE;
            //Set mouse over sprite
            /*switch( e->type )
            {
                case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;

                case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                break;

                case SDL_MOUSEBUTTONUP:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                break;

                default:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_INSIDE;
                break;
            }*/
        }
    }
    else
    {
        mCurrentSprite = BUTTON_SPRITE_MOUSE_GONE;
    }
}

void bulletobject::handleEvent( SDL_Event* e)
{
    //If mouse event happened
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );
        //Check if mouse is in button
        bool inside = true;
        if(float((x-(shiftx+0.5*bullet.getWidth()))*(x-(shiftx+0.5*bullet.getWidth()))+(y-(shifty+0.5*bullet.getHeight()))*(y-(shifty+0.5*bullet.getHeight())))>=180.0)
        {
            inside = false;
        }
        //Mouse is outside button
        if( !inside )
        {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        //Mouse is inside button
        else
        {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_INSIDE;
            //Set mouse over sprite
            /*switch( e->type )
            {
                case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;

                case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                break;

                case SDL_MOUSEBUTTONUP:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                break;

                default:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_INSIDE;
                break;
            }*/
        }
    }
    else
    {
        mCurrentSprite = BUTTON_SPRITE_MOUSE_GONE;
    }
}

void healobject::handleEvent( SDL_Event* e)
{
    //If mouse event happened
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );
        //Check if mouse is in button
        bool inside = true;
        if(float((x-(shiftx+0.5*heal.getWidth()))*(x-(shiftx+0.5*heal.getWidth()))+(y-(shifty+0.5*heal.getHeight()))*(y-(shifty+0.5*heal.getHeight())))>=180.0)
        {
            inside = false;
        }
        //Mouse is outside button
        if( !inside )
        {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        //Mouse is inside button
        else
        {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_INSIDE;
            //Set mouse over sprite
            /*switch( e->type )
            {
                case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;

                case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                break;

                case SDL_MOUSEBUTTONUP:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                break;

                default:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_INSIDE;
                break;
            }*/
        }
    }
    else
    {
        mCurrentSprite = BUTTON_SPRITE_MOUSE_GONE;
    }
}


int object::getWidth()
{
    return mWidth;
}

int object::getHeight()
{
    return mHeight;
}

int object::getmCurrentSprite()
{
    return mCurrentSprite;
}

int object::getposx()
{
    return shiftx;
}

int object::getposy()
{
    return shifty;
}

void shoot(int startx,int starty,int endx,int endy,int number,int vx,int vy,int timetoshoot)
{
    if(timer.getTicks()==unsigned(timetoshoot))
    for(int i=0;i<number;i++)
    {
        countbullets++;
        bullets.push_back(bulletpos);
        bullets[countbullets-1].reset();
        bullets[countbullets-1].changev(vx,vy);
        bullets[countbullets-1].changepos(startx+i*((endx-startx)/number),starty+i*((endy-starty)/number));
    }
}

void randanshoot()
{
    static int t1 = 0;
    int t2 = timer.getTicks()/500;
    int f= 100+t2;
    if(t2 > t1)
    {
        for(int i=0;i<(f/50);i++)
        {
            shoot(-640,min2(rand()%1000,(rand()*rand())%1000),1280,min2((rand()*rand())%1000,rand()%1000),5+rand()%(f/30),2-(f/30)+(rand()*rand()*rand())%(2*f/30),(f/30)+rand()%(f/30),timer.getTicks());
        }
        f+=1;
    }
    t1 = t2;

}

int min2(int a, int b)
{
    if(a>b)
    {
        return -20-(a-b);
    }
    else
    {
        return -20;
    }
}

void newheal(int x,int y,int intervals)
{
    static int t0=0;
    int t1;
    t1=timer.getTicks()/intervals;
    if(t1>t0)
    {
        countheals++;
        heals.push_back(healpos);
        heals[countheals-1].reset();
        heals[countheals-1].changepos(x,y);
    }
    t0=t1;
}

void handleheals(SDL_Event e)
{
    for(int i=0;i<countheals;i++)
    {

        heals[i].handleEvent(&e);
        if(heals[i].getposx()>-20 && heals[i].getposx()<660 && heals[i].getposy()>-20 &&heals[i].getposy()<480)
        {
            heal.render(heals[i].getposx(),heals[i].getposy());
            //heal.render(0,0);
        }
        if(heals[i].getmCurrentSprite()==BUTTON_SPRITE_MOUSE_INSIDE )//get healed
        {

            if(hp<maxHP)hp++;
            heals[i].changepos(-100,550);
        }
        if(heals[i].getposy()>=550)
        {
            heals.erase(heals.begin()+i);
            countheals--;
        }
    }
}

void handlebullet(SDL_Event e)
{

    static int t;

    for(int i=0;i<countbullets;i++)
    {
        if(bullet.getmCurrentSprite()!= BUTTON_SPRITE_MOUSE_GONE)
        {
            bullets[i].moving(5);
        }
        bullets[i].handleEvent(&e);
        if(bullets[i].exploding==true)
        {
            explosion.render(bullets[i].getposx(),bullets[i].getposy(),bullets[i].startexploding);
        }
        else
        {
            if(bullets[i].getposx()>-20 && bullets[i].getposx()<660 && bullets[i].getposy()>-20 &&bullets[i].getposy()<480)
            {
                bullet.render(bullets[i].getposx(),bullets[i].getposy());
            }
        }
        if(bullets[i].getmCurrentSprite()==BUTTON_SPRITE_MOUSE_INSIDE && wontgethurttime==false && protecting==false)//get hurt
        {
            hp--;
            t=timer.getTicks();
            bullets[i].startexploding=timer.getTicks();
            wontgethurttime=true;
            bullets[i].exploding=true;
            //bullets[i].changepos(-100,550);
            bullets[i].changev(0,0);
        }
        if(protecting==true)
        {
            int a;
            int b;
            SDL_GetMouseState( &a, &b );
            if(((a)-bullets[i].getposx())*((a)-bullets[i].getposx())+((b)-bullets[i].getposy())*((b)-bullets[i].getposy())<10000&&bullets[i].exploding==false)
            {
                bullets[i].startexploding=timer.getTicks();
                bullets[i].exploding=true;
                bullets[i].changev(0,0);
            }
        }


        /*if(bullets[i].getposy()>=550)
        {
            bullets.erase(bullets.begin()+i);
            countbullets--;
        }*/
        if(bullets[i].exploding==true)
        {
            if(timer.getTicks()>unsigned(bullets[i].startexploding+1000))
            {
                bullets.erase(bullets.begin()+i);
                countbullets--;
            }
        }

    }
    if(wontgethurttime==true)
    {
        if(timer.getTicks()>unsigned(t+1000))
        {
            wontgethurttime=false;
        }
    }
}

void restart()
{
    hp=maxHP;
    skill=maxSkill;
    protecting=false;
    for(int i=0;i<countbullets;i++)
    {
        bullets[i].changepos(-100,550);
        bullets[i].changev(0,0);
        if(bullets[i].getposy()>=550)
        {
            bullets.erase(bullets.begin()+i);
        }
    }
    countbullets=0;

}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Summerproject2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
                 //Initialize SDL_ttf
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;
    //Load Foo' texture
    if( !heal.loadFromFile( "heal.png" ) )
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    if( !shield.loadFromFile( "shield.png" ) )
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    if( !bullet.loadFromFile( "bullet4.png" ) )
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    else
    {
        //Set sprite clips
        bulletflash[ 0 ].x =   0;
        bulletflash[ 0 ].y =   0;
        bulletflash[ 0 ].w =  35;
        bulletflash[ 0 ].h = 35;

        bulletflash[ 1 ].x =  0;
        bulletflash[ 1 ].y =   35;
        bulletflash[ 1 ].w =  35;
        bulletflash[ 1 ].h = 35;

        bulletflash[ 2 ].x = 0;
        bulletflash[ 2 ].y =   70;
        bulletflash[ 2 ].w =  35;
        bulletflash[ 2 ].h = 35;

        bulletflash[ 3 ].x = 0;
        bulletflash[ 3 ].y =   105;
        bulletflash[ 3 ].w =  35;
        bulletflash[ 3 ].h = 35;
    }

    if( !explosion.loadFromFile( "explosion.png" ) )
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    else
    {
        int x=0;
        int y=0;
        for(int i=0;i<25;i++)//Set sprite clips
        {
            if(y==175) y=0;
            if(x==175)
            {
                x=0;
                y+=35;
            }
            explorsionflash[ i ].x =   x;
            explorsionflash[ i ].y =   y;
            explorsionflash[ i ].w =   35;
            explorsionflash[ i ].h =   35;
            x+=35;
        }

    }
    //Open the font
    TTF_Init();
    gFont = TTF_OpenFont( "lazy.ttf", 28 );
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Render text
        SDL_Color textColor = { 0, 0, 0 };
        if( !gTextTexture.loadFromRenderedText( "Time:", textColor ) )
        {
            printf( "Failed to render text texture!\n" );
            success = false;
        }
    }


	return success;
}

void close()
{

    bullet.free();
    heal.free();
    gTimeTextTexture.free();
    //Free loaded images

    gTextTexture.free();

    //Free global font
    TTF_CloseFont( gFont );
    gFont = NULL;

    //Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}


int main( int argc, char* args[] )
{
    //HWND hWnd= GetForegroundWindow();
    //ShowWindow(hWnd, SW_HIDE);
    //Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{

		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			fstream file;
			char buffer[200];
			file.open ("data.txt", ios::in );
            if(!file)
            {
                cout<<"Fail to open file\n";
            }
            else
            {
                file.read(buffer,sizeof(buffer));
            }

			//Event handler
			SDL_Event e;



			//set randon
			srand(time(NULL));

            timer.start();

            //Set text color as black
            SDL_Color textColor = { 255, 255, 255, 255 };

			//In memory text stream
			std::stringstream timeText;
			std::stringstream bestText;
			std::stringstream hpText;
			std::stringstream skillText;
            //health points


            //While application is running
            while( !quit )
            {
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    else if( e.type == SDL_KEYDOWN )
					{
					    switch(e.key.keysym.sym)
					    {
                        case SDLK_UP:
                            break;
                        case SDLK_DOWN:
                            break;
                        case SDLK_RIGHT:
                            break;
                        case SDLK_LEFT:
                            break;
                        case SDLK_a:
                            break;
					    }
					}
                }

                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0xFF );
                SDL_RenderClear( gRenderer );
                if(stage==MENU)
                {
                    if( !gTextTexture.loadFromRenderedText( "click to start", textColor ) )
                    {
                        printf( "Unable to render time texture!\n" );
                    }
                    gTextTexture.render( 220, -30+( SCREEN_HEIGHT - gTimeTextTexture.getHeight() ) / 2 );
                    if(e.type == SDL_MOUSEBUTTONDOWN)
                    {
                        stage=DOWNWARD;
                    }

                }
                if(stage==DOWNWARD)
                {
                    //downward

                    if( e.type == SDL_KEYDOWN )
                        {
                            switch(e.key.keysym.sym)
                            {
                            case SDLK_s:
                                if(protecting==false&&skill>0)
                                {
                                    skill--;
                                    startprotecting=timer.getTicks();
                                    protecting=true;
                                }
                            break;
                            }
                        }
                    if(protecting==true)
                    {
                        shield.render(0,0);
                        if(timer.getTicks()>=unsigned(startprotecting+5000))
                        {
                            protecting=false;
                        }

                    }
                    bullet.handleEvent(&e);

                    if( bullet.getmCurrentSprite()!= BUTTON_SPRITE_MOUSE_GONE)
                    {
                        randanshoot();
                        srand(timer.getTicks()+time(NULL));
                        //newheal(,100,5000);
                        newheal( rand()%640, (rand()*rand())%480, 10000);
                        if( timer.isPaused() && hp!=0)
                        {
                            timer.unpause();
                        }
                    }
                    else
                    {
                        timer.pause();
                    }
                    if(hp!=0)
                    {
                        handlebullet(e);
                        handleheals(e);
                    }
                    //Render current frame
                    //gTextTexture.render( ( SCREEN_WIDTH - gTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gTextTexture.getHeight() ) / 2 );
                    //Update screen

                    hpText.str( "" );
                    hpText << "Hp : "<<hp;
                    timeText.str( "" );
                    timeText << "Time : " << ((timer.getTicks()-timer.getTicks()%10)/1000.f) ;
                    if(unsigned(atoi(buffer))<=timer.getTicks())
                    {
                        sprintf(buffer,"%d",timer.getTicks());
                    }
                    file.open("data.txt",ios::out);
                    file<<buffer;
                    file.close();
                    bestText.str( "" );
                    bestText << "Best : " << ((atoi(buffer)-atoi(buffer)%10)/1000.f) ;
                    skillText.str( "" );
                    skillText << "skill : " << skill ;
                    if(hp==0)
                    {
                        timer.pause();
                        if( !gTextTexture.loadFromRenderedText( "Press a to restart", textColor ) )
                        {
                            printf( "Unable to render time texture!\n" );
                        }
                        gTextTexture.render( 250, -50+( SCREEN_HEIGHT - gTimeTextTexture.getHeight() ) / 2 );
                        if( e.type == SDL_KEYDOWN )
                        {
                            switch(e.key.keysym.sym)
                            {
                            case SDLK_a:
                                restart();
                                timer.start();
                            break;
                            }
                        }
                    }
                    if( !gTimeTextTexture.loadFromRenderedText( timeText.str().c_str(), textColor ) )
                    {
                        printf( "Unable to render time texture!\n" );
                    }
                    gTimeTextTexture.render( 250, -25+( SCREEN_HEIGHT - gTimeTextTexture.getHeight() ) / 2 );
                    if( !gTimeTextTexture.loadFromRenderedText( bestText.str().c_str(), textColor ) )
                    {
                        printf( "Unable to render time texture!\n" );
                    }
                    gTimeTextTexture.render( 250, 0+( SCREEN_HEIGHT - gTimeTextTexture.getHeight() ) / 2 );
                    if( !gTimeTextTexture.loadFromRenderedText( hpText.str().c_str(), textColor ) )
                    {
                        printf( "Unable to render time texture!\n" );
                    }
                    gTimeTextTexture.render( 250, 25+( SCREEN_HEIGHT - gTimeTextTexture.getHeight() ) / 2 );
                    if( !gTimeTextTexture.loadFromRenderedText( skillText.str().c_str(), textColor ) )
                    {
                        printf( "Unable to render time texture!\n" );
                    }
                    gTimeTextTexture.render( 250, 50+( SCREEN_HEIGHT - gTimeTextTexture.getHeight() ) / 2 );
                }
                //printf("%d\n",bullet.getmCurrentSprite());
                SDL_RenderPresent( gRenderer );


                //SDL_Delay(1);

            }
		}
	}
	//Free resources and close SDL
	close();
	return 0;
}
