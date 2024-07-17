# 🟩 Discord Token Retrieval 🟩 <a href="https://github.com/yurtrimu/discord-token-retrieval/actions/workflows/main.yml"><img src="https://github.com/yurtrimu/discord-token-retrieval/actions/workflows/main.yml/badge.svg" alt="Working"></a>

**Discord Token Retrieval project is for retrieving decrypted discord tokens.**

## Usage

```c++
#include "utils/utils.hpp"
#include "DiscordToken.hpp"

int main()
{
    std::vector<std::string> token_list;
    DiscordToken::Collect(token_list);

    for (const std::string &token : token_list) {

        std::cout << "--------------------\n";
        std::cout << "Token: " << token << "\n";
        std::cout << "\n--------------------\n\n";
    }
}
```

## Example.png
![alt text](https://github.com/yurtrimu/discord-token-retrieval/blob/main/Example.png?raw=true)
These tokens are fake, don't waste your time.

## Operating System
- **Below Windows 7 - 🟦 Untested**
- **Windows 7 - 🟦 Untested**
- **Windows 10 - 🟩 Working**
- **Windows 11 - 🟦 Untested**
- **Mac OS - 🟥 Not Working**
- **Linux - 🟥 Not Working**

## Requirements

**You need to have the OpenSSL library installed and linked to your compiler.**

- You could download the OpenSSL library from [OpenSSL website](https://www.openssl.org/source/).

## Linker

- **libssl.lib**
- **libcrypto.lib**
- **crypt32.lib**

## Compiling

- **Build - 🟦 Untested**
- **Release - 🟩 Working**
- **C++14 and Below - 🟥 Not Working**
- **C++17 - 🟩 Working**
- **C++20 - 🟩 Working**

## Contributing

**Pull requests are welcome.**

## Legal Disclaimer
🟥 **The content provided is for educational and informational purposes only.** 🟥
