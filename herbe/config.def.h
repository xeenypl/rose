static const char *background_color = "#000000";
static const char *border_color = "#FFFFFF";
static const char *font_color = "#FFFFFF";
static const char *font_pattern = "monospace:size=10";
static const unsigned line_spacing = 5;
static const unsigned int padding = 4;

static const unsigned int width = 450;
static const unsigned int border_size = 1;
static const unsigned int pos_x = 0;
static const unsigned int pos_y = 0;

enum corners { TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT };
enum corners corner = TOP_RIGHT;

static const unsigned int duration = 10; /* in seconds */

#define DISMISS_BUTTON Button1
#define ACTION_BUTTON Button3
