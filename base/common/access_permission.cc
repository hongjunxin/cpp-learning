#include <iostream>

class Base {
public:
    void base_pub_fn() {};
    int base_pub_mem;
protected:
    void base_prot_fn() {};
    int base_prot_mem;
private:
    void base_priv_fn() {};
    int base_priv_mem;
};


/* publibc 继承测试 */
class DrivedPublicInherited : public Base {
    friend void clobber(DrivedPublicInherited&);
    friend void clobber(Base&);
public:
    void accessBaseDataTest();
protected:
    int prot_mem_PublicInherited;
private:
    int priv_mem_PublicInherited;
};

/* public 继承，子类只能访问基类中的 protected/public 成员 */
void DrivedPublicInherited::accessBaseDataTest()
{
    //base_priv_mem = 0;  /* 错误！不能访问 */
    base_prot_mem = 0;
    base_pub_mem = 0;

    //base_priv_fn();  /* 错误！不能访问 */
    base_prot_fn();
    base_pub_fn();    
}

/* 类对象中所有访问等级的成员，对于它的友元函数来说都是可以访问的 */
void clobber(DrivedPublicInherited &s) { 
    s.priv_mem_PublicInherited = 0;
    s.base_prot_mem = 0; 
}  

/* 错误：clobber 不是 Base 的友元函数，所以不能访问 Base 的 protected/private 成员 */
void clobber(Base &b) { 
    //b.base_priv_mem = 0; 
    //b.base_prot_mem = 0;
}  

class D_DrivedPublicInherited : public DrivedPublicInherited {
public:
    void accessBaseDataTest();
};

/* 父类 public 继承了爷爷类后，各个域的访问属性不变
   所以孙类仍然可以访问爷爷类的 public/protected 成员
*/
void D_DrivedPublicInherited::accessBaseDataTest()
{
    //base_priv_mem = 0;  /* 错误！不能访问 */
    base_pub_mem = 0;     /* 保持了 public 访问属性 */
    base_prot_mem = 0;    /* 保持了 protected 访问属性 */

    //base_priv_fn();  /* 错误！不能访问 */
    base_pub_fn();
    base_prot_fn();
}

void outAccessPublicInherited() {
    DrivedPublicInherited d;
    d.base_pub_fn();  /* public 继承后 public 域的访问属性不变，所以外界能访问 */
    d.base_pub_mem = 0;
}


/* protected 继承测试 */
class DerivedProtectedInherited : protected Base {
public:
    void accessBaseDataTest();
};

/* protected 继承，子类只能访问基类中的 protected/public 成员 */
void DerivedProtectedInherited::accessBaseDataTest()
{
    //base_priv_mem = 0;  /* 错误！不能访问 */
    base_prot_mem = 0;
    base_pub_mem = 0;

    //base_priv_fn();  /* 错误！不能访问 */
    base_prot_fn();
    base_pub_fn();
}

/* 基类中的 public/protected 控制域被 protected 继承后，
   在子类中都变成了 protected，所以外界不能访问
*/
void outAccessProtectedInherited()
{
    DerivedProtectedInherited d;
    //d.base_pub_mem = 0;  /* 错误！不能访问 */
}

class D_DerivedProtectedInherited : public DerivedProtectedInherited {
public:
    void accessBaseDataTest();
};

void D_DerivedProtectedInherited::accessBaseDataTest()
{
    base_prot_mem = 0;  /* 保持了 protected 访问属性 */
    base_pub_mem = 0;   /* 在 DerivedProtectedInherited 类中已经被转变为 protected 访问属性 */
}


/* private 继承测试 */
class DerivedPrivateInherited : private Base {
public:
    void accessBaseDataTest();
};

/* private 继承，子类只能访问基类中的 protected/public 成员 */
void DerivedPrivateInherited::accessBaseDataTest()
{
    //base_priv_mem = 0;  /* 错误！不能访问 */
    base_prot_mem = 0;
    base_pub_mem = 0;
    
    //base_priv_fn();  /* 错误！不能访问 */
    base_prot_fn();
    base_pub_fn();
}

class D_DerivedPrivateInherited : public DerivedPrivateInherited {
public:
    void accessBaseDataTest();
};

/* private 继承后，基类中的 public/protected 域被强制转换为 private 访问属性,
   所以不能被外界访问，也不能对于孙类访问，因为爷爷类中的 public/protected 域在父类
   中已经被转变为 private 访问属性 */
void D_DerivedPrivateInherited::accessBaseDataTest()
{
    // base_prot_mem = 0;  /* 错误！不能访问 */
    // base_pub_mem = 0;   /* 错误！不能访问 */
}
void outAccessPrivateInherited() {
    DerivedPrivateInherited d;
    //d.base_pub_fn();  /* 错误！不能访问 */
}

/* 总结：
   - 对于子类，不管是那种继承（public/protected/private），子类都可以访问基类中的
     public/protected 成员，但是就是不能访问基类的 private 成员
   - 对于外界，只能访问类的 public 成员
   - public 继承后，基类中的 public/protected/private 对应的域访问属性不变
   - protected 继承后，基类中的 public 域被强制转为 protected 访问属性
   - private 继承后，基类中的 public/protected 域被强制转换为 private 访问属性
 */

/* private/protected 继承的子类，其对象不能向下转换为基类，
   因为如果允许的话，则子类对象中的基类 public 部分就变得允许给外界使用了，这就
   违背了 private 继承将基类的 public 转变为 private，protected 继承将基类的 public
   转变为 protected 的初衷了。 */
class Base_2 {
public:
    Base_2() = default;
    ~Base_2() = default;
    void say_hi() {
        std::cout << "hi" << std::endl;
    }
};

class Derived : private Base_2 {};

void test_private_drived()
{
    // Base_2 *b = new Derived();  // 编译报错
}

int main()
{
    DrivedPublicInherited d;

    Base* pp = new DrivedPublicInherited;
    pp->base_pub_fn();

    DrivedPublicInherited dp;
    Base b = dp;

    return 0;
}

