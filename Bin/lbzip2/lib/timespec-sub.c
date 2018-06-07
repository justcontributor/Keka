/* Subtract two struct timespec values.

   Copyright (C) 2011-2014 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* Written by Paul Eggert.  */

/* Return the difference between two timespec values A and B.  On
   overflow, return an extremal value.  This assumes 0 <= tv_nsec <
   TIMESPEC_RESOLUTION.  */

#include <config.h>
#include "timespec.h"

#include "intprops.h"

struct timespec
timespec_sub (struct timespec a, struct timespec b)
{
  time_t rs = a.tv_sec;
  time_t bs = b.tv_sec;
  int ns = a.tv_nsec - b.tv_nsec;
  int rns = ns;

  if (ns < 0)
    {
      rns = ns + TIMESPEC_RESOLUTION;
      if (rs == TYPE_MINIMUM (time_t))
        {
          if (bs <= 0)
            goto low_overflow;
          bs--;
        }
      else
        rs--;
    }

  if (INT_SUBTRACT_OVERFLOW (rs, bs))
    {
      if (rs < 0)
        {
        low_overflow:
          rs = TYPE_MINIMUM (time_t);
          rns = 0;
        }
      else
        {
          rs = TYPE_MAXIMUM (time_t);
          rns = TIMESPEC_RESOLUTION - 1;
        }
    }
  else
    rs -= bs;

  return make_timespec (rs, rns);
}
