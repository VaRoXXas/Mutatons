#pragma once

class Component;
class StandardGraphicsComponent : public GraphicsComponent {
private:
	sf::Sprite m_Sprite;
	std::string m_SpecificType = "standard";
public:
	/****************************************************
	*****************************************************
	From Component interface base class
	*****************************************************
	*****************************************************/
	std::string Component::GetSpecificType() {
		return m_SpecificType;
	}

	//void Component::start(GameObjectSharer* gos, GameObject* self) {
	//}

	void Draw(GLFWwindow& window, std::shared_ptr<TransformComponent> t) override;

	void InitializeGraphics(std::string textureName) override;
};
