#pragma once

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

#include <windows.h>
#include <wincrypt.h>
#include <codecvt>
#include <regex>

#include "base64/base64.hpp"
#include "fileIO.hpp"
#include <iomanip>

#include <cstdlib>
#include <ctime>

#include <chrono>

#include <filesystem>

#include <openssl/evp.h>

namespace utils {

    namespace vector {

        void remove_duplicates(std::vector<std::string> &vec) {
            std::sort(vec.begin(), vec.end());
            vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
        }
    }

    namespace string {

        void trim_str(std::string &str) {
            str.erase(std::remove(str.begin(), str.end(), '\0'), str.end());
            str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
            str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
        }

        void match_regex(std::string content, const std::string &pattern, std::vector<std::string> &out) {
            std::regex regexPattern(pattern);
            std::smatch match;

            while (std::regex_search(content, match, regexPattern)) {
                out.push_back(match.str(1));
                content = match.suffix().str();
            }
        }
    }

    namespace conversion {

        void string_to_byte_vector(const std::string &in, std::vector<unsigned char> &out) {
            out = std::vector<unsigned char>(in.begin(), in.end());
        }

        void bytes_array_to_string(const unsigned char *in, size_t len, std::string &out) {
            out = std::string(reinterpret_cast<const char *>(in), len);
        }

        void string_to_hexstring(const std::string &in, std::string &out) {
            std::stringstream str_stream;
            for (const auto &item : in) {
                str_stream << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << +static_cast<uint8_t>(item);
            }
            out =  str_stream.str();
        }

        void utf8_to_wstring(const std::string &in, std::wstring &out) {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            out = converter.from_bytes(in);
        }

        void wstring_to_utf8(const std::wstring &in, std::string &out) {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            out = converter.to_bytes(in);
        }

        void utf8_encode(std::string &str) {
            int wideCharSize = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
            if (wideCharSize == 0) {
                throw std::runtime_error("MultiByteToWideChar failed");
            }

            std::wstring wideStr(wideCharSize, L'\0');
            if (MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wideStr[0], wideCharSize) == 0) {
                throw std::runtime_error("MultiByteToWideChar failed");
            }

            int utf8Size = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
            if (utf8Size == 0) {
                throw std::runtime_error("WideCharToMultiByte failed");
            }

            std::string utf8Str(utf8Size, '\0');
            if (WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &utf8Str[0], utf8Size, nullptr, nullptr) == 0) {
                throw std::runtime_error("WideCharToMultiByte failed");
            }

            utf8Str.pop_back(); // Remove the null terminator added by WideCharToMultiByte

            str = utf8Str;
        }
    }

    namespace crypt {

        int aes_dec_gcm(unsigned char *ciphertext, int ciphertext_len, unsigned char *tag,
            unsigned char *key, unsigned char *iv, int iv_len,
            unsigned char *plaintext) {
            EVP_CIPHER_CTX *ctx;
            int len;
            int plaintext_len;
            int ret;

            ctx = EVP_CIPHER_CTX_new();

            EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL);

            EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv_len, NULL);

            EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv);

            EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);

            plaintext_len = len;

            EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, tag);

            ret = EVP_DecryptFinal_ex(ctx, plaintext + len, &len);

            EVP_CIPHER_CTX_free(ctx);

            if (ret > 0) {
                plaintext_len += len;
                return plaintext_len;
            }
            else {
                return -1;
            }
        }
    }
}