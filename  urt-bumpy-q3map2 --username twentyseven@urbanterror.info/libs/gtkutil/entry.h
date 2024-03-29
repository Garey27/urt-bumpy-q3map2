
#if !defined(INCLUDED_GTKUTIL_ENTRY_H)
#define INCLUDED_GTKUTIL_ENTRY_H

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtkentry.h>

inline void entry_set_string(GtkEntry* entry, const char* string)
{
  gtk_entry_set_text(entry, string);
}

inline void entry_set_int(GtkEntry* entry, int i)
{
  char buf[32];
  sprintf(buf, "%d", i);
  entry_set_string(entry, buf);
}

inline void entry_set_float(GtkEntry* entry, float f)
{
  char buf[32];
  sprintf(buf, "%g", f);
  entry_set_string(entry, buf);
}

inline const char* entry_get_string(GtkEntry* entry)
{
  return gtk_entry_get_text(entry);
}

inline int entry_get_int(GtkEntry* entry)
{
  return atoi(entry_get_string(entry));
}

inline double entry_get_float(GtkEntry* entry)
{
  return atof(entry_get_string(entry));
}

#endif
