#!/bin/bash

read -r -a status <<< "$(pulseaudio-ctl full-status)"
volume=${status[0]}
mute=${status[1]}
symbol=""

if [[ $volume = "0" || $mute = "yes" ]]; then
    symbol=""
elif [[ $volume -lt 60 ]]; then
    symbol=""
else
    symbol=""
fi

echo "<span font_desc='FontAwesome'>$symbol</span> $volume"
