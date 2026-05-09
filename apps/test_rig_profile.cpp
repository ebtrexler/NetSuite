// apps/test_rig_profile.cpp
//
// Unit tests for RigProfile: JSON roundtrip, multi-binding correctness,
// missing-role fallback, and manager active-profile state.

#include "rig_profile.h"
#include "network_json.h"
#include "RT_BiologicalCell.h"
#include "factory.h"

#include <cstdio>
#include <filesystem>
#include <string>

static int g_failures = 0;

#define CHECK(cond) do {                                                     \
    if (!(cond)) {                                                           \
        std::fprintf(stderr, "  [FAIL] %s:%d: %s\n",                         \
                     __FILE__, __LINE__, #cond);                             \
        ++g_failures;                                                        \
        return;                                                              \
    }                                                                        \
} while (0)

static void run(const char *name, void (*test)()) {
    int before = g_failures;
    std::printf("[TEST] %s ...\n", name);
    test();
    if (g_failures == before) std::printf("       OK\n");
}

static std::filesystem::path test_dir() {
    auto d = std::filesystem::temp_directory_path() / "netrex_rig_profile_tests";
    std::filesystem::create_directories(d);
    return d;
}

// ---------------------------------------------------------------------------

static RigProfile make_sample_profile() {
    RigProfile p;
    p.name = "PatchRig_Room207";
    p.description = "Axopatch 200B, NI USB-6343";
    p.bindings.push_back({
        /*role*/ "Cell_A",
        /*ai*/ "Dev1/ai0",
        /*ao*/ "Dev1/ao0",
        /*ai_gain*/ 100.0,
        /*ao_gain*/ 1.0,
        /*pos*/ 5.0,
        /*neg*/ -5.0
    });
    p.bindings.push_back({
        "Cell_B",
        "Dev1/ai1",
        "Dev1/ao1",
        100.0,
        1.0,
        5.0,
        -5.0
    });
    return p;
}

static void json_roundtrip() {
    RigProfile p = make_sample_profile();
    auto j = rig_profile_json::to_json(p);
    RigProfile back = rig_profile_json::from_json(j);

    CHECK(back.name == p.name);
    CHECK(back.description == p.description);
    CHECK(back.bindings.size() == p.bindings.size());
    for (std::size_t i = 0; i < p.bindings.size(); ++i) {
        const auto& a = p.bindings[i];
        const auto& b = back.bindings[i];
        CHECK(a.role == b.role);
        CHECK(a.ai_channel == b.ai_channel);
        CHECK(a.ao_channel == b.ao_channel);
        CHECK(a.ai_gain == b.ai_gain);
        CHECK(a.ao_gain == b.ao_gain);
        CHECK(a.pos_limit_nA == b.pos_limit_nA);
        CHECK(a.neg_limit_nA == b.neg_limit_nA);
    }
}

static void file_roundtrip() {
    auto path = test_dir() / "sample.rigprofile.json";
    std::filesystem::remove(path);

    RigProfile p = make_sample_profile();
    std::string err;
    CHECK(rig_profile_json::save(p, path.string(), &err));
    CHECK(err.empty());

    RigProfile loaded;
    CHECK(rig_profile_json::load(path.string(), loaded, &err));
    CHECK(err.empty());
    CHECK(loaded.name == p.name);
    CHECK(loaded.bindings.size() == 2);
    CHECK(loaded.bindings[0].role == "Cell_A");
    CHECK(loaded.bindings[0].ai_channel == "Dev1/ai0");
    CHECK(loaded.bindings[1].role == "Cell_B");
}

static void find_returns_nullopt_for_unknown_role() {
    RigProfile p = make_sample_profile();
    CHECK(p.find("Cell_A").has_value());
    CHECK(p.find("Cell_A")->ai_channel == "Dev1/ai0");
    CHECK(!p.find("NotARole").has_value());
    CHECK(!p.find("").has_value());
}

static void role_names_preserves_order() {
    RigProfile p = make_sample_profile();
    auto names = p.role_names();
    CHECK(names.size() == 2);
    CHECK(names[0] == "Cell_A");
    CHECK(names[1] == "Cell_B");
}

static void manager_active_profile_round_trip() {
    auto &m = RigProfileManager::instance();
    m.set_active(nullptr);
    CHECK(!m.active());
    CHECK(!m.resolve("Cell_A").has_value());

    auto p = std::make_shared<RigProfile>(make_sample_profile());
    m.set_active(p);
    CHECK(m.active().get() == p.get());
    auto resolved = m.resolve("Cell_A");
    CHECK(resolved.has_value());
    CHECK(resolved->ai_channel == "Dev1/ai0");
    CHECK(!m.resolve("Nope").has_value());

    // Clear
    m.set_active(nullptr);
    CHECK(!m.active());
}

static void load_handles_missing_and_invalid_files() {
    std::string err;
    RigProfile dummy;
    CHECK(!rig_profile_json::load("/no/such/path.json", dummy, &err));
    CHECK(!err.empty());

    auto path = test_dir() / "corrupt.rigprofile.json";
    {
        std::ofstream f(path);
        f << "this is not JSON";
    }
    err.clear();
    CHECK(!rig_profile_json::load(path.string(), dummy, &err));
    CHECK(!err.empty());
}

static void empty_profile_is_valid() {
    RigProfile empty;
    empty.name = "empty";
    auto j = rig_profile_json::to_json(empty);
    RigProfile back = rig_profile_json::from_json(j);
    CHECK(back.name == "empty");
    CHECK(back.bindings.empty());
    CHECK(back.role_names().empty());
}

