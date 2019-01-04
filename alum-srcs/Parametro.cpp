#include <cassert>
#include <cmath>
#include "Parametro.hpp"
#include "Objeto3D.hpp"
#include "matrices-tr.hpp"

using namespace std;

// -----------------------------------------------------------------------------
// ** Informática Gráfica, curso 2018-19
// ** Montserrat Rodríguez Zamorano
// crear grado de libertad no acotado

Parametro::Parametro(std::string p_descripcion, Matriz4f * p_ptr_mat,
                    TFuncionCMF p_fcm, bool p_acotado, float p_c,
                    float p_s, float p_f){
  ////
  aceleracion = 0.1;
  incremento = 1.0;
  velocidad_inicial = 0.1;
  ////
  descripcion = p_descripcion;
  ptr_mat = p_ptr_mat;
  fun_calculo_matriz = p_fcm;
  acotado = p_acotado;
  c = p_c;
  s = p_s;
  f = p_f;
  valor_norm = c; //se inicializa al valor inicial
  velocidad = velocidad_inicial;
}

// -----------------------------------------------------------------------------
/*actualizar valor y matriz al siguiente frame*/
void Parametro::siguiente_cuadro(){
  valor_norm += velocidad;
  *ptr_mat = fun_calculo_matriz(leer_valor_actual());
}
// -----------------------------------------------------------------------------
/*vuelve al estado inicial de valor, aceleracion y velocidad */
void Parametro::reset(){
  valor_norm = c;
  velocidad = velocidad_inicial;
  *ptr_mat = fun_calculo_matriz(leer_valor_actual());
}
// -----------------------------------------------------------------------------
/*incrementar el valor*/
void Parametro::incrementar(){
  valor_norm += incremento;
  *ptr_mat = fun_calculo_matriz(leer_valor_actual());
}
// -----------------------------------------------------------------------------
/*decrementar el valor*/
void Parametro::decrementar(){
  valor_norm -= incremento;
  //if(valor_norm < 0){
  //  valor_norm = -valor_norm;
  //}
  *ptr_mat = fun_calculo_matriz(leer_valor_actual());
}
// -----------------------------------------------------------------------------
/*acelerar (aumentar velocidad)*/
void Parametro::acelerar(){
  velocidad += aceleracion;
}
// -----------------------------------------------------------------------------
/*decelerar (disminuir la velocidad)*/
void Parametro::decelerar(){
  velocidad -= aceleracion;
  if(velocidad < 0.0){
    velocidad = 0.0;
  }
}
// -----------------------------------------------------------------------------
/*devuelve el valor actual (valor de q)*/
float Parametro::leer_valor_actual(){
  float valor_actual;
  if(acotado){
    valor_actual = c + s*valor_norm; //crece indefinidamente
  }
  else{
    valor_actual = c + s*sin(f*2*M_PI*valor_norm); //toma valores entre c+s y c-s
  }
  return valor_actual;
}
// -----------------------------------------------------------------------------
//devuelve velocidad actual
float Parametro::leer_velocidad_actual(){
  return velocidad;
}
// -----------------------------------------------------------------------------
/*devuelve decripcion*/
string Parametro::leer_descripcion(){
  return descripcion;
}

Matriz4f * Parametro::leer_ptr(){
  return ptr_mat;
}
// -----------------------------------------------------------------------------
