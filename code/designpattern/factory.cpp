#include <iostream>

using namespace std;

class ExportFileProduct
{
public:
    ExportFileProduct() {}
    virtual ~ExportFileProduct() {}

    virtual bool Export(string data) = 0;
};

// 保存成文件
class ExportTextProduct : public ExportFileProduct
{
public:
    ExportTextProduct() {}
    virtual ~ExportTextProduct() {}
    virtual bool Export(string data)
    {
        cout << "导出数据:[" << data << "]保存成文本的方式" << endl;
        return true;
    }
};

class ExportDBProduct : public ExportFileProduct
{
public:
    ExportDBProduct() {}
    virtual ~ExportDBProduct() {}
    virtual bool Export(string data)
    {
        cout << "导出数据:[" << data << "]保存数据库的方式" << endl;
        return true;
    }
};

class ExportFactory
{
public:
    ExportFactory() {}
    virtual ~ExportFactory() {}
    /**
     * @brief Export
     * @param type 导出的类型
     * @param data 具体的数据
     * @return
     */
    virtual bool Export(int type, string data)
    {
        ExportFileProduct *product = nullptr;
        product = factoryMethod(type);

        bool ret = false;
        if (product)
        {
            ret = product->Export(data);
            delete product;
        }
        else
        {
            cout << "没有对应的导出方式";
        }
        return ret;
    }

protected:
    virtual ExportFileProduct *factoryMethod(int type)
    {
        ExportFileProduct *product = nullptr;
        if (1 == type)
        {
            product = new ExportTextProduct();
        }
        else if (2 == type)
        {
            product = new ExportDBProduct();
        }
        return product;
    }
};

// 加一种新的导出方式:
// (1)修改原来的工厂方法
// (2)继承工厂方法去拓展
class ExportXMLProduct : public ExportFileProduct
{
public:
    ExportXMLProduct() {}
    virtual ~ExportXMLProduct() {}
    virtual bool Export(string data)
    {
        cout << "导出数据:[" << data << "]保存XML的方式" << endl;
        return true;
    }
};

class ExportPortobufferProduct : public ExportFileProduct
{
public:
    ExportPortobufferProduct() {}
    virtual ~ExportPortobufferProduct() {}
    virtual bool Export(string data)
    {
        cout << "导出数据:[" << data << "]保存Portobuffer的方式" << endl;
        return true;
    }
};

class ExportFactory2 : public ExportFactory
{
public:
    ExportFactory2() {}
    virtual ~ExportFactory2() {}

protected:
    virtual ExportFileProduct *factoryMethod(int type)
    {
        ExportFileProduct *product = nullptr;
        if (3 == type)
        {
            product = new ExportXMLProduct();
        }
        else if (4 == type)
        {
            product = new ExportPortobufferProduct();
        }
        else
        {
            product = ExportFactory::factoryMethod(type);
        }
        return product;
    }
};

int main()
{
    cout << "ExportFactory" << endl;
    ExportFactory *factory = new ExportFactory();

    factory->Export(1, "上课人数");
    factory->Export(2, "上课人数");
    factory->Export(3, "上课人数");

    cout << "\nExportFactory2" << endl;
    ExportFactory *factory2 = new ExportFactory2();

    factory2->Export(1, "上课人数");
    factory2->Export(2, "上课人数");
    factory2->Export(3, "上课人数");
    factory2->Export(4, "上课人数");

    delete factory;
    delete factory2;
    return 0;
}
