#include "pch.h"
#include "GameObject/GameObjectBlueprint.h"
#include "ObjectTags.h"
#include "GameObject/BlueprintObjectParser.h"



//That's file parser, all the file contents are being read and written to the GameObjectBlueprint object
void BlueprintObjectParser::ParseNextObjectForBlueprint(std::ifstream& reader, GameObjectBlueprint& bp)
{
	std::string lineFromFile;
	std::string value = "";
	while (getline(reader, lineFromFile))
	{
		if (lineFromFile.find(ObjectTags::COMPONENT) != std::string::npos)
		{
			value = ExtractStringBetweenTags(lineFromFile, ObjectTags::COMPONENT, ObjectTags::COMPONENT_END);
			bp.AddToComponentList(value);
		}
		else if (lineFromFile.find(ObjectTags::TAG) != std::string::npos)
		{
			value = ExtractStringBetweenTags(lineFromFile, ObjectTags::TAG, ObjectTags::TAG_END);
			bp.SetTag(value);
		}
		else if (lineFromFile.find(ObjectTags::X) != std::string::npos)
		{
			value = ExtractStringBetweenTags(lineFromFile, ObjectTags::X, ObjectTags::X_END);
			bp.SetX(stof(value));
		}
		else if (lineFromFile.find(ObjectTags::Y) != std::string::npos)
		{
			value = ExtractStringBetweenTags(lineFromFile, ObjectTags::Y, ObjectTags::Y_END);
			bp.SetY(stof(value));
		}
		else if (lineFromFile.find(ObjectTags::Z) != std::string::npos)
		{
			value = ExtractStringBetweenTags(lineFromFile, ObjectTags::Z, ObjectTags::Z_END);
			bp.SetZ(stof(value));
		}
		else if (lineFromFile.find(ObjectTags::SCALE) != std::string::npos)
		{
			value = ExtractStringBetweenTags(lineFromFile, ObjectTags::SCALE, ObjectTags::SCALE_END);
			bp.SetScale(stof(value));
		}
		else if (lineFromFile.find(ObjectTags::ROTATION) != std::string::npos)
		{
			value = ExtractStringBetweenTags(lineFromFile, ObjectTags::ROTATION, ObjectTags::ROTATION_END);
			bp.SetRotation(stof(value));
		}
		else if (lineFromFile.find(ObjectTags::ROTATION_AXIS) != std::string::npos)
		{
			value = ExtractStringBetweenTags(lineFromFile, ObjectTags::ROTATION_AXIS, ObjectTags::ROTATION_AXIS_END);
			bp.SetRotationAxis(value[0]);
		}
		else if (lineFromFile.find(ObjectTags::MODEL_ID) != std::string::npos)
		{
			value = ExtractStringBetweenTags(lineFromFile, ObjectTags::MODEL_ID, ObjectTags::MODEL_ID_END);
			bp.SetModelID(stoi(value));
		}
		else if (lineFromFile.find(ObjectTags::END_OF_OBJECT) != std::string::npos)
		{
			return;
		}
	}
}

// this function captures the content between two tags
std::string BlueprintObjectParser::ExtractStringBetweenTags(std::string stringToSearch, std::string startTag, std::string endTag)
{
	int start = startTag.length();
	int count = stringToSearch.length() - startTag.length() - endTag.length();
	std::string stringBetweenTags = stringToSearch.substr(start, count);
	return stringBetweenTags;
}