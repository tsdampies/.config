// TSDAMPIES --- DWM CONFIGURATION FILE ---
/*
sudo nano /etc/systemd/logind.conf
sudo grub-mkconfig -o /boot/grub/grub.cfg

//Touchpad
/etc/X11/xorg.conf.d/90-touchpad.conf
Section "InputClass"
        Identifier "touchpad"
        MatchIsTouchpad "on"
        Driver "libinput"
        Option "Tapping" "on"
        Option "TappingButtonMap" "lrm"
        Option "NaturalScrolling" "on"
        Option "ScrollMethod" "twofinger"
EndSection

//Autologin
/etc/systemd/system/getty@tty1.service.d/override.conf
[Service]
ExecStart=
ExecStart=-/usr/bin/agetty --autologin username --noclear %I $TERM


//Auto startx 
~/.bash_profile
if [[ -z "$DISPLAY" ]] && [[ $(tty) = /dev/tty1 ]]; then
. startx
logout
fi


xset dpms 900 900 900
sudo systemctl mask hibernate.target
sudo systemctl mask hybrid-sleep.target

//enable sleep 
sudo vim /etc/systemd/logind.conf

//firewall
sudo pacman -S ufw
sudo ufw limit 22/tcp
sudo ufw allow 80/tcp
sudo ufw allow 443/tcp
sudo ufw default deny incoming
sudo ufw default allow outgoing
sudo ufw enable

//ipbanning
sudo pacman -S fail2ban
sudo vim /etc/fail2ban/fail2ban.conf
#[DEFAULT]
ignoreip = 127.0.0.1/8 ::1
bantime = 3600
findtime = 600
maxretry = 5

[sshd]
enabled = true

sudo systemctl enable fail2ban
sudo systemctl start fail2ban

//tlp
/etc/default/tlp
WIFI_PWR_ON_AC=off
WIFI_PWR_ON_BAT=off

*/
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#384252";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#384252";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3" };

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
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 60;  /* refresh rate (per second) for client move/resize */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]",      monocle },    /* first entry is default */
	{ "<>",      NULL },    /* no layout function means floating behavior */
	{ "|=|",      tile },
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
static const char *dmenucmd[] =		{ "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = 	{ "terminator", NULL };
static const char *ranger[]  =	 	{ "terminator", "-e", "ranger", NULL };
static const char *volumeup[]      = { "/usr/bin/amixer",  "set", "Master", "5%+", NULL };
static const char *volumedown[]    = { "/usr/bin/amixer",  "set", "Master", "5%-", NULL };
static const char *volumemute[]    = { "/usr/bin/amixerl", "set", "Master", "toggle", NULL };
static const char *brightnessup[] =	{ "brightnessctl", "set", "10%+", NULL };
static const char *brightnessdown[] =	{ "brightnessctl", "set", "10%-", NULL };
static const char *filemanager[] = 	{ "dolphin" , NULL };


static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_i,      spawn,          {.v = dmenucmd } },
	{ MODKEY,	                XK_t,      spawn,          {.v = termcmd } },
	{ Mod1Mask,                     XK_space,  togglebar,      {0} },
	{ Mod1Mask,                     XK_z,      focusstack,     {.i = +1 } },
	{ Mod1Mask,                     XK_x,      focusstack,     {.i = -1 } },
	{ MODKEY,			XK_j,      incnmaster,     {.i = +1 } },
	{ MODKEY,			XK_k,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_9,      view,           {0} },
	{ MODKEY,	                XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_b,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_a,                        0)
	TAGKEYS(                        XK_s,                        1)
	TAGKEYS(                        XK_d,                        2)
	{ MODKEY|ShiftMask,             XK_o,      quit,           {0} },
	{ MODKEY,			XK_e,	spawn,	{.v = filemanager } },
	{ MODKEY,                       XK_y,      spawn,          {.v = ranger } },
	{ MODKEY|ShiftMask,    		XK_g,	spawn,		SHCMD("sleep 1 && systemctl suspend") },
	{ MODKEY|ShiftMask,		XK_u,	spawn,          SHCMD("shutdown -r now") },
	{ MODKEY|ShiftMask,  		XK_n,	spawn,		SHCMD("poweroff")},
	{ MODKEY,	  		XK_p,	spawn,		SHCMD("xrandr --auto")},
	{ MODKEY,	  		XK_r,	spawn,		SHCMD("j4-dmenu-desktop") },
	{ MODKEY|ControlMask,		XK_q,	spawn,		SHCMD("redshift -P -O 4600") },
	{ MODKEY,			XK_q,	spawn,		SHCMD("redshift -P -O 3600") },
	{ MODKEY|ControlMask,		XK_w,	spawn,		SHCMD("vibrant-cli eDP-1 1") },
	{ MODKEY,			XK_w,	spawn,		SHCMD("vibrant-cli eDP-1 0") },
	{ 0,            		XF86XK_AudioRaiseVolume, 	spawn,	{.v = volumeup } },
	{ 0,				XF86XK_AudioLowerVolume, 	spawn,	{.v = volumedown } },
	{ 0,				XF86XK_AudioMute, 		spawn,	{.v = volumemute } },
	{ 0, 				XF86XK_MonBrightnessUp, 	spawn,	{.v = brightnessup } },
    	{ 0,				XF86XK_MonBrightnessDown,	spawn,	{.v = brightnessdown } },

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
};
