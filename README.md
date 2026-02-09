# Simple C++ Argument Parser

## 项目简介 (Introduction)
这是一个轻量级的 C++ 命令行参数解析器（Command Line Argument Parser）。
该项目旨在帮助 C++ 初学者理解如何处理 `argc` 和 `argv`，并实现了一个类似 Linux 标准工具的参数处理机制。

它支持：
- **短选项** (Short options): 如 `-n`
- **长选项** (Long options): 如 `--name`
- **参数值获取**: 如 `-o output.txt`
- **布尔开关** (Flags): 如 `--help`
- **自动生成帮助文档**

##  技术实现 (Technical Implementation)
本项目采用 **面向对象 (OOP)** 设计，将解析逻辑封装在 `ArgParser` 类中。

### 核心数据结构
1.  **`std::vector<Option>`**: 用于存储所有注册的参数规则（“菜单”）。
    - 每个 `Option` 结构体包含：短选项、长选项、是否需要值、描述信息。
2.  **`std::map<std::string, std::string>`**: 用于存储解析后的结果（“背包”）。
    - **归一化处理**: 无论用户输入的是短选项（`-n`）还是长选项（`--name`），在 map 中统一使用长选项（`--name`）作为 Key。这大大简化了后续的数据查询。

### 模块化设计
- `ArgParser.h`: 类的声明（接口）。
- `ArgParser.cpp`: 类的具体实现。
- `main.cpp`: 业务逻辑调用示例。

## 如何构建与运行 (Build & Run)

### 依赖环境
- Linux / macOS / Windows (WSL)
- g++ 编译器

### 编译方法
如果你安装了 `make`，直接运行：
```bash
make