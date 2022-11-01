# PongCPP

# Archivo Principales

SDL_Project.cpp -> 
Variables y constantes globales. 
Iniciacion de Engine. 
Destroy Engine. 
Carga de Assets. 
Descarga de Assets. 
Input Updates. 
Update Game. 
Renderizado. 
main().

InputModule.cpp ->

Configuracion de teclas
Estado de teclas

GSLogoState.cpp ->
Cambio de escenas?

# Definicion de Estructuras

StructsDef.h ->

Input State: Acciones que puede realizar el jugado (bool)

Sprite: Textura, canvas y bool para ocultar como variables

Text: font, color, surface(?), textura, canvas bool para ocultar como variables

Bgm: Musica de fondo

enum(?) GAME_STAGES: Escenas

GameStage: Nombre del stage, ID y # de nivel

ResourceManager: Contiene sprites, textos, musica, juegos e inputs

# Como funciona SDL_Project.cpp?

El flujo de main() funciona de la siguiente manera:

1. Se llama a initEngine();

SDL_Init
Mix_OpenAudio
TTF_Init

Son funciones de SDL por defecto

Luego de esto, se verifica si el motor de sonido fue iniciado exitosamente

La variable window se inicializa con la funcion nativa de SDL: SDL_CreateWindow. Esta funcion tiene como parametro el nombre de la ventana, la posicion en x donde se abrira (se usa una constante undefined de SDL para centrarla), la posicion en y donde se abrira (se usa una constante undefined de SDL para centrarla), el ancho (Que es una constante declarada), el alto (constante declarada) y algunas flags que sirven para configurar la ventana sea pantalla completa, ventana, ventana a pantalla completa (en este caso solo se usa la flag SDL_WINDOW_SHOWN para mostrar la ventana una vez inicializada)

La variable renderer es el lienzo donde se crearan los sprites, es inicializada con SDL_CreateRenderer y toma como parametro la variable window, -1 para inicializar el driver por defecto para renderizar, como ultimo parametro unas flags sobre sincronizacion vertical

Al ResourceManager se le asignan los assets

Se crea un GameStage haciendo referencia a la pantalla de Logo, y anadida a gameStages

2. Se llama a loadAssets()

Para cargar un sprite:

Es declarado un string con la ruta de la imagen, y luego es declarado una textura con IMG_LoadTexture, con el renderer como parametro y la direccion como string. Un SLD_Rect es declarado como dest y luego inicializados sus componente x,y,w y h

Es declarado de Sprite e inicializado su .dest con el rect recien inicializado y ademas se le es inicializado la textura. Luego insertada en el array SpritesAssets

Para cargar una fuente:

Se declarada nuevamente una direccion, una fuente y un color. Luego son tomados como parametro en la variable msg que llama como funcion a TTF_RenderText_Solid y luego es creado el sprite con SDL_CreateTextureFromSurface
Es creado un rect, y un text para luego anadir el text al array textAssets

Para cargar un audio:

Se declara la direccion, crea una variable Mix_Music y luego es inicializada con Mix_LoadMUS y la direccion como parametro
Es agragado al array music Assets

3. Se llama a Mix_playMusic(arraymusic[indice 0], -1 (en loop?))

4. Se crea una variable currentTime con la funcion SDL_GetTicks64();

5. Se usa un while con argumento isGameRunning

GameRunning es verdadero cuando se inicializa la variable. Luego es cambiada a falso cuando es el switch en inputUpdate se activa

Se crea un delta como la difencia entre dos GetTicks

Se llama al inputUpdate para que sean leidos los inputs con el loop

Es llamado updateGame con este delta como parametro y el multiplicador del tiempo, update game tiene el efecto de parpadeo de la pantalla inicial y el selector de escenas

se llama Render() que pinta todos los sprites y textos con dos for separados

6. Si el while es roto, se llama a Mix_HaltMusic que detiene la musica
7. Se llama a unloadassets, Destroye todas las texturas y sonidos en los array correspondientes
8. Se destruye el engine, el rendeder, la ventana, se apaga el motor de texto, es cerrado el audio, la mezclas de sonido y es llamado SDL_Quit

# En busqueda de los assets

Las paletas originales miden 30px*8px
La pelota 8px * 8 px
Las linea linea punteada de la mitad 8px * 4 px y son 32 