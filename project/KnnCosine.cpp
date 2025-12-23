#include "KnnCosine.h"
#include <iostream>

KnnCosine::KnnCosine(uint32_t k) : Knn::Knn(k){

}


double KnnCosine::getDistanceKnn(const Sample& sampleTest, const Sample& sampleTrain) const{

    const FeatureVector& x = sampleTest.GetFeatures();
    const FeatureVector& y = sampleTrain.GetFeatures();

    // Produit scalaire entre vecteurs déjà normalisés min–max
    double cosTheta = x.DotProductNormalizedL2(y);
   
    // Distance cosinus : plus c'est petit, plus les vecteurs sont proches
    return 1.0 - cosTheta;
}
  
