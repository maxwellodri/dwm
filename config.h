/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[] = { "FiraCode Nerd Font Mono", "Noto Color Emoji", "Twemoji" }; //fonts are ttf-joypixels, ttf-twemoji, and nerd-fonts-fira-code
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const char *tagsel[][2] = {
	{ "#000000", "#1462EA" }, //Bright Blue
	{ "#000000", "#E2781C" }, //Tangelo Orange
	{ "#000000", "#11ff11" }, //Green
	{ "#000000", "#ffff00" }, //Yellow
	{ "#000000", "#9547B7" }, //Purpurus
	{ "#000000", "#C66363" }, //Light Red
	{ "#000000", "#FF69B4" }, //Hot pink
	{ "#000000", "#ffffff" }, //White
	{ "#000000", "#00EDFF" }, //Cyan
};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,          0,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,       -1 },
    { "nvide_daemon", NULL,     NULL,           0,         0,          1,          -1,        -1 },
	{ "St",      NULL,     NULL,           0,         0,          1,          0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,          -1,       -1 }, /* xev */
	{ "Steam",   NULL,     "Steam",		   1 << 1,    0,          0,          1,        1  }, /* steam */
	{ "game",    NULL,     "bevy",		   1 << 0,    0,          0,          1,        0  }, /* bevy game */
	{ "bevy",    NULL,     "bevy",		   1 << 0,    0,          0,          1,        0  }, /* bevy game */
	{ "bevy",    NULL,     "app",		   1 << 0,    0,          0,          1,        0 }, /* bevy game */
	{ "discord", NULL,		"Discord",     1 << 2,    0,          0,          0,        1  }, 
	{ "signal", "signal",		"Signal",  1 << 0,    0,          0,          0,        1  }, 
	//{ "dota2",  "dota2",   NULL,           0,         0,          0,          0,        -1  },  //0 = main monitor,  1 = side vertical monitor
	//{ NULL,     NULL,   "Eww - bar",           0,         0,          0,          0,        0  },  //0 = main monitor,  1 = side vertical monitor
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[T]",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[W]",     dwindle },
	{ "[D]",      deck },
	{ "[S]",      bstack },
	{ "[U]",      bstackhoriz },
	{ "[g]",      grid },
	{ "[G]",      nrowgrid },
	{ "[-]",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|F|",      centeredmaster },
	{ ">F>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
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
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

#include "movestack.c"
static const Key keys[] = {
	/* modifier                     key        function        argument */
	//{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	//{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_p,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	//{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	//{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	//{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY|ShiftMask,             XK_i,      incrgaps,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_p,      incrgaps,       {.i = -1 } },
	//{ MODKEY|Mod4Mask,              XK_i,      incrigaps,      {.i = +1 } },
	//{ MODKEY|Mod4Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	//{ MODKEY|Mod4Mask,              XK_o,      incrogaps,      {.i = +1 } },
	//{ MODKEY|Mod4Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	//{ MODKEY|Mod4Mask,              XK_6,      incrihgaps,     {.i = +1 } },
	//{ MODKEY|Mod4Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	//{ MODKEY|Mod4Mask,              XK_7,      incrivgaps,     {.i = +1 } },
	//{ MODKEY|Mod4Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	//{ MODKEY|Mod4Mask,              XK_8,      incrohgaps,     {.i = +1 } },
	//{ MODKEY|Mod4Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	//{ MODKEY|Mod4Mask,              XK_9,      incrovgaps,     {.i = +1 } },
	//{ MODKEY|Mod4Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	//{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_x,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} }, //tile
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} }, //monocle
	//{ MODKEY,                       ?,      setlayout,      {.v = &layouts[2]} }, //spiral
	//{ MODKEY,                       ?,      setlayout,      {.v = &layouts[3]} }, //dwindle
	//{ MODKEY,                       ?,      setlayout,      {.v = &layouts[4]} }, //deck
	{ MODKEY,                       XK_s,      setlayout,      {.v = &layouts[5]} }, //bstack
	//{ MODKEY,                       ?,      setlayout,      {.v = &layouts[6]} }, //bstackhoriz
	//{ MODKEY,                       ?,      setlayout,      {.v = &layouts[7]} }, //grid
	//{ MODKEY,                       ?,      setlayout,      {.v = &layouts[8]} }, //nrowgrid
	//{ MODKEY,                       ?,      setlayout,      {.v = &layouts[9]} }, //horizgrid
	//{ MODKEY,                       ?,      setlayout,      {.v = &layouts[10]} }, //gaplessgrid
	{ MODKEY,                       XK_c,      setlayout,      {.v = &layouts[11]} }, //centeredmaster
    //{ MODKEY,                       ?,      setlayout,      {.v = &layouts[12]} }, //centeredfloatingmaster
	//{ MODKEY,                       ?,      setlayout,      {.v = &layouts[13]} }, //All floating
	//{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	//{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
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

