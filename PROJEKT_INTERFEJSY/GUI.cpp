#include "GUI.h"

MyFrame::MyFrame(wxWindow* parent,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style): wxFrame(parent, id, title, pos, size, style)
{
    //glowny size panel wykresu + panel z kontrolkami
    wxBoxSizer* bSizerMain = new wxBoxSizer(wxHORIZONTAL);

    // panel do rysowania wykresu
    WxPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    WxPanel->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));
    WxPanel->SetMinSize(this->FromDIP(wxSize(900, 800)));
    bSizerMain->Add(WxPanel, 1, wxEXPAND | wxALL, 5);

    // pionowy panel z przyciskami i suwakami
    wxBoxSizer* bSizerRight = new wxBoxSizer(wxVERTICAL);

    button_load_file = new wxButton(this, wxID_ANY, _("Wczytaj dane z pliku"));
    bSizerRight->Add(button_load_file, 0, wxEXPAND | wxALL, 5);

    button_add_keyboard = new wxButton(this, wxID_ANY, _("Dodaj dane z klawiatury"));
    bSizerRight->Add(button_add_keyboard, 0, wxEXPAND | wxALL, 5);

    button_auto_scale = new wxButton(this, wxID_ANY, _("Auto skala"));
    bSizerRight->Add(button_auto_scale, 0, wxEXPAND | wxALL, 5);

    label_zoox = new wxStaticText(this, wxID_ANY, _("Zoom X:"));
    bSizerRight->Add(label_zoox, 0, wxLEFT | wxRIGHT | wxTOP, 5);
    slider_zoox = new wxSlider(this, wxID_ANY, 50, 10, 200, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
    bSizerRight->Add(slider_zoox, 0, wxEXPAND | wxALL, 5);

    label_zooy = new wxStaticText(this, wxID_ANY, _("Zoom Y:"));
    bSizerRight->Add(label_zooy, 0, wxLEFT | wxRIGHT | wxTOP, 5);
    slider_zooy = new wxSlider(this, wxID_ANY, 50, 10, 200, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
    bSizerRight->Add(slider_zooy, 0, wxEXPAND | wxALL, 5);

    label_point_size = new wxStaticText(this, wxID_ANY, _("Rozmiar punktów:"));
    bSizerRight->Add(label_point_size, 0, wxLEFT | wxRIGHT | wxTOP, 5);
    slider_point_size = new wxSlider(this, wxID_ANY, 5, 1, 20, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
    bSizerRight->Add(slider_point_size, 0, wxEXPAND | wxALL, 5);

    label_point_style = new wxStaticText(this, wxID_ANY, _("Typ punktu:"));
    bSizerRight->Add(label_point_style, 0, wxLEFT | wxRIGHT | wxTOP, 5);
    wxString styles[] = { "Kółko", "Kwadrat", "Krzyżyk" };
    choice_point_style = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, styles);
    choice_point_style->SetSelection(0);
    bSizerRight->Add(choice_point_style, 0, wxEXPAND | wxALL, 5);

    check_show_errorbars = new wxCheckBox(this, wxID_ANY, _("Pokaż słupki błędów"));
    bSizerRight->Add(check_show_errorbars, 0, wxEXPAND | wxALL, 5);

    label_reg_type = new wxStaticText(this, wxID_ANY, _("Rodzaj regresji:"));
    bSizerRight->Add(label_reg_type, 0, wxLEFT | wxRIGHT | wxTOP, 5);
    wxString regTypes[] = { "Liniowa ", "Ortogonalna", "Nieliniowa (wykładnicza)" };
    choice_reg_type = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, regTypes);
    choice_reg_type->SetSelection(0);
    bSizerRight->Add(choice_reg_type, 0, wxEXPAND | wxALL, 5);

    label_datasets = new wxStaticText(this, wxID_ANY, _("Zestawy danych:"));
    bSizerRight->Add(label_datasets, 0, wxLEFT | wxRIGHT | wxTOP, 5);
    list_datasets = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 120));
    bSizerRight->Add(list_datasets, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer* bSizerDataButtons = new wxBoxSizer(wxHORIZONTAL);
    button_add_dataset = new wxButton(this, wxID_ANY, _("Dodaj zestaw"));
    button_remove_dataset = new wxButton(this, wxID_ANY, _("Usuń zestaw"));
    bSizerDataButtons->Add(button_add_dataset, 1, wxEXPAND | wxALL, 2);
    bSizerDataButtons->Add(button_remove_dataset, 1, wxEXPAND | wxALL, 2);
    bSizerRight->Add(bSizerDataButtons, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    button_point_color = new wxButton(this, wxID_ANY, _("Kolor punktów"));
    bSizerRight->Add(button_point_color, 0, wxEXPAND | wxALL, 5);

    button_bg_color = new wxButton(this, wxID_ANY, _("Kolor tła"));
    bSizerRight->Add(button_bg_color, 0, wxEXPAND | wxALL, 5);

    button_save_plot = new wxButton(this, wxID_ANY, _("Zapisz wykres do pliku"));
    bSizerRight->Add(button_save_plot, 0, wxEXPAND | wxALL, 5);

    button_copy_clipboard = new wxButton(this, wxID_ANY, _("Kopiuj wykres do schowka"));
    bSizerRight->Add(button_copy_clipboard, 0, wxEXPAND | wxALL, 5);

    button_print = new wxButton(this, wxID_ANY, _("Drukuj wykres"));
    bSizerRight->Add(button_print, 0, wxEXPAND | wxALL, 5);

    bSizerRight->AddStretchSpacer();

    bSizerMain->Add(bSizerRight, 0, wxEXPAND | wxALL, 5);

    // przypisanie sizer i dopasowanie okna
    this->SetSizerAndFit(bSizerMain);
    bSizerMain->SetSizeHints(this);
    this->Centre(wxBOTH);

    // polaczenia zdarzen
    this->Connect(wxEVT_CLOSE_WINDOW,wxCloseEventHandler(MyFrame::MainFormClose));

    WxPanel->Connect(wxEVT_UPDATE_UI,wxUpdateUIEventHandler(MyFrame::WxPanel_Repaint),NULL, this);
    
    WxPanel->Connect(wxEVT_LEFT_DOWN,wxMouseEventHandler(MyFrame::panel_left_down), NULL, this);
    WxPanel->Connect(wxEVT_MOTION,wxMouseEventHandler(MyFrame::panel_mouse_move),NULL, this);
    WxPanel->Connect(wxEVT_LEFT_UP,wxMouseEventHandler(MyFrame::panel_left_up),NULL, this);

    button_load_file->Connect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MyFrame::button_load_file_click),NULL, this);

    button_add_keyboard->Connect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MyFrame::button_add_keyboard_click),NULL, this);

    button_auto_scale->Connect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MyFrame::button_auto_scale_click),NULL, this);

    slider_zoox->Connect(wxEVT_SLIDER,wxScrollEventHandler(MyFrame::slider_zoox_scroll),NULL, this);

    slider_zooy->Connect(wxEVT_SLIDER,wxScrollEventHandler(MyFrame::slider_zooy_scroll),NULL, this);

    slider_point_size->Connect(wxEVT_SLIDER,wxScrollEventHandler(MyFrame::slider_point_size_scroll),NULL, this);

    choice_point_style->Connect(wxEVT_CHOICE,wxCommandEventHandler(MyFrame::choice_point_style_change),NULL, this);

    check_show_errorbars->Connect(wxEVT_CHECKBOX,wxCommandEventHandler(MyFrame::check_show_errorbars_click),NULL, this);

    choice_reg_type->Connect(wxEVT_CHOICE,wxCommandEventHandler(MyFrame::choice_reg_type_change),NULL, this);

    list_datasets->Connect(wxEVT_LISTBOX,wxCommandEventHandler(MyFrame::list_datasets_select),NULL, this);

    button_add_dataset->Connect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MyFrame::button_add_dataset_click),NULL, this);

    button_remove_dataset->Connect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MyFrame::button_remove_dataset_click),NULL, this);

    button_point_color->Connect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MyFrame::button_point_color_click),NULL, this);

    button_bg_color->Connect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MyFrame::button_bg_color_click),NULL, this);

    button_save_plot->Connect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MyFrame::button_save_plot_click),NULL, this);

    button_copy_clipboard->Connect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MyFrame::button_copy_clipboard_click),NULL, this);

    button_print->Connect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MyFrame::button_print_click),NULL, this);
}

