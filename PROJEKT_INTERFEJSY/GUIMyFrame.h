#ifndef __GUIMyFrame__
#define __GUIMyFrame__

#include "GUI.h"
#include <wx/dcbuffer.h>
#include <memory>

class ConfigClass;   // forward
class ChartClass;    // forward

class GUIMyFrame : public MyFrame{
    protected:
        //handlery
        void MainFormClose(wxCloseEvent& event) override;
        void WxPanel_Repaint(wxUpdateUIEvent& event) override;

        void button_load_file_click(wxCommandEvent& event) override;
        void button_add_keyboard_click(wxCommandEvent& event) override;

        void button_auto_scale_click(wxCommandEvent& event) override;
        void slider_zoox_scroll(wxScrollEvent& event) override;
        void slider_zooy_scroll(wxScrollEvent& event) override;

        void slider_point_size_scroll(wxScrollEvent& event) override;
        void choice_point_style_change(wxCommandEvent& event) override;

        void check_show_errorbars_click(wxCommandEvent& event) override;

        void choice_reg_type_change(wxCommandEvent& event) override;

        void list_datasets_select(wxCommandEvent& event) override;
        void button_add_dataset_click(wxCommandEvent& event) override;
        void button_remove_dataset_click(wxCommandEvent& event) override;

        void button_point_color_click(wxCommandEvent& event) override;
        void button_bg_color_click(wxCommandEvent& event) override;
        void button_save_plot_click(wxCommandEvent& event) override;
        void button_copy_clipboard_click(wxCommandEvent& event) override;
        void button_print_click(wxCommandEvent& event) override;

    public:
        GUIMyFrame(wxWindow* parent);

        void Repaint();

        std::shared_ptr<ConfigClass> cfg; 

        ~GUIMyFrame() = default;
};

#endif // __GUIMyFrame__
