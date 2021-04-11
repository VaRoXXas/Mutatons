#pragma once

class UpdateComponent : public Component
{
private:
	std::string m_Type = "update";
	bool m_Enabled = false;
public:
	virtual void Update(float fps) = 0;

	std::string Component::GetType() {
		return m_Type;
	}
	void Component::DisableComponent() {
		m_Enabled = false;
	}
	void Component::EnableComponent() {
		m_Enabled = true;
	}
	bool Component::Enabled() {
		return m_Enabled;
	}
	//void Component::Start(
	//	GameObjectSharer* gos, GameObject* self) {
	//}
};

