// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Montserrat Rodríguez Zamorano
// ** Implementación de la clase 'MallaRevol'
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include <matrices-tr.hpp>
#include <math.h>
#include "MallaRevol.hpp"


// *****************************************************************************
//constructor por defecto
MallaRevol::MallaRevol(){}

//funcion que nos da el punto que resulta de rotar el punto alrededor del eje Y
//un angulo angulo_rot
Tupla3f MallaRevol::rotarY(Tupla3f punto, float angulo_rotacion){
  vector <vector <float> > matriz_rot = {{cos(angulo_rotacion), 0, sin(angulo_rotacion)},
                                          {0,1,0},
                                          {-sin(angulo_rotacion),0,cos(angulo_rotacion)}};
  //multiplicar punto por angulo rotacion
  Tupla3f resultado;
  for(int i=0; i<3;i++){//filas
    float suma = 0.0;
    for(int j=0; j<3;j++){//columnas
      suma+=(matriz_rot.at(i)).at(j)*(punto[j]);
    }
    resultado[i]=suma;
  }
  return resultado;
}

void MallaRevol::crearMallaRevol(const std::vector <Tupla3f> &perfil_original, //vertices
                    const unsigned nperfiles, //numero de perfiles
                    const bool crear_tapas, //true para crear tapas
                    const bool cerrar_malla,
                    const bool usar_texturas){ //true para cerrar malla
   //angulo de rotacion
   const float angulo_rot = (2*3.14159)/nperfiles;
   setVertices(perfil_original); //guardamos los puntos del perfil
   int puntosPerfil = perfil_original.size();

   //en primer lugar vamos a meter todos los vertices y despues construiremos
   //las caras.

   for(unsigned i=1; i<nperfiles; i++){
     for(unsigned j=0; j<puntosPerfil; j++){
       Tupla3f nuevoPunto(rotarY(vertices.at(j), angulo_rot*(i)));
       //introducimos el nuevo vertice de forma consecutiva
       vertices.push_back(nuevoPunto);
     }
   }
   //ahora construimos las caras
   if(!cerrar_malla){ //usamos modulos
     //volvemos a añadir el primer perfil
     for(int i=0; i<puntosPerfil; i++){
       vertices.push_back(vertices.at(i));
     }
     for(int i=0; i<nperfiles; i++){
       for(int j=0; j<puntosPerfil-1; j++){
         //se guardan los vertices en el sentido contrario a las agujas del reloj
         Tupla3i caraA(i*puntosPerfil+j,(i+1)*puntosPerfil+j,i*puntosPerfil+j+1);
         Tupla3i caraB((i+1)*puntosPerfil+j,(i+1)*puntosPerfil+j+1,i*puntosPerfil+j+1);
         //introducimos el nuevo vertice de forma consecutiva
         caras.push_back(caraA);
         caras.push_back(caraB);
       }
     }
   }
   else{
     for(unsigned i=0; i<nperfiles; i++){
       for(unsigned j=0; j<puntosPerfil-1; j++){
         //se guardan los vertices en el sentido contrario a las agujas del reloj
         Tupla3i caraA(i*puntosPerfil+j,((i+1)%nperfiles)*puntosPerfil+j,i*puntosPerfil+j+1);
         Tupla3i caraB(((i+1)%nperfiles)*puntosPerfil+j,((i+1)%nperfiles)*puntosPerfil+j+1,i*puntosPerfil+j+1);
         //introducimos el nuevo vertice de forma consecutiva
         caras.push_back(caraA);
         caras.push_back(caraB);
       }
     }
   }
   //para terminar creamos las tapas

   if(crear_tapas){
     //guardamos los puntos para las tapas
     Tupla3f tapa1(0.0,vertices.at(0)[1],0.0);
     Tupla3f tapa2(0.0,vertices.at(puntosPerfil-1)[1],0.0);

     vertices.push_back(tapa1);
     vertices.push_back(tapa2);
     //tapa superior
     for(unsigned i=0; i<nperfiles-1; i++){
       Tupla3i nuevaCaraSup(vertices.size()-2,(i+1)*puntosPerfil,i*puntosPerfil);
       caras.push_back(nuevaCaraSup);
       Tupla3i nuevaCaraInf(vertices.size()-1,(i+1)*puntosPerfil+(puntosPerfil-1),
                            i*puntosPerfil+(puntosPerfil-1));
       caras.push_back(nuevaCaraInf);
     }

   }

   calcular_normales();

   if(usar_texturas){
     calcularDistancias(perfil_original);
     iniCoordenadasTextura();
   }

} //fin crear malla

MallaRevol::MallaRevol( const std::string & nombre_arch,
                        const unsigned nperfiles,
                        const bool     crear_tapas,
                        const bool     cerrar_malla){
   //ponemos nombre a la malla de revolucion
   nper = nperfiles;
   ponerNombre( std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ));
   //leer perfil del archivo
   std::vector <float> perfil_original;
   const char *nombreArch = nombre_arch.c_str();
   ply::read_vertices(nombreArch, perfil_original);
   //Convertimos vector de float a vector de 3f
   std::vector <Tupla3f> perfil;
   for(int i=0; i<perfil_original.size(); i+=3){
     perfil.push_back(Tupla3f(perfil_original.at(i),perfil_original.at(i+1),
              perfil_original.at(i+2)));
   }
   nvp = perfil.size();
   //crear malla revolucion
   crearMallaRevol(perfil, nper, crear_tapas, cerrar_malla, false);
}

