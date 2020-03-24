// Jeu Pong
// Raphael Thibierge
// Aubin Puyoou
// Verison finale 1.0 finit le 29 octobre 2014

#include<iostream>
#include<cstdlib>
#include<string>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h" // inclusion de la gestion de l'affichage des fontes
#include<sstream> // pour conversion du score


using namespace std;

struct Balle
{	// création d'une structure balle pour faciliter sa gestion
	SDL_Surface *image ; // image de la balle
	int x ; // position x de la balle
	int y ; // position y de la balle
	int vitesseX ; // vitesse horizontale
	int vitesseY ; // vitesse verticale
	int hauteur ; // hauteur de la balle pour faciliter la gestion des collisions
} ;


struct Joueur
{	// structure de la barre pour faciliter sa gestion
	SDL_Surface *image ; // image de la raquette
	int x ; // position x de la raquette
	int y ; // position y de la la raquette
	int vitesse ; // vitesse de déplacement de la raquette
	int largeur ; // largeur de la raquette pour faciliter la gestion des colisions
	int hauteur ; // de même pour la hauteur
	int score ; // score du joueur
	int numero ; // numéro du joueur
};


void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
  SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface( source, clip, destination, &offset );
}


void balleAuCentre(Balle & balle, int hauteurEcran, int largeurEcran)
{ // fonction qui remet la balle au centre
	//la fonction remet la balle au centre
	balle.x = largeurEcran/2 ;
	balle.y = hauteurEcran/2 ;
}


void affichage(SDL_Surface *screen, SDL_Surface *background,
	Balle & balle, Joueur joueur1, Joueur joueur2)
	{ // fontion s'occupat de tout afficher d'un coup
    SDL_FillRect(screen,&screen->clip_rect, SDL_MapRGB(screen->format,255,0,0));
    apply_surface(0,0,background, screen, NULL); // image de fond
    apply_surface(balle.x, balle.y, balle.image, screen, NULL) ; // balle
    apply_surface(joueur1.x, joueur1.y, joueur1.image, screen, NULL) ; // barre du joueur 1
    apply_surface(joueur2.x, joueur2.y, joueur2.image, screen, NULL) ; // barre du joueur 2
	}


bool collision(Balle balle, Joueur joueur)
{ // gestion des collision entre la balle et la barre du joueur
	bool reponse = false ; // on considere d'abbord qu'il n'y a pas collision
	if ((balle.x + balle.hauteur >= joueur.x) & (balle.x <= joueur.x+joueur.largeur))
	// si la balle se situe au même niveau horizontal (abscisse) que la barre du joueur
	{
		if ((balle.y + balle.hauteur>= joueur.y) & (balle.y <= joueur.y + joueur.hauteur))
			// si la balle se sittue au même niveau vertical (ordonée) que le joueur
			reponse = true ; // il y a collision
	}
	return reponse ;
}


void barreEspace(Joueur &joueur1, Joueur &joueur2,bool &play, bool &pause,
ostringstream &messageJ1, ostringstream &messageJ2, int limiteScore)
{ // fonction gérant le rôle de la barre espace durant le jeu
    if ((joueur1.score < limiteScore) && (joueur2.score < limiteScore) && (play == true))
    // si le score du joueur 1 et du joueur 2 sont inférieur à la limite de score et qu'une partie est en cours :
        pause = false ; // on désactive le mode pause pour continuer à joueur et relancer la balle
    else {
        play = false ; // on arrete la partie
        joueur1.score = 0 ; // réinitialisation du score du joueur 1
        messageJ1.flush() ;
        messageJ1.str("") ; // On convertie le message affichant le score en chaîne de charactère
        messageJ1 << joueur1.score ; // on introduit la valeur du score dans la variable message
        joueur2.score = 0 ; // réinitialisatio du score du joueur 2
        messageJ2.flush() ;
        messageJ2.str("") ; // On convertie le message affichant le score en chaîne de charactère
        messageJ2 << joueur2.score ; // on introduit la valeur du score dans la variable message
        SDL_Delay(100) ; // permet d'attendre le temps que la barre d'espace soit relachée afin de différencier
        // dans le but de séparer la réinitialisation du score et le lancé e la balle

        }
}


