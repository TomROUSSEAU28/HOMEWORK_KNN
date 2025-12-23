#include "FeatureVector.h"
#include <cmath>
#include <cstdint>
#include <exception>
#include <ostream>
#include <vector>
#include <iostream>
#include <algorithm>


FeatureVector::FeatureVector()
{   
    //
}

FeatureVector::FeatureVector(const std::vector<double>& featureVec) : m_featureVec(featureVec)
{   
    
}

void FeatureVector::PrintDebug() const noexcept {
    std::cout << "  [VECTEUR BRUT] (taille=" << size() << ") : ";
    for (const auto& val : m_featureVec) {
        std::cout << val << ", ";
    }
    std::cout << "\n  [VECTEUR NORMALISE MIN-MAX] : ";
    if(m_normalized_featureVec.empty()) {
        std::cout << "(non initialisé)";
    } else {
        for (const auto& val : m_normalized_featureVec) {
            std::cout << val << ", ";
        }
    }
    std::cout << "\n  [VECTEUR NORMALISE L2] : ";
    if(m_normalized_featureVec_L2.empty()) {
        std::cout << "(non initialisé)";
    } else {
        for (const auto& val : m_normalized_featureVec_L2) {
            std::cout << val << ", ";
        }
    }
    std::cout << "\n\n";
}


void FeatureVector::UpdateNormalizedVector(const std::vector<double>& max,const std::vector<double>& min){
    const uint32_t n = m_featureVec.size();
    m_normalized_featureVec.resize(n);

    for (uint32_t i = 0; i < n; ++i) {
        const double denom = max[i] - min[i];
        if (denom == 0.0) {
            // cas où max == min : toute la colonne est constante
            m_normalized_featureVec[i] = 0.0; 
        } else {
            m_normalized_featureVec[i] = (m_featureVec[i] - min[i]) / denom;
        }
    }
}

void FeatureVector::NormalizeL2() {
    double norm2 = NormeNormalized();
    m_normalized_featureVec_L2 = m_normalized_featureVec;
    if (norm2 == 0.0) return; // vecteur nul : on ne change rien
    for (double& v : m_normalized_featureVec_L2)
        v /= norm2;
}


double FeatureVector::Norme() const noexcept{

    double norme = 0;
    for (const auto& val : m_featureVec){
        norme += val * val;
    }
    norme = std::sqrt(norme);
    
    return norme;
}

double FeatureVector::NormeNormalized() const noexcept{

    double norme = 0;
    for (const auto& val : m_normalized_featureVec){
        norme+= std::pow(val, 2);
    }
    norme = std::sqrt(norme);
    
    return norme;
}


double FeatureVector::DotProductNormalizedL2(const FeatureVector& otherVector) const{
   
    uint32_t size_ = m_normalized_featureVec_L2.size();
    if(size_ != otherVector.m_normalized_featureVec_L2.size()) throw std::invalid_argument("DotProductNormalized operation in class otherVector: size mismatch");

    double result = 0;
    for(uint32_t i = 0; i < size_; i++){
        result+= m_normalized_featureVec_L2[i] * otherVector.m_normalized_featureVec_L2[i];
    }
    return result;
}




double FeatureVector::EuclideanDistanceNormalized(const FeatureVector& other) const {
    const uint32_t n = m_normalized_featureVec.size();
    
    // Calcul de la somme des carrés des différences
    double sumSquaredDiff = 0.0;
    for (uint32_t i = 0; i < n; ++i) {
        const double diff = m_normalized_featureVec[i] - other.m_normalized_featureVec[i];
        sumSquaredDiff += diff * diff;
    }
    
    // Racine carrée pour obtenir la distance euclidienne
    return std::sqrt(sumSquaredDiff);
}

double FeatureVector::ManhattanDistanceNormalized(const FeatureVector& other) const {
    const uint32_t n = m_normalized_featureVec.size();
    
    // Calcul de la somme des valeurs absolues des différences
    double sumAbsDiff = 0.0;
    for (uint32_t i = 0; i < n; ++i) {
        const double diff = m_normalized_featureVec[i] - other.m_normalized_featureVec[i];
        sumAbsDiff += std::abs(diff);
    }
    
    return sumAbsDiff;
}
uint32_t FeatureVector::size() const noexcept{
    return m_featureVec.size();
}

FeatureVector::const_iterator FeatureVector::begin() const noexcept{
    return m_featureVec.begin();
}

FeatureVector::const_iterator FeatureVector::end() const noexcept{
    return m_featureVec.end();
}

void FeatureVector::reserve(uint32_t val){
    m_featureVec.reserve(val);
}

const double& FeatureVector::operator[](uint32_t index) const noexcept {
    return m_featureVec[index];
}


