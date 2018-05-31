#ifndef __CONNECTIONSTABLE_H__
#define __CONNECTIONSTABLE_H__

#include "parameters.h"

#include <FL/Fl_Table.h>
#include <FL/Fl.h>
#include <vector>
#include <string>

class ConnectionsTable : public Fl_Table {
protected:
    void draw_cell(TableContext context,
        int R=0, int C=0, int X=0, int Y=0, int W=0, int H=0);

public:
    ConnectionsTable(int x, int y, int w, int h, const char *l=0);
    ~ConnectionsTable();
    void setRecentConnections(HostnameList &);

private:
    static const int NUM_COLS = 3;
    static const int SERVER_COL = 0;
    static const int PIN_COL    = 1;
    static const int RUN_COL    = 2;
    static const int PIN_COL_SIZE = 22;
    static const int RUN_COL_SIZE = 28;

    static void handleRun(Fl_Widget *widget, void* data);
    void handleRun(Fl_Widget *w);
};

#endif //__CONNECTIONSTABLE_H__
