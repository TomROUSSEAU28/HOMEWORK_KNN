#include "Data.h"
#include "FeatureVector.h"
#include "Sample.h"
#include <cstdint>
#include <fstream>
#include <memory>
#include <vector>


Data::Data(){
  
}

const Sample& Data::operator[](uint32_t index) const noexcept{
    return *m_data[index];
}


void Data::PrintDebug()const noexcept{
    for(int i = 0; i < m_data.size(); i++){
        if(m_data[i] == nullptr) continue;
        m_data[i]->PrintDebug();
    }
}

bool Data::LoadFile(const char* dir) {

    std::ifstream file(dir);
    if(file){
        if (!(file >> m_nbSamples >> m_nbFeatures)) return false;

        m_data.reserve(m_nbSamples);

        for(int s = 0; s < m_nbSamples; s++){
            uint32_t tag;
            std::vector<double> vec;
            vec.reserve(m_nbFeatures);
            file >> tag;
            for(int f = 0; f < m_nbFeatures; f++){
                double val = 0;
                if (!(file >> val)) return false;
                vec.push_back(val);
            }
            FeatureVector vector(vec);
            m_data.push_back(std::make_unique<Sample>(tag, vector));
        }

        return true;

    }

    else{
        return false;
    }
}