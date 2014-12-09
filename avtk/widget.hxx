
#ifndef OPENAV_AVTK_WIDGET_HXX
#define OPENAV_AVTK_WIDGET_HXX

#include <stdio.h>
#include <string>
#include <cairo/cairo.h>

#include "utils.hxx"
#include "theme.hxx"
#include "helpers.hxx"
#include "pugl/pugl.h"


namespace Avtk
{

class UI;

class Widget
{
  public:
    Widget( Avtk::UI* ui_, int x_, int y_, int w_, int h_, std::string label_) :
      ui(ui_),
      x( x_ ),
      y( y_ ),
      w( w_ ),
      h( h_ ),
      label( label_ ),
      value_( 0 ),
      mouseButtonPressed_(0),
      
      callback( 0 ),
      callbackUD( 0 )
    {
      if( theme == 0 )
      {
        theme = new Theme();
      }
    }
    
    virtual ~Widget(){}
    
    
    /// get the current value
    float value()
    {
      return value_;
    }
    
    /// set a new value, triggers "new-value" event
    void value( float v )
    {
      value_ = v;
      
      // call the callback if its set, and not told not to
      if ( true && callback )
        callback( this, callbackUD );
    }
    
    virtual void draw( cairo_t* cr ) = 0;
    
    
    bool touches( int inx, int iny )
    {
      return ( inx >= x && inx <= x + w && iny >= y && iny <= y + h);
    }
    
    /// called by the UI class on any event that occurs
    int handle( const PuglEvent* event );
    
    // FIXME: move to UI
    static Theme* theme;
    
    /// the callback and its userdata pointer
    void (*callback)(Widget* , void*);
    void* callbackUD;
    
    int x, y, w, h;         /// widget co-ords and size
    std::string label;      /// widget name - sometimes shown in UI
    float value_;           /// widget value
    
    // 0 when no mouse button is down, otherwise the mouse button pressed
    int mouseButtonPressed_;
  
  private:
    /// the Avtk::UI pointer, used to redraw the view etc
    Avtk::UI* ui;
    
};

};

#endif // OPENAV_AVTK_WIDGET_HXX
