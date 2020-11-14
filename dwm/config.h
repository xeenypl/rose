/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const unsigned int baralpha = 0xe6;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
    /*               fg         bg         border   */
    [SchemeNorm] = { "#FFFFFF", "#000000", "#FFFFFF" },
    [SchemeSel]  = { "#FF0000", "#000000", "#FF0000" },
};
static const unsigned int alphas[][3]      = {
    /*               fg      bg        border     */
    [SchemeNorm] = { OPAQUE, baralpha, borderalpha },
    [SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "I" ,  "II",   "III", "IV", "V",  "VI", 
                              "VII", "VIII", "IX",  "X",  "XI", "XII" };

static const Rule rules[] = {
    /* xprop(1):
     *    WM_CLASS(STRING) = instance, class
     *    WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    { "Gimp",     NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#define L_WIDTH(X)                ((X)->w + 2 * (X)->bw + 2 * gappx)
#define L_HEIGHT(X)               ((X)->h + 2 * (X)->bw + 2 * gappx)

// orginall tile
void
rstack(Monitor *m)
{
    unsigned int i, n, h, mw, my, ty;
    Client *c;

    for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
    if (n == 0)
        return;

    if (n > m->nmaster)
        mw = m->nmaster ? m->ww * m->mfact : 0;
    else
        mw = m->ww;
    for (i = my = ty = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
        if (i < m->nmaster) {
            h = (m->wh - my) / (MIN(n, m->nmaster) - i);
            resize(c, m->wx, m->wy + my, mw - (2*c->bw), h - (2*c->bw), 0);
            if (my + HEIGHT(c) < m->wh)
                my += HEIGHT(c);
        } else {
            h = (m->wh - ty) / (n - i);
            resize(c, m->wx + mw, m->wy + ty, m->ww - mw - (2*c->bw), h - (2*c->bw), 0);
            if (ty + HEIGHT(c) < m->wh)
                ty += HEIGHT(c);
        }
}

void
lstack(Monitor *m)
{
    unsigned int i, n, h, mw, my, ty;
    Client *c;

    for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
    if (n == 0)
        return;

    if (n > m->nmaster) {
        mw = m->nmaster ? m->ww * m->mfact : 0;
        for (i = my = ty = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
            if (i < m->nmaster) {
                h = (m->wh - my) / (MIN(n, m->nmaster) - i);
                resize(c, m->wx + mw, m->wy + my, mw - (2*c->bw), h - (2*c->bw), 0);
                if (my + HEIGHT(c) < m->wh)
                    my += HEIGHT(c);
            } else {
                h = (m->wh - ty) / (n - i);
                resize(c, m->wx, m->wy + ty, m->ww - mw - (2*c->bw), h - (2*c->bw), 0);
                if (ty + HEIGHT(c) < m->wh)
                    ty += HEIGHT(c);
            }
    } else {
        mw = m->ww;
        for (i = my = ty = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
            h = (m->wh - my) / (MIN(n, m->nmaster) - i);
            resize(c, m->wx, m->wy + my, mw - (2*c->bw), h - (2*c->bw), 0);
            if (my + HEIGHT(c) < m->wh)
                my += HEIGHT(c);
        }
    }
}

static void
bstack(Monitor *m) {
    int w, h, mh, mx, tx, ty, tw;
    unsigned int i, n;
    Client *c;

    for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
    if (n == 0)
        return;
    if (n > m->nmaster) {
        mh = m->nmaster ? m->mfact * m->wh : 0;
        tw = m->ww / (n - m->nmaster);
        ty = m->wy + mh;
    } else {
        mh = m->wh;
        tw = m->ww;
        ty = m->wy;
    }
    for (i = mx = 0, tx = m->wx, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
        if (i < m->nmaster) {
            w = (m->ww - mx) / (MIN(n, m->nmaster) - i);
            resize(c, m->wx + mx, m->wy, w - (2 * c->bw), mh - (2 * c->bw), 0);
            mx += L_WIDTH(c);
        } else {
            h = m->wh - mh;
            resize(c, tx, ty, tw - (2 * c->bw), h - (2 * c->bw), 0);
            if (tw != m->ww)
                tx += L_WIDTH(c);
        }
    }
}

void
centeredmaster(Monitor *m)
{
    unsigned int i, n, h, mw, mx, my, oty, ety, tw;
    Client *c;

    /* count number of clients in the selected monitor */
    for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
    if (n == 0)
        return;

    /* initialize areas */
    mw = m->ww;
    mx = 0;
    my = 0;
    tw = mw;

    if (n > m->nmaster) {
        /* go mfact box in the center if more than nmaster clients */
        mw = m->nmaster ? m->ww * m->mfact : 0;
        tw = m->ww - mw;

        if (n - m->nmaster > 1) {
            /* only one client */
            mx = (m->ww - mw) / 2;
            tw = (m->ww - mw) / 2;
        }
    }

    oty = 0;
    ety = 0;
    for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
    if (i < m->nmaster) {
        /* nmaster clients are stacked vertically, in the center
         * of the screen */
        h = (m->wh - my) / (MIN(n, m->nmaster) - i);
        resize(c, m->wx + mx, m->wy + my, mw - (2*c->bw),
               h - (2*c->bw), 0);
        my += HEIGHT(c);
    } else {
        /* stack clients are stacked vertically */
        if ((i - m->nmaster) % 2 ) {
            h = (m->wh - ety) / ( (1 + n - i) / 2);
            resize(c, m->wx, m->wy + ety, tw - (2*c->bw),
                   h - (2*c->bw), 0);
            ety += HEIGHT(c);
        } else {
            h = (m->wh - oty) / ((1 + n - i) / 2);
            resize(c, m->wx + mx + mw, m->wy + oty,
                   tw - (2*c->bw), h - (2*c->bw), 0);
            oty += HEIGHT(c);
        }
    }
}

void
centeredfloatingmaster(Monitor *m)
{
    unsigned int i, n, w, mh, mw, mx, mxo, my, myo, tx;
    Client *c;

    /* count number of clients in the selected monitor */
    for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
    if (n == 0)
        return;

    /* initialize nmaster area */
    if (n > m->nmaster) {
        /* go mfact box in the center if more than nmaster clients */
        if (m->ww > m->wh) {
            mw = m->nmaster ? m->ww * m->mfact : 0;
            mh = m->nmaster ? m->wh * 0.9 : 0;
        } else {
            mh = m->nmaster ? m->wh * m->mfact : 0;
            mw = m->nmaster ? m->ww * 0.9 : 0;
        }
        mx = mxo = (m->ww - mw) / 2;
        my = myo = (m->wh - mh) / 2;
    } else {
        /* go fullscreen if all clients are in the master area */
        mh = m->wh;
        mw = m->ww;
        mx = mxo = 0;
        my = myo = 0;
    }

    for(i = tx = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
    if (i < m->nmaster) {
        /* nmaster clients are stacked horizontally, in the center
         * of the screen */
        w = (mw + mxo - mx) / (MIN(n, m->nmaster) - i);
        resize(c, m->wx + mx, m->wy + my, w - (2*c->bw),
               mh - (2*c->bw), 0);
        mx += WIDTH(c);
    } else {
        /* stack clients are stacked horizontally */
        w = (m->ww - tx) / (n - i);
        resize(c, m->wx + tx, m->wy, w - (2*c->bw),
               m->wh - (2*c->bw), 0);
        tx += WIDTH(c);
    }
}


void
gaplessgrid(Monitor *m) {
	unsigned int n, cols, rows, cn, rn, i, cx, cy, cw, ch;
	Client *c;

	for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) ;
	if(n == 0)
		return;

	/* grid dimensions */
	for(cols = 0; cols <= n/2; cols++)
		if(cols*cols >= n)
			break;
	if(n == 5) /* set layout against the general calculation: not 1:2:2, but 2:3 */
		cols = 2;
	rows = n/cols;

	/* window geometries */
	cw = cols ? m->ww / cols : m->ww;
	cn = 0; /* current column number */
	rn = 0; /* current row number */
	for(i = 0, c = nexttiled(m->clients); c; i++, c = nexttiled(c->next)) {
		if(i/rows + 1 > cols - n%cols)
			rows = n/cols + 1;
		ch = rows ? m->wh / rows : m->wh;
		cx = m->wx + cn*cw;
		cy = m->wy + rn*ch;
		resize(c, cx, cy, cw - 2 * c->bw, ch - 2 * c->bw, False);
		rn++;
		if(rn >= rows) {
			rn = 0;
			cn++;
		}
    }
}

void
doubledeck(Monitor *m) {
	unsigned int i, n, mw;
	Client *c;

	for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if(n == 0)
		return;

	if(n > m->nmaster)
		mw = m->nmaster ? m->ww * m->mfact : 0;
	else
		mw = m->ww;

	for(i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if(i < m->nmaster)
			resize(c, m->wx, m->wy, mw - (2*c->bw), m->wh - (2*c->bw), False);
		else
			resize(c, m->wx + mw, m->wy, m->ww - mw - (2*c->bw), m->wh - (2*c->bw), False);
}

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "R",        rstack },    /* first entry is default */
    { "L",        lstack }, 
    { "B",        bstack },
    { "M",        monocle },
    { "",         NULL },    /* no layout function means floating behavior */
    // bottomstack
    // centeredmaster
    { "D",       centeredmaster },
    { "M",       centeredfloatingmaster },

    { "G",       gaplessgrid },

    { "D",       doubledeck },
    { NULL,      NULL }
};

