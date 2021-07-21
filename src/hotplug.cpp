
/*
 * build by lishuwei  
 * date:2021-07-22
*/

#include "hotplug.h"
#include  "common.h"

void  HotPlug::init()
{
    dev_handle=NULL;
    usb_dev=NULL;
    ctx=NULL;

    //临时日志文件，后面封装
    PropertyConfigurator::configure("../log4cxx.properties");
    LoggerPtr logger_demo(Logger::getLogger("demo"));
    logger_demo_ptr=logger_demo;
}

void  HotPlug::stop()
{
    ;
}

void  HotPlug::start()
{
    pthread_create(&usb_listener_thread, NULL,usb_listener_method, (void*)this);
    pthread_join(usb_listener_thread,NULL);
}

 int HotPlug::hotplug_callback(
    struct libusb_context *ctx,
    struct libusb_device *device,
    libusb_hotplug_event event,
    void *user_data)
{
    HotPlug  hogPlug=*(HotPlug*)user_data;

    //通过设备获取设备地址
    uint8_t deviceAddress = libusb_get_device_address(device);

    struct libusb_device_descriptor desc;
    memset(&desc,sizeof(struct libusb_device_descriptor),0);
    if(libusb_get_device_descriptor(device,&desc) != 0)
    {  
        LOG4CXX_INFO(hogPlug.logger_demo_ptr,"invoke libusb_get_device_descriptor fail...");
    }else{
        char temp[128]={0};
        snprintf(temp,sizeof(temp),"vid:0x%04x pid:0x%04x", desc.idVendor, desc.idProduct);
        LOG4CXX_INFO(hogPlug.logger_demo_ptr,temp);
    }

    if(event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED)
    {
        LOG4CXX_INFO(hogPlug.logger_demo_ptr,"device connected");
    } else if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT) 
    {
        LOG4CXX_INFO(hogPlug.logger_demo_ptr,"device disconnect");
    }
    return 0;
}

 void* HotPlug::usb_listener_method(void *data){
 
      HotPlug  hogPlug=*(HotPlug*)data;
      libusb_context *context;
      libusb_hotplug_callback_handle handle;

      int ret = libusb_init(&context);

       if (!libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG))
       {
           LOG4CXX_INFO(hogPlug.logger_demo_ptr,"this version's libusb doesn't support hotplug");
       }else
       {
           LOG4CXX_INFO(hogPlug.logger_demo_ptr,"this version's libusb can support hotplug");

           int vendor_id = LIBUSB_HOTPLUG_MATCH_ANY; 
           int product_id = LIBUSB_HOTPLUG_MATCH_ANY;
           int device_class = LIBUSB_HOTPLUG_MATCH_ANY;


           int result_Register = libusb_hotplug_register_callback(context, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED| LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT, 1, vendor_id, product_id, device_class, hotplug_callback,(void*)&hogPlug, &handle);
           if (result_Register != LIBUSB_SUCCESS)
           {
                LOG4CXX_INFO(hogPlug.logger_demo_ptr,"resigter hotplug_callback failed");
           } else 
           {
                LOG4CXX_INFO(hogPlug.logger_demo_ptr,"resigter hotplug_callback successfully");

              while(1)
              {
                     //设置允许处理事件的超时时间
                     libusb_handle_events(context);
              }
           }
        }
} 
