#include "../JuceLibraryCode/JuceHeader.h"
#include <Python.h>

class MyPluginAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    MyPluginAudioProcessor()
    {
        // Initialise Python environment
        Py_Initialize();

        // Add the path to the Python script to the Python path
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('/path/to/scripts')");

        // Import the Python modules
        pModuleInfo = PyImport_ImportModule("midi_info");
        pModuleTransfer = PyImport_ImportModule("midi_transfer");

        // Set the Python functions
        pFuncInfo = PyObject_GetAttrString(pModuleInfo, "get_info");
        pFuncTransfer = PyObject_GetAttrString(pModuleTransfer, "transfer_style");

        // Initialise the Python arguments and results
        pArgsInfo = PyTuple_New(1);
        pArgsTransfer = PyTuple_New(4);
        pResult = nullptr;
    }

    ~MyPluginAudioProcessor() override
    {
        // Cleanup Python objects
        Py_XDECREF(pArgsInfo);
        Py_XDECREF(pArgsTransfer);
        Py_XDECREF(pFuncInfo);
        Py_XDECREF(pFuncTransfer);
        Py_XDECREF(pModuleInfo);
        Py_XDECREF(pModuleTransfer);

        // Finalise Python environment
        Py_Finalize();
    }

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override
    {
        // TODO: Add prepareToPlay logic
    }

    void releaseResources() override
    {
        // TODO: Add releaseResources logic
    }

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override
    {
        // TODO: Add isBusesLayoutSupported logic
        return true;
    }
#endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override
    {
        // TODO: Add processBlock logic
    }

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override
    {
        // TODO: Add createEditor logic
        return nullptr;
    }

    bool hasEditor() const override
    {
        // TODO: Add hasEditor logic
        return false;
    }

    //==============================================================================
    const juce::String getName() const override
    {
        return JucePlugin_Name;
    }

   
// In PluginEditor.h

class MidiGeneratorPluginAudioProcessorEditor : public AudioProcessorEditor
{
public:
    MidiGeneratorPluginAudioProcessorEditor (MidiGeneratorPluginAudioProcessor&);
    ~MidiGeneratorPluginAudioProcessorEditor() override;

    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MidiGeneratorPluginAudioProcessor& audioProcessor;

    // Add UI components here
    TextButton generateButton;
    ComboBox keyComboBox;
    ComboBox typeComboBox;
    Label keyLabel;
    Label typeLabel;

    void generateMidi();
    void generateButtonClicked();
    void updateLabels();
    void updateComboBoxes();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiGeneratorPluginAudioProcessorEditor)
};


// In PluginEditor.cpp

MidiGeneratorPluginAudioProcessorEditor::MidiGeneratorPluginAudioProcessorEditor (MidiGeneratorPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Set up UI components
    setSize (400, 300);

    addAndMakeVisible(generateButton);
    generateButton.setButtonText("Generate");
    generateButton.onClick = [this] { generateButtonClicked(); };

    addAndMakeVisible(keyComboBox);
    keyComboBox.addItem("C", 1);
    keyComboBox.addItem("C#", 2);
    keyComboBox.addItem("D", 3);
    keyComboBox.addItem("D#", 4);
    keyComboBox.addItem("E", 5);
    keyComboBox.addItem("F", 6);
    keyComboBox.addItem("F#", 7);
    keyComboBox.addItem("G", 8);
    keyComboBox.addItem("G#", 9);
    keyComboBox.addItem("A", 10);
    keyComboBox.addItem("A#", 11);
    keyComboBox.addItem("B", 12);
    keyComboBox.onChange = [this] { updateLabels(); };

    addAndMakeVisible(typeComboBox);
    typeComboBox.addItem("Melody", 1);
    typeComboBox.addItem("Chords", 2);
    typeComboBox.addItem("Bassline", 3);
    typeComboBox.onChange = [this] { updateLabels(); };

    addAndMakeVisible(keyLabel);
    keyLabel.setText("Key:", dontSendNotification);

    addAndMakeVisible(typeLabel);
    typeLabel.setText("Type:", dontSendNotification);

    updateLabels();
}

MidiGeneratorPluginAudioProcessorEditor::~MidiGeneratorPluginAudioProcessorEditor()
{
}

void MidiGeneratorPluginAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    // Add any painting code here
}

void MidiGeneratorPluginAudioProcessorEditor::resized()
{
    // Set the bounds of UI components here
    auto area = getLocalBounds();

    auto keyRow = area.removeFromTop(30);
    keyLabel.setBounds(keyRow.removeFromLeft(50));
    keyComboBox.setBounds(keyRow.removeFromLeft(100));

    auto typeRow = area.removeFromTop(30);
    typeLabel.setBounds(typeRow.removeFromLeft(50));
    typeComboBox.setBounds(typeRow.removeFromLeft(100));

    generateButton.setBounds(area.reduced(10));
}

void MidiGeneratorPluginAudioProcessorEditor::generateButtonClicked()
{
    generateMidi();
}

void MidiGeneratorPluginAudioProcessorEditor::updateLabels()
{
    String key = keyComboBox.getText();
    String type = typeComboBox.getText();

    if (type == "Melody")
    {
        keyLabel.setText("Melody key:", dontSendNotification);
    }
