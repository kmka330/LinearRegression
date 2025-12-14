#pragma warning(disable: 4996)
#include "ChartClass.h"
#include "ConfigClass.h"
#include <wx/pen.h>
#include <wx/brush.h>
#include <wx/font.h>
#include <algorithm>
#include <cmath>
#include <vector>

//funkcje pomocnicze

//krok podzialki
static double GetStep(double range){
    if (range == 0.0) return 1.0;

    double step = range / 10.0;
    double magnitude = std::pow(10.0, std::floor(std::log10(step)));
    double norm = step / magnitude;
    double final_step;
    if (norm < 1.5) final_step = 1.0;
    else if (norm < 3.5) final_step = 2.0;
    else if (norm < 7.5) final_step = 5.0;
    else final_step = 10.0;
    return final_step * magnitude;
}

//rysowanie strzalki na osi
static void DrawArrow(wxDC* dc, int x, int y, bool isXAxis) {
    const int size = 10;
    const int width = 4;
    wxPoint points[3];
    points[0] = wxPoint(x, y);
    if (isXAxis) {
        points[1] = wxPoint(x - size, y - width);
        points[2] = wxPoint(x - size, y + width);
    }
    else {
        points[1] = wxPoint(x - width, y + size);
        points[2] = wxPoint(x + width, y + size);
    }
    dc->DrawPolygon(3, points);
}

ChartClass::ChartClass(std::shared_ptr<ConfigClass> c) : cfg(std::move(c)) {}

int ChartClass::WorldToScreenX(double x, double minX, double scaleX, int left) {
    return left + (int)((x - minX) * scaleX);
}

int ChartClass::WorldToScreenY(double y, double minY, double scaleY, int h, int bottom) {
    return h - bottom - (int)((y - minY) * scaleY);
}


