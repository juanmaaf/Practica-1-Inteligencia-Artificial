#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
using namespace std;

struct state{
  int fil;
  int col;
  Orientacion brujula;
};

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado

      last_action = actIDLE;
      current_state.fil = current_state.col = 99;
      current_state.brujula = norte;
      bien_situado = false;
      tiene_bikini = false;
      tiene_zapatillas = false;
      ha_chocado = false;
      reinicio = false;
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);

  private:
  
  // Declarar aquí las variables de estado
  
  state current_state;
  Action last_action;
  bool bien_situado;
  bool tiene_bikini;
  bool tiene_zapatillas;
  bool ha_chocado;
  bool reinicio;
};

#endif
