/**
 *
 * strftime.c
 *
 * implements the ansi c function strftime()
 *
 * written 6 september 1989 by jim nutt
 * released into the public domain by jim nutt
 *
 * modified 21-Oct-89 by Rob Duff
 *
 * modified 08-Dec-04 by Tobi Oetiker (added %V)
**/

#ifndef STRFTIME_H_INCLUDED
  #define STRFTIME_H_INCLUDED

size_t strftime_(char *s, size_t maxs, const char *f, const struct tm *t);

#endif // !STRFTIME_H_INCLUDED
