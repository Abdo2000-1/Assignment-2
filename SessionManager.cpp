#include "SessionManager.h"

SessionManager::SessionManager(const juce::File& jsonFile)
    : file(jsonFile)
{
    load();
}

void SessionManager::load()
{
    if (file.existsAsFile())
    {
        juce::FileInputStream stream(file);
        if (stream.openedOk())
            data = juce::JSON::parse(stream.readEntireStreamAsString());
    }

    if (!data.isObject())
    {
        juce::DynamicObject::Ptr root = new juce::DynamicObject();
        root->setProperty("files", juce::Array<juce::var>());
        data = root.get();
    }
}

void SessionManager::save()
{
    file.replaceWithText(juce::JSON::toString(data, true));
}

void SessionManager::saveSession(const juce::String& fileId,
    double positionSeconds)
{
    auto* filesArray = data["files"].getArray();
    if (filesArray == nullptr)
        return;

    // Try to find existing fileId
    for (auto& entry : *filesArray)
    {
        auto* obj = entry.getDynamicObject();
        if (obj && obj->getProperty("fileId") == fileId)
        {
            obj->setProperty("position", positionSeconds);
            save();
            return;
        }
    }

    // Not found — create new session object
    juce::DynamicObject::Ptr newObj = new juce::DynamicObject();
    newObj->setProperty("fileId", fileId);
    newObj->setProperty("position", positionSeconds);

    filesArray->add(newObj.get());
    save();
}

juce::var SessionManager::getSession(const juce::String& fileId)
{
    auto* filesArray = data["files"].getArray();
    if (filesArray == nullptr)
        return juce::var();

    for (auto& entry : *filesArray)
    {
        auto* obj = entry.getDynamicObject();
        if (obj && obj->getProperty("fileId") == fileId)
            return entry;
    }

    return juce::var(); // not found
}

