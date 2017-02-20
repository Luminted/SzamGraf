#include <SDL.h>
#include <iostream>
#include <math.h>

void exitProgram()
{
	std::cout << "Kilépéshez nyomj meg egy billentyût..." << std::endl;
	std::cin.get();
}

int main( int argc, char* args[] )
{
	atexit( exitProgram );
	//
	// 1. lépés: inicializáljuk az SDL-t
	//

	// a grafikus alrendszert kapcsoljuk csak be, ha gond van, akkor jelezzük és lépjün ki
	if ( SDL_Init( SDL_INIT_VIDEO ) == -1 )
	{
		// irjuk ki a hibat es terminaljon a program
		std::cout << "[SDL indítása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
		return 1;
	}
			
	//
	// 2. lépés: hozzuk létre az ablakot, amire rajzolni fogunk
	//

	SDL_Window *win = 0;
    win = SDL_CreateWindow( "Hello SDL!",				// az ablak fejléce
							100,						// az ablak bal-felsõ sarkának kezdeti X koordinátája
							100,						// az ablak bal-felsõ sarkának kezdeti Y koordinátája
							640,						// ablak szélessége
							480,						// és magassága
							SDL_WINDOW_SHOWN);			// megjelenítési tulajdonságok

	// ha nem sikerült létrehozni az ablakot, akkor írjuk ki a hibát, amit kaptunk és lépjünk ki
    if (win == 0)
	{
		std::cout << "[Ablak létrehozása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 3. lépés: hozzunk létre egy renderelõt, rajzolót
	//

    SDL_Renderer *ren = 0;
    ren = SDL_CreateRenderer(	win, // melyik ablakhoz rendeljük hozzá a renderert
								-1,  // melyik indexú renderert inicializáljuka
									 // a -1 a harmadik paraméterben meghatározott igényeinknek megfelelõ elsõ renderelõt jelenti
								SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);	// az igényeink, azaz
																						// hardveresen gyorsított és vsync-et beváró
    if (ren == 0)
	{
        std::cout << "[Renderer létrehozása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 4. lépés: indítsuk el a fõ üzenetfeldolgozó ciklust
	// 

	// véget kell-e érjen a program futása?
	bool quit = false;
	// feldolgozandó üzenet ide kerül
	SDL_Event ev;
	// egér X és Y koordinátái
	Sint32 mouseX = 0, mouseY = 0;

	int red, green, blue;

	red = 0;
	green = 255;
	blue = 0;

	while (!quit)
	{
		// amíg van feldolgozandó üzenet dolgozzuk fel mindet:
		while ( SDL_PollEvent(&ev) )
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if ( ev.key.keysym.sym == SDLK_ESCAPE )
					quit = true;
				break;
			case SDL_MOUSEMOTION:
				mouseX = ev.motion.x;
				mouseY = ev.motion.y;
				break;
			case SDL_MOUSEBUTTONUP:
				if (ev.button.button == SDL_BUTTON_LEFT) {
					red = 255;
					green = 0;
					blue = 0;
				}
				if (ev.button.button == SDL_BUTTON_RIGHT) {
					red = 0;
					green = 0;
					blue = 255;
				}
				break;
			}
		}

		// töröljük a hátteret fehérre
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		// aktuális rajzolási szín legyen zöld és rajzoljunk ki egy vonalat
		SDL_SetRenderDrawColor(	ren,	// renderer címe, aminek a rajzolási színét be akarjuk állítani
								0,		// piros
								255,	// zöld
								0,		// kék
								255);	// átlátszatlanság

		SDL_RenderDrawLine(	ren,	// renderer címe, ahová vonalat akarunk rajzolni
							0, 0, // vonal kezdõpontjának (x,y) koordinátái
							mouseX, mouseY);// vonal végpontjának (x,y) koordinátái
		
		// definiáljunk egy (mouseX, mouseY) középpontó, tengelyekkel párhuzamos oldalú
		// 20x20-as négyzetet:
		SDL_Rect cursor_rect;
		cursor_rect.w = sin(SDL_GetTicks() / 500.0f) * 10 + 25;
		cursor_rect.h = sin(SDL_GetTicks() / 500.0f) * 10 + 25;
		cursor_rect.x = mouseX - cursor_rect.w / 2;
		cursor_rect.y = mouseY - cursor_rect.h / 2;
		// legyen a kitöltési szín piros
		SDL_SetRenderDrawColor( ren, red, green, blue, 255 );
		SDL_RenderFillRect( ren, &cursor_rect);

		// 1. feladat: az eltelt idõ függvényében periodikusan nõjjön és csökkenjen
		//    az egérmutató középpontjával kirajzolt 

		// 2. feladat: ha a user a bal egérgombot nyomja meg akkor a téglalap színe váltson pirosra,
		//    ha a jobb egérgombot, akkor kékre

		// 3. feladat: rajzolj ki egy 50 sugarú körvonalat az egérmutató köré!
		// segítség: használd a SDL_RenderDrawLines()-t

		SDL_Point points[1000];
		const int N = 1000;
		int R = 50;
		for (int i = 0; i < N; ++i) {
			float angle = 2 * M_PI / N * i;
			float c = cos(angle);
			float s = sin(angle);
			points.x = c;
			points.y = s;
		}

		SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
		SDL_RenderDrawLines(ren, points, N + 1);

		// jelenítsük meg a backbuffer tartalmát
		SDL_RenderPresent(ren);
	}



	//
	// 4. lépés: lépjünk ki
	// 

	SDL_DestroyRenderer( ren );
	SDL_DestroyWindow( win );

	SDL_Quit();

	return 0;
}