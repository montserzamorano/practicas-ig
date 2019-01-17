// *********************************************************************
// **
// ** Gestión de materiales y texturas (implementación)
// **
// ** Copyright (C) 2014 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************
// ** Informática Gráfica, curso 2018-19
// ** Montserrat Rodríguez Zamorano


#include "matrices-tr.hpp"
#include "materiales.hpp"

using namespace std ;

const bool trazam = false ;

//*********************************************************************

PilaMateriales::PilaMateriales()
{
   actual = nullptr ;
}
// -----------------------------------------------------------------------------

void PilaMateriales::activarMaterial( Material * material )
{
   if ( material != actual )
   {
      actual = material ;
      if ( actual != nullptr )
         actual->activar();
   }
}
// -----------------------------------------------------------------------------

void PilaMateriales::activarActual()
{
   if ( actual != nullptr )
      actual->activar() ;
}
// -----------------------------------------------------------------------------

void PilaMateriales::push(  )
{
   pila.push_back( actual );
}
// -----------------------------------------------------------------------------

void PilaMateriales::pop(  )
{
   if ( pila.size() == 0 )
   {
      cout << "error: intento de hacer 'pop' de un material en una pila de materiales vacía." << endl << flush ;
      exit(1);
   }

   Material * anterior = pila[pila.size()-1] ;
   pila.pop_back();
   activarMaterial( anterior );  // cambia 'actual'
}

//**********************************************************************

Textura::Textura( const std::string & nombreArchivoJPG )
{
  ident_textura = 0;
  imagen = new jpg::Imagen(nombreArchivoJPG);
  //generacion de coordenadas de textura desactivada
  modo_gen_ct = mgct_desactivada ;
  enviada = false ;
}

// ---------------------------------------------------------------------

//----------------------------------------------------------------------

void Textura::enviar()
{
  unsigned long x = imagen-> tamX();
  unsigned long y = imagen-> tamY();
  unsigned char * texels = imagen-> leerPixels();
  //especificar los coficientes de las funciones lineales
  //transp. 164 tema 3 (?)
  glGenTextures(1, &ident_textura);
  glBindTexture(GL_TEXTURE_2D, ident_textura);
  //especificar imagen
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, x, y, GL_RGB, GL_UNSIGNED_BYTE, texels);
}

//----------------------------------------------------------------------

Textura::~Textura( )
{
   using namespace std ;
   cout << "Destruyendo textura...imagen ==" << imagen << endl ;
   if ( imagen != NULL )
      delete imagen ;

   imagen = NULL ;
   cout << "Hecho (no hecho!)" << endl << flush ;
}

//----------------------------------------------------------------------
// por ahora, se asume la unidad de texturas #0

void Textura::activar(  )
{
  //enviar la textura la primera vez
  if(!enviada){
    enviar();
    enviada = true ;
  }
  //si ya se ha enviado solo se activa
  //generacion procedural de las coords de textura
  if(modo_gen_ct == mgct_coords_ojo){ //coordenadas de ojo
    glEnable( GL_TEXTURE_GEN_S );
    glEnable( GL_TEXTURE_GEN_T );
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR) ;
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR) ;
    glTexGenfv(GL_S, GL_EYE_PLANE, coefs_s) ;
    glTexGenfv(GL_T, GL_EYE_PLANE, coefs_t) ;
  }
  else if(modo_gen_ct == mgct_coords_objeto){ //coordenadas de objeto
    glEnable( GL_TEXTURE_GEN_S );
    glEnable( GL_TEXTURE_GEN_T );
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR) ;
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR) ;
    glTexGenfv(GL_S, GL_OBJECT_PLANE, coefs_s) ;
    glTexGenfv(GL_T, GL_OBJECT_PLANE, coefs_t) ;
  }
  else if(modo_gen_ct == mgct_desactivada){ //desactivadas
    glDisable( GL_TEXTURE_GEN_S );
    glDisable( GL_TEXTURE_GEN_T );
  }

}
// *********************************************************************

Material::Material()
{
   iluminacion = false ;
   tex = NULL ;
   coloresCero() ;
}
// ---------------------------------------------------------------------

Material::Material( const std::string & nombreArchivoJPG )
{
   iluminacion    = false ;
   tex            = new Textura( nombreArchivoJPG ) ;
   coloresCero();

   del.emision   = VectorRGB(0.0,0.0,0.0,1.0);
   del.ambiente  = VectorRGB( 0.0, 0.0, 0.0, 1.0);
   del.difusa    = VectorRGB( 0.5, 0.5, 0.5, 1.0 );
   del.especular = VectorRGB( 1.0, 1.0, 1.0, 1.0 );

   del.emision   = VectorRGB(0.0,0.0,0.0,1.0);
   del.ambiente  = VectorRGB( 0.0, 0.0, 0.0, 1.0);
   tra.difusa    = VectorRGB( 0.2, 0.2, 0.2, 1.0 );
   tra.especular = VectorRGB( 0.2, 0.2, 0.2, 1.0 );

   ponerNombre("material con textura de imagen sin iluminacion") ;
}

