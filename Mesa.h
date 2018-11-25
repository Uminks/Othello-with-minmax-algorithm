
class Mesa {
        int tablero[8][8];
    public:
        Mesa();
        void imprimir(bool consola=true);
        bool jugarTablero(int, int, int);
        bool movimientoValido(int, int, int);
        bool comprobarVoltearFichas(int, int, int, int, int, bool);
        int getValorPosicion(int, int);
        int score();
        bool tableroLleno();
        bool tieneMovimientoValido(int);
        void setTablero(Mesa *b); //copy over another board's squares
        int evaluar(int, int); //Evaluacion heuristica del tablero actual para se usada en minmax
        int vecinosLibres(int, int);
};

void Mesa::imprimir(bool consola) {
	char cforvalplusone[] = {'B', '_', 'N'};
	if(consola)cout << "  1 2 3 4 5 6 7 8" << endl;
	for(int i=0; i<8;i++) {
		if(consola)cout << i+1 << '|';
		for(int j=0; j<8; j++){
            if(consola)cout << cforvalplusone[tablero[i][j]+1] << '|';
            draw_sprite(buffer, cuadro, j*60, i*60);
            if(cforvalplusone[tablero[i][j]+1] == 'B') draw_sprite(buffer, fichaB, j*60, i*60);
            else if(cforvalplusone[tablero[i][j]+1] == 'N') draw_sprite(buffer, fichaN, j*60, i*60);
            //else draw_sprite(buffer, cuadro, j*30, i*30);
		}
			//cout << cforvalplusone[tablero[i][j]+1] << '|';
		if(consola)cout << endl;
	}

	///ALLEGRO draw_sprite(buffer, cuadro, 0, 0);
}

Mesa::Mesa() {
	for(int i=0; i<8;i++)
		for(int j=0; j<8; j++)
			tablero[i][j] = 0;
	tablero[3][3]=-1;
	tablero[4][4]=-1;
	tablero[3][4]=1;
	tablero[4][3]=1;
}

bool Mesa::tableroLleno() {
	for(int i=0; i<8;i++)
		for(int j=0; j<8; j++)
			if(tablero[i][j]==0)
				return false;
	return true;
}

bool Mesa::jugarTablero(int fila, int col, int val) {
	if(!movimientoValido(fila, col, val))
		return false;
	tablero[fila-1][col-1] = val;
	for(int i = -1; i <= 1; i++)
		for(int j = -1; j <= 1; j++)
			comprobarVoltearFichas(fila-1, col-1, i, j, val, true);

	return true;
}

int Mesa::getValorPosicion(int fila, int col) {
	return tablero[fila-1][col-1];
}

void Mesa::setTablero(Mesa *b) {
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			tablero[i][j] = b->getValorPosicion(i+1,j+1);
		}
	}

}

bool Mesa::movimientoValido(int fila, int columna, int val) {
	int f = fila-1;
	int c = columna-1;
	if(f < 0 || f > 7 || c < 0 || c > 7)
		return false;

	if(tablero[f][c]!=0)
		return false;

	for(int i = -1; i <= 1; i++)
		for(int j = -1; j <= 1; j++)
			if(comprobarVoltearFichas(f, c, i, j, val, false)) return true;

	return false;
}

/** f = fila, c = columna, fady = fila adyacente, cady = columna adyacente, voltear **/
bool Mesa::comprobarVoltearFichas(int f, int c, int fady, int cady, int val, bool voltear) {
	int pathf = f + fady;
	int pathc = c + cady;
	if(pathf < 0 || pathf > 7 || pathc < 0 || pathc > 7 || tablero[pathf][pathc]!=-1*val)
		return false;

	while(true) {
		pathf += fady;
		pathc += cady;
		if(pathf < 0 || pathf > 7 || pathc < 0 || pathc > 7 || tablero[pathf][pathc]==0)
			return false;
		if(tablero[pathf][pathc]==val) {
			if(voltear) {
				pathf=f+fady;
				pathc=c+cady;
				while(tablero[pathf][pathc]!=val) {
					tablero[pathf][pathc]=val;
					pathf += fady;
					pathc += cady;
				}
			}
			return true;
		}
	}
	return false;
}

