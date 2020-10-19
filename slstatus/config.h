/* See LICENSE file for copyright and license details. */

/* interval between updates (in ms) */
const unsigned int interval = 1000;

/* text to show if no value can be retrieved */
static const char unknown_str[] = "n/a";

/* maximum output string length */
#define MAXLEN 4096

/*
 * function            description                     argument (example)
 *
 * battery_perc        battery percentage              battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_state       battery charging state          battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_remaining   battery remaining HH:MM         battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * cpu_perc            cpu usage in percent            NULL
 * cpu_freq            cpu frequency in MHz            NULL
 * datetime            date and time                   format string (%F %T)
 * disk_free           free disk space in GB           mountpoint path (/)
 * disk_perc           disk usage in percent           mountpoint path (/)
 * disk_total          total disk space in GB          mountpoint path (/")
 * disk_used           used disk space in GB           mountpoint path (/)
 * entropy             available entropy               NULL
 * gid                 GID of current user             NULL
 * hostname            hostname                        NULL
 * ipv4                IPv4 address                    interface name (eth0)
 * ipv6                IPv6 address                    interface name (eth0)
 * kernel_release      `uname -r`                      NULL
 * keyboard_indicators caps/num lock indicators        format string (c?n?)
 *                                                     see keyboard_indicators.c
 * keymap              layout (variant) of current     NULL
 *                     keymap
 * load_avg            load average                    NULL
 * netspeed_rx         receive network speed           interface name (wlan0)
 * netspeed_tx         transfer network speed          interface name (wlan0)
 * num_files           number of files in a directory  path
 *                                                     (/home/foo/Inbox/cur)
 * ram_free            free memory in GB               NULL
 * ram_perc            memory usage in percent         NULL
 * ram_total           total memory size in GB         NULL
 * ram_used            used memory in GB               NULL
 * run_command         custom shell command            command (echo foo)
 * swap_free           free swap in GB                 NULL
 * swap_perc           swap usage in percent           NULL
 * swap_total          total swap size in GB           NULL
 * swap_used           used swap in GB                 NULL
 * temp                temperature in degree celsius   sensor file
 *                                                     (/sys/class/thermal/...)
 *                                                     NULL on OpenBSD
 *                                                     thermal zone on FreeBSD
 *                                                     (tz0, tz1, etc.)
 * uid                 UID of current user             NULL
 * uptime              system uptime                   NULL
 * username            username of current user        NULL
 * vol_perc            OSS/ALSA volume in percent      mixer file (/dev/mixer)
 * wifi_perc           WiFi signal in percent          interface name (wlan0)
 * wifi_essid          WiFi ESSID                      interface name (wlan0)
 */
