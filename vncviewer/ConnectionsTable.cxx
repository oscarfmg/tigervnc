#include "ConnectionsTable.h"

#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/fl_draw.H>
#include <iostream>

static const char* TABLE_HEADER[] = { "VNC Server", "Pin", "Run" };

ConnectionsTable::ConnectionsTable(int x, int y, int w, int h, const char *l)
: Fl_Table(x,y,w,h,l) {
    col_header(1);
    col_resize(0);
    row_header(0);
    row_resize(0);
    when(FL_WHEN_RELEASE);
    end();
}

ConnectionsTable::~ConnectionsTable() {
}

void ConnectionsTable::draw_cell(TableContext context,
    int R, int C, int X, int Y, int W, int H) {
    switch (context) {
        case CONTEXT_STARTPAGE:
            fl_font(FL_HELVETICA, FL_NORMAL_SIZE * 2);
        break;

        case CONTEXT_RC_RESIZE: {
            int X,Y,W,H;
            int index = 0;
            for (int r=0;r<rows();++r) {
                for (int c=0;c<cols();++c) {
                    if(index>=children())break;
                    find_cell(CONTEXT_TABLE,r,c,X,Y,W,H);
                    child(index++)->resize(X,Y,W,H);
                }
            }
            init_sizes();
        }
        break;

        case CONTEXT_ROW_HEADER:
        break;

        case CONTEXT_COL_HEADER: {
            fl_push_clip(X,Y,W,H);
            fl_draw_box(FL_THIN_UP_BOX,X,Y,W,H,col_header_color());
            fl_color(FL_BLACK);
            fl_draw(TABLE_HEADER[C],X,Y,W,H,FL_ALIGN_CENTER);
            fl_pop_clip();
        }
        break;

        case CONTEXT_CELL:
        break;

        default:
        return;
    }
}

void ConnectionsTable::setRecentConnections(std::vector< std::pair<std::string,bool> > &history) {
    clear();
    cols(NUM_COLS);
    rows(history.size());

    begin(); {
        col_width(SERVER_COL,tiw-PIN_COL_SIZE-RUN_COL_SIZE);
        col_width(PIN_COL,PIN_COL_SIZE);
        col_width(RUN_COL,RUN_COL_SIZE);
        for ( size_t r = 0; r < history.size(); ++r) {
            int X,Y,W,H;
            find_cell(CONTEXT_TABLE,r,0,X,Y,W,H);

            Fl_Output *out = new Fl_Output(X,Y,W,H);
            // out->value(strdup(conns[r].c_str()));
            out->value(strdup(history[r].first.c_str()));

            find_cell(CONTEXT_TABLE,r,1,X,Y,W,H);

            Fl_Check_Button *pin = new Fl_Check_Button(X,Y,W,H);
            pin->value(history[r].second);

            find_cell(CONTEXT_TABLE,r,2,X,Y,W,H);
            Fl_Button *run = new Fl_Button(X,Y,W,H,"@>");
            run->callback(handleRun, static_cast<void*>(this));
        }
    }
    end();
}

void ConnectionsTable::handleRun(Fl_Widget *widget, void* data) {
    ConnectionsTable *conTable = static_cast<ConnectionsTable*>(data);
    conTable->handleRun(widget);
}

void ConnectionsTable::handleRun(Fl_Widget *widget) {
    int row = find(widget) / NUM_COLS;

    Fl_Output *out = static_cast<Fl_Output*>(child(NUM_COLS*row + SERVER_COL));
    Fl_Check_Button *check = static_cast<Fl_Check_Button*>(child(NUM_COLS*row + PIN_COL));
    int val = check->value();

    std::cout << "Handle Run" << std::endl;
    std::cout << "row: " << row;
    std::cout << ", server: " << out->value();
    std::cout << ", checked: " << val << std::endl;

    do_callback(CONTEXT_CELL,row,RUN_COL);
}
