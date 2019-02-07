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

vector <Tupla3f> MallaRevol::rotarY(vector <Tupla3f> perfil, Matriz4f & mrot){
  vector <Tupla3f> sigPerfil;
  for(int i=0; i<perfil.size(); i++){
    sigPerfil.push_back(mrot*perfil.at(i));
  }
  return sigPerfil;
}

void MallaRevol::crearMallaRevol(const std::vector <Tupla3f> &perfil_original, //vertices
                    const unsigned nperfiles, //numero de perfiles
                    const bool crear_tapas, //true para crear tapas
                    const bool cerrar_malla,
                    const bool usar_texturas){ //true para cerrar malla
   //angulo y matriz de rotacion
   const float angulo_rot = 360.0/nperfiles; //si usamos mat_rot tienen que ser grados
   Matriz4f mrot = MAT_Rotacion(angulo_rot, 0.0,0.1,0.0);
   //guardamos los puntos del perfil
   setVertices(perfil_original);

   //en primer lugar vamos a meter todos los vertices y despues construiremos
   //las caras.
   vector <Tupla3f> siguientePerfil = perfil_original;
   for(unsigned i=1; i<nper; i++){
     siguientePerfil = rotarY(siguientePerfil,mrot);
     for(int j=0; j<siguientePerfil.size(); j++){
       vertices.push_back(siguientePerfil.at(j));
     }
   }
   //ahora construimos las caras
   if(!cerrar_malla){ //usamos modulos
     //volvemos a añadir el primer perfil
     for(int i=0; i<nvp; i++){
       vertices.push_back(vertices.at(i));
     }
     for(int i=0; i<nper; i++){
       for(int j=0; j<nvp-1; j++){
         //se guardan los vertices en el sentido contrario a las agujas del reloj
         Tupla3i caraA(i*nvp+j,(i+1)*nvp+j,i*nvp+j+1);
         Tupla3i caraB((i+1)*nvp+j,(i+1)*nvp+j+1,i*nvp+j+1);
         //introducimos el nuevo vertice de forma consecutiva
         caras.push_back(caraA);
         caras.push_back(caraB);
       }
     }
   }
   else{
     for(unsigned i=0; i<nper; i++){
       for(unsigned j=0; j<nvp-1; j++){
         //se guardan los vertices en el sentido contrario a las agujas del reloj
         Tupla3i caraA(i*nvp+j,((i+1)%nper)*nvp+j,i*nvp+j+1);
         Tupla3i caraB(((i+1)%nper)*nvp+j,((i+1)%nper)*nvp+j+1,i*nvp+j+1);
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
     Tupla3f tapa2(0.0,vertices.at(nvp-1)[1],0.0);

     vertices.push_back(tapa1);
     vertices.push_back(tapa2);
     //tapa superior
     for(unsigned i=0; i<nperfiles; i++){
       Tupla3i nuevaCaraSup(vertices.size()-2,((i+1)%nper)*nvp,(i%nper)*nvp);
       caras.push_back(nuevaCaraSup);
       Tupla3i nuevaCaraInf(vertices.size()-1,((i+1)%nper)*nvp+(nvp-1),
                            (i%nper)*nvp+(nvp-1));
       caras.push_back(nuevaCaraInf);
     }

   }

   calcular_normales();

   if(usar_texturas & !crear_tapas){
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
   ponerNombre( std::string("malla por revolución de la esfera" ));
   std::vector <Tupla3f> perfil;
   float radio = 1.0;
   float seccion = (radio*2.0)/(num_verts_per-1.0);
   for(unsigned i=0; i<num_verts_per;i++){
     float y = radio-seccion*i;
     perfil.push_back(Tupla3f(sqrt(radio*radio-y*y), y ,0.0));
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
      for (unsigned i = 0; i<nper ; i++){
        for(unsigned j=0; j<nvp; j++){
          si = (float)i/(float)(nper-1); //coordenada X en el espacio de la textura
          ti = 1-distancias.at(j)/distancias.at(nvp-1);
          cctt.push_back(Tupla2f(si,ti));
        }
      }
  }

  void MallaRevol::calcularDistancias(const std::vector <Tupla3f> &p){
    if(distancias.size()==0){distancias.push_back(0.0);}
    for(unsigned i=1; i<nvp; i++){
      float d = 0;
      d = distancias[i-1] + sqrt((p.at(i) - p.at(i-1)).lengthSq());
      distancias.push_back(d);
    }
  }

// *****************************************************************************
