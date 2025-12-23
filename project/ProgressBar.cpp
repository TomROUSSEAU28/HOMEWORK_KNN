#include "ProgressBar.h"
#include <iterator>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif


void ProgressBar::increment() {
    display(++m_current);
}

void ProgressBar::incrementThreadSafe(){
    int current = ++m_current; // atomique
    // Affiche seulement tous les 1% ou à la fin (évite surcharge console)
    int step = std::max(1, m_total / 100);
    if (current % step == 0 || current == m_total) {
        #pragma omp critical // véroulle pour single thread
        {
            display(current);
        }
    }
}

void ProgressBar::finish(){
    display(m_total);
    std::cout << WHITEstr << '\n';
}

void ProgressBar::display(int current) const{
    float progress = static_cast<float>(current) / m_total; // entre 0 et 1
    int filled = static_cast<int>(progress * m_width);      // entre 0 et width
    const char* color = getColor(m_color);

    std::cout << "\r[";                                     // retour a la meme ligne
    for (int i = 0; i < m_width; ++i) {
        std::cout << color << (i < filled ? m_fill : m_empty);       //affiche une bar de progression
    }
    std::cout << "] " << static_cast<int>(progress * 100) << "% (" 
                << current << "/" << m_total << ")   " << std::flush;

}

const char* ProgressBar::getColor(const Color color) const{
    switch (color) {
        case Color::BLACK: return BLACKstr;
        case Color::RED: return REDstr;
        case Color::GREEN: return GREENstr;
        case Color::YELLOW: return YELLOWstr;
        case Color::BLUE: return BLUEstr;
        case Color::MAGENTA: return MAGENTAstr;
        case Color::CYAN: return CYANstr;
        case Color::WHITE: return WHITEstr;
        default: return WHITEstr;
    };
}



void ProgressBar::hideCursor() {
#ifdef _WIN32
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut != INVALID_HANDLE_VALUE) {
		CONSOLE_CURSOR_INFO cursorInfo;
		if (GetConsoleCursorInfo(hOut, &cursorInfo)) {
			cursorInfo.bVisible = FALSE;
			SetConsoleCursorInfo(hOut, &cursorInfo);
		}
	}
#else
	// ANSI : hide cursor
	std::cout << "\x1b[?25l" << std::flush;
#endif
	m_cursorHidden = true;
}

void ProgressBar::showCursor() {
	if (!m_cursorHidden) return;
#ifdef _WIN32
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut != INVALID_HANDLE_VALUE) {
		CONSOLE_CURSOR_INFO cursorInfo;
		if (GetConsoleCursorInfo(hOut, &cursorInfo)) {
			cursorInfo.bVisible = TRUE;
			SetConsoleCursorInfo(hOut, &cursorInfo);
		}
	}
#else
	// ANSI : show cursor
	std::cout << "\x1b[?25h" << std::flush;
#endif
	m_cursorHidden = false;
}