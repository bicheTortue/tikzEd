#include "appwindow.h"
#include <iostream>

constexpr int GRID_SIZE = 20;

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
  m_previewText.set_editable(true);
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
    // Background
    cr->set_source_rgb(1.0, 1.0, 1.0);
    cr->paint();

    // Draw grid
    cr->set_source_rgb(0.9, 0.9, 0.9);
    for (int x = 0; x < width; x += GRID_SIZE)
      cr->move_to(x, 0), cr->line_to(x, height);
    for (int y = 0; y < height; y += GRID_SIZE)
      cr->move_to(0, y), cr->line_to(width, y);
    cr->stroke();

    // Draw nodes
    cr->set_source_rgb(0.0, 0.0, 0.8);
    for (auto& node : m_nodes) {
      // Draw the node circle
      cr->arc(node.x, node.y, 5, 0, 2 * M_PI);
      cr->fill();

      // Draw the label
      cr->move_to(node.x + 8, node.y - 8); // offset to the top-right
      cr->show_text(node.label);
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
    int grid_x = static_cast<int>(x / GRID_SIZE + 0.5) * GRID_SIZE;
    int grid_y = static_cast<int>(y / GRID_SIZE + 0.5) * GRID_SIZE;

    // Generate label: A, B, C, ..., Z, AA, AB, ...
    std::string label;
    int id = m_nextNodeId++;
    do {
      char c = 'A' + (id % 26);
      label = c + label;
      id = id / 26 - 1;
    } while (id >= 0);

    std::cout << "Placed node '" << label << "' at: " << grid_x << ", " << grid_y << std::endl;

    m_nodes.push_back({ grid_x, grid_y, label });
    m_canvas.queue_draw();

    // Update TikZ code
    auto buffer = m_previewText.get_buffer();
    std::string existing = buffer->get_text();
    // std::string end = existing.substr(existing.length() - 19);
    existing.erase(existing.length() - 19); // TODO : CHange to dynamic instead of hard coded

    existing += "\n\\node (" + label + ") at (" + std::to_string(grid_x / 10.0) + "," + std::to_string(grid_y / 10.0) + ") {};\n";
    existing += "\\end{tikzpicture}";

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
        "\n\\begin{tikzpicture}\n\\end{tikzpicture}");
  }
}
