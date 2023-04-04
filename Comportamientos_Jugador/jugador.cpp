#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;



Action ComportamientoJugador::think(Sensores sensores){

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

	// Pintamos los Precipicios -> 3 de anchcura en cada lado

	if(!precipicios_pintados){
		pintaPrecipicios(tamanio, mapaResultado);
		precipicios_pintados = true;
	}

	// Si se activa el sensor de Reinicio -> Inicializamos todo de nuevo

	if(sensores.reset){
		bien_situado = false;
		tiene_zapatillas = false;
		tiene_bikini = false;
		if(sensores.nivel != 0){
			reiniciaMatrizChar(tamanio*2 + 1, matrizNoPosicionado);
			reiniciaMatrizInt(tamanio*2 +1, matrizVecesPasadasNoPosicionado);
			usarMatrizGrande = false;
		}
	}
	
	// Si no estamos bien situados pintaremos en la matriz Doble, situándonos inicialmente en el centro

	if(sensores.nivel != 0 && !bien_situado && !usarMatrizGrande){
		current_state.fil = tamanio;
		current_state.col = tamanio;
		current_state.brujula = norte;
		usarMatrizGrande = true;
	}

	if(sensores.nivel == 0){
		current_state.fil = sensores.posF;
		current_state.col= sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;
	}
	else if (sensores.terreno[0] == 'G' && !bien_situado){
		// Para poder pintar en Mapa resultado todo lo recorrido sin estar posicionados, debemos almacenar
		// la fila y columna en la que acabamos en la matriz doble + 1.

		filaMatrizNoPosicionado = current_state.fil;
		columnaMatrizNoPosicionado = current_state.col;

		current_state.fil = sensores.posF;
		current_state.col= sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;
		
		for(int i = 0; i < tamanio; i++){
				for(int j = 0; j < tamanio; j++){
					if(matrizNoPosicionado[i+(filaMatrizNoPosicionado-current_state.fil)][j+(columnaMatrizNoPosicionado-current_state.col)] != '?'){
						mapaResultado[i][j] = matrizNoPosicionado[i+(filaMatrizNoPosicionado-current_state.fil)][j+(columnaMatrizNoPosicionado-current_state.col)];
					}
					if(matrizVecesPasadasNoPosicionado[i+(filaMatrizNoPosicionado-current_state.fil)][j+(columnaMatrizNoPosicionado-current_state.col)] != 0){
						matrizVecesPasadas[i][j] += matrizVecesPasadasNoPosicionado[i+(filaMatrizNoPosicionado-current_state.fil)][j+(columnaMatrizNoPosicionado-current_state.col)];
					}
				}
		}

		//Tenemos un error. No nos pinta bien después de un RESET 
		// SOLUCIONADO -> Faltaba línea 112 "current_state.brujula = norte"
	}

	// SI está bien situado. vamos a pintar en la matriz MapaResultado
	// Si NO, pintaremos en la matriz AUXILIAR para, cuando nos situemos correctamente, poder pintar en la matriz original.

	if (bien_situado){
		contabilizaPasoPorCasilla(current_state, matrizVecesPasadas);
		pintaVision(current_state, sensores.terreno, mapaResultado);
	}else{
		contabilizaPasoPorCasilla(current_state, matrizVecesPasadasNoPosicionado);
		pintaVision(current_state, sensores.terreno, matrizNoPosicionado);
	}

	// MOVIMIENTO

	// Buscamos una casilla especial en función de la Visión
	
	casillaEspecialEncontrada = false;
	brujulaNSOE = false;

	if(current_state.brujula == norte || current_state.brujula == sur || current_state.brujula == este || current_state.brujula == oeste){
		brujulaNSOE = true;
	}

	if(!bien_situado){ // Máxima Prioridad a Posicionarse, para pintar lo más rápido posible en Matriz Resultado
		encontrarCasillaUtil(sensores.terreno, 'G', accion, casillaEspecialEncontrada);
	}
	if(!tiene_bikini && !casillaEspecialEncontrada){
		encontrarCasillaUtil(sensores.terreno, 'K', accion, casillaEspecialEncontrada);
	} 
	if (!tiene_zapatillas && !casillaEspecialEncontrada){
		encontrarCasillaUtil(sensores.terreno, 'D', accion, casillaEspecialEncontrada);
	} 
	if(!casillaEspecialEncontrada){
		encontrarCasillaUtil(sensores.terreno, 'X', accion, casillaEspecialEncontrada);
	}
	if(casillaEspecialEncontrada){
		if(accion == actFORWARD){
			desplazamientoElegido = 2;
		}else if(accion == actTURN_SL){
			desplazamientoElegido = 1;
		}else{
			desplazamientoElegido = 3;
		}
	} else{ //Podemos tener que atravesar por un hueco, sea de muros o precipicios dentro del mapa. Caso de MUROS
			if(sensores.terreno[1] != 'M' && sensores.terreno[5] == 'M' && brujulaNSOE){
				desplazamientoElegido = 1;
				accion = actTURN_SL;
			} else if(sensores.terreno[3] != 'M' && sensores.terreno[7] == 'M' && brujulaNSOE){
				desplazamientoElegido = 3;
				accion = actTURN_SR;
			} 
			
			// Caso de Precipicios -> Importante para exámen. Puede haber huecos de precipicios como de muros
			else if(sensores.terreno[1] != 'P' && sensores.terreno[5] == 'P' && brujulaNSOE){
				desplazamientoElegido = 1;
				accion = actTURN_SL;
			} else if(sensores.terreno[3] != 'P' && sensores.terreno[7] == 'P' && brujulaNSOE){
				desplazamientoElegido = 3;
				accion = actTURN_SR;
			}
			else{ //Default -> TIRA RECTO
				desplazamientoElegido = 2;
				accion = actFORWARD;
			}
	}
	
	paso_no_permitido = false;
	if (sensores.superficie[desplazamientoElegido] == '_'){

		//accion = actFORWARD;

		switch(sensores.terreno[desplazamientoElegido]){
			// Los tipos no especificados no requieren condiciones - Para los muros, ya tenemos el sensor de colisión
			case 'B':
				if(!tiene_zapatillas || last_action == actFORWARD){
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
		// Hay un Aldeano o un Lobo -> No se puede avanzar a esa casilla -> PASO NO PERMITIDO
		paso_no_permitido = true;
	}

	// Si estamos en la casilla de Recarga, nos quedamos hasta que se recargue completamente
	if(sensores.terreno[0] == 'X' && sensores.bateria != bateriaMax && sensores.bateria < umbralRecarga){
		accion = actIDLE;
	}

	// PASO NO PERMITIDO o Colisión Por Muro o RESET-> Movimiento Aleatorio - ¿NO ALEATORIO?
	else if(paso_no_permitido or sensores.reset or sensores.colision){
		if(bien_situado){
				elegirMovimiento(matrizVecesPasadas, current_state, accion);
		} else{
				elegirMovimiento(matrizVecesPasadasNoPosicionado, current_state, accion);
		}
	}

	last_action = accion;

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

void ComportamientoJugador::printMatrizNoPosicionado(const int tam, vector< vector<unsigned char> > &matriz){
	for(int i = 0; i < tam*2 + 1; i++){
		for(int j = 0; j < tam*2 +1; j++){
			cout << matriz[i][j];
		}
		cout << endl;
	}
}

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

void ComportamientoJugador::reiniciaMatrizChar(const int tam, vector< vector<unsigned char> > &matriz){
	for(int i = 0; i < tam; i++){
		for(int j = 0; j < tam; j++){
			matriz[i][j] = '?';
		}
	}
}

void ComportamientoJugador::reiniciaMatrizInt(const int tam, vector< vector<unsigned int> > &matriz){
	for(int i = 0; i < tam; i++){
		for(int j = 0; j < tam; j++){
			matriz[i][j] = 0;
		}
	}
}

void ComportamientoJugador::encontrarCasillaUtil(const vector<unsigned char> terreno, const char tipo, Action &accion, bool &casillaEspecialEncontrada){
	int numero;
	for(int i = 0; i < terreno.size(); i++){
		if(terreno[i] == tipo){
			numero = i;
			casillaEspecialEncontrada = true;
		}
	}
	if(numero == 1 || numero == 4 || numero == 9){
		accion = actTURN_SL;
	} else if(numero == 3 || numero == 8 || numero == 15){
		accion = actTURN_SR;
	} else{
		accion = actFORWARD;
	}
}

void ComportamientoJugador::elegirMovimiento(const vector< vector<unsigned int> > matriz, const state current_state, Action &accion){
	//elegirMovimientoAleatorio(accion);
	Action auxiliar = accion;

	int sumaCuadrante1 = 0;
	int sumaCuadrante2 = 0;
	int sumaCuadrante3 = 0;
	int sumaCuadrante4 = 0;

	for(int i = 0; i < current_state.fil; i++){
		for(int j = 0; j < current_state.col; j++){
			sumaCuadrante1 += matriz[i][j];
		}
	}
	for(int i = 0; i < current_state.fil; i++){
		for(int j = current_state.col + 1; j < matriz[0].size(); j++){
			sumaCuadrante2 += matriz[i][j];
		}
	}
	for(int i = current_state.fil + 1; i < matriz[0].size(); i++){
		for(int j = 0; j < current_state.col; j++){
			sumaCuadrante3 += matriz[i][j];
		}
	}
	for(int i = current_state.fil + 1; i < matriz[0].size(); i++){
		for(int j = current_state.col + 1; j < matriz[0].size(); j++){
			sumaCuadrante4 += matriz[i][j];
		}
	}
	sumaCuadrantes.push_back(sumaCuadrante1);
	sumaCuadrantes.push_back(sumaCuadrante2);
	sumaCuadrantes.push_back(sumaCuadrante3);
	sumaCuadrantes.push_back(sumaCuadrante4);

	int menor = sumaCuadrante1;
	int idMenor = 1;

	for(int i = 1; i < sumaCuadrantes.size(); i++){
		if(sumaCuadrantes[i] < menor){
			menor = sumaCuadrantes[i];
			idMenor = i + 1;
		}
	}

	switch(current_state.brujula){
		case norte:
			if(idMenor == 1 && auxiliar != actTURN_SL){
				accion = actTURN_SL;
			} else if(idMenor == 2 && auxiliar != actTURN_SR){
				accion = actTURN_SR;
			} else if(idMenor == 3 && auxiliar != actTURN_BL){
				accion = actTURN_BL;
			} else if(idMenor == 4 && auxiliar != actTURN_BR){
				accion = actTURN_BR;
			} else{
				elegirMovimientoAleatorio(accion);
			}
		break;
		case noreste:
			if(idMenor == 1 && auxiliar != actTURN_SL){
				accion = actTURN_SL;
			} else if(idMenor == 2 && auxiliar != actFORWARD){
				accion = actFORWARD;
			} else if(idMenor == 3 && auxiliar != actTURN_BL){
				accion = actTURN_BL;
			} else if(idMenor == 4 && auxiliar != actTURN_SR){
				accion = actTURN_SR;
			} else{
				elegirMovimientoAleatorio(accion);
			}
		break;
		case este:
			if(idMenor == 1 && auxiliar != actTURN_BL){
				accion = actTURN_BL;
			} else if(idMenor == 2 && auxiliar != actTURN_SL){
				accion = actTURN_SL;
			} else if(idMenor == 3 && auxiliar != actTURN_BR){
				accion = actTURN_BR;
			} else if(idMenor == 4 && auxiliar != actTURN_SR){
				accion = actTURN_SR;
			} else{
				elegirMovimientoAleatorio(accion);
			}
		break;
		case sureste:
			if(idMenor == 1 && auxiliar != actTURN_BR){
				actTURN_BR;
			} else if(idMenor == 2 && auxiliar != actTURN_SL){
				actTURN_SL;
			} else if(idMenor == 3 && auxiliar != actTURN_SR){
				actTURN_SR;
			} else if(idMenor == 4 && auxiliar != actFORWARD){
				accion = actFORWARD;
			} else{
				elegirMovimientoAleatorio(accion);
			}
		break;
		case sur:
			if(idMenor == 1 && auxiliar != actTURN_BR){
				accion = actTURN_BR;
			} else if(idMenor == 2 && auxiliar != actTURN_BL){
				accion = actTURN_BL;
			} else if(idMenor == 3 && auxiliar != actTURN_SR){
				accion = actTURN_SR;
			} else if(idMenor == 4 && auxiliar != actTURN_SL){
				accion = actTURN_SL;
			} else{
				elegirMovimientoAleatorio(accion);
			}
		break;
		case suroeste:
			if(idMenor == 1 && auxiliar != actTURN_SR){
				accion = actTURN_SR;
			} else if(idMenor == 2 && auxiliar != actTURN_BR){
				accion = actTURN_BR;
			} else if(idMenor == 3 && auxiliar != actFORWARD){
				accion = actFORWARD;
			} else if(idMenor == 4 && auxiliar != actTURN_SL){ 
				accion = actTURN_SL;
			} else{
				elegirMovimientoAleatorio(accion);
			}
		break;
		case oeste:
			if(idMenor == 1 && auxiliar != actTURN_SR){
				accion = actTURN_SR;
			} else if(idMenor == 2 && auxiliar != actTURN_BR){
				accion = actTURN_BR;
			} else if(idMenor == 3 && auxiliar != actTURN_SL){
				accion = actTURN_SL;
			} else if(idMenor == 4 && auxiliar != actTURN_BL){
				accion = actTURN_BL;
			} else{
				elegirMovimientoAleatorio(accion);
			}
		break;
		case noroeste:
			if(idMenor == 1 && auxiliar != actFORWARD){
				accion = actFORWARD;
			} else if(idMenor == 2 && auxiliar != actTURN_SR){
				accion = actTURN_SR;
			} else if(idMenor == 3 && auxiliar != actTURN_SL){
				accion = actTURN_SL;
			} else if(idMenor == 4 && auxiliar != actTURN_BL){
				accion = actTURN_BL;
			} else{
				elegirMovimientoAleatorio(accion);
			}
		break;
	}
}

void ComportamientoJugador::elegirMovimientoAleatorio(Action &accion){
	Action auxiliar = accion;
	int eleccion = rand()%4;
	switch(auxiliar){
		case actFORWARD:
			//Movimiento, cualquiera menos actForward
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
		break;
		case actTURN_SL:
			//Movimiento, cualquiera menos actTurnSL
			switch(eleccion){
			case 0:
				accion = actFORWARD;
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
		break;
		case actTURN_SR:
			//Movimiento, cualquiera menos actTurnSR
			switch(eleccion){
			case 0:
				accion = actTURN_SL;
			break;
			case 1:
				accion = actFORWARD;
			break;
			case 2:
				accion = actTURN_BL;
			break;
			case 3:
				accion = actTURN_BR;
			break;
		}
		break;
	}
}