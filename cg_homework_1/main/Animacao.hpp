#ifndef ANIMACAO_HPP
#define ANIMACAO_HPP

extern int FrameNumber;
extern int speed;
extern int msecs;

extern bool rightArrowPressed;
extern bool leftArrowPressed;
extern bool upArrowPressed;
extern bool downArrowPressed;
extern bool r_key_pressed;
extern bool e_key_pressed;

void init(void);
void display(void);
void anim(int valor);
void arrowKeysDown(int key, int x, int y);
void arrowKeysUp(int key, int x, int y);
void keyboard_callback(unsigned char key, int x, int y);
void keyboard_up_callback(unsigned char key, int x, int y);

#endif
