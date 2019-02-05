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

      //normales
      vector <Tupla3f> normales_vertices ;
      vector <Tupla3f> normales_caras ;

      //colores
      vector <Tupla3f> col_ver ;
      vector <Tupla3f> col_tri ;

      //textura
      vector <Tupla2f> cctt; //tabla de coords de textura

      //identificadores y VBOs
      bool modoVBO = false ;
      GLuint id_vbo_ver ; //identificador del VBO con la tabla de vertices
      GLuint id_vbo_tri ; //identificador del VBO con la tabla de caras
      GLuint id_vbo_col_ver ; //identificador del VBO con la tabla de colores vertices
      GLuint id_vbo_norm_ver ;
      GLuint id_vbo_cctt ; //identificador de VBO de la tabla de texturas

      //tamaños
      unsigned num_tri; //caras.size()
      unsigned num_ver; //vertices.size()
      unsigned tam_cctt;

      // calculo de las normales de esta malla
      Tupla3f normalizar(Tupla3f tupla);
      Tupla3f hallarNormal(Tupla3f tupla1, Tupla3f tupla2);
      void calcular_normales();

      //////////////////// visualizacion //////////////////

      //vbo unico
      GLuint VBO_Crear(GLuint tipo, GLuint tamanio, GLvoid * puntero);
      void crearVBOs();

      //establece el modo
      void setPolygonMode(ContextoVis & cv);
      //INMEDIATO
      // visualizar con 'draw elements', en modo inmediato
      void visualizarDE();
      void visualizarDE_MI( ContextoVis & cv );
      // visualizar con 'draw elements' con textura
      void visualizarDE_NT( ContextoVis & cv );
      //DIFERIDO
      // visualizar con 'draw elements', en modo diferido (con VBOS)
      void visualizarVBOs(); //aux
      void visualizarDE_VBOs( ContextoVis & cv );
      //void visualizarVBOs_NT( ContextoVis & cv );
      void visualizarDE_Plano( ContextoVis & cv );
      // visualizar con 'draw elements', en modo diferido usando texturas
      //void visualizarBEAtrTri();
      //void visualizarBEAtrVer();

      // colores
      void fijarColorNodo( const Tupla3f & nuevo_color );
      //setters
      void setVertices(vector <Tupla3f> v);
      void setCaras(vector <Tupla3i> c);
      void setLineasPuntos(float grosorL, float grosorP);

   public:
      // crea una malla vacía (nombre: "malla indexada nueva vacía")
      MallaInd() ;
      // crea una malla vacía con un nombre concreto:
      MallaInd( const string & nombreIni );
      // visualizar el objeto con OpenGL
      virtual void visualizarGL( ContextoVis & cv) ;

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
