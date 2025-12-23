#include <iostream>
#include <ostream>
#include "KnnImporter.h"

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif

#ifdef _WIN32
void configWindowsConsole() {
	// Switch Windows console to UTF-8 and enable ANSI/VT processing
	SetConsoleOutputCP(CP_UTF8);

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut != INVALID_HANDLE_VALUE) {
		DWORD dwMode = 0;
		if (GetConsoleMode(hOut, &dwMode)) {
			// Enable virtual terminal processing for better rendering of ANSI sequences
			SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		}
	}

	// Keep stdout as narrow UTF-8 bytes (do not switch to _O_U16TEXT unless you convert to wcout)
	_setmode(_fileno(stdout), _O_BINARY);
}
#endif


// TODO réaliser algo de PCA, avoir le choix via la class knn de réduire la dimention des vecteurs pour améliorer l'algorithme.

int main (int argc, char* argv[]){

    #ifdef _WIN32
        configWindowsConsole();
    #endif



    try{

        std::cout << "Nombre d'arguments passé " << argc << std::endl;
        uint32_t k = 3;
        if(argc == 2){
            k = std::stoi(argv[1]);
        }

        std::cout << "Le paramtre k est : " << k << std::endl;
       

        

        const char* m_dir_train = RESOURCES_PATH "Base de test-20251125/train.svm"; 
        const char* m_dir_test = RESOURCES_PATH "Base de test-20251125/test.svm"; 

      

			KnnManhattan kman(k);
            kman.SetWeightedVoting(true);
            kman.SetClassBalancing(false);
			kman.loadTrainData(m_dir_train, false);
			kman.loadTestData(m_dir_test, false);
			kman.Predict();
            std::cout << "[OPTIMISATION] AVEC K : " << k << "\n";
			kman.Report();

        
       

        const char* m_dir_train2 = RESOURCES_PATH "JeuxVideos_train/Split/train.ssvm"; 
        const char* m_dir_test2 = RESOURCES_PATH "JeuxVideos_train/Split/test.ssvm"; 

        
       
            KnnCosine kcos2(k);
            kcos2.SetWeightedVoting(true);
            kcos2.SetClassBalancing(false);
			kcos2.loadTrainData(m_dir_train2, true);
			kcos2.loadTestData(m_dir_test2, true);
			kcos2.Predict();
            std::cout << "[OPTIMISATION] AVEC K : " << k << "\n";
			kcos2.Report();
        
        

    }

    catch(std::invalid_argument& e){
        std::cout << "invalid_argument: " << e.what() << std::endl;
    }
    catch(std::ios_base::failure& e){
        std::cout << "std::ios_base::failure: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "std::exception: " << e.what() << std::endl;
    }

    return 0;

    
}

