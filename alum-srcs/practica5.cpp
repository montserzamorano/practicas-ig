// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Montserrat Rodríguez Zamorano
// ** Práctica 5  (implementaciones)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica5.hpp"
#include "CamaraInter.hpp"
#include "grafo-escena.hpp"
#include "materiales.hpp"

using namespace std ;

// COMPLETAR: práctica 5: declarar variables de la práctica 5 (static)
//    (escena, viewport, el vector de camaras, y las variables
//      que sirven para gestionar el modo arrastrar

//Escena
static Objeto3D* objetoActivo5 = nullptr ;
static ColFuentesLuz *luces = nullptr;

//Camaras
static int camActiva;
static const int numCamaras = 2;
static CamaraInteractiva * camaras[numCamaras] = {nullptr,nullptr};

//Desplazamiento
static float d;
// viewport actual (se recalcula al inicializar y al fijar las matrices)
Viewport viewport ;
// true si se está en modo arrastrar, false si no
static bool modo_arrastrar = false ;

//Posicion anterior del raton
static int xant = 0;
static int yant = 0;


// ---------------------------------------------------------------------

void P5_Inicializar(  int vp_ancho, int vp_alto )
{
   cout << "Creando objetos de la práctica 5 .... " << flush ;
   // inicializar las variables de la práctica 5 (incluyendo el viewport)
   viewport = Viewport(0.0,0.0, vp_ancho, vp_alto);
   float radio = (float) vp_alto/vp_ancho;
   camaras[0] = new CamaraInteractiva(true, radio, 0, 0, {0,0,0},true,80.0,1.8);
   camActiva = 0;

   luces = new ColeccionFuentesP4();
   objetoActivo5 = new EscenaObjetosLuces();


   cout << "hecho." << endl << flush ;
}
// ---------------------------------------------------------------------

void P5_FijarMVPOpenGL( int vp_ancho, int vp_alto )
{
   // actualizar viewport, actualizar y activar la camara actual
   // (en base a las dimensiones del viewport)
   viewport = Viewport(0, 0, vp_ancho, vp_alto);
   viewport.fijarViewport();

   camaras[camActiva]->ratio_yx_vp = (float) vp_alto/vp_ancho; //alto-ancho
   camaras[camActiva]->calcularViewfrustum();
   camaras[camActiva]->activar();


}
// ---------------------------------------------------------------------

void P5_DibujarObjetos( ContextoVis & cv )
{

   // activar las fuentes de luz y visualizar la escena
   //      (se supone que la camara actual ya está activada)
   luces->activar(0);
   if(objetoActivo5!=nullptr){
     objetoActivo5->visualizarGL(cv);
   }
   glDisable(GL_LIGHTING);
}

// ---------------------------------------------------------------------

bool P5_FGE_PulsarTeclaCaracter(  unsigned char tecla ){
   bool result = true ;

   switch ( toupper( tecla ) )
   {
      case 'C':
         // Activar siguiente camara
         camActiva=(camActiva+1)%numCamaras;
         cout << "Cámara activa cambiada a cámara " << camActiva << endl;
         break ;

      case 'V':
         if(camaras[camActiva]->examinar){
           camaras[camActiva]->modoPrimeraPersona();
           cout << "Modo cambiado a primera persona." << endl;
         }
         else{
           camaras[camActiva]->modoExaminar();
           cout << "Modo cambiado a examinar." << endl;
         }

         break ;

      case '+':
         // desplazamiento en Z de la cámara actual (positivo) (desplaZ)
         camaras[camActiva]->desplaZ(d);
         cout << "Desplazamiento en Z de la cámara actual (positivo)" << endl;
         break;

      case '-':
         // desplazamiento en Z de la cámara actual (negativo) (desplaZ)
         camaras[camActiva]->desplaZ(-d);
         cout << "Desplazamiento en Z de la cámara actual (negativo)" << endl;
         break;
         break;

      default:
         result = false ;
         break ;
	}
   return result ;
}
// ---------------------------------------------------------------------

