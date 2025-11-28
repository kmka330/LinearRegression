#pragma warning(disable: 4996) 
#include "ChartClass.h"
#include "ConfigClass.h"
#include <wx/brush.h>
#include <wx/pen.h>

ChartClass::ChartClass(std::shared_ptr<ConfigClass> c): cfg(std::move(c)){
}

void ChartClass::Draw(wxDC* dc, int w, int h){
    //t³o
    dc->SetBackground(wxBrush(wxColour(255, 255, 255)));
    dc->Clear();

}
