#pragma once

#include "json.hpp"
#include "Resource.hpp"

#include <filesystem>
#include <fstream>
#include <mutex>
#include <unordered_map>
#include <map>
#include <string>
#include <random>
#include <sstream>
#include <iomanip>

class JsonDB
{
public:
    explicit JsonDB(const std::string &filepath)
        : m_filepath(filepath)
    {
        load();
    }

    bool userExists(const std::string &username) const
    {
        std::lock_guard lk(m_mutex);
        return m_accounts.count(username) > 0;
    }

    bool verifyUser(const std::string &username,
                    const std::string &password) const
    {
        std::lock_guard lk(m_mutex);
        auto it = m_accounts.find(username);
        return it != m_accounts.end() && it->second.password == password;
    }

    std::string getUser(const std::string &username,
                        const std::string &password) const
    {
        std::lock_guard lk(m_mutex);
        auto it = m_accounts.find(username);
        if (it == m_accounts.end())
            throw std::runtime_error("User not found: " + username);
        if (it->second.password != password)
            throw std::runtime_error("Invalid password for " + username);
        return it->second.uuid;
    }

    std::string addUser(const std::string &username,
                        const std::string &password)
    {
        std::lock_guard lk(m_mutex);
        if (m_accounts.count(username))
            throw std::runtime_error("User already exists: " + username);
        Account acc;
        acc.password = password;
        acc.uuid = generateUuid();
        m_accounts[username] = acc;
        save();
        return acc.uuid;
    }

    Resources getResources(const std::string &uuid) const
    {
        std::lock_guard lk(m_mutex);
        for (auto const &[user, acc] : m_accounts)
            if (acc.uuid == uuid)
                return acc.resources;
        throw std::runtime_error("UUID not found: " + uuid);
    }

    void updateResources(const std::string &uuid, const Resources &r)
    {
        std::lock_guard lk(m_mutex);
        for (auto &[user, acc] : m_accounts)
        {
            if (acc.uuid == uuid)
            {
                acc.resources = r;
                save();
                return;
            }
        }
        throw std::runtime_error("UUID not found: " + uuid);
    }

    std::map<std::string, int> getCampLevels(const std::string &uuid) const
    {
        std::lock_guard lk(m_mutex);
        for (auto const &[user, acc] : m_accounts)
            if (acc.uuid == uuid)
                return acc.campLevels;
        throw std::runtime_error("UUID not found: " + uuid);
    }

    int getCampLevel(const std::string &uuid,
                     const std::string &camp) const
    {
        auto levels = getCampLevels(uuid);
        auto it = levels.find(camp);
        return it == levels.end() ? 0 : it->second;
    }

    void setCampLevel(const std::string &uuid,
                      const std::string &camp,
                      int level)
    {
        std::lock_guard lk(m_mutex);
        for (auto &[user, acc] : m_accounts)
        {
            if (acc.uuid == uuid)
            {
                acc.campLevels[camp] = level;
                save();
                return;
            }
        }
        throw std::runtime_error("UUID not found: " + uuid);
    }

private:
    struct Account
    {
        std::string password;
        std::string uuid;
        Resources resources;
        std::map<std::string, int> campLevels;
    };

    const std::string m_filepath;
    mutable std::mutex m_mutex;
    std::unordered_map<std::string, Account> m_accounts;

    void load()
    {
        std::ifstream in(m_filepath);
        if (!in)
            return;

        json top;
        in >> top;
        auto jacc = top.value("accounts", json::object());
        for (auto &it : jacc.items())
        {
            Account acc;
            auto &val = it.value();
            acc.password = val.value("password", "");
            acc.uuid = val.value("uuid", "");

            // resources
            auto jres = val.value("resources", json::object());
            acc.resources.wood = jres.value("wood", 0ULL);
            acc.resources.food = jres.value("food", 0ULL);
            acc.resources.iron = jres.value("iron", 0ULL);

            // campLevels
            acc.campLevels.clear();
            if (val.contains("campLevels"))
            {
                for (auto &cl : val["campLevels"].items())
                {
                    acc.campLevels[cl.key()] = cl.value().get<int>();
                }
            }

            m_accounts[it.key()] = std::move(acc);
        }
    }

    void save()
    {
        json top;
        json jacc = json::object();
        for (auto &[user, acc] : m_accounts)
        {
            json val;
            val["password"] = acc.password;
            val["uuid"] = acc.uuid;

            val["resources"] = {
                {"wood", acc.resources.wood},
                {"food", acc.resources.food},
                {"iron", acc.resources.iron}};

            json jcl = json::object();
            for (auto &[camp, lvl] : acc.campLevels)
            {
                jcl[camp] = lvl;
            }
            val["campLevels"] = jcl;

            jacc[user] = val;
        }
        top["accounts"] = jacc;

        // atomic write
        auto tmp = m_filepath + ".tmp";
        {
            std::ofstream out(tmp);
            out << top.dump(2) << "\n";
        }
        std::filesystem::rename(tmp, m_filepath);
    }

    static std::string generateUuid()
    {
        static std::random_device rd;
        static std::mt19937_64 gen(rd());
        static std::uniform_int_distribution<uint64_t> dist;
        uint64_t a = dist(gen), b = dist(gen);
        std::ostringstream os;
        os << std::hex << std::setw(16) << std::setfill('0') << a
           << std::setw(16) << std::setfill('0') << b;
        return os.str();
    }
};