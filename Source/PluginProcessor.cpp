#include "PluginProcessor.h"
#include "PluginEditor.h"

TheGVNRProcessor::TheGVNRProcessor()
    : AudioProcessor(BusesProperties()
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", gvnr::createParameterLayout()),
      presetManager(apvts)
{
}

TheGVNRProcessor::~TheGVNRProcessor() = default;

const juce::String TheGVNRProcessor::getName() const { return JucePlugin_Name; }
bool TheGVNRProcessor::acceptsMidi() const { return true; }
bool TheGVNRProcessor::producesMidi() const { return false; }
bool TheGVNRProcessor::isMidiEffect() const { return false; }
double TheGVNRProcessor::getTailLengthSeconds() const { return 2.0; }
int TheGVNRProcessor::getNumPrograms() { return 1; }
int TheGVNRProcessor::getCurrentProgram() { return 0; }
void TheGVNRProcessor::setCurrentProgram(int) {}
const juce::String TheGVNRProcessor::getProgramName(int) { return {}; }
void TheGVNRProcessor::changeProgramName(int, const juce::String&) {}

bool TheGVNRProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void TheGVNRProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    signalChain.prepare(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    modMatrix.prepare(sampleRate, samplesPerBlock);
    midiProcessor.prepare(sampleRate);
}

void TheGVNRProcessor::releaseResources()
{
    signalChain.reset();
    modMatrix.reset();
}

void TheGVNRProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    auto bypassParam = apvts.getRawParameterValue(gvnr::ParamIDs::BYPASS);
    if (bypassParam != nullptr && bypassParam->load() > 0.5f)
        return;

    midiProcessor.process(midiMessages);
    modMatrix.process(buffer.getNumSamples());
    signalChain.process(buffer, apvts, modMatrix, midiProcessor);

    const float* output = buffer.getReadPointer(0);
    int numSamples = buffer.getNumSamples();
    int pos = waveformWritePos.load(std::memory_order_relaxed);
    for (int i = 0; i < numSamples; ++i) {
        waveformRing[static_cast<size_t>(pos)] = output[i];
        pos = (pos + 1) % kWaveformBufferSize;
    }
    waveformWritePos.store(pos, std::memory_order_relaxed);
}

juce::AudioProcessorEditor* TheGVNRProcessor::createEditor()
{
    return new TheGVNREditor(*this);
}

bool TheGVNRProcessor::hasEditor() const { return true; }

void TheGVNRProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void TheGVNRProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr && xmlState->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TheGVNRProcessor();
}
