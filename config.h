/* See LICENSE file for copyright and license details. */

/* appearance */
static       unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int gapsborderpx = 2;
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 16;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 16;       /* vert inner gap between windows */
static const unsigned int gappoh    = 32;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 32;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static       int warp_enabled = 1;
static const char *fonts[] = { "FiraCode Nerd Font Mono", "Noto Color Emoji", "Twemoji" }; //fonts are ttf-joypixels, ttf-twemoji, and nerd-fonts-fira-code
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_white[]       = "#ffffff";
static const char col_cyan[]        = "#005577";
static const char col_magenta[]        = "#ff5555";
static const char col_red[]        = "#ff0000";
static const char col_purplepink[]        = "#4c004c";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
    [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
    [SchemeSel]  = { col_gray4, col_purplepink,  col_white  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

void view_click(const Arg* arg) { //disable warp on clicks
    warp_enabled = 0;
    view(arg);
    warp_enabled = 1;
}
void nowarpkillclient(const Arg* arg) {
    warp_enabled = 0;
    killclient(arg);
    /*set warp_enabled = 1; in dwm.c:unmanage*/
}

static const char *tagsel[][2] = {
	{ "#000000", "#1462EA" }, //Bright Blue
	{ "#000000", "#E2781C" }, //Tangelo Orange
	{ "#000000", "#11ff11" }, //Green
	{ "#000000", "#ffff00" }, //Yellow
	{ "#000000", "#9547B7" }, //Purpurus
	{ "#000000", "#FF2222" }, //Red
	{ "#000000", "#FF69B4" }, //Hot pink
	{ "#000000", "#d0ff7f" }, //Pale Yellow-Green
	{ "#000000", "#00EDFF" }, //Cyan
};

static const Rule rules[] = {
	/* xprop(1):
     *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	*/
	/* class     instance   title           tags mask  isfloating  isterminal  noswallow  ispermanent attachdirection,  nourgent,      monitor */
	{ "Gimp",    NULL,      NULL,           0,          1,          0,          0,         0,         -1,               -1,         -1 },
    { "firefox", NULL,      NULL,           0,          0,          0,          -1,        1,         +1,               -1,         -1 },
    { "nvide_daemon", NULL, NULL,           0,          0,          1,          -1,        0,         -1,               -1,         -1 },
	{ "St",      NULL,      NULL,           0,          0,          1,          0,         0,         -1,               -1,         -1 },
	{ NULL,      NULL,      "Event Tester", 0,          0,          0,          -1,        0,         -1,               -1,         -1 }, /* xev */
	{ "discord", NULL,	    "Discord",      1 << 3,     0,          0,          0,         0,         -1,               -1,         +1 }, 
	{ "steam",   NULL,      "Steam",		1 << 1,     0,          0,          1,         1,         -1,               +1,         +1 },
	{ "Signal",  NULL,      NULL,           1 << 0,     0,          0,          0,         1,         -1,               -1,         +1 }, 
	{ "Spotify", NULL,      NULL,           1 << 0,     0,          0,          0,         1,         -1,               -1,         +1 }, 
	{ "zenity",  NULL,      NULL,           1 << 0,     0,          0,          0,         1,         -1,               -1,         +1 }, 
	{ "thunderbird", NULL,  NULL,           1 << 2,     0,          0,          0,         1,         -1,               -1,         +1 }, 

};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
#include "attach.c"
#define PERTAG_PATCH

void debug(const Arg *arg) {
    int i=0;
    int j,k;
    Monitor *m;
    for (m = mons; m; m = m->next) {
        j=m->nmaster;
        k=m->sellt;
        char command[256];
        if (m == selmon) {
            snprintf(command, sizeof(command), "notify-send -t 1000 'Current Monitor %d: nmaster: %d'", i, j); 
        } else {
            snprintf(command, sizeof(command), "notify-send -t 1000 'Other Monitor %d: nmaster: %d'", i, j); 
        }
        system(command); 
        i++;
    }
}

static const unsigned int lpm[] = {
        /* Index of preferred layout], if LENGTH(lpm)<#monitors -> default layout */
        11 /*centeredmaster*/, 6 /*bstack*/
};
static const unsigned int nmasterpermon[] = {
    1,0
};

static const Layout layouts[] = {
	/* symbol     arrange function */
/* 0*/	{ "[T]",      tile },    /* first entry is default */
/* 1*/	{ "[M]",      monocle },
/* 2*/	{ "[@]",      spiral },
/* 3*/	{ "[W]",     dwindle },
/* 4*/	{ "[D]",      deck },
/* 5*/	{ "[S]",      bstack },
/* 6*/	{ "[U]",      bstackhoriz },
/* 7*/	{ "[g]",      grid },
/* 8*/	{ "[G]",      nrowgrid },
/* 9*/	{ "[-]",      horizgrid },
/*10*/	{ ":::",      gaplessgrid },
/*11*/	{ "|F|",      centeredmaster },
/*12*/	{ ">F>",      centeredfloatingmaster },
/*13*/	{ "><>",      NULL },    /* no layout function means floating behavior */
	    { NULL,       NULL },
};

static inline Bool ismaster(Client *c) {
    int i = 0;
    Client *t;

    for (t = nexttiled(selmon->clients); t && i < selmon->nmaster; t = nexttiled(t->next), i++) {
        if (t == c)
            return True;
    }
    return False;
}

Client *deckcyclechildclients() {
    Client **pc, *c, *first_child = NULL, *last_child = NULL;
    int n = 0, i;

    for (c = nexttiled(selmon->clients); c; c = nexttiled(c->next))
        n++;

    int num_child_clients = n - selmon->nmaster;
    if (num_child_clients < 2) {
        return NULL;
    }
    c = nexttiled(selmon->clients);
    for (i = 0; i < selmon->nmaster && c; c = nexttiled(c->next), i++); // Skip master clients
    first_child = c;
    if (!first_child)
        return NULL;
    for (last_child = first_child; nexttiled(last_child->next); last_child = nexttiled(last_child->next));

    pc = &selmon->clients;
    while (*pc && *pc != first_child)
        pc = &(*pc)->next;

    if (*pc == first_child) {
        *pc = first_child->next;          
        last_child->next = first_child;   
        first_child->next = NULL;         
    }
    c = nexttiled(selmon->clients);
    for (i = 0; i < selmon->nmaster && c; c = nexttiled(c->next), i++);

    return c;
}

#include "movestack.c"
static void attachstack(Client *c);
void deckcmd(const Arg *arg) {
    if (selmon->lt[selmon->sellt]->arrange != &deck) {
        Arg arg;
        arg.v = &layouts[4];
        setlayout(&arg);
    } else {
        int n;
        Client *c;
        for (n = 0, c = nexttiled(selmon->clients); c; c = nexttiled(c->next), n++);
        int num_child_clients = n - (int)(selmon->nmaster);
        if (num_child_clients <= 1) {return;}

        Bool bismaster = ismaster(selmon->sel);
        Client *oldfocus = selmon->sel;
        Client *newfocused = deckcyclechildclients();
		detachstack(newfocused);
		attachstack(newfocused);
        if ( bismaster ) {
            focus(oldfocus);
        }
        else {
            focus(newfocused);
        }
        restack(selmon);
    }
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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *layoutmenu_cmd = "layoutmenu.sh";

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
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	//{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY|ControlMask,             XK_space, debug,           {0} },
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
	{ MODKEY,                       XK_semicolon,      togglegaps,     {0} },
	//{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	//{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_x,      nowarpkillclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} }, //tile
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} }, //monocle
	//{ MODKEY,                       ?,      setlayout,      {.v = &layouts[2]} }, //spiral
	//{ MODKEY,                       ?,      setlayout,      {.v = &layouts[3]} }, //dwindle
	{ MODKEY|ShiftMask,             XK_d,      deckcmd,         {0} }, //deck
	{ MODKEY,                       XK_s,      setlayout,      {.v = &layouts[5]} }, //bstack
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[6]} }, //bstackhoriz
	//{ MODKEY,                       ?,      setlayout,      {.v = &layouts[7]} }, //grid
	//{ MODKEY,                       ?,      setlayout,      {.v = &layouts[8]} }, //nrowgrid
	//{ MODKEY,                       ?,      setlayout,      {.v = &layouts[9]} }, //horizgrid
	//{ MODKEY,                       ?,      setlayout,      {.v = &layouts[10]} }, //gaplessgrid
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[11]} }, //centeredmaster
    { MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[12]} }, //centeredfloatingmaster
	//{ MODKEY,                       ?,      setlayout,      {.v = &layouts[13]} }, //All floating
	//{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ControlMask,		    XK_comma,  cyclelayout,    {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },
	{ MODKEY,                       XK_space,  togglefloating, {0} },
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
	//{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	//{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	//{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	//{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkLtSymbol,          0,              Button3,        layoutmenu,     {0} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	//{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view_click,           {0} },
	//{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	//{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	//{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

