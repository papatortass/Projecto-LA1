    #include <ncurses.h>
    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>
    #include <math.h>



    typedef struct Player{
        int playerx;
        int playery;
        int vida;
        int dano;
    }Player;

    typedef struct Mob {
        int mobx;
        int moby;
        int vida;
        int vida_atual;
        int dano;
    } Mob;

    typedef struct Moeda {
        int moedax;
        int moeday;
    } Moeda;

    typedef struct FlashLight {
        int lanternay;
        int lanternax;
    } Lanterna;

    typedef struct Knife {
        int facay;
        int facax;
    } Faca;

    typedef struct Brassknuckles {
        int soqueiray;
        int soqueirax;
    } Soqueira;



    volatile int yMax, xMax;
    volatile float luz = 7.5;
    Moeda *coins;




    int mapSetUp(int y,int x,char **m);
    int drawMap(char ** m, int y);
    Player * playerSetUp(int y, int x,char** m,int yMax,int xMax);
    int handInput(int input, Player * user,char **m,int yMax,int xMax,Mob * mob);
    int checkPosition(int ny,int nx, Player * user,char **m,int yMax,int xMax);
    int playerMove(int y, int x, Player * user,float radius, char ** m,int yMax,int xMax);
    void moveMobs(Player *user, char **m, Mob *mobs, int numMobs);
    float calculateDistance(float x1, float y1, float x2, float y2);
    void exibirVidas(Player * user);
    void posicionarMoeda(Player *user,int i,char **m);
    int combat(char ** m,Player * user,Mob *mob,int input);


    void drawMenu(int y,int x){

        clear();

        int menuHeight = -10;
        int menuWidth = 15;

        int menuY = (y - menuHeight) / 2;
        int menuX = (x - menuWidth) / 2;
        
        init_pair(1, COLOR_WHITE, COLOR_BLACK);	

        attron(COLOR_PAIR(1));

        mvprintw(menuY-(menuY-1),menuX-15,"                                                    ");
        mvprintw(menuY-(menuY-2),menuX-15,"                                               ,--, ");
        mvprintw(menuY-(menuY-3),menuX-15,"          ____       ___                     ,--.'| ");
        mvprintw(menuY-(menuY-4),menuX-15,"        ,'  , `.   ,--.'|_                ,--,  | : ");
        mvprintw(menuY-(menuY-5),menuX-15,"     ,-+-,.' _ |   |  | :,'    __  ,-. ,---.'|  : ' ");
        mvprintw(menuY-(menuY-6),menuX-15,"  ,-+-. ;   , ||   :  : ' :  ,' ,'/ /| |   | : _' | ");
        mvprintw(menuY-(menuY-7),menuX-15," ,--.'|'   |  || .;__,'  /   '  | |' | :   : |.'  | ");
        mvprintw(menuY-(menuY-8),menuX-15,"|   |  ,', |  |, |  |   |    |  |   ,' |   ' '  ; : ");
        mvprintw(menuY-(menuY-9),menuX-15,"|   | /  | |--'  :__,'| :    '  :  /   '   |  .'. | ");
        mvprintw(menuY-(menuY-10),menuX-15,"|   : |  | ,       '  : |__  |  | '    |   | :  | ' ");
        mvprintw(menuY-(menuY-11),menuX-15,"|   : |  |/        |  | '.'| ;  : |    '   : |  : ; ");
        mvprintw(menuY-(menuY-12),menuX-15,"|   | |`-'         ;  :    ; |  , ;    |   | '  ,/  ");
        mvprintw(menuY-(menuY-13),menuX-15,"|   ;/             |  ,   /   ---'     ;   : ;--'   ");
        mvprintw(menuY-(menuY-14),menuX-15,"'---'               ---`-'             |   ,/       ");
        mvprintw(menuY-(menuY-15),menuX-15,"                                       '---'        ");
        mvprintw(menuY-(menuY-16),menuX-15,"                                                    ");

        

        mvprintw(menuY,menuX,"===== Menu =====\n");
        mvprintw(menuY+1,menuX,"1. Jogar\n");
        mvprintw(menuY+2,menuX,"2. Sair\n");
        refresh();
        attroff(COLOR_PAIR(1));
    }

    void drawDeathMenu(int y,int x){

        //Resetar a luz
        luz = 7.5;

        //Limpar o mapa
        clear();


        //Calcular posição da mensagem

        int menuHeight = -10;
        int menuWidth = 15;

        int menuY = (y - menuHeight) / 2;
        int menuX = (x - menuWidth) / 2;


        //Exibir mensagem de morte

        init_pair(1, COLOR_WHITE, COLOR_BLACK);	
        init_pair(2, COLOR_RED, COLOR_BLACK);	
        attron(COLOR_PAIR(1));

        attron(COLOR_PAIR(2));
        mvprintw(menuY-4,menuX,"Voce morreu\n");
        attroff(COLOR_PAIR(2));

        mvprintw(menuY,menuX,"===== Menu =====\n");
        mvprintw(menuY+1,menuX,"1. Jogar Novamente\n");
        mvprintw(menuY+2,menuX,"2. Sair\n");
        refresh();
        attroff(COLOR_PAIR(1));
    }

    int handleMenuInput(int input){
        switch (input) {
            case '1':
                return 1;
                break;
            case '2':
                return 0;
                break;
            default:
                return 7;
                break;

        }
    }

    int main(){

        int numMobs = 10;

        WINDOW * win = initscr();
        noecho();
        cbreak();
        refresh();
        getmaxyx(win,yMax,xMax);
        

        start_color();
        
        refresh();
        keypad(stdscr, TRUE);

        //Alocar espaço para o mapa
        char**map = calloc(yMax,sizeof(char*));
        for(int i = 0; i<yMax; i++){
            map[i]= calloc(xMax,sizeof(char));
        }

        //Imprimir menu inicial
        drawMenu(yMax,xMax);
        int flag = 7;
        while (flag == 7)
        {
            //Guardar input do jogador
            flag = handleMenuInput(getch());
        }
        
        //Iniciar o jogo
        if(flag == 1){
            int gameover=7;
            while(gameover != 0){
                gameover = 7;
                init_pair(1, COLOR_WHITE, COLOR_BLACK);
                bkgd(COLOR_PAIR(1));


                //Inicializar e imprimir mapa
                mapSetUp(yMax,xMax,map);

                char** displayMap = calloc(yMax, sizeof(char*));
                for (int i = 0; i < yMax; i++) {
                    displayMap[i] = calloc(xMax, sizeof(char));
                    memcpy(displayMap[i], map[i], xMax);
                }

                drawMap(map,yMax);


                //inicializar e posicionar mLanterna
                Lanterna * lanterna = malloc(1 * sizeof(Lanterna));
                int posy=0,posx=0;
                while(displayMap[posy][posx] == '#' || displayMap[posy][posx] == '+' ){
                    posy = rand() % ((int)(yMax-2) - 2) + 1;
                    posx = rand() % ((int)xMax - 2) + 1;
                }
                    lanterna->lanternay = posy;
                    lanterna->lanternax = posx;
                    displayMap[posy][posx] = 'L';
                

                //inicializar e posicionar faca
                Faca * faca = malloc(1 * sizeof(Faca));
                int facay=0,facax=0;
                while(displayMap[facay][facax] == '#' || displayMap[facay][facax] == 'L' || displayMap[facay][facax] == '+' ){
                    facay = rand() % ((int)(yMax-2) - 2) + 1;
                    facax = rand() % ((int)xMax - 2) + 1;
                }
                    faca->facay = facay;
                    faca->facax = facax;
                    displayMap[facay][facax] = 'F';
                

                //inicializar e posicionar soqueira
                Soqueira * soqueira = malloc(1 * sizeof(Soqueira));
                int soqueiray=0,soqueirax=0;
                while(displayMap[soqueiray][soqueirax] == '#' || displayMap[soqueiray][soqueirax] == 'L'|| displayMap[soqueiray][soqueirax] == 'F' || displayMap[soqueiray][soqueirax] == '+' ){
                    soqueiray = rand() % ((int)(yMax-2) - 2) + 1;
                    soqueirax = rand() % ((int)xMax - 2) + 1;
                }
                    soqueira->soqueiray = soqueiray;
                    soqueira->soqueirax = soqueirax;
                    displayMap[soqueiray][soqueirax] = 'S';
                

                //Inicializar e posicionar jogador
                Player * user;
                user = playerSetUp(yMax-3,xMax,displayMap,yMax,xMax);


                //inicializar e posicionar moedas
                coins = malloc(30 * sizeof(Moeda));
                for (int i = 0; i < 30; i++) {
                    coins[i].moeday = 1;
                    coins[i].moedax = 1;
                }

                for (int i = 0; i < 30; i++) {
                    posicionarMoeda(user,i,displayMap);
                }


                //inicializar e posicionar mobs
                Mob *mobs = malloc(numMobs * sizeof(Mob));
                for (int i = 0; i < numMobs; i++) {
                    mobs[i].moby = 1;
                    mobs[i].mobx = 1;
                }
                for (int i = 0; i < numMobs; i++) {
                int initialY, initialX = 1;
                do {
                    initialY = rand() % ((int)(yMax-2) - 2) + 1;
                    initialX = rand() % ((int)xMax - 2) + 1;
                } while (displayMap[initialY][initialX] == '#' || displayMap[initialY][initialX] == 'C' || displayMap[initialY][initialX] == 'F' || displayMap[initialY][initialX] == 'S' || displayMap[initialY][initialX] == 'L'  || displayMap[initialY][initialX] == '+' || (int)calculateDistance(user->playerx, user->playery, (float)initialX, (float)initialY ) < 10);
                mobs[i].moby = initialY;
                mobs[i].mobx = initialX;
                mobs[i].vida = 10;
                mobs[i].dano = 1;
                mobs[i].vida_atual = mobs[i].vida;
                displayMap[initialY][initialX] = 'C';
                }


                exibirVidas(user);
                int ch;
                while(gameover != 1){
                    ch = getch();
                    if(user->vida < 2){
                        drawDeathMenu(yMax,xMax);
                        while (gameover == 7){
                            gameover = handleMenuInput(getch());
                        }
                        if (gameover == 0) break;
                    }    
                    handInput(ch, user, displayMap,yMax,xMax,mobs);
                    moveMobs(user, displayMap, mobs, numMobs);
                    exibirVidas(user);
                }

                for(int i= 0;i<yMax;i++){
                free(displayMap[i]);
                }
                free(displayMap);
            }
        }
        
        endwin();

        for(int i= 0;i<yMax;i++){
            free(map[i]);
        }
        free(map);

        

        return 0;
    }

    //Calcular as paredes na vizinhança de um ponto
    int vizinho(int y, int x,char** m){
        int soma=0;
        for(int i =y-1;i<=y+1;i++){
            for(int j = x-1 ;j<=x+1;j++){
                if(!(j==x && i==y)){
                    if(m[i][j] == '#')
                        soma++;
                    }
                }
            }
        return soma;
    }

    int mapSetUp(int y, int x, char** m) {

        time_t t;
        srand((unsigned) time(&t));
        clear();


        //Bordas do mapa
        //cima e embaixo
        for (int i = 0; i < x; i++) { 
            m[0][i] = '#';
            m[(y-3)][i] = '#';
        }
        //lados
        for (int j = 1; j < y-3; j++) { 
            m[j][0] = '#';
            m[j][(int) x - 1] = '#';
        }


        //Preencher o restante do mapa com espaços vazios ou paredes
        for (int i = 1; i < y-3; i++) {
            for (int j = 1; j < x - 1; j++) {
                //Preencher aleatóriamente com parede ou espaço vazio
                if (rand() % 100 > 48)
                    m[i][j] = '#';
                else
                    m[i][j] = ' ';
            }
        }
        
        //Realizar iterações para ajustar a geração do mapa
        for(int gen=0;gen<10;gen++){
            for(int i = 1;i<y-3;i++){
                for(int j= 1;j<x-3;j++){
                    //Calcula a soma dos vizinhos de cada célula
                    int soma = vizinho(i,j,m);
                    //Agrupar paredes e apagar paredes isoladas
                    if(m[i][j] == ' ' && soma >=6)m[i][j] = '#';
                    if(m[i][j] == '#' && soma <=3)m[i][j] = ' ';
                }
            }
        }

        //Abrir espaço para as informações
        for (int i = 0; i<x; i++)
        {
            m[(int)y-1][i] = ' ';
            m[(int)y-2][i] = ' ';
            m[(int)y-3][i] = '#';
        }

        //Inserir informações
        m[(int)y-2][0] = 'H';
        m[(int)y-2][1] = 'e';
        m[(int)y-2][2] = 'a';
        m[(int)y-2][3] = 'l';
        m[(int)y-2][4] = 't';
        m[(int)y-2][5] = 'h';

        m[(int)y-1][0] = 'P';
        m[(int)y-1][1] = 'o';
        m[(int)y-1][2] = 'i';
        m[(int)y-1][3] = 'n';
        m[(int)y-1][4] = 't';
        m[(int)y-1][5] = 's';

        return 0;
    }

    //Imprimir mapa
    int drawMap(char ** m, int y){
        clear();
        for (int i = 0; i < (int) y; i++) {
            printw("%s", m[i]);
        }
        refresh();
        return 0;
    }


    Player * playerSetUp(int y, int x,char **m,int yMax,int xMax){
        Player * newPlayer;
        newPlayer = malloc(sizeof(Player));

        int initialY = 1;
        int initialX = 1;

        //Gerar posições aleatórias até que não tenha paredes na vizinhança
        while (m[initialY][initialX] == '#' && vizinho(initialY,initialX,m) != 0) {
            initialY = rand() % ((int) y - 2) + 1;
            initialX = rand() % ((int) x - 2) + 1;
        }

        newPlayer->playerx = initialX;
        newPlayer->playery = initialY;
        newPlayer->vida = 20;
        newPlayer->dano = 2;

        //Mover o jogador para a posição gerada
        playerMove(newPlayer->playery, newPlayer->playerx,newPlayer,luz,m,yMax,xMax);

        return newPlayer;
    }

    int handInput(int input, Player * user,char **m,int yMax,int xMax,Mob * mob){
        int newy=0,newx=0;
        switch (input){
            case KEY_UP:
            case 'w':
            case 'W':
            case '8':
                newy = user->playery-1;
                newx = user->playerx;
                break;
            case KEY_DOWN:
            case 's':
            case 'S':
            case '2':
                newy = user->playery+1;
                newx = user->playerx;
                break;
            case KEY_RIGHT:
            case 'd':
            case 'D':
            case '6':
                newy = user->playery;
                newx = user->playerx+1;
                break;
            case KEY_LEFT:
            case 'a':
            case 'A':
            case '4':
                newy = user->playery;
                newx = user->playerx-1;
                break;
            case ' ':
                combat(m,user,mob,1);
                break;
        }
        checkPosition(newy,newx, user,m,yMax,xMax);
        return 0;
    }

    int checkPosition(int ny,int nx, Player * user,char **m,int yMax,int xMax){

        switch (m[ny][nx]){
        //Mover jogador para o espaço livre
        case ' ':
            playerMove(ny,nx,user,luz,m,yMax,xMax);
            break;
        //Apanhar moeda
        case '+':
            //Eliminar a moeda do mapa
            m[ny][nx] = ' ';

            //Incrementar a vida do jogador se não estivar cheia
            if (user->vida < 20) {
               user->vida++;
                exibirVidas(user);
                refresh();
            }
            else {
                exibirVidas(user);
                refresh();
            }
            
            //Mover o jogador para nova posição
            playerMove(ny,nx,user,luz,m,yMax,xMax);

            //Encontrar e reposicionar a moeda
            for (int i=0; i<15; i++){
                if (coins[i].moedax == (int)nx && coins[i].moeday == (int)ny){
                    posicionarMoeda(user,i,m);
                }
            }
            break;
        //Apanhar lanterna
        case 'L':
            //Eliminar a lanterna do mapa
            m[ny][nx] = ' ';

            //Aumentar o raio de visão do jogador
            luz+=5;

            //Mover o jogador para nova posição
            playerMove(ny,nx,user,luz,m,yMax,xMax);

            //Exibir mensagem
            mvprintw(yMax-1,(xMax - 15) / 2,"voce apanhou uma lanterna");
            break;
        //Apanhar faca
        case 'F':
            //Eliminar a faca do mapa
            m[ny][nx] = ' ';

            //Aumentar o dano do jogador
            user->dano += 2;

            //Mover o jogador para nova posição
            playerMove(ny,nx,user,luz,m,yMax,xMax);

            //Exibir mensagem
            mvprintw(yMax-1,(xMax - 15) / 2,"voce apanhou uma faca");
            break;
        //Apanhar soqueira
        case 'S':
            //Eliminar a soqueira do mapa
            m[ny][nx] = ' ';

            //Aumentar o dano do jogador
            user->dano += 1;

            //Mover o jogador para nova posição
            playerMove(ny,nx,user,luz,m,yMax,xMax);

            //Exibir mensagem
            mvprintw(yMax-1,(xMax - 15) / 2,"voce apanhou uma soqueira");
            break;
        }
        
        return 0;
    }


    int playerMove(int y, int x, Player *user, float radius, char **m,int yMax,int xMax){

        //Alocar espaço para a matriz da visão
        char** light = calloc(yMax, sizeof(char*));
            for (int i = 0; i < yMax; i++) {
                light[i] = calloc(xMax, sizeof(char));
            }
        //Limpar a matriz da visão
        for(int i=0;i<yMax;i++){
            for(int j=0;j<xMax;j++){
                light[i][j] = ' ';
            }
        }

        //Abrir espaço para as informações
        for (int i = 0; i<xMax; i++){
            light[(int)yMax-1][i] = ' ';
            light[(int)yMax-2][i] = ' ';
            light[(int)yMax-3][i] = '#';
        }

        //Inserir informações
        light[(int)yMax-2][0] = 'H';
        light[(int)yMax-2][1] = 'e';
        light[(int)yMax-2][2] = 'a';
        light[(int)yMax-2][3] = 'l';
        light[(int)yMax-2][4] = 't';
        light[(int)yMax-2][5] = 'h';

        light[(int)yMax-1][0] = 'P';
        light[(int)yMax-1][1] = 'o';
        light[(int)yMax-1][2] = 'i';
        light[(int)yMax-1][3] = 'n';
        light[(int)yMax-1][4] = 't';
        light[(int)yMax-1][5] = 's';
                
        int prevY = (int) user->playery;
        int prevX = (int) user->playerx;

        user->playery = y;
        user->playerx = x;

        int centerX = (int) user->playerx;
        int centerY = (int) user->playery;

        // Limpa a posição anterior do jogador
        light[prevY][prevX] = m[prevY][prevX];

        // Atualiza o mapa para os pontos desaparecerem no raio do jogador
        for (int i = centerY - radius; i <= centerY + radius && i < yMax; i++) {
            for (int j = centerX - radius; j <= centerX + radius && j < xMax; j++) {
                if (i >= 0 && j >= 0 && sqrt(pow(j - centerX, 2) + pow(i - centerY, 2)) <= radius) {
                    if (m[i][j] == ' ') {
                        // Verifica se a linha de visão é obstruída por uma parede
                        int dx = j - centerX;
                        int dy = i - centerY;
                        int cellX = centerX;
                        int cellY = centerY;
                        int stepX = dx > 0 ? 1 : -1;
                        int stepY = dy > 0 ? 1 : -1;
                        int absDx = abs(dx);
                        int absDy = abs(dy);
                        int lineOfSightObstructed = 0;

                        if (absDx > absDy) {
                            int p = absDx / 2;
                            for (int k = 0; k < absDx; k++) {
                                cellX += stepX;
                                p -= absDy;
                                if (p < 0) {
                                    cellY += stepY;
                                    p += absDx;
                                }
                                if (m[cellY][cellX] == '#') {
                                    lineOfSightObstructed = 1;
                                    break;  // Linha de visão obstruída por uma parede
                                }
                            }
                        } else {
                            int p = absDy / 2;
                            for (int k = 0; k < absDy; k++) {
                                cellY += stepY;
                                p -= absDx;
                                cellX += stepX;
                                if (p < 0) {
                                    
                                    p += absDy;
                                }
                                if (m[cellY][cellX] == '#') {
                                    lineOfSightObstructed = 1;
                                    break;  // Linha de visão obstruída por uma parede
                                }
                            }
                        }

                        // Define a iluminação de acordo com a linha de visão
                        if (lineOfSightObstructed) {
                            light[i][j] = ' ';  // Espaço nao visivel
                        } else {
                            // Verifica se a célula é uma parede adjacente
                            if (m[i][j] == '#') {
                                light[i][j] = ' ';  // Parede
                            } 
                            else {
                                light[i][j] = '.';  // Espaço visivel
                            }
                        }
                    }
                    if(m[i][j] == '#'){
                                // Verifica se a linha de visão é obstruída por uma parede
                        int dx = j - centerX;
                        int dy = i - centerY;
                        int cellX = centerX;
                        int cellY = centerY;
                        int stepX = dx > 0 ? 1 : -1;
                        int stepY = dy > 0 ? 1 : -1;
                        int absDx = abs(dx);
                        int absDy = abs(dy);
                        int lineOfSightObstructed = 0;

                        if (absDx > absDy) {
                            int p = absDx / 2;
                            for (int k = 0; k < absDx; k++) {
                                cellX += stepX;
                                p -= absDy;
                                if (p < 0) {
                                    cellY += stepY;
                                    p += absDx;
                                }
                                if (m[cellY][cellX] == '#') {
                                    lineOfSightObstructed = 1;
                                    break;  // Linha de visão obstruída por uma parede
                                }
                            }
                        } else {
                            int p = absDy / 2;
                            for (int k = 0; k < absDy; k++) {
                                cellY += stepY;
                                p -= absDx;
                                if (p < 0) {
                                    cellX += stepX;
                                    p += absDy;
                                }
                                if (m[cellY][cellX] == '#') {
                                    lineOfSightObstructed = 1;
                                    break;  // Linha de visão obstruída por uma parede
                                }
                            }
                        }

                        // Define a iluminação de acordo com a linha de visão
                        if (lineOfSightObstructed) {
                            light[i][j] = '#';  // Parede
                        } else {
                            // Verifica se a célula é uma parede adjacente
                            if (m[i][j] == '#') {
                                light[i][j] = ' ';  // espaço tapado pela parede 
                            } else {
                                light[i][j] = '.';  // Espaço visivel
                            }
                        }	
                    }
                    if(m[i][j] == '+'){
                                // Verifica se a linha de visão é obstruída por uma parede
                        int dx = j - centerX;
                        int dy = i - centerY;
                        int cellX = centerX;
                        int cellY = centerY;
                        int stepX = dx > 0 ? 1 : -1;
                        int stepY = dy > 0 ? 1 : -1;
                        int absDx = abs(dx);
                        int absDy = abs(dy);
                        int lineOfSightObstructed = 0;

                        if (absDx > absDy) {
                            int p = absDx / 2;
                            for (int k = 0; k < absDx; k++) {
                                cellX += stepX;
                                p -= absDy;
                                if (p < 0) {
                                    cellY += stepY;
                                    p += absDx;
                                }
                                if (m[cellY][cellX] == '#') {
                                    lineOfSightObstructed = 1;
                                    break;  // Linha de visão obstruída por uma parede
                                }
                            }
                        } else {
                            int p = absDy / 2;
                            for (int k = 0; k < absDy; k++) {
                                cellY += stepY;
                                p -= absDx;
                                if (p < 0) {
                                    cellX += stepX;
                                    p += absDy;
                                }
                                if (m[cellY][cellX] == '#') {
                                    lineOfSightObstructed = 1;
                                    break;  // Linha de visão obstruída por uma parede
                                }
                            }
                        }

                        // Define a iluminação de acordo com a linha de visão
                        if (lineOfSightObstructed) {
                            light[i][j] = ' ';  // Parede
                        } else {
                            // Verifica se a célula é uma parede adjacente
                            if (m[i][j] == '#') {
                                light[i][j] = ' ';  // espaço tapado pela parede 
                            } else {
                                light[i][j] = '+';  // Espaço visivel
                            }
                        }	
                    }
                    if(m[i][j] == 'C'){
                                // Verifica se a linha de visão é obstruída por uma parede
                        int dx = j - centerX;
                        int dy = i - centerY;
                        int cellX = centerX;
                        int cellY = centerY;
                        int stepX = dx > 0 ? 1 : -1;
                        int stepY = dy > 0 ? 1 : -1;
                        int absDx = abs(dx);
                        int absDy = abs(dy);
                        int lineOfSightObstructed = 0;

                        if (absDx > absDy) {
                            int p = absDx / 2;
                            for (int k = 0; k < absDx; k++) {
                                cellX += stepX;
                                p -= absDy;
                                if (p < 0) {
                                    cellY += stepY;
                                    p += absDx;
                                }
                                if (m[cellY][cellX] == '#') {
                                    lineOfSightObstructed = 1;
                                    break;  // Linha de visão obstruída por uma parede
                                }
                            }
                        } else {
                            int p = absDy / 2;
                            for (int k = 0; k < absDy; k++) {
                                cellY += stepY;
                                p -= absDx;
                                if (p < 0) {
                                    cellX += stepX;
                                    p += absDy;
                                }
                                if (m[cellY][cellX] == '#') {
                                    lineOfSightObstructed = 1;
                                    break;  // Linha de visão obstruída por uma parede
                                }
                            }
                        }

                        // Define a iluminação de acordo com a linha de visão
                        if (lineOfSightObstructed) {
                            light[i][j] = ' ';  // Parede
                        } else {
                            // Verifica se a célula é uma parede adjacente
                            if (m[i][j] == '#') {
                                light[i][j] = ' ';  // espaço tapado pela parede 
                            } else {
                                light[i][j] = 'C';  // Espaço visivel
                            }
                        }
                    }
                    if(m[i][j] == 'L'){
                                // Verifica se a linha de visão é obstruída por uma parede
                        int dx = j - centerX;
                        int dy = i - centerY;
                        int cellX = centerX;
                        int cellY = centerY;
                        int stepX = dx > 0 ? 1 : -1;
                        int stepY = dy > 0 ? 1 : -1;
                        int absDx = abs(dx);
                        int absDy = abs(dy);
                        int lineOfSightObstructed = 0;

                        if (absDx > absDy) {
                            int p = absDx / 2;
                            for (int k = 0; k < absDx; k++) {
                                cellX += stepX;
                                p -= absDy;
                                if (p < 0) {
                                    cellY += stepY;
                                    p += absDx;
                                }
                                if (m[cellY][cellX] == '#') {
                                    lineOfSightObstructed = 1;
                                    break;  // Linha de visão obstruída por uma parede
                                }
                            }
                        } else {
                            int p = absDy / 2;
                            for (int k = 0; k < absDy; k++) {
                                cellY += stepY;
                                p -= absDx;
                                if (p < 0) {
                                    cellX += stepX;
                                    p += absDy;
                                }
                                if (m[cellY][cellX] == '#') {
                                    lineOfSightObstructed = 1;
                                    break;  // Linha de visão obstruída por uma parede
                                }
                            }
                        }

                        // Define a iluminação de acordo com a linha de visão
                        if (lineOfSightObstructed) {
                            light[i][j] = ' ';  // Parede
                        } else {
                            // Verifica se a célula é uma parede adjacente
                            if (m[i][j] == '#') {
                                light[i][j] = ' ';  // espaço tapado pela parede 
                            } else {
                                light[i][j] = 'L';  // Espaço visivel
                            }
                        }
                    }
                    if(m[i][j] == 'F'){
                        // Verifica se a linha de visão é obstruída por uma parede
                        int dx = j - centerX;
                        int dy = i - centerY;
                        int cellX = centerX;
                        int cellY = centerY;
                        int stepX = dx > 0 ? 1 : -1;
                        int stepY = dy > 0 ? 1 : -1;
                        int absDx = abs(dx);
                        int absDy = abs(dy);
                        int lineOfSightObstructed = 0;

                        if (absDx > absDy) {
                            int p = absDx / 2;
                            for (int k = 0; k < absDx; k++) {
                                cellX += stepX;
                                p -= absDy;
                                if (p < 0) {
                                    cellY += stepY;
                                    p += absDx;
                                }
                                if (m[cellY][cellX] == '#') {
                                    lineOfSightObstructed = 1;
                                    break;  // Linha de visão obstruída por uma parede
                                }
                            }
                        } else {
                            int p = absDy / 2;
                            for (int k = 0; k < absDy; k++) {
                                cellY += stepY;
                                p -= absDx;
                                if (p < 0) {
                                    cellX += stepX;
                                    p += absDy;
                                }
                                if (m[cellY][cellX] == '#') {
                                    lineOfSightObstructed = 1;
                                    break;  // Linha de visão obstruída por uma parede
                                }
                            }
                        }

                        // Define a iluminação de acordo com a linha de visão
                        if (lineOfSightObstructed) {
                            light[i][j] = ' ';  // Parede
                        } else {
                            // Verifica se a célula é uma parede adjacente
                            if (m[i][j] == '#') {
                                light[i][j] = ' ';  // espaço tapado pela parede 
                            } else {
                                light[i][j] = 'F';  // Espaço visivel
                            }
                        }
                    }
                    if(m[i][j] == 'S'){
                        // Verifica se a linha de visão é obstruída por uma parede
                        int dx = j - centerX;
                        int dy = i - centerY;
                        int cellX = centerX;
                        int cellY = centerY;
                        int stepX = dx > 0 ? 1 : -1;
                        int stepY = dy > 0 ? 1 : -1;
                        int absDx = abs(dx);
                        int absDy = abs(dy);
                        int lineOfSightObstructed = 0;

                        if (absDx > absDy) {
                            int p = absDx / 2;
                            for (int k = 0; k < absDx; k++) {
                                cellX += stepX;
                                p -= absDy;
                                if (p < 0) {
                                    cellY += stepY;
                                    p += absDx;
                                }
                                if (m[cellY][cellX] == '#') {
                                    lineOfSightObstructed = 1;
                                    break;  // Linha de visão obstruída por uma parede
                                }
                            }
                        } else {
                            int p = absDy / 2;
                            for (int k = 0; k < absDy; k++) {
                                cellY += stepY;
                                p -= absDx;
                                if (p < 0) {
                                    cellX += stepX;
                                    p += absDy;
                                }
                                if (m[cellY][cellX] == '#') {
                                    lineOfSightObstructed = 1;
                                    break;  // Linha de visão obstruída por uma parede
                                }
                            }
                        }

                        // Define a iluminação de acordo com a linha de visão
                        if (lineOfSightObstructed) {
                            light[i][j] = ' ';  // Parede
                        } else {
                            // Verifica se a célula é uma parede adjacente
                            if (m[i][j] == '#') {
                                light[i][j] = ' ';  // espaço tapado pela parede 
                            } else {
                                light[i][j] = 'S';  // Espaço visivel
                            }
                        }
                    }
                }
            }   
        }

        light[(int)user->playery][(int)user->playerx] = '@';  

        //Redesenhar o mapa
        drawMap(light, yMax);

        for(int i= 0;i<yMax;i++){
            free(light[i]);
        }
        free(light);
        return 0;
    }

    void moveMobs(Player *user, char **m, Mob *mobs, int numMobs) {
        for (int i = 0; i < numMobs; i++){
            float nextX = mobs[i].mobx;
            float nextY = mobs[i].moby;

            //Calcular distâncio do jogador ao mob
            float distanceToPlayer = calculateDistance(user->playerx, user->playery, nextX, nextY);

            //Distância maior que 6: mobs não se movem
            if (distanceToPlayer > 6) {
                
                m[(int)mobs[i].moby][(int)mobs[i].mobx] = 'C';

            //Distância menor ou igual a 6: mobs perseguem o jogador
            } else if (distanceToPlayer <= 6) {
                
                if (user->playerx < mobs[i].mobx) {
                    nextX -= 1;
                }
                if (user->playerx > mobs[i].mobx) {
                    nextX += 1;
                }
                if (user->playery < mobs[i].moby) {
                    nextY -= 1;
                }
                if (user->playery > mobs[i].moby) {
                    nextY += 1;
                }
            }

            //Atualizar as coordenadas apenas se a posição nova estiver livre
            if (m[(int)nextY][(int)nextX] == ' ') {
                m[(int)mobs[i].moby][(int)mobs[i].mobx] = ' ';
                mobs[i].moby = nextY;
                mobs[i].mobx = nextX;
                m[(int)(mobs[i].moby)][(int)(mobs[i].mobx)] = 'C';
            }
        }

        combat(m,user,mobs,0);
    }

    float calculateDistance(float x1, float y1, float x2, float y2) {
        //Teorema de pitágoras
        float dist = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
        return dist;
    }

    void exibirVidas(Player * user) {
        int vidasPos = 7;  //Posição para exibir as vidas
        int maximo = yMax - 1;  //Altura do mapa

        //Posição atual do cursor
        int curY, curX;
        getyx(stdscr, curY, curX);

        //Mover o cursor para a posição correta no fundo da tela
        move(maximo , vidasPos);

        //Imprimir as barras de vida
        for (int i = 0; i <= user->vida; i++) {
            if (i < user->vida){
                addch('|');  // Exqibe uma vida representada por '|'
            } else {
                addch(' ');  // Espaço vazio para representar vidas não disponíveis
            }
        }

        maximo--;
        //Mover o cursor para a posição correta no fundo da tela
        move(maximo , vidasPos);
        //Imprimir as barras de vida
        for (int i = 0; i <= user->vida; i++) {
            if (i < user->vida) {
                addch('|');  //Exibe uma vida representada por '|'
            } else {
                addch(' ');  //Espaço vazio para representar vidas não disponíveis
            }
        }

        //Restaurar a posição original do cursor
        move(curY, curX);

        //Atualizar a tela para mostrar as alterações
        refresh();
    }

    void posicionarMoeda(Player *user,int i, char **m)
    {
        int initialY, initialX = 1;

        //Gerar uma posição livre para a moeda
        do {
            initialY = rand() % ((int)yMax - 2) + 1;
            initialX = rand() % ((int)xMax - 2) + 1;
        } while (m[initialY][initialX] == '#' || m[initialY][initialX] == 'C'  || m[initialY][initialX] == '+' || m[initialY][initialX] == 'F' || m[initialY][initialX] == 'S' || (int)calculateDistance(user->playerx, user->playery, (float)initialX, (float)initialY ) < 5);
        
        //Posicionar moeda na posição gerada
        coins[i].moeday = initialY;
        coins[i].moedax = initialX;
        m[initialY][initialX] = '+';
    }

    int combat(char ** m,Player * user,Mob * mob,int input){
        if(input== 1){
            int index = -1;
            int numMobs=10;

            // Procura o monstro nas imediações do jogador
            for (int i = 0; i < numMobs; i++) {
                for(int j = user->playery-1;j<user->playery+2;j++){
                    for(int k=user->playerx-1;k<user->playerx+2;k++){
                        if (k == mob[i].mobx && j == mob[i].moby) {
                            index = i;
                            break;
                    }
                }
                }
            }

            if (index != -1) {
                //Reduz a vida do monstro com o dano do jogador
                mob[index].vida_atual -= user->dano;
                mvprintw(yMax-1,(xMax - 15) / 2,"voce atacou o montro");
                user->vida+=1;

                if (mob[index].vida_atual <= 0) {

                    //Monstro Derrotado
                    mvprintw(yMax-1,(xMax - 15) / 2,"voce matou o monstro");

                    //Remover monstro do mapa e da lista de mobs
                    m[mob[index].moby][mob[index].mobx] = ' ';
                    
                    for (int i = index; i < numMobs - 1; i++) {
                        mob[i] = mob[i + 1];
                    }

                    numMobs--;
                    mob = realloc(mob, sizeof(Mob) * numMobs);
                }
            }

            //Reduz a vida do jogador com o dano do monstro
            for(int i=0;i<10;i++){
                    if(user->playery == mob[i].moby && user->playerx == mob[i].mobx){
                        user->vida-=mob[i].dano;
                    }
            }
        }

        if(input == 0){
            for(int i=0;i<10;i++){
                //Reduz a vida do jogador com o dano do monstro
                if(user->playery == mob[i].moby && user->playerx == mob[i].mobx){
                    user->vida-=mob[i].dano;
                }
            }
        }
        return 0;
    }
