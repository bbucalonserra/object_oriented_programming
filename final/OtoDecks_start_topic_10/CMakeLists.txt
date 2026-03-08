cmake_minimum_required(VERSION 3.22)

project(OTODECKS VERSION 0.0.1)

# Se você estiver usando o Projucer para gerar o projeto, o CMake é um extra, 
# mas mantê-lo atualizado é fundamental para quem usa CLion ou VS Code.
add_subdirectory(../JUCE JUCE)

juce_add_gui_app(OtoDecks
    PRODUCT_NAME "OtoDecks")

juce_generate_juce_header(OtoDecks)

target_sources(OtoDecks
    PRIVATE
        Source/Main.cpp
        Source/MainComponent.h
        Source/MainComponent.cpp
        Source/DeckGUI.h
        Source/DeckGUI.cpp
        Source/DJAudioPlayer.h
        Source/DJAudioPlayer.cpp
        Source/WaveformDisplay.h
        Source/WaveformDisplay.cpp
        Source/PlaylistComponent.h
        Source/PlaylistComponent.cpp
)

target_compile_definitions(OtoDecks
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:OtoDecks,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:OtoDecks,JUCE_VERSION>")

juce_add_binary_data(GuiAppData 
    SOURCES 
        Source/icon_large.png
        Source/icon_small.png)

target_link_libraries(OtoDecks
    PRIVATE
        GuiAppData
        juce::juce_gui_extra
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)