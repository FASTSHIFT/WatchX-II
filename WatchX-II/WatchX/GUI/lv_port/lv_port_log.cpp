#include "GUI/DisplayPrivate.h"

#if LV_USE_LOG != 0
/* Serial debugging */
static void log_print(lv_log_level_t level, const char * file, uint32_t line, const char * function, const char * description)
{
    //Serial.printf("%s@%d->%s(%s)\r\n", file, line, function, description);
    Serial.print(file);
    Serial.print('@');
    Serial.print(line);
    Serial.print("->");
    Serial.print(function);
    Serial.print('<');
    Serial.print(description);
    Serial.println('>');
}
#endif

void lv_port_log_init()
{
#if LV_USE_LOG != 0
    lv_log_register_print_cb(log_print); /* register print function for debugging */
#endif
}
