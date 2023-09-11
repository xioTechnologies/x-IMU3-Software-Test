#pragma once

#include <span>

// TODO: Simplify
class AxisLimits
{
public:
    float min = 0.0f;
    float max = 1.0f;

    AxisLimits() = default;

    AxisLimits(const AxisLimits& other)
    {
        setLimits(other.min, other.max);
    }

    AxisLimits& operator=(const AxisLimits& other)
    {
        setLimits(other.min, other.max);
        return *this;
    }

    AxisLimits(float min_, float max_) : min(min_), max(max_)
    {
    }

    [[nodiscard]] float getMin() const
    {
        return min;
    }

    [[nodiscard]] float getMax() const
    {
        return max;
    }

    [[nodiscard]] float getRange() const
    {
        return max - min;
    }

    void setLimits(float min_, float max_)
    {
        min = min_;
        max = max_;
        ensureValidRange();
    }

    void offset(float value)
    {
        setLimits(min + value, max + value);
    }

    [[nodiscard]] AxisLimits withOffset(float value) const
    {
        AxisLimits newLimits;
        newLimits.setLimits(min + value, max + value);
        return newLimits;
    }

    void shiftToNegativeRange()
    {
        if (min > 0 || max > 0)
        {
            auto range = getRange();
            max = 0.0f;
            min = -range;
        }
    }

private:
    void ensureValidRange()
    {
        // NaN checks
        if (std::isnan(min))
        {
            min = 0.0f; // any legitimate value
        }

        if (std::isnan(max))
        {
            max = 0.0f; // any legitimate value
        }

        // Ensure max/min relationship
        if (min > max)
        {
            std::swap(min, max);
        }

        // Limit maximum range - can handle values of +Inf for max and -Inf for min
        const float maximumRange = std::numeric_limits<float>::max();
        if (getRange() > maximumRange)
        {
            const float halfMax = maximumRange / 2.0f;
            max = halfMax;
            min = -halfMax;
        }

        // Limit maximum range
        constexpr float minRange = 1e-6f; // a micro unit
        static_assert(minRange > std::numeric_limits<float>::min() && minRange / 2.0f > std::numeric_limits<float>::min()); // minRange value is too small to be used for reliable calculations
        if (getRange() < minRange)
        {
            const float centre = (min + max) / 2.0f;
            min = centre - (minRange / 2.0f);
            max = centre + (minRange / 2.0f);
        }

        // Ensure non-zero range - possible for large values with less decimal precision for float calculations
        if (min == max)
        {
            max = std::nextafter(max, std::numeric_limits<float>::max());
        }
    }
};

class AxesLimits
{
public:
    AxisLimits x { -5.64f, 0.0f };
    AxisLimits y { -1.5f, 1.5f };

    [[nodiscard]] const AxisLimits& getXLimits() const
    {
        return x;
    }

    [[nodiscard]] const AxisLimits& getYLimits() const
    {
        return y;
    }

    void setXLimits(const AxisLimits& newXLimits)
    {
        x = newXLimits;
        x.shiftToNegativeRange(); // x-axis can never show positive range because it represents time
    }

    void setXLimits(float min, float max)
    {
        x.setLimits(min, max);
        x.shiftToNegativeRange(); // x-axis can never show positive range because it represents time
    }

    void setYLimits(const AxisLimits& newYLimits)
    {
        y = newYLimits;
    }

    void setYLimits(float min, float max)
    {
        y.setLimits(min, max);
    }

    // TODO: fix jassert (! approximatelyEqual (sourceRangeMax, sourceRangeMin)); in juce::jmap
    void autoscale(const bool horizontal, const bool vertical, const std::vector<std::span<const juce::Point<GLfloat>>>& channelBuffers, const std::vector<bool>& enabledChannels)
    {
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
            // Sizes should always be the same. If we hit this assert, there is a programming mistake somewhere.
            jassertfalse;
            return;
        }

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
            setXLimits(oldestTimestamp, newestTimestamp);
        }

        // Vertical autoscale
        if (vertical && (x.getMax() >= oldestTimestamp) && (x.getMin() <= newestTimestamp))
        {
            float newYMin = std::numeric_limits<float>::max();
            float newYMax = std::numeric_limits<float>::lowest(); // NOT min(), which means minimum positive value

            bool dataFoundInXRange = false;

            for (size_t index = 0; index < channelBuffers.size(); index++)
            {
                if (enabledChannels[index] == false)
                {
                    continue;
                }
                for (const auto& point : channelBuffers[index])
                {
                    if (point.x < x.getMin())
                    {
                        continue;
                    }
                    if (point.x > x.getMax())
                    {
                        break;
                    }
                    newYMin = std::min(newYMin, point.y);
                    newYMax = std::max(newYMax, point.y);
                    dataFoundInXRange = true;
                }
            }

            // Commit new y limits only if valid data was found in the x range.
            // NOTE: When x range is zoomed in so that data points are only outside the range, we leave y limits as they were.
            if (dataFoundInXRange)
            {
                setYLimits(newYMin, newYMax);
            }
        }
    }

private:

};
