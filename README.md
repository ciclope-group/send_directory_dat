# Send directory dat

Utilidad que vigila permantentemente un directorio y envia por mqtt el contenido de los nuevos 
archivos *.dat que se crean en él

## Instalación

Para instalarlo, primero hay que [descargar el paquete](https://github.com/ciclope-group/send_directory_dat/blob/master/dist/sendat-1.0.tar.gz?raw=true) del repositorio de github.

```bash
wget https://github.com/ciclope-group/send_directory_dat/raw/master/dist/sendat-1.0.tar.gz
```
Después, extraer el contenido del paquete
```bash
tar -xzvf sendat-1.0.tar.gz
```
A continuación entrar al directorio e instalar el programa

```bash
cd sendat-1.0
./configure
make
make install
```
Es importante pasar a continuación al apartado de [Configuración](#Configuración)

## Desinstalación

Para desinstalar el programa, basta con entrar al mismo directorio donde se ha
clonado el repositorio y desinstalar el programa

```bash
cd sendat-1.0
make uninstall
```

## Configuración

Para configurar el servicio hay que editar el fichero `/etc/sendat/sendat.conf`. Sus
parámetros son los siguientes:

* `DIRECTORY`: Directorio donde se van a crear los archivos .dat
* `TOPIC`:
* `HOST`: Dirección de Internet (IP o hostname) del servidor de MQTT
* `PORT`: Puerto en el que escucha el servidor de MQTT
* `KEEPALIVE`: Segundos que se mantiene la conexión al servidor de MQTT. No es necesario cambiarlo
* `CLEAN_SESSION`: Iniciar una nueva sesión de MQTT con cada conexión. No es necesario cambiarlo
* `MQTT_USER`: Usuario para autenticarse con el servidor de MQTT.
* `MQTT_PASSWORD`: Contraseña para autenticarse con el servidor de MQTT.

## Inicio del programa

El programa está diseñado para ejecutarse como un proceso en segundo plano, por lo que basta con ejecutar

```bash
/etc/init.d/sendat start
```
También se inicia automáticamente al encender la máquina
