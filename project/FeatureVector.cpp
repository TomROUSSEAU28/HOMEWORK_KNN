#include "FeatureVector.h"
#include <cmath>
#include <cstdint>
#include <exception>
#include <vector>
#include <iostream>



FeatureVector::FeatureVector()
{   
    //
}

FeatureVector::FeatureVector(const std::vector<double>& featureVec) : m_featureVec(featureVec)
{   
    UpdateNormalizedVector();
}

void FeatureVector::PrintDebug() const noexcept{
    uint32_t size_ = size();
    
    std::cout << "  [VECTEUR] : \n";
    std::cout << "      ";
    for (int i = 0; i<size_; i++){
        std::cout << m_featureVec[i] << ", ";
    }
    std::cout << '\n';

    std::cout << "  [VECTEUR NORMALISE] : \n";
    std::cout << "      ";
    for (int i = 0; i<size_; i++){
        std::cout << m_normalized_featureVec[i] << ", ";
    }
    std::cout << "\n\n";
}

void FeatureVector::UpdateNormalizedVector(){

    m_normalized_featureVec.clear();
    uint32_t size = m_featureVec.size();
    double norme = Norme();

    m_normalized_featureVec.reserve(size);
    for(const auto& val : m_featureVec){
        m_normalized_featureVec.push_back(val/norme);
    }
}

double FeatureVector::Norme() const noexcept{
    uint32_t size_ = size();

    double norme = 0;
    for (const auto& val : m_featureVec){
        norme+= std::pow(val, 2);
    }
    norme = std::sqrt(norme);
    
    return norme;
}

double FeatureVector::DotProduct(const FeatureVector& otherVector) const{
    
    uint32_t size_ = size();
    if(size_ != otherVector.size()) throw std::invalid_argument("DotProduct operation in class otherVector : size mismatch");

    double result = 0;
    for(uint32_t i = 0; i < size_; i++){
        result+= m_featureVec[i] * otherVector.m_featureVec[i];
    }
    return result;

}

double FeatureVector::DotProductNormalized(const FeatureVector& otherVector) const{
   
    uint32_t size_ = m_normalized_featureVec.size();
    if(size_ != otherVector.m_normalized_featureVec.size()) throw std::invalid_argument("DotProductNormalized operation in class otherVector: size mismatch");

    double result = 0;
    for(uint32_t i = 0; i < size_; i++){
        result+= m_normalized_featureVec[i] * otherVector.m_normalized_featureVec[i];
    }
    return result;
}


uint32_t FeatureVector::size() const noexcept{
    return m_featureVec.size();
}

FeatureVector::iterator FeatureVector::begin() noexcept{
    return m_featureVec.begin();
}

FeatureVector::const_iterator FeatureVector::begin() const noexcept{
    return m_featureVec.begin();
}

FeatureVector::iterator FeatureVector::end() noexcept{
    return m_featureVec.end();
}

FeatureVector::const_iterator FeatureVector::end() const noexcept{
    return m_featureVec.end();
}

void FeatureVector::reserve(uint32_t val){
    m_featureVec.reserve(val);
}

void FeatureVector::push_back(const double val){
    m_featureVec.push_back(val);
}

const double& FeatureVector::operator[](uint32_t index) const noexcept {
    return m_featureVec[index];
}

double& FeatureVector::operator[](uint32_t index) noexcept{
    return m_featureVec[index];
}


