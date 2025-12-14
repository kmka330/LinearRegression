#ifndef __CHART_CLASS_H__
#define __CHART_CLASS_H__

#include <memory>
#include <wx/dc.h>

class ConfigClass;

// klasa odpowiedzialna za rysowanie wykresu
class ChartClass {
    private:
        std::shared_ptr<ConfigClass> cfg;

        //funckje pomocnicze do przeliczania wspolrzednych
        int WorldToScreenX(double x, double minX, double scaleX, int left);
        int WorldToScreenY(double y, double minY, double scaleY, int h, int bottom);

    public:
        explicit ChartClass(std::shared_ptr<ConfigClass> c);

        void Draw(wxDC* dc, int w, int h);
};

#endif
