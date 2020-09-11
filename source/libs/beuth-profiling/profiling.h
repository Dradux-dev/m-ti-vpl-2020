#pragma once

#ifdef ENABLE_BEUTH_PROFILING
#include "chrome.h"
#include "exporter.h"
#include "function.h"
#include "observer.h"
#include "profiler.h"
#include "section.h"
#include "stopwatch.h"

#define BEUTH_PROFILING_CHROME_EXPORT() beuth::profiling::Profiler::Instance().addExporter<beuth::profiling::Chrome>()
#define BEUTH_PROFILING_BEGIN_SESSION(name) beuth::profiling::Profiler::Instance().beginSession(name)
#define BEUTH_PROFILING_END_SESSION() beuth::profiling::Profiler::Instance().endSession()
#define BEUTH_PROFILING_BEGIN_SECTION(name, categories) \
    beuth::profiling::Section section ## name(__FUNCTION__ + std::string("#") + #name, categories, &beuth::profiling::Profiler::Instance()); \
    section ## name.start()
#define BEUTH_PROFILING_END_SECTION(name) section##name.stop()
#define BEUTH_PROFILING_FUNCTION(categories) beuth::profiling::Function function##__LINE__(__PRETTY_FUNCTION__, "" categories, &beuth::profiling::Profiler::Instance())

#else

#include "stopwatch.h"

#define BEUTH_PROFILING_CHROME_EXPORT() do {} while(false)
#define BEUTH_PROFILING_BEGIN_SESSION(name) do {} while(false)
#define BEUTH_PROFILING_END_SESSION() do {} while(false)
#define BEUTH_PROFILING_BEGIN_SECTION(name, categories) do {} while(false)
#define BEUTH_PROFILING_END_SECTION(name) do {} while(false)
#define BEUTH_PROFILING_FUNCTION(categories) do {} while(false)
#endif
