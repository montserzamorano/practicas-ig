// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Montserrat Rodríguez Zamorano
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
//#include <tuplasg_impl.hpp>
#include "MallaInd.hpp"   // declaración de 'ContextoVis'

// *****************************************************************************
// funciones auxiliares


// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre("malla indexada, anónima");
}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;
}
// -----------------------------------------------------------------------------
// calcula las dos tablas de normales
Tupla3f MallaInd::normalizar(Tupla3f tupla){
  if(tupla[0]==0 && tupla[1]==0 && tupla[2]==0){
    return Tupla3f(1.0,0.0,0.0);
  }
  return tupla.normalized();
}

Tupla3f MallaInd::hallarNormal(Tupla3f tupla1, Tupla3f tupla2){
  Tupla3f resultado;
  resultado[0] = tupla1[1]*tupla2[2]-tupla1[2]*tupla2[1];
  resultado[1] = tupla1[0]*tupla2[2]-tupla1[2]*tupla2[0];
  resultado[2] = tupla1[0]*tupla2[1]-tupla1[1]*tupla2[0];

  return normalizar(resultado);
}

void MallaInd::calcular_normales(){
  //Para las caras producto cruz de dos de sus lados y normalizando
  //Para los vertices, es la combinacion de las normales de las caras que rodean
  for(unsigned i=0; i<vertices.size();i++){
    normales_vertices.push_back(Tupla3f(0.0,0.0,0.0));
  } //inicializamos tabla de vertices porque tendremos que sumar

  for(unsigned i=0; i<caras.size();i++){
    int v1 = caras.at(i)[0];
    int v2 = caras.at(i)[1];
    int v3 = caras.at(i)[2];
    //sacamos dos aristas
    Tupla3f a1= Tupla3f(vertices.at(v2)[0] - vertices.at(v1)[0],
                        vertices.at(v2)[1] - vertices.at(v1)[1],
                        vertices.at(v2)[2] - vertices.at(v1)[2]);

    Tupla3f a2= Tupla3f(vertices.at(v3)[0] - vertices.at(v1)[0],
                        vertices.at(v3)[1] - vertices.at(v1)[1],
                        vertices.at(v3)[2] - vertices.at(v1)[2]);

    Tupla3f normal = hallarNormal(a1,a2);

    normales_caras.push_back(normal);
    //ahora sumamos al vertice correspondiente
    normales_vertices.at(v1) = normales_vertices.at(v1) + normal;
    normales_vertices.at(v2) = normales_vertices.at(v1) + normal;
    normales_vertices.at(v3) = normales_vertices.at(v1) + normal;
  }

  //normalizamos a tabla de vertices
  for(unsigned i=0; i<normales_vertices.size(); i++){
    Tupla3f t(normales_vertices.at(i));
    normales_vertices.at(i) = normalizar(t);
  }

}

GLuint MallaInd::VBO_Crear(GLuint tipo, GLuint tamanio, GLvoid * puntero){
  assert(tipo == GL_ARRAY_BUFFER || tipo == GL_ELEMENT_ARRAY_BUFFER);
  GLuint id_vbo;
  glGenBuffers(1, & id_vbo);
  glBindBuffer(tipo, id_vbo);
  glBufferData(tipo, tamanio, puntero, GL_STATIC_DRAW);
  glBindBuffer(tipo, 0);
  return id_vbo;
}

void MallaInd::crearVBOs(){
  //crear VBO conteniendo la tabla de vértices
  id_vbo_ver = VBO_Crear(GL_ARRAY_BUFFER, 3*sizeof(float)*vertices.size(), vertices.front());
  id_vbo_tri = VBO_Crear(GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(int)*caras.size(),caras.front());
  if( col_ver.size() > 0){
    id_vbo_col_ver = VBO_Crear( GL_ARRAY_BUFFER, 3*sizeof(float)*vertices.size(), col_ver.front());
  }
  if( normales_vertices.size() > 0){
    id_vbo_norm_ver = VBO_Crear( GL_ARRAY_BUFFER, 3*sizeof(float)*vertices.size(), normales_vertices.front());
  }
}

