#pragma once
#include <cstring>
#include "packet_define.h"

using uint64 = unsigned long long;
using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;

extern void AddProtocol(const uint16 pktID_, void (*fpPacketHandler_)(const uint64, const char* const))noexcept;


#pragma pack (push, 1)

struct PacketHeader
{
    const uint8 pkt_size;
    const uint8 pkt_id;
};

// 패킷 구조체 이름과 동일하게 만들어야 편하다
enum class PKT_ID : uint8
{
    c2s_LOGIN = 0,
    s2c_LOGIN = 1,

    c2s_ENTER = 2,
    s2c_ENTER = 3,

    c2s_DESTROY_BLOCK = 4,
    s2c_DESTROY_BLOCK = 5,

    c2s_CREATE_BLOCK = 6,
    s2c_CREATE_BLOCK = 7,

    c2s_ADD_OBJECT = 8,
    s2c_ADD_OBJECT = 9,

    c2s_MOVE_OBJECT = 10,
    s2c_MOVE_OBJECT = 11,

    c2s_ADD_PROJECTILE = 12,
    s2c_ADD_PROJECTILE = 13,

    c2s_USE_ITEM = 14,
    s2c_USE_ITEM = 15,

    s2c_REMOVE_OBJECT = 16,


    END,
};

template <typename T>
static constexpr inline unsigned char etoi(const T eType)noexcept { return static_cast<uint8>(eType); }

#pragma pack (pop)

