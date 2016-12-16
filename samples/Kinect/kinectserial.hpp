#ifndef __KINECT_SERIAL__HPP__
#define __KINECT_SERIAL__HPP__

#include <QtCore/QString>

#ifdef WIN32_BUILD
#include <usb.h>
#endif
#ifdef UNIX_BUILD
#include <libusb-1.0/libusb.h>
#endif

QString getKinectSerial(int index = 0, ushort vid = 0x45e, ushort pid = 0x02ae)
{
#ifdef WIN32_BUILD
    struct usb_bus *bus;
    struct usb_device *dev;
    usb_init();
    usb_find_busses();
    usb_find_devices();
    int idx = -1;
    for (bus = usb_busses; bus; bus = bus->next) {
        for (dev = bus->devices; dev; dev = dev->next) {
            if ((dev->descriptor.idVendor == vid) && (dev->descriptor.idProduct == pid))
                ++idx;
            if (idx != index)
                continue;
            usb_dev_handle *udev = usb_open(dev);
            if (udev) {
                std::string sn;
                if (dev->descriptor.iSerialNumber) {
                    char buff[256];
                    int len = usb_get_string_simple(udev, dev->descriptor.iSerialNumber, buff, sizeof(buff));
                    if (len > 0)
                        sn = std::string(buff, buff + len);
                }
                usb_close(udev);
                return QString::fromStdString(sn);
            }
        }
    }
#endif
#ifdef UNIX_BUILD
    libusb_device **devs;
    int idx = -1;
    int r = libusb_init(NULL);
    if (r < 0)
            return QString();
    ssize_t cnt = libusb_get_device_list(NULL, &devs);
    if (cnt < 0)
            return QString();
    libusb_device *dev;
    int i = 0;
    while ((dev = devs[i++]) != 0) {
            struct libusb_device_descriptor desc;
            int r = libusb_get_device_descriptor(dev, &desc);
            if (r < 0)
                return QString();
            if ((desc.idVendor == vid) && (desc.idProduct == pid))
                ++idx;
            if (idx == index) {
                struct libusb_device_handle *handle = 0;
                int err = libusb_open(dev, &handle);
                if (err != 0)
                    return QString();
                unsigned char buff[256];
                int len = libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, buff, sizeof(buff));
                libusb_close(handle);
                libusb_free_device_list(devs, 1);
                return QString::fromStdString(std::string(buff, buff + len));
            }
    }
    libusb_free_device_list(devs, 1);
    libusb_exit(0);
#endif
    return QString();
}

#endif // __KINECT_SERIAL__HPP__
