#include <utility>

//
// Created by keven on 13.03.2019.
//

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>

namespace utils {

    [[nodiscard]] static inline bool contains(const std::string& container,const char* sub){
        return container.find(sub) != std::string::npos;
    }

    [[nodiscard]] static inline std::string toLowerCase(std::string trans) {
        std::transform(std::begin(trans), std::end(trans), std::begin(trans), tolower);
        return trans;
    }

    [[nodiscard]] static inline std::string toHigherCase(std::string trans) {
        std::transform(std::begin(trans), std::end(trans), std::begin(trans), toupper);
        return trans;
    }

    [[nodiscard]] static inline std::string toCamelCase(std::string trans) {
        trans = toLowerCase(trans);
        std::transform(std::begin(trans), ++std::begin(trans), std::begin(trans), toupper);
        return trans;
    }
}

namespace details {

    struct generatable {

        virtual std::string generate() noexcept = 0;
        virtual void style(std::string& val) noexcept {
            val += '\n';
        }
        virtual ~generatable() = default;
    };

    enum special {
        Data, Flag, Toggle, Control
    };

    class Reg : public generatable {
        static inline constexpr auto reg_str = "utils::Pair<reg::Register<";
        static inline constexpr auto R_str = "reg::accessmode::ReadOnly";
        static inline constexpr auto RW_str = "reg::accessmode::RW";
        static inline constexpr auto data_str = "reg::specialization::Data";
        static inline constexpr auto toggle_str = "reg::specialization::Toggle";
        static inline constexpr auto flag_str = "reg::specialization::Flag";
        static inline constexpr auto ctrl_str = "reg::specialization::Control";
        std::string name, protection, offset, values, specStr;
        int size;
        special spec;

        [[nodiscard]] std::string resolveString(special spec) {
            switch (spec) {
                case Data :
                    return data_str;
                case Flag :
                    return flag_str;
                case Toggle :
                    return toggle_str;
                case Control :
                    return ctrl_str;
                default:
                    return "";
            }
        }

        [[nodiscard]] std::string resolveString(int size) noexcept {
            switch (size) {
                case 1 :
                    return "uint8_t";
                case 2 :
                    return "uint16_t";
                case 4 :
                    return "uint32_t";
                case 8 :
                    return "uint64_t";
                default:
                    return "";
            }
        }

    public:
        Reg(std::string &&name, std::string &&protection, std::string &&offset, std::string &&size,
            std::string &&values, special spec) noexcept :
                name(name), protection((protection == "R" ? R_str : RW_str)), offset(offset),
                values((values.empty() ? "void" : values)), specStr(resolveString(spec)), spec(spec) {
            try {
                this->size = std::stoi(size);
            } catch (std::exception &e) {
                std::cerr << __PRETTY_FUNCTION__ << " exception: " << e.what() << '\n';
                size = 1;
            }
        }

        void style(std::string& val) noexcept override {

        }

        [[nodiscard]] std::string generate() noexcept override {
            std::string tmp = "using " + utils::toLowerCase(name) + " = " + reg_str + protection + "," + specStr;
            std::string size_str = resolveString(size);
            if (spec == Data || spec == Toggle) {
                if (size == 1) {
                    tmp += ">";
                } else {
                    tmp += ",void," + size_str + ">";
                }
            } else {
                if (size == 1) {
                    tmp += "," + values + ">";
                } else {
                    tmp += "," + values + "," + size_str + ">";
                }
            }
            tmp += "," + offset + ">;";
            return tmp;
        }

    };

    class Function : public generatable {
        std::string attributes, returnType, name, params, cvQualifier, content;
    public:
        Function(std::string &&attributes, std::string &&returnType, std::string &&name,
                 std::string &&params, std::string &&cvQualifier, std::string &&content) : attributes(
                std::move(attributes)),
                                                                                           returnType(returnType),
                                                                                           name(name),
                                                                                           params(params),
                                                                                           cvQualifier(cvQualifier),
                                                                                           content(content) {}

        void changeContent(const std::string &content) {
            this->content = content;
        }

        std::string generate() noexcept override {
            return attributes + " " + returnType + " " + name + "(" + params + ") " + cvQualifier + " { " + content +
                   "}";
        }

    };

    class Enum : public generatable {
        std::string name, type;
        std::string entrys{};

    public:
        Enum(std::string &&name, std::string &&type) noexcept : name(name), type(type) {}

