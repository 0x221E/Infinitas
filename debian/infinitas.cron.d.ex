#
# Regular cron jobs for the infinitas package.
#
0 4	* * *	root	[ -x /usr/bin/infinitas_maintenance ] && /usr/bin/infinitas_maintenance
