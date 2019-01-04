// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Montserrat Rodríguez Zamorano
// ** Declaraciones de la clase ColeccionFL
// *********************************************************************

#ifndef IG_COLECCIONFL_HPP
#define IG_COLECCIONFL_HPP

#include "FuenteLuz.hpp"

using namespace std;

class ColeccionFL{
  public:
    vector <FuenteLuz *> fuentes ; //vector fuentes de luz
    void activar( ) ; //activa todas las fuentes de luz
}

#endif
