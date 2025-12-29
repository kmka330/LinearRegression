#pragma warning(disable: 4996)
#include "GUIMyFrame.h"
#include "ConfigClass.h"
#include "ChartClass.h"
#include <wx/filedlg.h>
#include <wx/textfile.h>
#include <wx/colordlg.h>
#include <wx/msgdlg.h>
#include <wx/clipbrd.h>
#include <wx/dataobj.h>
#include <wx/dcmemory.h>
#include <wx/print.h>
#include <wx/printdlg.h>
#include <wx/textdlg.h> 
#include <sstream>
#include <vector>

//klasa do drukowania
class PlotPrintout : public wxPrintout {
    std::shared_ptr<ConfigClass> cfg;
public:
    PlotPrintout(const wxString& title, std::shared_ptr<ConfigClass> cfg) : wxPrintout(title), cfg(std::move(cfg)) {}
    bool OnPrintPage(int page) override {
        //druk jednej strony
        if (page != 1) return false;
        wxDC* dc = GetDC(); 
        if (!dc) return false;
        int w, h; 
        dc->GetSize(&w, &h); 
        //skalowanie
        FitThisSizeToPage(wxSize(w, h));
        ChartClass chart(cfg); 
        chart.Draw(dc, w, h);
        return true;
    }
    bool HasPage(int page) override { 
        return page == 1; 
    }
    void GetPageInfo(int* minPage, int* maxPage, int* PageFrom, int* PageTo) override { 
        *minPage = 1; 
        *maxPage = 1; 
        *PageFrom = 1;
        *PageTo = 1; 
    }
};

GUIMyFrame::GUIMyFrame(wxWindow* parent) : MyFrame(parent) {
    SetTitle("Projekt Linear Regression");
    CreateStatusBar();
    cfg = std::make_shared<ConfigClass>(this);
    cfg->SetZoomX(slider_zoox->GetValue() / 50.0);
    cfg->SetZoomY(slider_zooy->GetValue() / 50.0);
    cfg->SetShowErrorBars(check_show_errorbars->GetValue());
    RefreshDatasetList();
}

void GUIMyFrame::MainFormClose(wxCloseEvent& event) { 
    Destroy();
}

void GUIMyFrame::WxPanel_Repaint(wxUpdateUIEvent& event) {
    Repaint();
}


void GUIMyFrame::Repaint() {
    wxClientDC dc1(WxPanel); 
    wxBufferedDC dc(&dc1);
    int w, h; 
    WxPanel->GetSize(&w, &h);
    ChartClass chart(cfg); 
    chart.Draw(&dc, w, h);
}

void GUIMyFrame::RefreshDatasetList() {
    list_datasets->Clear();
    const auto& sets = cfg->GetAllDataSets();
    for (const auto& ds : sets) list_datasets->Append(ds.name);
    list_datasets->SetSelection(cfg->GetCurrentDataSetIndex());
    UpdateControlsForCurrentDataset();
}

void GUIMyFrame::UpdateControlsForCurrentDataset() {
    RegressionType rt = cfg->GetCurrentRegType();
    choice_reg_type->SetSelection((int)rt);
    int size = cfg->GetCurrentPointSize();
    slider_point_size->SetValue(size);
    PointShape shape = cfg->GetCurrentPointShape();
    int shape_index = 0;
    if (shape == PointShape::Square) shape_index = 1;
    if (shape == PointShape::Cross) shape_index = 2;
    choice_point_style->SetSelection(shape_index);
}

