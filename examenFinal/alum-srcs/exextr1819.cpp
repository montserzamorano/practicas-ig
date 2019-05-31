// Examen de prácticas IG (GIM, 18-19, conv.extraordinaria, 11-feb 19)
// Implementación de métodos de clases.
//
// Nombre: Montserrat Rodríguez Zamorano
// DNI: 77391269H
// e-mail ugr: montserzamorano@correo.ugr.es

#include "exextr1819.hpp"


// #############################################################################
// P1: prisma regular sin normales

//codigo reciclado de malla revolucion
PrismaRegP1::PrismaRegP1( const int n ){
  if(n < 2){
    cout << "Número no válido de lados para la base del prisma sin normales." << endl;
  }
  else{
    ponerNombre( std::string("malla indexada del prisma sin normales" ));
    std::vector <Tupla3f> perfil;
    for(unsigned i=0; i<2;i++){
      perfil.push_back(Tupla3f(1.0,1.0*i,0.0));
    }
    //ahora creamos el resto de los perfiles

    const float angulo_rot = 360.0/n; //si usamos mat_rot tienen que ser grados
    Matriz4f mrot = MAT_Rotacion(angulo_rot, 0.0,0.1,0.0);
    //guardamos los puntos del perfil
    setVertices(perfil);

    //en primer lugar vamos a meter todos los vertices
    vector <Tupla3f> siguientePerfil = perfil;
    for(unsigned i=1; i<n; i++){
      vector <Tupla3f> sigPerfil;
      for(int i=0; i<perfil.size(); i++){
        sigPerfil.push_back(mrot*siguientePerfil.at(i));
      }
      siguientePerfil = sigPerfil;
      for(int j=0; j<siguientePerfil.size(); j++){
        vertices.push_back(siguientePerfil.at(j));
      }
    }
    //ahora construimos las caras (suponemos que siempre se cierra la malla)
    for(unsigned i=0; i<n; i++){
      //se guardan los vertices en el sentido contrario a las agujas del reloj
      Tupla3i caraA(i*2,((i+1)%n)*2,i*2+1);
      Tupla3i caraB(((i+1)%n)*2,((i+1)%n)*2+1,i*2+1);
      //introducimos el nuevo vertice de forma consecutiva
      caras.push_back(caraA);
      caras.push_back(caraB);
    }

    //guardamos los puntos para las tapas
    Tupla3f tapa1(0.0,vertices.at(0)[1],0.0);
    Tupla3f tapa2(0.0,vertices.at(1)[1],0.0);

    vertices.push_back(tapa1);
    vertices.push_back(tapa2);
    //tapa superior
    for(unsigned i=0; i<n; i++){
      Tupla3i nuevaCaraSup(vertices.size()-2,((i+1)%n)*2,(i%n)*2);
      caras.push_back(nuevaCaraSup);
      Tupla3i nuevaCaraInf(vertices.size()-1,((i+1)%n)*2+(2-1),
                           (i%n)*2+(2-1));
      caras.push_back(nuevaCaraInf);
    }
    //comprobamos que el numero de vertices y caras esta bien
    if(vertices.size()==(2*n+2)){
      cout << "Número de vértices " << vertices.size() <<" correcto" << endl;
    }
    else{
      cout << "Número de vértices " << vertices.size() <<" no correcto" << endl;
    }
    if(caras.size()==4*n){
      cout << "Número de caras " << caras.size() <<" correcto" << endl;
    }
    else{
      cout << "Número de caras " << caras.size() <<" no correcto" << endl;
    }

  }//end else

}//end prismaregp1


// #############################################################################
// P2: prisma regular con normales

