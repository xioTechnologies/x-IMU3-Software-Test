#include "OpenGLComponent.h"

OpenGLComponent::OpenGLComponent(juce::OpenGLContext& context_) : context(context_)
{
    // Only visualisation, no mouse interaction
    setInterceptsMouseClicks(false, false);
}

OpenGLComponent::~OpenGLComponent()
{
    unregisterParents();
}

void OpenGLComponent::parentHierarchyChanged()
{
    unregisterParents();

    for (auto* parent = getParentComponent(); parent != nullptr; parent = parent->getParentComponent())
    {
        registeredParents.push_back(parent);
        parent->addComponentListener(this);
    }

    updateBounds();
}

juce::Rectangle<int> OpenGLComponent::getBoundsInMainWindow() const
{
    std::lock_guard _(boundsInMainWindowLock);
    return boundsInMainWindow;
}

juce::Rectangle<int> OpenGLComponent::toOpenGLBounds(const juce::Rectangle<int>& bounds) const
{
    auto glBounds = bounds.withY(topLevelHeight - bounds.getBottom()).toDouble() * context.getRenderingScale();
    return glBounds.getSmallestIntegerContainer();
}

void OpenGLComponent::unregisterParents()
{
    for (auto* const parent : registeredParents)
    {
        parent->removeComponentListener(this);
    }

    registeredParents.clear();
}

void OpenGLComponent::updateBounds()
{
    topLevelHeight = getTopLevelComponent()->getHeight();

    std::lock_guard _(boundsInMainWindowLock);
    boundsInMainWindow = getTopLevelComponent()->getLocalArea(this, getLocalBounds());
}

void OpenGLComponent::componentMovedOrResized(juce::Component&, bool, bool)
{
    updateBounds();
}
