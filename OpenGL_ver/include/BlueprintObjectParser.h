#pragma once



class BlueprintObjectParser {
private:
	std::string ExtractStringBetweenTags(std::string stringToSearch, std::string startTag, std::string endTag);
public:
	void ParseNextObjectForBlueprint(std::ifstream& reader, GameObjectBlueprint& bp);
};
