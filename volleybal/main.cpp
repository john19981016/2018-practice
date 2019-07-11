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
const int SCREEN_WIDTH = 1.5*640;
const int SCREEN_HEIGHT = 1.5*480;
const int dt=1000;
enum
{
    NOTHING = 0,
    RECEIVE = 1,
    SERVE = 2,
    SET = 3,
    PRE_SET = 4,
    SPIKE = 5,
    PRE_SPIKE = 6,
    BLOCK = 7,
    PRE_BLOCK = 8,
    SERVE_RECEIVE = 9,
    NOTHING2 = 10,
    ASPIKE = 11
};
enum
{
    OUTSIDESPIKER = 0,
    SETTER = 1,
    LIBERO = 2,
    MIDDLEBLOCKER = 3,
    OPPOSITEHITTER = 4
};
enum
{
    FOUR = 0,
    BACK = 1,
    TWO = 2,
    AQUICK = 3,
    RANDOM = 4
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();


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


class object
{
    public:
        object();
        ~object();
        bool loadFromFile(std::string path);
        void render();
        void free();
        void moving();
        int getWidth();
        int getHeight();
        void changev(float x,float y,float z);
        void changepos(float x,float y,float z);
        void changesize(int w);
        void changestate(int s);
        void changerotation(int r);
        void settar(float x,float y,float z);
        void ptf(float a,float b,float c);
        void changeposition(int p);
        float getvx();
        float getvy();
        float getvz();
        float getposx();
        float getposy();
        float getposz();
        float gettarx();
        float gettary();
        float gettarz();
        int getstate();
        int getrotation();
        int getposition();
    protected:
        //The actual hardware texture
        SDL_Texture* mTexture;
        //Image dimensions
        float X;
        float Y;
        float vx;
        float vy;
        float vz;
        float tarx;
        float tary;
        float tarz;
        float posx;
        float posy;
        float posz;
        int rotation;
        int state;
        int mWidth;
        int mHeight;
        int position;
};
void AIstate(object *team[],int Dt,int l,int *ourtc,int* opptc,int sta);
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

object testobj;
object net;
object ball;
object player1;
object player2;
object player3;
object player4;
object player5;
object player6;
object player7;
object player8;
object player9;
object player10;
object player11;
object player12;



LTexture background;
LTexture background2;




object::object()
{
    //Initialize
    X = 0;
    Y = 0;
    vx = 0;
    vy = 0;
    vz = 0;
    tarx = 0;
    tary = 0;
    tarz = 0;
    posx = 0;
    posy = 0;
    posz = 0;
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
    state = 0;
    rotation = 0;
    position = OUTSIDESPIKER;
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
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 150,150,150));//0xFF, 0xFF, 0xFF ) );
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
        X = 0;
        Y = 0;
        vx = 0;
        vy = 0;
        vz = 0;
        posx = 0;
        posy = 0;
        posz = 0;
        mWidth = 0;
        mHeight = 0;
    }
}
void object::ptf(float a,float b,float c)
{
    X = 1.5*(a/b*(160.0)/0.236842+320);
    Y = 1.5*(c/b*(-120.0)/0.189474+106.666885);
}
void object::render()
{
    ptf(posx,posy,posz);
    int k=19;
    SDL_Rect renderQuad = {X-0.5*k*mWidth/posy,Y-k*mHeight/posy, k*mWidth/posy, k*mHeight/posy };

    SDL_RenderCopy( gRenderer, mTexture, NULL, &renderQuad );

}
void object::changesize(int w)
{
    mHeight = int(mHeight*1.0*w/mWidth);
    mWidth = w;

}
void object::changev(float x,float y,float z)
{
    vx = x;
    vy = y;
    vz = z;
}
void object::changepos( float x, float y,float z )
{
    posx = x;
    posy = y;
    posz = z;
}
void object::changerotation(int r)
{
    rotation = r;
}
float object::getposx()
{
    return posx;
}
float object::getposy()
{
    return posy;
}
float object::getposz()
{
    return posz;
}
float object::getvx()
{
    return vx;
}
float object::getvy()
{
    return vy;
}
float object::getvz()
{
    return vz;
}
float object::gettarx()
{
    return tarx;
}
float object::gettary()
{
    return tary;
}

float object::gettarz()
{
    return tarz;
}

void object::settar(float x,float y,float z)
{
    tarx = x;
    tary = y;
    tarz = z;
}
void object::changestate(int s)
{
    state = s;
}
void object::changeposition(int p)
{
    position = p;
}
int object::getstate()
{
    return state;
}
int object::getrotation()
{
    return rotation;
}
int object::getposition()
{
    return position;
}
//Globally used font
TTF_Font *gFont = NULL;

//Rendered texture
LTexture gTextTexture;

