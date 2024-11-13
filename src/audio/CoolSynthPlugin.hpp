#pragma once

#include "gui/MainWindow.hpp"

#include <JuceHeader.h>



class CoolSynthPlugin : public juce::AudioProcessor
{
public:
    CoolSynthPlugin()
        : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true))
    {
        
    }

    void getStateInformation (juce::MemoryBlock& dest_data) override
    {
        juce::MemoryOutputStream(dest_data, true);
        // stream.writeFloat(); // write params to this memory block so the config is stored between sessions.
    }

    void setStateInformation(const void* data, int size) override
    {
        juce::MemoryInputStream stream(data, static_cast<size_t>(size), false);
        
        // damping->setValueNotifyingHost(stream.readFloat());
    }

    void prepareToPlay(double sample_rate, int samples_per_block_expected) override
    {
        m_synth.setCurrentPlaybackSampleRate(sample_rate);
        static_cast<void>(samples_per_block_expected);
        
    }

    void processBlock(juce::AudioBuffer<double>& audio_buffer, juce::MidiBuffer& midi_buffer) override
    {
        juce::AudioBuffer<float> audio_buffer_float(audio_buffer.getNumChannels(), audio_buffer.getNumSamples());
        
        processBlock(audio_buffer_float, midi_buffer);

        for(int channel_idx = 0; channel_idx < audio_buffer.getNumChannels(); channel_idx++)
        {
            const float *read_ptr = audio_buffer_float.getReadPointer(channel_idx);
            double* write_ptr = audio_buffer.getWritePointer(channel_idx);
            
            for(int sample_idx = 0; sample_idx < audio_buffer.getNumSamples(); sample_idx++)
            {
                write_ptr[sample_idx] = read_ptr[sample_idx];
            }
        }
    }
    
    void processBlock(juce::AudioBuffer<float>& audio_buffer, juce::MidiBuffer& midi_buffer) override
    {
        juce::AudioBuffer<float> stereo_buffer = getBusBuffer(audio_buffer, false, 0);
        m_synth.renderNextBlock(stereo_buffer, midi_buffer, 0, stereo_buffer.getNumSamples());
        stereo_buffer.applyGain(1.0f);
        
    }

    juce::AudioProcessorEditor* createEditor() override
    {
        return new MainComponent(*this); //TODO: idk what this will do
    }

    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return "Cool Synth"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    const juce::String getProgramName(int program) override { return "Scanner"; }
    
    void setCurrentProgram(int program) {}
    void changeProgramName(int program, const juce::String& new_name) override {}
    void releaseResources() override {}
    void reset() override {}

private:
    juce::Synthesiser m_synth;
    
};
