/*
 * @Author: gongluck 
 * @Date: 2020-11-27 21:12:58 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-27 21:30:54
 */

// g++ log1.cpp -llog4cpp -lpthread

#include "log4cpp/Category.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/BasicLayout.hh"

int main()
{
    // 实例化一个layout对象
    log4cpp::Layout *layout = new log4cpp::BasicLayout();
    // 初始化一个appender对象
    log4cpp::Appender *appender = new log4cpp::FileAppender("FileAppender","./log1.log");
    log4cpp::Appender *osappender = new log4cpp::OstreamAppender("OstreamAppender",&std::cout);
    // 把layout对象附着在appender对象上，一个layout格式样式对应一个appender
    appender->setLayout(layout);
    // 实例化一个category对象
    log4cpp::Category &warn_log = log4cpp::Category::getInstance("gongluck"); // 单例工厂
    // 设置additivity为false，替换已有的appender，不继承父类appender
    warn_log.setAdditivity(false);
    // 把appender对象附到category上
    warn_log.setAppender(appender);
    warn_log.addAppender(osappender);
    // 设置category的优先级，低于此优先级的日志不被记录
    warn_log.setPriority(log4cpp::Priority::INFO);
    // 记录一些日志
    warn_log.info("Program info which cannot be wirten");
    warn_log.debug("This debug message will fail to write");
    warn_log.alert("Alert info");
    // 其他记录日志方式
    warn_log.log(log4cpp::Priority::WARN, "This will be a logged warning");

    log4cpp::Priority::PriorityLevel priority = log4cpp::Priority::DEBUG;
    warn_log.log(priority, "Importance depends on context");
    warn_log.critStream() << "This will show up << as " << 1 << " critical message";
    // clean up and flush all appenders
    log4cpp::Category::shutdown();
    return 0;
}