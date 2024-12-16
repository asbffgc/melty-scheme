#include "../include/meltyscheme_window.h"

MeltySchemeWindow::MeltySchemeWindow() :
  window_box(Gtk::Orientation::VERTICAL),
  menu(),
  detection("MBAA.exe not detected, have you opened Melty?", true),
  inject_btn("Start Scheme REPL..."),
  status_wrapper("Console Output"),
  program_status()
{
  // Class variables
  this->melty_found_p = false;
  this->melty_pid = 0;

  // App metadata
  set_default_size(450, 350);
  set_title("Melty Scheme AACC");

  // Menu bar

  // PID Detection label
  this->detection.set_margin_top(10);

  // Button to inject DLL
  this->inject_btn.set_sensitive(melty_found_p);
  this->inject_btn.set_margin_start(100);
  this->inject_btn.set_margin_end(100);

  this->inject_btn.signal_clicked().connect(sigc::mem_fun(*this, &MeltySchemeWindow::inject_dll));
  Glib::signal_timeout().connect(sigc::mem_fun(*this, &MeltySchemeWindow::find_melty), 50);

  // Frame for console output
  this->status_wrapper.set_margin_start(10);
  this->status_wrapper.set_margin_end(10);
  this->status_wrapper.set_child(this->program_status);

  // Console output text
  this->program_status.set_editable(false);
  this->program_status.get_buffer()->set_text("\n\n\n\n\n\n\n");

  // Window grid
  this->window_box.set_spacing(10);
  this->window_box.append(this->detection);
  this->window_box.append(this->inject_btn);
  this->window_box.append(this->status_wrapper);

  // Finally, draw to window
  set_child(this->window_box);

  this->test = 0;
}

int MeltySchemeWindow::melty_alive_p() {
  HANDLE hSnapshot;
  PROCESSENTRY32 pe;
  bool hResult;

  hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (INVALID_HANDLE_VALUE == hSnapshot) return 0;

  pe.dwSize = sizeof(PROCESSENTRY32);
  hResult = Process32First(hSnapshot, &pe);
  while (hResult) {
    if (strcmp("MBAA.exe", pe.szExeFile) == 0) {
      this->melty_pid = pe.th32ProcessID;
      break;
    }
    hResult = Process32Next(hSnapshot, &pe);
  }
  CloseHandle(hSnapshot);

  return this->melty_pid;
}

bool MeltySchemeWindow::find_melty() {
  this->melty_pid = melty_alive_p();

  if (this->melty_pid) {
    DWORD ret_code{};
    HANDLE hMelty = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, this->melty_pid);

    if (GetExitCodeProcess(hMelty, &ret_code) && ret_code != STILL_ACTIVE) {
      this->melty_pid = 0;
    }
  }

  this->melty_found_p = (this->melty_pid) ? true : false;
  this->inject_btn.set_sensitive(melty_found_p);

  if (this->melty_found_p) this->detection.set_text("MBAA.exe found! Ready to start schemeing...");
  else this->detection.set_text("MBAA.exe not detected, have you opened Melty?");

  this->program_status.get_buffer()->set_text("\n\n\nFind MBAA.exe, PID: " + std::to_string(this->melty_pid));
  return true;
}

void MeltySchemeWindow::inject_dll() {
  std::string path_dll = std::filesystem::current_path().string() + "\\out\\melty_scheme.dll";
  std::wstring w_path_dll = std::wstring(path_dll.begin(), path_dll.end());
  size_t p_dll_len = (w_path_dll.length() + 1) * sizeof(wchar_t);

  std::ifstream f_dll(path_dll);
  std::cout << path_dll << std::endl;
  std::wcout << w_path_dll << std::endl;
  if (!f_dll) printf("no dll.\n");

  FARPROC load_address = GetProcAddress(GetModuleHandleW(L"Kernel32"), "LoadLibraryA");
  HANDLE proc_melty = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(this->melty_pid));
  if (!proc_melty) printf("failed to open process.\n");

  LPVOID dll_alloc = VirtualAllocEx(proc_melty, NULL, p_dll_len, (MEM_COMMIT | MEM_RESERVE), PAGE_EXECUTE_READWRITE);
  if (!dll_alloc) printf("no dll alloc");

  if (WriteProcessMemory(proc_melty, dll_alloc, path_dll.c_str(), p_dll_len, NULL) == 0)
    printf("no write to process mem.\n");

  DWORD tid;
  HANDLE remote = CreateRemoteThread(proc_melty, NULL, 0, (LPTHREAD_START_ROUTINE)load_address, dll_alloc, 0, &tid);
  if (!remote) printf("no remote thread.\n");

  printf("write address: %p\nloadlibrary address: %p\nthread id: %ld\n", dll_alloc, load_address, tid);

  WaitForSingleObject(remote, INFINITE);

  DWORD code = 0;
  if (GetExitCodeThread(remote, &code)) puts("dll loaded.");
  else puts("dll failed.");

  CloseHandle(proc_melty);
  CloseHandle(remote);
}
