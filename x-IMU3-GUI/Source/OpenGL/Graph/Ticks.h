#pragma once

struct Ticks
{
    struct Label
    {
        juce::String text;
        float value;
    };

    float major;
    unsigned int minorPerMajor;
    std::vector<Label> labels;

    static Ticks getDefault() // TODO: Remove getDefault
    {
        Ticks ticks;
        ticks.major = 1.0f;
        ticks.minorPerMajor = 5;
        return ticks;
    }

    static Ticks createTicks(const int lengthPixels, const AxisLimits& limits)
    {
        const float range = limits.getRange();

        // Prevent undefined log calculation or division by 0
        if (range <= 0.0 || lengthPixels <= 0) // TODO: Remove redundant range <= 0.0
        {
            jassertfalse;
            return Ticks::getDefault();
        }

        // Determine order of magnitude power ( 10^-1 [0.1s], 10^0 [1s], 10^1 [10s], 10^2 [100s], etc) of the range.
        const int oomPowerOfRange = (int) std::floor(std::log10(range));

        // Calculate possible divisions of the order of magnitude to display as major grid divisions.
        float previousOOMDouble = std::pow(10.0f, (float) (oomPowerOfRange - 1)) * 2.0f;
        float ooMFull = std::pow(10.0f, (float) oomPowerOfRange);
        float ooMHalf = ooMFull / 2.0f;
        float ooMDouble = ooMFull * 2.0f;

        // For the major tick length, choose the smallest division which is at least a minimum pixel length.
        constexpr float minimumMajorTickLengthPixels = 25.0f;
        float majorTickLengthDataUnits = previousOOMDouble;
        bool minorDivisionsUsesFourths = false;

        auto toPixels = [=](float dataUnits)
        {
            return dataUnits / range * (float) lengthPixels; // TODO: Add paranthesis
        };

        if (toPixels(previousOOMDouble) >= minimumMajorTickLengthPixels)
        {
            majorTickLengthDataUnits = previousOOMDouble;
            minorDivisionsUsesFourths = true;
        }
        else if (toPixels(ooMHalf) >= minimumMajorTickLengthPixels)
        {
            majorTickLengthDataUnits = ooMHalf;
        }
        else if (toPixels(ooMFull) >= minimumMajorTickLengthPixels)
        {
            majorTickLengthDataUnits = ooMFull;
        }
        else if (toPixels(ooMDouble) >= minimumMajorTickLengthPixels)
        {
            majorTickLengthDataUnits = ooMDouble;
            minorDivisionsUsesFourths = true;
        }
        else
        {
            // If we hit this jassert, we have chosen a minimumMajorTickLengthPixels which is too large and cannot contain
            // any of the possible divisions. Consider changing the value of minimumMajorTickLengthPixels to be smaller.
            // It is also possible we hit this assert due to our oom values reaching +Inf or our range being NaN
            jassertfalse;
        }

        // Setup Tick data
        Ticks ticks;
        ticks.major = majorTickLengthDataUnits;
        ticks.minorPerMajor = minorDivisionsUsesFourths ? 4 : 5;

        // Prevent divide by 0 when adding text labels
        if (ticks.major <= 0.0 || ticks.minorPerMajor <= 0)
        {
            jassertfalse;
            return ticks;
        }

        // Fill in text labels for major ticks
        const float firstMajorPosition = GLUtil::roundUpToNearestMultiple(limits.getMin(), ticks.major); // TODO: Avoid OpenGL knowledge
        const auto maxPossibleMajorTickCount = static_cast<unsigned int> (std::floor(limits.getRange() / ticks.major)) + 1;
        for (unsigned int majorTickIndex = 0; majorTickIndex < maxPossibleMajorTickCount; majorTickIndex++)
        {
            const float majorPosition = firstMajorPosition + (float) majorTickIndex * ticks.major;

            if (majorPosition > limits.getMax())
            {
                break;
            }

            // Ensure 0 is written properly with no rounding error
            if (GLUtil::approximatelyEqual(majorPosition, 0.0f, ticks.major / (float) ticks.minorPerMajor)) // TODO: Avoid OpenGL knowledge
            {
                ticks.labels.push_back({ "0", 0.0f });
            }
            else
            {
                ticks.labels.push_back({ juce::String(majorPosition), majorPosition });
            }
        }

        return ticks;
    }

    static Ticks createXTicks(const int widthPixels, const AxisLimits& xLimits)
    {
        return createTicks(widthPixels, xLimits);
    }

    static Ticks createYTicks(const int heightPixels, const AxisLimits& yLimits)
    {
        return createTicks(heightPixels, yLimits);
    }
};