// ---------------------------------------------------------------------------
// End-to-end integration: a network JSON with a "role" plus an active
// rig profile resolves to the profile's channels. Without an active
// profile, the loader falls back to the values stored in the JSON.
// ---------------------------------------------------------------------------

static void register_factories_once() {
    static bool done = false;
    if (done) return;
    done = true;
    try {
        GetCellFactory().registerBuilder(
            TBiologicalCell_KEY,
            TypeID<TBiologicalCell>(),
            TypeID<const std::wstring>());
    } catch (...) {}
}

static std::string make_network_json_with_role() {
    // Minimal network JSON: one TBiologicalCell with role "Cell_A"
    // and also explicit channel/gain values that the profile should
    // override.
    return R"({
        "name": "TestNet",
        "maxRK4Timestep": 0.01,
        "cells": [
            {
                "name": "Bio1",
                "classKey": "Biological Cell",
                "active": true,
                "x": 0, "y": 0,
                "posCurrentLimit": 3.0,
                "negCurrentLimit": -3.0,
                "aiChannel": "OLD/ai9",
                "aoChannel": "OLD/ao9",
                "aiGain": 77.0,
                "aoGain": 7.7,
                "role": "Cell_A",
                "currents": [],
                "electrodes": []
            }
        ],
        "electrodes": [],
        "synapses": []
    })";
}

static void role_override_wins_when_profile_active() {
    register_factories_once();

    // Install an active profile where Cell_A maps to NEW/ai0 etc.
    auto p = std::make_shared<RigProfile>();
    p->name = "ActiveRig";
    p->bindings.push_back({"Cell_A", "NEW/ai0", "NEW/ao0", 10.0, 0.5, 2.0, -2.0});
    RigProfileManager::instance().set_active(p);

    auto j = nlohmann::json::parse(make_network_json_with_role());
    TNetwork *net = NetworkJson::networkFromJson(j);
    CHECK(net != nullptr);

    auto cells = net->GetCells();
    CHECK(cells.size() == 1);
    auto *bc = dynamic_cast<TBiologicalCell*>(cells.begin()->second.get());
    CHECK(bc != nullptr);
    CHECK(bc->Role() == L"Cell_A");
    // Profile wins.
    CHECK(bc->AIChannel() == L"NEW/ai0");
    CHECK(bc->AOChannel() == L"NEW/ao0");
    CHECK(bc->AIGain() == 10.0);
    CHECK(bc->AOGain() == 0.5);
    CHECK(bc->PosCurrentLimit() == 2.0);
    CHECK(bc->NegCurrentLimit() == -2.0);

    delete net;
    RigProfileManager::instance().set_active(nullptr);
}

static void fallback_uses_stored_values_when_no_profile() {
    register_factories_once();
    RigProfileManager::instance().set_active(nullptr);   // no active profile

    auto j = nlohmann::json::parse(make_network_json_with_role());
    TNetwork *net = NetworkJson::networkFromJson(j);
    CHECK(net != nullptr);

    auto *bc = dynamic_cast<TBiologicalCell*>(net->GetCells().begin()->second.get());
    CHECK(bc != nullptr);
    CHECK(bc->Role() == L"Cell_A");   // still remembered
    CHECK(bc->AIChannel() == L"OLD/ai9");
    CHECK(bc->AOChannel() == L"OLD/ao9");
    CHECK(bc->AIGain() == 77.0);
    CHECK(bc->AOGain() == 7.7);
    CHECK(bc->PosCurrentLimit() == 3.0);
    CHECK(bc->NegCurrentLimit() == -3.0);

    delete net;
}

static void fallback_when_role_not_in_active_profile() {
    register_factories_once();

    auto p = std::make_shared<RigProfile>();
    p->name = "OtherRig";
    p->bindings.push_back({"Cell_X", "X/ai0", "X/ao0", 1.0, 1.0, 5.0, -5.0});
    RigProfileManager::instance().set_active(p);

    auto j = nlohmann::json::parse(make_network_json_with_role());
    TNetwork *net = NetworkJson::networkFromJson(j);
    CHECK(net != nullptr);

    auto *bc = dynamic_cast<TBiologicalCell*>(net->GetCells().begin()->second.get());
    CHECK(bc != nullptr);
    CHECK(bc->Role() == L"Cell_A");
    // Unresolved → stored values stand.
    CHECK(bc->AIChannel() == L"OLD/ai9");
    CHECK(bc->AOChannel() == L"OLD/ao9");
    CHECK(bc->AIGain() == 77.0);

    delete net;
    RigProfileManager::instance().set_active(nullptr);
}

// ---------------------------------------------------------------------------
int main() {
    std::printf("NETrex RigProfile tests\n");
    std::printf("=======================\n\n");

    std::error_code ec;
    for (auto& e : std::filesystem::directory_iterator(test_dir(), ec)) {
        std::filesystem::remove(e.path(), ec);
    }

    run("json roundtrip",                     json_roundtrip);
    run("file roundtrip",                     file_roundtrip);
    run("find returns nullopt for unknown",   find_returns_nullopt_for_unknown_role);
    run("role_names preserves order",         role_names_preserves_order);
    run("manager active profile round trip",  manager_active_profile_round_trip);
    run("load handles missing/invalid files", load_handles_missing_and_invalid_files);
    run("empty profile is valid",             empty_profile_is_valid);
    run("role override wins when profile active",
                                               role_override_wins_when_profile_active);
    run("fallback uses stored values when no profile",
                                               fallback_uses_stored_values_when_no_profile);
    run("fallback when role not in active profile",
                                               fallback_when_role_not_in_active_profile);

    std::printf("\n");
    if (g_failures == 0) { std::printf("All tests passed.\n"); return 0; }
    std::printf("%d test(s) FAILED.\n", g_failures);
    return 1;
}
