# The GVNR

A cutting-edge hybrid distortion and multi-effect audio plugin for macOS. Built with JUCE 8 and C++20, The GVNR delivers multi-polar waveform processing, multiband routing, morphing filters, tuned feedback, and deep modulation -- with switchable dark and light themes accented in pink and purple.

Inspired by Minimal Audio's Rift. Compatible with Logic Pro X (AU), Ableton Live (VST3), and FL Studio (VST3).

---

## Features

### Multi-Polar Distortion Engine

30 distortion algorithms organized into five categories, each applied independently to the positive and negative halves of the waveform:

| Category | Algorithms |
|----------|-----------|
| **Waveshapers** | Soft Clip, Hard Clip, Tube Saturation, Tape Saturation, Transistor, Asymmetric Clip |
| **Wavefolders** | Sine Fold, Triangle Fold, Chebyshev, Parabolic, Exponential Fold, Multi-Fold |
| **Noise** | Noise Gate Crush, Noise Blend, Noise Ring Mod, Noise AM, Granular Noise, Static |
| **Bit Crush** | 4-bit, 8-bit, 12-bit, Variable Bit, Stepped, Quantize |
| **Sample Rate** | Decimate 2x, Decimate 4x, Decimate 8x, Variable Rate, Jitter, Drift |

Stack 1--6 distortion stages with smooth or hard blend modes between polarities. Process in stereo, mono, or mid/side.

### Morphing Filters

24 stereo filter types with continuous morphing:

- Standard: LP 12/24dB, HP 12/24dB, Band Pass, Notch, All Pass, Peak
- Shelving: Low Shelf, High Shelf
- Comb: Comb+, Comb-
- Phaser: 1/2/4/6-stage
- Vocal: Vowel A, E, I, O
- Special: Formant Shift, Resonator, LP-to-HP Morph, LP-to-BP Morph

Position filters before or after the distortion engine, each with independent cutoff, resonance, and morph controls.

### Melodic Feedback

Stereo delay line with pitch-snapped resonant feedback that turns delays into tuned instruments:

