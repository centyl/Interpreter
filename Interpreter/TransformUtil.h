#pragma once

#include "Object.h"
#include "Node.h"
#include "Tree.h"

class Node;
class Block;

namespace Util {
    namespace Transform {

        std::vector<ObjectOrError>::iterator findError(std::vector<ObjectOrError>::iterator begin,
                                                       std::vector<ObjectOrError>::iterator end);

        std::vector<ObjectOrError> toUnreferenced(std::vector<Object*>::const_iterator begin,
                                                  std::vector<Object*>::const_iterator end);

        std::vector<Object*> toObject(std::vector<ObjectOrError>::const_iterator begin,
                                      std::vector<ObjectOrError>::const_iterator end);


        std::vector<ObjectOrError> toObjectOrError(std::vector<Operand>::const_iterator begin,
                                                   std::vector<Operand>::const_iterator end);

        ObjectsOrError toObjects(std::vector<Operand>::const_iterator beg,
                                 std::vector<Operand>::const_iterator las);

        ObjectsOrError toUnreferencedObjects(std::vector<Operand>::const_iterator it_begin, std::vector<Operand>::const_iterator it_end);

        Object* toObject(const std::string_view s);

        ObjectOrError toObject(const Operand& operand);

        ObjectsOrError toUnreferencedObjects(std::vector<Object*>::const_iterator it_begin,
                                             std::vector<Object*>::const_iterator it_end);

        OptionalError<std::vector<std::string_view>> toArgumentNames(std::vector<Operand>::const_iterator it_begin,
                                                                     std::vector<Operand>::const_iterator it_end);

    }
}