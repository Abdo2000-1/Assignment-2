#include "MarkManager.h"

MarkManager::MarkManager(const juce::File& jsonFile)
    : file(jsonFile)
{
    load();
}

void MarkManager::load()
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

void MarkManager::save()
{
    file.replaceWithText(juce::JSON::toString(data, true));
}

void MarkManager::addMark(const juce::String& fileId, double seconds)
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
            auto* marks = obj->getProperty("marks").getArray();
            if (marks)
                marks->add(seconds);
            save();
            return;
        }
    }

    // If not found, create new entry
    juce::DynamicObject::Ptr newObj = new juce::DynamicObject();
    newObj->setProperty("fileId", fileId);

    juce::Array<juce::var> marks;
    marks.add(seconds);
    newObj->setProperty("marks", marks);

    filesArray->add(newObj.get());
    save();
}

void MarkManager::removeMark(const juce::String& fileId, int index)
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
            auto* marks = obj->getProperty("marks").getArray();
            if (marks)
                marks->remove(index);
            save();
            return;
        }
    }

}

juce::Array<double> MarkManager::getMarks(const juce::String& fileId)
{
    juce::Array<double> result;
    auto* filesArray = data["files"].getArray();
    if (filesArray == nullptr)
        return result;

    for (auto& entry : *filesArray)
    {
        auto* obj = entry.getDynamicObject();
        if (obj && obj->getProperty("fileId") == fileId)
        {
            auto* marks = obj->getProperty("marks").getArray();
            if (marks)
                for (auto& m : *marks)
                    result.add((double)m);
        }
    }

    return result;
}
