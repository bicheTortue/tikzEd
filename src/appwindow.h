#pragma once

#include <gtkmm.h>
#include <utility>
#include <vector>

class AppWindow : public Gtk::ApplicationWindow {
  public:
  AppWindow();
  ~AppWindow() override = default;

  private:
  std::vector<std::pair<double, double>> m_nodes; // list of node positions
  void on_canvas_click(int n_press, double x, double y);

  // Layout containers
  Gtk::HeaderBar m_header;
  Gtk::Box m_mainBox { Gtk::Orientation::VERTICAL };         // Top-down layout
  Gtk::Box m_topToolbarBox { Gtk::Orientation::HORIZONTAL }; // Toolbar under header
  Gtk::Box m_centerBox { Gtk::Orientation::HORIZONTAL };     // Canvas + preview

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
