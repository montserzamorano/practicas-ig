// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Montserrat Rodríguez Zamorano
// ** Declaraciones de la clase Textura
// *********************************************************************

#ifndef IG_TEXTURA_HPP
#define IG_TEXTURA_HPP

class Textura{
  public:
    GLuint idText ; //identificador OpenGL de la textura
    jpg::Imagen * img ; //puntero a objeto imagen con los texels en RAM
    unsigned mgct ; //modo generacion cc.tt.: 0=desactivado, 1=objeto, 2=mundo
    float cs[4], //coeficientes (S) para generación de cc.tt. (si mgct != 0)
          ct[4]; //coeficientes (T) para generación de cc.tt. (si mgct != 0)

    void activar(Contexto & cv) ;
}

#endif
