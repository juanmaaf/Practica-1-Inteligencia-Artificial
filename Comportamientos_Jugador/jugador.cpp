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

	if ((sensores.terreno[0] == 'G' and !bien_situado)){
		current_state.fil = sensores.posF;
		current_state.col= sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;
	}

	if (bien_situado && !sensores.colision){
		if(sensores.nivel == 0){
			current_state.fil = sensores.posF;
			current_state.col= sensores.posC;
			current_state.brujula = sensores.sentido;
		}
		mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
		switch(current_state.brujula){
			case norte:
				mapaResultado[current_state.fil - 1][current_state.col - 1] = sensores.terreno[1];
				mapaResultado[current_state.fil - 1][current_state.col] = sensores.terreno[2];
				mapaResultado[current_state.fil - 1][current_state.col + 1] = sensores.terreno[3];
				mapaResultado[current_state.fil - 2][current_state.col - 2] = sensores.terreno[4];
				mapaResultado[current_state.fil - 2][current_state.col - 1] = sensores.terreno[5];
				mapaResultado[current_state.fil - 2][current_state.col] = sensores.terreno[6];
				mapaResultado[current_state.fil - 2][current_state.col + 1] = sensores.terreno[7];
				mapaResultado[current_state.fil - 2][current_state.col + 2] = sensores.terreno[8];
				mapaResultado[current_state.fil - 3][current_state.col - 3] = sensores.terreno[9];
				mapaResultado[current_state.fil - 3][current_state.col - 2] = sensores.terreno[10];
				mapaResultado[current_state.fil - 3][current_state.col - 1] = sensores.terreno[11];
				mapaResultado[current_state.fil - 3][current_state.col] = sensores.terreno[12];
				mapaResultado[current_state.fil - 3][current_state.col + 1] = sensores.terreno[13];
				mapaResultado[current_state.fil - 3][current_state.col + 2] = sensores.terreno[14];
				mapaResultado[current_state.fil - 3][current_state.col + 3] = sensores.terreno[15];
			break;
			case noroeste:
				mapaResultado[current_state.fil][current_state.col - 1] = sensores.terreno[1];
				mapaResultado[current_state.fil - 1][current_state.col - 1] = sensores.terreno[2];
				mapaResultado[current_state.fil - 1][current_state.col] = sensores.terreno[3];
				mapaResultado[current_state.fil][current_state.col - 2] = sensores.terreno[4];
				mapaResultado[current_state.fil - 1][current_state.col - 2] = sensores.terreno[5];
				mapaResultado[current_state.fil - 2][current_state.col - 2] = sensores.terreno[6];
				mapaResultado[current_state.fil - 2][current_state.col - 1] = sensores.terreno[7];
				mapaResultado[current_state.fil - 2][current_state.col] = sensores.terreno[8];
				mapaResultado[current_state.fil][current_state.col - 3] = sensores.terreno[9];
				mapaResultado[current_state.fil - 1][current_state.col - 3] = sensores.terreno[10];
				mapaResultado[current_state.fil - 2][current_state.col - 3] = sensores.terreno[11];
				mapaResultado[current_state.fil - 3][current_state.col - 3] = sensores.terreno[12];
				mapaResultado[current_state.fil - 3][current_state.col - 2] = sensores.terreno[13];
				mapaResultado[current_state.fil - 3][current_state.col - 1] = sensores.terreno[14];
				mapaResultado[current_state.fil - 3][current_state.col] = sensores.terreno[15];
			break;
			case oeste:
				mapaResultado[current_state.fil + 1][current_state.col - 1] = sensores.terreno[1];
				mapaResultado[current_state.fil][current_state.col - 1] = sensores.terreno[2];
				mapaResultado[current_state.fil - 1][current_state.col - 1] = sensores.terreno[3];
				mapaResultado[current_state.fil + 2][current_state.col - 2] = sensores.terreno[4];
				mapaResultado[current_state.fil + 1][current_state.col - 2] = sensores.terreno[5];
				mapaResultado[current_state.fil][current_state.col - 2] = sensores.terreno[6];
				mapaResultado[current_state.fil - 1][current_state.col - 2] = sensores.terreno[7];
				mapaResultado[current_state.fil - 2][current_state.col - 2] = sensores.terreno[8];
				mapaResultado[current_state.fil + 3][current_state.col - 3] = sensores.terreno[9];
				mapaResultado[current_state.fil + 2][current_state.col - 3] = sensores.terreno[10];
				mapaResultado[current_state.fil + 1][current_state.col - 3] = sensores.terreno[11];
				mapaResultado[current_state.fil][current_state.col - 3] = sensores.terreno[12];
				mapaResultado[current_state.fil - 1][current_state.col - 3] = sensores.terreno[13];
				mapaResultado[current_state.fil - 2][current_state.col - 3] = sensores.terreno[14];
				mapaResultado[current_state.fil - 3][current_state.col - 3] = sensores.terreno[15];
			break;
			case suroeste:
				mapaResultado[current_state.fil + 1][current_state.col] = sensores.terreno[1];
				mapaResultado[current_state.fil + 1][current_state.col - 1] = sensores.terreno[2];
				mapaResultado[current_state.fil][current_state.col - 1] = sensores.terreno[3];
				mapaResultado[current_state.fil + 2][current_state.col] = sensores.terreno[4];
				mapaResultado[current_state.fil + 2][current_state.col - 1] = sensores.terreno[5];
				mapaResultado[current_state.fil + 2][current_state.col - 2] = sensores.terreno[6];
				mapaResultado[current_state.fil + 1][current_state.col - 2] = sensores.terreno[7];
				mapaResultado[current_state.fil][current_state.col - 2] = sensores.terreno[8];
				mapaResultado[current_state.fil + 3][current_state.col] = sensores.terreno[9];
				mapaResultado[current_state.fil + 3][current_state.col - 1] = sensores.terreno[10];
				mapaResultado[current_state.fil + 3][current_state.col - 2] = sensores.terreno[11];
				mapaResultado[current_state.fil + 3][current_state.col - 3] = sensores.terreno[12];
				mapaResultado[current_state.fil + 2][current_state.col - 3] = sensores.terreno[13];
				mapaResultado[current_state.fil + 1][current_state.col - 3] = sensores.terreno[14];
				mapaResultado[current_state.fil][current_state.col - 3] = sensores.terreno[15];
			break;
			case sur:
				mapaResultado[current_state.fil + 1][current_state.col + 1] = sensores.terreno[1];
				mapaResultado[current_state.fil + 1][current_state.col] = sensores.terreno[2];
				mapaResultado[current_state.fil + 1][current_state.col - 1] = sensores.terreno[3];
				mapaResultado[current_state.fil + 2][current_state.col + 2] = sensores.terreno[4];
				mapaResultado[current_state.fil + 2][current_state.col + 1] = sensores.terreno[5];
				mapaResultado[current_state.fil + 2][current_state.col] = sensores.terreno[6];
				mapaResultado[current_state.fil + 2][current_state.col - 1] = sensores.terreno[7];
				mapaResultado[current_state.fil + 2][current_state.col - 2] = sensores.terreno[8];
				mapaResultado[current_state.fil + 3][current_state.col + 3] = sensores.terreno[9];
				mapaResultado[current_state.fil + 3][current_state.col + 2] = sensores.terreno[10];
				mapaResultado[current_state.fil + 3][current_state.col + 1] = sensores.terreno[11];
				mapaResultado[current_state.fil + 3][current_state.col] = sensores.terreno[12];
				mapaResultado[current_state.fil + 3][current_state.col - 1] = sensores.terreno[13];
				mapaResultado[current_state.fil + 3][current_state.col - 2] = sensores.terreno[14];
				mapaResultado[current_state.fil + 3][current_state.col - 3] = sensores.terreno[15];
			break;
			case sureste:
				mapaResultado[current_state.fil][current_state.col + 1] = sensores.terreno[1];
				mapaResultado[current_state.fil + 1][current_state.col + 1] = sensores.terreno[2];
				mapaResultado[current_state.fil + 1][current_state.col] = sensores.terreno[3];
				mapaResultado[current_state.fil][current_state.col + 2] = sensores.terreno[4];
				mapaResultado[current_state.fil + 1][current_state.col + 2] = sensores.terreno[5];
				mapaResultado[current_state.fil + 2][current_state.col + 2] = sensores.terreno[6];
				mapaResultado[current_state.fil + 2][current_state.col + 1] = sensores.terreno[7];
				mapaResultado[current_state.fil + 2][current_state.col] = sensores.terreno[8];
				mapaResultado[current_state.fil][current_state.col + 3] = sensores.terreno[9];
				mapaResultado[current_state.fil + 1][current_state.col + 3] = sensores.terreno[10];
				mapaResultado[current_state.fil + 2][current_state.col + 3] = sensores.terreno[11];
				mapaResultado[current_state.fil + 3][current_state.col + 3] = sensores.terreno[12];
				mapaResultado[current_state.fil + 3][current_state.col + 2] = sensores.terreno[13];
				mapaResultado[current_state.fil + 3][current_state.col + 1] = sensores.terreno[14];
				mapaResultado[current_state.fil + 3][current_state.col] = sensores.terreno[15];
			break;
			case este:
				mapaResultado[current_state.fil - 1][current_state.col + 1] = sensores.terreno[1];
				mapaResultado[current_state.fil][current_state.col + 1] = sensores.terreno[2];
				mapaResultado[current_state.fil + 1][current_state.col + 1] = sensores.terreno[3];
				mapaResultado[current_state.fil - 2][current_state.col + 2] = sensores.terreno[4];
				mapaResultado[current_state.fil - 1][current_state.col + 2] = sensores.terreno[5];
				mapaResultado[current_state.fil][current_state.col + 2] = sensores.terreno[6];
				mapaResultado[current_state.fil + 1][current_state.col + 2] = sensores.terreno[7];
				mapaResultado[current_state.fil + 2][current_state.col + 2] = sensores.terreno[8];
				mapaResultado[current_state.fil - 3][current_state.col + 3] = sensores.terreno[9];
				mapaResultado[current_state.fil - 2][current_state.col + 3] = sensores.terreno[10];
				mapaResultado[current_state.fil - 1][current_state.col + 3] = sensores.terreno[11];
				mapaResultado[current_state.fil][current_state.col + 3] = sensores.terreno[12];
				mapaResultado[current_state.fil + 1][current_state.col + 3] = sensores.terreno[13];
				mapaResultado[current_state.fil + 2][current_state.col + 3] = sensores.terreno[14];
				mapaResultado[current_state.fil + 3][current_state.col + 3] = sensores.terreno[15];
			break;
			case noreste:
				mapaResultado[current_state.fil - 1][current_state.col] = sensores.terreno[1];
				mapaResultado[current_state.fil - 1][current_state.col + 1] = sensores.terreno[2];
				mapaResultado[current_state.fil][current_state.col + 1] = sensores.terreno[3];
				mapaResultado[current_state.fil - 2][current_state.col] = sensores.terreno[4];
				mapaResultado[current_state.fil - 2][current_state.col + 1] = sensores.terreno[5];
				mapaResultado[current_state.fil - 2][current_state.col + 2] = sensores.terreno[6];
				mapaResultado[current_state.fil - 1][current_state.col + 2] = sensores.terreno[7];
				mapaResultado[current_state.fil][current_state.col + 2] = sensores.terreno[8];
				mapaResultado[current_state.fil - 3][current_state.col] = sensores.terreno[9];
				mapaResultado[current_state.fil - 3][current_state.col + 1] = sensores.terreno[10];
				mapaResultado[current_state.fil - 3][current_state.col + 2] = sensores.terreno[11];
				mapaResultado[current_state.fil - 3][current_state.col + 3] = sensores.terreno[12];
				mapaResultado[current_state.fil - 2][current_state.col + 3] = sensores.terreno[13];
				mapaResultado[current_state.fil - 1][current_state.col + 3] = sensores.terreno[14];
				mapaResultado[current_state.fil][current_state.col + 3] = sensores.terreno[15];
			break;
		}
	}

	paso_no_permitido = false;
	if (sensores.superficie[2] == '_'){
		accion = actFORWARD;
		switch(sensores.terreno[2]){
			case 'B':
				if(!tiene_zapatillas){
					paso_no_permitido = true;
				}
			break;
			case 'A':
				if(!tiene_bikini){
					paso_no_permitido = true;
				}
			break;
			//case 'M':
				
			//break;
			case 'P':
				paso_no_permitido = true;
			break;
			//case 'S':
			//break;
			//case 'T':
			//break;
			//case 'G':
			//break;
			case 'K':
				tiene_bikini = true;
			break;
			case 'D':
				tiene_zapatillas = true;
			break;
			//case 'X':
			//break;
		}
	}

	if(paso_no_permitido or sensores.colision or sensores.reset){
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

	last_action = accion;

	// Determinar el efecto de la ultima accion enviada
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
