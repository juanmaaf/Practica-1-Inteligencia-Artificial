#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;



Action ComportamientoJugador::think(Sensores sensores){

	// Pintamos los Precipicios -> 3 de anchcura en cada lado

	if(!precipicios_pintados){
		pintaPrecipicios(tamanio, mapaResultado);
		precipicios_pintados = true;
	}

	Action accion = actIDLE;

	cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC << " ";
	switch(sensores.sentido){
		case 0: cout << "Norte" << endl; break;
		case 1: cout << "Noreste" << endl; break;
		case 2: cout << "Este" << endl; break;
		case 3: cout << "Sureste" << endl; break;
		case 4: cout << "Sur " << endl; break;
		case 5: cout << "Suroeste" << endl; break;
		case 6: cout << "Oeste" << endl; break;
		case 7: cout << "Noroeste" << endl; break;
	}
	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];
	cout << endl;

	cout << "Superficie: ";
	for (int i=0; i<sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;

	cout << "Colisión: " << sensores.colision << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout << endl;

	// Si hemos colisionado, no actualizamos movemos posición en Estado
	if(!sensores.colision){
		switch(last_action){
			case actFORWARD:
				switch(current_state.brujula){
					case norte:
						current_state.fil--;
					break;
					case noroeste:
						current_state.fil--;
						current_state.col--;
					break;
					case oeste:
						current_state.col--;
					break;
					case suroeste:
						current_state.col--;
						current_state.fil++;
					break;
					case sur:
						current_state.fil++;
					break;
					case sureste:
						current_state.col++;
						current_state.fil++;
					break;
					case este:
						current_state.col++;
					break;
					case noreste:
						current_state.col++;
						current_state.fil--;
					break;
				}
			break;
			case actTURN_SL:
				current_state.brujula = static_cast<Orientacion>((current_state.brujula+7)%8);
			break;
			case actTURN_SR:
				current_state.brujula = static_cast<Orientacion>((current_state.brujula+1)%8);
			break;
			case actTURN_BL:
				current_state.brujula = static_cast<Orientacion>((current_state.brujula+5)%8);
			break;
			case actTURN_BR:
				current_state.brujula = static_cast<Orientacion>((current_state.brujula+3)%8);
			break;
		}
	}

	if(sensores.reset){
		bien_situado = false;
		tiene_zapatillas = false;
		tiene_bikini = false;
		if(sensores.nivel != 0){
			copiaTrasPosionamiento = false;
		}
	}
	
	if(sensores.nivel != 0 && !bien_situado){
		current_state.fil = tamanio;
		current_state.col = tamanio;
	}

	if ((sensores.terreno[0] == 'G' and !bien_situado) || sensores.nivel == 0){
		current_state.fil = sensores.posF;
		current_state.col= sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;

		if(sensores.nivel != 0 && !copiaTrasPosionamiento){
			/*for(int i = 0; i < tamanio; i++){
				for(int j = 0; j < tamanio; j++){
					if(matrizNoPosicionado[i+tamanio][j+tamanio] != '?'){
						mapaResultado[i][j] = matrizNoPosicionado[i+tamanio][j+tamanio];
					}
				}
			}

			for(int i = 0; i < tamanio; i++){
				for(int j = 0; j < tamanio; j++){
					matrizVecesPasadas[i][j] += matrizVecesPasadasNoPosicionado[i+tamanio][j+tamanio];
				}
			}
			copiaTrasPosionamiento = true;*/
		}
	}

	if (bien_situado){
		contabilizaPasoPorCasilla(current_state, matrizVecesPasadas);
		pintaVision(current_state, sensores.terreno, mapaResultado);
	}else{
		contabilizaPasoPorCasilla(current_state, matrizVecesPasadasNoPosicionado);
		pintaVision(current_state, sensores.terreno, matrizNoPosicionado);
	}

	paso_no_permitido = false;
	if (sensores.superficie[2] == '_'){
		accion = actFORWARD;
		switch(sensores.terreno[2]){
			// Los tipos no especificados no requieren condiciones
			case 'B':
				if(!tiene_zapatillas && last_action == actFORWARD){
					paso_no_permitido = true;
				}
			break;
			case 'A':
				if(!tiene_bikini){
					paso_no_permitido = true;
				}
			break;
			case 'P':
				paso_no_permitido = true;
			break;
			case 'K':
				tiene_bikini = true;
			break;
			case 'D':
				tiene_zapatillas = true;
			break;
		}
	}else{
		paso_no_permitido = true;
	}

	if(sensores.terreno[0] == 'X' && sensores.bateria != bateriaMax){
		accion = actIDLE;
	}

	if(paso_no_permitido or sensores.reset){
		int eleccion = rand()%4;
		switch(eleccion){
			case 0:
				accion = actTURN_SL;
			break;
			case 1:
				accion = actTURN_SR;
			break;
			case 2:
				accion = actTURN_BL;
			break;
			case 3:
				accion = actTURN_BR;
			break;
		}
	}

	if(sensores.colision){
		int elec = rand()%4;
		switch(elec){
			case 0:
				accion = actTURN_BL;
			break;
			case 1:
				accion = actTURN_BR;
			break;
		}
	}

	last_action = accion;

	printMatrizVecesPasadas(tamanio, matrizVecesPasadas);

	/*cout << endl << endl;

	if(sensores.nivel != 0){
		printMatrizNoPosicionado(tamanio, matrizNoPosicionado);
	}*/

	// Determinar el efecto de la ultima accion enviada
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}

