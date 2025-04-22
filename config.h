/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
//#include <stddef.h>
/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 8;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = { "Monocraft Nerd Font:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#050406";
static const char col_gray2[]       = "#0D0D14";
static const char col_gray3[]       = "#8A7EA8";
static const char col_gray4[]       = "#CDBBF9";
static const char col_cyan[]        = "#90128B";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "xarchiver",NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       0,            0,           -1 },
	{ "Kitty",    NULL,       NULL,       0,            0,           -1 },
	{ "mpv",      NULL,       NULL,       0,            1,           -1 },
	{ "jamesdsp", NULL,       NULL,       0,            1,           -1 },
	{ "zenity",   NULL,       NULL,       0,            1,           -1 },
	{ "steamwebhelper",NULL,  NULL,       0,            1,           -1 },
	{ "steam",    NULL,       NULL,       0,            1,           -1 },
	{ "qBittorrent","qbittorrent",NULL,   0,            1,           -1 },
	{ "Alert",    NULL,       NULL,       0,            1,           -1 }
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "T",      tile },    /* first entry is default */
	{ "F",      NULL },    /* no layout function means floating behavior */
	{ "[M]",    monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { "kitty", NULL };
static const char *rofi[]={"rofi","-modi", "drun", "-show", "drun", "-show-icons", "-font", "Monocraft Nerd Font 12", NULL};
static const char *rofimoji[]={"rofi", "-modi", "emoji:rofimoji", "-show", "emoji"};
static const char *flame[]={"flameshot", "gui", NULL};
static const char scrotwindow[]="scrot -q 100 -u -Z -z -b - | xclip -selection clipboard -target image/png";
static const char *flamefull[]={"flameshot", "full", "-c", "-p", "/home/dxc/Pictures/screens/", NULL};
static const char *volup[]={"pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%",NULL};
static const char *voldn[]={"pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%",NULL};
static const char *bnup[]={"brightnessctl", "s", "+10%", NULL};
static const char *bndn[]={"brightnessctl", "s", "10%-", NULL};
static const char *redon[]={"redshift", "-PO", "2500", NULL};
static const char *redoff[]={"redshift", "-x", NULL};

static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */

	{ MODKEY,                       XK_d,      spawn,          {.v = rofi } },
	{ MODKEY,                       XK_e,      spawn,          {.v = rofimoji } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_Left,   focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_Right,  focusstack,     {.i = +1 } },
//	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_n,      spawn,          {.v = redon } },
	{ MODKEY|ShiftMask,		XK_n,      spawn,          {.v = redoff } },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ 0|ShiftMask,			XK_Print,  spawn,          {.v = flame } },
	{ MODKEY|ShiftMask,		XK_Print,  spawn,          SHCMD( scrotwindow ) },
	{ 0,				XK_Print,  spawn,          {.v = flamefull } },
	{ 0,				XF86XK_AudioRaiseVolume,   spawn,		   {.v=volup} },
	{ 0,				XF86XK_AudioLowerVolume,   spawn,		   {.v=voldn} },
	{ 0,				XF86XK_HomePage,           spawn,		   {.v=bnup} },
	{ 0,				XF86XK_Mail,               spawn,		   {.v=bndn} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
//	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
//	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
//	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
//	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
//	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
//	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
//	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_k,      setmfact,       {.f = +0.05} },
//	{ MODKEY,                       XK_b,      togglebar,      {0} },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
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