// -----------------------------------------------------------------------------
//visualizar en modo inmediato usando glDrawElements
void MallaInd::visualizarDE_MI( ContextoVis & cv )
{
  GLenum modo;
	ModosVis modovis = cv.modoVis;

	if(modovis == modoPuntos){
			modo = GL_POINTS;
  }
	else if(modovis == modoAlambre){
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      modo = GL_TRIANGLES;
  }
	else if(modovis == modoSolido){
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      modo = GL_TRIANGLES;
  }

	glLineWidth(2);	// grosor de línea
	glPointSize(4);	// grosor de punto
  //vertices
	glEnableClientState( GL_VERTEX_ARRAY ); // Habilitar array de vértices
	glVertexPointer( 3, GL_FLOAT, 0, vertices.data()); // Establecer dirección y estructura
	// Visualizar recorriendo los vértices en el orden de los índices
	glDrawElements( modo, caras.size()*3, GL_UNSIGNED_INT, caras.data());
	glDisableClientState( GL_VERTEX_ARRAY ); // Deshabilitar array
}
// ----------------------------------------------------------------------------
//visualizar en modo diferido (vertex buffer objects)

void MallaInd::visualizarDE_VBOs( ContextoVis & cv ){
  GLenum modo;
  ModosVis modovis = cv.modoVis;

  if(modovis == modoPuntos){
      modo = GL_POINTS;
  }
  else if(modovis == modoAlambre){
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      modo = GL_TRIANGLES;
  }
  else if(modovis == modoSolido){
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      modo = GL_TRIANGLES;
  }

  if(!modoVBO){
    crearVBOs();
    modoVBO = true;
  }
  glBindBuffer(GL_ARRAY_BUFFER, id_vbo_ver); //activar VBO usando su ident
  glVertexPointer( 3, GL_FLOAT, 0, 0); // Establecer dirección y offset
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glEnableClientState(GL_VERTEX_ARRAY); //usar tabla de vertices

  glLineWidth(2);	// grosor de línea
	glPointSize(4);	// grosor de punto

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_vbo_tri);
  glDrawElements( modo, caras.size()*3, GL_UNSIGNED_INT, nullptr);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisableClientState(GL_VERTEX_ARRAY); //desactivar puntero a vertices
}

void MallaInd::visualizarDE_VBOsAtrVer( ContextoVis & cv ){
  if(col_ver.size() > 0){
    glBindBuffer(GL_ARRAY_BUFFER, id_vbo_col_ver); //act. VBO col.v
    glColorPointer(3, GL_FLOAT, 0, 0); //formato y offset de colores
    glEnableClientState( GL_COLOR_ARRAY ); //activa uso de colores de v.
  }
  if(normales_vertices.size() > 0){
    glBindBuffer(GL_ARRAY_BUFFER, id_vbo_norm_ver ); //act. VBO norm.v
    glColorPointer(3, GL_FLOAT, 0, 0); //formato y offset de normales
    glEnableClientState( GL_NORMAL_ARRAY ); //activa uso de normales de v.
  }
  visualizarDE_VBOs(cv);
  glDisableClientState(GL_COLOR_ARRAY );
}

// -----------------------------------------------------------------------------
void MallaInd::visualizarGL( ContextoVis & cv){
  if(cv.modoVBO){
    visualizarDE_VBOsAtrVer(cv);
  }
  else{
    visualizarDE_MI(cv);
  }
}

Cubo::Cubo() : MallaInd("malla cubo"){
  float a = 1.0;
  vector <Tupla3f> vertices = {{-a,-a,-a},{a,-a,-a},{a,a,-a},{-a,a,-a},{-a,a,a},{-a,-a,a},{a,-a,a},{a,a,a}};
	vector <Tupla3i> caras =  {{0,1,3},{1,2,3},{0,3,4},{0,4,5},{5,6,7},{4,5,7},{1,6,7},
	 													{1,2,7},{5,6,1},{0,5,1},{3,4,7},{2,3,7}};
  setVertices(vertices);
  setCaras(caras);
}
// *****************************************************************************

Tetraedro::Tetraedro():MallaInd( "malla tetraedro"){
  float a = 1.0;
  vector <Tupla3f> vertices = {{-a,0.0,-a*sqrt(3.0)/2.0}, {a,0.0,-a*sqrt(3.0)/2.0},
                              {0.0,0.0,a*sqrt(3.0)/2.0}, {0.0,a*sqrt(3.0),0.0}};
  vector <Tupla3i> caras = {{2,0,1}, {3,2,1}, {0,1,3}, {3,2,0}};
  setVertices(vertices);
  setCaras(caras);
}
// *****************************************************************************

//AÑADIDOS
//setter vertices
void MallaInd::setVertices(vector <Tupla3f> v){
  for(unsigned i=0; i<v.size();i++){
    vertices.push_back(v.at(i));
  }
}
//setter caras
void MallaInd::setCaras(vector <Tupla3i> c){
  for(unsigned i=0; i<c.size();i++){
    caras.push_back(c.at(i));
  }
}

void MallaInd::fijarColorNodo( const Tupla3f & nuevo_color ){
  for(unsigned i=0; i<=vertices.size();i++){
    col_ver.push_back(nuevo_color);
  }
}
