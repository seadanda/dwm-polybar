/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h> // media keys

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int gappx     = 0;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int usealtbar          = 1;        /* 1 means use non-dwm status bar */
static const char *altbarclass      = "Polybar"; /* Alternate bar class name */
static const char *alttrayname      = "tray";    /* Polybar tray instance name */
static const char *altbarcmd        = "$HOME/.config/polybar/launch.sh"; /* Alternate bar launch command */
static const char *fonts[]          = {
    "noto:size=14",
	"Font Awesome 5 Free Regular-12",
	"Font Awesome 5 Free Solid-12"
};
static const char dmenufont[]       = "noto:size=14";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
    /*               fg         bg         border   */
    [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
    [SchemeSel] =  { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", ""};

static const Rule rules[] = {
    /* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
    /* class               instance    title       tags mask     isfloating   monitor */
    { "Gimp",              NULL,       NULL,       1 << 4,       1,           -1 },
    { "ProtonMail Bridge", NULL,       NULL,       1 << 6,       1,           -1 },
    { "Thunderbird",       NULL,       NULL,       1 << 6,       0,           -1 },
    { "Spotify",           NULL,       NULL,       1 << 7,       0,           -1 },
    { "Rambox",            NULL,       NULL,       1 << 8,       0,           -1 },
    { "Signal",            NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "",      tile },    /* first entry is default */
    { "",      NULL },    /* no layout function means floating behavior */
    { "",      monocle },
};

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
static char dmenumon[2]         = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *mutecmd[]    = { "amixer", "set", "Master", "toggle", NULL };
static const char *voldowncmd[] = { "amixer", "set", "Master", "5%-",    NULL };
static const char *volupcmd[]   = { "amixer", "set", "Master", "5%+",    NULL };
static const char *plyrprev[]   = { "playerctl", "previous",             NULL };
static const char *plyrplay[]   = { "playerctl", "play-pause",           NULL };
static const char *plyrnext[]   = { "playerctl", "next",                 NULL };
static const char *lockcmd[]    = { "i3lock-seadanda",                   NULL };
static const char *dispcon[]    = { "disp",                              NULL };
static const char *dispdiscon[] = { "dispr",                             NULL };
static const char *brightup[]   = { "xbacklight", "-inc",  "10",         NULL };
static const char *brightdown[] = { "xbacklight", "-dec",  "10",         NULL };
static const char *screenshot[] = { "screenshot",                        NULL };
static const char *kbtoggle[]   = { "kbtoggle",                          NULL };
static const char *termcmd[]    = { "alacritty",                         NULL };
static const char *dmenucmd[]   = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };

static Key keys[] = {
    /* modifier            key                       function        argument */
    { 0,                   XF86XK_AudioMute,         spawn,          {.v = mutecmd    } },
    { 0,                   XF86XK_AudioLowerVolume,  spawn,          {.v = voldowncmd } },
    { 0,                   XF86XK_AudioRaiseVolume,  spawn,          {.v = volupcmd   } },
    { 0,                   XF86XK_AudioPrev,         spawn,          {.v = plyrprev   } },
    { 0,                   XF86XK_AudioPlay,         spawn,          {.v = plyrplay   } },
    { 0,                   XF86XK_AudioNext,         spawn,          {.v = plyrnext   } },
    { MODKEY,              XK_F7,                    spawn,          {.v = lockcmd    } },
    { MODKEY,              XF86XK_Search,            spawn,          {.v = dispcon    } },
    { MODKEY|ShiftMask,    XF86XK_Search,            spawn,          {.v = dispdiscon } },
    { 0,                   XF86XK_MonBrightnessDown, spawn,          {.v = brightdown } },
    { 0,                   XF86XK_MonBrightnessUp,   spawn,          {.v = brightup   } },
    { 0,                   XK_Print,                 spawn,          {.v = screenshot } },
    { MODKEY,              XK_space,                 spawn,          {.v = kbtoggle   } },
    { MODKEY,              XK_p,                     spawn,          {.v = dmenucmd   } },
    { MODKEY|ShiftMask,    XK_Return,                spawn,          {.v = termcmd    } },
    { MODKEY,              XK_Tab,                   focusstack,     {.i = +1 } },
    { MODKEY,              XK_b,                     togglebar,      {0} },
    { MODKEY,              XK_j,                     focusstack,     {.i = +1 } },
    { MODKEY,              XK_k,                     focusstack,     {.i = -1 } },
    { MODKEY,              XK_i,                     incnmaster,     {.i = +1 } },
    { MODKEY,              XK_d,                     incnmaster,     {.i = -1 } },
    { MODKEY,              XK_h,                     setmfact,       {.f = -0.05} },
    { MODKEY,              XK_l,                     setmfact,       {.f = +0.05} },
    { MODKEY,              XK_Return,                zoom,           {0} },
    { MODKEY|ShiftMask,    XK_c,                     killclient,     {0} },
    { MODKEY,              XK_t,                     setlayout,      {.v = &layouts[0]} },
    { MODKEY,              XK_f,                     setlayout,      {.v = &layouts[1]} },
    { MODKEY,              XK_m,                     setlayout,      {.v = &layouts[2]} },
    { MODKEY|ShiftMask,    XK_space,                 togglefloating, {0} },
    { MODKEY,              XK_0,                     view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,    XK_0,                     tag,            {.ui = ~0 } },
    { MODKEY,              XK_comma,                 focusmon,       {.i = -1 } },
    { MODKEY,              XK_period,                focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,    XK_comma,                 tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,    XK_period,                tagmon,         {.i = +1 } },
	{ MODKEY,              XK_minus,                 setgaps,        {.i = -1 } },
	{ MODKEY,              XK_equal,                 setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_equal,                 setgaps,        {.i = 0  } },
    TAGKEYS(               XK_1,                                     0)
    TAGKEYS(               XK_2,                                     1)
    TAGKEYS(               XK_3,                                     2)
    TAGKEYS(               XK_4,                                     3)
    TAGKEYS(               XK_5,                                     4)
    TAGKEYS(               XK_6,                                     5)
    TAGKEYS(               XK_7,                                     6)
    TAGKEYS(               XK_8,                                     7)
    TAGKEYS(               XK_9,                                     8)
    { MODKEY|ShiftMask,    XK_q,                     quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
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

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(  view,                1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggleview,          1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tag,                 1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggletag,           1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tagmon,              1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  focusmon,            1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  focusstack,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  zoom,                1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  incnmaster,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  killclient,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  togglefloating,      1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  setmfact,            1,      {ARG_TYPE_FLOAT}  ),
  IPCCOMMAND(  setlayoutsafe,       1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  quit,                1,      {ARG_TYPE_NONE}   )
};
