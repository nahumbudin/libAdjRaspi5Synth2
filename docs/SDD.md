# Software Design Document (SDD) — libAdjRaspi5Synth2

- Project: **AdjHeart Raspberry Pi 5 Synthesizer Engine 2nd Version**
- Repository: https://github.com/nahumbudin/libAdjRaspi5Synth2
- Default branch: `master`
- Primary languages: C++ (dominant), C (small portion)
- SDD generated: 2026-04-28

## 1. Purpose and Scope

### 1.1 Purpose
`libAdjRaspi5Synth2` is a C++ library intended to run on a **Raspberry Pi 5B** and implement a **multi-core, polyphonic, modular music synthesizer engine**. The codebase integrates:
- a custom synthesizer engine (`AdjSynth`)
- instrument “wrappers”/managers (e.g., FluidSynth, Hammond Organ, Analog Synth)
- MIDI I/O and routing (ALSA MIDI, external MIDI interfaces, Bluetooth MIDI)
- audio output plumbing (JACK, ALSA)
- configuration/preset storage (XML-based settings / presets)
- hardware control interfaces (I2C, serial control box)

### 1.2 Scope
This SDD focuses on the *library-internal design* as represented in the repository:
- module decomposition (directories)
- major classes and responsibilities
- runtime flow (init, audio start/stop, MIDI flow, settings/presets)
- threading model (audio callback integration, CPU utilization thread, I2C polling)
- key interfaces and extension points

Out of scope:
- UI/application layer consuming this library (not present here)
- full DSP algorithmic detail inside `AdjSynth` and `DSP/` (only referenced at high level)

---

## 2. High-Level Architecture

### 2.1 Architectural Overview
The library is organized into functional subsystems (directory-per-subsystem). At runtime, a central singleton object **`ModSynth`** acts as the orchestrator:
- initializes and owns the main synthesizer engine (`AdjSynth`)
- registers an “update tasks” callback aligned to the audio processing cycle
- constructs and registers multiple “instruments” via an `InstrumentsManager`
- manages system-level MIDI and audio connectivity helpers
- manages presets and settings via `Settings` and `PatchsHandler`
- starts background services (CPU utilization monitoring, I2C polling threads)

Conceptually:

- **Control Inputs**
  - ALSA MIDI clients (hardware/software)
  - Bluetooth MIDI input -> ALSA output client
  - Serial-based “control box” -> ALSA output client
  - External MIDI interface board via serial / I2C

- **Event Processing**
  - MIDI parsing/handling and dispatch to selected instrument/synth
  - Per-audio-cycle tasks (e.g., portamento update)

- **Sound Generation**
  - `AdjSynth` (custom synthesis engine)
  - `InstrumentFluidSynth` (FluidSynth-based instrument)
  - other instruments (Hammond/Analog/MIDI player/mapper)

- **Audio Output**
  - JACK and/or ALSA driver (selected by configuration)

- **Configuration**
  - XML settings read/write via `Settings` + `utils/xmlFiles`
  - preset parameters (formerly “patches”)

### 2.2 Repository Module Map (by top-level directories)
From repository root:
- `AdjSynth/` — core custom synth engine and polyphony
- `DSP/` — DSP primitives and voice processing (referenced by `AdjSynth`)
- `Audio/` — audio management glue (likely used by AdjSynth’s `audio_manager`)
- `ALSA/` — ALSA MIDI clients/outputs and system control helpers
- `Jack/` — JACK connection control/scan utilities
- `MIDI/` — MIDI external interface, ALSA MIDI queue clients, parsers/handlers
- `Instrument/` — instrument abstractions and concrete instruments
- `Settings/` — settings manager, parameter model, XML reading/writing integration
- `Patches/` — preset/patch handling and persistence
- `I2C/` — I2C interface implementation + polling thread
- `CPU/` — CPU snapshot/utilization measurement utilities
- `Bluetooth/` — Bluetooth MIDI plumbing (used via ALSA client output)
- `Serial/` — serial port clients (used for control box / external MIDI)
- `utils/` — utilities such as XML helper code

---

## 3. Key Design Elements

## 3.1 Central Orchestrator: `ModSynth` (Singleton)

### 3.1.1 Responsibility
`ModSynth` is the **main modular synthesizer library object**. It is responsible for:
- system discovery / connection helpers:
  - `AlsaMidiSysControl` (uses system `aconnect` commands)
  - `JackConnections` (uses system `jack_lsp` commands)
