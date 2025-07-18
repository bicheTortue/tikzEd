#pragma once

#include <gtkmm.h>

class AppWindow : public Gtk::ApplicationWindow {
  public:
  AppWindow();
  ~AppWindow() override = default;

  private:
  Gtk::HeaderBar m_header;
  Gtk::DrawingArea m_canvas;
};
