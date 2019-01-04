// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Montserrat Rodríguez Zamorano
// ** Declaraciones de la clase MallaRevol
// ** Clse mallas indexadas obtenidas de un perfil por revolución
// *********************************************************************

#ifndef IG_MALLAREVOL_HPP
#define IG_MALLAREVOL_HPP

#include <vector>          // usar std::vector
#include <string>

#include "MallaInd.hpp"   // declaración de 'Objeto3D'

using namespace std;
// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaRevol : public MallaInd
{
   private:

   unsigned
      nper , // numero de perfiles
      nvp  ; // numero de vertices por perfil

   protected:
     //constructor por defecto (vacío)
     MallaRevol();
      //crear la malla de revolucion a partir del perfil original
      //(el numero de vertices M, es el número de tuplas del vector)
      // Método que crea las tablas, vértices y triángulos
      void crearMallaRevol(const vector <Tupla3f> &perfil_original, //vertices
                          const unsigned nperfiles, //numero de perfiles
                          const bool crear_tapas, //true para crear tapas
                          const bool cerrar_malla //true para cerrar la malla
                          );


   public:
      // crea una malla de revolucion
      Tupla3f rotarY(Tupla3f punto, float angulo_rotacion);
      MallaRevol( const string & nombre_arch,
                  const unsigned nperfiles,
                  const bool     crear_tapas,
                  const bool     cerrar_malla ) ;
      void setnper(unsigned n){nper=n;}
      void setnvp(unsigned n){nvp = n;}
} ;

//clases mallas indexadas por revolucion de un perfil generado proceduralmente

class Cilindro : public MallaRevol{
  public:
    //constructor: crea el perfil original y llama a crearMalla
    //la base tiene el centro en el origen, el radio y la altura son 1
    Cilindro(const int num_verts_per, //numero de vertices del perfil original (M)
      const unsigned nperfiles, //numero de perfiles (N)
      const bool crear_tapas, //true para crear tapas
      const bool cerrar_malla //true para cerrar la malla
    );
};

class Cono : public MallaRevol{
  public:
    //constructor: crea el perfil original y llama a crearMalla
    //la base tiene el centro en el origen, el radio y altura son 1
    Cono(const int num_verts_per, //numero de vertices del perfil original(M)
      const unsigned nperfiles, //numero de perfiles (N)
      const bool crear_tapas, //true para crear tapas
      const bool cerrar_malla //true para cerrar la malla
    );
};

class Esfera : public MallaRevol{
  public:
    //constructor: crea el perfil original y llama a crearMalla
    //la esfera tiene el centro en el origen, el radio es la unidad
    Esfera(const int num_verts_per, //numero de vertices del perfil original(M)
      const unsigned nperfiles, //numero de perfiles (N)
      const bool crear_tapas, //true para crear tapas
      const bool cerrar_malla //true para cerrar la malla
    );
};

//AÑADIDOS PARA MODELO JERARQUICO

class ConoTruncado : public MallaRevol{
  public:
    //constructor: crea el perfil original y llama a crearMalla
    //la base tiene el centro en el origen, el radio y altura son 1
    ConoTruncado(float radioBase, float radioTapa,
      const int num_verts_per, //numero de vertices del perfil original(M)
      const unsigned nperfiles, //numero de perfiles (N)
      const bool crear_tapas, //true para crear tapas
      const bool cerrar_malla //true para cerrar la malla
    );
};


#endif