- creating MIDI input/output “clients”:
  - Bluetooth ALSA output client
  - control box ALSA output clients
- constructing instruments and registering them with an `InstrumentsManager`
- configuring audio driver/sample rate/block size
- initializing the `AdjSynth` engine, its programs, voices, polyphony, JACK
- loading general settings (with default fallback)
- initializing presets (temp preset + preset repository init)
- starting background threads (CPU utilization, I2C polling)
- providing a public API surface for volume/audio settings, preset persistence, etc.

### 3.1.2 Main Public Interface (selected)
(From `modSynth.h`)
- `static ModSynth* get_instance()`
- `int init()`
- `void on_exit()`
- `int start_audio()`, `int stop_audio()`
- `int set_sample_rate(int rate[, bool restart])`
- `int set_audio_block_size(int size[, bool restart])`
- `int set_audio_driver_type(int driver[, bool restart])`
- `void set_master_volume(int vol)`, `int get_master_volume()`
- `void set_fluid_synth_volume(int vol)`, `int get_fluid_synth_volume()`
- MIDI API (currently stubbed in `modSynth.cpp`):
  - `note_on`, `note_off`, `change_program`, `controller_event`, etc.
- Settings/preset file operations:
  - `save_adj_synth_patch_file(...)`, `open_adj_synth_patch_file(...)`
  - `save_mod_synth_general_settings_file(...)`, `open_mod_synth_general_settings_file(...)`

### 3.1.3 Key Owned Components
`ModSynth` holds pointers/references to:
- `AdjSynth* adj_synth`
- `InstrumentsManager* instruments_manager`
- concrete instruments:
  - `InstrumentFluidSynth* fluid_synth`
  - `InstrumentHammondOrgan* hammond_organ`
  - `InstrumentAnalogSynth* analog_synth`
  - `InstrumentMidiPlayer* midi_player`
  - `InstrumentMidiMapper* midi_mapper`
  - `InstrumentControlBoxEventsHandler* control_box_events_handler`
- system I/O:
  - `AlsaMidiSysControl* alsa_midi_system_control`
  - `JackConnections* jack_connections`
  - `MidiExtInterface* midi_ext_interface`
  - `AlsaBtClientOutput* bt_alsa_out`
  - `ControlBoxClientAlsaOutput* control_box_alsa_out`
  - `ControlBoxExtMidiInClientAlsaOutput* control_box_ext_midi_in_alsa_out`
- hardware interface:
  - `I2Cinterface* i2c_interface_main_control`
  - `I2Cinterface* i2c_interface_control_board`
- configuration:
  - `Settings* general_settings_manager`
  - `Settings* fluid_synth_settings_manager`
  - `_settings_params_t active_general_synth_settings_params`
  - `_settings_params_t active_fluid_synth_settings_params`
  - `PatchsHandler* patches_handler`

### 3.1.4 Global/Static State
- `static ModSynth* mod_synth` — singleton instance
- `static int cpu_utilization` — updated by CPU utilization thread
- `static std::mutex settings_handler_mutex` — global settings handling mutex
- `uint16_t midi_channel_synth[...]` — maps MIDI channels/programs to assigned synthesizer

---

## 3.2 Synthesis Engine: `AdjSynth`
`AdjSynth` is referenced as the core custom synth engine. `ModSynth::init()` wires settings into it and calls:
- `adj_synth->init_synth_programs()`
- `adj_synth->init_synth_voices()`
- `adj_synth->init_poly()`
- `adj_synth->init_jack()`
and later audio control:
- `adj_synth->start_audio(audio_driver, sample_rate, audio_block_size)`
- `adj_synth->stop_audio()`

`ModSynth` also registers an audio-cycle callback via:
- `adj_synth->audio_manager->register_callback_audio_update_cycle_start_tasks(...)`

This indicates the audio engine has a periodic update loop where `ModSynth::update_tasks()` runs tasks that must remain synchronized with DSP/audio processing (e.g., portamento updates).

---

## 3.3 Instruments Subsystem
The `Instrument/` directory provides instrument abstractions and implementations.
From `modSynth.cpp`, instruments are instantiated and registered with `InstrumentsManager` using string keys:
- FluidSynth instrument (`InstrumentFluidSynth`)
- Hammond Organ (`InstrumentHammondOrgan`)
- Analog Synth (`InstrumentAnalogSynth`)
- MIDI Mapper (`InstrumentMidiMapper`)
- MIDI Player (`InstrumentMidiPlayer`)
- Control Box handler (`InstrumentControlBoxEventsHandler`)