void ChartClass::Draw(wxDC* dc, int w, int h) {
    dc->SetBackground(wxBrush(cfg->GetBackgroundColor()));
    dc->Clear();

    const auto& datasets = cfg->GetAllDataSets();
    bool anyData = false;
    for (const auto& ds : datasets) {
        if (!ds.points.empty()) anyData = true;
    }
    if (!anyData) {
        dc->SetTextForeground(*wxBLACK);
        dc->DrawText("Brak danych. Wczytaj dane lub dodaj punkt z klawiatury.", 20, 20);
        return;
    }

    //marginesy
    const int left = 60; 
    const int right = 40;
    const int top = 40;
    const int bottom = 40;

    double minX = 1e9;
    double maxX = -1e9;
    double minY = 1e9;
    double maxY = -1e9;
    bool first = true;
    const bool showErr = cfg->GetShowErrorBars();

    for (const auto& ds : datasets) {
        for (const auto& p : ds.points) {

            if (first) {
                minX = maxX = p.x;
                minY = maxY = p.y;
                first = false;
            }

            if (p.x < minX) minX = p.x;
            if (p.x > maxX) maxX = p.x;


            double yLow = p.y;
            double yHigh = p.y;

            if (showErr && p.uncertaintyY > 0.0) {
                yLow -= p.uncertaintyY;
                yHigh += p.uncertaintyY;
            }

            if (yLow < minY) minY = yLow;
            if (yHigh > maxY) maxY = yHigh;
        }
    }

    
    double dx = maxX - minX;
    double dy = maxY - minY;

    if (std::abs(dx) < 1e-9) {
        dx = 1.0; 
        minX -= 0.5;
        maxX += 0.5; 
    }
    if (std::abs(dy) < 1e-9) {
        dy = 1.0;
        minY -= 0.5;
        maxY += 0.5;
    }


    //zoom
    minX -= 0.05 * dx;
    maxX += 0.05 * dx;
    minY -= 0.05 * dy;
    maxY += 0.05 * dy;
    dx = maxX - minX;
    dy = maxY - minY;
    const double zoomX = cfg->GetZoomX();
    const double zoomY = cfg->GetZoomY();
    double midX = (minX + maxX) / 2.0;
    double midY = (minY + maxY) / 2.0;
    double visibleRangex = dx / zoomX;
    double visibleRangey = dy / zoomY;
    double viewMinX = midX - visibleRangex / 2.0;
    double viewMaxX = midX + visibleRangex / 2.0;
    double viewMinY = midY - visibleRangey / 2.0;
    double viewMaxY = midY + visibleRangey / 2.0;

    //skala
    const double scaleX = (w - left - right) / visibleRangex;
    const double scaleY = (h - top - bottom) / visibleRangey;

    //rysowanie siatki
    dc->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc->SetTextForeground(*wxBLACK);
    double stepX = GetStep(viewMaxX - viewMinX);
    double stepY = GetStep(viewMaxY - viewMinY);
    double startTickX = std::floor(viewMinX / stepX) * stepX;
    double startTickY = std::floor(viewMinY / stepY) * stepY;

    dc->SetPen(wxPen(wxColour(220, 220, 220), 1, wxPENSTYLE_SOLID));
    for (double v = startTickX; v <= viewMaxX + stepX * 0.01; v += stepX) {
        if (v < viewMinX) continue;
        int xPos = WorldToScreenX(v, viewMinX, scaleX, left);
        dc->DrawLine(xPos, top, xPos, h - bottom);
    }
    for (double v = startTickY; v <= viewMaxY + stepY * 0.01; v += stepY) {
        if (v < viewMinY) continue;
        int yPos = WorldToScreenY(v, viewMinY, scaleY, h, bottom);
        dc->DrawLine(left, yPos, w - right, yPos);
    }


    //rysowanie osi i strzalek
    dc->SetPen(wxPen(*wxBLACK, 2)); dc->SetBrush(*wxBLACK_BRUSH);
    dc->DrawLine(left, h - bottom, left, top - 10);
    DrawArrow(dc, left, top - 10, false);
    dc->DrawText("y", left - 15, top - 20);
    dc->DrawLine(left, h - bottom, w - right + 10, h - bottom);
    DrawArrow(dc, w - right + 10, h - bottom, true);
    dc->DrawText("x", w - right + 15, h - bottom - 5);

    dc->SetPen(wxPen(*wxBLACK, 1));
    dc->DrawLine(left, top, w - right, top);
    dc->DrawLine(w - right, top, w - right, h - bottom);

    //rysowanie podzialek na osiach
    dc->SetPen(*wxBLACK_PEN);
    for (double v = startTickX; v <= viewMaxX + stepX * 0.01; v += stepX) {
        if (v < viewMinX) continue;
        int xPos = WorldToScreenX(v, viewMinX, scaleX, left);
        dc->DrawLine(xPos, h - bottom, xPos, h - bottom + 5);
        wxString label = wxString::Format("%.4g", v);
        dc->DrawText(label, xPos - dc->GetTextExtent(label).GetWidth() / 2, h - bottom + 7);
    }
    for (double v = startTickY; v <= viewMaxY + stepY * 0.01; v += stepY) {
        if (v < viewMinY) continue;
        int yPos = WorldToScreenY(v, viewMinY, scaleY, h, bottom);
        dc->DrawLine(left - 5, yPos, left, yPos);
        wxString label = wxString::Format("%.4g", v);
        dc->DrawText(label, left - 7 - dc->GetTextExtent(label).GetWidth(), yPos - dc->GetTextExtent(label).GetHeight() / 2);
    }

    //osie kropkowane 
    dc->SetPen(wxPen(*wxBLACK, 1, wxPENSTYLE_DOT));
    if (viewMinY < 0 && viewMaxY > 0) dc->DrawLine(left, WorldToScreenY(0.0, viewMinY, scaleY, h, bottom), w - right, WorldToScreenY(0.0, viewMinY, scaleY, h, bottom));
    if (viewMinX < 0 && viewMaxX > 0) dc->DrawLine(WorldToScreenX(0.0, viewMinX, scaleX, left), top, WorldToScreenX(0.0, viewMinX, scaleX, left), h - bottom);



    // rysownaie punktow
    dc->SetClippingRegion(left, top, w - left - right, h - top - bottom);
    for (const auto& ds : datasets){
        dc->SetPen(wxPen(ds.color, 1));
        dc->SetBrush(wxBrush(ds.color));
        int ps = ds.size;
        PointShape shape = ds.shape;

        for (const auto& p : ds.points) {
            if (p.x < viewMinX || p.x > viewMaxX) continue;
            int x = WorldToScreenX(p.x, viewMinX, scaleX, left);
            int y = WorldToScreenY(p.y, viewMinY, scaleY, h, bottom);

            if (showErr && p.uncertaintyY > 0) {
                dc->SetPen(wxPen(wxColour(100, 100, 100), 1));
                int yt = WorldToScreenY(p.y + p.uncertaintyY, viewMinY, scaleY, h, bottom);
                int yb = WorldToScreenY(p.y - p.uncertaintyY, viewMinY, scaleY, h, bottom);
                dc->DrawLine(x, yt, x, yb);
                dc->DrawLine(x - 3, yt, x + 3, yt);
                dc->DrawLine(x - 3, yb, x + 3, yb);
                dc->SetPen(wxPen(ds.color, 1));
            }
            if (shape == PointShape::Circle) dc->DrawCircle(x, y, ps);
            else if (shape == PointShape::Square) dc->DrawRectangle(x - ps, y - ps, 2 * ps, 2 * ps);
            else if (shape == PointShape::Cross) {
                dc->DrawLine(x - ps, y - ps, x + ps, y + ps);
                dc->DrawLine(x - ps, y + ps, x + ps, y - ps);
            }
        }

        //rysowanie linii regresji
        if (ds.regResult.valid) {
            dc->SetPen(wxPen(ds.color, 2, wxPENSTYLE_SHORT_DASH));
            if (ds.regType == RegressionType::Linear || ds.regType == RegressionType::Orthogonal) {
                double x1 = viewMinX, x2 = viewMaxX;
                double y1 = ds.regResult.a * x1 + ds.regResult.b;
                double y2 = ds.regResult.a * x2 + ds.regResult.b;
                dc->DrawLine(WorldToScreenX(x1, viewMinX, scaleX, left),WorldToScreenY(y1, viewMinY, scaleY, h, bottom),WorldToScreenX(x2, viewMinX, scaleX, left),WorldToScreenY(y2, viewMinY, scaleY, h, bottom));
            }
            else if (ds.regType == RegressionType::Exponential) {
                int steps = 200;
                double step = (viewMaxX - viewMinX) / steps;
                for (int i = 0; i < steps; i++) {
                    double xc = viewMinX + i * step; double xn = viewMinX + (i + 1) * step;
                    double yc = ds.regResult.a * std::exp(ds.regResult.b * xc);
                    double yn = ds.regResult.a * std::exp(ds.regResult.b * xn);
                    if (std::abs(yc) > 1e10 || std::abs(yn) > 1e10) continue;
                    dc->DrawLine(WorldToScreenX(xc, viewMinX, scaleX, left),WorldToScreenY(yc, viewMinY, scaleY, h, bottom),WorldToScreenX(xn, viewMinX, scaleX, left),WorldToScreenY(yn, viewMinY, scaleY, h, bottom));
                }
            }
        }
    }
    dc->DestroyClippingRegion();


    //legenda
    int bx = cfg->GetInfoBoxX();
    int by = cfg->GetInfoBoxY();
    int line_height = 15; 
    int boxH = 5;

    for (const auto& ds : datasets) if (ds.regResult.valid) boxH += 4 * line_height + 5;
    if (boxH == 5) boxH = 30;
    int boxW = 240;
    if (bx < 5) bx = 5;
    if (by < 5) by = 5;
    if (bx + boxW > w - 5) bx = w - boxW - 5;
    if (by + boxH > h - 5) by = h - boxH - 5;
    cfg->SetInfoBoxPos(bx, by);

    dc->SetPen(*wxBLACK_PEN);
    dc->SetBrush(wxBrush(wxColour(255, 255, 255, 220)));
    dc->DrawRectangle(bx, by, boxW, boxH);
    dc->SetFont(wxFont(9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    int text_y = by + 5;

    for (const auto& ds : datasets) {
        if (!ds.regResult.valid) continue;
        dc->SetTextForeground(ds.color);
        dc->DrawText(ds.name + ":", bx + 5, text_y);
        text_y += line_height;
        dc->SetTextForeground(*wxBLACK);
        dc->SetFont(wxFont(9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        wxString wx_string;
        if (ds.regType == RegressionType::Linear) {
            wx_string = wxString::Format("y = %.4g x + %.4g", ds.regResult.a, ds.regResult.b);
            dc->DrawText(wx_string, bx + 10, text_y);
            text_y += line_height;
            dc->DrawText(wxString::Format("a=%.4g +/- %.2g", ds.regResult.a, ds.regResult.sa), bx + 10, text_y);
            text_y += line_height;
            dc->DrawText(wxString::Format("R^2=%.4f", ds.regResult.r2), bx + 10, text_y);
            text_y += line_height;
        }
        else if (ds.regType == RegressionType::Orthogonal) {
            wx_string = wxString::Format("Ort: y = %.4g x + %.4g", ds.regResult.a, ds.regResult.b);
            dc->DrawText(wx_string, bx + 10, text_y);
            text_y += line_height;
            dc->DrawText("Metoda ortogonalna", bx + 10, text_y);
            text_y += line_height;
            text_y += line_height;
        }
        else if (ds.regType == RegressionType::Exponential) {
            wx_string = wxString::Format("y = %.4g * e^(%.4g x)", ds.regResult.a, ds.regResult.b);
            dc->DrawText(wx_string, bx + 10, text_y);
            text_y += line_height;
            dc->DrawText(wxString::Format("R^2 (lin)=%.4f", ds.regResult.r2), bx + 10, text_y);
            text_y += line_height;
            text_y += line_height;
        }
        text_y += 5;
        dc->SetFont(wxFont(9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    }
}