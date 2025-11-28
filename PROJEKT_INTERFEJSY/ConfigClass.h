#ifndef __CONFIG_CLASS_H__
#define __CONFIG_CLASS_H__

//forward declaration
class GUIMyFrame;

/**
 * Klasa konfiguracji wykresu / regresji
 * Do trzymania:
 * zakresu osi
 * kolorów tla i punktow
 * rozmiaru i typu punktow
 * typu regresji
 * informacji o zestawach danych
 */
class ConfigClass
{
private:
    GUIMyFrame* MainWindow; // wskaznik do g³ownego okna

public:
    ConfigClass(GUIMyFrame* window);

    //ddoac gettery/setter load save itp
};

#endif // __CONFIG_CLASS_H__
