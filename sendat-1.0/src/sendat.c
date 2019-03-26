#include "config.h"
#include <sys/inotify.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef HAVE_LIBMOSQUITTO
#include <mosquitto.h>
#else
#error "libmosquitto is not installed. Please install libmosquitto-dev"
#endif

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

void msq_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str){
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
void SIGTERM_handler(int sig){
  if (sig == SIGTERM){
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    fprintf(stderr,"Finish\n");
    exit(0);
  }
}

void mqtt_setup(){
  // Get parameters from enviroment variables
  char *host; // localhost by default
  int port;// 1883 by default
  int keepalive;// 60 by default
  bool clean_session;//  true by default
  char* user;
  char* passwd;

  char* env;

  env = getenv("HOST");
  host = env == NULL?"localhost":env;

  env = getenv("TOPIC");
  topic = env == NULL?"test":env;

  env = getenv("PORT");
  if (!env || strcmp(env,"")==0){
    port = 1883;
  }else{
    sscanf(env,"%d",&port);
  }

  env = getenv("KEEPALIVE");
  if (!env || strcmp(env,"")==0){
    keepalive=60;
  }else{
    sscanf(env,"%d",&keepalive);
  }

  env = getenv("CLEAN_SESSION");
  if (!env || strcmp(env,"")==0){
    clean_session =true;
  }else{
    sscanf(env,"%d",(int*)&clean_session);
  }

  env = getenv("MQTT_USER");
  if (!env || strcmp(env,"")==0){
    user =NULL;
  }else{
    user = env;
  }

  env = getenv("MQTT_PASSWORD");
  if (!env || strcmp(env,"") == 0){
    passwd =NULL;
  }else{
    passwd = env;
  }


  mosquitto_lib_init();
  mosq = mosquitto_new(NULL, clean_session, NULL);
  if(!mosq){
    fprintf(stderr, "Error: Out of memory.\n");
    exit(1);
  }

  mosquitto_log_callback_set(mosq, msq_log_callback);

  if (user != NULL && passwd != NULL){
    mosquitto_username_pw_set(mosq,user,passwd);
  }

  if(mosquitto_connect(mosq, host, port, keepalive)){
    fprintf(stderr, "Unable to connect to tcp://%s:%d.\n",host,port);
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
  int id,wd,len,pid;
  struct inotify_event* ev;
  char* directory;
  // Get pid and write to /etc/sendat/sendat.pid
  pid = getpid();
  FILE* pidfile = fopen("/etc/sendat/sendat.pid","w");
  fprintf(pidfile,"%d\n",pid);
  fclose(pidfile);

  // Get environment variable for directory to watch
  directory = getenv("DIRECTORY");
  directory = directory == NULL?".":directory;

  // Handle sigterm
  signal(SIGTERM,SIGTERM_handler);

  // setup mqtt
   mqtt_setup();

  // allocate space for inotify event descriptor
   ev = (struct inotify_event*) malloc(sizeof(struct inotify_event) + NAME_MAX + 1);
  // start inotify
  id = inotify_init();
  // watch file creation in current directory
  wd = inotify_add_watch(id, directory, IN_CREATE);

  while(1){
    // will be read when a file is created
    len=read(id, ev, sizeof(struct inotify_event) + NAME_MAX + 1);
    printf("Bucle\n");
    if(len == -1){
      perror("read: ");
    }
    else if (ev->wd == wd){
      // process event
      onNewFile(ev);
    }
  }
}