void ComportamientoJugador::contabilizaPasoPorCasilla(const state current_state, vector< vector<unsigned int> > &matriz){
	matriz[current_state.fil][current_state.col]++;
}

void ComportamientoJugador::printMatrizVecesPasadas(const int tam, vector< vector<unsigned int> > &matriz){
	for(int i = 0; i < tam; i++){
		for(int j = 0; j < tam; j++){
			cout << matriz[i][j];
		}
		cout << endl;
	}
}

/*void ComportamientoJugador::printMatrizNoPosicionado(const int tam, vector< vector<unsigned char> > &matriz){
	for(int i = 0; i < tam*2 + 1; i++){
		for(int j = 0; j < tam*2 +1; j++){
			cout << matriz[i][j];
		}
		cout << endl;
	}
}*/

void ComportamientoJugador::pintaPrecipicios(const int tam, vector< vector<unsigned char> > &matriz){
	for(int i = 0; i < tam; i++){
			for(int j = 0; j < tam; j++){
				if(i>2 && i<tam-2){
					if(j<=2 || j >= tam -3){
						matriz[i][j] = 'P';
					}
				}
				if(i<=2 || i >= tamanio - 3){
					matriz[i][j] = 'P';
				}
			}
		}
}

void ComportamientoJugador::pintaVision(const state current_state, const vector<unsigned char> terreno, vector< vector<unsigned char> > &matriz){
	matriz[current_state.fil][current_state.col] = terreno[0];
	switch(current_state.brujula){
		case norte:
			matriz[current_state.fil - 1][current_state.col - 1] = terreno[1];
			matriz[current_state.fil - 1][current_state.col] = terreno[2];
			matriz[current_state.fil - 1][current_state.col + 1] = terreno[3];
			matriz[current_state.fil - 2][current_state.col - 2] = terreno[4];
			matriz[current_state.fil - 2][current_state.col - 1] = terreno[5];
			matriz[current_state.fil - 2][current_state.col] = terreno[6];
			matriz[current_state.fil - 2][current_state.col + 1] = terreno[7];
			matriz[current_state.fil - 2][current_state.col + 2] = terreno[8];
			matriz[current_state.fil - 3][current_state.col - 3] = terreno[9];
			matriz[current_state.fil - 3][current_state.col - 2] = terreno[10];
			matriz[current_state.fil - 3][current_state.col - 1] = terreno[11];
			matriz[current_state.fil - 3][current_state.col] = terreno[12];
			matriz[current_state.fil - 3][current_state.col + 1] = terreno[13];
			matriz[current_state.fil - 3][current_state.col + 2] = terreno[14];
			matriz[current_state.fil - 3][current_state.col + 3] = terreno[15];
		break;
		case noroeste:
			matriz[current_state.fil][current_state.col - 1] = terreno[1];
			matriz[current_state.fil - 1][current_state.col - 1] = terreno[2];
			matriz[current_state.fil - 1][current_state.col] = terreno[3];
			matriz[current_state.fil][current_state.col - 2] = terreno[4];
			matriz[current_state.fil - 1][current_state.col - 2] = terreno[5];
			matriz[current_state.fil - 2][current_state.col - 2] = terreno[6];
			matriz[current_state.fil - 2][current_state.col - 1] = terreno[7];
			matriz[current_state.fil - 2][current_state.col] = terreno[8];
			matriz[current_state.fil][current_state.col - 3] = terreno[9];
			matriz[current_state.fil - 1][current_state.col - 3] = terreno[10];
			matriz[current_state.fil - 2][current_state.col - 3] = terreno[11];
			matriz[current_state.fil - 3][current_state.col - 3] = terreno[12];
			matriz[current_state.fil - 3][current_state.col - 2] = terreno[13];
			matriz[current_state.fil - 3][current_state.col - 1] = terreno[14];
			matriz[current_state.fil - 3][current_state.col] = terreno[15];
		break;
		case oeste:
			matriz[current_state.fil + 1][current_state.col - 1] = terreno[1];
			matriz[current_state.fil][current_state.col - 1] = terreno[2];
			matriz[current_state.fil - 1][current_state.col - 1] = terreno[3];
			matriz[current_state.fil + 2][current_state.col - 2] = terreno[4];
			matriz[current_state.fil + 1][current_state.col - 2] = terreno[5];
			matriz[current_state.fil][current_state.col - 2] = terreno[6];
			matriz[current_state.fil - 1][current_state.col - 2] = terreno[7];
			matriz[current_state.fil - 2][current_state.col - 2] = terreno[8];
			matriz[current_state.fil + 3][current_state.col - 3] = terreno[9];
			matriz[current_state.fil + 2][current_state.col - 3] = terreno[10];
			matriz[current_state.fil + 1][current_state.col - 3] = terreno[11];
			matriz[current_state.fil][current_state.col - 3] = terreno[12];
			matriz[current_state.fil - 1][current_state.col - 3] = terreno[13];
			matriz[current_state.fil - 2][current_state.col - 3] = terreno[14];
			matriz[current_state.fil - 3][current_state.col - 3] = terreno[15];
		break;
		case suroeste:
			matriz[current_state.fil + 1][current_state.col] = terreno[1];
			matriz[current_state.fil + 1][current_state.col - 1] = terreno[2];
			matriz[current_state.fil][current_state.col - 1] = terreno[3];
			matriz[current_state.fil + 2][current_state.col] = terreno[4];
			matriz[current_state.fil + 2][current_state.col - 1] = terreno[5];
			matriz[current_state.fil + 2][current_state.col - 2] = terreno[6];
			matriz[current_state.fil + 1][current_state.col - 2] = terreno[7];
			matriz[current_state.fil][current_state.col - 2] = terreno[8];
			matriz[current_state.fil + 3][current_state.col] = terreno[9];
			matriz[current_state.fil + 3][current_state.col - 1] = terreno[10];
			matriz[current_state.fil + 3][current_state.col - 2] = terreno[11];
			matriz[current_state.fil + 3][current_state.col - 3] = terreno[12];
			matriz[current_state.fil + 2][current_state.col - 3] = terreno[13];
			matriz[current_state.fil + 1][current_state.col - 3] = terreno[14];
			matriz[current_state.fil][current_state.col - 3] = terreno[15];
		break;
		case sur:
			matriz[current_state.fil + 1][current_state.col + 1] = terreno[1];
			matriz[current_state.fil + 1][current_state.col] = terreno[2];
			matriz[current_state.fil + 1][current_state.col - 1] = terreno[3];
			matriz[current_state.fil + 2][current_state.col + 2] = terreno[4];
			matriz[current_state.fil + 2][current_state.col + 1] = terreno[5];
			matriz[current_state.fil + 2][current_state.col] = terreno[6];
			matriz[current_state.fil + 2][current_state.col - 1] = terreno[7];
			matriz[current_state.fil + 2][current_state.col - 2] = terreno[8];
			matriz[current_state.fil + 3][current_state.col + 3] = terreno[9];
			matriz[current_state.fil + 3][current_state.col + 2] = terreno[10];
			matriz[current_state.fil + 3][current_state.col + 1] = terreno[11];
			matriz[current_state.fil + 3][current_state.col] = terreno[12];
			matriz[current_state.fil + 3][current_state.col - 1] = terreno[13];
			matriz[current_state.fil + 3][current_state.col - 2] = terreno[14];
			matriz[current_state.fil + 3][current_state.col - 3] = terreno[15];
		break;
		case sureste:
			matriz[current_state.fil][current_state.col + 1] = terreno[1];
			matriz[current_state.fil + 1][current_state.col + 1] = terreno[2];
			matriz[current_state.fil + 1][current_state.col] = terreno[3];
			matriz[current_state.fil][current_state.col + 2] = terreno[4];
			matriz[current_state.fil + 1][current_state.col + 2] = terreno[5];
			matriz[current_state.fil + 2][current_state.col + 2] = terreno[6];
			matriz[current_state.fil + 2][current_state.col + 1] = terreno[7];
			matriz[current_state.fil + 2][current_state.col] = terreno[8];
			matriz[current_state.fil][current_state.col + 3] = terreno[9];
			matriz[current_state.fil + 1][current_state.col + 3] = terreno[10];
			matriz[current_state.fil + 2][current_state.col + 3] = terreno[11];
			matriz[current_state.fil + 3][current_state.col + 3] = terreno[12];
			matriz[current_state.fil + 3][current_state.col + 2] = terreno[13];
			matriz[current_state.fil + 3][current_state.col + 1] = terreno[14];
			matriz[current_state.fil + 3][current_state.col] = terreno[15];
		break;
		case este:
			matriz[current_state.fil - 1][current_state.col + 1] = terreno[1];
			matriz[current_state.fil][current_state.col + 1] = terreno[2];
			matriz[current_state.fil + 1][current_state.col + 1] = terreno[3];
			matriz[current_state.fil - 2][current_state.col + 2] = terreno[4];
			matriz[current_state.fil - 1][current_state.col + 2] = terreno[5];
			matriz[current_state.fil][current_state.col + 2] = terreno[6];
			matriz[current_state.fil + 1][current_state.col + 2] = terreno[7];
			matriz[current_state.fil + 2][current_state.col + 2] = terreno[8];
			matriz[current_state.fil - 3][current_state.col + 3] = terreno[9];
			matriz[current_state.fil - 2][current_state.col + 3] = terreno[10];
			matriz[current_state.fil - 1][current_state.col + 3] = terreno[11];
			matriz[current_state.fil][current_state.col + 3] = terreno[12];
			matriz[current_state.fil + 1][current_state.col + 3] = terreno[13];
			matriz[current_state.fil + 2][current_state.col + 3] = terreno[14];
			matriz[current_state.fil + 3][current_state.col + 3] = terreno[15];
		break;
		case noreste:
			matriz[current_state.fil - 1][current_state.col] = terreno[1];
			matriz[current_state.fil - 1][current_state.col + 1] = terreno[2];
			matriz[current_state.fil][current_state.col + 1] = terreno[3];
			matriz[current_state.fil - 2][current_state.col] = terreno[4];
			matriz[current_state.fil - 2][current_state.col + 1] = terreno[5];
			matriz[current_state.fil - 2][current_state.col + 2] = terreno[6];
			matriz[current_state.fil - 1][current_state.col + 2] = terreno[7];
			matriz[current_state.fil][current_state.col + 2] = terreno[8];
			matriz[current_state.fil - 3][current_state.col] = terreno[9];
			matriz[current_state.fil - 3][current_state.col + 1] = terreno[10];
			matriz[current_state.fil - 3][current_state.col + 2] = terreno[11];
			matriz[current_state.fil - 3][current_state.col + 3] = terreno[12];
			matriz[current_state.fil - 2][current_state.col + 3] = terreno[13];
			matriz[current_state.fil - 1][current_state.col + 3] = terreno[14];
			matriz[current_state.fil][current_state.col + 3] = terreno[15];
		break;
	}
}
