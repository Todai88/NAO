#include <boost\shared_ptr.hpp>
#include <alcommon\almodule.h>
#include <alcommon\albroker.h>
#include <alproxies\alvideodeviceproxy.h>
#include <alvision\alimage.h>
#include <opencv2\core\core.hpp>
#include <alproxies\almemoryproxy.h>
#include <althread\almutex.h>
#include <alcommon\alproxy.h>

class VidModule : public AL::ALModule
{
public:
	VidModule(boost::shared_ptr<AL::ALBroker> broker, const std::string& name);
	virtual ~VidModule();
	virtual void init();
	void stopVid();
	void onRightBumperPressed();
private:
	AL::ALProxy mProx;
	boost::shared_ptr<AL::ALMutex> fCallbackMutex;
	AL::ALMemoryProxy fMemoryProxy;
	float fState;
	AL::ALVideoDeviceProxy camProxy;	
	static AL::ALVideoDeviceProxy * camPtr;
	static void * startVid(void * pArg);
};