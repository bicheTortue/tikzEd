#include "appwindow.h"
#include <gtkmm/application.h>

int main(int argc, char* argv[])
{
  auto app = Gtk::Application::create("org.barbaza.tikzed");
  return app->make_window_and_run<AppWindow>(argc, argv);
}
