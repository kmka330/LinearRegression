#ifndef __CONFIG_CLASS_H__
#define __CONFIG_CLASS_H__
#include <vector>
#include <string>
#include <wx/colour.h>

class GUIMyFrame;

//typy regresji
enum class RegressionType{
    Linear = 0,
    Orthogonal = 1,
    Exponential = 2
};

//ksztalt punktow
enum class PointShape{
    Circle = 0,
    Square = 1,
    Cross = 2
};


//pojedynczy punkt pomiarowy z niepewnoscia
struct DataPoint{
    double x;
    double y;
    double uncertaintyY;
};


//wynik regresji z niepewnosciami wspolczynnikow i wspolczynnikiem determiancji r2
struct RegressionResult{
    double a = 0.0;
    double b = 0.0;
    double sa = 0.0;
    double sb = 0.0;
    double r2 = 0.0;
    bool valid = false; //czy obliczenia sie powiodly
};

//seria danych 
struct DataSet{
    std::string name;
    std::vector<DataPoint> points;
    wxColour color;
    RegressionType regType;
    RegressionResult regResult;
    PointShape shape;
    int size;

    //domyslna seria
    DataSet(std::string n, wxColour c): name(n), color(c), regType(RegressionType::Linear),shape(PointShape::Circle), size(5) {}
};

class ConfigClass{
    private:
        GUIMyFrame* MainWindow;

        std::vector<DataSet> datasets;
        int currentDatasetIndex;
        double zoomX;
        double zoomY;
        bool showErrorBars;
        wxColour backgroundColor;

        //pozycja legendy
        int infoBoxX;
        int infoBoxY;

    public:
        explicit ConfigClass(GUIMyFrame* window);

        void AddDataSet(const std::string& name);
        void RemoveCurrentDataSet();
        void SelectDataSet(int index);
        int GetCurrentDataSetIndex() const { 
            return currentDatasetIndex; 
        }

        DataSet& GetCurrentDataSet();
        const std::vector<DataSet>& GetAllDataSets() const { 
            return datasets; 
        }

        //edycja wybranej serii danych
        void SetData(const std::vector<DataPoint>& pts);
        void AppendPoint(const DataPoint& p);
        void SetCurrentRegType(RegressionType t);
        RegressionType GetCurrentRegType() const;
        void SetCurrentColor(const wxColour& c);
        wxColour GetCurrentColor() const;
        void SetCurrentPointSize(int s);
        int GetCurrentPointSize() const;
        void SetCurrentPointShape(PointShape s);
        PointShape GetCurrentPointShape() const;
     
        //obliczenia regresji
        void RecomputeRegression(int datasetIndex);
        void RecomputeAll();

        //globalne ustawienia zoomu slupkow tla i legendy
        void SetZoomX(double z) { 
            zoomX = z; 
        }
        void SetZoomY(double z) {
            zoomY = z; 
        }
        double GetZoomX() const {
            return zoomX; 
        }
        double GetZoomY() const { 
            return zoomY; 
        }
        void SetShowErrorBars(bool b) { 
            showErrorBars = b; 
        }
        bool GetShowErrorBars() const {
            return showErrorBars;
        }
        void SetBackgroundColor(const wxColour& c) { 
            backgroundColor = c; 
        }
        wxColour GetBackgroundColor() const { 
            return backgroundColor; 
        }
        void SetInfoBoxPos(int x, int y) {
            infoBoxX = x; 
            infoBoxY = y; 
        }
        int  GetInfoBoxX() const {
            return infoBoxX; 
        }
        int  GetInfoBoxY() const { 
            return infoBoxY; 
        }
};

#endif 