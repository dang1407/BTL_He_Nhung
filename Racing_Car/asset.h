#define Flappy_width 12
#define Flappy_height 12
#define Building_width 64
#define Building_height 64
#define Car_width 32
#define Car_height 15
#define Truck_width 44
#define Truck_height 17
const unsigned char Flappy[] = {
    0xC0,0x00,0x47,0x01,0xF9,0x07,0x61,0x0F,0x20,0x0B,
    0xE2,0x0F,0x22,0x0B,0x4C,0x0E,0x24,0x0E,0x10,0x05,
    0x18,0x05,0xE0,0x01,
};

const unsigned char Car[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0x10, 0x08, 0xFC, 0xFF, 0x9F, 0x3F, 
  0x9C, 0xFF, 0x07, 0x3F, 0x0E, 0xFF, 0x07, 0x7F, 0x0E, 0xEF, 0x07, 0x7F, 
  0x0C, 0xD5, 0x07, 0xFF, 0x02, 0x89, 0x07, 0xFF, 0x0A, 0xD0, 0x07, 0xFB, 
  0x08, 0xCA, 0x07, 0x7F, 0x0A, 0xC9, 0x06, 0x7F, 0xBC, 0xFF, 0x83, 0x3F, 
  0xF8, 0xFF, 0xFF, 0x3F, 0x00, 0x22, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 
  };

const unsigned char Building[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0xE0,0xFF,0xFF,0x01,0x00,0x00,0x00,0x00,0xE0,
    0xFF,0xFF,0x01,0x00,0x00,0x00,0x00,0xE0,0xFF,0xFF,
    0x01,0x00,0x00,0x00,0x00,0x60,0x00,0x80,0x01,0x00,
    0x00,0x00,0x00,0x60,0x00,0x80,0x01,0x00,0x00,0x00,
    0x00,0x60,0x38,0x86,0x01,0xFF,0xFF,0x0F,0x00,0x60,
    0x38,0x87,0x01,0xFF,0xFF,0x0F,0x00,0x60,0x38,0x87,
    0x01,0xFF,0xFF,0x0F,0x00,0x60,0x00,0x80,0x01,0x03,
    0x00,0x0C,0x00,0x60,0x00,0x80,0x01,0x03,0x00,0x0C,
    0x00,0x60,0x38,0x86,0x01,0xC3,0x10,0x0C,0x00,0x60,
    0x38,0x87,0x01,0xC3,0x39,0x0C,0x00,0x60,0x38,0x87,
    0x01,0xC3,0x39,0x0C,0x00,0x60,0x00,0x80,0x01,0x03,
    0x00,0x0C,0x00,0x60,0x00,0x80,0x01,0x03,0x00,0x0C,
    0x00,0x60,0x00,0x80,0x01,0x03,0x00,0x0C,0x00,0x60,
    0x00,0x80,0x01,0xC3,0x39,0x0C,0x00,0x60,0x00,0x80,
    0x01,0xC3,0x39,0x0C,0x00,0x60,0x00,0x80,0x01,0x03,
    0x00,0x0C,0x00,0x60,0x00,0x80,0x01,0x03,0x00,0x0C,
    0x00,0xF0,0x00,0x80,0x01,0x03,0x00,0x0C,0xF0,0xFF,
    0xFF,0x80,0x01,0xC3,0x39,0x0C,0xF0,0xFF,0xFF,0x80,
    0x01,0xC3,0x39,0x0C,0x30,0x00,0xC0,0x80,0x01,0x03,
    0x00,0x0C,0x30,0x00,0xC0,0x80,0x01,0x03,0x00,0x0C,
    0x30,0x00,0xC0,0x80,0x01,0x03,0x00,0x0C,0x30,0x9C,
    0xC3,0x80,0x01,0xC3,0x39,0x0C,0x30,0x9C,0xC3,0x80,
    0x01,0xC3,0x39,0x0C,0x30,0x00,0xC0,0x80,0x01,0x03,
    0x00,0x0C,0x30,0x00,0xC0,0x80,0x01,0x03,0x00,0x0C,
    0x30,0x00,0xC0,0x80,0x01,0x03,0x00,0x0C,0x30,0x9C,
    0xC3,0xFF,0xFF,0xC3,0x39,0x0C,0x30,0x9C,0xC3,0xFF,
    0xFF,0xC3,0x39,0x0C,0x30,0x00,0xC0,0x01,0x80,0x03,
    0x00,0x0C,0x30,0x00,0xC0,0x00,0x00,0x03,0x00,0x0C,
    0x30,0x00,0xC0,0x00,0x00,0x03,0x00,0x0C,0x30,0x9C,
    0xC3,0x70,0x0E,0xC3,0x39,0x0C,0x30,0x9C,0xC3,0x70,
    0x0E,0xC3,0x39,0x0C,0x30,0x08,0xC1,0x20,0x04,0x83,
    0x10,0x0C,0x70,0x00,0xC0,0x00,0x00,0x03,0x00,0x0C,
    0x70,0x00,0xC0,0x00,0x00,0x03,0x00,0x0C,0x70,0x00,
    0xC0,0x70,0x0E,0xC3,0x39,0x0C,0x70,0x00,0xC0,0x70,
    0x0E,0xC3,0x39,0x0C,0x70,0x00,0xC0,0x30,0x0C,0xC3,
    0x18,0x0C,0x70,0x00,0xC0,0x00,0x00,0x03,0x00,0x0C,
    0x70,0x00,0xC0,0x00,0x00,0x03,0x00,0x0C,0x70,0x60,
    0xC0,0x80,0x01,0x03,0x06,0x0C,0x70,0x60,0xC0,0x80,
    0x01,0x03,0x06,0x0C,0x70,0x60,0xC0,0x80,0x01,0x03,
    0x06,0x0C,0x70,0x60,0xC0,0x80,0x01,0x03,0x06,0x0C,
    0x70,0x60,0xC0,0x80,0x01,0x03,0x06,0x0C,0xF0,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0x0F,0xF0,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0x0F,0xF0,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,
};

const unsigned char Truck[] = {
  0x80, 0x80, 0xFF, 0xFF, 0xFF, 0x03, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 
  0xFE, 0x3F, 0x00, 0x00, 0x00, 0x04, 0xFE, 0x3F, 0x00, 0x00, 0x00, 0x04, 
  0xFE, 0x3F, 0x00, 0x00, 0x00, 0x04, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x04, 
  0xFF, 0x3F, 0x00, 0x00, 0x00, 0x04, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x04, 
  0xFF, 0x3F, 0x00, 0x00, 0x00, 0x04, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x04, 
  0xFF, 0x3F, 0x00, 0x00, 0x00, 0x04, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x04, 
  0xFF, 0x3F, 0x00, 0x00, 0x00, 0x04, 0xFE, 0x3F, 0x00, 0x00, 0x00, 0x04, 
  0xFE, 0x3F, 0x00, 0x00, 0x00, 0x04, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 
  0x80, 0xC1, 0xFF, 0xFF, 0xFF, 0x07, };