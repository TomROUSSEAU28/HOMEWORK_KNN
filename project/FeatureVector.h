#pragma once
#include "vector"
#include <cstdint>


/**
 * @brief Représente un vecteur de caractéristiques.
 * @details Stocke le vecteur ainsi que sa norme, et fournit des méthodes
 *          mathématiques (produit scalaire, norme, etc.) pour l'exploiter.
 */


 /**
 * @brief Workflow de préchargement des données :
 * 1. Construction : FeatureVector(raw_data)
 * 2. UpdateNormalizedVector(max, min)
 * 3. NormalizeL2()
 * 4. LockAfterNormalization()
 * 
 */

class FeatureVector {

private:

    std::vector<double> m_featureVec;               /**< Vecteur de caractéristiques */
    std::vector<double> m_normalized_featureVec;    /**< Vecteur de caractéristiques normalisé en Min Max */
    std::vector<double> m_normalized_featureVec_L2; /**< Vecteur de caractéristiques normalisé en Min Max et normalisé L2 */
    bool m_is_normalized = false;  /** Flag qui indique normalisation */
    
public:

    friend class Sample;
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
    const std::vector<double>& GetDataNormalizedL2() const {return m_normalized_featureVec_L2;}

	/* ===== GETTERS SETTERS ===== */
	std::vector<double>& GetData() { return m_featureVec; }
	std::vector<double>& GetDataNormalized() { return m_normalized_featureVec; }
	std::vector<double>& GetDataNormalizedL2() { return m_normalized_featureVec_L2; }
    

    /* ===== METHODES MEMBRES ===== */

    /**
     * @brief Affiche le vecteur de features et le vecteur normalisé
     */
    void PrintDebug()const noexcept;

     /**
     * @brief Met à jour le vecteur normalisé (normalisation min max)
     * 
     */
    void UpdateNormalizedVector(const std::vector<double>& max, const std::vector<double>& min);

    /**
     * @brief Applique la normalisation L2 sur le vecteur normalisé
     * 
     */
    void NormalizeL2();

    /**
     * @brief Calcule la norme du vecteur de features
     * @return La norme 
     */
    double Norme() const noexcept;


    /**
     * @brief Calcule la norme du vecteur normalisé min max
     * @return La norme 
     */
    double NormeNormalized() const noexcept;

    /**
    * @brief Calcule le produit scalaire entre vecteur de features normamisé min max et L2
    * @param otherVector : Le vecteur à droite de l'opération
    * @return Le résultat du produit scaire
    */
    double DotProductNormalizedL2(const FeatureVector& otherVector) const;

    /**
     * @brief Permet d'indiquer que les vecteurs normalisés ont était généré.
     */
    void LockAfterNormalization() { m_is_normalized = true; }
    
    /**
     * @brief Calcule la distance euclidean entre vecteur normalisé min max
     * 
     * @param other
     * @return double
     */
    double EuclideanDistanceNormalized(const FeatureVector& other) const;

    /**
     * @brief Calcule la distance manhattan entre vecteur normalisé min max 
     * 
     * @param other 
     * @return double 
     */
    double ManhattanDistanceNormalized(const FeatureVector& other) const;
    // Reprise des methodes STL pour manipuler m_featureVec
    /**
     * @brief Calcule le taille du vecteur 
     * @return La taille du vecteur
     */
    uint32_t size() const noexcept;


     /**
     * @brief Acces aux iterateurs sur le vecteur de features, version const car non modifiable
     * @return L'iterateur
     */
    using iterator = std::vector<double>::iterator;
    using const_iterator = std::vector<double>::const_iterator;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;

    /**
     * @brief Reserve de la mémoire dans le vecteur de features
     * @param val : nombre de cases à reserver
     */
    void reserve(uint32_t val);


   

    /* ===== SURCHARGE DES OPERATEURS ===== */

    /**
    * @brief Accède en lecture à la valeur du vecteur de caractéristiques (m_featureVec).
    * @param index Indice de la valeur à lire.
    * @return Référence constante vers la valeur à l'indice donné.
    */
    const double& operator[](uint32_t index) const noexcept;

  
		
};