#!/bin/bash

export PATH="$HOME/.nimble/bin/:$HOME/.local/bin:$HOME/.cargo/bin:$PATH"

export NNN_COLORS='1234'
export NNN_PLUG='i:imgview;d:dragdrop'

export XDG_CONFIG_HOME="$HOME/.config"
export XDG_CACHE_HOME="$HOME/.cache"
export XDG_DATA_HOME="$HOME/.local/share"

export GTK_IM_MODULE='fcitx'
export QT_IM_MODULE='fcitx'
export SDL_IM_MODULE='fcitx'
export XMODIFERS='@im=fcitx'

export EDITOR="nvim"
export BROWSER="qutebrowser"
export SHELL="fish"
export TERMINAL="alacritty"
export PAGER="less"
export DMENU="dmenu"


export GTK_IM_MODULE=fcitx
export QT_IM_MODULE=fcitx
export XMODIFIERS=@im=fcitx

export LESSHISTFILE=-

# for IntelliJ IDEA 
# export _JAVA_AWT_WM_NONREPARENTING=1 
# export AWT_TOOLKIT=MToolkit 
# wmname LG3D

xset dpms 0 0 1000
setxkbmap pl -option caps:super
xcape -e 'Shift_L=Escape;Super_L=BackSpace'

picom &
transmission-daemon &
xwallpaper --zoom ~/.config/wallpaper
slstatus &
fcitx -d &

tiramisu -j | while read -r line; do
    notification="$( echo $line | jq -r '.summary' ) $( echo $line | jq -r '.body' )"
    herbe "$notification"
    echo $notification >> ~/.cache/notifications
done &

#i3
dwm
