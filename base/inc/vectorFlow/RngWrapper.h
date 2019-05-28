#ifndef VECTORFLOW_RNGWRAPPER_H
#define VECTORFLOW_RNGWRAPPER_H

#include <VecMath/Rng.h>
#include <VecCore/VecCore>
#include <vectorFlow/VectorTypes.h>

namespace vectorflow {

class RngWrapper {
public:
  RngWrapper()
  {
    void *buff     = vecCore::AlignedAlloc(32, sizeof(vecRng::MRG32k3a<vecCore::backend::Scalar>));
    mrg32k3aScalar = new (buff) vecRng::MRG32k3a<vecCore::backend::Scalar>;

    buff        = vecCore::AlignedAlloc(32, sizeof(vecRng::MRG32k3a<VectorBackend>));
    mrg32k3aVec = new (buff) vecRng::MRG32k3a<VectorBackend>;

    mrg32k3aScalar->Initialize();
    mrg32k3aVec->Initialize();
  }

  ~RngWrapper()
  {
    mrg32k3aScalar->~MRG32k3a();
    vecCore::AlignedFree(mrg32k3aScalar);
    mrg32k3aVec->~MRG32k3a();
    vecCore::AlignedFree(mrg32k3aVec);
  }

  double uniform() { return mrg32k3aScalar->Uniform<vecCore::backend::Scalar>(); }

  double uniform(double a, double b) { return a + (b - a) * uniform(); }

  void uniform_array(size_t n, double *array, const double min = 0., const double max = 1.)
  {
    for (size_t i = 0; i < n; ++i) {
      array[i] = uniform(min, max);
    }
  }

  Double_v uniformV() { return mrg32k3aVec->Uniform<VectorBackend>(); }

  double Gauss(double mean, double sigma) { return mrg32k3aScalar->Gauss<vecCore::backend::Scalar>(mean, sigma); }

private:
  vecRng::MRG32k3a<vecCore::backend::Scalar> *mrg32k3aScalar;
  vecRng::MRG32k3a<VectorBackend> *mrg32k3aVec;
};
} // namespace vectorflow

#endif // VECTORFLOW_VECRNGWRAPPER_H
