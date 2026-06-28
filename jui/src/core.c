#include "jui/core.h"
#include "jui/core_memory.h"

#include <stddef.h>
#include <string.h>

char* str_to_cstr(arena* Arena, const jui_str* View)
{
  char* AllocatedText = PUSH_ARRAY_ZERO(Arena, char, View->Length + 1);
  memcpy(&AllocatedText[0], View->Data, View->Length + 1);
  AllocatedText[View->Length] = '\0';
  return AllocatedText;
}

bool str_is_empty(const jui_str* View)
{
  return View->Length == 0;
}

jui_str str_create(const char* Data, const size_t Length)
{
  const jui_str View = {.Data = Data, .Length = Length};
  return View;
}

jui_str str_substring(const size_t StartIndex, const size_t EndIndex, const jui_str* View)
{
  // Return an empty string if the substring source or indices are invalid.
  if (View == NULL || EndIndex < StartIndex)
  {
    return str_create(NULL, 0);
  }

  return str_create(&View->Data[StartIndex], EndIndex - StartIndex);
}

u64 hash_djb2(const uchar* Str)
{
  u64 Hash = 5381;
  i32 C;

  while ((C = *Str++))
  {
    Hash = (Hash << 5) + Hash + C; /* Hash * 33 + C */
  }

  return Hash;
}
