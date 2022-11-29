/*
  ==============================================================================

    TimingTest.h
    Created: 29 Nov 2022 3:13:22am
    Author:  Mouldy Soul

  ==============================================================================
*/

#pragma once
#include "Transport.h"

class TimingTest
{
    
    
    public :
    
    TimingTest() {}
    
    void prepare(double sampleRate, int blockSize)
    {
        transport.prepare(sampleRate, blockSize);
    }

    void process(juce::AudioBuffer<float>& buffer, juce::AudioPlayHead* playhead, juce::MidiBuffer& midiMessages)
    {
        auto numSamples = buffer.getNumSamples();
        transport.process(playhead, numSamples);
        midiBuff.clear();
        if (playhead != nullptr)
        {
            playhead->getCurrentPosition(info);
        }
        
        if (!info.isPlaying)
        {
            notes.clear();
        }
        for (auto message: midiMessages)
        {
            auto currentMessage = message.getMessage();
            
            if (currentMessage.isNoteOn())
            {
                notes.add(currentMessage.getNoteNumber());
                notes.add(currentMessage.getNoteNumber()+7);
                notes.add(currentMessage.getNoteNumber()+12);
                notes.add(currentMessage.getNoteNumber()+19);
            }
            
            if (currentMessage.isNoteOff())
            {
                notes.removeValue(currentMessage.getNoteNumber());
                notes.removeValue(currentMessage.getNoteNumber()+7);
                notes.removeValue(currentMessage.getNoteNumber()+12);
                notes.removeValue(currentMessage.getNoteNumber()+19);
                noteOffs.add(currentMessage.getNoteNumber());
                noteOffs.add(currentMessage.getNoteNumber()+7);
                noteOffs.add(currentMessage.getNoteNumber()+12);
                noteOffs.add(currentMessage.getNoteNumber()+19);
            }
        }
        
        for (int sample = 0; sample < numSamples; ++sample)
        {
            double relppqVal = fmod(transport.ppqPositions[sample],0.5f);
            
            if (relppqVal < 0.25 && toggle)
            {
                DBG("timeHit");
                toggle = false;
                
                if (!notes.isEmpty())
                {
                    auto message = juce::MidiMessage::noteOn(1, notes[n], (juce::uint8)100);
                    midiBuff.addEvent(message, sample);
                    
                }
            }
            if (relppqVal > 0.25 && !toggle)
            {
                toggle = true;
                for (auto note : noteOffs)
                {
                auto message = juce::MidiMessage::noteOff(1, note);
                    midiBuff.addEvent(message, sample);
                    noteOffs.removeValue(note);
                }
                
                if (noteOffs.isEmpty())
                {
                    noteOffs.clear();
                }
                ++n;
                if (notes.size() > 0)
                {
                    n %= notes.size();
                }
//                if (notes.size() == 0)
//                {
//                    for (int i = 0; i < 127; i++)
//                    {
//                        auto message = juce::MidiMessage::noteOff(1, i);
//                        midiBuff.addEvent(message, sample);
//                    }
//                }
            }
        }
        midiMessages.swapWith(midiBuff);
    }
    
    Transport transport;
    juce::AudioPlayHead::CurrentPositionInfo info;
    juce::SortedSet<int> notes;
    juce::SortedSet<int> noteOffs;
    juce::MidiBuffer midiBuff;
    bool toggle;
    int n = 0;
};
