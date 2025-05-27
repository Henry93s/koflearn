#include "koflearnPlatform.h"

unsigned long long KoflearnPlatform::getPrimaryKey() const {
	return this->primaryKey;
}
void KoflearnPlatform::setPrimaryKey(unsigned long long primaryKey) {
	this->primaryKey = primaryKey;
}