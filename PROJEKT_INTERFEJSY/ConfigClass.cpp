#pragma warning(disable: 4996)
#include "ConfigClass.h"
#include "GUIMyFrame.h"
#include <cmath>
#include <algorithm>

ConfigClass::ConfigClass(GUIMyFrame* window): MainWindow(window),zoomX(1.0), zoomY(1.0), showErrorBars(true), backgroundColor(*wxWHITE), infoBoxX(70), infoBoxY(50), currentDatasetIndex(0)
{
    datasets.emplace_back("Seria 1", *wxBLUE);
}

void ConfigClass::AddDataSet(const std::string& name) {
    wxColour colors[] = { *wxBLUE, *wxRED, *wxGREEN, wxColour(200, 0, 200), wxColour(255, 165, 0) };
    int colour_index = datasets.size() % 5;
    datasets.emplace_back(name, colors[colour_index]);
    currentDatasetIndex = datasets.size() - 1;
}

void ConfigClass::RemoveCurrentDataSet(){
    if (datasets.size() <= 1) return;
    datasets.erase(datasets.begin() + currentDatasetIndex);
    if (currentDatasetIndex >= datasets.size())
        currentDatasetIndex = datasets.size() - 1;
}

void ConfigClass::SelectDataSet(int index){
    if (index >= 0 && index < datasets.size())
        currentDatasetIndex = index;
}

DataSet& ConfigClass::GetCurrentDataSet(){
    return datasets[currentDatasetIndex];
}

void ConfigClass::SetData(const std::vector<DataPoint>& data_points){
    GetCurrentDataSet().points = data_points;
    RecomputeRegression(currentDatasetIndex);
}

void ConfigClass::AppendPoint(const DataPoint& p){
    GetCurrentDataSet().points.push_back(p);
    RecomputeRegression(currentDatasetIndex);
}

void ConfigClass::SetCurrentRegType(RegressionType type){
    GetCurrentDataSet().regType = type;
    RecomputeRegression(currentDatasetIndex);
}

RegressionType ConfigClass::GetCurrentRegType() const{
    if (datasets.empty()) return RegressionType::Linear;
    return datasets[currentDatasetIndex].regType;
}

void ConfigClass::SetCurrentColor(const wxColour& color){
    GetCurrentDataSet().color = color;
}

wxColour ConfigClass::GetCurrentColor() const{
    if (datasets.empty()) return *wxBLACK;
    return datasets[currentDatasetIndex].color;
}


void ConfigClass::SetCurrentPointSize(int s){
    GetCurrentDataSet().size = s;
}

int ConfigClass::GetCurrentPointSize() const{
    if (datasets.empty()) return 5;
    return datasets[currentDatasetIndex].size;
}

void ConfigClass::SetCurrentPointShape(PointShape shape){
    GetCurrentDataSet().shape = shape;
}

PointShape ConfigClass::GetCurrentPointShape() const{
    if (datasets.empty()) return PointShape::Circle;
    return datasets[currentDatasetIndex].shape;
}


void ConfigClass::RecomputeAll(){
    for (size_t i = 0; i < datasets.size(); i++)
        RecomputeRegression(i);
}

void ConfigClass::RecomputeRegression(int datasetIndex){
    if (datasetIndex < 0 || datasetIndex >= datasets.size()) return;

    DataSet& ds = datasets[datasetIndex];
    ds.regResult = RegressionResult();

    const auto& data_points = ds.points;
    size_t quantity = data_points.size();
    if (quantity < 2) {
        ds.regResult.valid = false;
        return;
    }

    double S = static_cast<double>(quantity);

    //sumy statystyczne
    double Sx = 0.0;
    double Sy = 0.0;
    double Sxx = 0.0;
    double Syy = 0.0;
    double Sxy = 0.0;

    if (ds.regType == RegressionType::Exponential){
        for (const auto& p : data_points) {
            if (p.y <= 1e-9) { 
                ds.regResult.valid = false;
                return; 
            }
        }
    }

    //sumowanie
    for (const auto& p : data_points){
        double x = p.x;
        double y = p.y;
        if (ds.regType == RegressionType::Exponential) y = std::log(y);

        Sx += x;
        Sy += y;
        Sxx += x * x; 
        Syy += y * y; 
        Sxy += x * y;
    }

    //wyznacznik glowny ukladu
    double denominator = S * Sxx - Sx * Sx;
    if (ds.regType == RegressionType::Linear){
        if (std::fabs(denominator) < 1e-12) {
            ds.regResult.valid = false;
            return;
        }

        double a = (S * Sxy - Sx * Sy) / denominator;
        double b = (Sxx * Sy - Sx * Sxy) / denominator;

        //jak bardzo punkty odstaja od prostej
        double ss_res = 0.0;
        for (const auto& p : data_points) {
            double fit = a * p.x + b;
            //suma kwadratow reszt
            ss_res += (p.y - fit) * (p.y - fit);
        }
        if (quantity > 2) {
            double sigma2 = ss_res / (quantity - 2);
            //niepewnosci
            ds.regResult.sa = std::sqrt(sigma2 * S / denominator);
            ds.regResult.sb = std::sqrt(sigma2 * Sxx / denominator);
        }
        //wspolczynnik determinacji
        double num = (S * Sxy - Sx * Sy);
        double denominatorR = (S * Sxx - Sx * Sx) * (S * Syy - Sy * Sy);
        ds.regResult.r2 = (denominatorR > 0.0) ? (num * num) / denominatorR : 0.0;
        ds.regResult.a = a;
        ds.regResult.b = b;
        ds.regResult.valid = true;
    }
    else if (ds.regType == RegressionType::Orthogonal){
        double s_xx = Sxx - (Sx * Sx) / S;
        double s_yy = Syy - (Sy * Sy) / S;
        double s_xy = Sxy - (Sx * Sy) / S;

        if (std::fabs(s_xy) < 1e-12) {
            ds.regResult.valid = false;
            return;
        }

        double term = s_yy - s_xx;
        double root = std::sqrt(term * term + 4 * s_xy * s_xy);
        double a = (term + root) / (2 * s_xy);
        double b = (Sy / S) - a * (Sx / S);

        ds.regResult.a = a;
        ds.regResult.b = b;
        ds.regResult.r2 = 0.0;
        ds.regResult.valid = true;
    }
    else if (ds.regType == RegressionType::Exponential){
        if (std::fabs(denominator) < 1e-12) {
            ds.regResult.valid = false;
            return;
        }
        double B = (S * Sxy - Sx * Sy) / denominator;
        double A = (Sxx * Sy - Sx * Sxy) / denominator;
        ds.regResult.a = std::exp(A);
        ds.regResult.b = B;
        double num = (S * Sxy - Sx * Sy);
        double denominatorR = (S * Sxx - Sx * Sx) * (S * Syy - Sy * Sy);
        ds.regResult.r2 = (denominatorR > 0.0) ? (num * num) / denominatorR : 0.0;
        ds.regResult.valid = true;
    }
}