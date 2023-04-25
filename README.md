# pyjuce
generic JUCE VST wrapper for calling python code


Creates a simple VST plugin called MyPlugin that generates MIDI data by calling Python code when the shouldGenerateMidi method returns true. The Python code is assumed to be located in a directory called /path/to/python/scripts and is expected to define a function called generate_midi that generates the MIDI data.

The generateMidi method calls the Python interpreter and executes a Python script that imports the midi_generator module and calls the generate_midi function. 
