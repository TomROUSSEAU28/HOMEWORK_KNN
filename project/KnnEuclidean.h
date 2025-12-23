#pragma once
#include "Knn.h"


class KnnEuclidean: public Knn {

private:

public:
    KnnEuclidean(uint32_t k);
protected:
  
    double getDistanceKnn(const Sample& sampleTest, const Sample& sampleTrain) const final;

 
};