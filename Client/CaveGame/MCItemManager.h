#pragma once

#include "Singleton.hpp"
#include "Registry.hpp"
#include "MCItem.h"

class MCItemManager : public Singleton<MCItemManager>
{
private:
	Registry<MCItem> m_registry;
	std::unordered_map<int, int> m_tileToItemMap; 
	void InitializeTileToItemMap();  

public:
	friend class Singleton;

	MCItemManager();
	~MCItemManager();

	void LoadItems();
	void Initialize();

	MCItem* GetItemByID(int id) const;
	MCItem* GetItemByKey(const std::string& key) const;

	int GetItemIDForTileID(int tileID) const;  // �߰�: Ÿ�� ID�� �ش��ϴ� ������ ID ��������
};