`InstrumentsManager` appears to be a registry/factory for instruments used by the system (and likely by MIDI routing logic).

---

## 3.4 MIDI Subsystem
The system uses ALSA MIDI, and includes code to scan and connect clients dynamically.

Key components:
- `AlsaMidiSysControl` (ALSA clients scanning and connection control via `aconnect`)
- ALSA clients for:
  - Bluetooth MIDI in -> ALSA output source
  - serial control box MIDI -> ALSA output source
  - external MIDI in from control box -> ALSA output source
- `MidiExtInterface` (external MIDI controllers interface; initializes serial port services)

There is code to connect a control box input to its handler output by resolving client IDs and calling:
- `alsa_midi_system_control->connect_midi_clients(in_dev, 0, out_dev, 0);`

The `ModSynth` MIDI event API (`note_on`, `controller_event`, etc.) exists but is currently empty in `modSynth.cpp`, implying either:
- MIDI is handled elsewhere (ALSA sequencer event handler path), or
- these are placeholders pending integration / refactor.

---

## 3.5 Audio Subsystem
Audio output is selected by a driver type (`_AUDIO_JACK`, `_AUDIO_ALSA`, etc.). `ModSynth` stores:
- `audio_driver`
- `sample_rate`
- `audio_block_size`

Start/stop operations:
- `ModSynth::start_audio()` delegates to `adj_synth->start_audio(...)`
- `ModSynth::stop_audio()` delegates to `adj_synth->stop_audio()`

This indicates `AdjSynth` is the primary audio engine endpoint, while `ModSynth` is responsible for choosing parameters and lifecycle.

---

## 3.6 Settings and Presets (Patches)

### 3.6.1 Settings Model
The library uses a `Settings` class that can:
- set parameters (`set_int_param`, etc.)
- read/write settings files (XML)

Two primary settings parameter structures are maintained:
- general synth settings parameters
- FluidSynth settings parameters

### 3.6.2 Presets/Patches
The codebase uses “patches” terminology but has refactoring notes indicating “rename patches to preset parameters.”

Capabilities include:
- saving/loading AdjSynth preset parameters (patch files) to/from XML
- saving/loading AdjSynth settings parameters to/from XML
- saving/loading ModSynth general settings

Notable post-load actions for patches:
After reading an AdjSynth patch, wavetable generation and MSO wavetable calculations are triggered:
- `generate_wavetable(...)`
- `calc_segments_lengths(...)`
- `calc_wtab(...)`

This is important: presets are not just “values”; they require **derived data recomputation**.

---

## 3.7 I2C Hardware Control
`ModSynth` constructs two I2C interfaces with configured bus/device addresses and register maps:
- main control device
- control board device

Both start polling threads via:
- `start_i2c_polling_thread()`

This indicates continuous background monitoring and/or command reception over I2C.

---

## 3.8 CPU Utilization Monitoring
A dedicated pthread is started:
- `start_cheack_cpu_utilization_thread()` (note typo “cheack” is consistent in code)

Thread function:
- takes periodic CPU snapshots and computes utilization percentage, writing to `ModSynth::cpu_utilization`.

This supports displaying CPU load (important in low-latency audio/DSP systems).

---

## 4. Runtime Behavior / Lifecycle

## 4.1 Initialization Sequence (observed)
Primary flow (from `ModSynth::ModSynth()` in `modSynth.cpp`):
1. Initialize system connection helpers (`AlsaMidiSysControl`, `JackConnections`)
2. Initialize ALSA MIDI output clients for BT/control box
3. Refresh ALSA client lists and capture client name strings
4. Initialize external MIDI interface object
5. Acquire `AdjSynth` instance
6. Apply default audio config (driver, block size, sample rate)
7. Acquire `InstrumentsManager` and create/register instruments
8. Configure and start I2C polling for main control + control board
9. Create/register control box events handler instrument
10. Refresh ALSA and JACK client lists
11. Connect control box input to handler output via ALSA client IDs
12. Acquire patch handler singleton
13. Initialize MIDI channel assignment state
14. Configure default settings directory path (hard-coded)
15. Create settings managers
16. Call `init()` to initialize `AdjSynth` and default settings
17. Register audio-cycle callback into `AdjSynth`’s audio manager
18. Load general settings file; if failed -> set defaults
19. Initialize a temp preset and preset system (`ModSynthPatches::init()`)
20. Start CPU utilization monitoring thread

