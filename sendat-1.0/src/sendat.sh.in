#!/bin/bash
start() {
# Parse configuration file

HOST=$(grep "^HOST" /etc/sendat/sendat.conf | cut -d= -f2);
PORT=$(grep "^PORT" /etc/sendat/sendat.conf | cut -d= -f2);
KEEPALIVE=$(grep "^KEEPALIVE" /etc/sendat/sendat.conf | cut -d= -f2);
CLEAN_SESSION=$(grep "^CLEAN_SESSION" /etc/sendat/sendat.conf | cut -d= -f2);
TOPIC=$(grep "^TOPIC" /etc/sendat/sendat.conf | cut -d= -f2);
MQTT_USER=$(grep "^MQTT_USER" /etc/sendat/sendat.conf | cut -d= -f2);
MQTT_PASSWORD=$(grep "^MQTT_PASSWORD" /etc/sendat/sendat.conf | cut -d= -f2);

DIRECTORY=$(grep "^DIRECTORY" /etc/sendat/sendat.conf | cut -d= -f2);
BIN_PATH=$(grep "^BIN_PATH" /etc/sendat/sendat.conf | cut -d= -f2);

# Launch sendat with config as environment variables

HOST=$HOST PORT=$PORT KEEPALIVE=$KEEPALIVE CLEAN_SESSION=$CLEAN_SESSION TOPIC=$TOPIC MQTT_USER=$MQTT_USER MQTT_PASSWORD=$MQTT_PASSWORD DIRECTORY=$DIRECTORY sendat
}

stop(){
    kill $(cat /etc/sendat/sendat.pid)
}

case "$1" in
  start)
        start
        ;;
  stop)
        stop
        ;;
  status)
        status FOO
        ;;
  restart|reload|condrestart)
        stop
        start
        ;;
  *)
        echo $"Usage: $0 {start|stop|restart|reload|status}"
        exit 1
esac

exit 0
