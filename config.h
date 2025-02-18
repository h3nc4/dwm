/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 6;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Terminess Nerd Font:size=11" };
static const char dmenufont[]       = "Terminess Nerd Font:size=11";
static const char col_gray1[]       = "#2F4F4F";  /* Dark Slate Gray for background */
static const char col_gray2[]       = "#8B0000";  /* Dark Red for borders */
static const char col_gray3[]       = "#D3D3D3";  /* Light Gray for normal text */
static const char col_gray4[]       = "#FFFFFF";  /* White for active text */
static const char col_active[]      = "#A40000";  /* Crimson Red for active */
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_active, col_active },
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
#define CTRL ControlMask
#define SHFT ShiftMask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|CTRL,                  KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|SHFT,                  KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|CTRL|SHFT,             KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_active, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
#include "movestack.c"
static const Key keys[] = {
	/* modifier    key         function        argument */
	{ MODKEY|SHFT, XK_0,       tag,            {.ui = ~0 } }, // Super+Shift+0 Tag the focused window with all tags
	{ MODKEY,      XK_0,       view,           {.ui = ~0 } }, // Super+0 View all tags (show all windows)
	{ MODKEY,      XK_a,       killclient,     {0} }, // Super+a Close the focused window
	{ MODKEY|SHFT, XK_a,       spawn,          SHCMD("pkill sxac || sxac") }, // Super+Shift+a Toggle Simple X Auto Clicker (kill if running, otherwise launch)
	{ MODKEY,      XK_b,       togglebar,      {0} }, // Super+b Toggle the status bar
	{ MODKEY|SHFT, XK_c,       spawn,          SHCMD("chromium") }, // Super+Shift+c Launch Chromium
	{ MODKEY,      XK_c,       spawn,          SHCMD("codium") }, // Super+c Launch Codium
	{ MODKEY|CTRL, XK_d,       incnmaster,     {.i = -1 } }, // Super+Ctrl+d Decrease the number of windows in the master area
	{ MODKEY,      XK_d,       spawn,          SHCMD("arandr") }, // Super+d Launch display settings (arandr)
	{ MODKEY|SHFT, XK_Escape,  spawn,          SHCMD("st -e htop") }, // Super+Shift+Escape Launch htop
	{ MODKEY,      XK_e,       spawn,          SHCMD("st -e lf") }, // Super+e Launch lf
	{ MODKEY,      XK_F11,     spawn,          SHCMD("doas brightnessctl set 10%%-") }, // Super+F11 Decrease brightness by 10%
	{ MODKEY,      XK_F12,     spawn,          SHCMD("doas brightnessctl set +10%%") }, // Super+F12 Increase brightness by 10%
	{ MODKEY,      XK_F1,      spawn,          SHCMD("pulsemixer --mute") }, // Super+F1 Mute volume
	{ MODKEY|SHFT, XK_F1,      spawn,          SHCMD("pulsemixer --unmute") }, // Super+Shift+F1 Unmute volume
	{ MODKEY,      XK_F2,      spawn,          SHCMD("pulsemixer --unmute --change-volume -5") }, // Super+F2 Decrease volume by 5
	{ MODKEY,      XK_F3,      spawn,          SHCMD("pulsemixer --unmute --change-volume +5") }, // Super+F3 Increase volume by 5
	{ MODKEY,      XK_F4,      spawn,          SHCMD("pulsemixer --unmute --set-volume 100") }, // Super+F4 Set volume to (100)
	{ MODKEY|CTRL, XK_f,       setlayout,      {.v = &layouts[1]} }, // Super+Ctrl+f Set layout to floating
	{ MODKEY|SHFT, XK_h,       shifttag,       {.ui = -1 } }, // Super+Shift+h Move client to the previous tag
	{ MODKEY,      XK_h,       shiftview,      {.ui = -1 } }, // Super+h Switch to the previous tag
	{ MODKEY|CTRL, XK_h,       tagmon,         {.i = -1 } }, // Super+Ctrl+h Move client to the previous monitor
	{ MODKEY|CTRL, XK_i,       incnmaster,     {.i = +1 } }, // Super+Ctrl+i Increase the number of windows in the master area
	{ MODKEY|CTRL, XK_j,       focusmon,       {.i = -1 } }, // Super+Ctrl+j Move focus to the previous monitor
	{ MODKEY,      XK_j,       focusstack,     {.i = +1 } }, // Super+j Move focus to the next window
	{ MODKEY|SHFT, XK_j,       movestack,      {.i = +1 } }, // Super+Shift+j Move the focused window down the stack
	{ MODKEY|CTRL, XK_k,       focusmon,       {.i = +1 } }, // Super+Ctrl+k Move focus to the next monitor
	{ MODKEY,      XK_k,       focusstack,     {.i = -1 } }, // Super+k Move focus to the previous window
	{ MODKEY|SHFT, XK_k,       movestack,      {.i = -1 } }, // Super+Shift+k Move the focused window up the stack
	{ MODKEY|SHFT, XK_Left,    shifttag,       {.ui = -1 } }, // Super+Shift+Left Move client to the previous tag
	{ MODKEY,      XK_Left,    shiftview,      {.ui = -1 } }, // Super+Left Switch to the previous tag
	{ MODKEY|CTRL, XK_Left,    tagmon,         {.i = -1 } }, // Super+Ctrl+Left Move client to the previous monitor
	{ MODKEY|SHFT, XK_l,       shifttag,       {.ui = +1 } }, // Super+Shift+l Move client to the next tag
	{ MODKEY,      XK_l,       shiftview,      {.ui = +1 } }, // Super+l Switch to the next tag
	{ MODKEY|CTRL, XK_l,       tagmon,         {.i = +1 } }, // Super+Ctrl+l Move client to the next monitor
	{ MODKEY|CTRL, XK_m,       setlayout,      {.v = &layouts[2]} }, // Super+Ctrl+m Set layout to monocle (fullscreen)
	{ MODKEY|SHFT, XK_m,       setmfact,       {.f = +0.05} }, // Super+Shift+m Increase the size of the master area
	{ MODKEY,      XK_m,       spawn,          SHCMD("st -e aerc") }, // Super+m Launch aerc
	{ MODKEY|SHFT, XK_n,       setmfact,       {.f = -0.05} }, // Super+Shift+n Decrease the size of the master area
	{ MODKEY|SHFT, XK_Print,   spawn,          SHCMD("maim \"$HOME/.cache/$(date +%s).png\"") }, // Super+Shift+Print Take a screenshot and save it
	{ ShiftMask,   XK_Print,   spawn,          SHCMD("maim -s \"$HOME/.cache/$(date +%s).png\"") }, // Shift+Print Take a screenshot of a selected area and save it
	{ 0,           XK_Print,   spawn,          SHCMD("maim -s | xclip -selection clipboard -t image/png") }, // Print Take a screenshot of a selected area and copy it to the clipboard
	{ MODKEY,      XK_Print,   spawn,          SHCMD("maim | xclip -selection clipboard -t image/png") }, // Super+Print Take a screenshot and copy it to the clipboard
	{ MODKEY,      XK_p,       spawn,          SHCMD("st -e pulsemixer") }, // Super+p Launch pulsemixer
	{ MODKEY|CTRL, XK_q,       spawn,          SHCMD("doas openvt -c7 -- sh -c \"USER=$USER vlock -a; chvt 1\" && doas chvt 7") }, // Super+Ctrl+q Lock screen
	{ MODKEY,      XK_q,       spawn,          {.v = termcmd } }, // Super+q Launch terminal
	{ MODKEY|CTRL, XK_Return,  focusmaster,    {0} }, // Super+Ctrl+Enter Move focus to the master window
	{ MODKEY|SHFT, XK_Return,  zoom,           {0} }, // Super+Shift+Enter Promote the focused window to the master area
	{ MODKEY|SHFT, XK_Right,   shifttag,       {.ui = +1 } }, // Super+Shift+Right Move client to the next tag
	{ MODKEY,      XK_Right,   shiftview,      {.ui = +1 } }, // Super+Right Switch to the next tag
	{ MODKEY|CTRL, XK_Right,   tagmon,         {.i = +1 } }, // Super+Ctrl+Right Move client to the next monitor
	{ MODKEY|SHFT, XK_r,       spawn,          SHCMD("doas reboot") }, // Super+Shift+r Reboot
	{ MODKEY,      XK_r,       spawn,          SHCMD("st -e newsboat") }, // Super+r Launch newsboat
	{ MODKEY,      XK_Shift_R, spawn,          {.v = dmenucmd } }, // Super+Right_Shift Launch dmenu
	{ MODKEY,      XK_space,   setlayout,      {0} }, // Super+Space Toggle between current and previous layout
	{ MODKEY|SHFT, XK_space,   togglefloating, {0} }, // Super+Shift+Space Toggle floating mode for the focused window
	{ MODKEY|SHFT, XK_s,       spawn,          SHCMD("doas poweroff") }, // Super+Shift+s Shutdown
	{ MODKEY|CTRL, XK_s,       spawn,          SHCMD("echo disk | doas tee /sys/power/state") }, // Super+Ctrl+s Hibernate to disk
	{ MODKEY,      XK_s,       spawn,          SHCMD("echo mem | doas tee /sys/power/state") }, // Super+s Suspend to ram
	{ MODKEY|CTRL, XK_Tab,     quit,           {1} }, // Super+Ctrl+Tab Restart dwm
	{ MODKEY|SHFT, XK_Tab,     spawn,          SHCMD("\"$HOME/.config/X11/monitors\"") }, // Super+Shift+Tab Change monitor configuration
	{ MODKEY,      XK_Tab,     view,           {0} }, // Super+Tab Switch to the previously selected tag
	{ MODKEY|CTRL, XK_t,       setlayout,      {.v = &layouts[0]} }, // Super+Ctrl+t Set layout to tile
	{ MODKEY|SHFT, XK_v,       spawn,          SHCMD("st -e nvim") }, // Super+Shift+v Launch nvim
	{ MODKEY,      XK_v,       spawn,          SHCMD("virt-manager") }, // Super+v Launch virt-manager
	{ MODKEY,      XK_w,       spawn,          SHCMD("firefox -P me") }, // Super+w Launch Firefox with profile "me"
	{ MODKEY|SHFT, XK_w,       spawn,          SHCMD("firefox -P rel") }, // Super+Shift+w Launch Firefox with profile "rel"
	{ MODKEY|CTRL, XK_w,       spawn,          SHCMD("tor-browser") }, // Super+Ctrl+w Launch Tor Browser
	{ MODKEY,      XK_x,       spawn,          SHCMD("keepassxc") }, // Super+x Launch keepassxc
	{ MODKEY|CTRL, XK_y,       spawn,          SHCMD("yt opus") }, // Super+Ctrl+y Download YouTube video as OPUS to "$HOME/media/music/%(title)s.%(ext)s"
	{ MODKEY|SHFT, XK_y,       spawn,          SHCMD("yt mp4") }, // Super+Shift+y Download YouTube video as MP4 to "$HOME/media/vids/%(title)s.%(ext)s"
	{ MODKEY,      XK_y,       spawn,          SHCMD("yt stream") }, // Super+y Stream YouTube video to mpv
	TAGKEYS(XK_1, 0) // Switch to tag 1
	TAGKEYS(XK_2, 1) // Switch to tag 2
	TAGKEYS(XK_3, 2) // Switch to tag 3
	TAGKEYS(XK_4, 3) // Switch to tag 4
	TAGKEYS(XK_5, 4) // Switch to tag 5
	TAGKEYS(XK_6, 5) // Switch to tag 6
	TAGKEYS(XK_7, 6) // Switch to tag 7
	TAGKEYS(XK_8, 7) // Switch to tag 8
	TAGKEYS(XK_9, 8) // Switch to tag 9
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
