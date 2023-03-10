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

	if((sensores.terreno[2] == 'T' or sensores.terreno[2] == 'S') and sensores.superficie[2] = '_'){
		action = actFORWARD;
	}else{
		action = actTURN_SR;
	}

	last_action = action;

	// Determinar el efecto de la ultima accion enviada
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