//The application timer
LTimer timer;



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
		gWindow = SDL_CreateWindow( "Volleyball", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
    if( !ball.loadFromFile( "ball.png" ))
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    if( !player1.loadFromFile( "player.png" ))
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    if( !player2.loadFromFile( "player.png" ))
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    if( !player3.loadFromFile( "player.png" ))
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    if( !player4.loadFromFile( "player.png" ))
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    if( !player5.loadFromFile( "player.png" ))
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    if( !player6.loadFromFile( "player.png" ))
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    if( !player7.loadFromFile( "player2.png" ))
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    if( !player8.loadFromFile( "player2.png" ))
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    if( !player9.loadFromFile( "player2.png" ))
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    if( !player10.loadFromFile( "player2.png" ))
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    if( !player11.loadFromFile( "player2.png" ))
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    if( !player12.loadFromFile( "player2.png" ))
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    if( !net.loadFromFile( "net.png" ))
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    if( !background.loadFromFile( "background.png" ) )
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    if( !background2.loadFromFile( "background6.png" ) )
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
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
/*
float X=0;
float Y=0;
void ptf(float a,float b,float c){
    X=1.5*(a/b*(160.0)/0.236842+320);
    Y=1.5*(c/b*(-120.0)/0.189474+106.666885);
}
void pp(float a,float b,float c){
    ptf(a,b,c);
    SDL_RenderDrawPoint( gRenderer, int(X),int(Y));
}*/

const int len=14;
object *obj[len]={&net,&ball,&player1,&player2,&player3,&player4,&player5,&player6,&player7,&player8,&player9,&player10,&player11,&player12};

int index[len];

void LS(object *obj[])
{

    float lts[len];
    float Y[len];
    for(int i=0;i<len;i++)
    {
        lts[i]=0;
        Y[i]= (obj[i]->getposy());
    }
    float max_y=-1;
    int ind=-1;
    for(int i=0;i<len;i++)
    {
        for(int j=0;j<len;j++)
        {
            if(i==0||j==0)
            {
                if(Y[j]>max_y)
                {
                    max_y=Y[j];
                    ind=j;
                }
            }
            else
            {
                if(Y[j]>max_y && max_y<=lts[i-1])
                {
                    max_y=Y[j];
                    ind=j;
                }
            }
        }
        lts[i]=max_y;
        Y[ind]=-1;
        index[i]=ind;
        max_y=-1;
        ind=-1;
    }
}
const int l_team1 = 6;
const int l_team2 = 6;
object *team1[l_team1]={&player1,&player2,&player3,&player4,&player5,&player6};
object *team2[l_team2]={&player7,&player8,&player9,&player10,&player11,&player12};

float abs(float a)
{
    if(a<0)
    {
        return -a;
    }
    else
    {
        return a;
    }
}

void playball(object *team[],int l,int Dt,int *teamcount,int *team1count,int *team2count,int sta)
{
    for(int i=0;i<l;i++)
    {
        if((team[i]->getposx()-ball.getposx())*(team[i]->getposx()-ball.getposx())+(team[i]->getposy()-ball.getposy())*(team[i]->getposy()-ball.getposy())/*+(team[i]->getposz()-ball.getposz())*(team[i]->getposz()-ball.getposz())*/ <= 0.7)
        {
            if(team[i]->getstate()==SERVE)
            {
                if(ball.getvz()<0 && ball.getposz()<=team[i]->getposz()+1.8+0.2 && ball.getposz()>=team[i]->getposz()+1.8 )
                {
                    float h=3;
                    ball.changev((team[i]->gettarx()-ball.getposx())*pow((9.8/2),0.5)/(pow(h+ball.getposz()-team[i]->gettarz(),0.5)+pow(h,0.5)),(team[i]->gettary()-ball.getposy())*pow((9.8/2),0.5)/(pow(h+ball.getposz()-team[i]->gettarz(),0.5)+pow(h,0.5)),pow(2*h*9.8,0.5));
                    ball.settar(team[i]->gettarx(),team[i]->gettary(),team[i]->gettarz());
                    printf("  SERVE:%f,%f,%f\n",team[i]->gettarx(),team[i]->gettary(),team[i]->gettarz());
                    AIstate(team1,Dt,l_team1,team1count,team2count,sta);
                    AIstate(team2,Dt,l_team2,team2count,team1count,sta);
                    //printf("%d,%d\n",*team1count,*team2count);
                }
            }
             else if(team[i]->getstate()==SPIKE)
             {
                if(ball.getposz()<=team[i]->getposz()+1.8+0.7 && ball.getposz()>=team[i]->getposz()+1.5 )
                {
                    float h=0;
                    ball.changev((team[i]->gettarx()-ball.getposx())*pow((9.8/2),0.5)/(pow(h+ball.getposz()-team[i]->gettarz(),0.5)+pow(h,0.5)),(team[i]->gettary()-ball.getposy())*pow((9.8/2),0.5)/(pow(h+ball.getposz()-team[i]->gettarz(),0.5)+pow(h,0.5)),pow(2*h*9.8,0.5));
                    ball.settar(team[i]->gettarx(),team[i]->gettary(),team[i]->gettarz());
                    printf("  SPIKE:%f,%f,%f\n",team[i]->gettarx(),team[i]->gettary(),team[i]->gettarz());
                    *teamcount+=1;
                    AIstate(team1,Dt,l_team1,team1count,team2count,sta);
                    AIstate(team2,Dt,l_team2,team2count,team1count,sta);
                    //printf("%d,%d\n",*team1count,*team2count);
                }
             }
            else if(team[i]->getstate()==RECEIVE)
            {
                if(ball.getposz()<=team[i]->getposz()+1.8+0.5 && ball.getposz()>=team[i]->getposz()+0.3 )
                {
                    float h=5;
                    ball.changev((team[i]->gettarx()-ball.getposx())*pow((9.8/2),0.5)/(pow(h+ball.getposz()-team[i]->gettarz(),0.5)+pow(h,0.5)),(team[i]->gettary()-ball.getposy())*pow((9.8/2),0.5)/(pow(h+ball.getposz()-team[i]->gettarz(),0.5)+pow(h,0.5)),pow(2*h*9.8,0.5));
                    ball.settar(team[i]->gettarx(),team[i]->gettary(),team[i]->gettarz());
                    printf("  RECEIVE:%f,%f,%f\n",team[i]->gettarx(),team[i]->gettary(),team[i]->gettarz());
                    *teamcount+=1;
                    AIstate(team1,Dt,l_team1,team1count,team2count,sta);
                    AIstate(team2,Dt,l_team2,team2count,team1count,sta);
                    //printf("%d,%d\n",*team1count,*team2count);
                }
            }
            else if(team[i]->getstate()==SET)
            {
                if(ball.getposz()<=team[i]->getposz()+1.8+0.5 && ball.getposz()>=team[i]->getposz()+0.3 )
                {
                    float h=0;
                    if(sta==AQUICK) h=0.8;
                    if(sta==FOUR)   h=2.5;
                    if(sta==BACK)   h=2.5;
                    if(sta==TWO) h=2;
                    ball.changev((team[i]->gettarx()-ball.getposx())*pow((9.8/2),0.5)/(pow(h+ball.getposz()-team[i]->gettarz(),0.5)+pow(h,0.5)),(team[i]->gettary()-ball.getposy())*pow((9.8/2),0.5)/(pow(h+ball.getposz()-team[i]->gettarz(),0.5)+pow(h,0.5)),pow(2*h*9.8,0.5));
                    ball.settar(team[i]->gettarx(),team[i]->gettary(),team[i]->gettarz());
                    printf("  SET:%f,%f,%f\n",team[i]->gettarx(),team[i]->gettary(),team[i]->gettarz());
                    *teamcount+=1;
                    AIstate(team1,Dt,l_team1,team1count,team2count,sta);
                    AIstate(team2,Dt,l_team2,team2count,team1count,sta);
                    //printf("%d,%d\n",*team1count,*team2count);
                }
            }
        }
        if(pow(team[i]->getposx()-ball.getposx(),2)<pow(0.3,2) && pow(team[i]->getposy()-ball.getposy(),2)<pow(0.1,2))
        {
            if(team[i]->getstate()==BLOCK)
            {
                if(ball.getposz()<=team[i]->getposz()+1.8+0.7 && ball.getposz()>=team[i]->getposz()+1.8 )
                {
                    float h=0;
                    ball.changev((team[i]->gettarx()-ball.getposx())*pow((9.8/2),0.5)/(pow(h+ball.getposz()-team[i]->gettarz(),0.5)+pow(h,0.5)),(team[i]->gettary()-ball.getposy())*pow((9.8/2),0.5)/(pow(h+ball.getposz()-team[i]->gettarz(),0.5)+pow(h,0.5)),pow(2*h*9.8,0.5));
                    ball.settar(team[i]->gettarx(),team[i]->gettary(),team[i]->gettarz());
                    printf("  BLOCK:%f,%f,%f\n",team[i]->gettarx(),team[i]->gettary(),team[i]->gettarz());
                    AIstate(team1,Dt,l_team1,team1count,team2count,sta);
                    AIstate(team2,Dt,l_team2,team2count,team1count,sta);
                    //printf("%d,%d\n",*team1count,*team2count);
                }
            }

        }
    }
}
void playerhandle(object *team[],int Dt,int l,int sta)
{
    int dt=1000;
    for(int i=0;i<l;i++)
    {
        if(team[i]->getposz()<-4)
        {
            team[i]->changepos(team[i]->getposx(),team[i]->getposy(),-4);
            team[i]->changev(team[i]->getvx(),team[i]->getvy(),0);
        }
        else if(team[i]->getposz()>-4)
        {
            team[i]->changev(team[i]->getvx(),team[i]->getvy(),team[i]->getvz()-9.8*Dt/dt);
        }

        if(team[i]->getposx()<-9 ||team[i]->getposx()>9)
        {
            team[i]->changepos(team[i]->getposx()-team[i]->getvx()*Dt/dt,team[i]->getposy()-team[i]->getvy()*Dt/dt,team[i]->getposz()-team[i]->getvz()*Dt/dt);
            team[i]->changev(0,team[i]->getvy(),team[i]->getvz());
        }
        if(team[i]->getposy()<1 ||team[i]->getposy()>28+9)
        {
            team[i]->changepos(team[i]->getposx()-team[i]->getvx()*Dt/dt,team[i]->getposy()-team[i]->getvy()*Dt/dt,team[i]->getposz()-team[i]->getvz()*Dt/dt);
            team[i]->changev(team[i]->getvx(),0,team[i]->getvz());
        }

        if(team[i]->getposx()<-9 ||team[i]->getposx()>9)
        {
            team[i]->changepos(team[i]->getposx()-team[i]->getvx()*Dt/dt,team[i]->getposy()-team[i]->getvy()*Dt/dt,team[i]->getposz()-team[i]->getvz()*Dt/dt);
            team[i]->changev(0,team[i]->getvy(),team[i]->getvz());
        }
    }
}
void AIstate(object *team[],int Dt,int l,int *ourtc,int *opptc,int sta)
{

    for(int i=0;i<l;i++)
    {
        if(ball.getposz()<=team[i]->getposz()+1.8+0.7 && ball.getposz()>=team[i]->getposz()+1.5 && (team[i]->getposx()-ball.getposx())*(team[i]->getposx()-ball.getposx())+(team[i]->getposy()-ball.getposy())*(team[i]->getposy()-ball.getposy())<= 0.7 && team[i]->getstate()==BLOCK)
        {
            //ball.settar(team[i]->gettarx(),team[i]->gettary(),team[i]->gettarz());
            /*if((ball.gettary()-19)*(team[i]->getposy()-19)>0)
            {
                if(team[i]->getposition()==SPIKER)
                {
                    team[i]->changestate(PRE_SPIKE);
                    printf("blocker prespike\n");
                }
                else if(team[i]->getposition()==SET)
                {
                    team[i]->changestate(PRE_SET);
                    printf("blocker prespike\n");
                }
            }
            else
            {
                team[i]->changestate(PRE_BLOCK);
                printf("blocker preblock\n");
            }*/
            team[i]->changestate(NOTHING2);
        }
    }
    //static int flag[6]={0};
    float mini=999;
    int ind=-1;
    printf("%f\n",ball.getposy());
    if((ball.getposy()-19)*(team[0]->getposy()-19)<0 && (ball.gettary()-19)*(team[0]->getposy()-19)>0)//球在對面要飛過來了
    {
        for(int i=0;i<l;i++)
        {
            //team[i]->changestate(NOTHING);
            if(team[i]->getposition()!=SETTER &&team[i]->getstate()!=BLOCK && pow(pow(team[i]->getposx()-ball.gettarx(),2)+pow(team[i]->getposy()-ball.gettary(),2)+pow(team[i]->getposz()-ball.gettarz(),2),0.5)<mini)
            {
                mini=pow(pow(team[i]->getposx()-ball.gettarx(),2)+pow(team[i]->getposy()-ball.gettary(),2),0.5);
                ind=i;
            }
        }

        if(ind!=-1) {
            team[ind]->changestate(RECEIVE);
            printf("receive\n");
        }
        for(int i=0;i<l;i++)
        {
            if(team[i]->getposition()==SETTER)
            {
                if(team[i]->getstate()==NOTHING ||team[i]->getstate()==NOTHING2 ||team[i]->getstate()==PRE_BLOCK)
                {
                    team[i]->changestate(PRE_SET);
                    printf("preset\n");
                }
            }
            else if(team[i]->getposition()==OUTSIDESPIKER||team[i]->getposition()==MIDDLEBLOCKER||team[i]->getposition()==OPPOSITEHITTER)
            {
                if(team[i]->getstate()==NOTHING || team[i]->getstate()==NOTHING2 || team[i]->getstate()==PRE_BLOCK)
                {
                    team[i]->changestate(PRE_SPIKE);
                    printf("prespike\n");
                }
            }

        }

    }

    else if((ball.getposy()-19)*(team[0]->getposy()-19)>0 && (ball.gettary()-19)*(team[0]->getposy()-19)>0)//球在自己隊(接到球&要舉球時呼叫)
    {
        float mini=999;
        int ind=-1;
        /*for(int i=0;i<l;i++)
        {
            if(team[i]->getstate()==PRE_SPIKE && *ourtc==2//flag[i]==1
                && pow(pow(team[i]->getposx()-ball.gettarx(),2)+pow(team[i]->getposy()-ball.gettary(),2)+pow(team[i]->getposz()-ball.gettarz(),2),0.5)<mini)
            {
                mini=pow(pow(team[i]->getposx()-ball.gettarx(),2)+pow(team[i]->getposy()-ball.gettary(),2),0.5);
                ind=i;
            }
        }*/
        for(int i=0;i<l;i++)
        {
            if((team[i]->getstate()==PRE_SPIKE||team[i]->getstate()==ASPIKE) && *ourtc==2 &&
               (team[i]->getrotation()==7 ||team[i]->getrotation()==8 ||team[i]->getrotation()==9
                || team[i]->getrotation()==1 ||team[i]->getrotation()==2 ||team[i]->getrotation()==3))
            {
                if(sta==FOUR && team[i]->getposition()==OUTSIDESPIKER)
                {
                    ind=i;
                }
                else if((sta==AQUICK||sta==TWO) && team[i]->getposition()==MIDDLEBLOCKER)
                {
                    ind=i;
                }
                else if(sta==BACK && team[i]->getposition()==OPPOSITEHITTER)
                {
                    ind=i;
                }

            }
        }

        for(int i=0;i<l;i++)
        {
            if(team[i]->getposition()==SETTER)
            {
                if(/*team[i]->getstate()==PRE_SET &&*/ *ourtc==1)
                {
                    team[i]->changestate(SET);
                    printf("set\n");
                }
                else if(team[i]->getstate()==SET && *ourtc==2)
                {
                    team[i]->changestate(NOTHING2);
                    printf("nothing2\n");
                }
            }

            else if(team[i]->getposition()==OUTSIDESPIKER||team[i]->getposition()==MIDDLEBLOCKER||team[i]->getposition()==OPPOSITEHITTER)
            {
                if(team[i]->getstate()==RECEIVE && *ourtc==1)
                {
                    team[i]->changestate(PRE_SPIKE);
                    printf("prespike\n");
                    //flag[i]=1;
                }
                else if((team[i]->getstate()==PRE_SPIKE||team[i]->getstate()==RECEIVE) && *ourtc==1 && team[i]->getposition()==MIDDLEBLOCKER)//&& sta==AQUICK)
                {

                    if(team[i]->getrotation()==7||team[i]->getrotation()==8||team[i]->getrotation()==9||team[i]->getrotation()==1||team[i]->getrotation()==2||team[i]->getrotation()==3)
                    {
                         team[i]->changestate(ASPIKE);
                         printf("A spike\n");
                    }
                }
                else if((team[i]->getstate()==PRE_SPIKE ||team[i]->getstate()==ASPIKE) && *ourtc==2)//flag[i]==1)
                {
                    if(i==ind) {
                        team[i]->changestate(SPIKE);
                        //flag[i]=0;
                        printf("spike\n");
                    }
                    else {
                        team[i]->changestate(NOTHING2);
                        printf("nothing2\n");
                        //flag[i]=0;
                    }
                }
                else
                {
                    if(team[i]->getstate()==SPIKE && sta!=AQUICK)
                    {
                        team[i]->changestate(NOTHING2);
                        printf("nothing2\n");
                    }
                //printf("nothing\n");}
                }
            }
        }
    }

    else if((ball.getposy()-19)*(team[0]->getposy()-19)>0 && (ball.gettary()-19)*(team[0]->getposy()-19)<0)//球在自己隊spike時呼叫
    {
        for(int i=0;i<l;i++)
        {
            if(team[i]->getstate()==SPIKE){
                team[i]->changestate(NOTHING2);
                //printf("nothing\n");
            }
            else if(team[i]->getstate()==SERVE){
                team[i]->changestate(NOTHING2);
                //printf("nothing\n");
            }
            switch (team[i]->getrotation())
            {
            case 1:
                {
                    team[i]->changestate(PRE_BLOCK);
                    printf("preblock\n");
                    break;
                }
            case 2:
                {
                    team[i]->changestate(PRE_BLOCK);
                    printf("preblock\n");
                    break;
                }
            case 3:
                {
                    team[i]->changestate(PRE_BLOCK);
                    printf("preblock\n");
                    break;
                }
            case 7:
                {
                    team[i]->changestate(PRE_BLOCK);
                    printf("preblock\n");
                    break;
                }
            case 8:
                {
                    team[i]->changestate(PRE_BLOCK);
                    printf("preblock\n");
                    break;
                }
            case 9:
                {
                    team[i]->changestate(PRE_BLOCK);
                    printf("preblock\n");
                    break;
                }
            default:
                {
                    team[i]->changestate(NOTHING2);
                    printf("nothing2\n");
                }
            }

        }
    }
    else if((ball.getposy()-19)*(team[0]->getposy()-19)<0 && (ball.gettary()-19)*(team[0]->getposy()-19)<0)
    {
        //static int flag[6]={0};
        for(int i=0;i<l;i++)
        {
            /*if(team[i]->getstate()==PRE_BLOCK && (flag[i]==0 || flag[i]==2))
            {
                flag[i]=1;
                //team[i]->changestate(PRE_BLOCK);
            }
            else*/ if(team[i]->getstate()==PRE_BLOCK && *opptc==2)//flag[i]==1)
            {
                float mini=999;
                int ind=-1;
                for(int i=0;i<l;i++)
                {
                    if(mini>pow(pow(ball.gettarx()-team[i]->getposx(),2)+pow(ball.gettary()-team[i]->getposy(),2),0.5))
                    {
                        mini=pow(pow(ball.gettarx()-team[i]->getposx(),2)+pow(ball.gettary()-team[i]->getposy(),2),0.5);
                        ind=i;
                    }
                }
                if(i==ind)
                {
                    team[ind]->changestate(BLOCK);
                    printf("block\n");

                }
                else
                {
                    team[i]->changestate(NOTHING2);
                    printf("nothing2\n");
                }
                //team[i]->changestate(PRE_BLOCK);
                //flag[i]=0;
            }
            else if(team[i]->getstate()==BLOCK)
            {
                if(team[i]->getposition()==OUTSIDESPIKER||team[i]->getposition()==MIDDLEBLOCKER||team[i]->getposition()==OPPOSITEHITTER)
                {
                     team[i]->changestate(PRE_SPIKE);
                     printf("prespike\n");
                }
                else if(team[i]->getposition()==SET)
                {
                    team[i]->changestate(PRE_SET);
                    printf("prespike\n");
                }
            }
        }
    }
    /*for(int i=0;i<l;i++)
        {
            printf("     %d---%d\n",i,team[i]->getstate());
        }*/
}


/*
void AIstate(object *team[],int Dt,int l)
{
    float mini=999;
    int ind=-1;
    for(int i=0;i<l;i++)
    {
        //team[i]->changestate(NOTHING);
        if(team[i]->getposition()!= SETTER && (ball.getposy()-19)*(team[ind]->getposy()-19)<0 && (ball.gettary()-19)*(team[i]->getposy()-19)>0 && pow(pow(team[i]->getposx()-ball.gettarx(),2)+pow(team[i]->getposy()-ball.gettary(),2)+pow(team[i]->getposz()-ball.gettarz(),2),0.5)<mini)
        {
            mini=pow(pow(team[i]->getposx()-ball.gettarx(),2)+pow(team[i]->getposy()-ball.gettary(),2),0.5);
            ind=i;
        }
    }

    if(ind!=-1 && (ball.getposy()-19)*(team[ind]->getposy()-19)<0 && (ball.gettary()-19)*(team[ind]->getposy()-19)>0)
    {
        team[ind]->changestate(RECEIVE);
    }
    for(int i=0;i<l;i++)
    {
        if(team[i]->getposition()==SETTER && (ball.getposy()-19)*(team[i]->getposy()-19)<0 && (ball.gettary()-19)*(team[i]->getposy()-19)>0)
        {
            if(team[i]->getstate()==NOTHING)
            {
                team[i]->changestate(PRE_SET);
                printf("preset\n");
            }
        }
        if(team[i]->getposition()==SETTER && (ball.getposy()-19)*(team[i]->getposy()-19)>0 && (ball.gettary()-19)*(team[i]->getposy()-19)>0)
        {
            team[i]->settar(-4.5,17.5,-4);
            if(team[i]->getstate()==PRE_SET)
            {
                 team[i]->changestate(SET);
                 printf("set\n");
            }
            else if(team[i]->getstate()==SET) team[i]->changestate(NOTHING);
        }
        else if(team[i]->getposition()==SPIKER && (ball.getposy()-19)*(team[i]->getposy()-19)>0 && (ball.gettary()-19)*(team[i]->getposy()-19)>0 && ball.getvy()*(team[i]->getposy()-19)<0)
        {
            printf("times\n");
            team[i]->settar(-4.5,27,-4);
            static int flag=0;
            if(team[i]->getstate()==NOTHING || team[i]->getstate()==RECEIVE)
            {
                team[i]->changestate(PRE_SPIKE);
                printf("prespike\n");
            }
            else if(team[i]->getstate()==PRE_SPIKE && flag==0)
            {
                flag=1;
            }
            else if(team[i]->getstate()==PRE_SPIKE && flag==1)
            {
                team[i]->changestate(SPIKE);
                flag=0;
                printf("spike\n");
            }
        }
    }

}
*/

void AIchangev(object *team[],int l,int sta)
{
    for(int i=0;i<l;i++)
    {
        switch (team[i]->getstate())
        {
        case RECEIVE:
            {
            float distance=0;
            float vx=0;
            float vy=0;
            float vz=0;
            distance=pow(pow(team[i]->getposx()-ball.gettarx(),2)+pow(team[i]->getposy()-ball.gettary(),2)/*+pow(team[i]->getposz()-ball.gettarz(),2)*/,0.5);
            if(distance!=0)
            {
                vx=3*(ball.gettarx()-team[i]->getposx())/distance;
                vy=3*(ball.gettary()-team[i]->getposy())/distance;
                vz=team[i]->getvz();
            }
            else
            {
                vx=0;
                vy=0;
                vz=team[i]->getvz();
            }
            team[i]->changev(vx,vy,vz);
            break;}
        case NOTHING:
            {
            float distance=0;
            float vx=0;
            float vy=0;
            float vz=0;
            float rota_pos[12][3];
            rota_pos[0][0]=3;
            rota_pos[0][1]=13;
            rota_pos[0][2]=-4;
            rota_pos[1][0]=3;
            rota_pos[1][1]=16;
            rota_pos[1][2]=-4;
            rota_pos[2][0]=0;
            rota_pos[2][1]=16;
            rota_pos[2][2]=-4;
            rota_pos[3][0]=-3;
            rota_pos[3][1]=16;
            rota_pos[3][2]=-4;
            rota_pos[4][0]=-3;
            rota_pos[4][1]=13;
            rota_pos[4][2]=-4;
            rota_pos[5][0]=0;
            rota_pos[5][1]=13;
            rota_pos[5][2]=-4;
            rota_pos[6][0]=-3;
            rota_pos[6][1]=25;
            rota_pos[6][2]=-4;
            rota_pos[7][0]=-3;
            rota_pos[7][1]=22;
            rota_pos[7][2]=-4;
            rota_pos[8][0]=0;
            rota_pos[8][1]=22;
            rota_pos[8][2]=-4;
            rota_pos[9][0]=3;
            rota_pos[9][1]=22;
            rota_pos[9][2]=-4;
            rota_pos[10][0]=3;
            rota_pos[10][1]=25;
            rota_pos[10][2]=-4;
            rota_pos[11][0]=0;
            rota_pos[11][1]=25;
            rota_pos[11][2]=-4;


            distance=pow(pow(team[i]->getposx()-rota_pos[team[i]->getrotation()][0],2)+pow(team[i]->getposy()-rota_pos[team[i]->getrotation()][1],2)+pow(team[i]->getposz()-rota_pos[team[i]->getrotation()][2],2),0.5);
            int ind_mini=-1;
            float mini=999;
            for(int j=0;j<l;j++)
            {
                if(j!=i)
                {
                    float distancetoteamate=pow(pow(team[i]->getposx()-team[j]->getposx(),2)+pow(team[i]->getposy()-team[j]->getposy(),2)+pow(team[i]->getposz()-team[j]->getposz(),2),0.5);
                    if(mini>distancetoteamate && (team[j]->getstate()==SET || team[j]->getstate()==RECEIVE || team[j]->getstate()==SPIKE || team[j]->getstate()==BLOCK))
                    {
                        mini=distancetoteamate;
                        ind_mini=j;
                    }
                }
            }
            if(mini<2 &&team[i]->getposz()<-4+0.1)
            {
                vx=3*(team[i]->getposx()-team[ind_mini]->getposx())/mini;
                vy=team[i]->getvy();
                vz=team[i]->getvz();
            }
            else if(distance!=0 && team[i]->getposz()<-4+0.1)
            {
                vx=3*(rota_pos[team[i]->getrotation()][0]-team[i]->getposx())/distance;
                vy=3*(rota_pos[team[i]->getrotation()][1]-team[i]->getposy())/distance;
                vz=team[i]->getvz();
            }
            else if(distance<0.1)
            {
                vx=0;
                vy=0;
                vz=team[i]->getvz();
            }
            else
            {
                vx=team[i]->getvx();
                vy=team[i]->getvy();
                vz=team[i]->getvz();
            }
            team[i]->changev(vx,vy,vz);
            break;}
        case NOTHING2:
            {
            float distance=0;
            float vx=0;
            float vy=0;
            float vz=0;
            float rota_pos[12][3];
            rota_pos[0][0]=3;
            rota_pos[0][1]=13;
            rota_pos[0][2]=-4;
            rota_pos[1][0]=3;
            rota_pos[1][1]=16;
            rota_pos[1][2]=-4;
            rota_pos[2][0]=0;
            rota_pos[2][1]=16;
            rota_pos[2][2]=-4;
            rota_pos[3][0]=-3;
            rota_pos[3][1]=16;
            rota_pos[3][2]=-4;
            rota_pos[4][0]=-3;
            rota_pos[4][1]=13;
            rota_pos[4][2]=-4;
            rota_pos[5][0]=0;
            rota_pos[5][1]=13;
            rota_pos[5][2]=-4;
            rota_pos[6][0]=-3;
            rota_pos[6][1]=25;
            rota_pos[6][2]=-4;
            rota_pos[7][0]=-3;
            rota_pos[7][1]=22;
            rota_pos[7][2]=-4;
            rota_pos[8][0]=0;
            rota_pos[8][1]=22;
            rota_pos[8][2]=-4;
            rota_pos[9][0]=3;
            rota_pos[9][1]=22;
            rota_pos[9][2]=-4;
            rota_pos[10][0]=3;
            rota_pos[10][1]=25;
            rota_pos[10][2]=-4;
            rota_pos[11][0]=0;
            rota_pos[11][1]=25;
            rota_pos[11][2]=-4;


            distance=pow(pow(team[i]->getposx()-rota_pos[team[i]->getrotation()][0],2)+pow(team[i]->getposy()-rota_pos[team[i]->getrotation()][1],2)+pow(team[i]->getposz()-rota_pos[team[i]->getrotation()][2],2),0.5);
            int ind_mini=-1;
            float mini=999;
            for(int j=0;j<l;j++)
            {
                if(j!=i)
                {
                    float distancetoteamate=pow(pow(team[i]->getposx()-team[j]->getposx(),2)+pow(team[i]->getposy()-team[j]->getposy(),2)+pow(team[i]->getposz()-team[j]->getposz(),2),0.5);
                    if(mini>distancetoteamate && (team[j]->getstate()==SET || team[j]->getstate()==RECEIVE || team[j]->getstate()==SPIKE || team[j]->getstate()==BLOCK))
                    {
                        mini=distancetoteamate;
                        ind_mini=j;
                    }
                }
            }
            if(mini<2 &&team[i]->getposz()<-4+0.1)
            {
                vx=3*(team[i]->getposx()-team[ind_mini]->getposx())/mini;
                vy=team[i]->getvy();
                vz=team[i]->getvz();
            }
            else if(distance!=0 && team[i]->getposz()<-4+0.1)
            {
                if(team[i]->getposition()==SETTER)
                {
                    if(team[i]->getrotation()==1 ||team[i]->getrotation()==2 ||team[i]->getrotation()==3 )
                    {
                        distance=pow(pow(team[i]->getposx()-rota_pos[1][0],2)+pow(team[i]->getposy()-rota_pos[1][1],2),0.5);
                        vx=3*(rota_pos[1][0]-team[i]->getposx())/distance;
                        vy=3*(rota_pos[1][1]-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                    else if(team[i]->getrotation()==7 ||team[i]->getrotation()==8 ||team[i]->getrotation()==9)
                    {
                        distance=pow(pow(team[i]->getposx()-rota_pos[7][0],2)+pow(team[i]->getposy()-rota_pos[7][1],2),0.5);
                        vx=3*(rota_pos[7][0]-team[i]->getposx())/distance;
                        vy=3*(rota_pos[7][1]-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                    else
                    {
                        vx=3*(rota_pos[team[i]->getrotation()][0]-team[i]->getposx())/distance;
                        vy=3*(rota_pos[team[i]->getrotation()][1]-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                }
                else if(team[i]->getposition()==OUTSIDESPIKER)
                {
                    if(team[i]->getrotation()==1 ||team[i]->getrotation()==2 ||team[i]->getrotation()==3)
                    {
                        distance=pow(pow(team[i]->getposx()-rota_pos[3][0],2)+pow(team[i]->getposy()-rota_pos[3][1],2),0.5);
                        vx=3*(rota_pos[3][0]-team[i]->getposx())/distance;
                        vy=3*(rota_pos[3][1]-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                    else if(team[i]->getrotation()==7 ||team[i]->getrotation()==8 ||team[i]->getrotation()==9)
                    {
                        distance=pow(pow(team[i]->getposx()-rota_pos[9][0],2)+pow(team[i]->getposy()-rota_pos[9][1],2),0.5);
                        vx=3*(rota_pos[9][0]-team[i]->getposx())/distance;
                        vy=3*(rota_pos[9][1]-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                    else
                    {
                        vx=3*(rota_pos[team[i]->getrotation()][0]-team[i]->getposx())/distance;
                        vy=3*(rota_pos[team[i]->getrotation()][1]-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }

                }
                else if(team[i]->getposition()==MIDDLEBLOCKER)
                {
                    if(team[i]->getrotation()==1 ||team[i]->getrotation()==2 ||team[i]->getrotation()==3)
                    {
                        distance=pow(pow(team[i]->getposx()-rota_pos[2][0],2)+pow(team[i]->getposy()-rota_pos[2][1],2),0.5);
                        vx=3*(rota_pos[2][0]-team[i]->getposx())/distance;
                        vy=3*(rota_pos[2][1]-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                    else if(team[i]->getrotation()==7 ||team[i]->getrotation()==8 ||team[i]->getrotation()==9)
                    {
                        distance=pow(pow(team[i]->getposx()-rota_pos[8][0],2)+pow(team[i]->getposy()-rota_pos[8][1],2),0.5);
                        vx=3*(rota_pos[8][0]-team[i]->getposx())/distance;
                        vy=3*(rota_pos[8][1]-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                    else
                    {
                        vx=3*(rota_pos[team[i]->getrotation()][0]-team[i]->getposx())/distance;
                        vy=3*(rota_pos[team[i]->getrotation()][1]-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                }
                else if(team[i]->getposition()==OPPOSITEHITTER)
                {
                    if(team[i]->getrotation()==1 ||team[i]->getrotation()==2 ||team[i]->getrotation()==3 )
                    {
                        distance=pow(pow(team[i]->getposx()-rota_pos[1][0],2)+pow(team[i]->getposy()-rota_pos[1][1],2),0.5);
                        vx=3*(rota_pos[1][0]-team[i]->getposx())/distance;
                        vy=3*(rota_pos[1][1]-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                    else if(team[i]->getrotation()==7 ||team[i]->getrotation()==8 ||team[i]->getrotation()==9)
                    {
                        distance=pow(pow(team[i]->getposx()-rota_pos[7][0],2)+pow(team[i]->getposy()-rota_pos[7][1],2),0.5);
                        vx=3*(rota_pos[7][0]-team[i]->getposx())/distance;
                        vy=3*(rota_pos[7][1]-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                    else
                    {
                        vx=3*(rota_pos[team[i]->getrotation()][0]-team[i]->getposx())/distance;
                        vy=3*(rota_pos[team[i]->getrotation()][1]-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                }

            }
            else if(distance<0.1)
            {
                vx=0;
                vy=0;
                vz=team[i]->getvz();
            }
            else
            {
                vx=team[i]->getvx();
                vy=team[i]->getvy();
                vz=team[i]->getvz();
            }
            team[i]->changev(vx,vy,vz);
            break;}
        case PRE_SET:
            {
            float distance=0;
            float vx=0;
            float vy=0;
            float vz=0;
            float set_pos[12][4];
            set_pos[0][0]=1.5;
            set_pos[0][1]=17.5;
            set_pos[0][2]=-4;
            set_pos[1][0]=1.5;
            set_pos[1][1]=17.5;
            set_pos[1][2]=-4;
            set_pos[2][0]=1.5;
            set_pos[2][1]=17.5;
            set_pos[2][2]=-4;
            set_pos[3][0]=1.5;
            set_pos[3][1]=17.5;
            set_pos[3][2]=-4;
            set_pos[4][0]=1.5;
            set_pos[4][1]=17.5;
            set_pos[4][2]=-4;
            set_pos[5][0]=1.5;
            set_pos[5][1]=17.5;
            set_pos[5][2]=-4;
            set_pos[6][0]=-1.5;
            set_pos[6][1]=20.5;
            set_pos[6][2]=-4;
            set_pos[7][0]=-1.5;
            set_pos[7][1]=20.5;
            set_pos[7][2]=-4;
            set_pos[8][0]=-1.5;
            set_pos[8][1]=20.5;
            set_pos[8][2]=-4;
            set_pos[9][0]=-1.5;
            set_pos[9][1]=20.5;
            set_pos[9][2]=-4;
            set_pos[10][0]=-1.5;
            set_pos[10][1]=20.5;
            set_pos[10][2]=-4;
            set_pos[11][0]=-1.5;
            set_pos[11][1]=20.5;
            set_pos[11][2]=-4;
            distance=pow(pow(team[i]->getposx()-set_pos[team[i]->getrotation()][0],2)+pow(team[i]->getposy()-set_pos[team[i]->getrotation()][1],2)+pow(team[i]->getposz()-set_pos[team[i]->getrotation()][2],2),0.5);
            int ind_mini=-1;
            float mini=999;
            for(int j=0;j<l;j++)
            {
                if(j!=i)
                {
                    float distancetoteamate=pow(pow(team[i]->getposx()-team[j]->getposx(),2)+pow(team[i]->getposy()-team[j]->getposy(),2)+pow(team[i]->getposz()-team[j]->getposz(),2),0.5);
                    if(mini>distancetoteamate && (team[j]->getstate()==SET || team[j]->getstate()==RECEIVE || team[j]->getstate()==SPIKE || team[j]->getstate()==BLOCK))
                    {
                        mini=distancetoteamate;
                        ind_mini=j;
                    }
                }
            }
            if(mini<2 &&team[i]->getposz()<-4+0.1)
            {
                vx=3*(team[i]->getposx()-team[ind_mini]->getposx())/mini;
                vy=team[i]->getvy();
                vz=team[i]->getvz();
            }
            else if(distance!=0 && team[i]->getposz()<-4+0.1)
            {
                vx=3*(set_pos[team[i]->getrotation()][0]-team[i]->getposx())/distance;
                vy=3*(set_pos[team[i]->getrotation()][1]-team[i]->getposy())/distance;
                vz=team[i]->getvz();
            }
            else
            {
                vx=0;
                vy=0;
                vz=team[i]->getvz();
            }
            team[i]->changev(vx,vy,vz);
            break;}
        case SET:
            {
            float distance=0;
            float vx=0;
            float vy=0;
            float vz=0;
            distance=pow(pow(team[i]->getposx()-ball.gettarx(),2)+pow(team[i]->getposy()-ball.gettary(),2)/*+pow(team[i]->getposz()-ball.gettarz(),2)*/,0.5);
            if(distance!=0 && team[i]->getposz()<-4+0.1)
            {
                vx=3*(ball.gettarx()-team[i]->getposx())/distance;
                vy=3*(ball.gettary()-team[i]->getposy())/distance;
                vz=team[i]->getvz();
            }
            else
            {
                vx=0;
                vy=0;
                vz=team[i]->getvz();
            }
            team[i]->changev(vx,vy,vz);
            break;}
        case PRE_SPIKE:
            {
            float distance=0;
            float vx=0;
            float vy=0;
            float vz=0;
            float spike_pos[12][3];
            spike_pos[0][0]=4;
            spike_pos[0][1]=13;
            spike_pos[0][2]=-4;
            spike_pos[1][0]=5;
            spike_pos[1][1]=16;
            spike_pos[1][2]=-4;
            spike_pos[2][0]=-1;
            spike_pos[2][1]=16;
            spike_pos[2][2]=-4;
            spike_pos[3][0]=-4.5;
            spike_pos[3][1]=16;
            spike_pos[3][2]=-4;
            spike_pos[4][0]=-3;
            spike_pos[4][1]=13;
            spike_pos[4][2]=-4;
            spike_pos[5][0]=0;
            spike_pos[5][1]=13;
            spike_pos[5][2]=-4;
            spike_pos[6][0]=-3;
            spike_pos[6][1]=25;
            spike_pos[6][2]=-4;
            spike_pos[7][0]=-5;
            spike_pos[7][1]=22;
            spike_pos[7][2]=-4;
            spike_pos[8][0]=1;
            spike_pos[8][1]=22;
            spike_pos[8][2]=-4;
            spike_pos[9][0]=5;
            spike_pos[9][1]=22;
            spike_pos[9][2]=-4;
            spike_pos[10][0]=3;
            spike_pos[10][1]=25;
            spike_pos[10][2]=-4;
            spike_pos[11][0]=0;
            spike_pos[11][1]=25;
            spike_pos[11][2]=-4;
            distance=pow(pow(team[i]->getposx()-spike_pos[team[i]->getrotation()][0],2)+pow(team[i]->getposy()-spike_pos[team[i]->getrotation()][1],2)+pow(team[i]->getposz()-spike_pos[team[i]->getrotation()][2],2),0.5);
            int ind_mini=-1;
            float mini=999;
            for(int j=0;j<l;j++)
            {
                if(j!=i)
                {
                    float distancetoteamate=pow(pow(team[i]->getposx()-team[j]->getposx(),2)+pow(team[i]->getposy()-team[j]->getposy(),2)+pow(team[i]->getposz()-team[j]->getposz(),2),0.5);
                    if(mini>distancetoteamate && (team[j]->getstate()==SET || team[j]->getstate()==RECEIVE || team[j]->getstate()==SPIKE || team[j]->getstate()==BLOCK))
                    {
                        mini=distancetoteamate;
                        ind_mini=j;
                    }
                }
            }
            if(mini<2 &&team[i]->getposz()<-4+0.1 )
            {
                vx=3*(team[i]->getposx()-team[ind_mini]->getposx())/mini;
                if((team[i]->getposy()-team[ind_mini]->getposy())/mini*(team[i]->getposy()-19)>0) vy=3*(team[i]->getposy()-team[ind_mini]->getposy())/mini;
                vz=team[i]->getvz();
            }
            else if(distance!=0 && team[i]->getposz()<-4+0.1)
            {
                if(team[i]->getposition()==OUTSIDESPIKER)
                {
                    if(team[i]->getrotation()==1||team[i]->getrotation()==2||team[i]->getrotation()==3)
                    {
                        distance=pow(pow(team[i]->getposx()-spike_pos[3][0],2)+pow(team[i]->getposy()-spike_pos[3][1],2),0.5);
                        vx=3*(spike_pos[3][0]-team[i]->getposx())/distance;
                        vy=3*(spike_pos[3][1]-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                    else if(team[i]->getrotation()==7||team[i]->getrotation()==8||team[i]->getrotation()==9)
                    {
                        distance=pow(pow(team[i]->getposx()-spike_pos[9][0],2)+pow(team[i]->getposy()-spike_pos[9][1],2),0.5);
                        vx=3*(spike_pos[9][0]-team[i]->getposx())/distance;
                        vy=3*(spike_pos[9][1]-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                    else
                    {
                        vx=3*(spike_pos[team[i]->getrotation()][0]-team[i]->getposx())/distance;
                        vy=3*(spike_pos[team[i]->getrotation()][1]-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                }
                else if(team[i]->getposition()==MIDDLEBLOCKER)
                {
                    if(team[i]->getrotation()==1||team[i]->getrotation()==2||team[i]->getrotation()==3)
                    {
                        distance=pow(pow(team[i]->getposx()-spike_pos[2][0],2)+pow(team[i]->getposy()-spike_pos[2][1],2),0.5);
                        vx=3*(spike_pos[2][0]-team[i]->getposx())/distance;
                        vy=3*(spike_pos[2][1]-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                    else if(team[i]->getrotation()==7||team[i]->getrotation()==8||team[i]->getrotation()==9)
                    {
                        distance=pow(pow(team[i]->getposx()-spike_pos[8][0],2)+pow(team[i]->getposy()-spike_pos[8][1],2),0.5);
                        vx=3*(spike_pos[8][0]-team[i]->getposx())/distance;
                        vy=3*(spike_pos[8][1]-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                    else
                    {
                        vx=3*(spike_pos[team[i]->getrotation()][0]-team[i]->getposx())/distance;
                        vy=3*(spike_pos[team[i]->getrotation()][1]-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                }
                else if(team[i]->getposition()==OPPOSITEHITTER)
                {
                    if(team[i]->getrotation()==1||team[i]->getrotation()==2||team[i]->getrotation()==3)
                    {
                        distance=pow(pow(team[i]->getposx()-spike_pos[1][0],2)+pow(team[i]->getposy()-spike_pos[1][1],2),0.5);
                        vx=3*(spike_pos[1][0]-team[i]->getposx())/distance;
                        vy=3*(spike_pos[1][1]-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                    else if(team[i]->getrotation()==7||team[i]->getrotation()==8||team[i]->getrotation()==9)
                    {
                        distance=pow(pow(team[i]->getposx()-spike_pos[7][0],2)+pow(team[i]->getposy()-spike_pos[7][1],2),0.5);
                        vx=3*(spike_pos[7][0]-team[i]->getposx())/distance;
                        vy=3*(spike_pos[7][1]-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                    else
                    {
                        vx=3*(spike_pos[team[i]->getrotation()][0]-team[i]->getposx())/distance;
                        vy=3*(spike_pos[team[i]->getrotation()][1]-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                }

            }
            else
            {
                vx=0;
                vy=0;
                vz=team[i]->getvz();
            }
            team[i]->changev(vx,vy,vz);
            break;}
        case SPIKE:
            {float distance=0;
            float vx=0;
            float vy=0;
            float vz=0;
            float flyt=0.3;
            float distancetoball;
            float vb;
            float posx_setter=0;
            distance=pow(pow(team[i]->getposx()-ball.gettarx(),2)+pow(team[i]->getposy()-ball.gettary(),2)/*+pow(team[i]->getposz()-ball.gettarz(),2)*/,0.5);
            distancetoball=pow(pow(ball.gettarx()-ball.getposx(),2)+pow(ball.gettary()-ball.getposy(),2),0.5);
            vb=pow(ball.getvx()*ball.getvx()+ball.getvy()*ball.getvy(),0.5);
            for(int j=0;j<l;j++)
            {
                if(team[j]->getposition()==SETTER) posx_setter=team[j]->getposx();
            }

            if(distancetoball/vb-distance/5.0<=flyt &&  distance>=0.1)//distance>=0.1)
            {
                /*if(team[i]->getposition()==MIDDLEBLOCKER )
                {

                    if(team[i]->getrotation()<=5)
                    {
                        distance=pow(pow(posx_setter-1.5-team[i]->getposx(),2)+pow(18-team[i]->getposy(),2),0.5);
                        vx=5*(posx_setter-1.5-team[i]->getposx())/distance;
                        vy=5*(18-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                    else if(team[i]->getrotation()>=6)
                    {
                        distance=pow(pow(posx_setter+1.5-team[i]->getposx(),2)+pow(20-team[i]->getposy(),2),0.5);
                        vx=5*(posx_setter+1.5-team[i]->getposx())/distance;
                        vy=5*(20-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }

                }
                else
                {*/
                    vx=5*(ball.gettarx()-team[i]->getposx())/distance;
                    vy=5*(ball.gettary()-team[i]->getposy())/distance;
                    vz=team[i]->getvz();
                //}
            }
            else if(distance<0.1 && team[i]->getposz()<-4+0.1 && ball.getposz()<-4+3.5)
            {
                vx=0;
                vy=0;
                vz=6;
            }
            /*else if(team[i]->getposition()==MIDDLEBLOCKER && team[i]->getposz()<-4+0.1)
            {
                if(team[i]->getrotation()<=5)
                {
                    distance=pow(pow(posx_setter-1-team[i]->getposx(),2)+pow(18-team[i]->getposy(),2),0.5);
                }
                else if(team[i]->getrotation()>=5)
                {
                    distance=pow(pow(posx_setter+1-team[i]->getposx(),2)+pow(20-team[i]->getposy(),2),0.5);
                }
                if(distance<0.1)
                {
                    vx=0;
                    vy=0;
                    vz=6;
                }
            }*/
            else
            {
                vx=0;
                vy=0;
                vz=team[i]->getvz();
            }
            team[i]->changev(vx,vy,vz);

            break;}
        case ASPIKE:
            {float distance=0;
            float vx=0;
            float vy=0;
            float vz=0;
            float flyt=0.3;
            float distancetoball;
            float vb;
            float posx_setter=0;
            distance=pow(pow(ball.getposx()-ball.gettarx(),2)+pow(ball.getposy()-ball.gettary(),2)/*+pow(team[i]->getposz()-ball.gettarz(),2)*/,0.5);
            distancetoball=pow(pow(ball.gettarx()-ball.getposx(),2)+pow(ball.gettary()-ball.getposy(),2),0.5);
            vb=pow(ball.getvx()*ball.getvx()+ball.getvy()*ball.getvy(),0.5);
            int ind_mini=-1;
            float mini=999;
            for(int j=0;j<l;j++)
            {
                if(j!=i)
                {
                    float distancetoteamate=pow(pow(team[i]->getposx()-team[j]->getposx(),2)+pow(team[i]->getposy()-team[j]->getposy(),2)+pow(team[i]->getposz()-team[j]->getposz(),2),0.5);
                    if(mini>distancetoteamate && (team[j]->getstate()==SET || team[j]->getstate()==RECEIVE || team[j]->getstate()==SPIKE || team[j]->getstate()==BLOCK))
                    {
                        mini=distancetoteamate;
                        ind_mini=j;
                    }
                }
            }
            for(int j=0;j<l;j++)
            {
                if(team[j]->getposition()==SETTER) posx_setter=team[j]->getposx();
            }

            if(mini<2 &&team[i]->getposz()<-4+0.1 )
            {
                vx=3*(team[i]->getposx()-team[ind_mini]->getposx())/mini;
                if((team[i]->getposy()-team[ind_mini]->getposy())/mini*(team[i]->getposy()-19)>0) vy=3*(team[i]->getposy()-team[ind_mini]->getposy())/mini;
                vz=team[i]->getvz();
            }


            else if(distancetoball/vb-distance/5.0<=flyt &&  distance>=0.1)//distance>=0.1)
            {
                if(team[i]->getposition()==MIDDLEBLOCKER )
                {

                    if(team[i]->getrotation()<=5)
                    {
                        distance=pow(pow(posx_setter-1.5-team[i]->getposx(),2)+pow(18-team[i]->getposy(),2),0.5);
                        vx=5*(posx_setter-1.5-team[i]->getposx())/distance;
                        vy=5*(18-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }
                    else if(team[i]->getrotation()>=6)
                    {
                        distance=pow(pow(posx_setter+1.5-team[i]->getposx(),2)+pow(20-team[i]->getposy(),2),0.5);
                        vx=5*(posx_setter+1.5-team[i]->getposx())/distance;
                        vy=5*(20-team[i]->getposy())/distance;
                        vz=team[i]->getvz();
                    }

                }
            }
            else if(distance<0.1 && team[i]->getposz()<-4+0.1)
            {
                vx=0;
                vy=0;
                vz=6;
            }
            else
            {
                vx=0;
                vy=0;
                vz=team[i]->getvz();
            }
            team[i]->changev(vx,vy,vz);

            break;}
        case SERVE:
            {
            float distance=0;
            float vx=0;
            float vy=0;
            float vz=0;
            float ser_pos[12][3];
            ser_pos[0][0]=4;
            ser_pos[0][1]=9;
            ser_pos[0][2]=-4;
            ser_pos[1][0]=4;
            ser_pos[1][1]=9;
            ser_pos[1][2]=-4;
            ser_pos[2][0]=4;
            ser_pos[2][1]=9;
            ser_pos[2][2]=-4;
            ser_pos[3][0]=4;
            ser_pos[3][1]=9;
            ser_pos[3][2]=-4;
            ser_pos[4][0]=4;
            ser_pos[4][1]=9;
            ser_pos[4][2]=-4;
            ser_pos[5][0]=4;
            ser_pos[5][1]=9;
            ser_pos[5][2]=-4;
            ser_pos[6][0]=-4;
            ser_pos[6][1]=29;
            ser_pos[6][2]=-4;
            ser_pos[7][0]=-4;
            ser_pos[7][1]=29;
            ser_pos[7][2]=-4;
            ser_pos[8][0]=-4;
            ser_pos[8][1]=29;
            ser_pos[8][2]=-4;
            ser_pos[9][0]=-4;
            ser_pos[9][1]=29;
            ser_pos[9][2]=-4;
            ser_pos[10][0]=-4;
            ser_pos[10][1]=29;
            ser_pos[10][2]=-4;
            ser_pos[11][0]=-4;
            ser_pos[11][1]=29;
            ser_pos[11][2]=-4;
            distance=pow(pow(team[i]->getposx()-ser_pos[team[i]->getrotation()][0],2)+pow(team[i]->getposy()-ser_pos[team[i]->getrotation()][1],2)+pow(team[i]->getposz()-ser_pos[team[i]->getrotation()][2],2),0.5);
            if(distance>=0.1)
            {
                vx=3*(ser_pos[team[i]->getrotation()][0]-team[i]->getposx())/distance;
                vy=3*(ser_pos[team[i]->getrotation()][1]-team[i]->getposy())/distance;
                vz=team[i]->getvz();

            }
            else if(ball.getposz()<-4+1.8 && ball.getvz()<=0)
            {
                ball.changev(0,0,5);

                vx=0;
                vy=0;
                vz=team[i]->getvz();

            }
            else
            {
                vx=team[i]->getvx();
                vy=team[i]->getvy();
                vz=team[i]->getvz();
            }
            team[i]->changev(vx,vy,vz);
            //printf("     %f,%f,%f\n",team[i]->getvx(),team[i]->getvy(),team[i]->getvz());
            //printf("     %f,%f,%f\n",ball.getvx(),ball.getvy(),ball.getvz());
            //printf("     %f,%f,%f\n",ball.getposx(),ball.getposy(),ball.getposz());
            break;}
        case PRE_BLOCK:
            {
                float distance=0;
                float block_pos[12][3];
                float vx=0;
                float vy=0;
                float vz=0;
                float d_net=0.8;
                block_pos[0][0]=0;
                block_pos[0][1]=19-d_net;
                block_pos[0][2]=-4;
                block_pos[1][0]=1.5;
                block_pos[1][1]=19-d_net;
                block_pos[1][2]=-4;
                block_pos[2][0]=0;
                block_pos[2][1]=19-d_net;
                block_pos[2][2]=-4;
                block_pos[3][0]=-1.5;
                block_pos[3][1]=19-d_net;
                block_pos[3][2]=-4;
                block_pos[4][0]=0;
                block_pos[4][1]=19-d_net;
                block_pos[4][2]=-4;
                block_pos[5][0]=0;
                block_pos[5][1]=19-d_net;
                block_pos[5][2]=-4;
                block_pos[6][0]=0;
                block_pos[6][1]=19+d_net;
                block_pos[6][2]=-4;
                block_pos[7][0]=-1.5;
                block_pos[7][1]=19+d_net;
                block_pos[7][2]=-4;
                block_pos[8][0]=0;
                block_pos[8][1]=19+d_net;
                block_pos[8][2]=-4;
                block_pos[9][0]=1.5;
                block_pos[9][1]=19+d_net;
                block_pos[9][2]=-4;
                block_pos[10][0]=0;
                block_pos[10][1]=19+d_net;
                block_pos[10][2]=-4;
                block_pos[11][0]=0;
                block_pos[11][1]=19+d_net;
                block_pos[11][2]=-4;

                distance=pow(pow(team[i]->getposx()-block_pos[team[i]->getrotation()][0],2)+pow(team[i]->getposy()-block_pos[team[i]->getrotation()][1],2)+pow(team[i]->getposz()-block_pos[team[i]->getrotation()][2],2),0.5);
                if(distance>=0.1 && team[i]->getposz()<-4+0.1 )
                {
                    if(team[i]->getposition()==SETTER)
                    {
                        if(team[i]->getrotation()==1 ||team[i]->getrotation()==2 ||team[i]->getrotation()==3 )
                        {
                            distance=pow(pow(team[i]->getposx()-block_pos[1][0],2)+pow(team[i]->getposy()-block_pos[1][1],2),0.5);
                            vx=3*(block_pos[1][0]-team[i]->getposx())/distance;
                            vy=3*(block_pos[1][1]-team[i]->getposy())/distance;
                            vz=team[i]->getvz();
                        }
                        else if(team[i]->getrotation()==7 ||team[i]->getrotation()==8 ||team[i]->getrotation()==9)
                        {
                            distance=pow(pow(team[i]->getposx()-block_pos[7][0],2)+pow(team[i]->getposy()-block_pos[7][1],2),0.5);
                            vx=3*(block_pos[7][0]-team[i]->getposx())/distance;
                            vy=3*(block_pos[7][1]-team[i]->getposy())/distance;
                            vz=team[i]->getvz();
                        }
                        else
                        {
                            vx=3*(block_pos[team[i]->getrotation()][0]-team[i]->getposx())/distance;
                            vy=3*(block_pos[team[i]->getrotation()][1]-team[i]->getposy())/distance;
                            vz=team[i]->getvz();
                        }
                    }
                    else if(team[i]->getposition()==OUTSIDESPIKER)
                    {
                        if(team[i]->getrotation()==1 ||team[i]->getrotation()==2 ||team[i]->getrotation()==3)
                        {
                            distance=pow(pow(team[i]->getposx()-block_pos[3][0],2)+pow(team[i]->getposy()-block_pos[3][1],2),0.5);
                            vx=3*(block_pos[3][0]-team[i]->getposx())/distance;
                            vy=3*(block_pos[3][1]-team[i]->getposy())/distance;
                            vz=team[i]->getvz();
                        }
                        else if(team[i]->getrotation()==7 ||team[i]->getrotation()==8 ||team[i]->getrotation()==9)
                        {
                            distance=pow(pow(team[i]->getposx()-block_pos[9][0],2)+pow(team[i]->getposy()-block_pos[9][1],2),0.5);
                            vx=3*(block_pos[9][0]-team[i]->getposx())/distance;
                            vy=3*(block_pos[9][1]-team[i]->getposy())/distance;
                            vz=team[i]->getvz();
                        }
                        else
                        {
                            vx=3*(block_pos[team[i]->getrotation()][0]-team[i]->getposx())/distance;
                            vy=3*(block_pos[team[i]->getrotation()][1]-team[i]->getposy())/distance;
                            vz=team[i]->getvz();
                        }

                    }
                    else if(team[i]->getposition()==MIDDLEBLOCKER)
                    {
                        if(team[i]->getrotation()==1 ||team[i]->getrotation()==2 ||team[i]->getrotation()==3)
                        {
                            distance=pow(pow(team[i]->getposx()-block_pos[2][0],2)+pow(team[i]->getposy()-block_pos[2][1],2),0.5);
                            vx=3*(block_pos[2][0]-team[i]->getposx())/distance;
                            vy=3*(block_pos[2][1]-team[i]->getposy())/distance;
                            vz=team[i]->getvz();
                        }
                        else if(team[i]->getrotation()==7 ||team[i]->getrotation()==8 ||team[i]->getrotation()==9)
                        {
                            distance=pow(pow(team[i]->getposx()-block_pos[8][0],2)+pow(team[i]->getposy()-block_pos[8][1],2),0.5);
                            vx=3*(block_pos[8][0]-team[i]->getposx())/distance;
                            vy=3*(block_pos[8][1]-team[i]->getposy())/distance;
                            vz=team[i]->getvz();
                        }
                        else
                        {
                            vx=3*(block_pos[team[i]->getrotation()][0]-team[i]->getposx())/distance;
                            vy=3*(block_pos[team[i]->getrotation()][1]-team[i]->getposy())/distance;
                            vz=team[i]->getvz();
                        }
                    }
                    else if(team[i]->getposition()==OPPOSITEHITTER)
                    {
                        if(team[i]->getrotation()==1 ||team[i]->getrotation()==2 ||team[i]->getrotation()==3 )
                        {
                            distance=pow(pow(team[i]->getposx()-block_pos[1][0],2)+pow(team[i]->getposy()-block_pos[1][1],2),0.5);
                            vx=3*(block_pos[1][0]-team[i]->getposx())/distance;
                            vy=3*(block_pos[1][1]-team[i]->getposy())/distance;
                            vz=team[i]->getvz();
                        }
                        else if(team[i]->getrotation()==7 ||team[i]->getrotation()==8 ||team[i]->getrotation()==9)
                        {
                            distance=pow(pow(team[i]->getposx()-block_pos[7][0],2)+pow(team[i]->getposy()-block_pos[7][1],2),0.5);
                            vx=3*(block_pos[7][0]-team[i]->getposx())/distance;
                            vy=3*(block_pos[7][1]-team[i]->getposy())/distance;
                            vz=team[i]->getvz();
                        }
                        else
                        {
                            vx=3*(block_pos[team[i]->getrotation()][0]-team[i]->getposx())/distance;
                            vy=3*(block_pos[team[i]->getrotation()][1]-team[i]->getposy())/distance;
                            vz=team[i]->getvz();
                        }
                    }
                }
                else
                {
                    vx=0;
                    vy=0;
                    vz=team[i]->getvz();
                }
                team[i]->changev(vx,vy,vz);
                break;
            }
        case BLOCK:
            {
                float distance=0;
                float block_posy[12]={0};
                float vx=0;
                float vy=0;
                float vz=0;
                block_posy[0]=19-0.11;
                block_posy[1]=19-0.11;
                block_posy[2]=19-0.11;
                block_posy[3]=19-0.11;
                block_posy[4]=19-0.11;
                block_posy[5]=19-0.11;
                block_posy[6]=19+0.11;
                block_posy[7]=19+0.11;
                block_posy[8]=19+0.11;
                block_posy[9]=19+0.11;
                block_posy[10]=19+0.11;
                block_posy[11]=19+0.11;
                distance=pow(pow(team[i]->getposx()-ball.gettarx(),2)/*+pow(team[i]->getposy()-block_posy[i],2)+pow(team[i]->getposz()-ball.getposz(),2)*/,0.5);
                if(distance>=0.1 && abs(ball.getposy()-19)<2 )
                {
                    vx=3*(ball.gettarx()-team[i]->getposx())/distance;
                    vy=0;//3*(block_posy[i]-team[i]->getposy())/distance;
                    vz=team[i]->getvz();
                }
                else if(team[i]->getposz()<-4+0.1 && ball.getposz()<-4+3.5 && ball.getvz()<0 && abs(ball.getposy()-19)<3)
                {
                    vx=0;
                    vy=0;
                    vz=4.5;
                }
                else
                {
                    vx=0;
                    vy=0;
                    vz=team[i]->getvz();
                }
                team[i]->changev(vx,vy,vz);
                if(team[i]->getposz()<-4+0.1 && team[i]->getvz()<0 )
                {
                    if((ball.gettary()-19)*(team[i]->getposy()-19)>0)
                    {
                        if(team[i]->getposition()==OUTSIDESPIKER||team[i]->getposition()==MIDDLEBLOCKER||team[i]->getposition()==OPPOSITEHITTER)
                        {
                            team[i]->changestate(PRE_SPIKE);
                            printf("blocker prespike\n");
                        }
                        else if(team[i]->getposition()==SET)
                        {
                            team[i]->changestate(PRE_SET);
                            printf("blocker prespike\n");
                        }
                    }
                    else
                    {
                        team[i]->changestate(PRE_BLOCK);
                        printf("blocker preblock %f \n",(ball.gettary()-19)*(team[i]->getposy()-19));
                    }
                    //team[i]->changestate(NOTHING);



                }
                break;
            }
        }

    }

}
void objmoving(object *a,int Dt)
{
    int dt=1000;
    a->changepos(a->getposx()+1.0*a->getvx()*Dt/dt,a->getposy()+1.0*a->getvy()*Dt/dt,a->getposz()+1.0*a->getvz()*Dt/dt);
}
void teammoving(object *team[],int Dt,int l)
{
    for(int i=1;i<l;i++)
    {
        objmoving(team[i],Dt);
    }
}
void originsetting(object *team[],int l,int c)
{
    float rota_pos[12][3];
    rota_pos[0][0]=3;
    rota_pos[0][1]=13;
    rota_pos[0][2]=-4;
    rota_pos[1][0]=3;
    rota_pos[1][1]=16;
    rota_pos[1][2]=-4;
    rota_pos[2][0]=0;
    rota_pos[2][1]=16;
    rota_pos[2][2]=-4;
    rota_pos[3][0]=-3;
    rota_pos[3][1]=16;
    rota_pos[3][2]=-4;
    rota_pos[4][0]=-3;
    rota_pos[4][1]=13;
    rota_pos[4][2]=-4;
    rota_pos[5][0]=0;
    rota_pos[5][1]=13;
    rota_pos[5][2]=-4;

    rota_pos[6][0]=-3;
    rota_pos[6][1]=25;
    rota_pos[6][2]=-4;
    rota_pos[7][0]=-3;
    rota_pos[7][1]=22;
    rota_pos[7][2]=-4;
    rota_pos[8][0]=0;
    rota_pos[8][1]=22;
    rota_pos[8][2]=-4;
    rota_pos[9][0]=3;
    rota_pos[9][1]=22;
    rota_pos[9][2]=-4;
    rota_pos[10][0]=3;
    rota_pos[10][1]=25;
    rota_pos[10][2]=-4;
    rota_pos[11][0]=0;
    rota_pos[11][1]=25;
    rota_pos[11][2]=-4;
    for(int i=0;i<l;i++)
    {
        team[i]->changesize(85);
        if(c==1)
        {
            team[i]->changerotation(i);
            team[i]->changepos(rota_pos[i][0],rota_pos[i][1],rota_pos[i][2]);
        }
        else if(c==2)
        {
            team[i]->changerotation(i+6);
            team[i]->changepos(rota_pos[i+6][0],rota_pos[i+6][1],rota_pos[i+6][2]);
        }


    }
}
void randtar(object *team[],int l,int c,int sta)
{
    for(int i=0;i<l;i++)
    {
        if(team[i]->getstate()==RECEIVE ||team[i]->getstate()==NOTHING)
        {
            if(c==1) team[i]->settar(1.5-3+3*((rand()*rand())%100)/100.0,16+3*(rand()%100)/100.0,-4);//(-4.5+9*((rand()*rand())%10000)/10000.0,21+7*(rand()%10000)/10000.0,-4);
            else if(c==2) team[i]->settar(-1.5-3+3*((rand()*rand())%100)/100.0,19+3*(rand()%100)/100.0,-4);//(-1.5,17.5,-4);
        }
        else if(team[i]->getstate()==PRE_SET || team[i]->getstate()==SET )
        {
            if(c==1)
            {
                if(sta==FOUR) team[i]->settar(-4.5+1*((rand()*rand())%100)/100.0,18,-4+2);
                if(sta==BACK) team[i]->settar(4.5-1*((rand()*rand())%100)/100.0,18,-4+2);
                if(sta==TWO) team[i]->settar(-0.5+1*((rand()*rand())%100)/100.0,18,-4+2);
                if(sta==AQUICK) team[i]->settar(team[i]->getposx()-1.5+-0.5+1*((rand()*rand())%100)/100.0,18,-4+2);
            }
            else if(c==2)
            {
                if(sta==FOUR) team[i]->settar(4.5-1*((rand()*rand())%100)/100.0,20,-4+2);
                if(sta==BACK) team[i]->settar(-4.5+1*((rand()*rand())%100)/100.0,20,-4+2);
                if(sta==TWO) team[i]->settar(0.5-1*((rand()*rand())%100)/100.0,20,-4+2);
                if(sta==AQUICK) team[i]->settar(team[i]->getposx()+1.5+0.5-1*((rand()*rand())%100)/100.0,20,-4+2);
            }
        }
        else if(team[i]->getstate()==PRE_SPIKE || team[i]->getstate()==SPIKE)
        {
            if(c==1) team[i]->settar(-4.5+9*((rand()*rand())%100)/100.0,28-7*(rand()%100)/100.0,-4);
            else if(c==2) team[i]->settar(4.5-9*((rand()*rand())%100)/100.0,10+7*(rand()%100)/100.0,-4);
        }
        else if(team[i]->getstate()==SERVE)
        {
            if(c==1) team[i]->settar(-4.5+9*((rand()*rand())%100)/100.0,21+7*(rand()%100)/100.0,-4);
            else if(c==2) team[i]->settar(-4.5+9*((rand()*rand())%100)/100.0,10+7*(rand()%100)/100.0,-4);
        }
        else if(team[i]->getstate()==BLOCK || team[i]->getstate()==PRE_BLOCK)
        {
            if(c==1) team[i]->settar(-4.5+9*((rand()*rand())%100)/100.0,28-7*(rand()%100)/100.0,-4);
            else if(c==2) team[i]->settar(4.5-9*((rand()*rand())%100)/100.0,10+7*(rand()%100)/100.0,-4);
        }
        if(team[i]->gettary()==19)
        {
            randtar(team,l,c,sta);
        }
    }
}
void playerfalling(object *team[],int l,int Dt)
{
    for(int i=0;i<l;i++)
    {
        if(team[i]->getposz()>-4) team[i]->changev(team[i]->getvx(),team[i]->getvy(),team[i]->getvz()-9.8*Dt/dt);
    }
}
void playerboundary(object *team[],int l,int Dt)
{
    for(int i=0;i<l;i++)
    {
        if(team[i]->getposz()<-4)
        {
            team[i]->changepos(team[i]->getposx()-team[i]->getvx()*Dt/dt,team[i]->getposy()-team[i]->getvy()*Dt/dt,-4);
            team[i]->changev(team[i]->getvx(),team[i]->getvy(),0);
        }
    }
}
void rotation(object *team[],int l)
{
    int mini=999;
    int ind=-1;
    for(int i=0;i<l;i++)
    {
        if(team[i]->getrotation()<mini)
        {
            mini=team[i]->getrotation();
            ind=i;
        }
    }
    for(int i=0;i<l;i++)
    {
        if(i==ind)
        {
            team[i]->changerotation(team[i]->getrotation()+5);
        }
        else
        {
            team[i]->changerotation(team[i]->getrotation()-1);
        }
    }
}
void resetstate()
{
    for(int i=0;i<l_team1;i++)
    {
        team1[i]->changestate(NOTHING);
    }
    for(int i=0;i<l_team2;i++)
    {
        team2[i]->changestate(NOTHING);
    }
}
void teamcountreset(object *team[],int *teamcount)
{
    if(*teamcount!=0 && (team[0]->getposy()-19)*(ball.gettary()-19)<0)
    {
        *teamcount=0;
        printf("ball change\n");
    }
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
            /*
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
            }*/

			//Event handler
			SDL_Event e;


			//set randon
			srand(time(NULL));
            int t=0;
            int a=0;
            int b=0;
            int c=0;
            int score1=0;
            int score2=0;
            int serve_right=1;
            bool start = false;
            bool stop = false;
            int Team1count=0;
            int Team2count=0;
            int team1sta=0;
            int team2sta=0;
            int* team1count;
            int finishtime=-1000;
            team1count=&Team1count;
            int* team2count;
            team2count=&Team2count;
            int sta=0;

            int Dt;
            timer.start();
            ball.changesize(18);
            //ball.changepos(3.5,19.1,1);
            /*
            player1.changesize(96);
            player1.changepos(-1.5,11,-4);
            player1.settar(-4,25,-4);
            player1.changerotation(6);
            player3.changesize(96);
            player3.changepos(-1.5,25,-4);
            player3.settar(-1.5,11,-4);
            player3.changerotation(7);
            player2.changesize(96);
            player2.changepos(1.5,25,-4);
            player2.settar(-1.5,11,-4);
            player2.changerotation(11);
            */
            originsetting(team1,l_team1,1);
            player3.changeposition(SETTER);
            player2.changeposition(MIDDLEBLOCKER);
            player5.changeposition(MIDDLEBLOCKER);
            player6.changeposition(OPPOSITEHITTER);
            player9.changeposition(SETTER);
            player8.changeposition(MIDDLEBLOCKER);
            player11.changeposition(MIDDLEBLOCKER);
            player12.changeposition(OPPOSITEHITTER);
            //player8.changestate(SPIKE);
            originsetting(team2,l_team2,2);

            //ball.settar(player8.getposx(),player8.getposy(),-4);
            //ball.changepos(player8.getposx(),player8.getposy(),5);

            //testobj.changepos(0,10,-4);
            net.changepos(0,19,-4);
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
                            a=0;
                            b=1;
                            c=0;
                            if(player1.getposz()==-4) player1.changev(4*a,4*b,4*c);
                            break;
                        case SDLK_DOWN:
                            a=0;
                            b=-1;
                            c=0;
                            if(player1.getposz()==-4) player1.changev(4*a,4*b,4*c);
                            break;
                        case SDLK_RIGHT:
                            a=1;
                            b=0;
                            c=0;
                            if(player1.getposz()==-4) player1.changev(4*a,4*b,4*c);
                            break;
                        case SDLK_LEFT:
                            a=-1;
                            b=0;
                            c=0;
                            if(player1.getposz()==-4) player1.changev(4*a,4*b,4*c);
                            break;
                        case SDLK_SPACE:
                            ball.changepos(player1.getposx(),player1.getposy(),player1.getposz()+5);
                            ball.settar(player1.getposx(),player1.getposy(),-4);
                            ball.changev(0,0,2);
                            //printf("%f,%f,%f\n",player1.getposx(),player1.getposy(),player1.getposz());
                            break;
                        case SDLK_c:
                            printf("    %d:%d\n",score1,score2);
                            break;
                        case SDLK_v:
                            //start=true;
                            //ball.changepos(player1.getposx(),player1.getposy(),0);
                            printf("%d\n",team1[2]->getstate());
                            break;
                        case SDLK_a:
                            team1sta=FOUR;
                            break;
                        case SDLK_s:
                            team1sta=TWO;
                            break;
                        case SDLK_d:
                            team1sta=BACK;
                            break;
                        case SDLK_f:
                            team1sta=AQUICK;
                            break;
                        case SDLK_l:
                            team2sta=FOUR;
                            break;
                        case SDLK_k:
                            team2sta=TWO;
                            break;
                        case SDLK_j:
                            team2sta=BACK;
                            break;
                        case SDLK_o:
                            team2sta=AQUICK;
                        case SDLK_r:
                            sta=RANDOM;

					    }
					}
					else
                    {
                        a=0;
                        b=0;
                        c=0;
                        //if(player1.getposz()==-4) player1.changev(4*a,4*b,4*c);
                    }
                }
                if(sta==RANDOM)
                {
                    if((ball.gettary()-19)*(ball.getposy()-19)<0)
                    {
                        team1sta=rand()%4;
                        team2sta=rand()%4;
                        if(team1sta==BACK) team1sta=FOUR;
                        if(team2sta==BACK) team2sta=FOUR;
                    }
                }
                //game logic
                if(timer.getTicks()-finishtime>1000)
                {
                    if(!start)
                {
                    if(ball.getvz()>0)
                    {
                        start=true;
                    }
                    if(serve_right==1)
                    {
                        for(int i=0;i<l_team1;i++)
                        {
                            if(team1[i]->getrotation()==0)
                            {
                                team1[i]->changestate(SERVE);
                                ball.changev(0,0,0);
                                ball.settar(team1[i]->getposx(),team1[i]->getposy()+0.1,-4);
                                ball.changepos(team1[i]->getposx(),team1[i]->getposy()+0.1,-4+1.4);
                            }
                        }

                    }
                    else if(serve_right==2)
                    {
                        for(int i=0;i<l_team2;i++)
                        {
                            if(team2[i]->getrotation()==6)
                            {
                                team2[i]->changestate(SERVE);
                                ball.changev(0,0,0);
                                ball.settar(team2[i]->getposx(),team2[i]->getposy()-0.1,-4);
                                ball.changepos(team2[i]->getposx(),team2[i]->getposy()-0.1,-4+1.4);
                            }
                        }
                    }

                }
                if(stop)
                {
                    static int last_serve_right=1;
                    if(serve_right==last_serve_right)
                    {
                        resetstate();
                    }
                    else if(serve_right==1)
                    {
                        rotation(team1,l_team1);
                        resetstate();
                    }
                    else if(serve_right==2)
                    {
                        rotation(team2,l_team2);
                        resetstate();
                    }
                    last_serve_right=serve_right;
                    stop=false;
                    start=false;
                }
                }

                Dt=timer.getTicks()-t;
                t=timer.getTicks();
                if(ball.getposz()>-4)ball.changev(ball.getvx(),ball.getvy(),ball.getvz()-9.8*Dt/dt);
                playerfalling(team1,l_team1,Dt);
                playerfalling(team2,l_team2,Dt);
                objmoving(&ball,Dt);
                teamcountreset(team1,team1count);
                teamcountreset(team2,team2count);
                srand(timer.getTicks());
                /*
                player1.settar(-4.5+9*((rand()*rand())%10000)/10000.0,21+7*(rand()%10000)/10000.0,-4);
                player2.settar(-4.5+9*((rand()*rand())%10000)/10000.0,19-2-7*(rand()%10000)/10000.0,-4);
                player3.settar(-4.5+9*((rand()*rand())%10000)/10000.0,19-2-7*(rand()%10000)/10000.0,-4);
                */
                randtar(team1,l_team1,1,team1sta);
                randtar(team2,l_team2,2,team2sta);
                playerhandle(team1,Dt,l_team1,team1sta);
                playerhandle(team2,Dt,l_team2,team2sta);

                AIchangev(team2,l_team2,team2sta);
                AIchangev(team1,l_team1,team1sta);
                if(!stop|| timer.getTicks()-finishtime<1000)
                {
                    objmoving(&player1,Dt);
                    objmoving(&player2,Dt);
                    objmoving(&player3,Dt);
                    objmoving(&player4,Dt);
                    objmoving(&player5,Dt);
                    objmoving(&player6,Dt);
                    objmoving(&player7,Dt);
                    objmoving(&player8,Dt);
                    objmoving(&player9,Dt);
                    objmoving(&player10,Dt);
                    objmoving(&player11,Dt);
                    objmoving(&player12,Dt);
                    //teammoving(team1,l_team1,Dt);
                    //teammoving(team2,l_team2,Dt);
                }

                //ball boundary condition
                if(ball.getposz() <= -4)
                {
                    ball.changepos(ball.getposx()-1.0*ball.getvx()*Dt/dt,ball.getposy()-1.0*ball.getvy()*Dt/dt,ball.getposz()-1.0*ball.getvz()*Dt/dt);
                    ball.changev(0.6*ball.getvx(),0.6*ball.getvy(),-0.6*ball.getvz());
                    if(start&&!stop)
                    {
                        if(ball.getposy()<19)
                    {
                        score2++;
                        serve_right=2;
                    }
                    else if(ball.getposy()>19)
                    {
                        score1++;
                        serve_right=1;
                    }
                    printf("    %d:%d\n",score1,score2);
                    *team1count=0;
                    *team2count=0;
                    finishtime=timer.getTicks();
                    stop = true;
                    start =false;
                    }

                }
                if(ball.getposy() >= 19-0.1 && ball.getposy() <= 19+0.1 && ball.getposx() <= 4.5 && ball.getposx() >=-4.5 && ball.getposz()<=-4+2.43 && ball.getposz()>=-4+1.2)
                {
                    if( ball.getposz()<=-4+2.43)
                    {
                        ball.changepos(ball.getposx()-1.0*ball.getvx()*Dt/dt,ball.getposy()-1.0*ball.getvy()*Dt/dt,ball.getposz()-1.0*ball.getvz()*Dt/dt);
                        ball.changev(0.6*ball.getvx(),0.6*ball.getvy(),-0.6*ball.getvz());
                    }
                    else
                    {
                        ball.changepos(ball.getposx()-1.0*ball.getvx()*Dt/dt,ball.getposy()-1.0*ball.getvy()*Dt/dt,ball.getposz()-1.0*ball.getvz()*Dt/dt);
                        ball.changev(0.6*ball.getvx(),-0.6*ball.getvy(),0.6*ball.getvz());
                    }
                }
                if(ball.getposx() >= 9 || ball.getposx() <= -9 )
                {
                    ball.changepos(ball.getposx()-1.0*ball.getvx()*Dt/dt,ball.getposy()-1.0*ball.getvy()*Dt/dt,ball.getposz()-1.0*ball.getvz()*Dt/dt);
                    ball.changev(-0.6*ball.getvx(),0.6*ball.getvy(),0.6*ball.getvz());
                }
                if(ball.getposy() >= 28+9 ||ball.getposy() <= 1 )
                {
                    ball.changepos(ball.getposx()-1.0*ball.getvx()*Dt/dt,ball.getposy()-1.0*ball.getvy()*Dt/dt,ball.getposz()-1.0*ball.getvz()*Dt/dt);
                    ball.changev(0.6*ball.getvx(),-0.6*ball.getvy(),0.6*ball.getvz());
                }
                playerboundary(team1,l_team1,Dt);
                playerboundary(team2,l_team2,Dt);
                //player contact ball
                if(!stop )
                {
                    playball(team1,l_team1,Dt,team1count,team1count,team2count,team1sta);
                    playball(team2,l_team2,Dt,team2count,team1count,team2count,team2sta);
                }

                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0xFF );
                SDL_RenderClear( gRenderer );
//Render current frame
                background2.render(0,0);
                background.render(0,-5);

                //gTextTexture.render( ( SCREEN_WIDTH - gTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gTextTexture.getHeight() ) / 2 );
                //render the picture from larger posy to smaller
                LS(obj);
                for(int i=0;i<len;i++)
                {
                    obj[index[i]]->render();
                }

                //net.render();
                //testobj.render();
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
