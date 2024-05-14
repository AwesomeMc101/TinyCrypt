#include <iostream>
#include <string>
#include <vector>

#define key_formula(v,c,n,s) v * c + (3*n) - (s*4)
namespace Key {
    typedef struct KeyData {
        size_t vowels = 0, consonants = 0, numbers = 0, spec_characters = 0;
    };

    KeyData get_data(std::string password) {
        //abc12$
        KeyData k_d;
        for (char c : password) {
            if (isdigit(c)) {
                k_d.numbers++;
            }
            else if (isalpha(c)) {
                switch (
                    c == 'a' || c == 'e' || c == 'u' || c == 'o' || c == 'i' || c == 'y' ||
                    c == 'A' || c == 'E' || c == 'U' || c == 'O' || c == 'I' || c == 'Y'
                    )
                {
                case true:
                    k_d.vowels++;
                    break;
                case false:
                    k_d.consonants++;
                    break;
                }
            }
            else {
                k_d.spec_characters++;
            }
        }
        return k_d;
    }

    float get_key_shift(std::string password) {
        KeyData k_d = get_data(password);
        float val = key_formula(k_d.vowels, k_d.consonants, k_d.numbers, k_d.spec_characters);
        return (val * password.length());
    }
    int to_skip(std::string password) {
        return ((password.length() % 2) + 1);
    }
    bool reverse(std::string password) {
        //first char of password is even
        if (!password.length()) { return false; }
        int c = password[0];
        return (!(c % 2));
    }
    bool addsub(std::string password) { //ADD true, SUB false
        if (!password.length()) { return true; }
        int c = password[password.size() - 1];
        return (!(c % 2));
    }
}

namespace Encrypt {
    std::vector<int> encrypt(std::string str, std::string pass) {
        int key_shift = ceil(Key::get_key_shift(pass));
        float skip = Key::to_skip(pass);
        bool reverse = Key::reverse(pass);
        bool add = Key::addsub(pass);

        if (!add) {
            key_shift *= -1;//genius method
        }

        std::vector<int> final_chars;
        int runs = 0;
        for (int i = 0; i < str.length(); i++) {
            if (runs >= skip) { final_chars.push_back(str[i]); runs = 0; continue; }
            final_chars.push_back((str[i] + key_shift));
            runs++;
        }

        if (reverse) {
            std::reverse(final_chars.begin(), final_chars.end());
        }

        return final_chars;
    }
}

namespace Decrypt {
    std::string decrypt(std::vector<int> data, std::string pass) {
        int key_shift = ceil(Key::get_key_shift(pass));
        float skip = Key::to_skip(pass);
        bool reverse = Key::reverse(pass);
        bool add = Key::addsub(pass);
        if (add) {
            key_shift *= -1; //undo
        }
      
        if (reverse) {
        
            std::reverse(data.begin(), data.end());
        }
       
        std::string final_chars;
        int runs = 0;
        for (int i = 0; i < data.size(); i++) {
            if (runs >= skip) { final_chars.push_back(data[i]); runs = 0; continue; }
            final_chars.push_back((data[i] + key_shift));
            runs++;
        }
        
        return final_chars;
        
    }
}

int main()
{
    std::string key = "1234%^&*abCE";
    std::string to_crypt = "Hello world?";

    std::vector<int> encrypted  = (Encrypt::encrypt(to_crypt, key));

   std::string decrypted = Decrypt::decrypt(encrypted, key);
}
