/*
 * @Author: gongluck 
 * @Date: 2020-11-27 21:31:03 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-27 21:44:29
 */

// g++ log2.cpp -llog4cpp -lpthread

#include "log4cpp/Category.hh"
#include "log4cpp/PropertyConfigurator.hh"

int main()
{
    try
    {
        log4cpp::PropertyConfigurator::configure("./log2.conf");
    }
    catch (log4cpp::ConfigureFailure &f)
    {
        std::cout << "Configure Problem " << f.what() << std::endl;
        return -1;
    }

    // 实例化category对象
    log4cpp::Category &root = log4cpp::Category::getRoot();

    log4cpp::Category &category1 = log4cpp::Category::getInstance(std::string("category1"));
    log4cpp::Category &category3 = log4cpp::Category::getInstance(std::string("category1.category2"));
   
    category1.info("This is some info");
    category1.alert("A warning");

    category3.debug("This debug message will fail to write");
    category3.alert("All hands abandon ship");
    category3.critStream() << "This will show up << as " << 1 << " critical message";
    category3 << log4cpp::Priority::ERROR<< "And this will be an error";
    category3.log(log4cpp::Priority::WARN, "This will be a logged warning");
    
    log4cpp::Category::shutdown();
    return 0;
}