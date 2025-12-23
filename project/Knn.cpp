#include "Knn.h"
#include "Data.h"
#include "ProgressBar.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <omp.h>


Knn::Knn(uint32_t k) : m_k(k) {
    
}


void Knn::computeClassWeights()
{
	std::map<uint32_t, int> class_counts;

	for (const auto& sample : m_trainData.GetData()) {
		class_counts[sample->GetTag()]++;
	}

	uint32_t total = m_trainData.GetNbSamples();

	// Poids = total / (nb_classes * count_classe)
    // Plus une classe est rare plus son poids est élevé
	for (const auto& [label, count] : class_counts) {
		m_class_weights[label] = (double)total / (class_counts.size() * count);
	}

}

void Knn::loadTrainData(const char* trainDataPath, bool isHollowData){

    if(isHollowData){
        m_trainData.LoadFileHollow(trainDataPath);
    }
    else{
        m_trainData.LoadFile(trainDataPath);
    }

    // normalise le TRAIN avec SES min/max
    m_trainData.UpdateMinMax();
    m_trainData.UpdateNormalizedVector();

    computeClassWeights();

    
}

void Knn::loadTestData(const char* testDataPath, bool isHollowData){

    if(m_trainData.GetData().empty()){
        throw std::runtime_error("Train data are empty. You must load train data before test data");
    }
    if(isHollowData){
        m_testData.LoadFileHollow(testDataPath);
    }
    else{
        m_testData.LoadFile(testDataPath);
    }
    // normalise le test avec les min max du test
    m_testData.updateNormalizedVectorWith(m_trainData.GetMax(), m_trainData.GetMin());

}




void Knn::Report()
{
    report.ComputeMetrics();
    report.PrintReport();
}

void Knn::Predict() 
{   
    const auto& testSamples = m_testData.GetData();
    const size_t n = testSamples.size();
    
  
    report.m_ground_truth.resize(n);
    report.m_predictions.resize(n);
    std::cout << "[Prediction en cours] ..." << std::endl;
    ProgressBar bar(n, ProgressBar::Color::YELLOW);
    // Boucle avec index pour threadSafe
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; ++i) {
        const auto& sampleTest = testSamples[i];
        
        // Prédiction
        uint32_t predicted_tag = predictSingle(*sampleTest);
        uint32_t true_tag = sampleTest->GetTag();
        
        // Écriture directe par index (thread-safe !)
        report.m_predictions[i] = predicted_tag;
        report.m_ground_truth[i] = true_tag;

        bar.incrementThreadSafe();

    }
    bar.finish();
    
}


uint32_t Knn::predictSingle(const Sample& sampleTest) const
{
    std::vector<std::pair<double, uint32_t>> distances; //stock 
    distances.reserve(m_trainData.GetNbSamples());

    for (const auto& sampleTrainPtr : m_trainData.GetData()) {
        const Sample& sampleTrain = *sampleTrainPtr; 
        double d = getDistanceKnn(sampleTest, sampleTrain);
        distances.emplace_back(d, sampleTrain.GetTag());
    }

    // On ne garde que les k plus proches (m_k)
    if (distances.size() > m_k) {
        // rearrange le vecteurs distances pour placer de begin a begin + _mk les élements les plus petits.
        std::nth_element(distances.begin(), distances.begin() + m_k, distances.end(), [](const auto& a, const auto& b){ return a.first < b.first; });
        // garde seulement les premiers m_k élements
        distances.resize(m_k);
    }

	if (m_weighted_voting) {
		// Vote PONDÉRÉ majoritaire sur les m_k plus proches
		std::unordered_map<uint32_t, double> weighted_votes;

		for (const auto& [dist, tag] : distances) {
			// Poids inversement proportionnel à la distance
			double weight_dist = 1.0 / (dist + 1e-6);
            double class_weight = m_use_class_balancing ? m_class_weights.at(tag) : 1.0;
            double weight = weight_dist * class_weight;
			weighted_votes[tag] += weight;
		}

		uint32_t bestTag = 0;
		double bestWeight = 0.0;
		for (const auto& [tag, weight] : weighted_votes) {
			if (weight > bestWeight) {
				bestWeight = weight;
				bestTag = tag;
			}
		}
		return bestTag;

	}

	// Vote majoritaire sur les m_k plus proches si m_weighted_voting = false
	std::unordered_map<uint32_t, uint32_t> counts;
	for (const auto& [dist, tag] : distances) {
		counts[tag]++;
	}

	uint32_t bestTag = 0;
	uint32_t bestCount = 0;
	for (const auto& [tag, c] : counts) {
		if (c > bestCount) {
			bestCount = c;
			bestTag = tag;
		}
	}

    return bestTag;

}


void Knn::ReduceDimension(const double threshold)
{
	if (m_trainData.GetData().empty()) {
		throw std::runtime_error("Train data are empty. You must load train data");
	}
	if (m_testData.GetData().empty()) {
		throw std::runtime_error("Test data are empty. You must load test data");
	}
    const std::vector<bool> dataKept = m_trainData.ReduceDimension(threshold);
    m_testData.ReduceDimensionWith(dataKept, m_trainData.GetMax(), m_trainData.GetMin());

    if (m_trainData.GetNbFeatures() == dataKept.size()){
        std::cout << "[DATA] : " << "Dimention non réduite car threshold trop faible " << "\n";;
    }
    else {
        std::cout << "[DATA] : " << "Dimention réduire à " << m_trainData.GetNbFeatures() << "\n";;
    }
    
}