#pragma once

#include "Singleton.hpp"
#include "Registry.hpp"
#include "MCItem.h"

class MCItemManager : public Singleton<MCItemManager>
{
private:
	Registry<MCItem> m_registry;
	std::unordered_map<int, int> m_tileToItemMap;  // 타일 ID -> 아이템 ID 매핑 테이블
	void InitializeTileToItemMap();  // 추가: 매핑 테이블 초기화 함수

public:
	friend class Singleton;

	MCItemManager();
	~MCItemManager();

	void LoadItems();
	void Initialize();

	MCItem* GetItemByID(int id) const;
	MCItem* GetItemByKey(const std::string& key) const;

	int GetItemIDForTileID(int tileID) const;  // 추가: 타일 ID에 해당하는 아이템 ID 가져오기
};

