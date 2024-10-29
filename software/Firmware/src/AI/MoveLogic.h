#ifndef MAINLOGIC_H
#define MAINLOGIC_H

#include <Arduino.h>
#include <QueueArray.h>

extern QueueArray<String> commandQueue;

void parseAndEnqueueCommands(const String& commands);

void enqueueCommand(const String& command);

void processNextCommand();

void IA_Movements(String response);

#endif
