#include <iostream>
#include <vector>
#include "Data.h"
int main (int argc, char* argv[]){
 
    try{

        const char* m_dir = RESOURCES_PATH "Digits/digits.svm"; 
        Data d;
        if(d.LoadFile(m_dir)){
            std::cout << "yes" << '\n';
        }
        else{
            std::cout << "no" << '\n';
        }
        d.GetData()[0]->PrintDebug();
        d.GetData()[d.GetNbSamples()-1]->PrintDebug();

    }

    catch(std::invalid_argument& e){
        std::cout << "invalid_argument: " << e.what() << '\n';
    }

    catch (const std::exception& e) {
        std::cout << "std::exception: " << e.what() << '\n';
    }

    

    
}