#define DECLARE_PACKET(pkt_name)                                                                    \
    pkt_name() : PacketHeader{sizeof(pkt_name), etoi(PKT_ID::pkt_name)} {}                          \
    private:                                                                                        \
       static void HandlePacket(const uint64 id, const char* const pBuff_) {                        \
        return pkt_name::Handle(id, *reinterpret_cast<const pkt_name* const>(pBuff_));              \
    }                                                                                               \
    static bool RegisterHandleFunc() noexcept                                                       \
    {                                                                                               \
        if constexpr ((#pkt_name[0] == 'c' && G_PROJECT == PROJECT_TYPE::SERVER) ||                 \
                      (#pkt_name[0] == 's' && G_PROJECT == PROJECT_TYPE::CLIENT))                   \
        {                                                                                           \
            pkt_name temp;                                                                          \
            AddProtocol(static_cast<unsigned short>(temp.pkt_id), &pkt_name::HandlePacket);         \
        }                                                                                           \
        return true;                                                                                \
    }                                                                                               \
    static const inline bool g_bInitPacket = RegisterHandleFunc();                                  \
    static void Handle(const uint64 id, const pkt_name& pkt_) noexcept;


// 패킷만들기
// 1. 패킷 구조체 이름과 동일한 enum 위에다가 추가해주기
// 2. 밑에 #define하기
// 3. 함수정의만들기 (Handle)
// 4. 필요한 멤버변수들 추가하기
// 5. Nice..

static inline int OnRecv(const uint64 id, char* const buffer, const int len, void (* const* const packet_func)(const uint64_t, const char* const))noexcept
{
    int processLen = 0;

    for (;;)
    {
        const int dataSize = len - processLen;

        if (dataSize < static_cast<int>(sizeof(PacketHeader)))
            break;

        const PacketHeader* const __restrict header = reinterpret_cast<const PacketHeader* const>(buffer + processLen);
        const uint8 packetSize = header->pkt_size;
        const uint8 packetId = header->pkt_id;

        if (dataSize < packetSize)
            break;

        packet_func[packetId](id, (char*)header);

        processLen += packetSize;
    }

    return processLen;
}

//---------------------------------------------------------------------------------------------------------//
#pragma pack (push, 1)

// c2s의 핸들러 함수는 서버에,
// s2c의 핸들러 함수는 클라이언트 쪽에서 만들어야 함

/// <summary>
///  로그인
/// </summary>
struct c2s_LOGIN
    :public PacketHeader
{
    DECLARE_PACKET(c2s_LOGIN);
};

struct s2c_LOGIN
    :public PacketHeader
{
    int mc_seed;
    uint32 id;
    DECLARE_PACKET(s2c_LOGIN);
};
//

/// <summary>
/// 입장
/// </summary>

struct c2s_ENTER
    :public PacketHeader
{
    DECLARE_PACKET(c2s_ENTER);
};

struct s2c_ENTER
    :public PacketHeader
{
    uint32 other_player_id;
    DECLARE_PACKET(s2c_ENTER);
};
//

/// <summary>
/// 블록 파괴
/// </summary>
struct c2s_DESTROY_BLOCK
    :public PacketHeader
{
    uint8 x, y, z;
    DECLARE_PACKET(c2s_DESTROY_BLOCK);
};

struct s2c_DESTROY_BLOCK
    :public PacketHeader
{
    uint8 x, y, z;
    DECLARE_PACKET(s2c_DESTROY_BLOCK);
};
//

/// <summary>
/// 블록 설치
/// </summary>
struct c2s_CREATE_BLOCK
    :public PacketHeader
{
    uint8 x, y, z;
    uint8_t tile_id;
    DECLARE_PACKET(c2s_CREATE_BLOCK);
};

struct s2c_CREATE_BLOCK
    :public PacketHeader
{
    uint8 x, y, z;
    uint8_t tile_id;
    DECLARE_PACKET(s2c_CREATE_BLOCK);
};

/// <summary>
/// 오브젝트 생성
/// </summary>
struct c2s_ADD_OBJECT
    :public PacketHeader
{
    uint32 object_id;
    float position_x, position_y, position_z;
    float rotation_y;
    DECLARE_PACKET(c2s_ADD_OBJECT);
};

struct s2c_ADD_OBJECT
    :public PacketHeader
{
    uint32 object_id;
    float position_x, position_y, position_z;
    float rotation_y;
    uint8 obj_type;
    DECLARE_PACKET(s2c_ADD_OBJECT);
};

/// <summary>
/// 오브젝트 이동
/// </summary>
struct c2s_MOVE_OBJECT
    :public PacketHeader
{
    float position_x, position_y, position_z;
    float velocity_x, velocity_y, velocity_z;
    float acceleration_x, acceleration_y, acceleration_z;
    float rotation_y;
    float yaw, pitch;
    DECLARE_PACKET(c2s_MOVE_OBJECT);
};

struct s2c_MOVE_OBJECT
    :public PacketHeader
{
    uint32 object_id;
    float position_x, position_y, position_z;
    float velocity_x, velocity_y, velocity_z;
    float acceleration_x, acceleration_y, acceleration_z;
    float rotation_y;
    float yaw, pitch;
    DECLARE_PACKET(s2c_MOVE_OBJECT);
};

/// <summary>
/// 투사체 생성
/// </summary>
struct c2s_ADD_PROJECTILE
    :public PacketHeader
{
    float position_x, position_y, position_z;
    float velocity_x, velocity_y, velocity_z;
    DECLARE_PACKET(c2s_ADD_PROJECTILE);
};

struct s2c_ADD_PROJECTILE
    :public PacketHeader
{
    uint32 projectile_id;
    float position_x, position_y, position_z;
    float velocity_x, velocity_y, velocity_z;
    DECLARE_PACKET(s2c_ADD_PROJECTILE);
};

/// <summary>
/// 아이템 사용
/// </summary>
struct c2s_USE_ITEM
    :public PacketHeader
{
    uint8 item_id;
    DECLARE_PACKET(c2s_USE_ITEM);
};

struct s2c_USE_ITEM
    :public PacketHeader
{
    uint8 item_id;
    uint8 user_id;
    uint32 object_id;
    DECLARE_PACKET(s2c_USE_ITEM);
};

/// <summary>
/// 오브젝트 삭제
/// </summary>

struct s2c_REMOVE_OBJECT
    :public PacketHeader
{
    uint32 object_id;
    DECLARE_PACKET(s2c_REMOVE_OBJECT);
};

/// <summary>
/// 플레이어 처치
/// </summary>
struct c2s_KILL_PLAYER {
    uint32 target_id;  // 처치 대상의 플레이어 ID
    DECLARE_PACKET(c2s_KILL_PLAYER);
};

struct s2c_KILL_PLAYER {
    uint32 killer_id;  // 처치한 플레이어 ID
    uint32 target_id;  // 처치된 플레이어 ID
    bool success;      // 처치 성공 여부
    DECLARE_PACKET(s2c_KILL_PLAYER);
};

/// <summary>
/// 몬스터 처치
/// </summary>

struct c2s_KILL_MONSTER {
    uint32 monster_id; // 처치 대상 몬스터 ID
    DECLARE_PACKET(c2s_KILL_MONSTER);
};

struct s2c_KILL_MONSTER {
    uint32 killer_id;   // 몬스터를 처치한 플레이어 ID
    uint32 monster_id;  // 처치된 몬스터 ID
    bool success;       // 처치 성공 여부
    DECLARE_PACKET(s2c_KILL_MONSTER);
};

/// <summary>
/// 몬스터 처치
/// </summary>
struct c2s_SPAWN_BOSS {
    uint32 boss_id;       // 소환할 보스의 고유 ID
    float position_x;     // 소환할 위치 X
    float position_y;     // 소환할 위치 Y
    float position_z;     // 소환할 위치 Z
    DECLARE_PACKET(c2s_SPAWN_BOSS);
};
struct s2c_SPAWN_BOSS {
    uint32 boss_id;       // 소환된 보스의 고유 ID
    float position_x;     // 소환된 위치 X
    float position_y;     // 소환된 위치 Y
    float position_z;     // 소환된 위치 Z
    bool success;         // 소환 성공 여부
    DECLARE_PACKET(s2c_SPAWN_BOSS);
};
#pragma pack (pop)