#include "KnnEuclidean.h"

KnnEuclidean::KnnEuclidean(uint32_t k) : Knn::Knn(k){

}


double KnnEuclidean::getDistanceKnn(const Sample& sampleTest, const Sample& sampleTrain) const{

    const FeatureVector& x = sampleTest.GetFeatures();
    const FeatureVector& y = sampleTrain.GetFeatures();
    
    // Distance euclidienne entre vecteurs normalisés min-max
    return x.EuclideanDistanceNormalized(y);
}
  
