#ifndef BUFFERBUILDER_H
#define BUFFERBUILDER_H

#include "objects.h"

int get_object_interval(object obj);
int buffer_object(Objects objects, object obj);
int buffer_objects(Objects objects);

#endif