/*
 * getflag.c
 *
 */

#include <string.h>
#include <stdio.h>

int flagind = 1;
int flagflag = '?';
const char *flagarg = NULL;

static void
reverse(char **buf, int pos1, int pos2)
{
     char **buf1;
     char **buf2;
     char *tmp;

     buf1 = buf + pos1;
     buf2 = buf + pos2 - 1;

     while(buf1 < buf2)
     {
          tmp = *buf1;
          *buf1 = *buf2;
          *buf2 = tmp;

          ++buf1;
          --buf2;
     }
}


int
getflag(int argc, char **argv, const char *flags)
{
     int argstart, flagstart, c;
     const char *flagp;
     
     c = -1;
     argstart = flagind;
     flagstart = argc;
     
     /* skip over non-flags */
     while(flagind < argc && argv[flagind][0] != '-')
     {
          ++flagind;
     }
     if(flagind == argc)
     {
          /* no more args */
          flagind = argstart;
          return c;
     }
     /* we have an arg to work with */
     do {
          if(argv[flagind][1] == '-')
          {
               /* stop parsing at '--' flag */
               break;
          }
          c = flagflag = argv[flagind][1];
          flagstart = flagind;
          if(c == ':' || c == '\0')
          {
               /* this is an illegal flag */
               c = '?';
               break;
          }
          flagp = strchr(flags, c);
          if (flagp == NULL)
          {
               /* this is an unknown flag */
               c = '?';
               break;
          }
          if(flagp[1] != ':')
          {
               /* this is a simple flag */
               break;
          }

          /* flag with arg */
          if(argv[flagind][2] != '\0')
          {
               /* flag-arg in same argv[] */
               flagarg = argv[flagind] + 2;
               break;
          }
          /* flag-arg is in the next argv[] */
          if(flagind + 1 == argc)
          {
               /* auahh, no flag-arg */
               flagstart = argstart;
               c = '?';
               break;
          }
          flagarg = argv[++flagind];
     } while (0);
     /* skip to next arg */
     ++flagind;

     if(flagstart < flagind &&
        argstart < flagstart)
     {
          /* we have found some args */
          /* we have also skipped over some non-args */
#if 0
          printf("argstart %d, flagstart %d, flagind %d\n",
                 argstart, flagstart, flagind);
#endif
          /* shuffle the non-flag arg to the end of argv */
          reverse(argv, argstart, flagstart);
          reverse(argv, flagstart, flagind);
          reverse(argv, argstart, flagind);
     }
     flagind = argstart + flagind - flagstart;
     
     return c;
}

#ifdef GETFLAG_TEST
int
main(int argc, char *argv[])
{
     int c, a1, a2, a3;
     const char *a4;

     a1 = a2 = a3 = 0;
     a4 = NULL;

     while((c = getflag(argc, argv, "abc:")) != -1)
     {
          switch (c) {
          case 'a':
               a1 = 1;
               break;
          case 'b':
               a2 = 1;
               break;
          case 'c':
               a3 = 1;
               a4 = flagarg;
               break;
          default:
               printf("got unexpected flag \'%c\'\n", c);
          }
          for(c = 0; c < argc; ++c)
          {
               if(c == flagind)
               {
                    printf("-----------------------\n");
               }
               printf("argv[%d] = \"%s\"\n", c, argv[c]);
          }
     }
     printf("a1 %d, a2 %d, a3 %d, a4 \"%s\"\n", a1, a2, a3, a4);

     return 0;
}
#endif
