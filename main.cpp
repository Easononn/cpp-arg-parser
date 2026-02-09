#include <iostream>
#include <string>
#include <vector>
#include <map>

// 1. 定义一个结构体：这代表一条“规则”
struct Option {
    std::string shortName; // 短选项: -n
    std::string longName;  // 长选项: --name
    bool needValue;        // 是否需要值? (true=要值, false=只是个开关)
    std::string desc;      // 帮助描述
};

class ArgParser {
private:
    //Key总是存长选项，方便统一查询
    std::map<std::string, std::string> params; 
    
    //记录了哪些选项是合法的
    std::vector<Option> options;

public:
    // 注册选项
    void addOption(std::string shortName, std::string longName, bool needValue, std::string desc) {
        Option opt;
        opt.shortName = shortName;
        opt.longName = longName;
        opt.needValue = needValue;
        opt.desc = desc;
        options.push_back(opt); // 把规则存进列表
    }

    void parse(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
            std::string currentArg = argv[i];
            bool matched = false; // 标记是否在菜单里找到了这个参数

            // 遍历options，看看用户输入的是不是有效选项
            for (int k = 0; k < options.size(); k++) {
                Option opt = options[k];

                // 核心逻辑：不管用户输入 -n 还是 --name，我们要能认出来
                if (currentArg == opt.shortName || currentArg == opt.longName) {
                    matched = true; // 找到了！

                    if (opt.needValue) {
                        // 如果这个选项需要值 (比如 -n Alice)
                        if (i + 1 < argc) {
                            // 重点：我们统一用 longName (--name) 作为 Key 存进背包
                            // 这样无论用户输 -n 还是 --name，我们在背包里都只查 --name
                            params[opt.longName] = argv[i + 1]; 
                            i++; // 跳过参数值
                        } else {
                            std::cout << "错误: " << currentArg << " 后面缺少参数值！" << std::endl;
                        }
                    } else {
                        // 如果只是个开关 (比如 --help)，存个 "true" 就行
                        params[opt.longName] = "true";
                    }
                    break; // 匹配到了就不用继续查菜单了
                }
            }

            if (!matched) {
                // 如果菜单里没有这个选项
                std::cout << "警告: 未知的参数 " << currentArg << std::endl;
            }
        }
    }

    // 获取参数值 (统一用长选项查)
    std::string getString(std::string key) {
        if (params.count(key) > 0) {
            return params[key];
        }
        return "";
    }

    //检查开关是否存在 (专门用于处理 --help 这种)
    bool hasFlag(std::string key) {
        return params.count(key) > 0;
    }

    // 自动生成帮助文档
    void printHelp() {
        std::cout << "Available Options:" << std::endl;
        for (int k = 0; k < options.size(); k++) {
            std::cout << "  " << options[k].shortName << ", " 
                      << options[k].longName << "\t" 
                      << options[k].desc << std::endl;
        }
    }
};

int main(int argc, char* argv[]) {
    ArgParser parser;

    // 1. 【注册阶段】先定义好规则
    //    短名   长名       需要值?   描述
    parser.addOption("-n", "--name",  true,   "Specify user name");
    parser.addOption("-o", "--output", true,   "Specify output file");
    parser.addOption("-h", "--help",   false,  "Show help message");
    parser.addOption("-v", "--verbose",false,  "Enable verbose mode");

    // 2. 【解析阶段】
    parser.parse(argc, argv);

    // 3. 【业务逻辑】
    if (parser.hasFlag("--help")) {
        parser.printHelp(); // 自动打印帮助！
        return 0;
    }

    // 统一用长选项取值，哪怕用户输入的是 -n
    std::string name = parser.getString("--name");
    std::string output = parser.getString("--output");

    if (name != "") {
        std::cout << "User: " << name << std::endl;
    }
    
    if (output != "") {
        std::cout << "Output File: " << output << std::endl;
    }

    if (parser.hasFlag("--verbose")) {
        std::cout << "[DEBUG] Verbose mode is ON." << std::endl;
    }

    return 0;
}