MyFrame::~MyFrame()
{
    this->Disconnect(wxEVT_CLOSE_WINDOW,wxCloseEventHandler(MyFrame::MainFormClose));

    WxPanel->Disconnect(wxEVT_UPDATE_UI,wxUpdateUIEventHandler(MyFrame::WxPanel_Repaint),NULL, this);

    button_load_file->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MyFrame::button_load_file_click),NULL, this);

    button_add_keyboard->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MyFrame::button_add_keyboard_click),NULL, this);

    button_auto_scale->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MyFrame::button_auto_scale_click),NULL, this);

    slider_zoox->Disconnect(wxEVT_SLIDER,wxScrollEventHandler(MyFrame::slider_zoox_scroll),NULL, this);

    slider_zooy->Disconnect(wxEVT_SLIDER,wxScrollEventHandler(MyFrame::slider_zooy_scroll),NULL, this);

    slider_point_size->Disconnect(wxEVT_SLIDER,wxScrollEventHandler(MyFrame::slider_point_size_scroll),NULL, this);

    choice_point_style->Disconnect(wxEVT_CHOICE,wxCommandEventHandler(MyFrame::choice_point_style_change),NULL, this);

    check_show_errorbars->Disconnect(wxEVT_CHECKBOX,wxCommandEventHandler(MyFrame::check_show_errorbars_click),NULL, this);

    choice_reg_type->Disconnect(wxEVT_CHOICE,wxCommandEventHandler(MyFrame::choice_reg_type_change),NULL, this);

    list_datasets->Disconnect(wxEVT_LISTBOX,wxCommandEventHandler(MyFrame::list_datasets_select),NULL, this);

    button_add_dataset->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MyFrame::button_add_dataset_click),NULL, this);

    button_remove_dataset->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MyFrame::button_remove_dataset_click),NULL, this);

    button_point_color->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MyFrame::button_point_color_click),NULL, this);

    button_bg_color->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MyFrame::button_bg_color_click),NULL, this);

    button_save_plot->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MyFrame::button_save_plot_click),NULL, this);

    button_copy_clipboard->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MyFrame::button_copy_clipboard_click),NULL, this);

    button_print->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MyFrame::button_print_click),NULL, this);

    WxPanel->Disconnect(wxEVT_LEFT_DOWN,wxMouseEventHandler(MyFrame::panel_left_down),NULL, this);
    WxPanel->Disconnect(wxEVT_MOTION,wxMouseEventHandler(MyFrame::panel_mouse_move),NULL, this);
    WxPanel->Disconnect(wxEVT_LEFT_UP,wxMouseEventHandler(MyFrame::panel_left_up),NULL, this);

}