void
cyclelayout(const Arg *arg) {
    Layout *l;
    for(l = (Layout *)layouts; l != selmon->lt[selmon->sellt]; l++);
    if(arg->i > 0) {
        if(l->symbol && (l + 1)->symbol)
            setlayout(&((Arg) { .v = (l + 1) }));
        else
            setlayout(&((Arg) { .v = layouts }));
    } else {
        if(l != layouts && (l - 1)->symbol)
            setlayout(&((Arg) { .v = (l - 1) }));
        else
            setlayout(&((Arg) { .v = &layouts[LENGTH(layouts) - 2] }));
    }
}

void
movestack(const Arg *arg) {
    Client *c = NULL, *p = NULL, *pc = NULL, *i;

    if(arg->i > 0) {
        /* find the client after selmon->sel */
        for(c = selmon->sel->next; c && (!ISVISIBLE(c) || c->isfloating); c = c->next);
        if(!c)
            for(c = selmon->clients; c && (!ISVISIBLE(c) || c->isfloating); c = c->next);

    }
    else {
        /* find the client before selmon->sel */
        for(i = selmon->clients; i != selmon->sel; i = i->next)
            if(ISVISIBLE(i) && !i->isfloating)
                c = i;
        if(!c)
            for(; i; i = i->next)
                if(ISVISIBLE(i) && !i->isfloating)
                    c = i;
    }
    /* find the client before selmon->sel and c */
    for(i = selmon->clients; i && (!p || !pc); i = i->next) {
        if(i->next == selmon->sel)
            p = i;
        if(i->next == c)
            pc = i;
    }

    /* swap c and selmon->sel selmon->clients in the selmon->clients list */
    if(c && c != selmon->sel) {
        Client *temp = selmon->sel->next==c?selmon->sel:selmon->sel->next;
        selmon->sel->next = c->next==selmon->sel?c:c->next;
        c->next = temp;

        if(p && p != c)
            p->next = c;
        if(pc && pc != selmon->sel)
            pc->next = selmon->sel;

        if(selmon->sel == selmon->clients)
            selmon->clients = c;
        else if(c == selmon->clients)
            selmon->clients = selmon->sel;

        arrange(selmon);
    }
}

