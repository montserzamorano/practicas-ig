// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Montserrat Rodríguez Zamorano
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#ifndef IG_MALLAIND_HPP
#define IG_MALLAIND_HPP

#include <vector>          // usar std::vector

#include "Objeto3D.hpp"   // declaración de 'Objeto3D'

using namespace std;
// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaInd : public Objeto3D
{
   protected:
      vector <Tupla3f> vertices ;
      vector <Tupla3i> caras ;
      vector <Tupla3f> normales_vertices ;
      vector <Tupla3f> normales_caras ;
      vector <Tupla3f> col_ver ;
      //vector <Tupla3f> col_tri ;
      bool modoVBO = false ;
      GLuint id_vbo_ver ; //identificador del VBO con la tabla de vertices
      GLuint id_vbo_tri ; //identificador del VBO con la tabla de caras
      GLuint id_vbo_col_ver ; //identificador del VBO con la tabla de colores vertices
      GLuint id_vbo_norm_ver ;
      //GLuint id_vbo_col_tri ; //identificador del VBO con la tabla de colores caras


      // calculo de las normales de esta malla
      Tupla3f normalizar(Tupla3f tupla);
      Tupla3f hallarNormal(Tupla3f tupla1, Tupla3f tupla2);
      void calcular_normales();
      GLuint VBO_Crear(GLuint tipo, GLuint tamanio, GLvoid * puntero);
      void crearVBOs();
      // visualizar con 'draw elements', en modo inmediato
      void visualizarDE_MI( ContextoVis & cv );
      // visualizar con 'draw elements', en modo diferido (con VBOS)
      void visualizarDE_VBOs( ContextoVis & cv );
      void visualizarDE_VBOsAtrVer( ContextoVis & cv );
      void fijarColorNodo( const Tupla3f & nuevo_color );

   public:
      // crea una malla vacía (nombre: "malla indexada nueva vacía")
      MallaInd() ;
      // crea una malla vacía con un nombre concreto:
      MallaInd( const string & nombreIni );
      // visualizar el objeto con OpenGL
      virtual void visualizarGL( ContextoVis & cv) ;

      void setVertices(vector <Tupla3f> v);
      void setCaras(vector <Tupla3i> c);

} ;
// ---------------------------------------------------------------------

class Cubo : public MallaInd
{
   public:
      Cubo();
};
// ---------------------------------------------------------------------

class Tetraedro : public MallaInd
{
   public:
      Tetraedro();
};


#endif
