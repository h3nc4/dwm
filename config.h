/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 6;        /* gaps between windows */
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
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};
static const unsigned int alphas[][3]      = {
    /*               fg      bg        border*/
    [SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
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
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask // Super key
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
	/* modifier            key         function        argument */
	{ MODKEY,             XK_Shift_R, spawn,          {.v = dmenucmd } }, // Launch dmenu
	{ MODKEY,             XK_q,       spawn,          {.v = termcmd } },  // Launch terminal
	{ MODKEY,             XK_c,	      spawn,          SHCMD("code") }, // Super + c Launch code
	{ MODKEY|ShiftMask,   XK_c,	      spawn,          SHCMD("chromium") }, // Super + Shift + c Launch chromium
	{ MODKEY,             XK_w,       spawn,          SHCMD("firefox -P me") }, // Super + w Launch firefox
	{ MODKEY,             XK_x,       spawn,          SHCMD("keepassxc") }, // Super + x Launch keepassxc
	{ MODKEY,             XK_v,       spawn,          SHCMD("virt-manager") }, // Super + v Launch virt-manager
	{ MODKEY,             XK_f,       spawn,          SHCMD("thunar") }, // Super + f Launch thunar
	{ MODKEY,             XK_e,       spawn,          SHCMD("st -e lf") }, // Super + e Launch lf
	{ MODKEY|ShiftMask,   XK_Escape,  spawn,          SHCMD("st -e htop") }, // Super + Shift + esc Launch htop
	{ MODKEY,             XK_r,       spawn,          SHCMD("st -e newsboat") }, // Super + r Launch newsboat
	{ MODKEY|ShiftMask,   XK_v,       spawn,          SHCMD("st -e nvim") }, // Super + Shift + v Launch nvim
	{ MODKEY,             XK_n,       spawn,          SHCMD("st -e nmtui") }, // Super + n Launch nmtui
	{ MODKEY,             XK_p,       spawn,          SHCMD("st -e pulsemixer") }, // Super + p Launch pulsemixer
	{ MODKEY,             XK_s,       spawn,          SHCMD("sudo systemctl suspend") }, // Super + s Suspend
	{ MODKEY|ShiftMask,   XK_s,       spawn,          SHCMD("sudo poweroff") }, // Super + Shift + s Shutdown
	{ MODKEY|ShiftMask,   XK_r,       spawn,          SHCMD("sudo reboot") }, // Super + Shift + r Reboot
	{ MODKEY,             XK_y,       spawn,          SHCMD("yt stream") }, // Super + y Stream youtube video to mpv
	{ MODKEY|ShiftMask,   XK_y,       spawn,          SHCMD("yt mp4") }, // Super + Shift + y Download youtube video
	{ MODKEY|ControlMask, XK_y,       spawn,          SHCMD("yt mp3") }, // Super + Ctrl + y Download youtube video as mp3
	{ 0,                  XK_Print,   spawn,          SHCMD("maim -s | xclip -selection clipboard -t image/png") }, // PrtSc Print Selection and copy
	{ ShiftMask,          XK_Print,   spawn,          SHCMD("maim \"$HOME/.cache/$(date +%s).png\"") }, // Shift + PrtSc Print Selection and save
	{ MODKEY,             XK_Print,   spawn,          SHCMD("maim -s | xclip -selection clipboard -t image/png") }, // Super + Print Screen and copy
	{ MODKEY|ShiftMask,   XK_Print,   spawn,          SHCMD("maim -s \"$HOME/.cache/$(date +%s).png\"") }, // Shift + Print Screen and save
	{ MODKEY,             XK_b,       togglebar,      {0} }, // Toggle the status bar
	{ MODKEY,             XK_j,       focusstack,     {.i = +1 } }, // Move focus to the next window
	{ MODKEY,             XK_k,       focusstack,     {.i = -1 } }, // Move focus to the previous window
	{ MODKEY|ControlMask, XK_i,       incnmaster,     {.i = +1 } }, // Increase the number of windows in the master area
	{ MODKEY|ControlMask, XK_d,       incnmaster,     {.i = -1 } }, // Decrease the number of windows in the master area
	{ MODKEY,             XK_h,       setmfact,       {.f = -0.05} }, // Decrease the size of the master area
	{ MODKEY,             XK_l,       setmfact,       {.f = +0.05} }, // Increase the size of the master area
	{ MODKEY|ShiftMask,   XK_j,       movestack,      {.i = +1 } }, // Move the focused window down the stack
	{ MODKEY|ShiftMask,   XK_k,       movestack,      {.i = -1 } }, // Move the focused window up the stack
	{ MODKEY|ShiftMask,   XK_Return,  zoom,           {0} }, // Promote the focused window to the master area
	{ MODKEY,             XK_Tab,     view,           {0} }, // Switch to the previously selected tag
	{ MODKEY,             XK_a,       killclient,     {0} }, // Super + a Close the focused window
	{ MODKEY|ControlMask, XK_t,       setlayout,      {.v = &layouts[0]} }, // Set layout to tile
	{ MODKEY|ControlMask, XK_f,       setlayout,      {.v = &layouts[1]} }, // Set layout to floating
	{ MODKEY|ControlMask, XK_m,       setlayout,      {.v = &layouts[2]} }, // Set layout to monocle (fullscreen)
	{ MODKEY,             XK_space,   setlayout,      {0} }, // Toggle between current and previous layout
	{ MODKEY|ShiftMask,   XK_space,   togglefloating, {0} }, // Toggle floating mode for the focused window
	{ MODKEY,             XK_comma,   focusmon,       {.i = -1 } }, // Move focus to the previous monitor
	{ MODKEY,             XK_period,  focusmon,       {.i = +1 } }, // Move focus to the next monitor
	{ MODKEY,             XK_Left,    shiftview,      {.ui = -1 } }, // Super + Left Arrow Switch to the previous tag
	{ MODKEY,             XK_Right,   shiftview,      {.ui = +1 } }, // Super + Right Arrow Switch to the next tag
	{ MODKEY|ShiftMask,   XK_Left,    shifttag,       {.ui = -1 } }, // Super + Shift + Left Arrow Move client to the previous tag
	{ MODKEY|ShiftMask,   XK_Right,   shifttag,       {.ui = +1 } }, // Super + Shift + Right Arrow Move client to the next tag
	{ MODKEY|ControlMask, XK_Left,    tagmon,         {.i = -1 } }, // Super + Ctrl + Left Arrow Move client to the previous monitor
	{ MODKEY|ControlMask, XK_Right,   tagmon,         {.i = +1 } }, // Super + Ctrl + Right Arrow Move client to the next monitor
	{ MODKEY|ControlMask, XK_Return,  focusmaster,    {0} }, // Super + Ctrl + Enter Move focus to the master window
	{ MODKEY|ControlMask, XK_q,       quit,           {1} }, // Super + Ctrl + q Restart dwm
	{ MODKEY,             XK_0,       view,           {.ui = ~0 } }, // View all tags (show all windows)
	{ MODKEY|ShiftMask,   XK_0,       tag,            {.ui = ~0 } }, // Tag the focused window with all tags
	TAGKEYS(              XK_1,                       0) // Switch to tag 1
	TAGKEYS(              XK_2,                       1) // Switch to tag 2
	TAGKEYS(              XK_3,                       2) // Switch to tag 3
	TAGKEYS(              XK_4,                       3) // Switch to tag 4
	TAGKEYS(              XK_5,                       4) // Switch to tag 5
	TAGKEYS(              XK_6,                       5) // Switch to tag 6
	TAGKEYS(              XK_7,                       6) // Switch to tag 7
	TAGKEYS(              XK_8,                       7) // Switch to tag 8
	TAGKEYS(              XK_9,                       8) // Switch to tag 9
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
