#include <iostream>
#include <mutex>
#include <memory>

static std::mutex mutex;

class Image {
public:
    Image() { }
    ~Image() { }
};

class PrettyMenu {
public:
    void changeBackfground(void);
private:
    Image* bgImage = new Image;
    std::shared_ptr<Image> sharedImage = std::make_shared<Image>();
};

void PrettyMenu::changeBackfground(void)
{
    std::lock_guard<std::mutex> lg(mutex);

    delete bgImage;
    bgImage = new Image;  /* new Image 可能会失败，而我们已经 delete 了老的资源 */

    /* shared_ptr::reset 执行成功的后，才会在它的内部执行 delete 来删除旧指针。 */
    sharedImage.reset(new Image);
}

int main(void)
{
    PrettyMenu p;
    p.changeBackfground();

    return 0;
}

