# Log--HTML Format
# Instructions
# Using methods in other classes
 Logger& logger = Logger::getInstance();
 
 logger.init("logfile.html");
 std::string temp = "初始化成功";
 int tempnum = 10;
 logger.log(Logging::LOG_INFO, "This is an informational message. %s", temp.c_str());
 logger.log(Logging::LOG_WARNING, "This is a warning message. %d", tempnum);
 logger.log(Logging::LOG_ERROR, "This is an error message.");
 logger.close();

 # Record results
 ![image](https://github.com/eric-guo132/Log/assets/89255266/0c265d0f-2b57-4879-989b-37fd1588db7e)
