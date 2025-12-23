#pragma once
#include "Sample.h"
#include <cstdint>
#include <memory.h>
#include <memory>


/**
 * @brief Représente les données d'entrainements
 * @details Stocke les données pour le knn
 */
class Data{
private:
    uint32_t m_nbFeatures;                          /**< Nombre de caractéristiques par sample */
    uint32_t m_nbSamples;                           /**< Nombre de samples aux totals   */
    std::vector<std::unique_ptr<Sample>> m_data;    /**< Vecteur qui stocke les samples */

    std::vector<double> m_max;  /**< Vecteur qui stocke les max de chaque colonnes pour les vecteurs de caracteristiques */
    std::vector<double> m_min;  /**< Vecteur qui stocke les min de chaque colonnes pour les vecteurs de caracteristiques */

    




   
public:
    Data();
    ~Data() = default;


    /* ===== GETTERS SETTERS ===== */
    /**
     * @brief Récupère en lecture seule le nombre de features par samples
     * @param m_nbFeatures : le nombre de features par samples
     * @return le nombre de features par samples
     */
    const uint32_t GetNbFeatures() const {return m_nbFeatures;}

    /* ===== GETTERS SETTERS ===== */
    /**
     * @brief Récupère en lecture seule le nombre de samples aux totals
     * @param m_nbFeatures : le nombre de samples aux totals
     * @return le nombre de samples aux totals
     */
    const uint32_t GetNbSamples() const {return m_nbSamples;}

    /* ===== GETTERS SETTERS ===== */
    /**
     * @brief Récupère en lecture seule le nombre de samples aux totals
     * @param m_nbFeatures : le nombre de samples aux totals
     * @return le nombre de samples aux totals
     */
    const std::vector<std::unique_ptr<Sample>>& GetData() const {return m_data;}


    const std::vector<double>& GetMax() const {return m_max;}
    const std::vector<double>& GetMin() const {return m_min;}

 

     /* ===== METHODES MEMBRES ===== */


    /**
     * @brief Charge les données pour crées des samples.
     * @details Retourne une exeption en cas d'echec
     */
    void LoadFile(const char* dir);


    /**
     * @brief Charge les données pour crées des samples format creux.
     * @details Retourne une exeption en cas d'echec
     */
    void LoadFileHollow(const char* dir);


    /**
     * @brief Update m_max et m_min
     * 
     */
    void UpdateMinMax();


    /**
     * @brief Update tous les Features vecteur
     * 
     */
    void UpdateNormalizedVector();

    /**
     * @brief Update tous les Features vecteur à partir de max et min d'un autre
     * 
     */
    void updateNormalizedVectorWith(const std::vector<double>& max, const std::vector<double>& min);


    /**
     * @brief Affiche toutes les données 
     */
    void PrintDebug()const noexcept;


    /**
     * @brief Réduit la dimention des données
     */
    const std::vector<bool> ReduceDimension(const double threshold = 0.1f);

    /**
    * @brief Réduit la dimention des données a partir d'un autre jeux de donnée, supprime les donnée non gardé par l'autre jeu de données
    */
    void ReduceDimensionWith(const std::vector<bool>& dataKept, const std::vector<double>& max, const std::vector<double>& min);

	

    /* ===== SURCHARGE DES OPERATEURS ===== */

    /**
    * @brief Accède en lecture à un sample.
    * @param index Indice du sample à lire.
    * @return Référence constante vers le sample à l'indice donné.
    */
    const Sample& operator[](uint32_t index) const noexcept;

    private:
	/**
	* @brief Supprime une variable du jeu de donnée
	*/
	void popVar(const uint32_t index);

};