        void addEntry(const std::string &name, const std::string &value) noexcept {

            if (!entrys.empty())
                entrys += ",\n";
            entrys += "    " + name;
            entrys += " = " + value;
        }

        [[nodiscard]] std::string generate() noexcept override {
            std::string tmp = "enum class " + name + " : " + type + " {\n";
            tmp += entrys;
            tmp += "\n};";
            return tmp;
        }
    };

    class Struct : public generatable {
        std::string mname;
        std::string mentrys{};

        void addEntry(const std::string &name, const std::string &value) noexcept {

            if (!mentrys.empty())
                mentrys += "\n";
            mentrys += "    " + name;
            mentrys += " = " + value + ";";
        }

    public:
        explicit Struct(std::string &&name) noexcept : mname(name) {}

        void addStatic(std::string &&type, std::string &&name, std::string &&value, bool constex = false) {
            std::string prefix;
            if (!constex)
                prefix = "static inline ";
            else {
                prefix = "static constexpr ";
            }
            addEntry((prefix + type + " " + name), value);
        }

        void addMember(generatable &gen) noexcept {
            if (!mentrys.empty())
                mentrys += "\n";
            auto tmp = "    " + gen.generate();

            for (uint32_t i = 0; i < tmp.size(); i++) {
                if (tmp[i] == '\n')
                    tmp.insert(i + 1, "    ");
            }
            gen.style(tmp);
            mentrys += tmp;
        }

        [[nodiscard]] std::string generate() noexcept override {
            std::string tmp = "struct " + mname + " {\n";
            tmp += mentrys;
            tmp += "\n};";
            return tmp;
        }

        [[nodiscard]] inline std::string &name() noexcept { return mname; }

        [[nodiscard]] inline std::string &entrys() noexcept { return mentrys; }
    };

    class Namespace : public Struct {

    public:
        Namespace(std::string &&name) : Struct(std::move(name)) {}

        [[nodiscard]] std::string generate() noexcept override {
            std::string tmp = "namespace " + name() + " {\n";
            tmp += entrys();
            tmp += "\n}";
            return tmp;
        }
    };
}


class MCUStructureBuilder {
    static inline constexpr auto header = "#pragma once\n #include \"../../hw_abstractions/Port.hpp\"\n #include \"../../hw_abstractions/Pin.hpp\"\n\n";
    static inline auto f = details::Function("[[nodiscard,gnu::always_inline]] static inline","auto&","value","","","" );
    std::string compname;
    std::string deviceName;
    details::Struct compStruct, regs{"registers"};
    details::Namespace nameSpace;
    std::vector<details::Enum> enums;
    std::vector<details::Struct> instances;

public:
    using reg_type = details::special;

    explicit MCUStructureBuilder(std::string &&deviceName, std::string &&ComponentName)
    noexcept : deviceName(deviceName), compname(ComponentName), compStruct(utils::toLowerCase(compname) + "Component"),
               nameSpace(deviceName.substr(2)) {}

    void addRegister(std::string &&name, std::string &&protection, std::string &&offset, std::string &&size,
                     std::string &&values, reg_type type) noexcept {
        auto tmp = details::Reg(std::move(name), std::move(protection), std::move(offset), std::move(size),
                                std::move(values), type);
        regs.addMember(tmp);
    }

    void addInstance(std::string &&name) {
        f.changeContent("return "+name+";");
        auto tmp = details::Struct(std::move(utils::toLowerCase(name)));
        tmp.addMember(f);
        instances.push_back(tmp);
    }

    void addEnum(std::string &&name, std::string &&type = "mem_width") noexcept {
        auto tmp = details::Enum(std::move(name), std::move(type));
        enums.push_back(tmp);
    }

    void addEnumEntry(std::string &&name, std::string &&value) {
        enums.back().addEntry(name, value);
    }

    void parse() noexcept {
        std::string tmp = header;
        for (auto &elem: enums) {
            compStruct.addMember(elem);
        }
        compStruct.addMember(regs);
        nameSpace.addMember(compStruct);
        auto ins = details::Struct(utils::toLowerCase(compname)+"s");
        for (auto &elem : instances) {
            ins.addMember(elem);
        }
        nameSpace.addMember(ins);
        tmp += nameSpace.generate();

        std::string outName = utils::toHigherCase(deviceName) + compname + ".hpp";
        try {
            std::ofstream of(outName);
            of << tmp;
        } catch (std::exception &e) {
            std::cerr << __FILE__ << " " << __LINE__ << " could not parse: " << e.what();
        }
    }
};
