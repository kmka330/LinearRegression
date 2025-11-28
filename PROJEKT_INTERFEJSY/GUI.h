#ifndef __GUI_H__
#define __GUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/slider.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include <wx/event.h>


//glowne okno aplikacji
class MyFrame : public wxFrame{
    public:
        wxPanel* WxPanel; 
        MyFrame(wxWindow* parent,wxWindowID id = wxID_ANY,const wxString& title = _("Linear Regression"),const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(1000, 600),long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
        ~MyFrame();

    protected:
        //PRZYCISKI
        // dane do wykresu
        wxButton* button_load_file;       //dane z pliku
        wxButton* button_add_keyboard;    //dane z klawiatury

        // skala
        wxButton* button_auto_scale;   
        wxStaticText* label_zoox;
        wxSlider* slider_zoox;
        wxStaticText* label_zooy;
        wxSlider* slider_zooy;

        // wyglad punktow
        wxStaticText* label_point_size;
        wxSlider* slider_point_size;
        wxStaticText* label_point_style;
        wxChoice* choice_point_style;

        //slupki bledow
        wxCheckBox* check_show_errorbars;

        //rodzaj regresji
        wxStaticText* label_reg_type;
        wxChoice* choice_reg_type;

        //zestawy danych
        wxStaticText* label_datasets;
        wxListBox* list_datasets;
        wxButton* button_add_dataset;
        wxButton* button_remove_dataset;

        //operacje na wykresie
        wxButton* button_point_color;
        wxButton* button_bg_color;
        wxButton* button_save_plot;
        wxButton* button_copy_clipboard;
        wxButton* button_print;

        //handlery do nadpisania w GUIMyFrame

        // zamkniecie okna programu
        virtual void MainFormClose(wxCloseEvent& event) { event.Skip(); }

        // odswiezenie panelu z wykresem
        virtual void WxPanel_Repaint(wxUpdateUIEvent& event) { event.Skip(); }

        // klikniecie: wczytaj dane z pliku
        virtual void button_load_file_click(wxCommandEvent& event) { event.Skip(); }

        // klikniecie: dodaj dane z klawiatury
        virtual void button_add_keyboard_click(wxCommandEvent& event) { event.Skip(); }

        // klikniecie: autoskalowanie wykresu
        virtual void button_auto_scale_click(wxCommandEvent& event) { event.Skip(); }

        // zmiana suwaka zoom x
        virtual void slider_zoox_scroll(wxScrollEvent& event) { event.Skip(); }

        // zmiana suwaka zoom y
        virtual void slider_zooy_scroll(wxScrollEvent& event) { event.Skip(); }

        // zmiana suwaka rozmiaru punktow
        virtual void slider_point_size_scroll(wxScrollEvent& event) { event.Skip(); }

        // zmiana wyboru typu punktu
        virtual void choice_point_style_change(wxCommandEvent& event) { event.Skip(); }

        // klikniecie: pokaz/ukryj slupki bledow
        virtual void check_show_errorbars_click(wxCommandEvent& event) { event.Skip(); }

        // zmiana typu regresji
        virtual void choice_reg_type_change(wxCommandEvent& event) { event.Skip(); }

        // wybranie zestawu danych z listy
        virtual void list_datasets_select(wxCommandEvent& event) { event.Skip(); }

        // klikniecie: dodaj zestaw danych
        virtual void button_add_dataset_click(wxCommandEvent& event) { event.Skip(); }

        // klikniecie: usun zestaw danych
        virtual void button_remove_dataset_click(wxCommandEvent& event) { event.Skip(); }

        // klikniecie: zmien kolor punktow
        virtual void button_point_color_click(wxCommandEvent& event) { event.Skip(); }

        // klikniecie: zmien kolor tla
        virtual void button_bg_color_click(wxCommandEvent& event) { event.Skip(); }

        // klikniecie: zapisz wykres do pliku
        virtual void button_save_plot_click(wxCommandEvent& event) { event.Skip(); }

        // klikniecie: kopiuj wykres do schowka
        virtual void button_copy_clipboard_click(wxCommandEvent& event) { event.Skip(); }

        // klikniecie: drukuj wykres
        virtual void button_print_click(wxCommandEvent& event) { event.Skip(); }

};

#endif // __GUI_H__
