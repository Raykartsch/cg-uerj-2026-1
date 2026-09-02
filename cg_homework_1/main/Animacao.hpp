#pragma once

extern int FrameNumber;
extern int speed;
extern int msecs;

extern bool r_key_pressed;
extern bool e_key_pressed;

void anim(int valor);
void display();
void keyboard_callback(unsigned char key, int x, int y);
void keyboard_up_callback(unsigned char key, int x, int y);
void arrowKeysDown(int key, int x, int y);
void arrowKeysUp(int key, int x, int y);
