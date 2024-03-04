#ifndef GBA_H
#define GBA_H

// Typedefs
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;

// Display constants
#define SCREENHEIGHT 160 // Height of the GBA display
#define SCREENWIDTH  240 // Width of the GBA display

// Finds 1D position from 2D coordinates
#define OFFSET(x, y, width) ((y) * (width) + (x))

// Display control register
#define REG_DISPCTL (*(volatile unsigned short *)0x4000000)

//Pointer to the start of video memory
extern volatile unsigned short* videoBuffer;

// Bits for display control register
#define MODE(x) ((x) & 7) // Sets GBA video mode (REG_DISPCTL)
#define MODE3 MODE(3) // Enables Mode 3 (REG_DISPCTL)
#define BG_ENABLE(x) (1 << (8 + ((x) % 4))) // Enables specified background (REG_DISPCTL)
#define BG2_ENABLE (BG_ENABLE(2)) // Enables BG2 (REG_DISPCTL)

// Read-only, holds which scanline is being drawn
#define REG_VCOUNT (*(volatile unsigned short *)0x4000006)

// Checks for collision between two rectangles
int collision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);

// Waits until scanline has just become 160
void waitForVBlank();

// Color
#define RGB(R, G, B) (((R) & 31) | ((G) & 31) << 5 | ((B) & 31) << 10)
#define BLACK   RGB(0, 0, 0)
#define WHITE   RGB(31, 31, 31)
#define GRAY    RGB(15, 15, 15)
#define RED     RGB(31, 0, 0)
#define GREEN   RGB(0, 31, 0)
#define BLUE    RGB(0, 0, 31)
#define CYAN    RGB(0, 31, 31)
#define MAGENTA RGB(31, 0, 31)
#define YELLOW  RGB(31, 31, 0)
#define BROWN   RGB(20, 10, 5)
// More Shades
#define LAVENDER RGB(18,19,27)
#define ROGUE RGB(22,6,13)
#define VIORED RGB(28,8,16)
#define HOTPINK RGB(31,13,21)
#define LAVPINK RGB(31,20,26)
#define TURQUOISE RGB(5,26,25)
#define TEAL RGB(0,16,16)

// Mode 3 Drawing Functions
#define setPixel(x, y, color) (videoBuffer[OFFSET(x, y, SCREENWIDTH)] = color)
void drawRectangle(int x, int y, int width, int height, volatile unsigned short color);
void fillScreen(volatile unsigned short color);
void drawChar(int x, int y, char ch, unsigned short color);
void drawString(int x, int y, char *str, unsigned short color);

// Buttons
#define REG_BUTTONS (*(volatile unsigned short *)0x04000130) // Buttons down register
#define BUTTON_A         (1<<0) // Mask for button A (REG_BUTTONS or REG_KEYCNT)
#define BUTTON_B         (1<<1) // Mask for button B (REG_BUTTONS or REG_KEYCNT)
#define BUTTON_SELECT    (1<<2) // Mask for button SELECT (REG_BUTTONS or REG_KEYCNT)
#define BUTTON_START     (1<<3) // Mask for button START (REG_BUTTONS or REG_KEYCNT)
#define BUTTON_RIGHT     (1<<4) // Mask for RIGHT button (REG_BUTTONS or REG_KEYCNT)
#define BUTTON_LEFT      (1<<5) // Mask for LEFT button (REG_BUTTONS or REG_KEYCNT)
#define BUTTON_UP        (1<<6) // Mask for UP button (REG_BUTTONS or REG_KEYCNT)
#define BUTTON_DOWN      (1<<7) // Mask for DOWN button (REG_BUTTONS or REG_KEYCNT)
#define BUTTON_RSHOULDER (1<<8) // Mask for RIGHT SHOULDER button (REG_BUTTONS or REG_KEYCNT)
#define BUTTON_LSHOULDER (1<<9) // Mask for LEFT SHOULDER button (REG_BUTTONS or REG_KEYCNT)

extern unsigned short oldButtons; // Keeps track of buttons pressed in previous frame
extern unsigned short buttons; // Keeps track of buttons pressed in current frame

#define BUTTON_HELD(key) (~buttons & (key)) // Checks if a button is currently pressed
#define BUTTON_PRESSED(key) ((~buttons & (key)) && !(~oldButtons & (key))) // Checks if a button is currently pressed and wasn't in the previous frame

// DMA
typedef volatile struct {
    volatile const void *src;
    volatile void *dst;
    volatile unsigned int cnt;
} DMA;

extern DMA *dma; // Address to first DMA register

// DMA bits
#define DMA_DESTINATION_INCREMENT (0 << 21) // Increment destination (move "forwards" in memory)
#define DMA_DESTINATION_DECREMENT (1 << 21) // Decrement destination (move "backwards" in memory)
#define DMA_DESTINATION_FIXED     (2 << 21) // Keep destination fixed
#define DMA_DESTINATION_RESET     (3 << 21) // Reset destination to beginning when repeating

#define DMA_SOURCE_INCREMENT      (0 << 23) // Increment source (move "forwards" in memory)
#define DMA_SOURCE_DECREMENT      (1 << 23) // Decrement source (move "backwards" in memory)
#define DMA_SOURCE_FIXED          (2 << 23) // Keep source fixed

#define DMA_REPEAT (1 << 25) // Repeats every time 

#define DMA_16     (0 << 26) // Copy 2 bytes at a time
#define DMA_32     (1 << 26) // Copy 4 bytes at a time

#define DMA_AT_NOW     (0 << 28) // Start DMA immediately
#define DMA_AT_VBLANK  (1 << 28) // Start DMA at VBlank
#define DMA_AT_HBLANK  (2 << 28) // Start DMA at HBlank
#define DMA_AT_REFRESH (3 << 28) // DMA1 and DMA2, refill FIFO once empty. DMA3, transfer starts at scaline==2 and repeats until scanline==162

#define DMA_IRQ (1 << 30) // Request an interrupt once DMA is finished

#define DMA_ON  (1 << 31) // Enable DMA!!!

// Immediately begins a DMA transfer using parameters
void DMANow(int channel, volatile const void *src, volatile void *dst, unsigned int cnt);

#endif