// ---------------------------------------------------------------------
// crea un material usando textura y coeficientes: ka,kd,ks y exponente
// (la textura puede ser NULL, la ilum. queda activada)

Material::Material( Textura * text, float ka, float kd, float ks, float exp )
:  Material()
{
   iluminacion = true;
   tex = text;
   coloresCero();

   del.emision = VectorRGB(0.0,0.0,0.0,1.0);
   del.ambiente = ka*color;
   del.difusa = kd*color;
   del.especular = ks*color;
   del.exp_brillo = exp ;

   tra.emision = VectorRGB(0.0,0.0,0.0,1.0);
   tra.ambiente = ka*color;
   tra.difusa = kd*color;
   tra.especular = ks*color;
   tra.exp_brillo = exp ;

   ponerNombre("material con textura e iluminación") ;
 }

// ---------------------------------------------------------------------
// crea un material con un color único para las componentes ambiental y difusa
// en el lugar de textura (textura == NULL)
Material::Material( const Tupla3f & colorAmbDif, float ka, float kd, float ks, float exp )
{
   iluminacion = true;
   tex = nullptr;

   color.emision = colorAmbDif[0];
   color.ambiente = colorAmbDif[1];
   color.difusa = colorAmbDif[2];
   color.especular = 1.0;

   del.emision = VectorRGB(0.0,0.0,0.0,1.0);
   del.ambiente = ka*color;
   del.difusa = kd*color;
   del.especular = ks*color;
   del.exp_brillo = exp ;

   tra.emision = VectorRGB(0.0,0.0,0.0,1.0);
   tra.ambiente = ka*color;
   tra.difusa = kd*color;
   tra.especular = ks*color;
   tra.exp_brillo = exp ;

   ponerNombre("material color plano, ilum.") ;
}
// ---------------------------------------------------------------------

Material::Material( const float r, const float g, const float b )
{
  iluminacion = false;
  tex = nullptr;
  coloresCero();
  color = VectorRGB(r,g,b,1.0);
  ponerNombre("material color plano, sin ilum.") ;
}

//----------------------------------------------------------------------

void Material::coloresCero()
{
   const VectorRGB ceroRGBopaco(0.0,0.0,0.0,1.0);

   color         =

   del.emision   =
   del.ambiente  =
   del.difusa    =
   del.especular =

   tra.emision   =
   tra.ambiente  =
   tra.difusa    =
   tra.especular = ceroRGBopaco ;

   del.exp_brillo =
   tra.exp_brillo = 1.0 ;
}
//----------------------------------------------------------------------

Material::~Material()
{
   if ( tex != nullptr )
   {  delete tex ;
      tex = nullptr ;
   }
}
//----------------------------------------------------------------------

void Material::ponerNombre( const std::string & nuevo_nombre )
{
   nombre_mat = nuevo_nombre ;
}
//----------------------------------------------------------------------

std::string Material::nombre() const
{
   return nombre_mat ;
}
//----------------------------------------------------------------------

void Material::activar(  )
{
  if(iluminacion){
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
  }
  glEnable(GL_COLOR_MATERIAL);

  //configurar el material
  //discriminamos el juego de ternas sobre el que estamos actuando

  glMaterialfv(GL_FRONT, GL_EMISSION, del.emision);
  glMaterialfv(GL_FRONT, GL_AMBIENT, del.ambiente);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, del.difusa);
  glMaterialfv(GL_FRONT, GL_SPECULAR, del.especular);
  glMaterialf(GL_FRONT, GL_SHININESS, del.exp_brillo);

  glMaterialfv(GL_BACK, GL_EMISSION, tra.emision);
  glMaterialfv(GL_BACK, GL_AMBIENT, tra.ambiente);
  glMaterialfv(GL_BACK, GL_DIFFUSE, tra.difusa);
  glMaterialfv(GL_BACK, GL_SPECULAR, tra.especular);
  glMaterialf(GL_BACK, GL_SHININESS, tra.exp_brillo);

  //se liga la componente ambiental y/o difusa del material con el color
  glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
  glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
  glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  glDisable(GL_COLOR_MATERIAL);

  //habilitar textura
  if(tex == nullptr){
    //glDisable(GL_TEXTURE_2D);
    //glDisable(GL_NORMALIZE);
  }
  else{
    tex -> activar();
  }
}

