#include <boost\shared_ptr.hpp>
#include <alcommon\almodule.h>
#include <alcommon\albroker.h>
#include <alproxies\almotionproxy.h>
#include <qi\log.hpp>


class Motionizr : public AL::ALModule 
{
public:
	Motionizr(boost::shared_ptr<AL::ALBroker> abroker, const std::string& aname);
	virtual ~Motionizr();
	virtual void init();
	void startMoving(const float &x, const float &theta);
	void stopMoving();
	void updateMovement();
	void moveHead(const float &yaw, const float &pitch);
	void resetHead();
	bool isVibrating();
	void setVibration(const bool &value);
	bool isHandMode();
	void moveLeftHand(const float &yaw, const float &roll);
	void moveRightHand(const float &yaw, const float &roll);
	void extendRight();
	void extendLeft();
	void contractLeft();
	void contractRight();
	void closeHand(const std::string& handName);
	int switchMode();
private:
	AL::ALMotionProxy mProxy;
	bool vibrate;
	bool handMode;
};