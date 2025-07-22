#include "appwindow.h"
#include <iostream>

AppWindow::AppWindow()
{
  set_title("TikzEd");
  set_default_size(800, 600);

  // Header bar
  m_header.set_title_widget(*Gtk::make_managed<Gtk::Label>("TikzEd"));
  m_header.set_show_title_buttons(true);
  set_titlebar(m_header);

  // Toolbar setup
  for (auto* btn : { &m_btn_select, &m_btn_node, &m_btn_edge }) {
    btn->set_can_focus(false);
    btn->signal_toggled().connect(sigc::bind(sigc::mem_fun(*this, &AppWindow::on_tool_selected), btn));
    m_toolbar.append(*btn);
  }
  m_toolbar.set_margin(6);
  m_toolbar.set_spacing(6);

  // Add toolbar to main box
  m_mainBox.append(m_toolbar);

  // Canvas
  m_canvas.set_hexpand(true);
  m_canvas.set_vexpand(true);
  m_canvas.set_draw_func([](const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) {
    cr->set_source_rgb(1.0, 1.0, 1.0);
    cr->paint();
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->move_to(10, 20);
    cr->show_text("Canvas ready...");
  });

  // TikZ Preview panel
  m_previewText.set_editable(false);
  m_previewText.get_buffer()->set_text("% TikZ preview will appear here.");
  m_previewScroll.set_child(m_previewText);
  m_previewScroll.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
  m_previewScroll.set_size_request(300, -1);

  // Center box: Canvas + TikZ preview
  m_centerBox.set_spacing(6);
  m_centerBox.set_margin(6);
  m_centerBox.append(m_canvas);
  m_centerBox.append(m_previewScroll);

  // Add center box to main layout
  m_mainBox.append(m_centerBox);
  set_child(m_mainBox);
}

void AppWindow::on_tool_selected(Gtk::ToggleButton* clicked_button)
{
  for (auto* btn : { &m_btn_select, &m_btn_node, &m_btn_edge }) {
    if (btn != clicked_button)
      btn->set_active(false);
  }

  if (clicked_button->get_active()) {
    auto label = clicked_button->get_label();
    std::cout << "Tool selected: " << label << std::endl;

    // Update preview panel (placeholder)
    m_previewText.get_buffer()->set_text(
        "% Tool selected: " + label + "\n\\begin{tikzpicture}\n  % ...\n\\end{tikzpicture}");
  }
}
