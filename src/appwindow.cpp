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

  // Toolbar (now horizontal)
  for (auto* btn : { &m_btn_select, &m_btn_node, &m_btn_edge }) {
    btn->set_can_focus(false);
    btn->signal_toggled().connect(sigc::bind(sigc::mem_fun(*this, &AppWindow::on_tool_selected), btn));
    m_topToolbarBox.append(*btn);
  }
  m_topToolbarBox.set_margin(6);
  m_topToolbarBox.set_spacing(6);
  m_mainBox.append(m_topToolbarBox); // Top toolbar

  // Canvas
  m_canvas.set_hexpand(true);
  m_canvas.set_vexpand(true);

  // TikZ Preview panel
  m_previewText.set_editable(false);
  m_previewText.get_buffer()->set_text("% TikZ preview will appear here.");
  m_previewScroll.set_child(m_previewText);
  m_previewScroll.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
  m_previewScroll.set_size_request(300, -1);

  // Canvas + Preview
  m_centerBox.set_spacing(6);
  m_centerBox.set_margin(6);
  m_centerBox.append(m_canvas);
  m_centerBox.append(m_previewScroll);
  m_mainBox.append(m_centerBox);

  set_child(m_mainBox);

  // Draw function
  m_canvas.set_draw_func([this](const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) {
    cr->set_source_rgb(1.0, 1.0, 1.0);
    cr->paint();

    // Draw existing nodes
    cr->set_source_rgb(0.0, 0.0, 0.8);
    for (auto& pt : m_nodes) {
      cr->arc(pt.first, pt.second, 5, 0, 2 * M_PI);
      cr->fill();
    }
  });

  // Handle mouse click
  m_canvas.add_controller(Gtk::GestureClick::create());
  auto click = Gtk::GestureClick::create();
  click->signal_pressed().connect(sigc::mem_fun(*this, &AppWindow::on_canvas_click), false);
  m_canvas.add_controller(click);
}

void AppWindow::on_canvas_click(int n_press, double x, double y)
{
  if (m_btn_node.get_active()) {
    std::cout << "Placed node at: " << x << ", " << y << std::endl;
    m_nodes.emplace_back(x, y);
    m_canvas.queue_draw();

    // Update preview
    auto buffer = m_previewText.get_buffer();
    std::string existing = buffer->get_text();
    existing += "\n\\node at (" + std::to_string(x / 10) + "," + std::to_string(y / 10) + ") {};";
    buffer->set_text(existing);
  }
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

    // Optional: update the TikZ preview
    m_previewText.get_buffer()->set_text(
        "% Tool selected: " + label + "\n\\begin{tikzpicture}\n  % ...\n\\end{tikzpicture}");
  }
}
