
/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <math.h>
int t=0;



//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexturebackground = NULL;
SDL_Texture* gTexturebutton = NULL;
SDL_Texture* gTexturetop = NULL;
SDL_Texture* gTexturetopcover = NULL;
SDL_Texture* gTexturenumber1 = NULL;
SDL_Texture* gTexturenumber2 = NULL;
SDL_Texture* gTexturenumber3 = NULL;
SDL_Texture* gTexturenumber4 = NULL;
SDL_Texture* gTexturenumber5 = NULL;
SDL_Texture* gTexturenumber6 = NULL;
SDL_Texture* gTextureaidice1 = NULL;
SDL_Texture* gTextureaidice2 = NULL;
SDL_Texture* gTextureaidice3 = NULL;
SDL_Texture* gTextureplayerdice1 = NULL;
SDL_Texture* gTextureplayerdice2 = NULL;
SDL_Texture* gTextureplayerdice3 = NULL;
int answer=0;
int guess=0;
int playerdicenumber[3]={0,0,0};
int aidicenumber[3]={0,0,0};
int total[6]={0,0,0,0,0,0};
int aitotal[6]={0,0,0,0,0,0};
int playertotal[6]={0,0,0,0,0,0};
int stage0=true;
int stage1=false;
int stage1_2=false;
int stage1_3=false;
int stage2=false;
int stage2_2=false;
int stage3=false;
int stage4=false;
int a=0;
int b=0;
int low=1;
int c_3_b=0;
int input=0;
int input2=0;
int inputing=0;
int inputing2=0;
char choice=0;

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
		gWindow = SDL_CreateWindow( "Liar's Dice", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load texture
	gTexturebackground = loadTexture( "background.png" );
	if( gTexturebackground == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gTexturebutton = loadTexture( "button.png" );
	if( gTexturebutton == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gTexturetop = loadTexture( "top.png" );
	if( gTexturetop == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
    gTexturenumber1 = loadTexture( "1.png" );
	if( gTexturenumber1 == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gTexturenumber2 = loadTexture( "2.png" );
	if( gTexturenumber2 == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gTexturenumber3 = loadTexture( "3.png" );
	if( gTexturenumber3 == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gTexturenumber4 = loadTexture( "4.png" );
	if( gTexturenumber4 == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gTexturenumber5 = loadTexture( "5.png" );
	if( gTexturenumber5 == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}
	gTexturenumber6 = loadTexture( "6.png" );
	if( gTexturenumber6 == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}



	//Nothing to load
	return success;
}

void close()
{
	//Free loaded image
	SDL_DestroyTexture( gTexturebackground );
	gTexturebackground = NULL;
	SDL_DestroyTexture( gTexturebutton );
	gTexturebutton = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
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


void dicing()
{
    low=1;
    for(int i=0;i<6;i++)
    {
        total[i]=0;
    }

    srand( time(NULL) );
    if(rand()%6==1)
        {
            gTextureplayerdice1=gTexturenumber1;
            playerdicenumber[0]=1;
            total[0]+=1;
            playertotal[0]+=1;
        }
    else if(rand()%6==2)
        {
            gTextureplayerdice1=gTexturenumber2;
            playerdicenumber[0]=2;
            total[1]+=1;
            playertotal[1]+=1;
        }
    else if(rand()%6==3)
        {
            gTextureplayerdice1=gTexturenumber3;
            playerdicenumber[0]=2;
            total[2]+=1;
            playertotal[2]+=1;
        }
    else if(rand()%6==4)
        {
            gTextureplayerdice1=gTexturenumber4;
            playerdicenumber[0]=4;
            total[3]+=1;
            playertotal[3]+=1;
        }
    else if(rand()%6==5)
        {
            gTextureplayerdice1=gTexturenumber5;
            playerdicenumber[0]=5;
            total[4]+=1;
            playertotal[4]+=1;
        }
    else
        {
            gTextureplayerdice1=gTexturenumber6;
            playerdicenumber[0]=6;
            total[5]+=1;
            playertotal[5]+=1;
        }

        srand( time(NULL)+80 );
    if(rand()%6==1)
        {
            gTextureplayerdice2=gTexturenumber1;
            playerdicenumber[1]=1;
            total[0]+=1;
            playertotal[0]+=1;
        }
    else if(rand()%6==2)
        {
            gTextureplayerdice2=gTexturenumber2;
            playerdicenumber[1]=2;
            total[1]+=1;
            playertotal[1]+=1;
        }
    else if(rand()%6==3)
        {
            gTextureplayerdice2=gTexturenumber3;
            playerdicenumber[1]=2;
            total[2]+=1;
            playertotal[2]+=1;
        }
    else if(rand()%6==4)
        {
            gTextureplayerdice2=gTexturenumber4;
            playerdicenumber[1]=4;
            total[3]+=1;
            playertotal[3]+=1;
        }
    else if(rand()%6==5)
        {
            gTextureplayerdice2=gTexturenumber5;
            playerdicenumber[1]=5;
            total[4]+=1;
            playertotal[4]+=1;
        }
    else
        {
            gTextureplayerdice2=gTexturenumber6;
            playerdicenumber[1]=6;
            total[5]+=1;
            playertotal[5]+=1;
        }

        srand( time(NULL)+100 );
    if(rand()%6==1)
        {
            gTextureplayerdice3=gTexturenumber1;
            playerdicenumber[2]=1;
            total[0]+=1;
            playertotal[0]+=1;
        }
    else if(rand()%6==2)
        {
            gTextureplayerdice3=gTexturenumber2;
            playerdicenumber[2]=2;
            total[1]+=1;
            playertotal[1]+=1;
        }
    else if(rand()%6==3)
        {
            gTextureplayerdice3=gTexturenumber3;
            playerdicenumber[2]=2;
            total[2]+=1;
            playertotal[2]+=1;
        }
    else if(rand()%6==4)
        {
            gTextureplayerdice3=gTexturenumber4;
            playerdicenumber[2]=4;
            total[3]+=1;
            playertotal[3]+=1;
        }
    else if(rand()%6==5)
        {
            gTextureplayerdice3=gTexturenumber5;
            playerdicenumber[2]=5;
            total[4]+=1;
            playertotal[4]+=1;
        }
    else
        {
            gTextureplayerdice3=gTexturenumber6;
            playerdicenumber[2]=6;
            total[5]+=1;
            playertotal[5]+=1;
        }
    srand( time(NULL)+10 );
    if(rand()%6==1)
        {
            gTextureaidice1=gTexturenumber1;
            aidicenumber[0]=1;
            total[0]+=1;
            aitotal[0]+=1;
        }
    else if(rand()%6==2)
        {
            gTextureaidice1=gTexturenumber2;
            aidicenumber[0]=2;
            total[1]+=1;
            aitotal[1]+=1;
        }
    else if(rand()%6==3)
        {
            gTextureaidice1=gTexturenumber3;
            aidicenumber[0]=3;
            total[2]+=1;
            aitotal[2]+=1;
        }
    else if(rand()%6==4)
        {
            gTextureaidice1=gTexturenumber4;
            aidicenumber[0]=4;
            total[3]+=1;
            aitotal[3]+=1;
        }
    else if(rand()%6==5)
        {
            gTextureaidice1=gTexturenumber5;
            aidicenumber[0]=5;
            total[4]+=1;
            aitotal[4]+=1;
        }
    else
        {
            gTextureaidice1=gTexturenumber6;
            aidicenumber[0]=6;
            total[5]+=1;
            aitotal[5]+=1;
        }

        srand( time(NULL)+20 );
    if(rand()%6==1)
        {
            gTextureaidice2=gTexturenumber1;
            aidicenumber[1]=1;
            total[0]+=1;
            aitotal[0]+=1;
        }
    else if(rand()%6==2)
        {
            gTextureaidice2=gTexturenumber2;
            aidicenumber[1]=2;
            total[1]+=1;
            aitotal[1]+=1;
        }
    else if(rand()%6==3)
        {
            gTextureaidice2=gTexturenumber3;
            aidicenumber[1]=3;
            total[2]+=1;
            aitotal[2]+=1;
        }
    else if(rand()%6==4)
        {
            gTextureaidice2=gTexturenumber4;
            aidicenumber[1]=4;
            total[3]+=1;
            aitotal[3]+=1;
        }
    else if(rand()%6==5)
        {
            gTextureaidice2=gTexturenumber5;
            aidicenumber[1]=5;
            total[4]+=1;
            aitotal[4]+=1;
        }
    else
        {
            gTextureaidice2=gTexturenumber6;
            aidicenumber[1]=6;
            total[5]+=1;
            aitotal[5]+=1;
        }

        srand( time(NULL)+30 );
    if(rand()%6==1)
        {
            gTextureaidice3=gTexturenumber1;
            aidicenumber[2]=1;
            total[0]+=1;
            aitotal[0]+=1;
        }
    else if(rand()%6==2)
        {
            gTextureaidice3=gTexturenumber2;
            aidicenumber[2]=2;
            total[1]+=1;
            aitotal[1]+=1;
        }
    else if(rand()%6==3)
        {
            gTextureaidice3=gTexturenumber3;
            aidicenumber[2]=3;
            total[2]+=1;
            aitotal[2]+=1;
        }
    else if(rand()%6==4)
        {
            gTextureaidice3=gTexturenumber4;
            aidicenumber[2]=4;
            total[3]+=1;
            aitotal[3]+=1;
        }
    else if(rand()%6==5)
        {
            gTextureaidice3=gTexturenumber5;
            aidicenumber[2]=5;
            total[4]+=1;
            aitotal[4]+=1;
        }
    else
        {
            gTextureaidice3=gTexturenumber6;
            aidicenumber[2]=6;
            total[5]+=1;
            aitotal[5]+=1;
        }
        //printf("%d %d %d %d %d %d\n",total[0],total[1],total[2],total[3],total[4],total[5]);
}

void checkanswer()
{
    if(total[a-1]>=b) printf("telling the truth.\n");
    else printf("lying.\n");
    gTexturetopcover=NULL;
}
void aiguess()
{
    double p[6]={0,0,0,0,0,0};
    double maxp=0;
    int new_a=0;
    int new_b=0;
    c_3_b=1.0;
    for(int i=0;i<(b-aitotal[a-1]);i++)
    {
        c_3_b*=(3-i)/(i+1);
    }
    if(pow(1.0/6,(b-aitotal[a-1]))*pow(5.0/6,3-(b-aitotal[a-1]))*c_3_b<0.1&&aitotal[a-1]<=b+1)
    {
        printf("AI don't believe you.\nyou are ");
        checkanswer();
        stage2=false;
        stage0=true;
    }
    else
    {

        for(int i=0;i<6;i++)
        {
            if(i==a-1)
            {
                if(aitotal[a-1]>=b+1) p[a-1]=1;
                else p[a-1]=pow(1.0/6,(b+1-aitotal[a-1]));
            }
            else
            {
                if(aitotal[i]>=b) p[i]=1;
                else p[i]=pow(1.0/6,(b-aitotal[i]));

            }

            if(maxp<p[i])
            {
                maxp=p[i];
                new_a=i+1;
            }
        }
        if(new_a==a)
        {
            b=b+1;
            low=b;
        }
        else
        {
            a=new_a;

        }
        printf("AI:There exists %d of [%d]\nDo you think it's lying?(y/n)\n",b,a);
        stage2=false;
        stage2_2=true;

    }
}

int main( int argc, char* args[] )
{
    inputing=false;
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

			//Event handler
			SDL_Event e;

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
					    if( e.key.keysym.sym == SDLK_SPACE )
                        {
                            if(stage0)
                            {
                                stage0=false;
                                stage1=true;
                                dicing();
                                gTexturetopcover=gTexturetop;
                                printf("Please input your guess.\n");
                            }

                        }
                        else if( e.key.keysym.sym == SDLK_1 )
                        {
                            input=1;
                            inputing=true;
                        }
                        else if( e.key.keysym.sym == SDLK_2 )
                        {
                            input=2;
                            inputing=true;
                        }
                        else if( e.key.keysym.sym == SDLK_3 )
                        {
                            input=3;
                            inputing=true;
                        }
                        else if( e.key.keysym.sym == SDLK_4 )
                        {
                            input=4;
                            inputing=true;
                        }
                        else if( e.key.keysym.sym == SDLK_5 )
                        {
                            input=5;
                            inputing=true;
                        }
                        else if( e.key.keysym.sym == SDLK_6 )
                        {
                            input=6;
                            inputing=true;
                        }
                        else if( e.key.keysym.sym == SDLK_7 )
                        {
                            input=7;
                            inputing=true;
                        }
                        else if( e.key.keysym.sym == SDLK_8 )
                        {
                            input=8;
                            inputing=true;
                        }
                        else if( e.key.keysym.sym == SDLK_9 )
                        {
                            input=9;
                            inputing=true;
                        }
                        else if( e.key.keysym.sym == SDLK_0 )
                        {
                            input=0;
                            inputing=true;
                        }
                        else if( e.key.keysym.sym == SDLK_c )
                        {
                            gTexturetopcover=NULL;
                        }
                        else if( e.key.keysym.sym == SDLK_y )
                        {
                            input2=true;
                            inputing2=true;
                            printf("AI is ");
                        }
                        else if( e.key.keysym.sym == SDLK_n )
                        {
                            input2=false;
                            inputing2=true;
                        }
                        else
                        {
                            printf("Wrong input,please input again.\n");
                        }



					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//background viewport
				SDL_Rect backgroundViewport;
				backgroundViewport.x = 0;
				backgroundViewport.y = 0;
				backgroundViewport.w = SCREEN_WIDTH ;
				backgroundViewport.h = SCREEN_HEIGHT;
				SDL_RenderSetViewport( gRenderer, &backgroundViewport );

				//Render texture to screen
				SDL_RenderCopy( gRenderer, gTexturebackground, NULL, NULL );

				SDL_Rect button1Viewport;
				button1Viewport.x = SCREEN_WIDTH/3;
				button1Viewport.y = 10;
				button1Viewport.w = SCREEN_WIDTH/3 ;
				button1Viewport.h = SCREEN_WIDTH/3;
				SDL_RenderSetViewport( gRenderer, &button1Viewport );

				//Render texture to screen
				SDL_RenderCopy( gRenderer, gTexturebutton, NULL, NULL );

				SDL_Rect button2Viewport;
				button2Viewport.x = SCREEN_WIDTH/3;
				button2Viewport.y = SCREEN_HEIGHT-(SCREEN_WIDTH/3)-10;
				button2Viewport.w = SCREEN_WIDTH/3 ;
				button2Viewport.h = SCREEN_WIDTH/3;
				SDL_RenderSetViewport( gRenderer, &button2Viewport );

				//Render texture to screen
				SDL_RenderCopy( gRenderer, gTexturebutton, NULL, NULL );

				SDL_Rect playerdice1Viewport;
				playerdice1Viewport.x = SCREEN_WIDTH/2-SCREEN_WIDTH/15/2-50;
				playerdice1Viewport.y = SCREEN_HEIGHT-(SCREEN_WIDTH/3)-10+(SCREEN_WIDTH/3)/2-SCREEN_WIDTH/15/2;
				playerdice1Viewport.w = SCREEN_WIDTH/15 ;
				playerdice1Viewport.h = SCREEN_WIDTH/15;
				SDL_RenderSetViewport( gRenderer, &playerdice1Viewport );

				//Render texture to screen
				SDL_RenderCopy( gRenderer, gTextureplayerdice1, NULL, NULL );

				SDL_Rect playerdice2Viewport;
				playerdice2Viewport.x = SCREEN_WIDTH/2-SCREEN_WIDTH/15/2;
				playerdice2Viewport.y = SCREEN_HEIGHT-(SCREEN_WIDTH/3)-10+(SCREEN_WIDTH/3)/2-SCREEN_WIDTH/15/2;
				playerdice2Viewport.w = SCREEN_WIDTH/15 ;
				playerdice2Viewport.h = SCREEN_WIDTH/15;
				SDL_RenderSetViewport( gRenderer, &playerdice2Viewport );

				//Render texture to screen
				SDL_RenderCopy( gRenderer, gTextureplayerdice2, NULL, NULL );

				SDL_Rect playerdice3Viewport;
				playerdice3Viewport.x = SCREEN_WIDTH/2-SCREEN_WIDTH/15/2+50;
				playerdice3Viewport.y = SCREEN_HEIGHT-(SCREEN_WIDTH/3)-10+(SCREEN_WIDTH/3)/2-SCREEN_WIDTH/15/2;
				playerdice3Viewport.w = SCREEN_WIDTH/15 ;
				playerdice3Viewport.h = SCREEN_WIDTH/15;
				SDL_RenderSetViewport( gRenderer, &playerdice3Viewport );

				//Render texture to screen
				SDL_RenderCopy( gRenderer, gTextureplayerdice3, NULL, NULL );

				SDL_Rect aidice1Viewport;
				aidice1Viewport.x = SCREEN_WIDTH/2-SCREEN_WIDTH/15/2-50;
				aidice1Viewport.y = 10+(SCREEN_WIDTH/3)/2-SCREEN_WIDTH/15/2;
				aidice1Viewport.w = SCREEN_WIDTH/15 ;
				aidice1Viewport.h = SCREEN_WIDTH/15;
				SDL_RenderSetViewport( gRenderer, &aidice1Viewport );

				//Render texture to screen
				SDL_RenderCopy( gRenderer, gTextureaidice1, NULL, NULL );

				SDL_Rect aidice2Viewport;
				aidice2Viewport.x = SCREEN_WIDTH/2-SCREEN_WIDTH/15/2;
				aidice2Viewport.y = 10+(SCREEN_WIDTH/3)/2-SCREEN_WIDTH/15/2;
				aidice2Viewport.w = SCREEN_WIDTH/15 ;
				aidice2Viewport.h = SCREEN_WIDTH/15;
				SDL_RenderSetViewport( gRenderer, &aidice2Viewport );

				//Render texture to screen
				SDL_RenderCopy( gRenderer, gTextureaidice2, NULL, NULL );


				SDL_Rect aidice3Viewport;
				aidice3Viewport.x = SCREEN_WIDTH/2-SCREEN_WIDTH/15/2+50;
				aidice3Viewport.y = 10+(SCREEN_WIDTH/3)/2-SCREEN_WIDTH/15/2;
				aidice3Viewport.w = SCREEN_WIDTH/15 ;
				aidice3Viewport.h = SCREEN_WIDTH/15;
				SDL_RenderSetViewport( gRenderer, &aidice3Viewport );

				//Render texture to screen
				SDL_RenderCopy( gRenderer, gTextureaidice3, NULL, NULL );



				SDL_Rect topViewport;
				topViewport.x = SCREEN_WIDTH/3;
				topViewport.y = 10;
				topViewport.w = SCREEN_WIDTH/3 ;
				topViewport.h = SCREEN_WIDTH/3;
				SDL_RenderSetViewport( gRenderer, &topViewport );

				//Render texture to screen
				SDL_RenderCopy( gRenderer,gTexturetopcover, NULL, NULL );



				//Update screen
				SDL_RenderPresent( gRenderer );

				if(stage1)
                {
                    if(inputing)
                    {
                        b=input;
                        inputing=false;
                        stage1=false;
                        stage1_2=true;
                        printf("You:There exists %d",b);
                    }


                }
                if(stage1_2)
                {
                    if(inputing)
                    {
                        a=input;

                        inputing=false;
                        stage1_2=false;
                        stage1_3=true;
                        printf(" of [%d]\n",a);
                    }
                }
                if(stage1_3)
                {
                    if(b<low||b>6||a<1||a>6)
                        {
                            printf("wrong input,please input again!");
                            inputing=false;
                            stage1_3=false;
                            stage1=true;

                        }
                    else
                    {
                        if(low<b) low=b;
                        stage1_3=false;
                        stage2=true;
                    }
                }
                if(stage2)
                {

                        aiguess();


                }
                if(stage2_2)
                {
                    if(inputing2)
                    {
                        if(input2)
                        {
                            input2=false;
                            checkanswer();
                            stage2_2=false;
                            stage0=true;
                        }
                        else if(!input2)
                        {
                            stage2_2=false;
                            stage1=true;
                            printf("Please input your guess.\n");
                        }
                        inputing2=false;

                    }
                }

			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

