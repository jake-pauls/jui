#include "jui/transient.h"

#include "jui/core.h"
#include "jui/core_memory.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <stdlib.h>

#define MAXIMUM_DRAW_LIST_SIZE_PER_FRAME (1000)
// TODO(jake): Implement byte helpers - (~8kB each)
#define MAXIMUM_FRAME_ARENA_SIZE_BYTES (8192)
#define MAXIMUM_APP_ARENA_SIZE_BYTES (8192)

#define DEFAULT_WIDGET_ID (0)

/* Forward Declarations */

typedef struct jui_draw_command_parameters jui_draw_command_parameters;
void jui_sdl_draw_impl(const jui_draw_command_parameters* Parameters);

/* Rendering */

typedef enum jui_draw_command_parameters_type
{
  DrawCommandParameters_DrawRect,
  DrawCommandParameters_DrawText
} jui_draw_command_parameters_type;

struct jui_draw_command_parameters
{
  void* Data;
  jui_draw_command_parameters_type Type;
};

typedef struct jui_draw_list jui_draw_list;
struct jui_draw_list
{
  jui_draw_command_parameters* Array[MAXIMUM_DRAW_LIST_SIZE_PER_FRAME];
  u32 Count;
};

typedef struct jui_sdl_draw_rect_params jui_sdl_draw_rect_params;
struct jui_sdl_draw_rect_params
{
  jui_rect Rect;
  jui_color Color;
};

typedef struct jui_sdl_draw_text_params jui_sdl_draw_text_params;
struct jui_sdl_draw_text_params
{
  const char* Text;
  jui_v2 Position;
};

/* Interactions */

typedef enum jui_box_flags
{
  JuiBoxFlags_None = 0 << 0,
  JuiBoxFlags_Clickable = 1 << 0,

  JuiBoxFlags_Active = 1 << 1,
  JuiBoxFlags_Hot = 1 << 2
} jui_box_flags;

typedef struct jui_box jui_box;
struct jui_box
{
  f32 X, Y, W, H;
  jui_id Id;
  jui_box_flags BoxFlags;
};

typedef enum jui_interaction_status
{
  JuiInteractionStatus_Clicked = 1 << 0,
  JuiInteractionStatus_Active = 1 << 1,
  JuiInteractionStatus_Hot = 1 << 2,
} jui_interaction_status;

/* State */

typedef struct jui_state jui_state;
struct jui_state
{
  f32 MouseX;
  f32 MouseY;

  bool MouseDown;
  bool MousePressed;
  bool MouseReleased;
  bool MouseClicked;

  arena AppArena;
  arena FrameArena;

  jui_draw_list DrawList;

  jui_id HotItemId;
  jui_id ActiveItemId;
};

typedef struct jui_sdl_state jui_sdl_state;
struct jui_sdl_state
{
  SDL_Renderer* Renderer;
};

static jui_state State = {.MouseX = 0,
                          .MouseY = 0,
                          .MouseDown = false,
                          .MousePressed = false,
                          .MouseReleased = false,
                          .MouseClicked = false,
                          .AppArena = {.Base = NULL, .Position = 0},
                          .FrameArena = {.Base = NULL, .Position = 0},
                          .DrawList = {.Array = {0}, .Count = 0},
                          .HotItemId = DEFAULT_WIDGET_ID,
                          .ActiveItemId = DEFAULT_WIDGET_ID};

static jui_sdl_state SDLState = {.Renderer = NULL};

/* Platform (SDL) */

void jui_sdl_consume_event(const SDL_Event* Event)
{
  switch (Event->type)
  {
  case SDL_EVENT_MOUSE_MOTION:
    State.MouseX = Event->motion.x;
    State.MouseY = Event->motion.y;
    break;
  case SDL_EVENT_MOUSE_BUTTON_DOWN:
    if (Event->button.button == SDL_BUTTON_LEFT)
    {
      State.MouseDown = true;
      State.MousePressed = true;
      State.MouseReleased = false;
    }
    break;
  case SDL_EVENT_MOUSE_BUTTON_UP:
    if (Event->button.button == SDL_BUTTON_LEFT)
    {
      State.MouseDown = false;
      State.MousePressed = false;
      State.MouseReleased = true;
    }
    break;
  default:;
  }
}

/* Lifecycle (General) */

static void jui_internal_init(void)
{
  // Allocate the frame-by-frame memory arena.
  arena_alloc(&State.AppArena, MAXIMUM_APP_ARENA_SIZE_BYTES);
  arena_alloc(&State.FrameArena, MAXIMUM_FRAME_ARENA_SIZE_BYTES);
}

