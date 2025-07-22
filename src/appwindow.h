#pragma once

#include <gtkmm.h>

class AppWindow : public Gtk::ApplicationWindow {
  public:
  AppWindow();
  ~AppWindow() override = default;

  private:
  // Layout containers
  Gtk::HeaderBar m_header;
  Gtk::Box m_mainBox { Gtk::Orientation::HORIZONTAL };   // Left toolbar + center box
  Gtk::Box m_centerBox { Gtk::Orientation::HORIZONTAL }; // Canvas + preview
  Gtk::Box m_toolbar { Gtk::Orientation::VERTICAL };     // Left-side vertical toolbar

  // Toolbar buttons
  Gtk::ToggleButton m_btn_select { "Select" };
  Gtk::ToggleButton m_btn_node { "Node" };
  Gtk::ToggleButton m_btn_edge { "Edge" };

  // Canvas
  Gtk::DrawingArea m_canvas;

  // TikZ preview
  Gtk::TextView m_previewText;
  Gtk::ScrolledWindow m_previewScroll;

  // Tool handler
  void on_tool_selected(Gtk::ToggleButton* clicked_button);
};
