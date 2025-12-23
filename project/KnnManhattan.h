#pragma once
#include "Knn.h"


class KnnManhattan : public Knn {

private:

public:
    KnnManhattan(uint32_t k);
protected:
  
    double getDistanceKnn(const Sample& sampleTest, const Sample& sampleTrain) const final;


};