#ifndef MELTYSCHEME_WINDOW_H
#define MELTYSCHEME_WINDOW_H

#include "../include/lib.h"

/**
 * GTK window class, contains main functionalities
 * to find a MBAACC executable, save its PID, and
 * create a handler to it or inject a DLL.
 */
class MeltySchemeWindow : public Gtk::Window {
 private:
  Gtk::Box            window_box;      // Attach child elements to application screen
  Gtk::PopoverMenuBar menu;            // Window menu for aux purposes
  Gtk::Label          detection;       // Tells user whether MBAA.exe has been found
  Gtk::Button         inject_btn;      // User presses button to inject DLL into Melty. Can't be pressed when MBAA.exe PID not found
  Gtk::Frame          status_wrapper;  // Frame for (temporary?) console output
  Gtk::TextView       program_status;  // (temporary?) console output window

  bool melty_found_p;   // Is MBAA.exe an living process?
  int  melty_pid;       // 0 = not found/dead process, ... = current process ID

 public:
  MeltySchemeWindow();
  ~MeltySchemeWindow() = default;

  // GTK widget hooks

  /**
   * Check if a process running MBAA.exe exists.
   * @return PID 0 if MBAA.exe not found, unique PID if found
   */
  int melty_alive_p();

  /**
   * Callback for GTK timeout function,
   * enables/disables injection button within window,
   * tells user whether they're able to create a handler/inject DLL.
   * @return TRUE to always keep timeout function processing
   */
  bool find_melty();      // bool because handler for timeout

  /**
   * Inject a custom DLL into MBAA.exe
   * (Currently being deprecated since DLLs don't want to work)
   */
  void inject_dll();
};

#endif