void GUIMyFrame::button_load_file_click(wxCommandEvent& event) {
    wxFileDialog openDlg(this, "Wybierz plik z danymi", "", "", "Pliki tekstowe (*.txt;*.dat;*.csv)|*.txt;*.dat;*.csv|Wszystkie pliki|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if (openDlg.ShowModal() != wxID_OK) return;

    wxTextFile file; 

    if (!file.Open(openDlg.GetPath())) {
        wxMessageBox("Blad pliku", "Blad", wxOK | wxICON_ERROR); 
        return;
    }

    std::vector<DataPoint> data_points;
    for (size_t i = 0; i < file.GetLineCount(); i++) {
        wxString line = file.GetLine(i); 
        line.Trim(true).Trim(false);

        if (line.IsEmpty() || line[0] == '#' || line[0] == ';') continue;

        line.Replace(",", ".");
        double x = 0.0;
        double y = 0.0;
        double e = 0.0;

        std::istringstream iss(line.ToStdString());

        if (!(iss >> x >> y)) continue;
        if (!(iss >> e)) e = 0.0;


        data_points.push_back({ x, y, e });
    }
    file.Close();

    if (data_points.size() < 2) { wxMessageBox("Za malo punktow.", "Uwaga", wxOK | wxICON_WARNING); return; }
    cfg->SetData(data_points);
    SetStatusText(wxString::Format("Pomyslnie wpisano dane z pliku."));
    Repaint();
}

void GUIMyFrame::button_add_keyboard_click(wxCommandEvent& event) {
    wxTextEntryDialog dlg(this, "Format: x y uncertaintyY\nnp. 1.5 2.3 0.1", "Dodaj punkt");
    
    if (dlg.ShowModal() != wxID_OK) return;

    wxString txt = dlg.GetValue(); 
    txt.Replace(",", ".");

    double x = 0.0;
    double y = 0.0;
    double e = 0.0;

    std::istringstream iss(txt.ToStdString());
    if (!(iss >> x >> y)) { 
        wxMessageBox("Bledny format.", "Blad");
        return; 
    }
    if (!(iss >> e)) e = 0.0;

    cfg->AppendPoint({ x, y, e }); 
    Repaint();
}

void GUIMyFrame::button_auto_scale_click(wxCommandEvent& event) {
    cfg->SetZoomX(1.0); 
    cfg->SetZoomY(1.0);
    slider_zoox->SetValue(50); 
    slider_zooy->SetValue(50);
    Repaint();
}

void GUIMyFrame::slider_zoox_scroll(wxScrollEvent& event) { 
    cfg->SetZoomX(slider_zoox->GetValue() / 50.0);
    Repaint(); 
}

void GUIMyFrame::slider_zooy_scroll(wxScrollEvent& event) {
    cfg->SetZoomY(slider_zooy->GetValue() / 50.0);
    Repaint();
}

void GUIMyFrame::slider_point_size_scroll(wxScrollEvent& event) {
    cfg->SetCurrentPointSize(slider_point_size->GetValue());
    Repaint();
}

void GUIMyFrame::choice_point_style_change(wxCommandEvent& event) {
    int i = choice_point_style->GetSelection();
    PointShape shape;

    switch (i){
        case 0:
            shape = PointShape::Circle;
            break;
        case 1:
            shape = PointShape::Square;
            break;
        case 2:
            shape = PointShape::Cross;
            break;
        default:
            shape = PointShape::Circle;
            break;
    }
    cfg->SetCurrentPointShape(shape);
    Repaint();
}

void GUIMyFrame::check_show_errorbars_click(wxCommandEvent& event) {
    cfg->SetShowErrorBars(check_show_errorbars->GetValue());
    Repaint();
}

void GUIMyFrame::choice_reg_type_change(wxCommandEvent& event) {
    RegressionType currentType = cfg->GetCurrentRegType();
    int i = choice_reg_type->GetSelection();
    RegressionType rt = static_cast<RegressionType>(i);
    if (rt == RegressionType::Exponential) {
        const auto& ds = cfg->GetCurrentDataSet();
        for (const auto& p : ds.points) {
            if (p.y <= 0) {
                wxMessageBox("Regresja wykladnicza wymaga y > 0!!!", "Uwaga", wxOK | wxICON_WARNING);
                choice_reg_type->SetSelection((int)currentType);
                return;
            }
        }
    }

    cfg->SetCurrentRegType(rt);
    Repaint();
}

void GUIMyFrame::list_datasets_select(wxCommandEvent& event) {
    int selection = list_datasets->GetSelection();
    if (selection != wxNOT_FOUND) {
        cfg->SelectDataSet(selection);
        UpdateControlsForCurrentDataset();
    }
}

void GUIMyFrame::button_add_dataset_click(wxCommandEvent& event) {
    wxTextEntryDialog dlg(this, "Podaj nazwe nowej serii:", "Nowa seria");
    if (dlg.ShowModal() == wxID_OK) {
        cfg->AddDataSet(dlg.GetValue().ToStdString());

        RefreshDatasetList(); 
        Repaint();
    }
}

void GUIMyFrame::button_remove_dataset_click(wxCommandEvent& event) {
    if (cfg->GetAllDataSets().size() <= 1) {
        wxMessageBox("Musi zostac conajmniej jeden zestaw!!!", "Informacja", wxOK | wxICON_INFORMATION);
        return;
    }
    cfg->RemoveCurrentDataSet();
    RefreshDatasetList();
    Repaint();
}

void GUIMyFrame::button_point_color_click(wxCommandEvent& event) {
    wxColourData data; 
    data.SetColour(cfg->GetCurrentColor());

    wxColourDialog colour_dialog(this, &data);
    if (colour_dialog.ShowModal() == wxID_OK) { 
        cfg->SetCurrentColor(colour_dialog.GetColourData().GetColour());
        Repaint(); 
    }
}

void GUIMyFrame::button_bg_color_click(wxCommandEvent& event) {
    wxColourData data;
    data.SetColour(cfg->GetBackgroundColor());

    wxColourDialog colour_dialog(this, &data);
    if (colour_dialog.ShowModal() == wxID_OK) {
        cfg->SetBackgroundColor(colour_dialog.GetColourData().GetColour()); 
        Repaint(); 
    }
}

void GUIMyFrame::button_save_plot_click(wxCommandEvent& event) {
    wxFileDialog save_dialog(this, "Zapisz", "", "plot.png", "PNG|*.png|BMP|*.bmp", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (save_dialog.ShowModal() != wxID_OK) return;

    int w, h; 
    WxPanel->GetSize(&w, &h);
    wxBitmap bmp(w, h); 
    wxMemoryDC memDC(bmp);

    ChartClass chart(cfg);
    chart.Draw(&memDC, w, h);
    memDC.SelectObject(wxNullBitmap);

    bmp.SaveFile(save_dialog.GetPath(), wxBITMAP_TYPE_PNG);
}

void GUIMyFrame::button_copy_clipboard_click(wxCommandEvent& event) {
    int w, h;
    WxPanel->GetSize(&w, &h);
    wxBitmap bmp(w, h); 
    wxMemoryDC memDC(bmp);

    ChartClass chart(cfg); 
    chart.Draw(&memDC, w, h);
    memDC.SelectObject(wxNullBitmap);

    if (wxTheClipboard->Open()) {
        wxTheClipboard->SetData(new wxBitmapDataObject(bmp));
        wxTheClipboard->Close(); 
        SetStatusText("Skopiowano do schowka."); 
    }
}

void GUIMyFrame::button_print_click(wxCommandEvent& event) {
    wxPrinter printer; 
    PlotPrintout printout("Wykres", cfg);

    if (!printer.Print(this, &printout, true)) { 
        if (printer.GetLastError() == wxPRINTER_ERROR) wxMessageBox("Blad drukowania.", "Blad");
    }
}

void GUIMyFrame::panel_left_down(wxMouseEvent& event) {
    int box_x = cfg->GetInfoBoxX(); 
    int box_y = cfg->GetInfoBoxY();
    wxRect rect(box_x, box_y, 240, 100);

    if (rect.Contains(event.GetPosition())) {
        dragInfoBox = true;
        dragStartMouse = event.GetPosition();
        dragStartBox = wxPoint(box_x, box_y); 
        WxPanel->CaptureMouse(); 
    }
}

void GUIMyFrame::panel_mouse_move(wxMouseEvent& event) {
    if (dragInfoBox && event.Dragging() && event.LeftIsDown()) {
        wxPoint p = event.GetPosition();
        cfg->SetInfoBoxPos(dragStartBox.x + p.x - dragStartMouse.x, dragStartBox.y + p.y - dragStartMouse.y);
        Repaint();
    }
}

void GUIMyFrame::panel_left_up(wxMouseEvent& event) {
    if (dragInfoBox) { 
        dragInfoBox = false;
        if (WxPanel->HasCapture()) WxPanel->ReleaseMouse();
    } 
}