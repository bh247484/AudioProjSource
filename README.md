## Source code for simple audio plugin written in the [JUCE framework](https://juce.com/).

There's a criminal amount of boilerplate but as a general guide `Editor`s handle GUI and `Processor`s handle audio processing.

`./Source/myModules` contains code for an envelope generator (affects sounds amplitude over time), noise/wavetable oscillators (generates synthesized sounds), and a panning knob (moves sounds in the stereo field).  Check out what's happening in each modules' `<module-name>Processor::processBlock` method to see what's actually being done to the audio buffers.

Audio buffers are simply arrays of amplitude intensity values (values that tell your speakers how to move and push the air around you).  Looping through audio buffers and algorithmically manipulating each successive value changes the way digital audio sounds.
