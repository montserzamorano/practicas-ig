// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Montserrat Rodríguez Zamorano
// ** Práctica 3  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica3.hpp"
#include "grafo-escena.hpp"


using namespace std ;

static unsigned objetoActivo3 = 0 ;
static constexpr int numObjetos3 = 1 ;
static Lampara * objetos3[numObjetos3] = {nullptr};
unsigned gradosLibertad = 0;
static unsigned gradoLibertadActivo = 0 ;
bool animaciones_activadas = false;

ColeccionFuentesP4 *luces;


// ---------------------------------------------------------------------
// Función para implementar en la práctica 1 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P3_Inicializar(  )
{
   cout << "Creando objetos de la práctica 3 .... " << flush ;

   objetos3[0] = new Lampara();
   gradosLibertad = objetos3[0]->numParametros();
   luces = new ColeccionFuentesP4();

   cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 1 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el grado de libertad o incrementar/decrementar valores
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P3_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
   bool res = true ; // valor devuelto (true si se ha procesado tecla)

   switch ( toupper( tecla ) )
   {
      case 'O' :
         objetoActivo3 = (objetoActivo3+1)%numObjetos3;
         cout << "práctica 3: nuevo objeto activo es: " << objetoActivo3 << endl;

         break ;

      case 'A' :
         if(animaciones_activadas){
           animaciones_activadas = false;
           cout << "Animaciones desactivadas" << endl;

         }
         else{
           animaciones_activadas = true;
           FijarFuncDesocupado( FGE_Desocupado );
           cout << "Animaciones activadas" << endl;
         }
         break ;

      case 'G' :
         gradoLibertadActivo = (gradoLibertadActivo+1)%gradosLibertad;
         cout << "Grado de libertad activado: " << gradoLibertadActivo << endl;
         cout << (objetos3[0]->leerPtrParametro(gradoLibertadActivo))->leer_descripcion()<< endl;
         break ;

      case 'R' :
         objetos3[objetoActivo3]->reiniciar();
         animaciones_activadas = false;
         cout << "Variables reseteadas" << endl;
         break ;

      case '>' :
          if(animaciones_activadas){
            (objetos3[0]->leerPtrParametro(gradoLibertadActivo))->acelerar();
          }
          else{
            (objetos3[0]->leerPtrParametro(gradoLibertadActivo))->incrementar();
          }
         break ;

      case '<' :
        if(animaciones_activadas){
          (objetos3[0]->leerPtrParametro(gradoLibertadActivo))->decelerar();
        }
        else{
          (objetos3[0]->leerPtrParametro(gradoLibertadActivo))->decrementar();
        }
         break ;
      default :
         break ;

   }
   return res ;
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 3  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P3_DibujarObjetos( ContextoVis & cv )
{
  glEnable(GL_LIGHTING);
  luces->activarTodas();
  objetos3[0]->visualizarGL(cv);
  glDisable(GL_LIGHTING);
}

//--------------------------------------------------------------------------

bool P3_FGE_Desocupado()
{
   if(!animaciones_activadas){
     return false;
   }
   //modificar los parametros animables segun sus velocidades actuales
   objetos3[0]->siguienteCuadro();
   //forzar llamada a VisualizarFrame en la proxima iteracion del bucle
   redibujar_ventana = true;
   //terminar, manteniendo activada la gestion del evento
   return true;
}
