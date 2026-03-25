cmake_minimum_required(VERSION 3.22)

project(TheGVNR VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(JUCE)

if(APPLE)
    set(GVNR_FORMATS AU VST3 Standalone)
    set(GVNR_VST3_COPY_DIR "$ENV{HOME}/Library/Audio/Plug-Ins/VST3")
    set(GVNR_AU_COPY_DIR "$ENV{HOME}/Library/Audio/Plug-Ins/Components")
elseif(WIN32)
    set(GVNR_FORMATS VST3 Standalone)
    set(GVNR_VST3_COPY_DIR "$ENV{COMMONPROGRAMFILES}/VST3")
    set(GVNR_AU_COPY_DIR "")
else()
    set(GVNR_FORMATS VST3 Standalone)
    set(GVNR_VST3_COPY_DIR "")
    set(GVNR_AU_COPY_DIR "")
endif()

juce_add_plugin(TheGVNR
    COMPANY_NAME "GVNR Audio"
    PLUGIN_MANUFACTURER_CODE Gvnr
    PLUGIN_CODE Gvng
    FORMATS ${GVNR_FORMATS}
    PRODUCT_NAME "GVNRs Grunger"
    BUNDLE_ID "com.gvnraudio.gvnrsgrunger"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST3_COPY_DIR "${GVNR_VST3_COPY_DIR}"
    AU_COPY_DIR "${GVNR_AU_COPY_DIR}"
)

target_sources(TheGVNR
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp

        Source/DSP/DistortionAlgorithms.cpp
        Source/DSP/MultiPolarDistortion.cpp
        Source/DSP/MorphingFilter.cpp
        Source/DSP/FeedbackDelay.cpp
        Source/DSP/MultibandCrossover.cpp
        Source/DSP/Oversampler.cpp
        Source/DSP/SignalChain.cpp

        Source/Modulation/EnvelopeFollower.cpp
        Source/Modulation/MorphingLFO.cpp
        Source/Modulation/CurveSequencer.cpp
        Source/Modulation/ModulationMatrix.cpp
        Source/Modulation/MacroControl.cpp

        Source/GUI/GVNRLookAndFeel.cpp
        Source/GUI/MainPanel.cpp
        Source/GUI/PlayView.cpp
        Source/GUI/AdvancedView.cpp
        Source/GUI/WaveformDisplay.cpp
        Source/GUI/DistortionPanel.cpp
        Source/GUI/FilterPanel.cpp
        Source/GUI/FeedbackPanel.cpp
        Source/GUI/MultibandPanel.cpp
        Source/GUI/ModulationPanel.cpp
        Source/GUI/CurveEditor.cpp
        Source/GUI/PresetBrowser.cpp
        Source/GUI/KnobComponent.cpp
        Source/GUI/TooltipOverlay.cpp
        Source/GUI/ModDragSource.cpp

        Source/Presets/PresetManager.cpp

        Source/Utility/MIDIProcessor.cpp
        Source/Utility/PitchSnap.cpp
)

juce_add_binary_data(GVNRBinaryData
    HEADER_NAME "BinaryData.h"
    NAMESPACE BinaryData
    SOURCES
        Resources/gvnr_logo.png
        Resources/gvnr_background.png
)

target_compile_definitions(TheGVNR
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
)

target_link_libraries(TheGVNR
    PRIVATE
        GVNRBinaryData
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_opengl
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

target_include_directories(TheGVNR
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

if(MSVC)
    target_compile_options(TheGVNR PRIVATE /utf-8)
    target_compile_definitions(TheGVNR PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
