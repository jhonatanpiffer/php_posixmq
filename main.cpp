#include <phpcpp.h>

#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define QUEUE_NAME      "/php_influxdb"
#define HARD_MSGMAX     65535
#define HARD_MSGSIZEMAX 131071
#define MSG_STOP    "exit"

static mqd_t mq;

Php::Value posixmq_open(Php::Parameters &params) {

    std::string queue_name = params[0];
    int return_code = 0; 

    struct mq_attr attr;

    /* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = HARD_MSGMAX;
    attr.mq_msgsize = HARD_MSGSIZEMAX;
    attr.mq_curmsgs = 0;

    /* create the message queue */
    mq = mq_open(queue_name.c_str(), O_CREAT | O_WRONLY , 0644, &attr); 

    if (mq ==  -1) {
        return_code = -1;
    }
    else {
        return_code = 0;
    }

    return  return_code;
}

Php::Value posixmq_send_message(Php::Parameters &params) {

    std::string message = params[0];
    int return_code = 0; 

    return_code = mq_send(mq, message.c_str(),
                 message.size(), 0);
    // done
    return return_code;
}

    /**
     *  Switch to C context, because the Zend-engine calls the get_module() method
     *  in C context, and not in C++ context
     */
extern "C" {

    /**
     *  When a PHP extension starts up, the Zend engine calls the get_module()
     *  function to find out which functions and classes are offered by the 
     *  extension
     *
     *  @return void*   pointer to memory address holding the extension information
     */
    PHPCPP_EXPORT void *get_module() {
        // create an instance of the Php::Extension class
        static Php::Extension extension("posixmq", "1.0");

        extension.add<posixmq_open>("posixmq_open");
        extension.add<posixmq_send_message>("posixmq_send_message");

        //extension.add("posixmq_send_message", posixmq_send_message, { 
        //    Php::ByVal("return_code", Php::Type::String)
        //});
        //extension.add("posixmq_open", posixmq_open, {
        //    Php::ByVal("return_code", Php::Type::String)
        //});

        // return the extension
        return extension;
    }
}
