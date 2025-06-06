#pragma once
#include <string>
#include <ctime>

struct PersonID {
    int series;
    int number;
    bool operator==(const PersonID& other) const {
        return series == other.series && number == other.number;
    }
};

class Person {
protected:
    PersonID id;
    std::string firstName, middleName, lastName;
    std::tm birthDate;
public:
    Person() = default;

    Person(PersonID pid, std::string fn, std::string mn, std::string ln, std::tm dob)
        : id(pid), firstName(fn), middleName(mn), lastName(ln), birthDate(dob) {}

    std::string GetFullName() const {
        return firstName + " " + middleName + " " + lastName;
    }

    PersonID GetID() const { return id; }

    std::tm GetBirthDate() const { return birthDate; }
};

class Student : public Person {
public:
    using Person::Person;
};

class Teacher : public Person {
public:
    using Person::Person;
};
