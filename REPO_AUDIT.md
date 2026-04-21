# Repository Audit: Bugs, Safety Concerns, and Improvements

Date: 2026-04-21

## High-priority bugs and safety concerns

1. **Data races on log queues in writer threads**
   - `ConsoleWriter()` and `FstreamWriter()` call `deque::empty()` without holding the corresponding mutex, while producer threads push under lock.
   - Accessing STL containers concurrently without synchronization is undefined behavior and may lead to intermittent crashes/corruption.
   - Affected code: `m_consoleLogDeque.empty()` and `m_fstreamLogDeque.empty()` checks done outside the locks.

2. **Global UTF converter is shared mutable state in a header**
   - `utfConverter` is defined in an anonymous namespace in `SingleLog.hpp`, and conversions call `to_bytes`/`from_bytes` without any locking.
   - Conversion facets and converter state are not guaranteed thread-safe for shared mutable use, and this logger is explicitly multi-threaded.

3. **Thread-unsafe reconfiguration of file output**
   - `SetLogFilePath()` updates `m_filePath`, opens `m_fileOut`, and sets buffer with no synchronization against `FstreamWriter()`, which concurrently flushes/writes.
   - This can race if callers reconfigure log path at runtime.

4. **Potentially unsafe format API exposure via macros**
   - `LOGF_*` macros pass format strings + variadic args into `std::snprintf` helper directly.
   - If untrusted input is ever used as format text, this can cause format-string vulnerabilities or crashes.

## Correctness and reliability issues

1. **Busy-wait writer loops**
   - Both writer threads poll queues every 2ms with `sleep_for`, causing unnecessary CPU wakeups and jitter under idle/low traffic.

2. **File open failures are silent**
   - `SetLogFilePath()` does not validate `m_fileOut.is_open()` / stream state and emits no diagnostic if open fails.

3. **Build script masks compiler failures**
   - `build.py` uses `os.system` and never checks return codes for each compile/link command, yet always prints success.

4. **No bounded queues/backpressure**
   - `m_consoleLogDeque` and `m_fstreamLogDeque` are unbounded; high log volume can lead to memory growth.

## Recommended improvements (general)

1. **Fix synchronization model**
   - Replace polling + raw deques with `std::condition_variable` + mutex-protected queue.
   - Move `empty()` checks under lock (or use wait predicates) and perform clean shutdown with exit flag + notify.

2. **Harden conversion and formatting**
   - Prefer UTF-8 native APIs and avoid shared global converter state.
   - Restrict `LOGF_*` usage to compile-time format strings (or use type-safe formatting, e.g. `std::format` / fmtlib in C++20+).

3. **Make configuration operations safe**
   - Guard `SetLogFilePath`, log-level setters, and file stream access with synchronization.
   - Consider atomic log levels and a dedicated config mutex for stream/file-path state.

4. **Improve failure handling and observability**
   - Return status from `SetLogFilePath()` and surface errors.
   - Add tests for concurrent logging, repeated reconfiguration, and shutdown under load.

5. **Improve build/test hygiene**
   - In `build.py`, check and propagate return codes, fail fast on any compile/link error.
   - Add CI (sanitizers, thread sanitizer where possible, warnings-as-errors, and unit/integration tests).

