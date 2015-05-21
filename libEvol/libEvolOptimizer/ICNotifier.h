#ifndef ICNOTIFIER_H
#define ICNOTIFIER_H

//! Basic class interface for notifiers
/** Interface class
	Provides methods for notifier class methods
	This class is designed to present messages from the optimizer class
 */
class ICNotifier {
  public:

	/** Default destructor */
	//virtual ~ICNotifier(){}

	/** Notifications shown before optimization start
		@params _name message from optimizer
	 */
	virtual void onBeforeStart(const char* _name)=0;

	/** Notifications shown after optimization start
	@params _name message from optimizer after optimization was stopped
	 */
	virtual void onAfterStop(const char* _name)=0;

	/** Notifications shown after error detection
	@params _name error description
	 */
	virtual void onError(const char* _error)=0;

	/** Notifications shown after generation evaluation
	@params _g generation number
	 */
	virtual void onGenerationDone(unsigned int _g)=0;

};

#endif
