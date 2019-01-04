// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Montserrat Rodríguez Zamorano
// ** Implementación de la clase 'MallaPLY'
// **
// *********************************************************************


#include <cassert>
#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include "MallaPLY.hpp"

using namespace std ;

// *****************************************************************************

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
   ponerNombre(string("malla leída del archivo '") + nombre_arch + "'" );
   vector <float> vertices;
   vector <int> caras;
   //lee un archivo ply y lo carga en vertices y caras
   //convertimos nombre_arch a cadena de caracteres para no tener problemas
   //probar sin convertir
   const char *nombre_arch_c = nombre_arch.c_str();
   ply::read(nombre_arch_c, vertices, caras);
   setVertices(vertices);
   setCaras(caras);
   // calcular la tabla de normales
   calcular_normales();
}
//guardamos los vertices en tuplas
void MallaPLY::setVertices(vector <float> vertices){
  for(int i=0; i<vertices.size(); i+=3){
    Tupla3f t(vertices.at(i),vertices.at(i+1),vertices.at(i+2));
    this->vertices.push_back(t);
  }
}
//guardamos las caras en tuplas
void MallaPLY::setCaras(vector <int> caras){
  for(int i=0; i<caras.size(); i+=3){
    Tupla3i t(caras.at(i),caras.at(i+1), caras.at(i+2));
    this->caras.push_back(t);
  }
}


// *****************************************************************************
