#pragma once
#include "FeatureVector.h"
#include <cstdint>




/**
 * @brief Représente un échantillon de données.
 * @details Stocke le tag et les caracteristiques correspondantes
 */

class Sample {
private:
    uint32_t m_tag;             /**< tag qui correspond aux caractéristiques */
    FeatureVector m_features;   /**< Vecteur de caractéristiques */
public:

    /* ===== CONSTRUCTEUR DESTRUCTEUR ===== */
    /**
     * @brief Construit un Sample à partir d'un tag et d'un FeatureVector
     * @param tag : tag correspondant à l'échantillon
     * @param features : le vecteur de caracteristique du sample
     */
     
    Sample(const uint32_t tag, const FeatureVector& features);
    ~Sample() = default;

    /* ===== GETTERS SETTERS ===== */

    /**
     * @brief Récupère en lecture seule le tag
     * @param m_tag : tag de l'échantillon
     * @return le tag
     */
    const uint32_t GetTag() const {return m_tag;}


    /**
     * @brief Récupère en lecture le vecteur de caracteristique
     * @param m_features : le vecteur de caracteristique
     * @return le vecteur de caracteristique
     */
    const FeatureVector& GetFeatures() const {return m_features;}


    /* ===== METHODES MEMBRES ===== */

    /**
     * @brief Affiche le Sample de features et le tag
     */
    void PrintDebug()const noexcept;



     /* ===== SURCHARGE DES OPERATEURS ===== */

    /**
    * @brief Accède en lecture au vecteur de caractéristiques.
    * @param index Indice de la valeur à lire.
    * @return Référence constante vers la valeur à l'indice donné.
    */
    const double& operator[](uint32_t index) const noexcept;


};