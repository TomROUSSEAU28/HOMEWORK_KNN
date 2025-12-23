#pragma once
#include "Data.h"
#include "Sample.h"
#include "ClassificationReport.h"


/**
 * @brief Représente une classe abstraite d'un knn
 * @details 
 */
class Knn {
    
private:
    void computeClassWeights();

protected:
    uint32_t m_k; /**< paramètre k */
    Data m_trainData; /**< Données d'entrainements */
    Data m_testData;  /**< Données de test */
    bool m_weighted_voting = false; /**< Permet d'acitivé le vote pondéré */

	std::map<uint32_t, double> m_class_weights; /** les poids pour chaque classe */
	bool m_use_class_balancing = false; /**< Permet d'acitivé le vote pondéré par class */

    ClassificationReport report; /**< Résultats de classification */


    /**
     * @brief Predit 
     * @param sampleTest sample de test
     * @return le tag prédit
     */
    uint32_t predictSingle(const Sample& sampleTest) const ;

    /**
      * @brief Donne la distance entre un sample test et un sample train
      * 
      * @param sampleTest 
      * @param sampleTrain 
      * @return double 
    */
    virtual double getDistanceKnn(const Sample& sampleTest, const Sample& sampleTrain) const = 0;

    
public:
    Knn(uint32_t k);
    ~Knn() = default;

    /**
    * @brief Setter pour m_weighted_voting, active ou non le vote pondéré
    */
    void SetWeightedVoting(bool weighted) { m_weighted_voting = weighted; }

	/**
	* @brief Setter pour m_use_class_balancing, active ou non le vote pondéré par class
	*/
    void SetClassBalancing(bool enable) { m_use_class_balancing = enable; }


   

    /**
     * @brief charge les données d'entrainement
     */
    void loadTrainData(const char* trainDataPath, bool isHollowData = 0);

    /**
    * @brief charge les données de test
    */
    void loadTestData(const char* testDataPath, bool isHollowData = 0);

    /**
    * @brief Réduit la dimentions des samples 
    */
    void ReduceDimension(const double threshold = 0.1f);


	/**
	* @brief Predit en test set complet
	*/
    void Predict();

	/**
	* @brief Calcule les metrics et affiche le rapport
	*/
    void Report();
    

};