void score(Joueur &joueur,ostringstream &message, int limiteScore)
{ // gestion du score des joueurs, en ajoutant un point à son score et en introduisant la valeur de son score
//dans la variable message qui est affiché à l'écran
    if (joueur.score < limiteScore ) { // si le score du joueur est inférieur à la limite de score
        joueur.score++ ; // score du joueur ayant gagné le point augmente de 1 point
        message.flush() ;
        message.str("") ; // On convertie le message affichant le score en chaîne de charactère
        message << joueur.score ; // on introduit la valeur du score dans la variable message

	}
}


void mouvementBalle(Balle &balle, Joueur &joueur1, Joueur &joueur2, int hauteurEcran, int largeurEcran,
	bool &pause, ostringstream &messageJ1, ostringstream &messageJ2, int limiteScore)
{ // Fonction gérant le déplacement de la balle
	if (collision(balle, joueur2) || collision(balle, joueur1))
	// si collision avec une des barres, la vitesse horizontale de la balle
		balle.vitesseX *= -1 ; // s'inverse pour repartir dans l'autre sens

	if ((balle.y<=0) or (balle.y+balle.hauteur>=hauteurEcran))
	// si la balle touche le haut ou le bas de l'écran, la vitesse verticale de la balle
		balle.vitesseY *= -1 ; // s'inverse pour repartir dans l'autre sens

	if (balle.x < 0-balle.hauteur) //si la balle sort du côté du joueur 1
	{	// si la balle sort de l'écran du côté du joueur 1
		pause = true ; // on met le jeu sur pause
		score(joueur2, messageJ2, limiteScore) ; // on met à jour le score du joueur 2
		balleAuCentre(balle, hauteurEcran, largeurEcran) ; // on remet la balle au centre
		//balleAuCentre(balle, hauteurEcran, largeurEcran) ;
	}

	else if (balle.x > largeurEcran) // si la balle sort du côté du joueur 2
	{	// si la balle sort de l'écran du côté du joueur 2
		pause=true ; // on met le jeu sur pause
		score(joueur1, messageJ1, limiteScore) ; // on met à jour le score du joueur 1
		balleAuCentre(balle, hauteurEcran, largeurEcran) ; // on remet la balle au centre
	}

	if (pause==false) // si le jeu n'est pas en pause
	{
		balle.x += balle.vitesseX ; // on ajoute la vitesse horizontale de la balle à son abscisse pour la déplacer
		balle.y += balle.vitesseY ; // on ajoute la vitesse verticale de la balle à son ordonnée pour la déplacer
	}
}


void barreHaut(Joueur & joueur)
{ // fonction qui fait monter une barre
	if (joueur.y > joueur.vitesse) // si la barre peut monter sans passer au dessus de l'écran
		joueur.y -= joueur.vitesse ; // on fait monter la barre
	else joueur.y = 0 ; // sinon on la place tout en haut de l'écran
}


void barreBas(Joueur & joueur, int hauteurEcran)
{ // fonction qui fait descendre une barre
	if (joueur.y > hauteurEcran-joueur.hauteur-joueur.vitesse) // si la barre peut descendre passer au dessous de l'écran
		joueur.y = hauteurEcran-joueur.hauteur ; // on fait descendre la balle
	else joueur.y += joueur.vitesse ; // sinon on la place tout en bas de l'écran
}


void showMessageScreen(string message,int x,int y,
          TTF_Font *font,int fontSize,SDL_Color textColor,SDL_Surface* &screen)
{ // fonction  pour afficher les messages à l'écran qui nous à été donnée donc nous ne la commentons pas
  string mot="";
  string space=" ";
  int i=0,j;
  SDL_Surface *mes=NULL;

  j = message.find(space);
  while( j != string::npos )
    {
      mot=message.substr(i,j-i);
      if(mot != "")
    {
      mes=TTF_RenderText_Solid(font,mot.c_str(),textColor);
      apply_surface(x,y,mes,screen, NULL);
      x+=mes->w;
      SDL_FreeSurface(mes);
    }
      x+=fontSize;
      i=j+1;
      j = message.find(space,i);
    }

  mot=message.substr(i);
  mes=TTF_RenderText_Solid(font,mot.c_str(),textColor);
  apply_surface(x,y,mes,screen, NULL);
  SDL_FreeSurface(mes);
}


