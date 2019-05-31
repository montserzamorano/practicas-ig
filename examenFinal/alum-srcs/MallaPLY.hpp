// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Montserrat Rodríguez Zamorano
// ** Declaraciones de la clase MallaPLY
// **
// *********************************************************************

#ifndef IG_MALLAPLY_HPP
#define IG_MALLAPLY_HPP

#include <vector>          // usar std::vector
#include <string>

#include "MallaInd.hpp"   // declaración de 'Objeto3D'
// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaPLY : public MallaInd
{
   public:
      // crea una malla de revolucion
      MallaPLY( const std::string & nombre_arch ) ;
      //setters
      void setVertices(std::vector <float> vertices) ;
      void setCaras(std::vector <int> caras) ;
} ;

#endif
