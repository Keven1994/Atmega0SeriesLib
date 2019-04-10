#include <iostream>
#include <string>
#include <ostream>
#include <vector>
#include <fstream>
#include <bitset>
#include "pugixml.hpp"
#include "MCUStructureBuilder.h"

using reg_type = typename MCUStructureBuilder::reg_type;
void processRegisterGroup(const pugi::xml_node &tool, const std::string &modName, MCUStructureBuilder &mbuilder,
                          const pugi::xml_node &node1);

void processRegisters(const std::string &modName, MCUStructureBuilder &mbuilder, const pugi::xml_node &node1,
                      const std::string &reg_prot, const std::string &reg_enum, reg_type &reg_t,
                      std::vector<utils::tuple<>> &val_group);

void processRegisterValueGroups(const pugi::xml_node &tool, const std::string &modName, MCUStructureBuilder &mbuilder,
                                 std::vector<utils::tuple<>> &val_group);

void processSignals(const std::vector<std::string> &ports_available, const std::vector<std::string> &pins_available,
                    const std::string &modName, MCUStructureBuilder &mbuilder, const pugi::xpath_node &node1);

using namespace utils;

auto avPins(pugi::xml_node &pinouts, const std::vector<std::string> &ports) {
    std::vector<std::string> ret{};
    for (auto elem : pinouts.children()) {
        if (utils::contains(elem.attribute("name").as_string(), "QFN")) {
            for (auto pin : elem.children()) {
                auto name = pin.attribute("pad").as_string();
                auto pname = name[1] + std::string();
                if (name[0] == 'P') {
                    for (auto &str : ports) {

                        if (str == pname) {

                            ret.emplace_back(name);
                        }
                    }
                }
            }
            return ret;
        }
    }
    return ret;
}

std::vector<std::string> avPorts(pugi::xml_node &pinouts) {
    std::vector<std::string> ret{};
    std::vector<std::string> processed{};

    for (auto elem : pinouts.children()) {

        if (utils::contains(elem.attribute("name").as_string(), "QFN")) {

            for (auto pin : elem.children()) {
                auto name = pin.attribute("pad").as_string();
                auto pname = name[1] + std::string();

                if (name[0] == 'P' && !(utils::contains(processed, pname))) {
                    ret.push_back(pname);
                    processed.push_back(pname);
                }
            }
            return ret;
        }
    }
    return ret;
}


