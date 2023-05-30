#include <cpr/cpr.h>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    auto res = cpr::Get(                           // GET请求    
            cpr::Url{"http://openresty.org"}
    );
    std::cout << res.elapsed << std::endl;

    std::cout << res.url << std::endl; 
    std::cout << res.status_code << std::endl; 
    std::cout << res.text.length() << std::endl;

    for(auto& x : res.header) {
        std::cout << x.first << "=>" 
            << x.second << std::endl;
    }    
}
