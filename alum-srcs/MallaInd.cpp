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
#include "MallaInd.hpp"

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
   modoVBO = false;
   id_vbo_ver = 0;
   id_vbo_tri = 0;
   id_vbo_col_ver = 0;
   id_vbo_norm_ver = 0;
   id_vbo_cctt = 0;
   centro_oc = {0.0, 0.0, 0.0};
}
// -----------------------------------------------------------------------------
// calcula las dos tablas de normales
Tupla3f MallaInd::normalizar(Tupla3f tupla){
  if(tupla[0]==0 && tupla[1]==0 && tupla[2]==0){
    return Tupla3f(1.0,0.0,0.0);
  }
  return tupla.normalized();
}

void MallaInd::calcular_normales(){
  normales_vertices.clear();
  normales_caras.clear();
  //cout << "inicializando tabla normales..." << endl;
  //Para las caras producto cruz de dos de sus lados y normalizando
  //Para los vertices, es la combinacion de las normales de las caras que rodean
  for(unsigned i=0; i<vertices.size();i++){
    normales_vertices.push_back(Tupla3f(0.0,0.0,0.0));
  } //inicializamos tabla de vertices porque tendremos que sumar
  for(unsigned i=0; i<caras.size();i++){
    normales_caras.push_back(Tupla3f(0.0,0.0,0.0));
  }

  for(unsigned i=0; i<caras.size();i++){
    Tupla3f v1 = vertices[caras.at(i)[0]];
    Tupla3f v2 = vertices[caras.at(i)[1]];
    Tupla3f v3 = vertices[caras.at(i)[2]];
    //sacamos dos aristas
    Tupla3f a1 = v2-v1;
    Tupla3f a2 = v3-v1;
    Tupla3f v = a1.cross(a2);

    cout << "Producto cruzado: " << v[0] << "," << v[1] << "," << v[2]  << endl;

    if(v.lengthSq() == 0.0){
      normales_caras.at(i) = v;
    }
    else{
      normales_caras.at(i) = v.normalized();
    }

  //añadimos ahora a tabla de vertices
    normales_vertices[caras.at(i)[0]] = normales_vertices[caras.at(i)[0]]+normales_caras.at(i);
    normales_vertices[caras.at(i)[1]] = normales_vertices[caras.at(i)[1]]+normales_caras.at(i);
    normales_vertices[caras.at(i)[2]] = normales_vertices[caras.at(i)[2]]+normales_caras.at(i);
  }

  for(unsigned i=0; i<vertices.size(); i++){
    normales_vertices.at(i) = normales_caras.at(i).normalized();
  }

}

GLuint MallaInd::VBO_Crear(GLuint tipo, GLuint tamanio, GLvoid * puntero){
  assert(tipo == GL_ARRAY_BUFFER || tipo == GL_ELEMENT_ARRAY_BUFFER);
  GLuint id_vbo;
  glGenBuffers(1, & id_vbo);
  glBindBuffer(tipo, id_vbo);
  glBufferData(tipo, tamanio, puntero, GL_STATIC_DRAW);
  glBindBuffer(tipo, 0);
  return id_vbo;
}

void MallaInd::crearVBOs(){
  //crear VBO conteniendo la tabla de vértices
  id_vbo_ver = VBO_Crear(GL_ARRAY_BUFFER, 3*sizeof(float)*vertices.size(), vertices.data());
  //crear VBO conteniendo la tabla de caras
  id_vbo_tri = VBO_Crear(GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(int)*caras.size(),caras.data());
  //si hay tabla de colores, crear VBO conteniendo la tabla de colores
  if( col_ver.size() > 0){
    id_vbo_col_ver = VBO_Crear( GL_ARRAY_BUFFER, 3*sizeof(float)*vertices.size(), col_ver.data());
  }
  //si hay tabla de normales, crear VBO
  if( normales_vertices.size() > 0){
    id_vbo_norm_ver = VBO_Crear( GL_ARRAY_BUFFER, 3*sizeof(float)*vertices.size(), normales_vertices.data());
  }
  //si hay tabla de texturas, crear VBO
  if( cctt.size() > 0){
    id_vbo_cctt = VBO_Crear( GL_ARRAY_BUFFER, 2*sizeof(float), cctt.data());
  }
}

// -----------------------------------------------------------------------------

void MallaInd::setLineasPuntos(float grosorL, float grosorP){
  glLineWidth(grosorL);	// grosor de línea
  glPointSize(grosorP);	// grosor de punto
}

void MallaInd::setPolygonMode(ContextoVis & cv){
  ModosVis modovis = cv.modoVis;
  if(modovis == modoPuntos){
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
  }
  else if(modovis == modoAlambre){
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  else if(modovis == modoSolido){
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
  else if(modovis == modoIluminacionPlano){
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_FLAT);
  }
  else if(modovis == modoGoroud){
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);
  }
}

