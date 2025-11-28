#ifndef __CHART_CLASS_H__
#define __CHART_CLASS_H__

#include <memory>
#include <wx/dc.h>

class ConfigClass;

/**
 * Klasa odpowiedzialna za rysowanie:
 *  punktow pomiarowych
 *  slupkow bledow
 *  prostej/krzywej regresji
 *  osi skali
 *
 */
class ChartClass
{
private:
    std::shared_ptr<ConfigClass> cfg;

public:
    ChartClass(std::shared_ptr<ConfigClass> c);

    void Draw(wxDC* dc, int w, int h);
};

#endif // __CHART_CLASS_H__
