#pragma once

#include "jui/types.h"

#include <stdbool.h>
#include <stddef.h>

/* Forward Declarations */

struct arena;

/* Core Structures */

/// \brief Represents the unique identifier for a widget in jui.
typedef u64 jui_id;

/// \brief Represents a two-float vector. Used to represent arbitrary vectors.
typedef struct jui_v2 jui_v2;
struct jui_v2
{
  f32 X, Y;
};

/// \brief Represents a three-float vector. Used to represent arbitrary vectors.
typedef struct jui_v3 jui_v3;
struct jui_v3
{
  f32 X, Y, Z;
};

/// \brief Represents a rectangle using floating point values.
typedef struct jui_rect jui_rect;
struct jui_rect
{
  f32 X, Y, W, H;
};

/// \brief Represents a color of four eight-bit unsigned integers.
typedef struct jui_color jui_color;
struct jui_color
{
  u8 R, G, B, A;
};

/// \brief Represents a view of a string with a length.
typedef struct jui_str jui_str;
struct jui_str
{
  const char* Data;
  size_t Length;
};

/* Strings */

/// \brief Checks if the provided string view is empty.
/// \param View The string view to check the length of.
/// \return <c>true</c> if the underlying string view is empty, otherwise <c>false</c>.
/// \remarks This only checks the <c>Length</c> property of the string view, and will <b>not</b> evaluate the underlying pointer.
bool str_is_empty(const jui_str* View);

/// \brief Creates a string view for a provided C string with a length.
/// \param Data The C string underlying the string view.
/// \param Length The length of the string view.
/// \return View of the created string.
jui_str str_create(const char* Data, size_t Length);

/// \brief Creates a string view for a provided C string, infers length using <c>strlen</c>.
/// \param Data The C string underlying the string view.
/// \return View of the created string.
jui_str str_create_from_cstr(const char* Data);

/// \brief Retrieves a view of a substring inside a provided source string view.
/// \param StartIndex The starting index for the substring.
/// \param EndIndex The end index for the substring.
/// \param View The string view to substring.
/// \return View of the substring from the provided view.
/// \remark Will return an empty string view if the <c>EndIndex</c> is greater than the <c>StartIndex</c> or if the underlying view is <c>NULL</c>.
jui_str str_substring(size_t StartIndex, size_t EndIndex, const jui_str* View);

/// \brief Converts a string view to a C string with the null-terminator.
/// \param Arena The memory arena to perform the allocations for the underlying C string in.
/// \param View The view to create a C string from.
char* str_to_cstr(struct arena* Arena, const jui_str* View);

/* Hashing */

/// \brief djb2 hash function for strings. The hash function itself has
///         clever properties that make it well-suited for hashing strings:
///
/// 1. Use of <c>unsigned char</c> ensures that incoming bytes are guaranteed to
///    be in the range 0 <= C <= 255.
///
/// 2. The hash uses 33 as the base (<c>(Hash << 5) + Hash</c>) since it's odd,
///    close to a power of two, and sufficiently mixes the input (as opposed to
///    using a power of two directly).
///
/// \param Str Input string to hash.
/// \remarks <a href="http://www.csegyorku.ca/~oz/hash.html">Reference</a>
u64 hash_djb2(const uchar* Str);
