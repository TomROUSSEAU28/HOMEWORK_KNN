#include "Data.h"
#include "FeatureVector.h"
#include "Sample.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include "ProgressBar.h"
#include <omp.h>

Data::Data(){
  
}

const Sample& Data::operator[](uint32_t index) const noexcept{
    return *m_data[index];
}


void Data::popVar(const uint32_t index)
{
    for (int ligne = 0; ligne < m_nbSamples; ligne++) {
        std::vector<double>& vec = (*m_data[ligne]).GetFeatures().GetData();
        std::vector<double>& vecNorm = (*m_data[ligne]).GetFeatures().GetDataNormalized();
        std::vector<double>& vecNormL2 = (*m_data[ligne]).GetFeatures().GetDataNormalizedL2();
        vec.erase(vec.begin() + index);
        if (!vecNorm.empty()) {
            vecNorm.erase(vecNorm.begin() + index);
        }
        if (!vecNormL2.empty()) {
            vecNormL2.erase(vecNormL2.begin() + index);
        }
        
        
    }	
}

void Data::PrintDebug()const noexcept{
    for(int i = 0; i < m_data.size(); i++){
        if(m_data[i] == nullptr) continue;
        m_data[i]->PrintDebug();
    }
}

void Data::LoadFile(const char* dir) {

    m_data.clear();
    std::ifstream file(dir);
    // active les exceptions sur le fichier
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    if(file){
        file >> m_nbSamples >> m_nbFeatures;

        m_data.reserve(m_nbSamples);

        std::cout << "[Ouverture du fichier] : " << dir << " en cours..." << std::endl;
        ProgressBar bar(m_nbSamples, ProgressBar::Color::BLUE);
        for(int s = 0; s < m_nbSamples; s++){
            uint32_t tag;
            std::vector<double> vec;
            vec.reserve(m_nbFeatures);
            file >> tag;
            for(int f = 0; f < m_nbFeatures; f++){
                double val = 0;
                file >> val;
                vec.push_back(val);
            }
            FeatureVector vector(vec);
            m_data.push_back(std::make_unique<Sample>(tag, vector));
            bar.increment();
        }
        bar.finish();
        
       
        std::cout << "[DATA] : Fichié chargé avec " << m_nbSamples <<" samples" << " de " << m_nbFeatures << " features" <<"\n";

    }

}




