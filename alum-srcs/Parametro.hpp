#include <string>
#include <functional>
#include <matrizg.hpp>

#ifndef GRADO_LIBERTAD_HPP
#define GRADO_LIBERTAD_HPP

class Objeto3D ;

// ** Informática Gráfica, curso 2018-19
// ** Montserrat Rodríguez Zamorano

using namespace std;

// tipo usado para las funciones que calculan una matriz a partir de un flotante
typedef function< Matriz4f( float )> TFuncionCMF  ;

// -----------------------------------------------------------------------------

class Parametro
{
  private:
     string descripcion; //descripcion del grado de libertad
     Matriz4f * ptr_mat; //puntero a la matriz dentro del modelo
     TFuncionCMF fun_calculo_matriz; //nueva matriz a partir de un valor flotante
     bool acotado; //true si el valor oscila entre dos valores
     float c; //valor inicial
     float s; //semiamplitud (si acotado) o factor de escala (si no acotado)
     float f; //si acotado: frecuencia (ciclos por unidad del valor normalizado)
     float valor_norm; //valor actual, normalizado, no acotado
     float velocidad; //velocidad actual (se suma al valor_norm)
     float aceleracion;
     float incremento ;
     float velocidad_inicial;

   public:

  Parametro(string p_descripcion, Matriz4f * p_ptr_mat,
            TFuncionCMF p_fcm, bool p_acotado, float p_c,
            float p_s, float p_f);

   void  siguiente_cuadro();   // actualizar valor y matriz al siguiente frame
   void  reset();        // vuelve al estado inicial
   void  incrementar();  // incrementar el valor
   void  decrementar() ; // decrementar el valor
   void  acelerar();     // acelerar (aumentar velocidad normalizada)
   void  decelerar();    // decelerar (disminuir la velocidad normalizada)
   float leer_valor_actual(); // devuelve el valor actual (escalado, no normalizado)
   float leer_velocidad_actual();    // devuelve velocidad actual
   string leer_descripcion();
   Matriz4f * leer_ptr();
};

#endif
