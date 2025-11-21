#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/HardStreak.hpp>
#include <Geode/modify/PlayLayer.hpp>

bool isInResetStreak = false;
bool isInToggleWave = false;

using namespace geode::prelude;

class $modify(MyPlayerObject, PlayerObject) {
	void update(float dt) {
		PlayerObject::update(dt);
		if (!Mod::get()->getSettingValue<bool>("enabled")) return;
		if (!PlayLayer::get() || (this != PlayLayer::get()->m_player1 && this != PlayLayer::get()->m_player2)) return;
		if (this == PlayLayer::get()->m_player2 && !PlayLayer::get()->m_gameState.m_isDualMode) return;
		this->m_waveTrail->m_drawStreak = true;
		this->m_waveTrail->m_currentPoint = this->m_position;
		this->m_waveTrail->addPoint(this->m_position);
		this->m_waveTrail->setOpacity(255);
	}
	void resetStreak() {
		if (!Mod::get()->getSettingValue<bool>("enabled")) return PlayerObject::resetStreak();
		isInResetStreak = true;
		PlayerObject::resetStreak();
		isInResetStreak = false;
	}
	void toggleDartMode(bool p0, bool p1) {
		if (!Mod::get()->getSettingValue<bool>("enabled") || Mod::get()->getSettingValue<bool>("dontResetOnWavePortal")) return PlayerObject::toggleDartMode(p0, p1);
		isInToggleWave = true;
		auto fooBar = this->m_waveTrail->m_pointArray->data->num;
		this->m_waveTrail->m_pointArray->data->num = 0;
		PlayerObject::toggleDartMode(p0, p1);
		this->m_waveTrail->m_pointArray->data->num = fooBar;
		isInToggleWave = false;
	}
};

class $modify(MyHardStreak, HardStreak) {
	void stopStroke() {
		if (Mod::get()->getSettingValue<bool>("enabled")) return;
		HardStreak::stopStroke();
	}
	void clear() {
		if (Mod::get()->getSettingValue<bool>("enabled") && (isInResetStreak || isInToggleWave)) return;
		HardStreak::clear();
	}
};

class $modify(MyPlayLayer, PlayLayer) {
	void onQuit() {
		isInResetStreak = false;
		isInToggleWave = false;
		PlayLayer::onQuit();
	}
};