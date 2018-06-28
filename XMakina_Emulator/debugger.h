#pragma once

#ifndef DEBUGGER_H
#define DEBUGGER_H

extern union XMakina_memory memory;
extern union XMakina_register_file register_file;
extern char program_name[MAX_PROG_NAME_SIZE];

void debugger();

#endif // !DEBUGGER_H