void Data::LoadFileHollow(const char* dir) {
    
    m_data.clear();
    std::ifstream file(dir);
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    if (file) {
        
        

        file >> m_nbSamples >> m_nbFeatures;
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // finir la ligne

        m_data.reserve(m_nbSamples);
        std::string line;

        std::cout << "[Ouverture du fichier] : " << dir << " en cours..." << std::endl;
        ProgressBar bar(m_nbSamples, ProgressBar::Color::BLUE);
        
        for (int s = 0; s < m_nbSamples; ++s) {
            if (!std::getline(file, line) || line.empty()) {
                continue; // ligne vide éventuelle
            }

            std::stringstream lineS(line);

            // vecteur de features initialisé à 0
            std::vector<double> vec(m_nbFeatures, 0.0);

            // lecture du label
            uint32_t tag;
            lineS >> tag;

            // lecture des couples index:value jusqu'à la fin de la ligne
            std::string token;
            while (lineS >> token) {
                auto pos = token.find(':');
                if (pos == std::string::npos) {
                    continue; // token mal formé, on ignore
                }

                int idx = std::stoi(token.substr(0, pos));      // index dans le format SVM (souvent 1-based)
                double val = std::stod(token.substr(pos + 1));  // valeur

                // si le format est 1-based, on décale de 1 pour l'accès au vecteur 0-based
                if (idx >= 1 && idx <= m_nbFeatures) {
                    vec[idx - 1] = val;
                }
            }

            FeatureVector vector(vec);
            m_data.push_back(std::make_unique<Sample>(tag, vector));
            bar.increment();
        }
        bar.finish();
        
        std::cout << "[DATA] : Fichier chargé avec "
                  << m_nbSamples << " samples de "
                  << m_nbFeatures << " features\n";
    }
}

 void Data::UpdateMinMax(){

    m_max.resize(m_nbFeatures, -10000);
    m_min.resize(m_nbFeatures, 10000);

	std::cout << "[Data Mise à jour de Min et Max] : " << " en cours..." << std::endl;
	ProgressBar bar(m_nbFeatures, ProgressBar::Color::MAGENTA);

    for (int col = 0; col<m_nbFeatures; col++){
        for(int ligne = 0; ligne<m_nbSamples; ligne++){
            if ((*m_data[ligne])[col] > m_max[col]){
                m_max[col] = (*m_data[ligne])[col];
            }

             if ((*m_data[ligne])[col] < m_min[col]){
                m_min[col] = (*m_data[ligne])[col];
            }

        }
        bar.increment();
       
    }
    bar.finish();

 }

 void Data::UpdateNormalizedVector(){
    for (const auto& sample : m_data){
        auto& fv = sample->GetFeatures();
        fv.UpdateNormalizedVector(m_max, m_min); // min–max
        fv.NormalizeL2(); 
        fv.LockAfterNormalization();  
    }
 }

 // !Utile pour normalisé les data de test à partir des max et min du test.
 void Data::updateNormalizedVectorWith(const std::vector<double>& max, const std::vector<double>& min){
    for (const auto& sample : m_data){
        auto& fv = sample->GetFeatures();
        fv.UpdateNormalizedVector(max, min); // min–max avec min/max du train
        fv.NormalizeL2();
        fv.LockAfterNormalization();
    }
 }

 const std::vector<bool> Data::ReduceDimension(const double threshold)
 {     
     //Réduit la dimention sur le Dataset
     if (threshold <= 0 || threshold >= 1) {
         throw std::runtime_error("threshold must be > 0 and < 1");
     }
     uint32_t originalNbFeatures = m_nbFeatures;
     std::vector<bool> dataKept(originalNbFeatures, true);

	 std::cout << "[Data Analyse de la variance] : " << " en cours..." << std::endl;
	 ProgressBar bar(originalNbFeatures, ProgressBar::Color::MAGENTA);

     #pragma omp parallel for schedule(static)
	 // ANALYSER TOUT sans rien modifier
	 for (int col = 0; col < originalNbFeatures; col++) {

		 double mean = 0.0;

		 for (int ligne = 0; ligne < m_nbSamples; ligne++) {
			 const std::vector<double>& dataNorm = (*m_data[ligne]).GetFeatures().GetDataNormalized();
			 mean += dataNorm[col];
		 }
		 mean /= static_cast<double>(m_nbSamples);

		 double variance = 0.0;
		 for (int ligne = 0; ligne < m_nbSamples; ligne++) {
			 const std::vector<double>& dataNorm = (*m_data[ligne]).GetFeatures().GetDataNormalized();
			 double diff = dataNorm[col] - mean;
			 variance += diff * diff;
		 }
		 variance /= static_cast<double>(m_nbSamples - 1);

		 if (variance <= threshold) {
			 dataKept[col] = false;
		 }
         bar.incrementThreadSafe();
		 
	 }
     bar.finish();

	 std::cout << "[Filtrage des features] : " << " en cours..." << std::endl;
	 ProgressBar bar_delete(originalNbFeatures, ProgressBar::Color::MAGENTA);
	 // SUPPRIMER de la FIN vers le DÉBUT, plus simple car tableau décale les élements de droite a gauche lors d'une suppression
	 for (int col = originalNbFeatures - 1; col >= 0; col--) {
		 if (dataKept[col] == false) {
			 popVar(col);
			 m_nbFeatures--;
		 }
         bar_delete.increment();
	 }
     bar_delete.finish();

     UpdateMinMax();
     UpdateNormalizedVector();
     return dataKept;

 }
 // mettre min max
 void Data::ReduceDimensionWith(const std::vector<bool>& dataKept, const std::vector<double>& max, const std::vector<double>& min)
 {
    
	 // Vérifier que le masque correspond aux features ACTUELLES
	 if (dataKept.size() != m_nbFeatures) {
		 std::stringstream ss;
		 ss << "ERREUR: Le masque a " << dataKept.size() << " éléments mais le dataset a " << m_nbFeatures << " features!";
		 throw std::runtime_error(ss.str());
	 }

	 // Supprimer de la FIN vers le DÉBUT
	 for (int col = m_nbFeatures - 1; col >= 0; col--) {
		 if (!dataKept[col]) {
			 popVar(col);
			 m_nbFeatures--;
		 }
	 }

	 m_max = max;
	 m_min = min;
     UpdateNormalizedVector();
     
 }