int main(int argc, char* argv[])
{
//-----------------------------------------------------------------------------
//		Initialisation des variables essentielles au fonctionnement du jeu
//-----------------------------------------------------------------------------
	bool quit = false ; // variable qui dit par la suite si on doit quiter le jeu ou non
	bool pause = true ; // variable qui dit par la suite sur le jeu est sur pause ou non
	bool play = false ; // variable qui dit par la suite si on lance la partie ou non
	const int limiteScore = 10 ; // limite de score d'une partie
	SDL_Surface *screen ; // création de la surface screen dans laquelle tout sera affiché
	SDL_Surface *background ; // création de la surface qui contiendra l'image de fond
	SDL_Surface *title ; // surface correspondant à l'image de fond du menu
	SDL_Init(SDL_INIT_EVERYTHING); // Initialisation de SDL
	SDL_Event event ; // création de la variable qui va gérer les évènement
	Uint8 *keystates = SDL_GetKeyState (NULL) ; // création de la variable qui détermine l'état des touches du clavier

	//---------------------------------------------------------Ecran
    const int largeurEcran = 640; // on définit la largeur de l'écran
    const int hauteurEcran = 400; // on définit la hauteur de l'écran
    screen=SDL_SetVideoMode(largeurEcran,hauteurEcran,32,SDL_SWSURFACE) ;// initialisation de screen
	SDL_WM_SetCaption("Pong", NULL ) ; // on nome la fenêtre "Pong"
    background = SDL_LoadBMP("fond.bmp") ; //Mise en place du fond
    title = SDL_LoadBMP("title.bmp") ; //Mise en place du fond du menu
	//---------------------------------------------------------Transparence
	Uint32 colorkey = SDL_MapRGB( background->format, 0, 255, 255 ); // on définit la couleur qu'il faudra
	// rendre transparente dans les images qu'on va charger
	//---------------------------------------------------------Ecran de démarage et boutons
	SDL_Surface *bouton ; // surface qui contiendra l'image des boutons
	bouton = SDL_LoadBMP("button.bmp") ; // chargement de l'image des boutons
	SDL_SetColorKey(bouton, SDL_SRCCOLORKEY, (0,0,0)) ;
	const int boutonX = (largeurEcran/2)-80 ; // on définit l'abscisse des boutons Play et Quit
	const int boutonY1 = 230 ; // on définit l'ordonnée du bouton Play
	const int boutonY2 = 310 ; // on définit l'odronée du bouton Quit
	const int boutonW = 150 ; // on définit la largeur des boutons
	const int boutonH = 60 ; // on définit la hauteur des boutons

    SDL_Rect boutonPlay ; // on crée un rectangle qui contient le bouton Play
    SDL_Rect boutonQuit ; // on crée un rectangle qui contient le bouton Quit

	SDL_Rect boutonQuitRouge ; // on crée le rectangle qui contient le bouton Quit rouge
    boutonQuitRouge.x = 0 ; // l'abscisse de début de ce bouton dans l'image est le pixel 0
	boutonQuitRouge.y = 0 ; // l'ordonnée de début de ce bouton dans l'image est le pixel 0
	boutonQuitRouge.w = boutonW ; // la largeur de ce bouton est la largeur standart de tous les boutons
	boutonQuitRouge.h = boutonH ; // la hauteur de ce bouton est la hauteur standart de tous les boutons

	SDL_Rect boutonQuitJaune ; // on crée le rectangle qui contient le bouton Quit Jaune
	boutonQuitJaune.x = 200 ; // l'abscisse de début de ce bouton dans l'image est le pixel 200
	boutonQuitJaune.y = 0 ; // l'ordonnée de début de ce bouton dans l'image est le pixel 0
	boutonQuitJaune.w = boutonW ; // la largeur de ce bouton est la largeur standart de tous les boutons
	boutonQuitJaune.h = boutonH ; // la hauteur de ce bouton est la hauteur standart de tous les boutons

	SDL_Rect boutonPlayRouge ; // on crée le rectangle qui contient le bouton Play rouge
	boutonPlayRouge.x = 0 ; // l'abscisse de début de ce bouton dans l'image est le pixel 0
	boutonPlayRouge.y = 100 ; // l'ordonnée de début de ce bouton dans l'image est le pixel 100
	boutonPlayRouge.w = boutonW ; // la largeur de ce bouton est la largeur standart de tous les boutons
	boutonPlayRouge.h = boutonH ; // la hauteur de ce bouton est la hauteur standart de tous les boutons

	SDL_Rect boutonPlayJaune ; // // on crée le rectangle qui contient le bouton Play jaune
	boutonPlayJaune.x = 200 ; // l'abscisse de début de ce bouton dans l'image est le pixel 200
	boutonPlayJaune.y = 100 ; // l'ordonnée de début de ce bouton dans l'image est le pixel 0
	boutonPlayJaune.w = boutonW ; // la largeur de ce bouton est la largeur standart de tous les boutons
	boutonPlayJaune.h = boutonH ; // la hauteur de ce bouton est la hauteur standart de tous les boutons

	boutonPlay = boutonPlayRouge ; // au début le bouton Play correspond au bouton Play Rouge
	boutonQuit = boutonQuitRouge ; // au début le bouton Quit correspond au bouton Quit Rouge

    //---------------------------------------------------------Fontes
    TTF_Init() ; // initialisation du module SDL qui gère les fontes
    //------message de présentation
    TTF_Font *presentationFonte ;
    string presentation1 = "Thibierge Raphael" ;
    string presentation2 = "Puyoou Aubin" ;
    string presentation3 = "Info S1 2014-2015" ;
    string presentationPolice = "cell.ttf" ;
    int presentationSize = 12 ;
    presentationFonte = TTF_OpenFont(presentationPolice.c_str(), presentationSize) ;

	TTF_Font *fonts ; //déclaration d'une variable de type fonte
	SDL_Color textColor={255,255,255} ; // on définit la couleur du texte
	const int fontSize = 56 ; // on définit la taille du texte
	string police = "droidloverlaser.ttf" ; // on choisit la police
	fonts = TTF_OpenFont(police.c_str(), fontSize) ; // on définit la fonte par sa police et sa taille

    //--------------------Message "Press Space"
    string messageStart = "press space to continue" ;
    string messageStartPolice = "spaceAge.ttf" ;
    int messageStartSize = 20 ;
    TTF_Font *messageStartFonte ;
    messageStartFonte = TTF_OpenFont(messageStartPolice.c_str(), messageStartSize) ;

	//---------------------------Message de fin de partie
	const string messageFinJ1 = "JOUEUR 1 GAGNE" ; // message affiché à la fin d'une partie lorsque le joueur 1 à gagné
	const string messageFinJ2 = "JOUEUR 2 GAGNE" ; // message affiché à la fin d'une partie lorsque le joueur 2 à gagné

    //---------------------------------------------------------Balle
    Balle balle ; // création d'une balle à partir de la structure Balle
    balle.image = SDL_LoadBMP("ball.bmp"); // on charge l'image de la balle
    SDL_SetColorKey(balle.image, SDL_SRCCOLORKEY, colorkey) ; // on rend transparent le bleu autour de la balle
    balleAuCentre(balle, hauteurEcran, largeurEcran) ; // on placela balle au centre du terrain
    balle.vitesseX = 2 ; // la vittesse horizontale de la balle est 2 pixel par déplacement
    balle.vitesseY = 2 ; // la vitesse verticale de la balle est 2 pixel par déplacement
    balle.hauteur = 6 ; // la hauteur de la balle est 6 pixels

    //---------------------------------------------------------Joueurs
    //--------------Joueur 1
    Joueur joueur1 ; // création d'une variable joueur1 de type Joueur
    joueur1.image = SDL_LoadBMP("player01.bmp"); // image de la barre du joueur 1
    SDL_SetColorKey(joueur1.image, SDL_SRCCOLORKEY, colorkey) ; // on rend transparent le bleu autour de la barre
    joueur1.hauteur = 43 ; // hauteur de la barre
    joueur1.largeur = 11 ; // largeur de la barre
    joueur1.x = 50 ; // position horizontale de la barre
    joueur1.y = (hauteurEcran/2) - (joueur1.hauteur/2) ; //positionnement verticale de la barre
    joueur1.vitesse = 4 ; // vitesse de la barre du joueur 1
    joueur1.score = -1 ; // initialisation du score à 0 car fonction score va ajouter 1
    ostringstream messageJ1 ; // variable contenant le message du score du Joueur 1
    joueur1.numero = 1 ; // joueur numero 1
    score(joueur1, messageJ1, limiteScore) ; //on augmente de 1 point le score du joueur 1 pour qu'il soit à 0 ;
    // la fonction score augmente le score de 1 et l'introduit dans la variable qui va être affichée
    // commencer le score joueur 1 à -1 point simplifie par la suite la gestion de son score

	//--------------Joueur 2
    Joueur joueur2 ; // création d'une variable joueur2 de type Joueur
    joueur2.image = SDL_LoadBMP("player02.bmp"); // image de la barre du joueur 2
    SDL_SetColorKey(joueur2.image, SDL_SRCCOLORKEY, colorkey) ; // on rend transparent le bleu autour de la barre
    joueur2.hauteur = 43 ; // hauteur de la barre
    joueur2.largeur = 11 ; // largeur de la barre
    joueur2.x = largeurEcran - joueur2.largeur - 50 ; // position horizontale de la barre
    joueur2.y = (hauteurEcran/2) - (joueur2.hauteur/2) ; //positionnement verticale de la barre
    joueur2.vitesse = 4 ; // vitesse de la barre
    joueur2.score = -1 ; // initialisation du score à -1 car fonction score va ajouter 1
    ostringstream messageJ2 ; // variable contenant le message du score du Joueur 2
    joueur2.numero = 2 ; // Joueur numero 2
 	score(joueur2, messageJ2, limiteScore) ; //on augmente de 1 point le score du joueur 2 pour qu'il soit à 0 ;
    // la fonction score augmente le score de 1 et l'introduit dans la variable qui va être affichée
    // commencer le score joueur 2 à -1 point simplifie par la suite la gestion de son score

//-------------------------------------------------------------------------------
//								Boucle Principale
//-------------------------------------------------------------------------------
    while(!quit) // tant qu'on ne doit pas quitter
        {
        if (!play) { // si on ne joue pas, on affiche le menu de départ
            apply_surface(0, 0, title, screen, NULL) ; // on applique l'image de fond du menu
            apply_surface(boutonX, boutonY1 , bouton, screen, &boutonPlay) ; // affichage du bouton Play
            apply_surface(boutonX, boutonY2 , bouton, screen, &boutonQuit) ; // affichage du bouto Quit
            showMessageScreen(presentation1, largeurEcran-175, 20, presentationFonte, presentationSize, textColor, screen) ;
            showMessageScreen(presentation2, largeurEcran-145, 40, presentationFonte, presentationSize, textColor, screen) ;
            showMessageScreen(presentation3, largeurEcran-180, 60, presentationFonte, presentationSize, textColor, screen) ;
        }
       // else if (pause ) {
         //   apply_surface(0, 0, background, screen, NULL) ; // on applique l'image de fond
         // et faire un message contenant le score des joueurs si on avait voulus suivre le modèle Pong élève
         // entre les points mais je préfère le jeu comme ca.
         // }
        else { //sinon on affiche le jeu (raquettes et balle)
            mouvementBalle(balle, joueur1, joueur2, hauteurEcran, largeurEcran, pause, messageJ1, messageJ2, limiteScore) ;
            // déplacement de la balle. Le fait qu'elle doive se déplacer ou non
            //est géré dans la fonction avec la variable pause
            affichage(screen, background, balle, joueur1, joueur2) ; // on affiche tous les différents éléments
            showMessageScreen(messageJ1.str(), 150, 35, fonts, fontSize, textColor, screen) ; //onaffiche le score du joueur 1
            showMessageScreen(messageJ2.str(), largeurEcran-150, 35, fonts, fontSize, textColor, screen) ; // on affiche le score du joueur 2
            if (pause)
                showMessageScreen(messageStart, 100, 300, messageStartFonte, messageStartSize, textColor, screen) ;
            if (joueur1.score==limiteScore) // si le joueur 1 atteind la limite de score
                showMessageScreen(messageFinJ1, 65, 160, fonts, 48, textColor, screen) ;
                // on affiche le message de fin qui contient "Joueur 1 à gagné"
            if (joueur2.score==limiteScore) // si le joueur 2 atteind la limite de score
                showMessageScreen(messageFinJ2, 65, 160, fonts, 48, textColor, screen) ;
                // on affiche le message de fin qui contient "Joueur 2 à gagné"
            }
    	SDL_Flip(screen); // on raffraichit l'écran

    	while(SDL_PollEvent(&event)) // boucle qui gère les évènements
            if(event.type==SDL_QUIT)
                quit=true; // pour quiter le jeu

        if (keystates[SDLK_SPACE])
            barreEspace(joueur1, joueur2, play, pause, messageJ1, messageJ2, limiteScore) ;
            // si la barre espace est enfoncée, la fonction barreEspace gère les différentes possibilités
        if (keystates[SDLK_UP]) // si la fleche du haut est enfoncée :
            barreHaut(joueur2) ; // la barre du joueur 2 monte

        if (keystates[SDLK_DOWN]) // si la fleche du bas st enfoncé :
            barreBas(joueur2, hauteurEcran) ; // la barre du joueur 2 descend

        if (keystates[SDLK_z]) // si la touche z est enfoncée :
            barreHaut(joueur1) ; // la barre du joueur 1 monte

        if (keystates[SDLK_s]) // si la touche s est enfoncée :
            barreBas(joueur1, hauteurEcran) ; // la barre du joueur 1 descend

        if ( event.type == SDL_MOUSEMOTION) // gestion des mouvement de la souris
        { // si elle bouge :
            int sourisX = event.motion.x ; // on récupère la position x de la souris dans une variable
            int sourisY = event.motion.y ; // on récupère la position y dela souris dans une autre variable
            if ((sourisX > boutonX) && (sourisX < boutonX+boutonW))
            { // si la souris se situe entre les extrémités gauche et droites des boutons :
                if ((sourisY > boutonY1) && (sourisY < boutonY1+boutonH)) // entre le haut et le bas du bouton Play
                    boutonPlay = boutonPlayJaune ; // le bouton play devient jaune
                else
                    boutonPlay = boutonPlayRouge ; // sinon il devient ou reste rouge

                if ((sourisY > boutonY2) && (sourisY < boutonY2+boutonH)) // entrele haut et le bas du bouton Quit
                    boutonQuit = boutonQuitJaune ; // le bouton Quit devient jaune
                else
                    boutonQuit = boutonQuitRouge ; // sinon il devient ou reste rouge
            }
            else // si la souris à l'extéreur des boutons
            {
                boutonPlay = boutonPlayRouge ; // le bouton play est rouge
                boutonQuit = boutonQuitRouge ; // le bouron play est jaune
            }
        }

        if ( event.type == SDL_MOUSEBUTTONDOWN ) // gestion du clic de la souris
        {
            int sourisX = event.button.x ; // on récupère l'abscisse de la souris dans une variable
            int sourisY = event.button.y ; // on récupère l'abscisse de la souris dans une autre variable
            if ((sourisX > boutonX) && (sourisX < boutonX+boutonW) && (event.button.button == SDL_BUTTON_LEFT))
            { // si la souris se situe entre les extrémités gauche et droites des boutons :
                if ((sourisY > boutonY1) && (sourisY < boutonY1+boutonH)) // entre le haut et le bas du bouton Play
                    play = true ; // le jeu peut commencer
                else if ((sourisY > boutonY2) && sourisY < boutonY2+boutonH) // entre le haut et le bas du bouton Quit
                    quit = true ; // on sort de de la boucle principal et le jeu se ferme
            }
        }
    	SDL_Delay(5) ; // l'ordinateur attend 5ms avant de continuer
    }

	SDL_FreeSurface(screen); // on "enlève" tout ce qui est est à l'écran
  	SDL_Quit() ; // on quitte la SDL
  	return EXIT_SUCCESS ;
}
