#ifndef UIDGENERATOR_H_
#define UIDGENERATOR_H_

#include <limits.h>

class UidGenerator {

private:
	static UidGenerator* m_instance;
	unsigned long m_val;

	UidGenerator() {
		m_val = 0;
	}

public:

	static UidGenerator* getInstance() {
		if (m_instance == 0)
			m_instance = new UidGenerator();

		return m_instance;
	}

	unsigned long getId() {
		return m_val++;
	}
};

#endif /* UIDGENERATOR_H_ */
