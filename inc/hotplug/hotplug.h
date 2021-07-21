#ifndef  __HOTPLUG__
#define  __HOTPLUG__

#include  "common.h"
using namespace log4cxx;
using namespace log4cxx::helpers;

class HotPlug
{
public:
    HotPlug()
    {
        init();
    }

    ~HotPlug()
    {
        stop();
    }

    void  init();
    void  stop();

    static int hotplug_callback(struct libusb_context *ctx,struct libusb_device *device,libusb_hotplug_event event,void *user_data);
    static void *usb_listener_method(void *data);
    void start();

private:
    libusb_context *ctx;
    struct libusb_device_handle *dev_handle ;
    struct libusb_device *usb_dev;
    LoggerPtr logger_demo_ptr;  
    pthread_t usb_listener_thread;
};
#endif