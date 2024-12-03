#include "pch.h"
#include "PacketBase.hpp"
#include "IOExecutor.h"
#include "Session.h"
#include "MCWorld.h"
#include "MCTilemap.h"
#include "Object.h"
#include "EntityMovement.h"

// c2s�� �����ϴ� CPP

#define DECLARE_PACKET_FUNC(pkt_name) void pkt_name::Handle(const uint64 id, const pkt_name& pkt_) noexcept

DECLARE_PACKET_FUNC(c2s_LOGIN)
{
	s2c_LOGIN pkt;
	pkt.mc_seed = MCWorld::G_MC_SEED;
	pkt.id = (uint32)id;
	Mgr(IOExecutor)->GetSession(id)->ReserveSend(pkt);
}

DECLARE_PACKET_FUNC(c2s_ENTER)
{
	glm::vec3(256.0f, 16.0f, 256.0f);

	s2c_ENTER pkt;
	pkt.other_player_id = (uint32)id;
	Mgr(IOExecutor)->AppendToSendBuffer(pkt);

	const auto s = Mgr(IOExecutor)->GetSession(id);
	for (const auto& [id_, session] : Mgr(IOExecutor)->GetAllSessions())
	{
		if (id_ == id)continue;
		pkt.other_player_id = (uint32)id_;
		s->ReserveSend(pkt);
	}

	auto session = Mgr(IOExecutor)->GetSession(id);
	session->SetMyGameObject(std::make_shared<Object>(session));

	Mgr(MCWorld)->PostWorldEvent([session = std::move(session)]() {Mgr(MCWorld)->AddAllObjects(session); });
}

// ������ Ÿ�� ID�� ������ ID�� �����ϴ� ���̺�
std::unordered_map<int, int> g_tileToItemMap = {
	{1, 0},  // ���� 
	{2, 2},  // ��� --
	{3, 3},  // �ܵ�� --
	{4, 4},  // �������� --
	{5, 6},  // ����--
	{6, 7},  // ������ --
	{7, 1},  // ������ --
	{8, 5},  // �����ٺ� --
	{9, 8},  // ������ --
};

DECLARE_PACKET_FUNC(c2s_DESTROY_BLOCK)
{
	s2c_DESTROY_BLOCK pkt;
	pkt.x = pkt_.x;
	pkt.y = pkt_.y;
	pkt.z = pkt_.z;

	// ����� ������ �������� ��Ȯ�ϰ� �����ɴϴ�.
	auto tilemap = Mgr(MCWorld)->GetTileMap();
	pkt.tile_id = tilemap->GetTile({ pkt_.x, pkt_.y, pkt_.z });  // ����� ������ ������

	// ��� �ı� �� tile_id�� ��ȿ���� Ȯ���մϴ�.
	//if (pkt.tile_id == 0) {
	//	//std::cout << "No block to destroy at: (" << pkt_.x << ", " << pkt_.y << ", " << pkt_.z << ")\n";
	//	return; // �� �̻� ó���� �ʿ䰡 ����
	//}

	// ��� Ŭ���̾�Ʈ���� �� �ı� �˸�
	for (const auto& [id_, session] : Mgr(IOExecutor)->GetAllSessions())
	{
		session->ReserveSend(pkt);
	}

	// ���� ���ο��� ��� �ı� ó��
	//std::cout << "Block destroyed at: (" << pkt_.x << ", " << pkt_.y << ", " << pkt_.z << "), Tile ID: " << static_cast<int>(pkt.tile_id) << '\n';

	tilemap->SetTile({ pkt_.x, pkt_.y, pkt_.z }, 0);

	// ��� ������ ó��
	s2c_ITEM_DROP dropPkt;
	dropPkt.x = pkt.x + 0.5f;
	dropPkt.y = pkt.y + 0.5f;
	dropPkt.z = pkt.z + 0.5f;

	// ���� ���̺��� ����Ͽ� ��� ������ Ÿ�� ����
	auto it = g_tileToItemMap.find(pkt.tile_id);
	if (it != g_tileToItemMap.end()) {
		dropPkt.item_type = it->second;
	}
	else {
		std::cout << "Invalid drop item type for tile ID: " << pkt.tile_id << '\n';
		return; // ���ε��� ���� Ÿ���� ��� ��� ������ ����
	}

	// ��ӵ� �������� ���� ID�� ���� (�� �����ۿ� ���� ������ ID�� �ο��ؾ� ��)
	//static uint32_t unique_item_id = 1;

	static std::atomic<uint32_t> unique_item_id = 11; // ������ �������� ����
	dropPkt.obj_id = unique_item_id++;

	// ��ӵ� �������� ������ ����Ͽ� Ȯ��
	/*std::cout << "Item dropped at: (" << dropPkt.x << ", " << dropPkt.y << ", " << dropPkt.z
		<< "), Item Type: " << static_cast<int>(dropPkt.item_type)
		<< ", Object ID: " << dropPkt.obj_id << '\n';*/

	// ��� ���� ��� Ŭ���̾�Ʈ���� ����
	for (const auto& [id_, session] : Mgr(IOExecutor)->GetAllSessions())
	{
		session->ReserveSend(dropPkt);
	}
}


