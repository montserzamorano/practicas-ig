// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Montserrat Rodríguez Zamorano
// ** Práctica 4  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica3.hpp"
#include "grafo-escena.hpp"


using namespace std ;

static Objeto3D* objetoActivo4 = nullptr ;
static float anguloActual = 0.0 ;
static ColeccionFuentesP4 * luces = nullptr;
static float incremento = 1.0;
FuenteDireccional *dir;

// ---------------------------------------------------------------------
// Función para implementar en la práctica 4 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P4_Inicializar(  )
{
   cout << "Creando objetos de la práctica 4 .... " << flush ;

   objetoActivo4 = new EscenaObjetosLuces();
   luces = new ColeccionFuentesP4();
   dir = (FuenteDireccional *)luces->ptrFuente(0);

   cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 4 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el grado de libertad o incrementar/decrementar valores
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P4_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
   bool res = false  ; // valor devuelto: es true solo cuando se ha procesado alguna tecla

   switch ( toupper( tecla ) )
   {
      case 'G' :
         // conmutar entre el angulo alpha y el angulo beta
         if(anguloActual==0){
           anguloActual=1;
         }
         else{
           anguloActual=0;
         }
         cout << "Ángulo actual: " << anguloActual << endl;
         break ;

      case '>' :
          dir->variarAngulo(anguloActual,incremento);
          //cout << "Aumentar el ángulo actual: " << anguloActual << endl;
      case '<' :
         dir->variarAngulo(anguloActual, incremento);
         //cout << "Decrementar el ángulo actual: " << anguloActual << endl;
         break ;
      default :
         break ;
   }
   return res ;

}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 4  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P4_DibujarObjetos( ContextoVis & cv )
{
  glEnable(GL_LIGHTING);
  luces->activarTodas();
  objetoActivo4->visualizarGL(cv);
  glDisable(GL_LIGHTING);
}
