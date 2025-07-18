#include "appwindow.h"

AppWindow::AppWindow()
{
  set_title("TikzEd");
  set_default_size(800, 600);

  // Setup header bar
  m_header.set_title_widget(*Gtk::make_managed<Gtk::Label>("TikzEd"));
  m_header.set_show_title_buttons(true);
  set_titlebar(m_header);

  // Setup drawing area
  m_canvas.set_draw_func([](const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) {
    cr->set_source_rgb(1.0, 1.0, 1.0);
    cr->paint();
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->move_to(10, 10);
    cr->show_text("Canvas ready...");
  });

  set_child(m_canvas);
}
