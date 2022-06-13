#include "Object.h"


void Object::incrementReferenceCounter() {
    referenceCounter++;
}

void Object::decrementReferenceCounter() {
    referenceCounter--;
}

void Object::erase() {
}

void Object::destructIfNotReferenced() {
    if (!referenceCounter) erase();
}

void Object::decrementAndDestroy() {
    if (!(--referenceCounter)) erase();
}