MallaRevol::MallaRevol( const std::string & nombre_arch,
                        const unsigned nperfiles,
                        const bool     crear_tapas,
                        const bool     cerrar_malla,
                        const bool usar_texturas){
   //ponemos nombre a la malla de revolucion
   nper = nperfiles;
   ponerNombre( std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ));
   //leer perfil del archivo
   std::vector <float> perfil_original;
   const char *nombreArch = nombre_arch.c_str();
   ply::read_vertices(nombreArch, perfil_original);
   //Convertimos vector de float a vector de 3f
   std::vector <Tupla3f> perfil;
   for(int i=0; i<perfil_original.size(); i+=3){
     perfil.push_back(Tupla3f(perfil_original.at(i),perfil_original.at(i+1),
              perfil_original.at(i+2)));
   }
   nvp = perfil.size();
   //crear malla revolucion
   crearMallaRevol(perfil, nper, crear_tapas, cerrar_malla, usar_texturas);
}

Cilindro::Cilindro(const int num_verts_per, //numero de vertices del perfil original (M)
                  const unsigned nperfiles, //numero de perfiles (N)
                  const bool crear_tapas, //true para crear tapas
                  const bool cerrar_malla){ //true para cerrar la malla
   setnper(nperfiles);
   setnvp(num_verts_per);
   ponerNombre( std::string("malla por revolución del cilindro" ));
   std::vector <Tupla3f> perfil;
   for(unsigned i=0; i<num_verts_per;i++){
     perfil.push_back(Tupla3f(1.0,(1.0/num_verts_per)*i,0.0));
   }
   crearMallaRevol(perfil,nperfiles,crear_tapas,cerrar_malla, false);
}
Cono::Cono(
          const int num_verts_per, //numero de vertices del perfil original (M)
          const unsigned nperfiles, //numero de perfiles (N)
          const bool crear_tapas, //true para crear tapas
          const bool cerrar_malla){ //true para cerrar la malla
    setnper(nperfiles);
   setnvp(num_verts_per);
   ponerNombre( std::string("malla por revolución del cono" ));
   float radio = 1.0;
   float altura = 1.0;

   std::vector <Tupla3f> perfil;
   for(unsigned i=0; i<num_verts_per;i++){
     perfil.push_back(Tupla3f((radio/(num_verts_per-1.0))*(num_verts_per-1.0-i),
                              (altura/(num_verts_per-1.0))*i,
                              0.0));
   }
   crearMallaRevol(perfil,nperfiles,crear_tapas,cerrar_malla, false);
}
Esfera::Esfera(
              const int num_verts_per, //numero de vertices del perfil original(M)
              const unsigned nperfiles, //numero de perfiles (N)
              const bool crear_tapas, //true para crear tapas
              const bool cerrar_malla){ //true para cerrar la malla

   setnper(nperfiles);
   setnvp(num_verts_per);
   ponerNombre( std::string("malla por revolución del esfera" ));
   std::vector <Tupla3f> perfil;
   float radio = 1.0;
   float seccion = (radio*2.0)/(num_verts_per-1.0);
   for(unsigned i=0; i<num_verts_per;i++){
     perfil.push_back(Tupla3f(sqrt(radio*radio-(radio-seccion*i)*(radio-seccion*i)),
                              radio-seccion*i,
                              0.0));
   }
   crearMallaRevol(perfil,nperfiles,crear_tapas,cerrar_malla, false);
}

ConoTruncado::ConoTruncado(float radioBase, float radioTapa,
                          const int num_verts_per, const unsigned nperfiles,
                          const bool crear_tapas, const bool cerrar_malla){
    setnper(nperfiles);
    setnvp(num_verts_per);
    ponerNombre( std::string("malla por revolución del cono truncado" ));
    std::vector <Tupla3f> perfil;
    for(unsigned i=0; i<num_verts_per;i++){
      perfil.push_back(Tupla3f(((radioBase-radioTapa)/(num_verts_per-1.0))*(num_verts_per-1.0-i)+radioTapa,
                               (1.0/(num_verts_per-1.0))*i,
                               0.0));
    }
    crearMallaRevol(perfil,nperfiles,crear_tapas,cerrar_malla, false);
  }

  void MallaRevol::iniCoordenadasTextura(){
      float si, ti;
      //cout << "inicializando coordenadas de textura..." << endl;
      for (unsigned i = 0; i<nper ; i++){
        for(unsigned j=0; j<nvp; j++){
          si = float(i)/float(nper-1); //coordenada X en el espacio de la textura
          ti = 1-distancias.at(j)/distancias.at(nvp-1);
          cctt.push_back(Tupla2f(si,ti));
          //cout << si << "," << ti << endl;
        }
      }
  }

  void MallaRevol::calcularDistancias(const std::vector <Tupla3f> &p){
    //cout << "inicializando distancias..." << endl;
    if(distancias.size()==0){distancias.push_back(0.0);}
    for(unsigned i=1; i<nvp; i++){
      float d = 0;
      d = distancias[i-1] + sqrt((p[i] - p[i-1]).lengthSq());
      distancias.push_back(d);
    }
  }

// *****************************************************************************
