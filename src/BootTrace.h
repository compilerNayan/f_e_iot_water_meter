#ifndef BOOTTRACE_H
#define BOOTTRACE_H

#ifdef ESP_PLATFORM
extern "C" void BootTrace_OnAppMainEnter();
extern "C" void BootTrace_OnAppMainExit();
#else
#define BootTrace_OnAppMainEnter() ((void)0)
#define BootTrace_OnAppMainExit() ((void)0)
#endif

#endif // BOOTTRACE_H
