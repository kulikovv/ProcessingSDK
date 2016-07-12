#include "callbackhook.h"
#include <utils/logs.h>

using namespace types;

CallbackHook::CallbackHook(QObject *parent) :
    QObject(parent)
{
    INFO("Callback created")
}
CallbackHook::~CallbackHook()
{
    INFO("Callback destroyed")
}
