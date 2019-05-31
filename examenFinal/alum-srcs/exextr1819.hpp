// Examen de prácticas IG (GIM, 18-19, conv.extraordinaria, 11-feb 2019)
// Declaración de clases
//
// Nombre: Montserrat Rodríguez Zamorano
// DNI: 77391269H
// e-mail ugr: montserzamorano@correo.ugr.es

#ifndef EXEXTR1819_HPP
#define EXEXTR1819_HPP

#include "MallaInd.hpp"
#include "grafo-escena.hpp"
#include "materiales.hpp"
#include "MallaRevol.hpp"

// examen prács. conv. extraord. 18-19

// -----------------------------------------------------------------------------
// P1: prisma regular sin normales

class PrismaRegP1 : public MallaInd
{
   public:
     PrismaRegP1( const int n );
} ;

// -----------------------------------------------------------------------------
// P2: prisma regular con normales

class PrismaRegP2 : public MallaInd
{
   public: PrismaRegP2( const int n );
} ;

// -----------------------------------------------------------------------------
// P3: rejilla de prismas

class PrismasP3 : public NodoGrafoEscenaParam
{
   public:
     PrismasP3( const int nf, const int nc);
     float aniadeFila(const int nf);
     void reiniciar();
     void aniadePrisma(const int ind);
} ;

// -----------------------------------------------------------------------------
// P4: rejilla de prismas, con material texturado

class TexturaPrisma : public Textura
{
   public :
    TexturaPrisma() ;
} ;

class MatPrismas : public Material
{
   public: MatPrismas();
};

class PrismasP4 : public NodoGrafoEscena
{
   public:
     PrismasP4( const int nf, const int nc );
     float aniadeFila(const int nf);
} ;

// -----------------------------------------------------------------------------
// P5: rejilla de prismas, preparada para selección.

/*class PrismasP5 : public NodoGrafoEscena
{
   public: PrismasP5( const int nf, const int nc );
} ;*/

#endif // EXEXTR_HPP