void jui_start_frame(void)
{
  // At the start of each frame clear the frame-by-frame memory arena.
  arena_clear(&State.FrameArena);

  // Reset the mouse pressed/released state every time an event is consumed.
  State.MousePressed = false;
  State.MouseReleased = false;

  // Reset the draw list every frame.
  State.DrawList.Count = 0;
}

void jui_render(void)
{
  for (u32 I = 0; I < State.DrawList.Count; ++I)
  {
    const jui_draw_command_parameters* const Parameters = State.DrawList.Array[I];
    jui_sdl_draw_impl(Parameters);
  }
}

void jui_teardown(void)
{
  arena_clear(&State.AppArena);
  arena_free(&State.AppArena);
  arena_clear(&State.FrameArena);
  arena_free(&State.FrameArena);
}

/* Renderer (SDL) */

void jui_sdl_init(SDL_Renderer* Renderer)
{
  SDLState.Renderer = Renderer;
  jui_internal_init();
}

static void jui_sdl_draw_rect_impl(SDL_Renderer* Renderer, const jui_rect Rect, const jui_color Color)
{
  SDL_FRect FRect;
  FRect.x = Rect.X;
  FRect.y = Rect.Y;
  FRect.w = Rect.W;
  FRect.h = Rect.H;
  SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(Renderer, Color.R, Color.G, Color.B, Color.A);
  SDL_RenderFillRect(Renderer, &FRect);
}

static void jui_sdl_draw_text_impl(SDL_Renderer* Renderer, const char* Text, const jui_v2 Position)
{
  SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
  SDL_RenderDebugText(Renderer, Position.X, Position.Y, Text);
}

void jui_sdl_draw_impl(const jui_draw_command_parameters* const Parameters)
{
  switch (Parameters->Type)
  {
  case DrawCommandParameters_DrawRect:
  {
    const jui_sdl_draw_rect_params* DrawRectParams = Parameters->Data;
    jui_sdl_draw_rect_impl(SDLState.Renderer, DrawRectParams->Rect, DrawRectParams->Color);
    break;
  }
  case DrawCommandParameters_DrawText:
  {
    const jui_sdl_draw_text_params* DrawTextParams = Parameters->Data;
    jui_sdl_draw_text_impl(SDLState.Renderer, DrawTextParams->Text, DrawTextParams->Position);
    break;
  }
  }
}

static void jui_create_draw_rect_command(const jui_rect Rect, const jui_color Color)
{
  jui_sdl_draw_rect_params* DrawRectParams = PUSH_STRUCT_ZERO(&State.FrameArena, jui_sdl_draw_rect_params);

  DrawRectParams->Rect.X = Rect.X;
  DrawRectParams->Rect.Y = Rect.Y;
  DrawRectParams->Rect.W = Rect.W;
  DrawRectParams->Rect.H = Rect.H;

  DrawRectParams->Color.R = Color.R;
  DrawRectParams->Color.G = Color.G;
  DrawRectParams->Color.B = Color.B;
  DrawRectParams->Color.A = Color.A;

  jui_draw_command_parameters* CommandParams = PUSH_STRUCT_ZERO(&State.FrameArena, jui_draw_command_parameters);
  CommandParams->Data = DrawRectParams;
  CommandParams->Type = DrawCommandParameters_DrawRect;

  State.DrawList.Array[State.DrawList.Count++] = CommandParams;
}

static void jui_create_draw_text_command(const jui_v2 Position, const jui_str* Text)
{
  jui_sdl_draw_text_params* DrawTextParams = PUSH_STRUCT_ZERO(&State.FrameArena, jui_sdl_draw_text_params);

  const char* TextCString = str_to_cstr(&State.FrameArena, Text);

  DrawTextParams->Text = TextCString;
  DrawTextParams->Position.X = Position.X + 2;
  DrawTextParams->Position.Y = Position.Y + 2;

  jui_draw_command_parameters* CommandParams = PUSH_STRUCT_ZERO(&State.FrameArena, jui_draw_command_parameters);
  CommandParams->Data = DrawTextParams;
  CommandParams->Type = DrawCommandParameters_DrawText;

  State.DrawList.Array[State.DrawList.Count++] = CommandParams;
}

/* API */

