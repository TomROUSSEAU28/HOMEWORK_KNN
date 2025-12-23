
#include <cstdint>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <set>


class ClassificationReport {
private:

    std::vector<uint32_t> m_predictions;     /**< Étiquettes prédites */
    std::vector<uint32_t> m_ground_truth;    /**< Étiquettes réelles */

    // Métriques calculées
    bool m_metric_computed = false;
    double m_accuracy = 0.0;
    std::map<uint32_t, std::map<uint32_t, uint32_t>> m_confusion_matrix;
    std::set<uint32_t> m_classes;
    std::map<uint32_t, double> m_precision;
    std::map<uint32_t, double> m_recall;
    std::map<uint32_t, double> m_f1_score;
    std::map<uint32_t, uint32_t> m_support;


     


public:
    ClassificationReport() = default;
    ~ClassificationReport() = default;


    /**
     * @brief Calcule toutes les métriques 
     */
    void ComputeMetrics();

    void PrintReport() const;



    friend class Knn;

    

};