bool Mesa::tieneMovimientoValido(int val) {
	for(int i=0; i<8;i++)
		for(int j=0; j<8; j++)
			if(movimientoValido(i+1, j+1, val))
				return true;
	return false;
}

int Mesa::vecinosLibres(int i, int j) {
	int count = 0;

	// examine the 8 possible neighborings unless not possible positions
	if ((i+1)>0 && j>0 && (i+1)<9 && j<9 && getValorPosicion(i+1, j) == 0)
		count++;
	if ((i+1)>0 && (j-1)>0 && (i+1)<9 && (j-1)<9 && getValorPosicion(i+1, j-1) == 0)
		count++;
	if (i>0 && (j-1)>0 && i<9 && (j-1)<9 && getValorPosicion(i, j-1) == 0)
		count++;
	if ((i-1)>0 && (j-1)>0 && (i-1)<9 && (j-1)<9 && getValorPosicion(i-1, j-1) == 0)
		count++;
	if ((i-1)>0 && j>0 && (i-1)<9 && j<9 && getValorPosicion(i-1, j) == 0)
		count++;
	if ((i-1)>0 && (j+1)>0 && (i-1)<9 && (j+1)<9 && getValorPosicion(i-1, j+1) == 0)
		count++;
	if (i>0 && (j+1)>0 && i<9 && (j+1)<9 && getValorPosicion(i, j+1) == 0)
		count++;
	if ((i+1)>0 && (j+1)>0 && (i+1)<9 && (j+1)<9 && getValorPosicion(i+1, j+1) == 0)
		count++;

	return count;

}

int Mesa::score() {
	int sum =0;
	for(int i=0; i<8;i++)
		for(int j=0; j<8; j++)
			sum+=tablero[i][j];
	return sum;
}

int Mesa::evaluar(int cpuval, int profundidad) {


	int score = 0; //evaluacion de score

	//cuenta los movimientos disponibles para computadora y jugador
	int mc = 0; int mp = 0;
	for (int i=1; i<9;i++) {
		for (int j=1; j<9; j++) {
			if (movimientoValido(i, j, cpuval))
				mc++;
			if (movimientoValido(i, j, -1*cpuval))
				mp++;
		}
	}

	// Suma la diferencia al score (escalado)
	score += 20*(mc - mp); //El número es solo una escala determinada a través del juego.

	// Contar esquinas para computadora y jugador.
	int cc = 0; int cp = 0;
	if (getValorPosicion(1, 1) == cpuval)
		cc++;
	else if (getValorPosicion(1, 1) == -1*cpuval)
		cp++;

	if (getValorPosicion(1, 8) == cpuval)
		cc++;
	else if (getValorPosicion(1, 8) == -1*cpuval)
		cp++;

	if (getValorPosicion(8, 1) == cpuval)
		cc++;
	else if (getValorPosicion(8, 1) == -1*cpuval)
		cp++;

	if (getValorPosicion(8, 8) == cpuval)
		cc++;
	else if (getValorPosicion(8, 8) == -1*cpuval)
		cp++;

	//Suma la diferencia al score (escalado)
	score += 200*(cc - cp);


    //limita la cantidad de espacio alrededor de nuestras piezas para que no nos rodeen tanto
    //(lo que conduce a grandes ganancias en el juego final para el oponente)
	int sc = 0; int sp = 0; //contadores para espacios abiertos al lado de las piezas de un jugador / cpu.
	for (int i=1; i<9;i++) {
		for (int j=1; j<9; j++) {
			if (getValorPosicion(i, j) == cpuval) {
				sc += vecinosLibres(i, j);
			}
			if (getValorPosicion(i, j) == -1*cpuval) {
				sp += vecinosLibres(i, j);
			}
		}
	}

	score -= 10*(sc - sp); //restar porque estamos tratando de minimizarlo
	return score;
}