/// \brief Simple hit test function for a box defined with an \p X and \p Y coordinate as well
///        as a provided width and height that is performed against the global state.
/// \param X The x coordinate used for the hit test.
/// \param Y The y coordinate used for the hit test.
/// \param W The width used for the hit test.
/// \param H The height used for the hit test.
/// \returns <c>true</c> if the hit test passed for the global state, otherwise <c>false</c>.
static bool jui_hit_test(const f32 X, const f32 Y, const f32 W, const f32 H)
{
  return !(State.MouseX < X || State.MouseY < Y || State.MouseX >= X + W || State.MouseY >= Y + H);
}

static bool jui_is_box_clickable(const jui_box_flags BoxFlags)
{
  return BoxFlags & JuiBoxFlags_Clickable;
}
static bool jui_is_box_hot(const jui_box_flags BoxFlags)
{
  return BoxFlags & JuiBoxFlags_Hot;
}
static bool jui_is_box_active(const jui_box_flags BoxFlags)
{
  return BoxFlags & JuiBoxFlags_Active;
}

static bool jui_is_clicked(const jui_interaction_status Interaction)
{
  return Interaction & JuiInteractionStatus_Clicked;
}
static bool jui_is_active(const jui_interaction_status Interaction)
{
  return Interaction & JuiInteractionStatus_Active;
}
static bool jui_is_hot(const jui_interaction_status Interaction)
{
  return Interaction & JuiInteractionStatus_Hot;
}

static void jui_box_submit_draw_command(const jui_rect Rect, const jui_color Color, const jui_interaction_status Interaction)
{
  // Draw main rect.
  jui_create_draw_rect_command(Rect, Color);

  // Draw overlay for active or hover effect.
  if (jui_is_active(Interaction))
  {
    // 12% opacity for active colors (255 * 0.12 ~= 31).
    const jui_color TwelvePercentWhite = {.R = 255, .G = 255, .B = 255, .A = 31};
    jui_create_draw_rect_command(Rect, TwelvePercentWhite);
  }
  else if (jui_is_hot(Interaction))
  {
    // 8% opacity for hover colors (255 * 0.08 ~= 204).
    const jui_color EightPercentWhite = {.R = 255, .G = 255, .B = 255, .A = 204};
    jui_create_draw_rect_command(Rect, EightPercentWhite);
  }
}

static void jui_text_submit_draw_command(const jui_v2 Position, const jui_str* Text)
{
  jui_create_draw_text_command(Position, Text);
}

// TODO(jake): Ideally when we have layouts there's no rect information here?
static jui_box* jui_box_create(arena* Arena, const jui_rect Rect, const jui_id Id, const jui_box_flags InitialBoxFlags)
{
  jui_box* Box = PUSH_STRUCT(Arena, jui_box);

  Box->X = Rect.X;
  Box->Y = Rect.Y;
  Box->W = Rect.W;
  Box->H = Rect.H;
  Box->Id = Id;
  // TODO(jake): May need to get any persisted flags for this box
  Box->BoxFlags = InitialBoxFlags;

  // Prevent doing any clickable logic if the box isn't clickable.
  if (jui_is_box_clickable(Box->BoxFlags))
  {
    // The button is hot when it passes the hit test.
    if (jui_hit_test(Box->X, Box->Y, Box->W, Box->H))
    {
      Box->BoxFlags |= JuiBoxFlags_Hot;
      State.HotItemId = Box->Id;
    }

    // The button is in it's "active" state when it's hot and the mouse is down.
    if ((jui_is_box_hot(Box->BoxFlags) && State.MouseDown && State.ActiveItemId == DEFAULT_WIDGET_ID) || State.ActiveItemId == Box->Id)
    {
      Box->BoxFlags |= JuiBoxFlags_Active;
      State.ActiveItemId = Box->Id;
    }
  }

  return Box;
}

static jui_interaction_status jui_box_process_interaction(const jui_box* Box)
{
  jui_interaction_status Interaction = {0};

  // The button is pressed when it's active, hot, and the mouse is released this frame.
  // This is a "completed" interaction, so reset the active item as well.
  if (Box->BoxFlags & JuiBoxFlags_Active && State.MouseReleased)
  {
    if (Box->BoxFlags & JuiBoxFlags_Hot)
    {
      Interaction |= JuiInteractionStatus_Clicked;
    }

    // Ensures that if the active item is no longer hot the default widget resets and the interaction
    // isn't clicked. This is equivalent to the mouse leaving an active item mid-interaction (e.g., the
    // mouse leaving the button mid-press) and is often treated as a "cancellation" of the user's interaction.
    State.ActiveItemId = DEFAULT_WIDGET_ID;
  }

  if (jui_is_box_active(Box->BoxFlags))
  {
    Interaction |= JuiInteractionStatus_Active;
  }
  if (jui_is_box_hot(Box->BoxFlags))
  {
    Interaction |= JuiInteractionStatus_Hot;
  }

  return Interaction;
}