void togglefullscr(const Arg *arg) {
  if(selmon->sel) setfullscreen(selmon->sel, !selmon->sel->isfullscreen);
}

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = { "st", NULL };

static const char *vol_up[]   = { "pulsemixer", "--change-volume", "+10", NULL };
static const char *vol_down[] = { "pulsemixer", "--change-volume", "-10", NULL };
static const char *vol_mute[] = { "pulsemixer", "--toggle-mute",          NULL };

static const char *music_toggle[] = { "mpc", "toggle", NULL };
static const char *music_next[]   = { "mpc", "next",   NULL };
static const char *music_prev[]   = { "mpc", "prev",   NULL };

static Key keys[] = {
    /* modifier                     key              function        argument */
    { MODKEY,                       XK_a,            spawn,          {.v = dmenucmd } },
    { MODKEY,                       XK_Return,       spawn,          {.v = termcmd } },
    { MODKEY,                       XK_m,            spawn,          {.v = vol_mute } },
    { MODKEY,                       XK_comma,        spawn,          {.v = vol_down } },
    { MODKEY,                       XK_period,       spawn,          {.v = vol_up } },
    { MODKEY|ShiftMask,             XK_m,            spawn,          {.v = music_toggle } },
    { MODKEY|ShiftMask,             XK_comma,        spawn,          {.v = music_prev } },
    { MODKEY|ShiftMask,             XK_period,       spawn,          {.v = music_next } },
    { MODKEY,                       XK_b,            togglebar,      {0} },
    { MODKEY,                       XK_j,            focusstack,     {.i = +1 } },
    { MODKEY,                       XK_k,            focusstack,     {.i = -1 } },    
    { MODKEY|ShiftMask,             XK_j,            movestack,      {.i = +1 } },
    { MODKEY|ShiftMask,             XK_k,            movestack,      {.i = -1 } },
    { MODKEY,                       XK_i,            incnmaster,     {.i = +1 } },
    { MODKEY,                       XK_o,            incnmaster,     {.i = -1 } },
    { MODKEY,                       XK_bracketleft,  setmfact,       {.f = +0.05} },
    { MODKEY,                       XK_bracketright, setmfact,       {.f = -0.05} },
    { MODKEY,                       XK_Tab,          view,           {0} },
    { MODKEY,                       XK_d,            killclient,     {0} },
    //{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
    { MODKEY|ShiftMask,             XK_f,            zoom,           {0} },
    { MODKEY,                       XK_f,            togglefullscr,  {0} },
    { MODKEY,                       XK_s,            view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_s,            tag,            {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_space,        togglefloating, {0} },
    { MODKEY,                       XK_h,            focusmon,       {.i = -1 } },
    { MODKEY,                       XK_l,            focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_h,            tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_l,            tagmon,         {.i = +1 } },
    { MODKEY,                       XK_space,        cyclelayout,    {.i = +1 } },
    { MODKEY|ControlMask,           XK_space,        cyclelayout,    {.i = -1 } },
    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    TAGKEYS(                        XK_6,                      5)
    TAGKEYS(                        XK_7,                      6)
    TAGKEYS(                        XK_8,                      7)
    TAGKEYS(                        XK_9,                      8)
    TAGKEYS(                        XK_0,                      9)
    TAGKEYS(                        XK_minus,                  10)
    TAGKEYS(                        XK_equal,                  11)
    //{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        cyclelayout,    {.i = -1} },
    { ClkLtSymbol,          0,              Button3,        cyclelayout,    {.i = +1} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

