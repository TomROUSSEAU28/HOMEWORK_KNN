#pragma once
#include <iostream>
#include <string>
#include <atomic>





class ProgressBar {
public:

    enum class Color : uint8_t{
    BLACK = 0,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
    };

    ProgressBar(int total, Color color = Color::WHITE ,int width = 50, char fill = '=', char empty = ' ') : 
    m_total(total), m_color(color), m_width(width), m_fill(fill), m_empty(empty), m_current(0) 
	{
		hideCursor();
	}

	~ProgressBar() {
		showCursor();
	}


    void increment();
    void incrementThreadSafe();
    void finish();

private:

    // Couleurs standard
    #define BLACKstr       "\033[30m"
    #define REDstr         "\033[31m"
    #define GREENstr       "\033[32m"
    #define YELLOWstr      "\033[33m"
    #define BLUEstr        "\033[34m"
    #define MAGENTAstr     "\033[35m"
    #define CYANstr        "\033[36m"
    #define WHITEstr       "\033[37m"

    const char* getColor(const Color color) const;
    void display(int current) const;

	// Gérer le curseur de façon cross-platform
	void hideCursor();
	void showCursor();

    int m_total;
    int m_width;
    char m_fill;
    char m_empty;
    std::atomic<int> m_current; // version atomique pour le thread safe
    Color m_color = Color::WHITE;


    bool m_cursorHidden;
};