#pragma once

struct Tick
{
    bool isMajor;
    float value;
    juce::String label;
};

typedef std::vector<Tick> Ticks;

static inline Ticks createTicks(const int lengthPixels, const AxisLimits& limits)
{
    const float range = limits.getRange();

    // Determine order of magnitude's exponent ( 10^-1 [0.1s], 10^0 [1s], 10^1 [10s], 10^2 [100s], etc) of the range.
    const int oomExponent = (int) std::floor(std::log10(range));

    // Calculate possible divisions of the order of magnitude to display as major grid divisions.
    float previousOomDouble = std::pow(10.0f, (float) (oomExponent - 1)) * 2.0f;
    float oomFull = std::pow(10.0f, (float) oomExponent);
    float oomHalf = oomFull / 2.0f;
    float oomDouble = oomFull * 2.0f;

    // For the major tick length, choose the smallest division which is at least a minimum pixel length.
    constexpr float minimumMajorDistancePixels = 25.0f;
    float majorDistance = previousOomDouble;
    bool minorDivisionsUsesFourths = false;

    auto toPixels = [=](float dataUnits)
    {
        return (dataUnits / range) * (float) lengthPixels;
    };

    if (toPixels(previousOomDouble) >= minimumMajorDistancePixels)
    {
        majorDistance = previousOomDouble;
        minorDivisionsUsesFourths = true;
    }
    else if (toPixels(oomHalf) >= minimumMajorDistancePixels)
    {
        majorDistance = oomHalf;
    }
    else if (toPixels(oomFull) >= minimumMajorDistancePixels)
    {
        majorDistance = oomFull;
    }
    else if (toPixels(oomDouble) >= minimumMajorDistancePixels)
    {
        majorDistance = oomDouble;
        minorDivisionsUsesFourths = true;
    }
    else
    {
        // If we hit this jassert, we have chosen a minimumMajorTickLengthPixels which is too large and cannot contain
        // any of the possible divisions. Consider changing the value of minimumMajorTickLengthPixels to be smaller.
        // It is also possible we hit this assert due to our oom values reaching +Inf or our range being NaN
        jassertfalse;
    }

    const unsigned int minorPerMajorDivisions = minorDivisionsUsesFourths ? 4 : 5;
    const float minorDistance = majorDistance / (float) minorPerMajorDivisions;

    // Prevent divide by 0 when adding text labels
    if (majorDistance <= 0.0 || minorPerMajorDivisions <= 0)
    {
        jassertfalse;
        return {};
    }

    // Fill in tick data
    Ticks ticks;

    auto roundUpToNearestMultiple = [](float valueToRound, float multiple)
    {
        float remainderToClosestMultiple = std::fmod(std::abs(valueToRound), multiple);
        bool nearestMultipleIsLessThanValue = valueToRound > 0.0f;
        return valueToRound + ((nearestMultipleIsLessThanValue) ? (multiple - remainderToClosestMultiple) : remainderToClosestMultiple);
    };

    const float firstMajorPosition = roundUpToNearestMultiple(limits.min, majorDistance);
    const auto maxPossibleMajorTickCount = static_cast<unsigned int> (std::floor(limits.getRange() / majorDistance)) + 1;
    for (unsigned int majorTickIndex = 0; majorTickIndex < maxPossibleMajorTickCount; majorTickIndex++)
    {
        const float majorPosition = firstMajorPosition + (float) majorTickIndex * majorDistance;

        if (majorPosition > limits.max)
        {
            break;
        }

        auto approximatelyEqual = [](float a, float b, float epsilon)
        {
            return std::fabs(a - b) <= epsilon;
        };

        // Major ticks
        if (approximatelyEqual(majorPosition, 0.0f, majorDistance / (float) minorPerMajorDivisions))
        {
            ticks.push_back({ true, 0.0f, "0" }); // ensure 0 is written properly with no rounding error
        }
        else
        {
            ticks.push_back({ true, majorPosition, juce::String(majorPosition) });
        }

        // Minor ticks
        for (unsigned int minorTickIndex = 1; minorTickIndex < minorPerMajorDivisions; minorTickIndex++)
        {
            const float minorPosition = majorPosition + (float) minorTickIndex * minorDistance;
            if (minorPosition > limits.max)
            {
                break;
            }
            ticks.push_back({ false, minorPosition, {}});
        }
    }

    // Minor ticks prior to first major position
    for (unsigned int minorTickIndex = 1; minorTickIndex < minorPerMajorDivisions; minorTickIndex++)
    {
        const float minorPosition = firstMajorPosition - (float) minorTickIndex * minorDistance;
        if (minorPosition < limits.min)
        {
            break;
        }
        ticks.insert(ticks.begin(), { false, minorPosition, {}});
    }

    return ticks;
}

static inline Ticks createXTicks(const int widthPixels, const AxisLimits& xLimits)
{
    return createTicks(widthPixels, xLimits);
}

static inline Ticks createYTicks(const int heightPixels, const AxisLimits& yLimits)
{
    return createTicks(heightPixels, yLimits);
}