
#if !defined(INCLUDED_GTKUTIL_ACCELERATOR_H)
#define INCLUDED_GTKUTIL_ACCELERATOR_H

#include <gdk/gdktypes.h>
#include <gdk/gdkkeysyms.h>

#include "generic/callback.h"


struct Accelerator
{
  Accelerator(guint _key)
    : key(_key), modifiers((GdkModifierType)0)
  {
  }
  Accelerator(guint _key, GdkModifierType _modifiers)
    : key(_key), modifiers(_modifiers)
  {
  }
  bool operator<(const Accelerator& other) const
  {
    return key < other.key || (!(other.key < key) && modifiers < other.modifiers);
  }
  guint key;
  GdkModifierType modifiers;
};

inline Accelerator accelerator_null()
{
  return Accelerator(0, (GdkModifierType)0);
}


void keydown_accelerators_add(Accelerator accelerator, const Callback& callback);
void keydown_accelerators_remove(Accelerator accelerator);
void keyup_accelerators_add(Accelerator accelerator, const Callback& callback);
void keyup_accelerators_remove(Accelerator accelerator);

typedef struct _GtkWidget GtkWidget;
typedef struct _GtkWindow GtkWindow;
void global_accel_connect_window(GtkWindow* window);
void global_accel_disconnect_window(GtkWindow* window);

void GlobalPressedKeys_releaseAll();

typedef struct _GtkAccelGroup GtkAccelGroup;
extern GtkAccelGroup* global_accel;
void global_accel_init();
void global_accel_destroy();

GClosure* global_accel_group_find(Accelerator accelerator);

void command_connect_accelerator(const Accelerator& accelerator, const Callback& callback);
void command_disconnect_accelerator(const Accelerator& accelerator, const Callback& callback);


class Command
{
public:
  Callback m_callback;
  const Accelerator& m_accelerator;
  Command(const Callback& callback, const Accelerator& accelerator) : m_callback(callback), m_accelerator(accelerator)
  {
  }
};

class Toggle
{
public:
  Command m_command;
  BoolExportCallback m_exportCallback;
  Toggle(const Callback& callback, const Accelerator& accelerator, const BoolExportCallback& exportCallback) : m_command(callback, accelerator), m_exportCallback(exportCallback)
  {
  }
};

class KeyEvent
{
public:
  const Accelerator& m_accelerator;
  Callback m_keyDown;
  Callback m_keyUp;
  KeyEvent(const Accelerator& accelerator, const Callback& keyDown, const Callback& keyUp) : m_accelerator(accelerator), m_keyDown(keyDown), m_keyUp(keyUp)
  {
  }
};



struct PressedButtons;
typedef struct _GtkWidget GtkWidget;
void PressedButtons_connect(PressedButtons& pressedButtons, GtkWidget* widget);

extern PressedButtons g_pressedButtons;

#endif
