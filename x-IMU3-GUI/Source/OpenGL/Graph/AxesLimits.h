#pragma once

#include <span>

class AxisLimits
{
public:
    static constexpr auto maximumValue = 1e15f;

    float min = 0.0f;
    float max = 1.0f;

    float getRange() const
    {
        return max - min;
    }

    void limitRange()
    {
        min = juce::jlimit(-maximumValue, maximumValue, min);
        max = juce::jlimit(-maximumValue, maximumValue, max);

        if (juce::exactlyEqual(min, max)) // TODO: Fix jassert (! approximatelyEqual (sourceRangeMax, sourceRangeMin)); in juce::jmap
        {
            max = std::nextafter(max, std::numeric_limits<float>::max());
        }
    }
};

class AxesLimits
{
public:
    AxisLimits x { -5.0f, 0.0f };
    AxisLimits y { -1.0f, 1.0f };

    void autoscale(const bool horizontal, const bool vertical, const std::vector<std::span<const juce::Point<GLfloat>>>& channelBuffers, const std::vector<bool>& enabledChannels)
    {
        x.limitRange();
        y.limitRange();

        if (channelBuffers.empty())
        {
            return;
        }

        if (channelBuffers[0].empty())
        {
            return;
        }

        if (channelBuffers.size() != enabledChannels.size())
        {
            jassertfalse;
            return;
        }

        // Return if no channels enabled
        bool anyChannelEnabled = false;
        for (bool enabled : enabledChannels)
        {
            anyChannelEnabled |= enabled;
        }
        if (anyChannelEnabled == false)
        {
            return;
        }

        // Horizontal autoscale
        const float oldestTimestamp = channelBuffers[0].front().x;
        const float newestTimestamp = channelBuffers[0].back().x;
        if (horizontal)
        {
            x.min = oldestTimestamp;
            x.max = newestTimestamp;
        }

        // Vertical autoscale
        if (vertical)
        {
            AxisLimits newY { std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest() };

            for (size_t index = 0; index < channelBuffers.size(); index++)
            {
                if (enabledChannels[index] == false)
                {
                    continue;
                }
                for (const auto& point : channelBuffers[index])
                {
                    if (point.x < x.min)
                    {
                        continue;
                    }
                    if (point.x > x.max)
                    {
                        break;
                    }
                    newY.min = std::min(newY.min, point.y);
                    newY.max = std::max(newY.max, point.y);
                }
            }

            if (newY.min < newY.max)
            {
                y.min = newY.min;
                y.max = newY.max;
            }
        }

        x.limitRange();
        y.limitRange();
    }
};
