#ifndef STRINGS_H
#define STRINGS_H

void int_to_ascii(int n, char str[]);
void hex_to_ascii(int n, char str[]);
void reverse(char s[]);
int strlen(char s[]);
void backspace(char s[]);
void append(char s[], char n);
int strcmp(char s1[], char s2[]);
char* strcpy(char* dest, const char* src);
int strncmp(const char* s1, const char* s2, int n);
int stricmp(const char* s1, const char* s2);
char* strcat(char* dest, const char* src);

#endif