//VISUALIZAR MODO INMEDIADO
void MallaInd::visualizarDE(){
  //vertices
  glEnableClientState( GL_VERTEX_ARRAY ); // Habilitar array de vértices
  glVertexPointer( 3, GL_FLOAT, 0, vertices.data()); // Establecer dirección y estructura
  // Visualizar recorriendo los vértices en el orden de los índices
  glDrawElements( GL_TRIANGLES, caras.size()*3, GL_UNSIGNED_INT, caras.data());
  glDisableClientState( GL_VERTEX_ARRAY ); // Deshabilitar array
}
//drawElements
void MallaInd::visualizarDE_MI( ContextoVis & cv )
{
  setPolygonMode(cv);
  setLineasPuntos(2,4);

  if(col_ver.size() > 0){
    glEnableClientState( GL_COLOR_ARRAY );
    glColorPointer( 3, GL_FLOAT, 0, col_ver.data() );
  }

  if(normales_vertices.size() > 0){
    glEnableClientState( GL_NORMAL_ARRAY );
    glNormalPointer( GL_FLOAT, 0, normales_vertices.data() );
  }

  if(cctt.size()>0){
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, cctt.data());
  }

  visualizarDE();

  glDisableClientState( GL_NORMAL_ARRAY );
  glDisableClientState( GL_COLOR_ARRAY );
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

// ----------------------------------------------------------------------------
//VISUALIZAR MODO DIFERIDO (vertex buffer objects)
void MallaInd::visualizarVBOs(){
  glBindBuffer(GL_ARRAY_BUFFER, id_vbo_ver); //activar VBO usando su ident
  glVertexPointer( 3, GL_FLOAT, 0, 0); // Establecer dirección y offset
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glEnableClientState(GL_VERTEX_ARRAY); //usar tabla de vertices

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_vbo_tri);
  glDrawElements( GL_TRIANGLES, 3*caras.size(), GL_UNSIGNED_INT, nullptr);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisableClientState(GL_VERTEX_ARRAY); //desactivar puntero a vertices
}

void MallaInd::visualizarDE_VBOs( ContextoVis & cv ){
  setPolygonMode(cv);
  setLineasPuntos(2,4);

  if(!modoVBO){
    crearVBOs();
    modoVBO = true;
  }

  if(col_ver.size() > 0){
    glBindBuffer(GL_ARRAY_BUFFER, id_vbo_col_ver); //act. VBO col.v
    glColorPointer(3, GL_FLOAT, 0, 0); //formato y offset de colores
    glEnableClientState( GL_COLOR_ARRAY ); //activa uso de colores de v.
  }

  if(normales_vertices.size() > 0){
    glBindBuffer(GL_ARRAY_BUFFER, id_vbo_norm_ver);
    glNormalPointer(GL_FLOAT, 0, 0);
    glEnableClientState(GL_NORMAL_ARRAY);
  }

  if(cctt.size()>0){
    glBindBuffer(GL_ARRAY_BUFFER, id_vbo_cctt);
    glTexCoordPointer(2, GL_FLOAT, 0,0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  }

  visualizarVBOs();

  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY );
  glDisableClientState(GL_NORMAL_ARRAY);
}

void MallaInd::visualizarDE_Plano(ContextoVis & cv){
  cout << "Objeto: "<< leerNombre() << endl;
  cout << "Tam" << cctt.size() << endl;
  cout << "Vers" << vertices.size() << endl;
  glBegin(GL_TRIANGLES);
  for(unsigned i=0; i<caras.size(); i++){
    glNormal3fv(normales_caras.at(i));
    for(unsigned j=0; j<3; j++){
      unsigned iv = caras.at(i)[j];
      if(col_ver.size()>0){
        glColor3fv(col_ver.at(iv)); //color actual i-esimo triangulo
      }
      if(cctt.size()>0){
        glTexCoord2fv(cctt.at(iv)); //textura actual
      }
      //enviar coordenadas del vertice j del triangulo i
      glVertex3fv(vertices.at(iv));
    }
  }

  glEnd();
}

// -----------------------------------------------------------------------------
void MallaInd::visualizarGL( ContextoVis & cv){
  if(cv.modoVis==modoPuntos||cv.modoVis==modoAlambre||cv.modoVis==modoSolido){
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    if(cv.modoVBO){
      visualizarDE_VBOs(cv);
    }
    else{
      visualizarDE_MI(cv);
    }
  }
  else{
    visualizarDE_Plano(cv);
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
  col_ver.clear();
  for(unsigned i=0; i<=vertices.size();i++){
    col_ver.push_back(nuevo_color);
  }
}