PrismaRegP2::PrismaRegP2( const int n ){
  if(n < 2){
    cout << "Número no válido de lados para la base del prisma." << endl;
  }
  else{
    ponerNombre( std::string("malla indexada del prisma" ));
    std::vector <Tupla3f> perfil;
    for(unsigned i=0; i<2;i++){
      perfil.push_back(Tupla3f(1.0,1.0*i,0.0));
    }
    //ahora creamos el resto de los perfiles

    const float angulo_rot = 360.0/n; //si usamos mat_rot tienen que ser grados
    Matriz4f mrot = MAT_Rotacion(angulo_rot, 0.0,0.1,0.0);
    //guardamos los puntos del perfil
    setVertices(perfil);

    //en primer lugar vamos a meter todos los vertices
    vector <Tupla3f> siguientePerfil = perfil;
    for(unsigned i=1; i<n; i++){
      vector <Tupla3f> sigPerfil;
      for(int i=0; i<perfil.size(); i++){
        sigPerfil.push_back(mrot*siguientePerfil.at(i));
      }
      siguientePerfil = sigPerfil;
      for(int j=0; j<siguientePerfil.size(); j++){
        vertices.push_back(siguientePerfil.at(j));
      }
    }
    //ahora construimos las caras (suponemos que siempre se cierra la malla)
    for(unsigned i=0; i<n; i++){
      //se guardan los vertices en el sentido contrario a las agujas del reloj
      Tupla3i caraA(i*2,((i+1)%n)*2,i*2+1);
      Tupla3i caraB(((i+1)%n)*2,((i+1)%n)*2+1,i*2+1);
      //introducimos el nuevo vertice de forma consecutiva
      caras.push_back(caraA);
      caras.push_back(caraB);
    }

    //guardamos los puntos para las tapas
    Tupla3f tapa1(0.0,vertices.at(0)[1],0.0);
    Tupla3f tapa2(0.0,vertices.at(1)[1],0.0);

    vertices.push_back(tapa1);
    vertices.push_back(tapa2);
    //tapa superior
    for(unsigned i=0; i<n; i++){
      Tupla3i nuevaCaraSup(vertices.size()-2,((i+1)%n)*2,(i%n)*2);
      caras.push_back(nuevaCaraSup);
      Tupla3i nuevaCaraInf(vertices.size()-1,((i+1)%n)*2+(2-1),
                           (i%n)*2+(2-1));
      caras.push_back(nuevaCaraInf);
    }
    //comprobamos que el numero de vertices y caras esta bien
    calcular_normales();
  }//end else
}

// #############################################################################
// P3: rejilla de prismas,

PrismasP3::PrismasP3( const int nf, const int nc ){
  agregar(MAT_Escalado(0.5,2.0,0.5));
  for(unsigned i=0; i<nc; i++){
    float retroceso=0.0;
    agregar(MAT_Traslacion(0.0,0.0,-3));
    retroceso=aniadeFila(nf);
    agregar(MAT_Traslacion(-retroceso,0.0,0.0));
  }
}

float PrismasP3::aniadeFila(const int nf){
  float retroceso = 0.0;
  for(unsigned j=3; j<=(nf+2); j++){
    retroceso+=3;
    agregar(MAT_Traslacion(3,0.0,0.0));
    aniadePrisma(j);
  }
  return retroceso;
}

//el parametro no esta bien, tendria que haber calculado la posicion
//para cada prisma en la que está la matriz MAT_Traslacion(0,0,0)
//y modificar esa

void PrismasP3::aniadePrisma(const int ind){
  glLoadIdentity();
  agregar(MAT_Traslacion(0.0,0.0,0.0)); //esta es la que modificara el parametro
  PrismaRegP2 *np = new PrismaRegP2(ind);
  agregar(np);
  string mensaje = "Rotación de los prismas alrededor del eje Y.";
  parametros.push_back(Parametro(mensaje, entradas[1].matriz,
              [=](float v){return MAT_Rotacion(v,0.0,1.0,0.0);},
              false, 0.0, 15.0, 0.5));
}

// #############################################################################
// P4: rejilla de prismas con material texturado.

TexturaPrisma::TexturaPrisma():Textura("./text-madera-examen.jpg"){
    modo_gen_ct = mgct_coords_objeto;
    //s=x
    coefs_s[0]=1.0/2 + 0.5;
    coefs_s[1]=0.5;
    coefs_s[2]=0.5;
    coefs_s[3]=0.5;
    //t=y
    coefs_t[0]=0.5;
    coefs_t[1]=0.5;
    coefs_t[2]=0.5;
    coefs_t[3]=1.0/2 + 0.5;
}
// -----------------------------------------------------------------------------

MatPrismas::MatPrismas():Material(new TexturaPrisma(),0.2,0.6,0.2,5.0){
  ponerNombre("material textura prisma examen");
}
// -----------------------------------------------------------------------------

PrismasP4::PrismasP4( const int nf, const int nc){
  agregar(new MatPrismas());
  agregar(MAT_Escalado(0.5,2.0,0.5));
  for(unsigned i=0; i<nc; i++){
    float retroceso=0.0;
    agregar(MAT_Traslacion(0.0,0.0,-3.0));
    retroceso=aniadeFila(nf);
    agregar(MAT_Traslacion(-retroceso,0.0,0.0));
  }
}

float PrismasP4::aniadeFila(const int nf){
  float retroceso = 0.0;
  for(unsigned j=3; j<=(nf+2); j++){
    retroceso+=3.0;
    agregar(MAT_Traslacion(3,0.0,0.0));
    PrismaRegP2 *np = new PrismaRegP2(j);
    agregar(np);
  }
  return retroceso;
}

void PrismasP3::reiniciar(){
  for(unsigned i=0; i<parametros.size(); i++){
    parametros.at(i).reset();
  }
}

// #############################################################################
// P5: rejilla de prismas , preparada para selección.

//PrismasP5::PrismasP5( const int nf, const int nc ){}

// #############################################################################
