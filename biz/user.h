#ifndef __USER_H__
#define __USER_H__

#include <memory>
#include <vector>
#include <concepts>
#include <string>
#include <atomic>
#include <chrono>
#include "packet.h"

class Session;

class User
{
public:
    using userid_t = std::uint32_t;

public:
    User(std::shared_ptr<Session> session, userid_t user_id);

    userid_t get_user_id() const;
    void set_username(std::string str);
    const std::string& get_username() const;

    bool is_connected() const;
    bool disconnect();

    template <PacketType T>
    bool send(const T& packet)
    {
        return send(packet.serialize());
    }

    bool send(const std::vector<std::byte>& data);
    
    bool is_login() const;
    void set_login(bool state);
    
public:
    struct PosInfo
    {
        float x;
        float y;
        bool facing_right;
    };
    PosInfo get_pos() const;
    void set_pos(float x, float y, bool facing_right);
    void set_facing(bool facing_right);

public:
    using clock_t = std::chrono::steady_clock;

    // Returns true and records the time when this attack kind is off cooldown.
    bool try_use_attack(int kind, clock_t::duration cooldown);

private:
    std::shared_ptr<Session> session_;
    std::atomic<bool> connected_{ true };
    userid_t user_id_;
    std::string username_;
    bool logined_{ false };

    std::pair<float, float> pos_{ 0.0f, 0.0f };
    bool facing_right_{ true };

    // Index 0: melee, 1: projectile
    clock_t::time_point last_attack_[2]{};
};

#endif // __USER_H__