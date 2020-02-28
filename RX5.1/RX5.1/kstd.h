#pragma once
// C/C++ snippets library

#define forever for (;;)
#define TIMER_SET(t) unsigned long int t = millis ()
#define TIMER_RST(t)                   t = millis ()
#define TIMER_GET(t) millis () - t