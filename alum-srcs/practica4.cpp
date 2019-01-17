// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Montserrat Rodríguez Zamorano
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica3.hpp"


using namespace std ;

static Objeto3D* objetoActivo4 = nullptr ;
static float anguloActual = 0.0 ;


// ---------------------------------------------------------------------
// Función para implementar en la práctica 4 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P4_Inicializar(  )
{
   cout << "Creando objetos de la práctica 4 .... " << flush ;

   // COMPLETAR: práctica 4: inicializar objetos de la práctica 4
   // ....

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
         break ;

      case '>' :
         cout << "Aumentar el ángulo activo." << endl;
         if(anguloActual==0){ //si alpha
           objetoActivo4->aumentarAlpha();
         }
         else{ //si beta
           objetoActivo4->aumentarBeta();
         }
         break ;

      case '<' :
         cout << "Decrementar el ángulo activo." << endl;
         if(anguloActual==0){ //si alpha
           objetoActivo4->decrementarAlpha();
         }
         else{ //si beta
           objetoActivo4->decrementarBeta();
         }
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
  objetoActivo4->visualizarGL();
  glDisable(GL_LIGHTING);
}
