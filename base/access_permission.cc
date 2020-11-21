#include <iostream>

class Base {
public:
    void pub_fn() {};
    int pub_mem;
protected:
    void prot_fn() {};
    int prot_mem;
private:
    void priv_fn() {};
    int priv_men;
};

class DrivedPublicInherited : public Base {
    friend void clobber(DrivedPublicInherited&);
    friend void clobber(Base&);
public:
    void accessBaseDataTest();
    void accessBaseFunTest();
protected:
    int prot_mem_PublicInherited;
private:
    int priv_mem_PublicInherited;
};

/* public 继承，子类只能访问基类中的 protected/public 成员 */
void DrivedPublicInherited::accessBaseDataTest()
{
    //pri_mem = 0;  /* 错误！不能访问 */
    prot_mem = 0;
    pub_mem = 0;
}
void DrivedPublicInherited::accessBaseFunTest()
{
    //priv_fn();  /* 错误！不能访问 */
    prot_fn();
    pub_fn();
}

void accessProtectedMemberTest()
{
    DrivedPublicInherited d;

    /* 错误！不能访问。外界不能访问类的 protected 成员 */
    //d.prot_mem_PublicInherited = 0;

    /* 错误！不能访问。子类内部能访问基类的 protected 成员。但外界不能通过
       子类对象去访问基类的 protected 成员 */
    //d.prot_mem = 0;  
}

/* 类对象中所有访问等级的成员，对于它的友元函数来说都是可以访问的 */
void clobber(DrivedPublicInherited &s) { 
    s.priv_mem_PublicInherited = 0;
    s.prot_mem = 0; 
}  

/* 错误：clobber 不是 Base 的友元函数，所以不能访问 Base 的 protected/private 成员 */
void clobber(Base &b) { 
    //b.priv_mem = 0; 
    //b.prot_mem = 0;
}  

class D_DrivedPublicInherited : public DrivedPublicInherited {
public:
    void accessGrandpaProtectedMemberTest();
};

/* 子类可以访问爷爷类的 protected 成员 */
void D_DrivedPublicInherited::accessGrandpaProtectedMemberTest()
{
    prot_mem = 0;
}

class DerivedPrivateInherited : protected Base {
public:
    void accessBaseDataTest();
    void accessBaseFunTest();
};

/* private 继承，子类只能访问基类中的 protected/public 成员 */
void DerivedPrivateInherited::accessBaseDataTest()
{
   // pri_mem = 0;  /* 错误！不能访问 */
    prot_mem = 0;
    pub_mem = 0;
}
void DerivedPrivateInherited::accessBaseFunTest()
{
   // priv_fn();  /* 错误！不能访问 */
    prot_fn();
    pub_fn();
}

class D_DerivedPrivateInherited : public DerivedPrivateInherited {
public:
    void accessBaseDataTest();
};

/* 虽然 DerivedPrivateInherited 私继承了 Base，但 
   D_DerivedPrivateInherited 仍能访问 Base 的 protected/public 成员，
   所以私继承限制的是类的使用者，而不是类继承体系中的子类 */
void D_DerivedPrivateInherited::accessBaseDataTest()
{
    prot_mem = 0;
    pub_mem = 0;
}

class DerivedProtectedInherited : protected Base {

};

/* 对类的使用者来说，基类中的 public/protected 控制域，protected 继承后，在子类中都变成了 protected */
void accessBaseMemberTest()
{
    DerivedProtectedInherited d;
    //d.pub_mem = 0;  /* 错误！不能访问 */
    //d.prot_mem = 0;  /* 错误！不能访问 */
}

int main()
{
    DrivedPublicInherited d;

    /* 子类对象不能向下转型为基类（不管是以引用、指针、还是非引用非指针的形式） */
    //Base* pp = new DerivedPri;
    //DerivedPri dp;
    //Base b = dp;

    return 0;
}

