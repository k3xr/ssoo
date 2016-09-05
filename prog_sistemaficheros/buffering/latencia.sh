#! /bin/sh
echo PID es $$ > /dev/tty
sleep 10
exec "$@"