/* "Widgets" - "These should be made of Lego blocks." */

void jui_label(const char* Text, const f32 X, const f32 Y)
{
  // TODO(jake): Needs to check parent container position.
  const jui_v2 LabelPosition = {.X = X, .Y = Y};
  const jui_str TextString = str_create_from_cstr(Text);

  // Check if an extra (invisible) identifier is appended to the string.
  const char* IdentifierIndex = strstr(Text, "###");
  if (IdentifierIndex != NULL)
  {
    const size_t SubstringIndex = IdentifierIndex - Text;
    const jui_str TextWithoutId = str_substring(0, SubstringIndex, &TextString);
    jui_text_submit_draw_command(LabelPosition, &TextWithoutId);
  }
  else
  {
    jui_text_submit_draw_command(LabelPosition, &TextString);
  }
}

void jui_panel(const char* Text, const f32 W, const f32 H)
{
  // TODO(jake): Remove fixed buffer?
  char TextWithExtraIdBuffer[200];
  const size_t TextLength = strlen(Text) + 1;

  const f32 GripHeight = H / 8.0f;

  // The top area is a clickable box (panel grip).
  {
    strncpy(TextWithExtraIdBuffer, Text, TextLength);
    strcat(TextWithExtraIdBuffer, "###PanelGrip");
    const jui_id PanelGripId = hash_djb2((uchar*)TextWithExtraIdBuffer);

    const jui_rect GripRect = {.X = 0.0f, .Y = 0.0f, .W = W, .H = GripHeight};
    // TODO(jake): This Id is cooked. Figure out how to solve widgets with multiple pieces.
    // TODO(jake): If the boxes need to persist this cannot use a FrameArena.
    const jui_box* GripBox = jui_box_create(&State.FrameArena, GripRect, PanelGripId, JuiBoxFlags_Clickable);
    const jui_interaction_status GripBoxInteraction = jui_box_process_interaction(GripBox);
    const jui_color GripBoxColor = {.R = 125, .G = 50, .B = 200, .A = 255};
    jui_box_submit_draw_command(GripRect, GripBoxColor, GripBoxInteraction);
  }

  // The grip should contain the window title.
  {
    strncpy(TextWithExtraIdBuffer, Text, TextLength);
    strcat(TextWithExtraIdBuffer, "###PanelGripText");
    jui_label(TextWithExtraIdBuffer, 0.0f, 0.0f);
  }

  // For the moment the content area is just a non-clickable box.
  {
    strncpy(TextWithExtraIdBuffer, Text, TextLength);
    strcat(TextWithExtraIdBuffer, "###PanelContentArea");
    const jui_id PanelContentId = hash_djb2((uchar*)TextWithExtraIdBuffer);

    const jui_rect ContentRect = {.X = 0.0f, .Y = GripHeight, .W = W, .H = H};
    // TODO(jake): If the boxes need to persist this cannot use a FrameArena.
    const jui_box* ContentBox = jui_box_create(&State.FrameArena, ContentRect, PanelContentId, JuiBoxFlags_None);
    const jui_interaction_status ContentBoxInteraction = jui_box_process_interaction(ContentBox);
    const jui_color ContentBoxColor = {.R = 127, .G = 127, .B = 127, .A = 255};
    jui_box_submit_draw_command(ContentRect, ContentBoxColor, ContentBoxInteraction);
  }
}

bool jui_button(const char* Text, const f32 X, const f32 Y, const f32 W, const f32 H)
{
  const jui_id Id = hash_djb2((uchar*)Text);
  const jui_str TextString = str_create_from_cstr(Text);

  // TODO(jake): Needs to check parent container position.
  const jui_rect Rect = {.X = X, .Y = Y, .W = W, .H = H};
  // For the moment this button is just a clickable box.
  // TODO(jake): If the boxes need to persist this cannot use a FrameArena.
  const jui_box* Box = jui_box_create(&State.FrameArena, Rect, Id, JuiBoxFlags_Clickable);
  const jui_interaction_status Interaction = jui_box_process_interaction(Box);

  const jui_color Color = {.R = 98, .G = 0, .B = 238, .A = 255};
  jui_box_submit_draw_command(Rect, Color, Interaction);

  // TODO(jake): Needs to check parent container position.
  const jui_v2 TextPosition = {.X = X, .Y = Y + H / 2.0f};
  jui_text_submit_draw_command(TextPosition, &TextString);

  return jui_is_clicked(Interaction);
}
