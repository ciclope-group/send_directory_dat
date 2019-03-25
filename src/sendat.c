#include <sys/inotify.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <mosquitto.h>

#define NAME_MAX 60

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "Client"
#define TOPIC       "test"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

struct mosquitto *mosq = NULL;
char *topic = NULL;
extern int errno;

void msq_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
  /* Pring all log messages regardless of level. */

  switch(level){
    //case MOSQ_LOG_DEBUG:
    //case MOSQ_LOG_INFO:
    //case MOSQ_LOG_NOTICE:
  case MOSQ_LOG_WARNING:
  case MOSQ_LOG_ERR: {
    printf("%i:%s\n", level, str);
  }
  }

}

void mqtt_setup(){

  char *host = "localhost";
  int port = 1883;
  int keepalive = 60;
  bool clean_session = true;
  topic = "test";

  mosquitto_lib_init();
  mosq = mosquitto_new(NULL, clean_session, NULL);
  if(!mosq){
    fprintf(stderr, "Error: Out of memory.\n");
    exit(1);
  }

  mosquitto_log_callback_set(mosq, msq_log_callback);

  if(mosquitto_connect(mosq, host, port, keepalive)){
    fprintf(stderr, "Unable to connect.\n");
    exit(1);
  }
  int loop = mosquitto_loop_start(mosq);
  if(loop != MOSQ_ERR_SUCCESS){
    fprintf(stderr, "Unable to start loop: %i\n", loop);
    exit(1);
  }
}
void mqtt_publish(char* payload){
  mosquitto_publish(mosq,NULL,topic,strlen(payload),payload,0,0);
}

void onNewFile(struct inotify_event* ev){
  // Test if file is *.dat
  int len;
  len = strlen(ev->name);
  if(strcmp(&ev->name[len-4],".dat") != 0){
    return;
  }
  // If file is .dat, open and store content in variable
  int dat;
  char* content;
  struct stat datstat;

  dat= open(ev->name, O_RDONLY);
  fstat(dat,&datstat);
  len = sizeof(char)*datstat.st_size + 1;
  content = (char*)malloc(len);
  memset(content, 0, len);
  read(dat, content, len);

  close(dat);

  //publish content in mqtt
  mqtt_publish(content);
  free(content);
}
int main(void){
  int id,wd,len;
  struct inotify_event* ev;

  mqtt_setup();

  ev = (struct inotify_event*) malloc(sizeof(struct inotify_event) + NAME_MAX + 1);

  id = inotify_init();
  wd = inotify_add_watch(id, ".", IN_CREATE);

  while(1){
    len=read(id, ev, sizeof(struct inotify_event) + NAME_MAX + 1);
    if(len == -1){
      perror("read: ");
    }
    else if (ev->wd == wd){
      onNewFile(ev);
    }
  }
}
