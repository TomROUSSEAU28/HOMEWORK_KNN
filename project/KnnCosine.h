#pragma once
#include "Knn.h"


class KnnCosine : public Knn {

private:

public:
    KnnCosine(uint32_t k);
protected:
  
    double getDistanceKnn(const Sample& sampleTest, const Sample& sampleTrain) const final;


};