- Ping-pong mode
- Sync to free time (ms), frequency (Hz), or BPM-locked divisions (1/16 through 1/1, triplets, dotted)
- MIDI pitch tracking from keyboard input
- **Key selection** -- choose any root key (C through B) to transpose scale snapping into any musical key
- Intuitive pitch snapping with scale presets across 10 scales: Chromatic, Major, Minor, Pentatonic Major/Minor, Blues, Dorian, Mixolydian, Harmonic Minor, Whole Tone
- Key + scale combine to give you pitch-corrected basslines in any key signature (e.g. D Minor, G# Blues, F Dorian)
- Tune absolutely everything -- scale presets, key selection, and MIDI tracking blur the line between effect and instrument

### Multiband Crossover

3-band Linkwitz-Riley crossover with per-band toggle for distortion, filtering, and feedback. Adjustable low and high crossover frequencies let you send any combination of the effects chain through the multi-band crossover.

### Glassmorphism UI

Moog-inspired digital aesthetic with a premium feel:

- **Glass panels** -- every panel uses frosted glass with semi-transparent backgrounds, subtle top-edge light refraction, and thin white-alpha borders
- **3D rotary knobs** -- radial gradient knob caps with drop shadows, chrome bezel rings, and neon pink glow bloom behind the active value arc
- **Instagram gradient waveform** -- the oscilloscope visualizer uses a full-spectrum warm-to-cool gradient (yellow, orange, red, pink, magenta, purple, blue) with a bloom glow pass for depth
- **Neon pink highlights** against a deep near-black background with subtle blue tint
- **Glass combo boxes, toggles, and buttons** -- every control has depth gradients, glass borders, and active-state glow effects

### Choose Your Look

Switch between beautifully designed dark and light UI modes with a single click. The theme toggle in the top bar instantly switches all panels, knobs, and backgrounds between the two palettes while keeping the signature pink and purple accents.

### Drag-and-Drop Modulation

Intuitive modulation routing via drag and drop. Each modulation source (Envelope Follower, LFO, Macro 1, Macro 2) has a colored drag handle that can be dropped onto any knob to create a modulation route:

- **Envelope Follower** -- amplitude-reactive modulation with configurable attack and release
- **Morphing LFO** -- sine, triangle, saw up/down, square, and random S&H with continuous shape morphing and tempo sync
- **2 Curve Sequencers** -- draw custom modulation shapes with bezier interpolation, randomize, save/load
- **2 Macro Controls** -- one-knob access to multiple parameter mappings
- **Visual feedback** -- purple indicator dots appear on knobs with active modulation routes
- **Right-click management** -- right-click any modulated knob to remove individual routes

### Two Powerful Views

- **Play View** -- simplified interface with drive, dry/wet, macros, input/output gain, and a real-time waveform oscilloscope fed from the processor output
- **Advanced View** -- full parameter access with six dedicated panels: Distortion, Pre-Filter, Post-Filter, Feedback, Multiband, and Modulation

### Flexible Preset Library

50 factory presets across 10 categories with tag-based searching, favorites, and category filtering:

| Category | Presets |
|----------|---------|
| Subtle / Warm | Warm Tube, Gentle Tape, Analog Warmth, Silk, Vintage Glow |
| Aggressive | Razorblade, Transistor Fury, Scorched Earth, Brutal, Fuzz Face |
| Bass Design | Sub Destroyer, Low End Theory, Seismic, Bass Face, Deep Impact |
| Bitcrusher / Lo-Fi | 8-Bit Dreams, Lo-Fi Crunch, Retro Digital, Chiptune, VHS Warm |
| Resonant Feedback | Resonance Machine, Harmonic Feedback, Minor Key Echo, Ping Pong Resonator, Blues Feedback |
| Filter Sweep | Auto Wah, Phaser Dreams, Slow Sweep, Rhythmic Filter, Bandpass Motion |
| Multiband Split | Three Way Split, Clean Bottom Dirty Top, Mid Scoop Crush, Hi-Fi Lows, Full Spectrum Chaos |
| Creative / Experimental | Sine Folder, Alien, Glitch Machine, Textural Noise, Dimension Shift |
| Vocal / Formant | Vowel A, Vowel E, Vowel I, Vowel O, Talking Machine |
| Classic | Marshall Stack, Neve Preamp, 1176 Crunch, Space Echo, Fairchild |

- **Category filtering** -- dropdown to browse by category (Subtle, Aggressive, Bass, Lo-Fi, Feedback, Filter, Multiband, Creative, Vocal, Classic)
- **Favorites** -- star any preset and browse the Favorites pseudo-category
- **Tag search** -- search presets by tag using the search field
- **Smart randomization** -- generate musical variations of any preset with carefully tuned intelligent randomization
- Save and load user presets in `.gvnr` XML format

### In-App Tooltips

Built-in tooltip overlay provides contextual help on hover, making it easy to learn every parameter.

---

## Compatibility

| DAW | Format | Platform | Tested |
|-----|--------|----------|--------|
| Logic Pro X | AU (aufx) | macOS | `auval` validated |
| Ableton Live | VST3 | macOS / Windows | Supported |
| FL Studio | VST3 | macOS / Windows | Supported |
| Standalone | App / EXE | macOS / Windows | Included |

**macOS:** 10.13 or later. Universal binary (arm64 + x86_64).
**Windows:** 10 or later (64-bit). VST3 installs to `C:\Program Files\Common Files\VST3\`.
No internet connection required after installation.

---

## UI Design

The interface follows Apple's Human Interface Guidelines adapted for audio production, with switchable dark and light themes:

### Dark Theme (default)

| Token | Hex | Usage |
|-------|-----|-------|
| Background | `#080810` | Window background (near-black with blue tint for depth) |
| Surface | `#1A1A2E` | Glass panel fills (rendered at ~60% alpha) |
| Surface elevated | `#252540` | Hover states, modal glass backgrounds |
| Accent | `#FF66C4` | Neon pink -- interactive elements, active knob arcs, glow effects |
| Accent dim | `#D94FA3` | Pressed states |
| Highlight | `#9B5CF6` | Modulation indicators, curve editor, secondary accent |
| Glass border | `#FFFFFF20` | Semi-transparent white panel borders |
| Glass highlight | `#FFFFFF18` | Top-edge light refraction on glass panels |
| Accent glow | `#FF66C430` | Focused control glow borders |
| Text primary | `#F5F5F7` | Labels and values |
| Text secondary | `#86868B` | Section headers, secondary info |
| Divider | `#2D2D3A` | Panel borders (with blue tint) |
| Knob track | `#3A3A4A` | Unfilled knob arcs |

### Light Theme

| Token | Hex | Usage |
|-------|-----|-------|
| Background | `#F5F5F7` | Window and panel backgrounds |
| Surface | `#FFFFFF` | Cards and panel fills |
| Surface elevated | `#E8E8ED` | Hover states, modal backgrounds |
| Accent | `#FF66C4` | Interactive elements, active knob arcs, waveform |
| Accent dim | `#D94FA3` | Pressed states |
| Highlight | `#7C3AED` | Modulation indicators, curve editor, secondary accent |
| Text primary | `#1A1A1A` | Labels and values |
| Text secondary | `#6B6B70` | Section headers, secondary info |
| Divider | `#D4D4D8` | Panel borders |
| Knob track | `#C0C0C5` | Unfilled knob arcs |

Design principles:

- Glassmorphism panels with frosted backgrounds, top-edge light refraction, and thin white-alpha borders
- 3D rotary knobs with radial gradient caps, drop shadows, chrome bezels, and neon glow bloom
- Instagram-gradient waveform oscilloscope (warm yellow through pink/magenta to cool blue/purple)
- Subtle radial glow behind the waveform area on Play View
- Glass top-bar navigation with bottom divider
- Vertical background gradient for depth instead of flat black
- Large corner radii (12--16px) on all panels
- System font (SF Pro on macOS)
- Uppercase, letter-spaced section labels
- Logo watermark in the bottom-left corner (1/3 window width, low opacity)
- 1000x700px default window, resizable with fixed 10:7 aspect ratio

---

## Signal Flow

```
Audio In --> [Pre-Filter] --> [Multi-Polar Distortion (1-6 stages)]
         --> [Feedback/Delay] --> [Post-Filter]
         --> [Multiband Crossover] --> [Dry/Wet Mix] --> Audio Out

MIDI In --> Pitch tracking for feedback tuning

Modulation (Env, LFO, Curves, Macros) --(drag & drop)--> Any parameter
```

All distortion processing runs through configurable oversampling (off, 2x, 4x, 8x) via `juce::dsp::Oversampling` to prevent aliasing artifacts.

---

## Build from Source

### macOS

#### Prerequisites

```bash
xcode-select --install
brew install cmake ninja
```

Xcode must be installed from the App Store. JUCE 8 is included as a git submodule.

#### Clone and Build

```bash
git clone --recurse-submodules <repo-url>
cd GVNR

cmake -B build -G Ninja -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
cmake --build build
```

On successful build, plugins are automatically copied to:

| Format | Install Location |
|--------|-----------------|
| AU | `~/Library/Audio/Plug-Ins/Components/GVNRs Grunger.component` |
| VST3 | `~/Library/Audio/Plug-Ins/VST3/GVNRs Grunger.vst3` |
| Standalone | `build/TheGVNR_artefacts/Standalone/GVNRs Grunger.app` |

No Apple Developer account is needed for local testing. Plugins load unsigned on your own machine.

#### Launch Standalone

```bash
open "build/TheGVNR_artefacts/Standalone/GVNRs Grunger.app"
```

#### Validate AU

```bash
auval -v aufx Gvn1 Gvnr
```

#### Create Installer (.pkg)

```bash
./Installers/build-installer.sh
```

Produces `Installers/TheGVNR-Installer-v1.0.0.pkg` with choosable components for VST3, AU, and standalone app.

### Windows (via CI)

Windows builds are automated through GitHub Actions. Every push to `main` produces a downloadable installer.

1. Push your code to GitHub
2. Go to the **Actions** tab in your repository
3. Download `GVNRs-Grunger-Windows-Installer` from the latest successful build's artifacts
4. Run `GVNRs-Grunger-Installer-v1.0.0.exe` on the target Windows machine

The installer copies the VST3 to `C:\Program Files\Common Files\VST3\` where FL Studio and other DAWs scan automatically. A standalone `.exe` is optionally installed to `C:\Program Files\GVNR Audio\GVNRs Grunger\`.

You can also trigger a build manually from the Actions tab using **Run workflow**.

---

## Project Structure

```
GVNR/
├── CMakeLists.txt                 # CMake build (C/C++20, JUCE 8)
├── README.md
├── JUCE/                          # JUCE framework (git submodule)
├── Source/
│   ├── PluginProcessor.h/.cpp     # Audio processing entry point
│   ├── PluginEditor.h/.cpp        # GUI entry point
│   ├── DSP/
│   │   ├── DistortionAlgorithms   # 30 distortion transfer functions
│   │   ├── MultiPolarDistortion   # Bi-polar split + blend + staging
│   │   ├── MorphingFilter         # 24 filter types with morphing
│   │   ├── FeedbackDelay          # Pitch-snapped stereo delay
│   │   ├── MultibandCrossover     # 3-band Linkwitz-Riley split/merge
│   │   ├── Oversampler            # Anti-aliasing wrapper (2x/4x/8x)
│   │   └── SignalChain            # Full DSP graph orchestrator
│   ├── Modulation/
│   │   ├── EnvelopeFollower       # Amplitude-tracking modulator
│   │   ├── MorphingLFO            # Multi-shape LFO with morphing
│   │   ├── CurveSequencer         # Drawable bezier modulation
│   │   ├── ModulationMatrix       # Source-to-target routing + drag-drop
│   │   └── MacroControl           # Multi-parameter macro mapping
│   ├── GUI/
│   │   ├── GVNRLookAndFeel        # Dark/light theme system
│   │   ├── MainPanel              # Top-level layout, view + theme toggle, DragAndDropContainer
│   │   ├── PlayView               # Simplified performance view
│   │   ├── AdvancedView           # Full parameter access
│   │   ├── WaveformDisplay        # Real-time oscilloscope
│   │   ├── KnobComponent          # Custom rotary knob + DragAndDropTarget for mod routing
│   │   ├── ModDragSource          # Draggable mod source handles (Env, LFO, Macro)
│   │   ├── CurveEditor            # Interactive curve drawing
│   │   ├── PresetBrowser          # Category/tag/favorites preset browser
│   │   ├── TooltipOverlay         # Contextual help
│   │   ├── DistortionPanel        # Distortion controls
│   │   ├── FilterPanel            # Pre/post filter controls
│   │   ├── FeedbackPanel          # Feedback/delay controls
│   │   ├── MultibandPanel         # Multiband routing controls
│   │   └── ModulationPanel        # Modulation source controls + drag handles
│   ├── Presets/
│   │   ├── PresetManager          # Save/load/delete/randomize/favorites/tags/categories
│   │   └── FactoryPresets         # 50 built-in presets with category metadata
│   └── Utility/
│       ├── Parameters             # All APVTS parameter definitions
│       ├── MIDIProcessor          # MIDI note tracking
│       └── PitchSnap              # Scale + key quantization (10 scales x 12 keys)
├── Resources/
│   └── gvnr_logo.png             # Logo (embedded as binary data, watermark in bottom-left)
├── Installers/
│   └── build-installer.sh        # macOS .pkg builder
└── Tests/
    ├── DSPTests.cpp
    ├── FilterTests.cpp
    └── PresetTests.cpp
```

66 source files (34 headers + 32 implementations).

---

## Technology

| Component | Detail |
|-----------|--------|
| Framework | JUCE 8 |
| Language | C++20 |
| Build system | CMake 3.22+ with Ninja |
| Plugin formats | VST3, AU (aufx), Standalone |
| DSP | Custom algorithms + juce::dsp (Oversampling, StateVariableTPTFilter, LinkwitzRileyFilter, IIR) |
| GUI rendering | juce::LookAndFeel_V4 with custom drawing, dark/light theme switching |
| State management | juce::AudioProcessorValueTreeState (APVTS) |
| Modulation routing | juce::DragAndDropContainer / DragAndDropTarget |
| Preset format | XML-serialized ValueTree (.gvnr files) with sidecar metadata |
| AU validation | Passes `auval` |
| Binary sizes | AU ~4 MB, VST3 ~4 MB, Standalone ~5 MB |

---

## Preset Storage

Factory presets, user presets, and metadata are stored at:

```
~/Library/GVNR Audio/The GVNR/Presets/
```

| File | Purpose |
|------|---------|
| `*.gvnr` | Preset files (XML-serialized APVTS state) |
| `presets-meta.xml` | Category and tag metadata for all presets |
| `favorites.xml` | List of user-favorited preset names |

Factory presets are generated automatically on first launch. Metadata is auto-populated from factory preset categories.

---

## License

All rights reserved. This is proprietary software.