//MATERIAL CONCRETO

MaterialLata::MaterialLata():Material("../imgs/text-lata-1.jpg"){
  ponerNombre("material lata");
}

MaterialTapasLata::MaterialTapasLata():Material(Tupla3f(0.65, 0.65, 0.65), 0.65, 0.05, 0.2, 0.7){
  ponerNombre("material tapas lata");
}

MaterialPeonMadera::MaterialPeonMadera():Material("../imgs/text-madera.jpg"){
  ponerNombre("material peon madera");
}

MaterialPeonBlanco::MaterialPeonBlanco():Material(1.0, 1.0, 1.0){
  ponerNombre("material peon blanco");
}

MaterialPeonNegro::MaterialPeonNegro(0.0, 0.0, 0.0){
  ponerNombre("material peon negro");
}

//**********************************************************************

FuenteLuz::FuenteLuz( GLfloat p_longi_ini, GLfloat p_lati_ini, const VectorRGB & p_color )
{
   //CError();

   if ( trazam )
      cout << "creando fuente de luz." <<  endl << flush ;

   // inicializar parámetros de la fuente de luz
   longi_ini = p_longi_ini ;
   lati_ini  = p_lati_ini  ;
   longi = longi_ini ;
   lati  = lati_ini ;

   col_ambiente  = p_color ;
   col_difuso    = p_color ;
   col_especular = p_color ;

   ind_fuente = -1 ; // la marca como no activable hasta que no se le asigne indice

   //CError();
}

//----------------------------------------------------------------------

void FuenteLuz::activar(){}

//----------------------------------------------------------------------

bool FuenteLuz::gestionarEventoTeclaEspecial( int key )
{
   bool actualizar = true ;
   const float incr = 3.0f ;

   switch( key )
   {
      case GLFW_KEY_RIGHT:
         longi = longi+incr ;
         break ;
      case GLFW_KEY_LEFT:
         longi = longi-incr ;
         break ;
      case GLFW_KEY_UP:
         lati = std::min( lati+incr, 90.0f) ;
         break ;
      case GLFW_KEY_DOWN:
         lati = std::max( lati-incr, -90.0f ) ;
         break ;
      case GLFW_KEY_HOME:
         lati  = lati_ini ;
         longi = longi_ini ;
         break ;
      default :
         actualizar = false ;
         cout << "tecla no usable para la fuente de luz." << endl << flush ;
   }

   //if ( actualizar )
   //   cout << "fuente de luz cambiada: longi == " << longi << ", lati == " << lati << endl << flush ;
   return actualizar ;
}

FuenteDireccional::FuenteDireccional(float alpha_inicial, float beta_inicial){
  longi_ini = alpha_inicial;
  lati_ini = beta_inicial;
  longi = longi_ini;
  lati = lati_ini;
  //inicializar colores luz
  color[0]={0.5,0.5,0.5,1.0};
  color[1]={0.5,0.5,0.5,1.0};
  color[2]={0.5,0.5,0.5,1.0};
  //por ser fuente direccional
  posicion = {0.0, 0.0, 0.0, 0.0};
}

FuenteDireccional::variarAngulo(unsigned angulo, float incremento){
  if(angulo == 0){
    longi += incremento;
  }
  else{
    lati += incremento;
  }
}

FuentePosicional::FuentePosicional( const Tupla3f & posicion ){
  this->posicion = {posicion[0], posicion[1], posicion[2], 1.0};
  //inicializar colores luz
  color[0]={0.5,0.5,0.5,1.0};
  color[1]={0.5,0.5,0.5,1.0};
  color[2]={0.5,0.5,0.5,1.0};
}

//**********************************************************************

ColFuentesLuz::ColFuentesLuz()
{
   max_num_fuentes = -1 ;
}
//----------------------------------------------------------------------

void ColFuentesLuz::insertar( FuenteLuz * pf )  // inserta una nueva
{
   assert( pf != nullptr );

   pf->ind_fuente = vpf.size() ;
   vpf.push_back( pf ) ;
}
//----------------------------------------------------------------------
// activa una colección de fuentes de luz

void ColFuentesLuz::activar( unsigned id_prog )
{
   // COMPLETAR: práctica 4: activar una colección de fuentes de luz
   // .....

}
//----------------------------------------------------------------------
FuenteLuz * ColFuentesLuz::ptrFuente( unsigned i )
{
   assert(i < vpf.size()) ;
   return vpf[i] ;
}
//----------------------------------------------------------------------
ColFuentesLuz::~ColFuentesLuz()
{
   for( unsigned i = 0 ; i < vpf.size() ; i++ )
   {
      assert( vpf[i] != NULL );
      delete vpf[i] ;
      vpf[i] = NULL ;
   }
}
