#ifndef ALREADY_INCLUDED_GETFLAG_H
#define ALREADY_INCLUDED_GETFLAG_H
/*
 * getflag.h
 *
 */

extern int flagind;
extern int flagflag;
extern const char *flagarg;

int
getflag(int argc, char *argv[], const char *flags);

#endif
