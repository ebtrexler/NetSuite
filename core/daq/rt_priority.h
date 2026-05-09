// core/daq/rt_priority.h
//
// Cross-platform helper for elevating the calling thread's scheduling
// priority. Used by DaqThread to pin the hot loop ahead of UI and
// background work, as laid out in Developer_Guide.md §4.6.
//
// Failure to elevate is NEVER fatal — many systems require capabilities
// or administrator rights that a user app won't have. The caller should
// log `last_error()` and carry on; the run will still work, it just
// won't have hard real-time guarantees.
//
// Header-only; kept small and compiler-portable.

#ifndef RT_PRIORITY_H
#define RT_PRIORITY_H

#include <string>

#if defined(_WIN32)
  // Prevent <windows.h> from defining min/max as preprocessor macros —
  // otherwise they clobber std::min / std::max in every TU that
  // transitively includes this header.
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif
  #include <windows.h>
#else
  #include <pthread.h>
  #include <sched.h>
  #include <errno.h>
  #include <cstring>
#endif

namespace rt_priority {

enum class Result {
    Ok,              // priority was elevated
    NotSupported,    // platform won't honor RT; best we could do
    PermissionDenied // OS refused (need CAP_SYS_NICE / admin)
};

struct Report {
    Result status;
    std::string detail;   // human-readable description for logging
};

/// Elevate the CURRENT thread's scheduling priority for real-time work.
/// Returns a Report; callers should log and continue on non-Ok.
inline Report elevate_current_thread() {
#if defined(_WIN32)
    // Process-level class stays NORMAL on purpose — elevating the whole
    // process starves device drivers. Only the thread goes up.
    if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL)) {
        return {Result::PermissionDenied,
                "SetThreadPriority failed, error=" +
                std::to_string(static_cast<long long>(GetLastError()))};
    }
    return {Result::Ok, "THREAD_PRIORITY_TIME_CRITICAL"};

#elif defined(__linux__)
    sched_param sp{};
    sp.sched_priority = 80;  // well above 0 (CFS), well below 99 (kernel)
    int rc = pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp);
    if (rc == 0) {
        return {Result::Ok, "SCHED_FIFO prio=80"};
    }
    if (rc == EPERM) {
        return {Result::PermissionDenied,
                "pthread_setschedparam EPERM (need CAP_SYS_NICE or "
                "/etc/security/limits.d rule)"};
    }
    return {Result::NotSupported,
            std::string("pthread_setschedparam: ") + std::strerror(rc)};

#elif defined(__APPLE__)
    // macOS honors SCHED_FIFO syntactically but does not give you hard
    // real-time without joining a QoS class / time-constraint policy
    // via thread_policy_set. For our use (simulation + mock DAQ on Mac)
    // we try the POSIX path and report NotSupported on failure.
    sched_param sp{};
    sp.sched_priority = 47;  // macOS default max is typically 47 for user
    int rc = pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp);
    if (rc == 0) {
        return {Result::Ok, "SCHED_FIFO prio=47 (macOS best-effort)"};
    }
    return {Result::NotSupported,
            std::string("macOS priority elevation not available: ") +
            std::strerror(rc)};

#else
    return {Result::NotSupported, "no RT-priority path for this platform"};
#endif
}

inline const char* to_string(Result r) {
    switch (r) {
        case Result::Ok:                return "Ok";
        case Result::NotSupported:      return "NotSupported";
        case Result::PermissionDenied:  return "PermissionDenied";
    }
    return "?";
}

}  // namespace rt_priority

#endif  // RT_PRIORITY_H
