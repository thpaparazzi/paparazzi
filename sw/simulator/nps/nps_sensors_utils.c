#include "nps_sensors_utils.h"

//#include <string.h>
#include "6dof.h"
#include "math/pprz_algebra.h"

void UpdateSensorLatency(double time, gpointer cur_reading, GSList* history, 
						 double latency, gpointer sensor_reading) {
  /* add new reading */
  struct BoozDatedSensor* cur_read = g_new(struct BoozDatedSensor, 1);
  cur_read->time = time;
  cur_read->value = (struct DoubleVect3*) g_memdup(cur_reading, sizeof(struct DoubleVect3));
  history = g_slist_prepend(history, cur_read);
  /* remove old readings */
  GSList* last =  g_slist_last(history);
  while (last && ((struct BoozDatedSensor*)last->data)->time < time - latency) {
    history = g_slist_remove_link(history, last);
	g_free(((struct BoozDatedSensor*)last->data)->value);
    g_free((struct BoozDatedSensor*)last->data);
    g_slist_free(last);
    last = g_slist_last(history);
  }
  /* update sensor        */
  //g_memmove((gpointer)sensor_reading, (gpointer)((struct BoozDatedSensor*)last->data)->value, sizeof(struct DoubleVect3));
  VECT3_COPY(*((struct DoubleVect3*)sensor_reading), *((struct BoozDatedSensor*)last->data)->value);
}