#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
int main(void){
  char *env,*host,*topic,*user,*passwd;
  int port,keepalive;
  int clean_session;


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
    clean_session =1;
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
  fprintf(stdout,"host:[%s]\nport:[%d]\ntopic:[%s]\n",host,port,topic);

  return 0;
}
