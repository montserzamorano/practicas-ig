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
      if ( actual != nullptr ){
         actual->activar();
      }
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

void Textura::activar(){
  //enviar la textura la primera vez
  glEnable(GL_TEXTURE_2D);
  if(!enviada){
    enviar();
    enviada = true ;
  }
  else{
    glBindTexture(GL_TEXTURE_2D, ident_textura);
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

TexturaXY::TexturaXY( const std::string & nom ):Textura(nom){
  modo_gen_ct = mgct_coords_ojo; //valdria tb coords_objeto?
  //s=x
  coefs_s[0]=1.0;
  coefs_s[1]=0.0;
  coefs_s[2]=0.0;
  coefs_s[3]=0.0;
  //t=y
  coefs_t[0]=0.0;
  coefs_t[1]=1.0;
  coefs_t[2]=0.0;
  coefs_t[3]=0.0;
}

// *********************************************************************

Material::Material()
{
   iluminacion = false ;
   tex = nullptr ;
   coloresCero() ;
   ponerNombre("material sin textura ni iluminacion") ;
}
// ---------------------------------------------------------------------

Material::Material( const std::string & nombreArchivoJPG ){
   iluminacion    = false ;
   tex            = new Textura( nombreArchivoJPG ) ;
   coloresCero();

   del.ambiente  = VectorRGB( 0.0, 0.0, 0.0, 1.0);
   del.difusa    = VectorRGB( 0.5, 0.5, 0.5, 1.0 );
   del.especular = VectorRGB( 1.0, 1.0, 1.0, 1.0 );

   del.ambiente  = VectorRGB( 0.0, 0.0, 0.0, 1.0);
   tra.difusa    = VectorRGB( 0.2, 0.2, 0.2, 1.0 );
   tra.especular = VectorRGB( 0.2, 0.2, 0.2, 1.0 );

   ponerNombre("material con textura de imagen sin iluminacion") ;
}

// ---------------------------------------------------------------------
// crea un material usando textura y coeficientes: ka,kd,ks y exponente
// (la textura puede ser NULL, la ilum. queda activada)

Material::Material( Textura * text, float ka, float kd, float ks, float exp ){
   iluminacion = true;
   tex = text;

   color = VectorRGB(0.0,0.0,0.0,1.0);

   del.emision = VectorRGB(0.0,0.0,0.0,1.0);
   del.ambiente = {ka,ka,ka,1.0};
   del.difusa = {kd,kd,kd,1.0};
   del.especular = {ks,ks,ks,1.0};
   del.exp_brillo = exp ;

   tra.emision = VectorRGB(0.0,0.0,0.0,1.0);
   tra.ambiente = {ka,ka,ka,1.0};
   tra.difusa = {kd,kd,kd,1.0};
   tra.especular = {ks,ks,ks,1.0};
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

   color = {colorAmbDif[0], colorAmbDif[1], colorAmbDif[2], 1.0};

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
  tex = NULL;
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
   if ( tex != nullptr ){
     delete tex ;
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

void Material::activar()
{
  if(iluminacion){
    glEnable(GL_LIGHTING);

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

    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
  }
  else{
    glDisable(GL_LIGHTING);
    glColor4f(color[0],color[1],color[2],color[3]);
  }
  //habilitar textura
  if(tex == nullptr){
    glDisable(GL_TEXTURE_2D);
  }
  else{
    tex -> activar();
  }
}

//MATERIAL ESTANDAR

//MaterialEstandar::MaterialEstandar(){}

MaterialLata::MaterialLata() : Material(new Textura("../imgs/lata-coke.jpg"),0.1,0.7,1.0,20.0){
  ponerNombre("material lata");
}

MaterialTapasLata::MaterialTapasLata():Material(Tupla3f(0.85, 0.85, 0.85),0.1,0.7,1.0,10.0){
  ponerNombre("material tapas lata");
}

MaterialPeonMadera::MaterialPeonMadera():Material(new TexturaXY("../imgs/text-madera.jpg"),0.5,0.4,0.5,10.0){
  ponerNombre("material peon madera");
}

MaterialPeonBlanco::MaterialPeonBlanco():Material(Tupla3f(0.9,0.9,0.9),0.1,0.7,1.0,1.0){
  ponerNombre("material peon blanco");
}

MaterialPeonNegro::MaterialPeonNegro():Material(Tupla3f(0.1,0.1,0.1),0.1,0.7,1.0,1.0){
  ponerNombre("material peon negro");
}

MaterialFlexo::MaterialFlexo():Material(Tupla3f(0.2, 0.2, 0.7),0.1,0.7,1.0,10.0){
  ponerNombre("material flexo");
}

MaterialPelota::MaterialPelota():Material(new TexturaXY("../imgs/text-marmol.jpg"),0.7,0.4,1.0,10.0){
  ponerNombre("material pelota");
}

MaterialBombilla::MaterialBombilla():Material(Tupla3f(0.9,0.9,0.7),0.7,0.7,1.0,80.0){
  ponerNombre("material bombilla");
}
//**********************************************************************

FuenteLuz::FuenteLuz( GLfloat p_longi_ini, GLfloat p_lati_ini, const VectorRGB & p_color )
{
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
}

//----------------------------------------------------------------------

void FuenteLuz::activar(){
  //por si no es activable
  assert(ind_fuente != -1);

  GLenum fuente = GL_LIGHT0+ind_fuente;
  glEnable(fuente); //activamos la iesima fuente de luz

  glLightfv(fuente, GL_AMBIENT, col_ambiente);
  glLightfv(fuente, GL_DIFFUSE, col_difuso);
  glLightfv(fuente, GL_SPECULAR, col_especular);

  if(posicion[3] == 1.0){ //si se trata de una fuente posicional
    glLightfv(fuente, GL_POSITION, posicion);
  }
  else if(posicion[3]==0.0){//si se trata de una fuente direccional
    //establecemos la direccion usando coordenadas polares
    const float dir[4] = {0.0,0.0,1.0,0.0};
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
      glLoadIdentity();
      //rotacion en torno al eje Y
      glRotatef(longi, 0.0, 1.0, 0.0);
      //rotacion en torno al eje X
      glRotatef(lati, -1.0, 0.0, 0.0);
      //hacer l_i = (0,0,1) paralela eje Z
      glLightfv(fuente, GL_POSITION, dir);
    glPopMatrix();
  }
}

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
   return actualizar ;
}

FuenteDireccional::FuenteDireccional(float alpha_inicial, float beta_inicial, const VectorRGB & p_color)
:FuenteLuz(alpha_inicial, beta_inicial, p_color){
  //por ser fuente direccional
  posicion = {0.0, 0.0, 0.0, 0.0};
}

void FuenteDireccional::variarAngulo(unsigned angulo, float incremento){
  if(angulo == 0){
    longi += incremento;
    cout << "Nuevo longi: " << longi << endl;
  }
  else{
    lati += incremento;
    cout << "Nueva lati: " << lati << endl;
  }
}

FuentePosicional::FuentePosicional( const Tupla3f & posicion, const VectorRGB & p_color)
:FuenteLuz(0.0,0.0,p_color){
  this->posicion = {posicion[0], posicion[1], posicion[2], 1.0};
}

//**********************************************************************

ColFuentesLuz::ColFuentesLuz()
{
   max_num_fuentes = 5 ;
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
   glEnable(GL_LIGHTING);
   glEnable(GL_NORMALIZE);

   if(id_prog < vpf.size())
     vpf[id_prog]->activar();
}

void ColFuentesLuz::activarTodas(){
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);

  float m = std::min((int) vpf.size(),max_num_fuentes);

  for(unsigned i=0; i<m; i++){
    vpf[i]->activar();
  }

 for(unsigned i=max_num_fuentes; i<vpf.size(); i++){
    glDisable(GL_LIGHT0+i);
  }

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

ColeccionFuentesP4::ColeccionFuentesP4(){
  const VectorRGB fuenteBlanca = {1.0,1.0,1.0,1.0};
  const VectorRGB fuenteVerde = {0.0,1.0,0.0,1.0};
  //insertar(new FuenteDireccional(-30.0,30.0,fuenteBlanca));
  //insertar(new FuentePosicional(Tupla3f{10.0,9.0,5.0},fuenteBlanca));
  insertar(new FuenteDireccional(20.0,20.0,fuenteBlanca));
}
