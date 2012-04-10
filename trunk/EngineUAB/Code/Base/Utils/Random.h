//-----------------------------------------------------------------------
// CRandom class
// Clase de números Aleatorios
//-----------------------------------------------------------------------
#ifndef RANDOM_H
#define RANDOM_H

#include <numeric>
#include <assert.h>

class CRandom
{
  enum { Rand_MAX = 0x7FFF };
  public:
    CRandom (unsigned seed = 0x12345678): m_uSeed(seed) { }

    // Mecánica de la clase
    bool init (unsigned seed = 0x12345678)  { m_uSeed = seed; return true; }

    // Obtención de números pseudo-aleatorios
    signed    getRandBits     (unsigned uBits)               const { return static_cast<int>(getRand () & ((1<<uBits)-1));                           }
    unsigned  getRandUnsigned (unsigned uMin, unsigned uMax) const { const unsigned uDiv = (uMax - uMin + 1); assert(uDiv > 0); return uMin +  getRand() % (uDiv == 0 ? 1 : uDiv);  }
    signed    getRandSigned   (signed   uMin, signed   uMax) const { const unsigned uDiv = (static_cast<unsigned>(uMax - uMin + 1)); assert(uDiv > 0); return (signed) (uMin + (getRand() % (uDiv == 0 ? 1 : uDiv)));  }
    float     getRandFloat    (float    fMin, float    fMax) const { return fMin + (fMax - fMin) * static_cast<float>(getRand()) * getInvRandMax (); }
    bool      getRandBool     ()                             const { return getRandUnsigned (0, 1) == 1;                                             }

    // Información
    unsigned  getSeed () const { return m_uSeed; }

		static const CRandom& GetGlobalRandom() { return s_Random; }

  private:

		static CRandom  s_Random;

  
    unsigned        getRand () const;

    static unsigned getRandMax ()    { return Rand_MAX; }
    static float    getInvRandMax () { return 1.f / Rand_MAX; }
    
    mutable unsigned m_uSeed;
};

#endif