#include <SDL.h>
#include <iostream>
#include <math.h>

void exitProgram()
{
	std::cout << "Kil�p�shez nyomj meg egy billenty�t..." << std::endl;
	std::cin.get();
}

int main( int argc, char* args[] )
{
	atexit( exitProgram );
	//
	// 1. l�p�s: inicializ�ljuk az SDL-t
	//

	// a grafikus alrendszert kapcsoljuk csak be, ha gond van, akkor jelezz�k �s l�pj�n ki
	if ( SDL_Init( SDL_INIT_VIDEO ) == -1 )
	{
		// irjuk ki a hibat es terminaljon a program
		std::cout << "[SDL ind�t�sa]Hiba az SDL inicializ�l�sa k�zben: " << SDL_GetError() << std::endl;
		return 1;
	}
			
	//
	// 2. l�p�s: hozzuk l�tre az ablakot, amire rajzolni fogunk
	//

	SDL_Window *win = 0;
    win = SDL_CreateWindow( "Hello SDL!",				// az ablak fejl�ce
							100,						// az ablak bal-fels� sark�nak kezdeti X koordin�t�ja
							100,						// az ablak bal-fels� sark�nak kezdeti Y koordin�t�ja
							640,						// ablak sz�less�ge
							480,						// �s magass�ga
							SDL_WINDOW_SHOWN);			// megjelen�t�si tulajdons�gok

	// ha nem siker�lt l�trehozni az ablakot, akkor �rjuk ki a hib�t, amit kaptunk �s l�pj�nk ki
    if (win == 0)
	{
		std::cout << "[Ablak l�trehoz�sa]Hiba az SDL inicializ�l�sa k�zben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 3. l�p�s: hozzunk l�tre egy renderel�t, rajzol�t
	//

    SDL_Renderer *ren = 0;
    ren = SDL_CreateRenderer(	win, // melyik ablakhoz rendelj�k hozz� a renderert
								-1,  // melyik index� renderert inicializ�ljuka
									 // a -1 a harmadik param�terben meghat�rozott ig�nyeinknek megfelel� els� renderel�t jelenti
								SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);	// az ig�nyeink, azaz
																						// hardveresen gyors�tott �s vsync-et bev�r�
    if (ren == 0)
	{
        std::cout << "[Renderer l�trehoz�sa]Hiba az SDL inicializ�l�sa k�zben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 4. l�p�s: ind�tsuk el a f� �zenetfeldolgoz� ciklust
	// 

	// v�get kell-e �rjen a program fut�sa?
	bool quit = false;
	// feldolgozand� �zenet ide ker�l
	SDL_Event ev;
	// eg�r X �s Y koordin�t�i
	Sint32 mouseX = 0, mouseY = 0;

	int red, green, blue;

	red = 0;
	green = 255;
	blue = 0;

	while (!quit)
	{
		// am�g van feldolgozand� �zenet dolgozzuk fel mindet:
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

		// t�r�lj�k a h�tteret feh�rre
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		// aktu�lis rajzol�si sz�n legyen z�ld �s rajzoljunk ki egy vonalat
		SDL_SetRenderDrawColor(	ren,	// renderer c�me, aminek a rajzol�si sz�n�t be akarjuk �ll�tani
								0,		// piros
								255,	// z�ld
								0,		// k�k
								255);	// �tl�tszatlans�g

		SDL_RenderDrawLine(	ren,	// renderer c�me, ahov� vonalat akarunk rajzolni
							0, 0, // vonal kezd�pontj�nak (x,y) koordin�t�i
							mouseX, mouseY);// vonal v�gpontj�nak (x,y) koordin�t�i
		
		// defini�ljunk egy (mouseX, mouseY) k�z�ppont�, tengelyekkel p�rhuzamos oldal�
		// 20x20-as n�gyzetet:
		SDL_Rect cursor_rect;
		cursor_rect.w = sin(SDL_GetTicks() / 500.0f) * 10 + 25;
		cursor_rect.h = sin(SDL_GetTicks() / 500.0f) * 10 + 25;
		cursor_rect.x = mouseX - cursor_rect.w / 2;
		cursor_rect.y = mouseY - cursor_rect.h / 2;
		// legyen a kit�lt�si sz�n piros
		SDL_SetRenderDrawColor( ren, red, green, blue, 255 );
		SDL_RenderFillRect( ren, &cursor_rect);

		// 1. feladat: az eltelt id� f�ggv�ny�ben periodikusan n�jj�n �s cs�kkenjen
		//    az eg�rmutat� k�z�ppontj�val kirajzolt 

		// 2. feladat: ha a user a bal eg�rgombot nyomja meg akkor a t�glalap sz�ne v�ltson pirosra,
		//    ha a jobb eg�rgombot, akkor k�kre

		// 3. feladat: rajzolj ki egy 50 sugar� k�rvonalat az eg�rmutat� k�r�!
		// seg�ts�g: haszn�ld a SDL_RenderDrawLines()-t

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

		// jelen�ts�k meg a backbuffer tartalm�t
		SDL_RenderPresent(ren);
	}



	//
	// 4. l�p�s: l�pj�nk ki
	// 

	SDL_DestroyRenderer( ren );
	SDL_DestroyWindow( win );

	SDL_Quit();

	return 0;
}