static const struct arg args[] = {
	/* function format          argument */
	/* { run_command, ",[{\"full_text\":\"\", \"color\": \"#1f0e11AA\", \"separator\":false, \"separator_block_width\": 0 }, {\"color\":\"#CCFF90\", \"background\": \"#1f0e11AA\", \"separator\":false, \"separator_block_width\": 0, \"full_text\":\" %s \"},", NULL, "playerctl --player=vlc metadata vlc:nowplaying", 1, 0 }, */
	{ run_command, ",[{\"full_text\":\"\", \"color\": \"#1f0e11AA\", \"separator\":false, \"separator_block_width\": 0 }, {\"color\":\"#CCFF90\", \"background\": \"#1f0e11AA\", \"separator\":false, \"separator_block_width\": 0, \"full_text\":\" %s \"},", NULL, "mocp -Q %title", 1, 0 },
	/* { run_command, ",[{\"full_text\":\"\", \"color\": \"#1f0e11AA\", \"separator\":false, \"separator_block_width\": 0 }, {\"color\":\"#CCFF90\", \"background\": \"#1f0e11AA\", \"separator\":false, \"separator_block_width\": 0, \"full_text\":\" %s \"},", NULL, "playerctl --player=vlc metadata --format '{{duration(position)}}|{{duration(mpris:length)}} - {{xesam:artist}} - {{xesam:title}}'", 1, 0 }, */
	{ vol_perc, "{\"color\":\"#CCFF90\", \"background\": \"#1f0e11AA\", \"separator\":false, \"separator_block_width\": 0, \"full_text\":\" %s%% \"},", NULL, "/dev/mixer", 1, 1}, // modprobe snd-pcm-oss
	{ ram_used, "{\"full_text\":\"\", \"background\": \"#1f0e11AA\", \"color\": \"#FFFFFFAA\", \"separator\":false, \"separator_block_width\": 0 }, {\"separator\": false, \"separator_block_width\": 0, \"color\":\"#000000\", \"background\": \"#FFFFFFAA\", \"full_text\":\" %s", NULL, NULL, 1, 0},
	{ ram_total, "/%s \"},", NULL, NULL, 0, 0 },
	{ disk_free, "{\"full_text\":\"\", \"color\": \"#BC913DAA\", \"background\": \"#FFFFFFAA\", \"separator\":false, \"separator_block_width\": 0}, {\"separator\":false, \"separator_block_width\": 0, \"background\": \"#BC913DAA\", \"color\": \"#FFFFFF\", \"full_text\":\" %s/", NULL, "/", 30, 0 },
	{ disk_total, "%s\"},", NULL, "/", 0, 0 },
	{ netbytes_rx, "{\"full_text\":\"\", \"background\": \"#BC913DAA\", \"color\": \"#1f0e11AA\", \"separator\":false, \"separator_block_width\": 0 }, {\"separator\":false, \"separator_block_width\": 0, \"background\": \"#1f0e11AA\", \"min_width\":80,\"full_text\":\"▼ %s\"},", NULL, "enp2s0", 1, 0 },
	{ netbytes_tx, "{\"background\": \"#1f0e11AA\", \"min_width\":80,\"separator\":false, \"separator_block_width\": 0, \"full_text\":\"▲ %s\"},", NULL, "enp2s0", 1, 0 },
	{ ping, "{\"background\": \"#1f0e11AA\", \"min_width\":80,\"separator\":false, \"separator_block_width\": 0, \"full_text\":\"PING: %sms \"},", "{\"background\": \"#1f0e11AA\", \"color\": \"#FF0000\", \"min_width\":80,\"separator\":false, \"separator_block_width\": 0, \"full_text\":\"PING: %sms \"},", "ya.ru", 1, 40 },
//	{ ping_errors, "{\"background\": \"#1f0e11AA\", \"min_width\":30,\"separator\":false, \"separator_block_width\": 0, \"full_text\":\" E:%s\"},", NULL, NULL, 1, 0 },
//	{ crit_abs, "{\"background\": \"#1f0e11AA\", \"min_width\":30,\"separator\":false, \"separator_block_width\": 0, \"full_text\":\" C:%s/\"},", NULL, 9, 1, 0 },
//	{ crit_per, "{\"background\": \"#1f0e11AA\", \"min_width\":30,\"separator\":false, \"separator_block_width\": 0, \"full_text\":\"%s \"},", NULL, 9, 1, 0 },
//	{ netspeed_rx, "(%s/", "enp3s0" },
//	{ netspeed_tx, "%s)", "enp3s0" },
	{ cpu_perc, "{\"full_text\":\"\", \"color\": \"#FFFFFFAA\", \"background\": \"#1F0E11AA\", \"separator\":false, \"separator_block_width\": 0 }, {\"separator\":false, \"separator_block_width\": 0, \"background\": \"#FFFFFFAA\", \"color\": \"#000000\", \"min_width\":50, \"full_text\":\" %2s%%\"},", "{\"full_text\":\"\", \"color\": \"#FFFFFFAA\", \"background\": \"#1F0E11AA\", \"separator\":false, \"separator_block_width\": 0 }, {\"separator\":false, \"separator_block_width\": 0, \"background\": \"#FFFFFFAA\", \"color\": \"#FF0000\", \"min_width\":50, \"full_text\":\" %2s%%\"},", NULL, 1, 30 },
	{ cpu_freq, "{\"background\": \"#FFFFFFAA\", \"color\": \"#000000\", \"separator\":false, \"separator_block_width\": 0, \"full_text\":\" / %s \"},", NULL, NULL, 1, 0 },
	{ temp, "{\"full_text\":\"\", \"color\": \"#00FF00AA\", \"background\": \"#FFFFFFAA\", \"separator\":false, \"separator_block_width\": 0}, {\"separator\":false, \"separator_block_width\": 0, \"background\": \"#00FF00AA\", \"color\": \"#FFFFFF\", \"full_text\":\" %s°C \"},", "{\"full_text\":\"\", \"color\": \"#00FF00AA\", \"background\": \"#FFFFFFAA\", \"separator\":false, \"separator_block_width\": 0}, {\"separator\":false, \"separator_block_width\": 0, \"background\": \"#00FF00AA\", \"color\": \"#FF0000\", \"full_text\":\" %s°C \"},", "/sys/devices/platform/coretemp.0/hwmon/hwmon1/temp2_input", 1, 60},
	{ temp, "{\"full_text\":\"/ \", \"color\": \"#FFFFFF\", \"background\": \"#00FF00AA\", \"separator\":false, \"separator_block_width\": 0}, {\"separator\":false, \"separator_block_width\": 0, \"background\": \"#00FF00AA\", \"color\": \"#FFFFFF\", \"full_text\":\"%s°C \"},","{\"full_text\":\"/ \", \"color\": \"#FFFFFF\", \"background\": \"#00FF00AA\", \"separator\":false, \"separator_block_width\": 0}, {\"separator\":false, \"separator_block_width\": 0, \"background\": \"#00FF00AA\", \"color\": \"#FF0000\", \"full_text\":\"%s°C \"},", "/sys/class/hwmon/hwmon0/temp1_input", 5, 50 },
	{ keymap, "{\"full_text\":\"\", \"color\": \"#FFFFFFAA\", \"background\": \"#00FF00AA\", \"separator\":false, \"separator_block_width\": 0}, {\"separator\":false, \"separator_block_width\": 0, \"background\": \"#FFFFFFAA\", \"color\": \"#FF0000\", \"full_text\": \" %s \"},", NULL, NULL, 1, 0 },
	{ datetime, "{\"full_text\":\"\", \"color\": \"#1f0e11AA\", \"background\": \"#FFFFFFAA\", \"separator\":false, \"separator_block_width\": 0 }, {\"separator\":false, \"separator_block_width\": 0, \"background\": \"#1f0e11AA\", \"markup\":\"pango\",\"full_text\":\"%s\"},", NULL, " %a, %d %b ", 60, 0 },
	{ datetime, "{\"full_text\":\"\", \"color\": \"#FFFFFFAA\", \"background\": \"#1F0E11FA\", \"separator\":false, \"separator_block_width\": 0 }, {\"separator\":false, \"separator_block_width\": 0, \"background\": \"#FFFFFFAA\", \"color\":\"#000000\", \"markup\":\"pango\",\"full_text\":\"<span></span> <b>%s </b>\"}, {\"full_text\":\"\", \"color\": \"#1F0E1100\", \"background\": \"#FFFFFFAA\", \"separator\":false, \"separator_block_width\": 0 }]", NULL, "%H:%M", 1, 0 }
};
