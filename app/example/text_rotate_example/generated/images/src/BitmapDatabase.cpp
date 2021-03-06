// Generated by imageconverter. Please, do not edit!

#include <touchgfx/Bitmap.hpp>
#include <BitmapDatabase.hpp>

#ifndef NO_USING_NAMESPACE_TOUCHGFX
using namespace touchgfx;
#endif

extern const unsigned char _background[]; 
extern const unsigned char _btn[]; 
extern const unsigned char _btn_alpha_channel[]; 
extern const unsigned char _btn2[]; 
extern const unsigned char _btn2_alpha_channel[]; 
extern const unsigned char _btn2_pressed[]; 
extern const unsigned char _btn2_pressed_alpha_channel[]; 
extern const unsigned char _btn_pressed[]; 
extern const unsigned char _btn_pressed_alpha_channel[]; 

const touchgfx::Bitmap::BitmapData bitmap_database[] = {
    { _background, 0, 480, 272, 0, 0, 480, 272, touchgfx::Bitmap::RGB565 },
    { _btn, _btn_alpha_channel, 130, 56, 3, 0, 124, 55, touchgfx::Bitmap::RGB565 },
    { _btn2, _btn2_alpha_channel, 56, 130, 1, 3, 55, 124, touchgfx::Bitmap::RGB565 },
    { _btn2_pressed, _btn2_pressed_alpha_channel, 56, 130, 1, 3, 55, 124, touchgfx::Bitmap::RGB565 },
    { _btn_pressed, _btn_pressed_alpha_channel, 130, 56, 3, 0, 124, 55, touchgfx::Bitmap::RGB565 }
};

namespace BitmapDatabase
{
  const touchgfx::Bitmap::BitmapData* getInstance()
  {
    return bitmap_database;
  }
  uint16_t getInstanceSize()
  {
    return (uint16_t)(sizeof(bitmap_database) / sizeof(touchgfx::Bitmap::BitmapData));
  }
}

