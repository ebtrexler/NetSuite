// core/io/rig_profile.h
//
// A RigProfile describes the physical wiring of one experimental rig:
// which NI-DAQmx channels a named biological cell "role" is connected
// to, the amplifier gain constants for that channel pair, and the
// per-role current-injection limits.
//
// Network .json files reference roles by name (e.g. "Cell_A") rather
// than hardcoding `Dev1/ai0`, `AIGain=10`, etc. A rig profile provides
// the missing half — the mapping from role → hardware — so the same
// network model can run on different rigs just by activating a
// different profile.
//
// Profile files are plain JSON, stored by the RigProfileManager under
// the user's config directory. See User_Guide.md §Rig Profiles for the
// user-facing workflow.
//
// Header-only to keep the dependency graph shallow: only nlohmann::json
// and the standard library.

#ifndef RIG_PROFILE_H
#define RIG_PROFILE_H

#include <algorithm>
#include <fstream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "json.hpp"

/// One role's hardware binding on a specific rig.
struct RoleBinding {
    std::string role;           // symbolic name (e.g. "Cell_A")
    std::string ai_channel;     // e.g. "Dev1/ai0"
    std::string ao_channel;     // e.g. "Dev1/ao0"
    double ai_gain = 1.0;       // mV / V
    double ao_gain = 1.0;       // V / nA
    double pos_limit_nA = 10.0;
    double neg_limit_nA = -10.0;
};

/// A named collection of role bindings — one rig's profile.
struct RigProfile {
    std::string name;           // unique within a user's profile folder
    std::string description;    // free-form
    std::vector<RoleBinding> bindings;

    /// Lookup by role name. Returns nullopt if the role isn't in this
    /// profile — the caller decides what to do (fall back to whatever
    /// is stored in the network file, warn the user, etc.).
    std::optional<RoleBinding> find(const std::string& role) const {
        auto it = std::find_if(bindings.begin(), bindings.end(),
            [&](const RoleBinding& rb) { return rb.role == role; });
        if (it == bindings.end()) return std::nullopt;
        return *it;
    }

    /// All defined role names, in insertion order.
    std::vector<std::string> role_names() const {
        std::vector<std::string> out;
        out.reserve(bindings.size());
        for (const auto& b : bindings) out.push_back(b.role);
        return out;
    }
};

// ---------------------------------------------------------------------------
// JSON roundtrip
// ---------------------------------------------------------------------------
// Schema version is carried in the file so we can evolve the format.

namespace rig_profile_json {

constexpr int kSchemaVersion = 1;

inline nlohmann::json binding_to_json(const RoleBinding& b) {
    nlohmann::json j;
    j["role"]        = b.role;
    j["ai_channel"]  = b.ai_channel;
    j["ao_channel"]  = b.ao_channel;
    j["ai_gain"]     = b.ai_gain;
    j["ao_gain"]     = b.ao_gain;
    j["pos_limit_nA"] = b.pos_limit_nA;
    j["neg_limit_nA"] = b.neg_limit_nA;
    return j;
}

inline RoleBinding binding_from_json(const nlohmann::json& j) {
    RoleBinding b;
    if (j.contains("role"))         b.role = j["role"].get<std::string>();
    if (j.contains("ai_channel"))   b.ai_channel = j["ai_channel"].get<std::string>();
    if (j.contains("ao_channel"))   b.ao_channel = j["ao_channel"].get<std::string>();
    if (j.contains("ai_gain"))      b.ai_gain = j["ai_gain"].get<double>();
    if (j.contains("ao_gain"))      b.ao_gain = j["ao_gain"].get<double>();
    if (j.contains("pos_limit_nA")) b.pos_limit_nA = j["pos_limit_nA"].get<double>();
    if (j.contains("neg_limit_nA")) b.neg_limit_nA = j["neg_limit_nA"].get<double>();
    return b;
}

inline nlohmann::json to_json(const RigProfile& p) {
    nlohmann::json j;
    j["schema_version"] = kSchemaVersion;
    j["name"] = p.name;
    j["description"] = p.description;
    nlohmann::json bindings = nlohmann::json::array();
    for (const auto& b : p.bindings) bindings.push_back(binding_to_json(b));
    j["bindings"] = bindings;
    return j;
}

inline RigProfile from_json(const nlohmann::json& j) {
    RigProfile p;
    if (j.contains("name"))        p.name = j["name"].get<std::string>();
    if (j.contains("description")) p.description = j["description"].get<std::string>();
    if (j.contains("bindings") && j["bindings"].is_array()) {
        for (const auto& bj : j["bindings"]) p.bindings.push_back(binding_from_json(bj));
    }
    return p;
}

inline bool save(const RigProfile& p, const std::string& path,
                 std::string* err = nullptr) {
    std::ofstream out(path);
    if (!out) {
        if (err) *err = "cannot open " + path + " for writing";
        return false;
    }
    out << to_json(p).dump(2);
    return out.good();
}

inline bool load(const std::string& path, RigProfile& out,
                 std::string* err = nullptr) {
    std::ifstream in(path);
    if (!in) {
        if (err) *err = "cannot open " + path;
        return false;
    }
    try {
        nlohmann::json j;
        in >> j;
        // Future: branch on j["schema_version"] if we bump the format.
        out = from_json(j);
        return true;
    } catch (const std::exception& e) {
        if (err) *err = std::string("parse error: ") + e.what();
        return false;
    }
}

}  // namespace rig_profile_json

// ---------------------------------------------------------------------------
// Manager: the app-wide handle to "which profile is active"
// ---------------------------------------------------------------------------
//
// The manager is deliberately simple — it holds a pointer to the active
// profile. There is one instance, accessed via RigProfileManager::instance().
// UI code updates the active pointer; loaders / dialogs consult it.
//
// File discovery (listing all profile files on disk, choosing a
// default directory, etc.) is the UI's job; the manager only deals
// with an already-loaded profile.

class RigProfileManager {
public:
    static RigProfileManager& instance() {
        static RigProfileManager m;
        return m;
    }

    /// Set the active profile. Pass nullptr to clear.
    void set_active(std::shared_ptr<RigProfile> p) { m_active = std::move(p); }

    /// Current active profile, or nullptr if none.
    std::shared_ptr<const RigProfile> active() const { return m_active; }

    /// Convenience: resolve a role via the active profile.
    /// Returns nullopt if no profile is active OR the role isn't in it.
    std::optional<RoleBinding> resolve(const std::string& role) const {
        if (!m_active || role.empty()) return std::nullopt;
        return m_active->find(role);
    }

private:
    RigProfileManager() = default;
    std::shared_ptr<RigProfile> m_active;
};

#endif  // RIG_PROFILE_H
