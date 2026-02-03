#ifndef NCURSES_MACRO_H
#define NCURSES_MACRO_H

#include <ncurses.h>

#define BOLD_BEGIN attron(A_BOLD);
#define BOLD_END attroff(A_BOLD);
#define REVERSE_BEGIN attron(A_REVERSE);
#define REVERSE_END attroff(A_REVERSE);

#define PRIMARY_COLOR COLOR_YELLOW
#define SECONDARY_COLOR COLOR_GREEN
#define F_PRI_COL_BEGIN attron(COLOR_PAIR(1));
#define F_PRI_COL_END attroff(COLOR_PAIR(1));
#define F_SEC_COL_BEGIN attron(COLOR_PAIR(2));
#define F_SEC_COL_END attroff(COLOR_PAIR(2));

#endif // !NCURSES_MACRO_H
