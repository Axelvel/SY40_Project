#ifndef UTILS_H
#define UTILS_H

int initsem(key_t semkey);

void P(int semnum);

void V(int semnum);

void startProduction(int mode);

#endif
