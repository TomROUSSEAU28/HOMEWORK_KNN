#include "KnnManhattan.h"

KnnManhattan::KnnManhattan(uint32_t k) : Knn::Knn(k){

}


double KnnManhattan::getDistanceKnn(const Sample& sampleTest, const Sample& sampleTrain) const{

    const FeatureVector& x = sampleTest.GetFeatures();
    const FeatureVector& y = sampleTrain.GetFeatures();
    
    // Distance euclidienne entre vecteurs normalisés min-max
    return x.ManhattanDistanceNormalized(y);
}
  
