#pragma once
#include "vector"
#include <cstdint>


/**
 * @brief Représente un vecteur de caractéristiques.
 * @details Stocke le vecteur ainsi que sa norme, et fournit des méthodes
 *          mathématiques (produit scalaire, norme, etc.) pour l'exploiter.
 */


class FeatureVector {

private:

    std::vector<double> m_featureVec;            /**< Vecteur de caractéristiques */
    std::vector<double> m_normalized_featureVec; /**< Norme du vecteur de caractéristiques */
    
public:


    /* ===== CONSTRUCTEUR DESTRUCTEUR ===== */
    FeatureVector();

     /**
     * @brief Construit un FeatureVector à partir d'un std::vector
     * @param featureVec : le vecteur servant de base à la construction
     */
    FeatureVector(const std::vector<double>& featureVec);


    ~FeatureVector() = default;

    /* ===== GETTERS SETTERS ===== */
    const std::vector<double>& GetData() const {return m_featureVec;}
    const std::vector<double>& GetDataNormalized() const {return m_normalized_featureVec;}

    /* ===== METHODES MEMBRES ===== */

    /**
     * @brief Affiche le vecteur de features et le vecteur normalisé
     */
    void PrintDebug()const noexcept;

     /**
     * @brief Met à jour le vecteur normalisé
     */
    void UpdateNormalizedVector();

    /**
     * @brief Calcule la norme du vecteur de features
     * @return La norme 
     */
    double Norme() const noexcept;

    /**
     * @brief Calcule le produit scalaire entre vecteur de features
     * @param otherVector : Le vecteur à droite de l'opération
     * @return Le résultat du produit scaire
     */
    double DotProduct(const FeatureVector& otherVector) const;

    /**
    * @brief Calcule le produit scalaire entre vecteur de features normamisé
    * @param otherVector : Le vecteur à droite de l'opération
    * @return Le résultat du produit scaire
    */
    double DotProductNormalized(const FeatureVector& otherVector) const;
    

    // Reprise des methodes STL pour manipuler m_featureVec
    /**
     * @brief Calcule le taille du vecteur 
     * @return La taille du vecteur
     */
    uint32_t size() const noexcept;


     /**
     * @brief Acces aux iterateurs sur le vecteur de features
     * @return L'iterateur
     */
    using iterator = std::vector<double>::iterator;
    using const_iterator = std::vector<double>::const_iterator;
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;

    /**
     * @brief Reserve de la mémoire dans le vecteur de features
     * @param val : nombre de cases à reserver
     */
    void reserve(uint32_t val);

    /**
     * @brief Permet d'ajouter une valeur à la fin du vecteur
     * @param val : valeur à ajouter
     */
    void push_back(const double val);
   

    /* ===== SURCHARGE DES OPERATEURS ===== */

    /**
    * @brief Accède en lecture à la valeur du vecteur de caractéristiques (m_featureVec).
    * @param index Indice de la valeur à lire.
    * @return Référence constante vers la valeur à l'indice donné.
    */
    const double& operator[](uint32_t index) const noexcept;

    /**
    * @brief Accède en lecture/écriture à la valeur du vecteur de caractéristiques (m_featureVec).
    * @param index Indice de la composante à modifier.
    * @return Référence vers la valeur à l'indice donné.
    */
    double& operator[](uint32_t index) noexcept;



    /**
     * @brief 
     * 
     */
    
    
};