#include <ncurses.h>
#include <string>
#include <unordered_set>
#include <vector>

#define BOLD_BEGIN attron(A_BOLD);
#define BOLD_END attroff(A_BOLD);

#define PRIMARY_COLOR COLOR_YELLOW
#define SECONDARY_COLOR COLOR_GREEN
#define F_PRI_COL_BEGIN attron(COLOR_PAIR(1));
#define F_PRI_COL_END attroff(COLOR_PAIR(1));
#define F_SEC_COL_BEGIN attron(COLOR_PAIR(2));
#define F_SEC_COL_END attroff(COLOR_PAIR(2));

#define VERSION "0.0.1"

enum Tab { BULBFLOW = 0, EDITOR = 1, PROFILE = 2, HELP = 3 };
enum Mode { NORMAL = 0, EDIT = 1 };
std::unordered_set<int> HotKey = {':', '@', '\t', KEY_BTAB, 27};

class BulbFlowApp {
private:
  Tab currentTab;
  Mode currentMode;
  std::vector<std::string> tabNames;
  std::vector<std::string> modeNames;
  int maxY, maxX;
  bool running;
  std::string eBuff;

  void drawStatusLine(int ch) {
    int statusY = maxY - 1;

    move(statusY, 0);
    attron(A_REVERSE);

    F_PRI_COL_BEGIN
    BOLD_BEGIN
    printw(" %s ", tabNames[currentTab].c_str());
    BOLD_END
    F_PRI_COL_END

    attroff(A_REVERSE);
    if (HotKey.find(ch) != HotKey.end()) {
      switch (ch) {
      case '\t':
        printw(" ->|");
        break;
      case KEY_BTAB:
        printw(" |<-");
        break;
      case 27:
        printw(" ESC");
        break;
      default:
        printw(" %c  ", ch);
        break;
      }
    } else {
      printw((ch == -1) ? "    " : " ?  ");
    }

    int currentX = getcurx(stdscr);
    for (int i = currentX + 4; i < maxX; i++) {
      addch(' ');
    }
    attron(A_REVERSE);

    attroff(A_REVERSE);
    refresh();
  }

  void drawContent(int ch) {
    for (int i = 0; i < maxY - 1; i++) {
      move(i, 0);
      clrtoeol();
    }

    switch (currentTab) {
    case BULBFLOW:
      drawBulbFlow();
      break;
    case EDITOR:
      drawEditor((currentMode == EDIT) ? ch : -1);
      break;
    case PROFILE:
      drawProfile();
      break;
    case HELP:
      drawHelp();
      break;
    }
  }

  void drawBulbFlow() {
    mvprintw(0, 0, "BulbFlow");
    mvprintw(3, 0, "Building...");
  }

  void drawEditor(int ch) {
    if (ch != -1) {
      if (ch >= 32) {
        eBuff += ch;
      }
    }

    mvprintw(0, 0, "editing => %s", modeNames[currentMode].c_str());

    move(1, 0);
    for (int i = 0; i < maxX; i++) {
      printw("-");
    }

    mvprintw(2, 0, "%s", eBuff.c_str());
  }

  void drawProfile() {
    mvprintw(2, 2, "=== Profile ===");
    mvprintw(4, 2, "User Profile Settings");
    mvprintw(5, 2, "Manage your account and preferences here.");
    mvprintw(7, 2, "Settings:");
    mvprintw(8, 4, "- Username: demo_user");
    mvprintw(9, 4, "- Theme: Dark");
    mvprintw(10, 4, "- Language: C++");
  }

  void drawHelp() {
    mvprintw(0, 0, "BulbFlow v%s", VERSION);
    mvprintw(3, 0, "Control");
  }

  void nextTab() {
    currentTab = static_cast<Tab>((currentTab + 1) % tabNames.size());
    currentMode = NORMAL;
  }

  void prevTab() {
    int prev = currentTab - 1;
    if (prev < 0)
      prev = tabNames.size() - 1;
    currentTab = static_cast<Tab>(prev);
    currentMode = NORMAL;
  }

public:
  BulbFlowApp()
      : currentTab(BULBFLOW), currentMode(NORMAL), running(true), eBuff("") {
    tabNames = {"BULBFLOW", "EDITOR", "PROFILE", "HELP"};
    modeNames = {"NOR", "EDT"};
  }

  void init() {
    // Initialize ncurses
    initscr();            // Start curses mode
    cbreak();             // Disable line buffering
    noecho();             // Don't echo input
    keypad(stdscr, TRUE); // Enable function keys
    curs_set(0);          // Hide cursor
    setlocale(LC_ALL, "");
    set_escdelay(42);

    // Initialize color
    start_color();
    use_default_colors();

    init_pair(1, PRIMARY_COLOR, -1);
    init_pair(2, SECONDARY_COLOR, -1);

    // Get terminal size
    getmaxyx(stdscr, maxY, maxX);
  }

  void run() {
    int ch, prev_ch = -1;
    while (running) {

      drawContent(prev_ch);
      drawStatusLine((prev_ch == -2) ? '@'
                                     : ((currentMode == EDIT) ? -1 : prev_ch));

      ch = getch();

      switch (ch) {
      case '@':
        if (currentTab == EDITOR) {
          currentMode = (currentMode == NORMAL) ? EDIT : NORMAL;
          ch = -2; // Special treatment for @
        }
        break;
      case 'q':
      case 'Q':
        if (currentMode == NORMAL && prev_ch == ':')
          running = false;
        break;
      case '\t':
        if (currentMode == NORMAL)
          nextTab();
        break;
      case KEY_BTAB:
        if (currentMode == NORMAL)
          prevTab();
        break;
      case KEY_RESIZE:
        getmaxyx(stdscr, maxY, maxX);
        break;
      }

      prev_ch = ch;
    }

    for (int i = 0; i < maxY; i++) {
      move(i, 0);
      clrtoeol();
    }

    mvprintw(maxY / 2 - 1, 0, "Goodbye!");
    mvprintw(maxY / 2 + 2, 0, "Press Any Key to Quit...");
    getch();
  }

  void cleanup() { endwin(); }
};

int main() {
  BulbFlowApp app;

  app.init();
  app.run();
  app.cleanup();

  return 0;
}
