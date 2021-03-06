/* See LICENSE file for copyright and license details. */

/* appearance */
static const char font[]        = "monospace:size=9";
static const char* normbgcolor  = "#222222";
static const char* normfgcolor  = "#FFFFFF";
static const char* selbgcolor   = "#000000";
static const char* selfgcolor   = "#FF0000";
static const char* urgbgcolor   = "#111111";
static const char* urgfgcolor   = "#cc0000";
static const char before[]      = "<";
static const char after[]       = ">";
static const char titletrim[]   = "...";
static const int  tabwidth      = 200;
static const Bool foreground    = True;
static       Bool urgentswitch  = False;

/*
 * Where to place a new tab when it is opened. When npisrelative is True,
 * then the current position is changed + newposition. If npisrelative
 * is False, then newposition is an absolute position.
 */
static int  newposition   = 0;
static Bool npisrelative  = False;

#define SETPROP(p) { \
        .v = (char *[]){ "/bin/sh", "-c", \
                "prop=\"`xwininfo -children -id $1 | grep '^     0x' |" \
                "sed -e's@^ *\\(0x[0-9a-f]*\\) \"\\([^\"]*\\)\".*@\\1 \\2@' |" \
                "xargs -0 printf %b | dmenu -l 10 -w $1`\" &&" \
                "xprop -id $1 -f $0 8s -set $0 \"$prop\"", \
                p, winid, NULL \
        } \
}

#define MODKEY ControlMask
static Key keys[] = {
	/* modifier             key           function     argument */
	{ MODKEY|Mod1Mask,      XK_Return,    focusonce,   { 0 } },
	{ MODKEY|Mod1Mask,      XK_Return,    spawn,       { 0 } },

	{ MODKEY|Mod1Mask,      XK_j,         rotate,      { .i = +1 } },
	{ MODKEY|Mod1Mask,      XK_k,         rotate,      { .i = -1 } },
	{ MODKEY|Mod1Mask,      XK_h,         movetab,     { .i = -1 } },
	{ MODKEY|Mod1Mask,      XK_l,         movetab,     { .i = +1 } },

	{ MODKEY|Mod1Mask,      XK_c,         spawn,       SETPROP("_TABBED_SELECT_TAB") },
	{ MODKEY|Mod1Mask,      XK_d,         killclient,  { 0 } },

	{ MODKEY,               XK_Alt_L,     showbar,    { .i = 1 } },
	{ Mod1Mask,             XK_Control_L, showbar,    { .i = 1 } },
};

static Key keyreleases[] = {
	/* modifier             key           function     argument */    
	{ MODKEY|Mod1Mask,      XK_Alt_L,     showbar,     { .i = 0 } },
	{ MODKEY|Mod1Mask,      XK_Control_L, showbar,     { .i = 0 } },
};
