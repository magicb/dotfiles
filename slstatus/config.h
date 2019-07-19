/* See LICENSE file for copyright and license details. */

/* interval between updates (in ms) */
const unsigned int interval = 1000;

/* text to show if no value can be retrieved */
static const char unknown_str[] = "n/a";

/* maximum output string length */
#define MAXLEN 2048

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
	{ run_command, ",[{\"color\":\"#CCFF90\", \"full_text\":\" %s\"},", "playerctl --player=vlc metadata vlc:nowplaying", 1 },
//	{ vol_perc, "{\"full_text\":\" %s%%\"},", "/dev/mixer", 1}, // modprobe snd-pcm-oss
	{ ram_used, "{\"full_text\":\" %s", NULL, 1},
	{ ram_total, "/%s\"},", NULL, 0 },
	{ disk_free, "{\"full_text\":\" %s/", "/", 30 },
	{ disk_total, "%s\"},", "/", 0 },
	{ disk_free, "{\"full_text\":\" %s/", "/home/magy", 30 },
	{ disk_total, "%s\"},", "/home/magy", 0 },
	{ netbytes_rx, "{\"min_width\":60,\"full_text\":\"▼ %s\"},", "enp3s0", 1 },
	{ netbytes_tx, "{\"min_width\":60,\"full_text\":\"▲ %s\"},", "enp3s0", 1 },
//	{ netspeed_rx, "(%s/", "enp3s0" },
//	{ netspeed_tx, "%s)", "enp3s0" },
	{ run_command, "{\"full_text\":\" %s\"},", "wget -O- \"http://proxy.datakrat.ru/sams/main.php?show=exe&function=userform&userid=5767824c41dce\" --no-proxy -q --no-cookies --header \"Cookie: domainuser=lam; gauditor=0; clickedFolder=3%5E4%5E156%5E\" | awk '/kb/ {print $1 \".\" $3 \"Gb\"}'", 60*5 },
	//{ run_command, "{\"full_text\":\"%s\"},", "sh ~/proxy.sh", 60},
	{ cpu_perc, "{\"min_width\":50, \"full_text\":\" %2s%%\"},", NULL, 1 },
	{ cpu_freq, "{\"full_text\":\"%s\"},", NULL, 1 },
	{ temp, "{\"full_text\":\" %s°C\"},", "/sys/devices/platform/coretemp.0/hwmon/hwmon0/temp2_input", 1},
	{ keymap, "{\"markup\":\"pango\",\"full_text\":\"<span foreground='#00F0FF'>%s</span>\"},", NULL, 1 },
	{ datetime, "{\"markup\":\"pango\",\"full_text\":\"%s\"},", "%a, %d %b", 60 },
	{ datetime, "{\"markup\":\"pango\",\"color\":\"#2DD500\",\"full_text\":\"<span font_desc='FontAwesome'></span> <b>%s</b>\"}]", "%H:%M", 1 }
};
