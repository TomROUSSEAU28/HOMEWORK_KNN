#pragma once
#include "Sample.h"
#include <cstdint>
#include <memory.h>
#include <memory>
class Data{
private:
    uint32_t m_nbFeatures;                          /**< Nombre de caractéristiques par sample */
    uint32_t m_nbSamples;                           /**< Nombre de samples aux totals   */
    std::vector<std::unique_ptr<Sample>> m_data;    /**< Vecteur qui stocke les samples */
   
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



     /* ===== METHODES MEMBRES ===== */


    /**
     * @brief Charge les données pour crées des samples.
     * 
     */
    bool LoadFile(const char* dir);


    /**
     * @brief Affiche toutes les données 
     */
    void PrintDebug()const noexcept;
 

    /* ===== SURCHARGE DES OPERATEURS ===== */

    /**
    * @brief Accède en lecture à un sample.
    * @param index Indice du sample à lire.
    * @return Référence constante vers le sample à l'indice donné.
    */
    const Sample& operator[](uint32_t index) const noexcept;

};