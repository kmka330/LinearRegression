#include "GUIMyFrame.h"
#include "ConfigClass.h"
#include "ChartClass.h"

GUIMyFrame::GUIMyFrame(wxWindow* parent) : MyFrame(parent){
    SetTitle("Linear Regression");
    CreateStatusBar();
    cfg = std::make_shared<ConfigClass>(this);
}

void GUIMyFrame::MainFormClose(wxCloseEvent& event){
    Destroy();
}

void GUIMyFrame::WxPanel_Repaint(wxUpdateUIEvent& event)
{
    Repaint();
}

void GUIMyFrame::Repaint(){
    wxClientDC dc1(WxPanel);
    wxBufferedDC dc(&dc1);

    int w, h;
    WxPanel->GetSize(&w, &h);

    ChartClass chart(cfg);
    chart.Draw(&dc, w, h);
}

//handlery przycisko

void GUIMyFrame::button_load_file_click(wxCommandEvent& event){
    SetStatusText("Klikniêto: Wczytaj dane z pliku.");
}

void GUIMyFrame::button_add_keyboard_click(wxCommandEvent& event){
    SetStatusText("Klikniêto: Dodaj dane z klawiatury.");
}

void GUIMyFrame::button_auto_scale_click(wxCommandEvent& event){
    SetStatusText("Klikniêto: Auto skala.");
}

void GUIMyFrame::slider_zoox_scroll(wxScrollEvent& event){
    SetStatusText("Zmiana Zoom X.");
}

void GUIMyFrame::slider_zooy_scroll(wxScrollEvent& event){
    SetStatusText("Zmiana Zoom Y.");
}

void GUIMyFrame::slider_point_size_scroll(wxScrollEvent& event){
    SetStatusText("Zmiana rozmiaru punktów.");
}

void GUIMyFrame::choice_point_style_change(wxCommandEvent& event){
    SetStatusText("Zmiana typu punktu.");
}

void GUIMyFrame::check_show_errorbars_click(wxCommandEvent& event){
    SetStatusText("Prze³¹czono s³upki b³êdów.");
}

void GUIMyFrame::choice_reg_type_change(wxCommandEvent& event){
    SetStatusText("Zmiana typu regresji.");
}

void GUIMyFrame::list_datasets_select(wxCommandEvent& event){
    SetStatusText("Wybrano zestaw danych.");
}

void GUIMyFrame::button_add_dataset_click(wxCommandEvent& event){
    SetStatusText("Klikniêto: Dodaj zestaw danych.");
}

void GUIMyFrame::button_remove_dataset_click(wxCommandEvent& event){
    SetStatusText("Klikniêto: Usuñ zestaw danych.");
}

void GUIMyFrame::button_point_color_click(wxCommandEvent& event){
    SetStatusText("Klikniêto: Kolor punktów.");
}

void GUIMyFrame::button_bg_color_click(wxCommandEvent& event){
    SetStatusText("Klikniêto: Kolor t³a.");
}

void GUIMyFrame::button_save_plot_click(wxCommandEvent& event){
    SetStatusText("Klikniêto: Zapisz wykres do pliku.");
}

void GUIMyFrame::button_copy_clipboard_click(wxCommandEvent& event){
    SetStatusText("Klikniêto: Kopiuj wykres do schowka.");
}

void GUIMyFrame::button_print_click(wxCommandEvent& event){
    SetStatusText("Klikniêto: Drukuj wykres.");
}
