/*
  ==============================================================================

    FlushStrip.h
    Created: 29 Nov 2022 1:10:43pm
    Author:  Mouldy Soul

  ==============================================================================
*/

#pragma once
class FlushStrip
{
    public :
    
    FlushStrip() {}
    
    void prepare()
    {
        notes.clear();
    }
    
    void process(juce::AudioBuffer<float>& buffer, juce::AudioPlayHead* playhead, juce::MidiBuffer& midiMessages)
    {
        midiBuff.clear();
        for (auto message: midiMessages)
        {
            auto currentMessage = message.getMessage();
            auto sample = message.samplePosition;
            if (currentMessage.isNoteOn())
            {
                auto nn  = currentMessage.getNoteNumber();
                
                for (auto note : notes)
                {
                    if (notes.contains(nn))
                    {
                        DBG("note already here");
                        clearSingle(nn);
                        DBG(nn);
                    }
                    
                    if (!notes.contains(nn))
                    {
                        DBG("not in set");
                        DBG(nn);
                    }
                    
                }
                notes.add(nn);
                auto message = juce::MidiMessage::noteOn(1, nn, (juce::uint8)100);
                midiBuff.addEvent(message, sample);
            }
            
            if (currentMessage.isNoteOff())
            {
                auto nn  = currentMessage.getNoteNumber();
                needsEmptying = true;
                clearSingle(nn);
            }
        }
        midiMessages.swapWith(midiBuff);
    }
    
    void clearSingle(int note)
    {
        auto message = juce::MidiMessage::noteOff(1, note);
        midiBuff.addEvent(message, 0);
        notes.removeFirstMatchingValue(note);
        if (notes.isEmpty())
        {
            if (needsEmptying)
            {
                notes.clear();
                needsEmptying = false;
                DBG("notes are empty");
            }
        }
    }
    
    void flush(bool shouldFlush, juce::MidiBuffer& midiMessages)
    {
        if( shouldFlush == true )
        {
            midiBuff.clear();
            for (auto note : notes)
            {
                auto message = juce::MidiMessage::noteOff(1, note);
                midiBuff.addEvent(message, 0);
                notes.removeFirstMatchingValue(note);
                needsEmptying = true;
            }
            
            if (notes.isEmpty())
            {
                if (needsEmptying)
                {
                    notes.clear();
                    needsEmptying = false;
                    DBG("notes are empty");
                }
            }
            midiMessages.swapWith(midiBuff);
        }
    }
    
    bool lastFlush = false;
    juce::Array<int>notes;
    juce::MidiBuffer midiBuff;
    bool needsEmptying = false;
};
