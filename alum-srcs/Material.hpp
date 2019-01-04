// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Montserrat Rodríguez Zamorano
// ** Declaraciones de la clase Material
// *********************************************************************

#ifndef IG_MATERIAL_HPP
#define IG_MATERIAL_HPP

#include "Material.hpp"
#include "Textura.hpp"

////////////////////// MATERIAL (clase abstracta) /////////////////////////
class Material {
  public:
    virtual void activar(ContextoVis &cv) = 0;
};
////////////////////// MATERIAL ESTANDAR /////////////////////////////////
class MaterialEstandar : public Material {
  public:
    Textura * textura ; //punt. a textura (NULL si no hay)
    Tupla4f color[4] ; //0=Me; 1=Ma; 2=Md; 3=Ms
    float exponente; //exponente (e)

    virtual void activar(ContextoVis & cv); //activa material
};
////////////////////// MATERIAL CONCRETO /////////////////////////////////
//tipo de material lata con textura de coca-cola

class MaterialLata : public MaterialEstandar{
  public:
    MaterialLata();
};

//tipo de material: ninguno

class MaterialTapasLata : public MaterialEstandar{
  public:
    MaterialTapasLata();
};

//tipo de material peon con textura de madera

class MaterialPeonMadera : public MaterialEstandar{
  public:
    MaterialPeonMadera();
};

//tipo de material: ninguno

class MaterialPeonBlanco : public MaterialEstandar{
  public:
    MaterialPeonBlanco();
};

//tipo de material: ninguno

class MaterialPeonNegro : public MaterialEstandar{
  public:
    MaterialPeonNegro();
};

#endif
