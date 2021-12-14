/* 
 * File:   Person.cpp
 * 
 * Copyright (C) 2016 raodm@miamiOH.edu
 */

#include "Person.h"
#include <string>
#include <iomanip>

Person::Person(int id, unsigned short age, const std::string& name) :
    id(id), age(age), name(name) {
    // Nothing else to do.
}

Person::~Person() {
    // Nothing to be done here.
}

std::istream&
operator>>(std::istream& is, Person& p) {
    return (is >> p.id >> p.age >> std::quoted(p.name));
}

std::ostream&
operator<<(std::ostream& os, const Person& p) {
    return (os << p.id << " " << p.age << " " << std::quoted(p.name));
}

bool
Person::operator<(const Person& other) const {
    return (this->age < other.age);
}