int main(int argc, const char **argv) {

    pugi::xml_document doc;

    pugi::xml_parse_result result;
    if (argc > 1)
        result = doc.load_file(argv[1]);
    else result = doc.load_file("../ATmega4808.atdf");

    if (!result)
        return -1;

    std::string devname = doc.select_nodes("avr-tools-device-file/devices/device").begin()->node().attribute(
            "name").as_string();
    auto module = doc.select_nodes("avr-tools-device-file/modules/module");

    auto pinout = doc.select_nodes("avr-tools-device-file/pinouts").begin()->node();
    auto ports_available = avPorts(pinout);
    auto pins_available = avPins(pinout, ports_available);

    auto reg = doc.select_nodes("avr-tools-device-file/modules/module/register-group/register");
    auto peripherals = doc.select_nodes("avr-tools-device-file/devices/device/peripherals/module");
    std::string path;
#ifdef __linux__
    if(argc > 2) path = argv[2]+std::string("/");
#elif _WIN32
    if (argc > 2) path = argv[2] + std::string("\\");
#else
        static_assert(false,"OS not supported");
#endif

    for (auto node: module) {
        pugi::xml_node tool = node.node();
        std::string modName = tool.attribute("name").as_string();
        MCUStructureBuilder mbuilder = MCUStructureBuilder(devname+"", tool.attribute("name").as_string());
        for (auto node1: tool.child("register-group").children("register")) {
            processRegisterGroup(tool, modName, mbuilder, node1);

        }

        for (auto node1 : peripherals) {
            processSignals(ports_available, pins_available, modName, mbuilder, node1);
        }

        mbuilder.parse(path);
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}

void processSignals(const std::vector<std::string> &ports_available, const std::vector<std::string> &pins_available,
                    const std::string &modName, MCUStructureBuilder &mbuilder, const pugi::xpath_node &node1) {
    if (node1.node().attribute("name").as_string() == modName) {
        int instNum = 0;
        for (auto node2 : node1.node().children()) {
            std::string instName = node2.attribute("name").as_string();
            if (modName != "PORT" || contains(ports_available, std::string() + instName[4])) {

                std::vector<triple<>> tmp;
                for (auto node3 :node2.child("signals").children()) {
                    bool grpValid = true;
                    const std::string sig_func = node3.attribute("function").as_string();
                    std::string sig_group = node3.attribute("group").as_string();
                    std::string sig_pad = node3.attribute("pad").as_string();

                    instName = node2.attribute("name").as_string();

                    if (contains(pins_available, (sig_pad)) && grpValid) {
                        if (modName == "USART")
                        tmp.push_back(triple<>{sig_func, sig_group, sig_pad});
                    }

                }
                if (!tmp.empty()) {
                    mbuilder.addSignal(tmp, std::move(instName), instNum++);
                }
            }
        }

    }
}

void processRegisterGroup(const pugi::xml_node &tool, const std::string &modName, MCUStructureBuilder &mbuilder,
                          const pugi::xml_node &node1) {
    std::string reg_name = node1.attribute("name").as_string();
    std::string reg_caption = node1.attribute("caption").as_string();
    std::string reg_off = node1.attribute("offset").as_string();
    std::string reg_prot = node1.attribute("rw").as_string();
    std::string reg_size = node1.attribute("size").as_string();
    std::string reg_enum = reg_name + "Masks";
    reg_type reg_t;
    bool hasBitfields = node1.children("bitfield").begin() != node1.children("bitfield").end();
    if (contains(reg_name, "TGL") || contains(reg_caption, "Toggle")) {
        reg_t = details::Toggle;
    } else if ((contains(reg_name, "CTRL") || contains(reg_caption, "Select") ||
                contains(reg_caption, "Control")) && hasBitfields) {
        reg_t = details::Control;
    } else if ((contains(reg_name, "FLAGS") || contains(reg_caption, "Status")) && hasBitfields) {
        reg_t = details::Flag;
    } else {
        reg_t = details::Data;
    }
    std::vector<tuple<>> val_group;
    processRegisters(modName, mbuilder, node1, reg_prot, reg_enum, reg_t, val_group);

    processRegisterValueGroups(tool, modName, mbuilder, val_group);
    mbuilder.addRegister(std::move(reg_name), std::move(reg_prot), std::move(reg_off), std::move(reg_size),
                         std::move(reg_enum), reg_t);
}

void processRegisterValueGroups(const pugi::xml_node &tool, const std::string &modName, MCUStructureBuilder &mbuilder,
                                std::vector<utils::tuple<>> &val_group) {
    for (auto &valstr : val_group) {
        std::string enumname = modName;

        auto f = valstr.str2.find("DEFAULT_");
        if (f != std::string::npos) {
            valstr.str2 = valstr.str2.substr(0, f) + valstr.str2.substr(f + 8, valstr.str2.size());

        }
        enumname.append("_").append(valstr.str2).append("_enum").append("::").append(
                modName + "_" + valstr.str2 + "_");
        for (auto node2 : tool.children("value-group")) {

            if (node2.attribute("name").as_string() == valstr.str1) {
                for (auto node3 : node2.children("value")) {

                    std::string noDef = node3.attribute("name").as_string();
                    std::string valname = enumname + noDef + "_gc";

                    mbuilder.addEnumEntry(
                            toCamelCase(valstr.str2 + "_" + noDef),
                            std::move(valname));
                }
            }
        }
    }
}

void processRegisters(const std::string &modName, MCUStructureBuilder &mbuilder, const pugi::xml_node &node1,
                      const std::string &reg_prot, const std::string &reg_enum, reg_type &reg_t,
                      std::vector<tuple<>> &val_group) {
    bool entryGenerated = false;

    auto it = node1.children("mode");
    bool hasmode = it.begin() != it.end();
    auto iterator = it.begin();
    auto it2 = node1.children("bitfield");

    do {
        if (hasmode) it2 = iterator->children("bitfield");

        for (auto node2 :  it2) {
            std::string noDef = node2.attribute("name").as_string();
            auto f = noDef.find("DEFAULT_");
            if (f != std::string::npos) {
                noDef = noDef.substr(0, f) + noDef.substr(f + 8, noDef.size());

            }
            if (reg_t == details::Data) {
                if (reg_prot == "RW") {
                    reg_t = details::Control;
                } else {
                    reg_t = details::Flag;
                }
            }
            auto tempstr = std::string(node2.attribute("values").as_string());

            if (!entryGenerated) {
                mbuilder.addEnum(reg_enum + "");
                entryGenerated = true;
            }
            if (tempstr.empty()) {

                auto bs = std::bitset<32>(static_cast<size_t>(node2.attribute("mask").as_int()));
                if (bs.count() > 1) {
                    for (uint32_t i = 0; bs.test(i) && i < 32; i++) {
                        mbuilder.addEnumEntry(
                                toCamelCase(noDef) +
                                std::to_string(i),
                                modName + "_" + noDef + std::to_string(i) +
                                "_bm");
                    }
                } else {
                    mbuilder.addEnumEntry(toCamelCase(noDef),
                                          modName + "_" + noDef + "_bm");
                }
            } else {
                val_group.emplace_back(tempstr, noDef);
            }

        }
        if (hasmode)
            iterator++;
    } while (iterator != it.end());
}