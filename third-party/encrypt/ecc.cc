#include <iostream>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>
#include <openssl/rand.h>
#include <openssl/err.h>

// 生成椭圆曲线密钥对
EC_KEY* generate_key_pair()
{
    EC_KEY *key_pair = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
    if (key_pair == nullptr) {
        std::cerr << "Failed to create EC key pair" << std::endl;
        return nullptr;
    }

    if (EC_KEY_generate_key(key_pair) != 1) {
        std::cerr << "Failed to generate EC key pair" << std::endl;
        EC_KEY_free(key_pair);
        return nullptr;
    }

    return key_pair;
}

// 加密
std::string encrypt(const std::string& plaintext, EC_KEY* public_key)
{
    std::string ciphertext;

    // 获取椭圆曲线的参数
    const EC_GROUP* group = EC_KEY_get0_group(public_key);
    const EC_POINT* public_key_point = EC_KEY_get0_public_key(public_key);

    // 创建临时密钥对
    EC_KEY* ephemeral_key_pair = generate_key_pair();
    if (ephemeral_key_pair == nullptr) {
        return ciphertext;
    }

    // 获取临时公钥
    const EC_POINT* ephemeral_public_key_point = EC_KEY_get0_public_key(ephemeral_key_pair);

    // 创建共享密钥
    unsigned char shared_secret[32];
    int shared_secret_len = ECDH_compute_key(shared_secret, sizeof(shared_secret), ephemeral_public_key_point, public_key, nullptr);
    if (shared_secret_len <= 0) {
        std::cerr << "Failed to compute shared secret" << std::endl;
        EC_KEY_free(ephemeral_key_pair);
        return ciphertext;
    }

    // 使用共享密钥进行对称加密
    // 这里使用AES加密算法作为示例，你可以根据需要选择其他加密算法
    // ...

    // 清理临时密钥对
    EC_KEY_free(ephemeral_key_pair);

    return ciphertext;
}

// 解密
std::string decrypt(const std::string& ciphertext, EC_KEY* private_key)
{
    std::string plaintext;

    // 获取椭圆曲线的参数
    const EC_GROUP* group = EC_KEY_get0_group(private_key);

    // 创建共享密钥
    unsigned char shared_secret[32];
    int shared_secret_len = ECDH_compute_key(shared_secret, sizeof(shared_secret), EC_KEY_get0_public_key(private_key), private_key, nullptr);
    if (shared_secret_len <= 0) {
        std::cerr << "Failed to compute shared secret" << std::endl;
        return plaintext;
    }

    // 使用共享密钥进行对称解密
    // 这里使用AES解密算法作为示例，你可以根据加密时选择的算法进行解密
    // ...

    return plaintext;
}

int main()
{
    // 生成椭圆曲线密钥对
    EC_KEY* key_pair = generate_key_pair();
    if (key_pair == nullptr) {
        return 1;
    }

    // 加密示例
    std::string plaintext = "Hello, World!";
    std::string ciphertext = encrypt(plaintext, key_pair);
    std::cout << "Ciphertext: " << ciphertext << std::endl;

    // 解密示例
    std::string decrypted_text = decrypt(ciphertext, key_pair);
    std::cout << "Decrypted Text: " << decrypted_text << std::endl;

    // 清理密钥对
    EC_KEY_free(key_pair);

    return 0;
}