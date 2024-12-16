/**
 * main.cpp
 * By: asbffgc
 * Date: 16 December 2024
 * Entry into melty-scheme application,
 * creating a GTK window to load scheme
 * scripts into an instance of MBAA.exe.
 */
#include "include/lib.h"
#include "include/meltyscheme_window.h"

/**
 * Entry point, creates a GTK application window
 * that houses all program functionalities.
 * @param argc command line argument count, UNUSED
 * @param arv  command line arguments, UNUSED
 */
int main(int argc, char **argv) {
  auto application = Gtk::Application::create("org.asbf.meltyscheme");

  return application->make_window_and_run<MeltySchemeWindow>(argc, argv);
}
