// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Montserrat Rodríguez Zamorano
// ** Declaraciones de la clase FuenteLuz
// *********************************************************************

#ifndef IG_FUENTELUZ_HPP
#define IG_FUENTELUZ_HPP

#include <vector>
#include "aux.hpp"
#include "matrices-tr.hpp"
#include "tuplasg.hpp"

using namespace std;

class FuenteLuz{
  public:
    Tupla4f posvec ; //posicion (si w = 1) o vetor de direccion (si w = 0)
    Tupla4f colores[3] ; //colores 0=ambiental, 1=difuso, 2=especular
    float longi, lati ; //angulos de rotacion (si direccional)

    void activar(int i) ; //activarla
}

#endif