DECLARE_PACKET_FUNC(c2s_CREATE_BLOCK)
{
	s2c_CREATE_BLOCK pkt;
	pkt.x = pkt_.x;
	pkt.y = pkt_.y;
	pkt.z = pkt_.z;
	pkt.tile_id = pkt_.tile_id;
	
	Mgr(IOExecutor)->AppendToSendBuffer(pkt);

	Mgr(MCWorld)->GetTileMap()->SetTile({ pkt_.x ,pkt_.y ,pkt_.z }, pkt_.tile_id);
}

DECLARE_PACKET_FUNC(c2s_ADD_OBJECT)
{
	s2c_ADD_OBJECT pkt;
	pkt.object_id = pkt_.object_id;
	pkt.position_x = pkt_.position_x;
	pkt.position_y = pkt_.position_y;
	pkt.position_z = pkt_.position_z;
	pkt.rotation_y = pkt_.rotation_y;
	pkt.obj_type = (uint8)MC_OBJECT_TYPE::PLAYER;
	Mgr(IOExecutor)->AppendToSendBuffer(pkt);
}


DECLARE_PACKET_FUNC(c2s_MOVE_OBJECT)
{
	s2c_MOVE_OBJECT pkt;
	pkt.object_id = (uint32)id;
	pkt.position_x = pkt_.position_x;
	pkt.position_y = pkt_.position_y;
	pkt.position_z = pkt_.position_z;

	pkt.acceleration_x = pkt_.acceleration_x;
	pkt.acceleration_y = pkt_.acceleration_y;
	pkt.acceleration_z = pkt_.acceleration_z;

	pkt.velocity_x = pkt_.velocity_x;
	pkt.velocity_y = pkt_.velocity_y;
	pkt.velocity_z = pkt_.velocity_z;

	pkt.pitch = pkt_.pitch;
	pkt.rotation_y = pkt_.rotation_y;
	pkt.yaw = pkt_.yaw;


	const auto& players = Mgr(MCWorld)->GetWorldObjects(MC_OBJECT_TYPE::PLAYER);
	auto b = players.data();
	const auto e = b + players.size();
	while (e != b) 
	{
		const auto& obj = *b++;
		if (obj->GetObjectID() == id)
		{
			const auto movement = obj->GetEntityMovement();
			movement->current_position = { pkt_.position_x ,pkt_.position_y ,pkt_.position_z };
			obj->SetPos(movement->current_position);
			movement->m_vVelocity = { pkt_.velocity_x ,pkt_.velocity_y ,pkt_.velocity_z };
			movement->m_vAccelation = { pkt_.acceleration_x ,pkt_.acceleration_y ,pkt_.acceleration_z };
			movement->m_cameraAngleAxisSmooth = { pkt_.cam_x,pkt_.cam_y ,pkt_.cam_z };
			break;
		}
	}
	
	Mgr(IOExecutor)->AppendToSendBuffer(pkt);
}

DECLARE_PACKET_FUNC(c2s_ADD_PROJECTILE)
{

}

DECLARE_PACKET_FUNC(c2s_USE_ITEM)
{

}

