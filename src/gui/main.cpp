/*
  ==============================================================================

    This file was auto-generated and contains the startup code for a PIP.

  ==============================================================================
*/

#include "audio/CoolSynthPlugin.hpp"

#include <JuceHeader.h>

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CoolSynthPlugin();
}