## 4.2 Shutdown Sequence
`~ModSynth()` calls `on_exit()`:
- deinitialize FluidSynth
- stop CPU utilization thread
- TODO: stop other background services (I2C polling threads, ALSA threads, etc.)

---

## 5. Concurrency and Threading Model

### 5.1 Threads/Async Tasks Identified
- Audio processing/update thread(s) (inside `AdjSynth` / `audio_manager`)
- CPU utilization pthread (created by `pthread_create`)
- I2C polling threads (inside `I2Cinterface`)
- ALSA output threads (e.g., `startAlsaOutThread()` for control box ALSA outputs)

### 5.2 Synchronization
- `ModSynth::settings_handler_mutex` exists as a global mutex for settings handling.
- Ensure any settings mutations that can occur concurrently with audio thread are guarded (exact usage should be audited in `Settings/` and parameter set paths).

### 5.3 Real-time Considerations
- Audio-thread callbacks must avoid blocking operations (file I/O, system calls, dynamic allocation).
- Derived-data computations after patch loads (wavetable creation, MSO tables) should ideally not run on audio thread.

---

## 6. Interfaces and APIs

## 6.1 Library Entry Points
### 6.1.1 `ModSynth` Public API
`ModSynth` is effectively the library’s facade.

### 6.1.2 `libAdjRaspi5Synth2.h`
This header currently includes `modSynth.h` and declares:
- `void callback_message_id(int);`

This suggests the “stable API header” may be incomplete or under migration. Consumers likely include `modSynth.h` directly at present.

## 6.2 External Dependencies / System Interfaces
- ALSA MIDI utilities and clients
- JACK utilities and connection enumeration
- Bluetooth RFCOMM SPP (via ALSA BT client output)
- I2C device access on Raspberry Pi
- POSIX threads (`pthread_*`)
- filesystem paths (hard-coded default settings path under `/home/pi/...`)

---

## 7. Data Design

### 7.1 Key Data Structures
- `_settings_params_t` — central settings/preset parameter structure used across systems
- `midi_channel_synth[]` — mapping of MIDI channel/program to synth assignment state
- Preset temporary structure: `preset_temp` and human-readable `preset_temp_summary_text`

### 7.2 Persistence Format
- XML files for settings and presets.
- `utils/xmlFiles` used to map and manage XML file naming.

---

## 8. Error Handling Strategy (observed)
- Settings load: if reading fails, defaults are applied.
- Many operations return integer status codes (`0` success, negative/enum on error).
- There are macros like `return_val_if_true(...)` used for parameter validation.

Recommended to standardize:
- a consistent `enum`/error code type for public API boundaries
- logging hooks (especially for runtime on embedded hardware)

---

## 9. Build / Tooling Notes (observed)
Repository includes Visual Studio / VisualGDB artifacts:
- `.sln`, `.vcxproj`, `.visualgdb/`, `.vgdbsettings`

This indicates development is commonly done using Visual Studio tooling targeting Raspberry Pi (cross-compile / remote).

---

## 10. Known Gaps / TODOs (as seen in code)
- Shutdown does not yet stop all services (I2C polling threads, ALSA threads, etc.)
- MIDI public methods in `ModSynth` are stubbed (empty implementations)
- Settings directories are hard-coded; there’s a TODO to load them from config
- Some comments indicate ongoing refactoring from “patches” to “preset parameters”

---

## 11. Suggested Extensions (optional)
If you want to evolve this design further, the next high-impact improvements would be:
1. **Explicit lifecycle manager** for thread shutdown and resource cleanup (RAII + join/stop)
2. **Non-hardcoded configuration** for settings paths and device addresses
3. **Clear public API boundary**: define a stable C/C++ API in `libAdjRaspi5Synth2.h` (and document it)
4. **MIDI routing design doc**: document how ALSA seq events flow into instrument selection and voice triggering

---

## Appendix A — Primary Sources Used
- `modSynth.h`, `modSynth.cpp` (main architecture)
- repository root structure (module map)
- repo metadata and language breakdown
