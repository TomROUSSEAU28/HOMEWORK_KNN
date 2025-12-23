#include "Sample.h"
#include "FeatureVector.h"
#include <iostream>


Sample::Sample(const uint32_t tag, const FeatureVector& features) : m_tag(tag), m_features(features)
{
    //
}

void Sample::PrintDebug() const noexcept{
    std::cout << "[SAMPLE] Tag : " << m_tag << '\n';
    m_features.PrintDebug();
}

const double& Sample::operator[](uint32_t index) const noexcept{
    return m_features[index];
}
