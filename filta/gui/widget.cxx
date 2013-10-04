// generated by Fast Light User Interface Designer (fluid) version 1.0300

#include "widget.h"

void Widget::cb_headerImage_i(Avtk::Image*, void*) {
  //system("xdg-open http://www.openavproductions.com/artyfx#ducka");
}
void Widget::cb_headerImage(Avtk::Image* o, void* v) {
  ((Widget*)(o->parent()->user_data()))->cb_headerImage_i(o,v);
}

void Widget::cb_graph_i(Avtk::Filtergraph* o, void*) {
  float tmp = o->value();
freq->value( tmp );
writePort(FILTA_FREQ_CONTROL, tmp);

float a = o->getActive();
writePort(FILTA_ACTIVE, a);
printf("active %f\n", a );
}
void Widget::cb_graph(Avtk::Filtergraph* o, void* v) {
  ((Widget*)(o->parent()->user_data()))->cb_graph_i(o,v);
}

void Widget::cb_freq_i(Avtk::Dial* o, void*) {
  float tmp = o->value();
graph->value( tmp );
writePort(FILTA_FREQ_CONTROL, tmp);
}
void Widget::cb_freq(Avtk::Dial* o, void* v) {
  ((Widget*)(o->parent()->user_data()))->cb_freq_i(o,v);
}

/**
   if the type of filter changes, this function will highlight the right button
*/
void Widget::update_button(int button) {
}

Widget::Widget() {
  { window = new Fl_Double_Window(160, 220);
    window->user_data((void*)(this));
    { headerImage = new Avtk::Image(0, 0, 160, 29, "header.png");
      headerImage->box(FL_NO_BOX);
      headerImage->color(FL_BACKGROUND_COLOR);
      headerImage->selection_color(FL_BACKGROUND_COLOR);
      headerImage->labeltype(FL_NORMAL_LABEL);
      headerImage->labelfont(0);
      headerImage->labelsize(14);
      headerImage->labelcolor((Fl_Color)20);
      headerImage->callback((Fl_Callback*)cb_headerImage);
      headerImage->align(Fl_Align(FL_ALIGN_CENTER));
      headerImage->when(FL_WHEN_RELEASE_ALWAYS);
      headerImage->setPixbuf(header.pixel_data,4);
    } // Avtk::Image* headerImage
    { graph = new Avtk::Filtergraph(5, 36, 150, 126, "graph");
      graph->box(FL_UP_BOX);
      graph->color((Fl_Color)179);
      graph->selection_color(FL_INACTIVE_COLOR);
      graph->labeltype(FL_NO_LABEL);
      graph->labelfont(0);
      graph->labelsize(14);
      graph->labelcolor(FL_FOREGROUND_COLOR);
      graph->callback((Fl_Callback*)cb_graph);
      graph->align(Fl_Align(FL_ALIGN_BOTTOM));
      graph->when(FL_WHEN_CHANGED);
    } // Avtk::Filtergraph* graph
    { freq = new Avtk::Dial(62, 169, 37, 37, "Frequency");
      freq->box(FL_NO_BOX);
      freq->color((Fl_Color)90);
      freq->selection_color(FL_INACTIVE_COLOR);
      freq->labeltype(FL_NORMAL_LABEL);
      freq->labelfont(0);
      freq->labelsize(10);
      freq->labelcolor(FL_FOREGROUND_COLOR);
      freq->callback((Fl_Callback*)cb_freq);
      freq->align(Fl_Align(FL_ALIGN_BOTTOM));
      freq->when(FL_WHEN_CHANGED);
    } // Avtk::Dial* freq
    window->color( fl_rgb_color( 17, 17, 17) );
    window->end();
  } // Fl_Double_Window* window
}

void Widget::idle() {
  Fl::check();
  Fl::flush();
}

int Widget::getWidth() {
  return window->w();
}

int Widget::getHeight() {
  return window->h();
}

void Widget::writePort(int port, float& value) {
  //cout << "port " << port << " value " << value << endl;
  write_function(controller, port, sizeof(float), 0, &value);
}
