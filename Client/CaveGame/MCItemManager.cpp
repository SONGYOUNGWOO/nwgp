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

	// ���߿� �������� ����ʹٸ�
	//MCItemTable::CreateItemForClient<MCArrow>("arrow")

	for (const auto& [key, val] : MCItemTable::GetStrTable())
	{
		// m_registry.Insert(key, make_shared<MCItem>(val.first, val.second));

		m_registry.Insert(key.data(), MCItemTable::CreateItemForClient<MCItem>(key));
	}

	// ���� ���̺� �ʱ�ȭ
	//InitializeTileToItemMap();
}
void MCItemManager::InitializeTileToItemMap()
{
	//// Ÿ�� ID�� ������ ID�� �ϰ��� ���� ����
	//m_tileToItemMap[1] = 1;  // ���� -> �� ������
	//m_tileToItemMap[2] = 2;  // ��� -> �� ������
	//m_tileToItemMap[3] = 3;  // �ܵ�� -> �ܵ� ������
	//m_tileToItemMap[4] = 4;  // ���� -> �� ������
	//m_tileToItemMap[5] = 8;  // �����ٺ� -> ������ ������
	//m_tileToItemMap[6] = 6;  // ���� -> �� ������
	//m_tileToItemMap[7] = 7;  // ������ -> ���� ������
	//m_tileToItemMap[8] = 5;  // �ܵ�� -> �ܵ� ������
	//m_tileToItemMap[9] = 9;  // ������ -> ���� ������
	//// �ʿ��� ��� ������ �߰�
}

int MCItemManager::GetItemIDForTileID(int tileID) const
{
	//auto it = m_tileToItemMap.find(tileID);
	//if (it != m_tileToItemMap.end()) {
	//	return it->second;
	//}
	//return -1; // ���ε��� ���� ���
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
