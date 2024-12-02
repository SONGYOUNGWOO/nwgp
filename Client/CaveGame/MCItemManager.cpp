#include "pch.h"
#include "MCItemManager.h"
#include "MCItemTable.h"

MCItemManager::MCItemManager()
{
}

MCItemManager::~MCItemManager()
{
}

void MCItemManager::LoadItems()
{
	//m_registry.Insert("block_stone", make_shared<MCItem>("Stone", "tile_preview_01.png"));
	//m_registry.Insert("block_dirt", make_shared<MCItem>("Dirt", "tile_preview_02.png"));
	//m_registry.Insert("block_grass", make_shared<MCItem>("Grass", "tile_preview_03.png"));
	//m_registry.Insert("block_cobblestone", make_shared<MCItem>("Cobblestone", "tile_preview_04.png"));
	//m_registry.Insert("block_wooden_planks", make_shared<MCItem>("Wooden Planks", "tile_preview_05.png"));
	//m_registry.Insert("block_bricks", make_shared<MCItem>("Bricks", "tile_preview_06.png"));
	//m_registry.Insert("block_wood", make_shared<MCItem>("Wood", "tile_preview_07.png"));
	//m_registry.Insert("block_leaves", make_shared<MCItem>("Leaves", "tile_preview_08.png"));
	//m_registry.Insert("block_glass", make_shared<MCItem>("Glass", "tile_preview_09.png"));
	//m_registry.Insert("eye", make_shared<MCItem>("Ender Eye", "item_eye.png"));
	//m_registry.Insert("arrow", make_shared<MCItem>("Arrow", "item_arrow.png"));

	// 나중에 다형성을 쓰고싶다면
	//MCItemTable::CreateItemForClient<MCArrow>("arrow")

	for (const auto& [key, val] : MCItemTable::GetStrTable())
	{
		// m_registry.Insert(key, make_shared<MCItem>(val.first, val.second));

		m_registry.Insert(key.data(), MCItemTable::CreateItemForClient<MCItem>(key));
	}

	// 매핑 테이블 초기화
	//InitializeTileToItemMap();
}
void MCItemManager::InitializeTileToItemMap()
{
	//// 타일 ID와 아이템 ID의 일관된 매핑 정의
	//m_tileToItemMap[1] = 1;  // 돌블럭 -> 돌 아이템
	//m_tileToItemMap[2] = 2;  // 흙블럭 -> 흙 아이템
	//m_tileToItemMap[3] = 3;  // 잔디블럭 -> 잔디 아이템
	//m_tileToItemMap[4] = 4;  // 돌블럭 -> 돌 아이템
	//m_tileToItemMap[5] = 8;  // 나뭇잎블럭 -> 나뭇잎 아이템
	//m_tileToItemMap[6] = 6;  // 돌블럭 -> 돌 아이템
	//m_tileToItemMap[7] = 7;  // 나무블럭 -> 나무 아이템
	//m_tileToItemMap[8] = 5;  // 잔디블럭 -> 잔디 아이템
	//m_tileToItemMap[9] = 9;  // 유리블럭 -> 유리 아이템
	//// 필요한 모든 매핑을 추가
}

int MCItemManager::GetItemIDForTileID(int tileID) const
{
	//auto it = m_tileToItemMap.find(tileID);
	//if (it != m_tileToItemMap.end()) {
	//	return it->second;
	//}
	//return -1; // 매핑되지 않은 경우
}

void MCItemManager::Initialize()
{

}

MCItem* MCItemManager::GetItemByID(int id) const
{
	return m_registry[id];
}

MCItem* MCItemManager::GetItemByKey(const std::string& key) const
{
	return m_registry[key];
}
