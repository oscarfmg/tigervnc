#include "ConnectionsTable.h"
#include "ServerDialog.h"

#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/fl_draw.H>
#include <algorithm>

static const char* TABLE_HEADER[] = { "VNC Server", "Pin", "Run" };

ConnectionsTable::ConnectionsTable(int x, int y, int w, int h, ServerList &history)
: Fl_Table(x,y,w,h)
, history(history)
, callbackServername("")
, callbackPinned(false) {
    col_header(1);
    col_resize(0);
    row_header(0);
    row_resize(0);
    when(FL_WHEN_RELEASE);
    end();
    setRecentConnections();
}

ConnectionsTable::~ConnectionsTable() {
}

void ConnectionsTable::draw_cell(TableContext context,
    int R, int C, int X, int Y, int W, int H) {
    switch (context) {
        case CONTEXT_STARTPAGE:
            fl_font(FL_HELVETICA, FL_NORMAL_SIZE);
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

void ConnectionsTable::setRecentConnections() {
    std::sort(history.begin(),history.end());

    clear();
    cols(NUM_COLS);
    rows(history.size());

    begin(); {
        col_width(SERVER_COL,tiw-PIN_COL_SIZE-RUN_COL_SIZE);
        col_width(PIN_COL,PIN_COL_SIZE);
        col_width(RUN_COL,RUN_COL_SIZE);
        for ( size_t r = 0; r < history.size(); ++r) {
            int X,Y,W,H;

            find_cell(CONTEXT_TABLE,r,SERVER_COL,X,Y,W,H);
            Fl_Output *out = new Fl_Output(X,Y,W,H);
            out->value(strdup(history[r].getName().c_str()));

            find_cell(CONTEXT_TABLE,r,PIN_COL,X,Y,W,H);
            Fl_Check_Button *pin = new Fl_Check_Button(X,Y,W,H);
            pin->value(history[r].isPinned());

            find_cell(CONTEXT_TABLE,r,RUN_COL,X,Y,W,H);
            Fl_Button *run = new Fl_Button(X,Y,W,H,"@>");
            char shortcut[2] = {'0','\0'};
            shortcut[0] += (r + 1) % 10; // 1,2,3,...,9,0
            run->shortcut(FL_ALT | shortcut[0]);
            run->tooltip(strdup(shortcut));
            run->callback(handleRun, static_cast<void*>(this));
        }
    }
    end();
}

void ConnectionsTable::handleRun(Fl_Widget *widget, void* data) {
    ConnectionsTable *conTable = reinterpret_cast<ConnectionsTable*>(data);
    conTable->handleRun(widget);
}

void ConnectionsTable::handleRun(Fl_Widget *widget) {
    int row = find(widget) / NUM_COLS;

    Fl_Output *out = reinterpret_cast<Fl_Output*>(child(NUM_COLS*row + SERVER_COL));
    Fl_Check_Button *check = reinterpret_cast<Fl_Check_Button*>(child(NUM_COLS*row + PIN_COL));

    callbackServername =  out->value();
    callbackPinned = check->value();

    updatePinnedStatus();

    do_callback(CONTEXT_CELL,row,RUN_COL);
}

void ConnectionsTable::updatePinnedStatus(std::string newServer) {
    history.clear();
    int total_children = children();

    // First append the new server
    if (!newServer.empty()) {
        RankedServer server(0,newServer,false);
        history.push_back(server);
    }

    // Then rearrange with correct order in vector
    ServerList::iterator unpinned_it = history.begin();
    int row;
    for (row = 0; row < total_children / NUM_COLS; ++row) {
        Fl_Output *out = reinterpret_cast<Fl_Output*>(this->child(NUM_COLS * row + SERVER_COL));
        Fl_Check_Button *check = reinterpret_cast<Fl_Check_Button*>(this->child(NUM_COLS * row + PIN_COL));
        RankedServer server(row,out->value(),check->value());

        if (check->value()) {
            unpinned_it = history.insert(unpinned_it,server)+1;
        } else {
            history.push_back(server);
        }
    }

    if (history.size() > MAX_ROWS) { //TODO: Add parameter for MAX_ROWS
        history.pop_back();
    }

    history.erase(std::unique(history.begin(),history.end()),history.end());

    // Setting up the right rank number
    row = 0;
    for (ServerList::iterator it = history.begin(); it != history.end(); ++it, ++row) {
        it->setRank(row);
    }
}

std::string ConnectionsTable::callback_servername() {
    return callbackServername;
}

bool ConnectionsTable::callback_pinned() {
    return callbackPinned;
}