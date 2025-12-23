#include "ClassificationReport.h"
#include <stdexcept>

void ClassificationReport::ComputeMetrics() {
    if (m_predictions.empty()) {
        throw std::runtime_error("m_predictions is empty !!");
    }

    const size_t n = m_predictions.size();
    uint32_t correct = 0;
    
    // Réinitialisation
    m_confusion_matrix.clear();
    m_classes.clear();
    m_precision.clear();
    m_recall.clear();
    m_f1_score.clear();
    m_support.clear();

    // Construction de la matrice de confusion et identification des classes
    for (size_t i = 0; i < n; ++i) {
        uint32_t true_label = m_ground_truth[i];
        uint32_t pred_label = m_predictions[i];
        
        m_classes.insert(true_label);
        m_classes.insert(pred_label);
        
        m_confusion_matrix[true_label][pred_label]++;
        
        if (true_label == pred_label) {
            correct++;
        }
    }
    
    // Calcul de l'accuracy globale
    m_accuracy = static_cast<double>(correct) / n;

    // Calcul des métriques par classe
    for (uint32_t class_label : m_classes) {
        // Calcul du support (nombre d'instances réelles de la classe)
        uint32_t support = 0;
        if (m_confusion_matrix.count(class_label)) {
            for (const auto& [pred_label, count] : m_confusion_matrix.at(class_label)) {
                support += count;
            }
        }
        m_support[class_label] = support;

        // Calcul des vrais positifs, faux positifs et faux négatifs
        uint32_t true_positives = m_confusion_matrix[class_label][class_label];
        
        uint32_t false_positives = 0;
        for (const auto& [true_label, predictions] : m_confusion_matrix) {
            if (true_label != class_label && predictions.count(class_label)) {
                false_positives += predictions.at(class_label);
            }
        }

        uint32_t false_negatives = support - true_positives;

        // Calcul de la précision
        uint32_t total_predicted = true_positives + false_positives;
        m_precision[class_label] = total_predicted > 0 ? 
            static_cast<double>(true_positives) / total_predicted : 0.0;
        
        // Calcul du rappel (recall)
        uint32_t total_actual = true_positives + false_negatives;
        m_recall[class_label] = total_actual > 0 ? 
            static_cast<double>(true_positives) / total_actual : 0.0;
        
        // Calcul du F1-Score
        double precision = m_precision[class_label];
        double recall = m_recall[class_label];
        m_f1_score[class_label] = (precision + recall > 0.0) ? 
            2.0 * (precision * recall) / (precision + recall) : 0.0;
    }

    m_metric_computed = true;
}

void ClassificationReport::PrintReport() const {
    if (!m_metric_computed) {
        std::cout << "Error: Metrics not computed. Call ComputeMetrics() first." << std::endl;
        return;
    }
    
    // ===== EN-TÊTE =====
    std::cout << "\n╔════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║        CLASSIFICATION REPORT SUMMARY               ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════╝\n" << std::endl;
    
    // ===== INFORMATIONS GÉNÉRALES =====
    std::cout << "Total samples:    " << m_predictions.size() << std::endl;
    std::cout << "Number of classes: " << m_classes.size() << std::endl;
    std::cout << "Overall Accuracy:  " << std::fixed << std::setprecision(2) 
              << (m_accuracy * 100.0) << "%\n" << std::endl;
    
    // ===== MÉTRIQUES PAR CLASSE =====
    std::cout << "┌───────────────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│                   Per-Class Metrics                           │" << std::endl;
    std::cout << "├────────┬───────────┬───────────┬───────────┬──────────────────┤" << std::endl;
    std::cout << "│ Class  │ Support   │ Precision │  Recall   │    F1-Score      │" << std::endl;
    std::cout << "├────────┼───────────┼───────────┼───────────┼──────────────────┤" << std::endl;
    
    for (uint32_t class_label : m_classes) {
        double precision = m_precision.count(class_label) ? m_precision.at(class_label) : 0.0;
        double recall = m_recall.count(class_label) ? m_recall.at(class_label) : 0.0;
        double f1_score = m_f1_score.count(class_label) ? m_f1_score.at(class_label) : 0.0;
        uint32_t support = m_support.count(class_label) ? m_support.at(class_label) : 0;

        std::cout << "│ " << std::setw(6) << class_label << " │ "
                  << std::setw(9) << support << " │ "
                  << std::fixed << std::setprecision(2)
                  << std::setw(8) << (precision * 100.0) << "% │ "
                  << std::setw(8) << (recall * 100.0) << "% │ "
                  << std::setw(15) << (f1_score * 100.0) << "% │" << std::endl;
    }
    
    std::cout << "└────────┴───────────┴───────────┴───────────┴──────────────────┘\n" << std::endl;
    
    // ===== MATRICE DE CONFUSION =====
    std::cout << "┌──────────────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│                    Confusion Matrix                          │" << std::endl;
    std::cout << "└──────────────────────────────────────────────────────────────┘\n" << std::endl;
    
    // Header
    std::cout << "         ";
    for (uint32_t pred_label : m_classes) {
        std::cout << " Pred" << std::setw(4) << pred_label;
    }
    std::cout << std::endl;
    
    std::cout << "         ";
    for (size_t i = 0; i < m_classes.size(); ++i) {
        std::cout << " ────────";
    }
    std::cout << std::endl;
    
    // Rows
    for (uint32_t true_label : m_classes) {
        std::cout << "True" << std::setw(4) << true_label << " │";
        for (uint32_t pred_label : m_classes) {
            uint32_t count = 0;
            if (m_confusion_matrix.count(true_label) && m_confusion_matrix.at(true_label).count(pred_label)) {
                count = m_confusion_matrix.at(true_label).at(pred_label);
            }
            std::cout << std::setw(8) << count << " ";
        }
        std::cout << std::endl;
    }
    
    std::cout << "\n════════════════════════════════════════════════════════════════\n" << std::endl;
}