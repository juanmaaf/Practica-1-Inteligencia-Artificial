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
      paso_no_permitido = false;
      tamanio = size;
      precipicios_pintados = false;
      bateriaMax = 5000;
      usarMatrizGrande = false;
      filaMatrizNoPosicionado = columnaMatrizNoPosicionado = 99;
      //paso_elegido = 0;

      vector<unsigned int> columnaMVP(size, 0);
      for(int i = 0; i < size; i++){
        matrizVecesPasadas.push_back(columnaMVP);
      }

      vector<unsigned char> columnaMNP(size*2 + 1, '?');
      for(int i = 0; i < size*2 + 1; i++){
        matrizNoPosicionado.push_back(columnaMNP);
      }

      vector<unsigned int> columnaMVPNP(size*2 + 1, 0);
      for(int i = 0; i < size*2 + 1; i++){
        matrizVecesPasadasNoPosicionado.push_back(columnaMVPNP);
      }

    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    
    void contabilizaPasoPorCasilla(const state current_state, vector< vector<unsigned int> > &matriz);
    void printMatrizVecesPasadas(const int tam, vector< vector<unsigned int> > &matriz);
    void printMatrizNoPosicionado(const int tam, vector< vector<unsigned char> > &matriz);

    void pintaPrecipicios(const int tam, vector< vector<unsigned char> > &matriz);
    void pintaVision(const state current_state, const vector<unsigned char> terreno, vector< vector<unsigned char> > &matriz);

    void reiniciaMatrizChar(const int tam, vector< vector<unsigned char> > &matriz);
    void reiniciaMatrizInt(const int tam, vector< vector<unsigned int> > &matriz);

    //int encontrarCasillasUtiles(const vector<unsigned char> terreno, const unsigned char tipo);

  private:
  
  // Declarar aquí las variables de estado
  
  state current_state;
  Action last_action;
  bool bien_situado;
  bool tiene_bikini;
  bool tiene_zapatillas;
  bool paso_no_permitido;
  int tamanio;
  bool precipicios_pintados;
  int bateriaMax;
  bool usarMatrizGrande;
  int filaMatrizNoPosicionado;
  int columnaMatrizNoPosicionado;
  //int paso_elegido;

  vector< vector<unsigned int> > matrizVecesPasadas;
  vector< vector<unsigned char> > matrizNoPosicionado;
  vector< vector<unsigned int> > matrizVecesPasadasNoPosicionado;
};

#endif