bool P5_FGE_PulsarTeclaEspecial(  int tecla  )
{

   bool result = true ;


   switch ( tecla )
   {
      case GLFW_KEY_LEFT:
         // desplazamiento/rotacion hacia la izquierda (moverHV)
         camaras[camActiva]->moverHV(-d,0);
         cout << "Desplazamiento/rotación hacia la izquierda." << endl;
         break;
      case GLFW_KEY_RIGHT:
         //desplazamiento/rotación hacia la derecha (moverHV)
         camaras[camActiva]->moverHV(d,0);
         cout << "Desplazamiento/rotación hacia la derecha." << endl;
         break;
      case GLFW_KEY_UP:
         //desplazamiento/rotación hacia arriba (moverHV)
         camaras[camActiva]->moverHV(0,d);
         cout << "Desplazamiento/rotación hacia arriba." << endl;
         break;
      case GLFW_KEY_DOWN:
         // desplazamiento/rotación hacia abajo (moverHV)
         camaras[camActiva]->moverHV(0,d);
         cout << "Desplazamiento/rotación hacia abajo." << endl;
         break;
      default:
         result = false ;
         break ;
	}

   return result ;
}
// ---------------------------------------------------------------------
// se llama al hacer click con el botón izquierdo

void P5_ClickIzquierdo( int x, int y )
{

   //  visualizar escena en modo selección y leer el color del pixel en (x,y)


   // 1. crear un 'contextovis' apropiado
   // .....

   // 2. visualizar en modo selección (sobre el backbuffer)
   // ....

   // 3. leer el color del pixel, si es 0 no se hace nada
   // .....

   // 4. buscar el objeto en el grafo de escena e informar del mismo
   // .....

}
// ---------------------------------------------------------------------
// se llama al mover el botón en modo arrastrar

void P5_InicioModoArrastrar( int x, int y )
{
   // activar bool de modo arrastrar, registrar (x,y) de inicio del modo arrastrar
   modo_arrastrar = true;
   xant = x;
   yant = y;
}
// ---------------------------------------------------------------------
// se llama al subir el botón derecho del ratón

void P5_FinModoArrastrar()
{
   // desactivar bool del modo arrastrar
   modo_arrastrar = false;

}
// ---------------------------------------------------------------------
// se llama durante el modo arrastrar

void P5_RatonArrastradoHasta( int x, int y )
{
   // calcular desplazamiento desde inicio de modo arrastrar, actualizar la camara (moverHV)


}
// ---------------------------------------------------------------------
// pulsar/levantar botón del ratón, específico de la práctica 5

bool P5_FGE_ClickRaton( int boton, int estado, int x, int y )
{
   //cout << "P5_FGE_ClickRaton" << endl ;
   if ( estado == GLFW_PRESS && boton == GLFW_MOUSE_BUTTON_LEFT  )
      P5_ClickIzquierdo( x, viewport.alto-y );
   else if ( estado == GLFW_PRESS && boton == GLFW_MOUSE_BUTTON_RIGHT )
      P5_InicioModoArrastrar( x, viewport.alto-y );
   else if ( estado == GLFW_RELEASE && boton == GLFW_MOUSE_BUTTON_RIGHT )
      P5_FinModoArrastrar();
   else
      return false ;

   return true ;
}
// ---------------------------------------------------------------------
bool P5_FGE_RatonMovidoPulsado( int x, int y )
{
   if ( modo_arrastrar )
   {
      P5_RatonArrastradoHasta( x, viewport.alto-y );
      return true ;
   }
   else
      return false ;
}
// ---------------------------------------------------------------------

bool P5_FGE_Scroll( int direction )
{
   // COMPLETAR: práctica 5: acercar/alejar la camara (desplaZ)
   camaras[camActiva]->desplaZ(direction);

   return true ;
}
// ---------------------------------------------------------------------

void FijarColorIdent( const int ident )  // 0 ≤ ident < 2^24
{
   // fijar color actual de OpenGL usando 'ident' (glColor3ub)
    const unsigned char
    byteR = (ident ) %0x100U, //rojo = byte menos significativo
    byteG = (ident/ 0x100U)%0x100U, //verde = byte intermedio
    byteB = (ident/ 0x10000U)%0x100U; //azul = byte más significativo

    glColor3ub(byteR, byteG, byteB); //cambio de color en opengl

}
//---------------

int LeerIdentEnPixel( int xpix, int ypix )
{
   // leer el identificador codificado en el color del pixel (x,y)
   unsigned char bytes[3]; //para guardar los tres bytes
   //leer los 3 bytes del frame-buffer
   glReadPixels(xpix, ypix, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *)bytes);
   //reconstruir el identificador y devolverlo
   return bytes[0]+(0x100U*bytes[1])+(0x10000U*bytes[2]);

}
//---------------
