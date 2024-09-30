/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  = 1;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char font[]            = "JetBrainsMono:size=11:antialias=true";
static char dmenufont[]       = "JetBrainsMono:size=11:antialias=true";
static const char *fonts[]          = { font };
static char normbgcolor[]           = "#282828";
static char normbordercolor[]       = "#a89984";
static char normfgcolor[]           = "#ebdbb2";
static char selfgcolor[]            = "#1d2021";
static char selbordercolor[]        = "#d79921";
static char selbgcolor[]            = "#98971a";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-n", "spnote",  "-g", "144x41", "-e", "nvim", NULL };
const char *spcmd3[] = {"st", "-n", "spcalc", "-f", "monospace:size=16", "-g", "50x20", "-e", "bc", "-lq", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spnote",      spcmd2},
	{"spcalc",      spcmd3},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "st-256color", NULL,  NULL,           0,         	0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester",  0,         	0,          0,           1,        -1 }, /* xev */
	{ NULL,	    "spterm",	NULL,		SPTAG(0),	1,	    1,		 1,	   -1 },
	{ NULL,	    "spnote",	NULL,		SPTAG(1),	1,	    1,           1,	   -1 },
	{ NULL,	    "spcalc",	NULL,		SPTAG(2),	1,          1,		 1,	   -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

#include <X11/XF86keysym.h>

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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, "-l", "20", NULL };
static const char *termcmd[]  = { "/usr/local/bin/st", NULL };
static const char *upvol[] = { "/usr/bin/pamixer", "-u", "-i", "5", NULL };
static const char *downvol[] = { "/usr/bin/pamixer", "-u", "-d", "5", NULL };
static const char *mute[] = { "/usr/bin/pamixer", "-t", NULL };
static const char *mic[] = { "/usr/bin/amixer", "-q", "set", "Capture", "toggle", NULL };
static const char *upbrtt[] = { "/usr/bin/light", "-A", "5", NULL };
static const char *downbrtt[] = { "/usr/bin/light", "-U", "5", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_x,      spawn,          {.v = dmenucmd } },
	{ MODKEY,			XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,			XK_grave,  togglescratch,  {.ui = 0 } },
	{ MODKEY|ShiftMask,            	XK_grave,  togglescratch,  {.ui = 1 } },
	{ MODKEY,		XK_Num_Lock,	   togglescratch,  {.ui = 2 } },
	{ MODKEY,			XK_F3, 	   spawn,          {.v = upvol } },
	{ MODKEY,			XK_F2, 	   spawn,          {.v = downvol } },
	{ MODKEY,			XK_F1, 	   spawn,          {.v = mute } },
	{ MODKEY,			XK_F4, 	   spawn,          {.v = mic } },
	{ MODKEY,                       XK_F5,     spawn,          {.v = downbrtt } },
	{ MODKEY,                       XK_F6,     spawn,          {.v = upbrtt } },
	{ MODKEY,			XK_Print,  spawn,          {.v = (const char*[]){ "/usr/bin/scrot", NULL } } },
	{ MODKEY,		        XK_h,	   spawn,	   SHCMD("st -e /usr/bin/htop") },
	{ MODKEY,        	        XK_f, 	   spawn,          SHCMD("/usr/bin/librewolf") },
	{ MODKEY,			XK_g, 	   spawn,          SHCMD("/usr/bin/brave") },
	{ MODKEY,			XK_y, 	   spawn,          SHCMD("st -e /usr/bin/vendor_perl/youtube-viewer") },
	{ MODKEY,			XK_e, 	   spawn,          SHCMD("st -e /usr/bin/nvim") },
	{ MODKEY,     	                XK_m, 	   spawn,          SHCMD("st -e /usr/bin/claws-mail") },
	{ MODKEY|ShiftMask,		XK_l, 	   spawn,          SHCMD("/usr/local/bin/slock") },
	{ MODKEY,            	        XK_b, 	   spawn,          {.v = (const char*[]){ "bookmarkthis", NULL } } },	
	{ MODKEY,            	        XK_Insert, spawn,          SHCMD("/usr/bin/grep -v '^#' ~/.local/share/bookmarks | /usr/local/bin/dmenu -i -l 20 | /usr/bin/cut -d' ' -f1 | /usr/bin/xclip -selection clipboard") },


	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

	{ MODKEY,	                XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_k,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_k,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_j,      incnmaster,     {.i = -1 } },
	{ MODKEY,			XK_d,      setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_d,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_F12,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_F12,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

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
	{ ClkClientWin,         MODKEY,         Button1,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

