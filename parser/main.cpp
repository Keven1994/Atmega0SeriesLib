#include <iostream>
#include <string>
#include <ostream>
#include <vector>
#include <fstream>
#include <bitset>
#include "pugixml.hpp"
#include "MCUStructureBuilder.h"


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
    using reg_type = typename MCUStructureBuilder::reg_type;

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
            std::string reg_name = node1.attribute("name").as_string();
            std::string reg_caption = node1.attribute("caption").as_string();
            std::string reg_off = node1.attribute("offset").as_string();
            std::string reg_prot = node1.attribute("rw").as_string();
            std::string reg_size = node1.attribute("size").as_string();
            std::string reg_enum = reg_name + "Masks";
            reg_type reg_t;
            if (utils::contains(reg_name, "TGL") || utils::contains(reg_caption, "Toggle")) {
                reg_t = reg_type::Toggle;
            } else if (utils::contains(reg_name, "CTRL") || utils::contains(reg_caption, "Select") ||
                       utils::contains(reg_caption, "Control")) {
                reg_t = reg_type::Control;
            } else if (utils::contains(reg_name, "FLAGS") || utils::contains(reg_caption, "Status")) {
                reg_t = reg_type::Flag;
            } else {
                reg_t = reg_type::Data;
            }
            std::vector<tuple<>> val_group{};
            bool entryGenerated = false;

            auto it = node1.children("mode");
            bool hasmode = it.begin() != it.end();
            auto iterator = it.begin();
            auto it2 = node1.children("bitfield");
            do {
                if(hasmode) it2 = iterator->children("bitfield");

                for (auto node2 :  it2) {
                    std::string mname;
                    if (hasmode) {
                        mname = (*iterator).attribute("name").as_string() + std::string("_");
                    }

                    if (reg_t == reg_type::Data) {
                        if (reg_prot == "RW") {
                            reg_t = reg_type::Control;
                        } else {
                            reg_t = reg_type::Flag;
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
                                        utils::toCamelCase(mname + node2.attribute("name").as_string()) +
                                        std::to_string(i),
                                        modName + "_" + node2.attribute("name").as_string() + std::to_string(i) +
                                        "_bm");
                            }
                        } else
                            mbuilder.addEnumEntry(utils::toCamelCase(mname + node2.attribute("name").as_string()),
                                                  modName + "_" + node2.attribute("name").as_string() + "_bm");
                    } else {
                        val_group.push_back(tuple<>{tempstr, mname + node2.attribute("name").as_string()});
                    }

                }
                if(hasmode)
                    iterator++;
            } while(iterator != it.end());

            for (auto &valstr : val_group) {
                std::string enumname = modName;
                enumname.append("_").append(valstr.str2).append("_enum").append("::").append(
                        modName + "_" + valstr.str2 + "_");
                for (auto node2 : tool.children("value-group")) {
                    if (node2.attribute("name").as_string() == valstr.str1) {
                        for (auto node3 : node2.children("value")) {
                            std::string valname = enumname + node3.attribute("name").as_string() + "_gc";
                            mbuilder.addEnumEntry(
                                    utils::toLowerCase(valstr.str2 + "_" + node3.attribute("name").as_string()),
                                    std::move(valname));
                        }
                    }
                }
            }
            mbuilder.addRegister(std::move(reg_name), std::move(reg_prot), std::move(reg_off), std::move(reg_size),
                                 std::move(reg_enum), reg_t);
        }

        for (auto node1 : peripherals) {
            if (node1.node().attribute("name").as_string() == modName) {
                for (auto node2 : node1.node().children()) {
                    std::string instName = node2.attribute("name").as_string();
                    if (modName != "PORT" || utils::contains(ports_available, std::string() + instName[4])) {
                        mbuilder.addInstance(instName + "");

                        std::vector<utils::triple<>> tmp;
                        for (auto node3 :node2.child("signals").children()) {

                            std::string sig_func = node3.attribute("function").as_string();
                            std::string sig_group = node3.attribute("group").as_string();
                            std::string sig_pad = node3.attribute("pad").as_string();
                            instName = node2.attribute("name").as_string();

                            if (modName != "PORT" || utils::contains(pins_available, (sig_pad)))
                                tmp.push_back(utils::triple<>{sig_func, sig_group, sig_pad});

                        }
                        if (!tmp.empty())
                            mbuilder.addSignal(tmp, std::move(instName));
                    }
                }

            }
        }

        mbuilder.parse(path);
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}